// Parse the HSD library headers from the Melee decomp (cleaned by tools/clean_headers.py) and our own
// symbols/cr_types.h into this program's data types; check the parse against the headers' ASSERT_SIZE lines
// and /* +XX */ offset comments; apply the prototypes (return type, parameter names and types) to verified
// library functions and to our cr_ functions; create the typed globals of symbols/cr_globals.txt.
// Usage (headless): -postScript ParseHeaders.java <clean headers dir> <verify_report.txt> <report out>
//                                                  <cr_types.h> <cr_globals.txt>
//   Library functions get a prototype only when verify_report.txt says VERIFIED or CLOSE: same name and
//   (near) same code size as Melee. Struct sizes are checked against the game by tools/check_class_sizes.py.
import java.io.File;
import java.io.PrintWriter;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import ghidra.app.cmd.function.ApplyFunctionSignatureCmd;
import ghidra.app.script.GhidraScript;
import ghidra.app.util.cparser.C.CParserUtils;
import ghidra.program.model.data.*;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.SourceType;

public class ParseHeaders extends GhidraScript {
	private static final Pattern ASSERT = Pattern.compile("ASSERT_SIZE\\(\\s*(?:struct\\s+)?(\\w+)\\s*,\\s*(0x[0-9A-Fa-f]+|\\d+)\\s*\\)");
	private static final Pattern VERDICT = Pattern.compile("^(VERIFIED|CLOSE)\\s+[0-9A-F]{8}\\s+(\\S+)");

	@Override
	public void run() throws Exception {
		String melee = getScriptArgs()[0];
		Path verify = Path.of(getScriptArgs()[1]);
		File reportFile = new File(getScriptArgs()[2]);
		String crTypes = getScriptArgs()[3];
		Path crGlobals = Path.of(getScriptArgs()[4]);
		File baselib = new File(melee, "src/sysdolphin/baselib");

		List<String> headers = new ArrayList<>();
		Map<String, Long> asserted = new TreeMap<>();
		Map<String, Map<String, Integer>> commented = new TreeMap<>();  // struct -> field -> /* +XX */ offset
		for (File h : Objects.requireNonNull(baselib.listFiles((d, n) -> n.endsWith(".h")))) {
			headers.add(h.getAbsolutePath());
			String text = Files.readString(h.toPath());
			Matcher m = ASSERT.matcher(text);
			while (m.find()) {
				asserted.put(m.group(1), Long.decode(m.group(2)));
			}
			collectOffsetComments(text, commented);
		}
		Collections.sort(headers);
		headers.add(crTypes);  // last: may use any SDK / HSD type
		String crText = Files.readString(Path.of(crTypes));  // its ASSERT_SIZE comments and offsets are checked too
		Matcher cm = ASSERT.matcher(crText);
		while (cm.find()) asserted.put(cm.group(1), Long.decode(cm.group(2)));
		collectOffsetComments(crText, commented);
		String[] includes = {
			melee + "/src", melee + "/src/MSL", melee + "/src/Runtime", melee + "/libs/doldecomp/include",
			melee + "/libs/dolphin/include", melee + "/libs/dolphin/include/libc"
		};
		// no __MWERKS__: inline asm and compiler pragmas drop out; M2CTX selects the plain-C paths
		String[] args = { "-DM2CTX", "-D__attribute__(x)=", "-D__declspec(x)=", "-DATTRIBUTE_ALIGN(x)=",
			"-D__INTPTR_TYPE__=int", "-D__UINTPTR_TYPE__=unsigned int", "-D__SIZE_TYPE__=unsigned long",
			"-D__PTRDIFF_TYPE__=int", "-D__WCHAR_TYPE__=unsigned short" };

		DataTypeManager dtm = currentProgram.getDataTypeManager();
		CParserUtils.CParseResults res =
			CParserUtils.parseHeaderFiles(null, headers.toArray(new String[0]), includes, args, dtm, monitor);

		// 1. check parsed struct sizes against ASSERT_SIZE
		int sizeOk = 0, sizeBad = 0, sizeMissing = 0;
		List<String> lines = new ArrayList<>();
		Map<String, DataType> byName = new HashMap<>();
		Iterator<DataType> all = dtm.getAllDataTypes();
		while (all.hasNext()) {
			DataType dt = all.next();
			byName.putIfAbsent(dt.getName(), dt);
		}
		for (Map.Entry<String, Long> e : asserted.entrySet()) {
			DataType dt = byName.get(e.getKey());
			if (dt == null) { sizeMissing++; lines.add("MISSING  " + e.getKey()); continue; }
			if (dt.getLength() == e.getValue()) sizeOk++;
			else {
				sizeBad++;
				lines.add("SIZE     " + e.getKey() + " parsed=0x" + Integer.toHexString(dt.getLength()) + " header=0x" + Long.toHexString(e.getValue()));
				if (dt instanceof TypeDef td) dt = td.getBaseDataType();
				if (dt instanceof Composite comp) {  // field offsets, to compare with the header's /* +XX */ comments
					for (DataTypeComponent c : comp.getDefinedComponents()) {
						lines.add(String.format("           +%X %s %s (0x%X)", c.getOffset(), c.getDataType().getName(), c.getFieldName(), c.getLength()));
					}
				}
			}
		}

		// 1b. check parsed field offsets against the headers' /* +XX */ comments
		int offOk = 0, offBad = 0, offNoStruct = 0;
		for (Map.Entry<String, Map<String, Integer>> e : commented.entrySet()) {
			DataType dt = byName.get(e.getKey());
			if (dt instanceof TypeDef td) dt = td.getBaseDataType();
			if (!(dt instanceof Composite comp)) { offNoStruct++; continue; }
			Map<String, Integer> have = new HashMap<>();
			for (DataTypeComponent c : comp.getDefinedComponents()) have.put(c.getFieldName(), c.getOffset());
			for (Map.Entry<String, Integer> f : e.getValue().entrySet()) {
				Integer got = have.get(f.getKey());
				if (got == null) continue;  // field inside a nested union/struct: not compared
				if (got.intValue() == f.getValue()) offOk++;
				else { offBad++; lines.add(String.format("OFFSET   %s.%s parsed=+%X header=+%X", e.getKey(), f.getKey(), got, f.getValue())); }
			}
		}

		// 2. prototypes for verified library functions and our cr_ functions
		Set<String> allowed = new HashSet<>();
		for (String l : Files.readAllLines(verify)) {
			Matcher m = VERDICT.matcher(l);
			if (m.find()) allowed.add(m.group(2));
		}
		int applied = 0, failed = 0;
		for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
			if (!allowed.contains(f.getName()) && !f.getName().startsWith("cr_")) continue;
			DataType dt = byName.get(f.getName());
			if (!(dt instanceof FunctionDefinition fd)) continue;
			ApplyFunctionSignatureCmd cmd = new ApplyFunctionSignatureCmd(f.getEntryPoint(), fd, SourceType.IMPORTED);
			if (cmd.applyTo(currentProgram, monitor)) {
				applied++;
				// header prototypes carry no calling convention; use the PowerPC EABI default
				f.setCallingConvention(currentProgram.getCompilerSpec().getDefaultCallingConvention().getName());
			}
			else { failed++; lines.add("PROTO    " + f.getName() + ": " + cmd.getStatusMsg()); }
		}

		// 3. typed globals: "cr_g_Name = .section:0xADDR; // type:object datatype:Type[*][N] // why: ..."
		int globals = 0, globalsBad = 0;
		Pattern gl = Pattern.compile("^(\\S+) = \\S*:0x([0-9A-Fa-f]{8});.*?datatype:(\\w+)(\\**)(?:\\[(\\d+)\\])?");
		for (String l : Files.readAllLines(crGlobals)) {
			Matcher m = gl.matcher(l);
			if (!m.find()) continue;
			DataType dt = byName.get(m.group(3));
			if (dt == null) { globalsBad++; lines.add("GLOBAL   " + m.group(1) + ": unknown type " + m.group(3)); continue; }
			for (int i = 0; i < m.group(4).length(); i++) dt = new PointerDataType(dt, dtm);
			if (m.group(5) != null) dt = new ArrayDataType(dt, Integer.parseInt(m.group(5)), dt.getLength(), dtm);
			ghidra.program.model.address.Address a = toAddr(Long.parseLong(m.group(2), 16));
			clearListing(a, a.add(dt.getLength() - 1));
			createData(a, dt);
			createLabel(a, m.group(1), true, SourceType.IMPORTED);
			globals++;
		}

		String summary = "Parsed " + headers.size() + " headers; ASSERT_SIZE ok " + sizeOk + ", wrong " + sizeBad +
			", not parsed " + sizeMissing + "; field offsets ok " + offOk + ", wrong " + offBad +
			" (structs not found: " + offNoStruct + "); prototypes applied " + applied + ", failed " + failed +
			"; globals typed " + globals + ", failed " + globalsBad;
		try (PrintWriter w = new PrintWriter(reportFile, "UTF-8")) {
			w.println(summary);
			for (String l : lines) w.println(l);
			// sizes of every parsed HSD struct, for tools/check_class_sizes.py
			for (Map.Entry<String, DataType> e : new TreeMap<>(byName).entrySet()) {
				DataType dt = e.getValue() instanceof TypeDef td ? td.getBaseDataType() : e.getValue();
				if (e.getKey().startsWith("HSD_") && dt instanceof Composite) {
					w.println("STRUCTSIZE " + e.getKey() + " 0x" + Integer.toHexString(dt.getLength()));
				}
			}
			w.println();
			w.println("---- C parser messages");
			w.println(res.cParseMessages());
			w.println("---- preprocessor messages");
			w.println(res.cppParseMessages());
		}
		println(summary);
	}

	private static final Pattern STRUCT_OPEN = Pattern.compile("struct\\s+(\\w+)\\s*\\{");
	private static final Pattern FIELD = Pattern.compile(
		"/\\*\\s*\\+([0-9A-Fa-f]+)\\s*\\*/[^;{]*?(\\w+)\\s*(?:\\[[^\\]]*\\]\\s*)*(?:DAT_\\w+\\([^;]*\\))?\\s*;");

	// top-level fields of each "struct Name { ... }" that carry an offset comment
	private static void collectOffsetComments(String text, Map<String, Map<String, Integer>> out) {
		Matcher s = STRUCT_OPEN.matcher(text);
		while (s.find()) {
			int i = s.end(), depth = 1, lineStart = i;
			Map<String, Integer> fields = out.computeIfAbsent(s.group(1), k -> new LinkedHashMap<>());
			while (i < text.length() && depth > 0) {
				char c = text.charAt(i);
				if (c == '{') depth++;
				else if (c == '}') depth--;
				else if (c == '\n') {
					if (depth == 1) {
						Matcher f = FIELD.matcher(text.substring(lineStart, i));
						if (f.find()) fields.putIfAbsent(f.group(2), Integer.parseInt(f.group(1), 16));
					}
					lineStart = i + 1;
				}
				i++;
			}
		}
	}
}
