// Create a function at every function start decomp-toolkit found that Ghidra did not (names stay FUN_).
// Run before ApplySymbols so the new functions get any saved names too.
// Usage (headless): -postScript CreateDtkFunctions.java <dtk symbols.txt>
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;

public class CreateDtkFunctions extends GhidraScript {
	private static final Pattern LINE =
		Pattern.compile("^\\S+ = \\.(?:init|text):0x([0-9A-Fa-f]{8});.*?type:function");

	@Override
	public void run() throws Exception {
		Path file = Path.of(getScriptArgs()[0]);
		int created = 0, failed = 0;
		for (String line : Files.readAllLines(file)) {
			Matcher m = LINE.matcher(line);
			if (!m.find()) {
				continue;
			}
			Address addr = toAddr(Long.parseLong(m.group(1), 16));
			if (getFunctionAt(addr) != null) {
				continue;
			}
			if (getInstructionAt(addr) == null) {
				disassemble(addr);
			}
			if (createFunction(addr, null) != null) {
				created++;
			}
			else {
				failed++;
			}
		}
		println("Created " + created + " functions from dtk starts (" + failed + " failed) from " + file);
	}
}
