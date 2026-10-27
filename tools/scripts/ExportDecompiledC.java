// Decompile every function in the current program and write it to one C file.
// Usage (headless): -postScript ExportDecompiledC.java <output.c>
import java.io.PrintWriter;

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;

public class ExportDecompiledC extends GhidraScript {
	@Override
	public void run() throws Exception {
		String out = getScriptArgs().length > 0 ? getScriptArgs()[0] : "decompiled.c";
		DecompInterface decomp = new DecompInterface();
		decomp.openProgram(currentProgram);
		int ok = 0, failed = 0;
		try (PrintWriter w = new PrintWriter(out, "UTF-8")) {
			w.println("// Decompiled from " + currentProgram.getName() + " by Ghidra");
			for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
				if (monitor.isCancelled()) {
					break;
				}
				DecompileResults res = decomp.decompileFunction(f, 60, monitor);
				w.println();
				w.println("// " + f.getEntryPoint() + "  " + f.getName());
				if (res.decompileCompleted()) {
					w.println(res.getDecompiledFunction().getC());
					ok++;
				}
				else {
					w.println("// decompile failed: " + res.getErrorMessage());
					failed++;
				}
			}
		}
		decomp.dispose();
		println("Exported " + ok + " functions (" + failed + " failed) to " + out);
	}
}
