// Apply names from a dtk-style symbols.txt ("name = .section:0xADDR; // type:function size:0x..").
// Generic dtk names (fn_, lbl_, jumptable_, @, gap_) are skipped so Ghidra's own names stay.
// Usage (headless): -postScript ApplySymbols.java <symbols.txt>
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.SourceType;

public class ApplySymbols extends GhidraScript {
	private static final Pattern LINE =
		Pattern.compile("^(\\S+) = \\S*:0x([0-9A-Fa-f]{8});.*?type:(\\w+)");

	@Override
	public void run() throws Exception {
		Path file = Path.of(getScriptArgs()[0]);
		int funcs = 0, labels = 0;
		for (String line : Files.readAllLines(file)) {
			Matcher m = LINE.matcher(line);
			if (!m.find()) {
				continue;
			}
			String name = m.group(1);
			if (name.startsWith("fn_") || name.startsWith("lbl_") || name.startsWith("jumptable_") ||
				name.startsWith("@") || name.startsWith("gap_")) {
				continue;
			}
			Address addr = toAddr(Long.parseLong(m.group(2), 16));
			if (m.group(3).equals("function")) {
				Function f = getFunctionAt(addr);
				if (f == null) {
					f = createFunction(addr, name);
				}
				if (f != null) {
					f.setName(name, SourceType.IMPORTED);
					funcs++;
				}
			}
			else {
				createLabel(addr, name, true, SourceType.IMPORTED);
				labels++;
			}
		}
		println("Applied " + funcs + " function names and " + labels + " labels from " + file);
	}
}
