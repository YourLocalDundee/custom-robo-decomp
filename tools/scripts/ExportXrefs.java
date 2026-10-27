// Export functions, references and defined strings as TSV files for tools/build_index.py.
// Usage (headless): -postScript ExportXrefs.java <output dir>
//   functions.tsv  entry, name, size (bytes in body)
//   refs.tsv       from_function, from_addr, to_addr, ref type, target symbol (calls, reads, writes, pointers)
//   strings.tsv    addr, label, value
import java.io.File;
import java.io.PrintWriter;

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Data;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.Symbol;

public class ExportXrefs extends GhidraScript {
	private static String clean(String s) {
		return s.replace('\t', ' ').replace('\n', ' ').replace('\r', ' ');
	}

	@Override
	public void run() throws Exception {
		File dir = new File(getScriptArgs()[0]);
		dir.mkdirs();
		int nf = 0, nr = 0, ns = 0;
		try (PrintWriter fw = new PrintWriter(new File(dir, "functions.tsv"), "UTF-8");
				PrintWriter rw = new PrintWriter(new File(dir, "refs.tsv"), "UTF-8")) {
			for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
				fw.println(f.getEntryPoint() + "\t" + f.getName() + "\t" + f.getBody().getNumAddresses());
				nf++;
				for (Instruction in : currentProgram.getListing().getInstructions(f.getBody(), true)) {
					for (Reference r : in.getReferencesFrom()) {
						Address to = r.getToAddress();
						if (!to.isMemoryAddress()) {
							continue;
						}
						Symbol s = getSymbolAt(to);
						rw.println(f.getEntryPoint() + "\t" + in.getAddress() + "\t" + to + "\t" +
							r.getReferenceType() + "\t" + (s == null ? "" : clean(s.getName())));
						nr++;
					}
				}
			}
		}
		try (PrintWriter sw = new PrintWriter(new File(dir, "strings.tsv"), "UTF-8")) {
			for (Data d : currentProgram.getListing().getDefinedData(true)) {
				if (!d.hasStringValue()) {
					continue;
				}
				Symbol s = getSymbolAt(d.getAddress());
				sw.println(d.getAddress() + "\t" + (s == null ? "" : clean(s.getName())) + "\t" +
					clean(String.valueOf(d.getValue())));
				ns++;
			}
		}
		println("Exported " + nf + " functions, " + nr + " references, " + ns + " strings to " + dir);
	}
}
