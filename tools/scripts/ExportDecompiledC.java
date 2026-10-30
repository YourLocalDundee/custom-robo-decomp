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
		// CRLF line ends on every OS (the file is checksummed: tools/checksums.ps1 / .sh); the decompiler's own text
		// uses the OS line separator, so it is normalised too
		try (PrintWriter w = new PrintWriter(out, "UTF-8")) {
			w.print("// Decompiled from " + currentProgram.getName() + " by Ghidra\r\n");
			for (Function f : currentProgram.getFunctionManager().getFunctions(true)) {
				if (monitor.isCancelled()) {
					break;
				}
				DecompileResults res = decomp.decompileFunction(f, 60, monitor);
				w.print("\r\n");
				w.print("// " + f.getEntryPoint() + "  " + f.getName() + "\r\n");
				if (res.decompileCompleted()) {
					w.print(res.getDecompiledFunction().getC().replace("\r\n", "\n").replace("\n", "\r\n") + "\r\n");
					ok++;
				}
				else {
					w.print("// decompile failed: " + res.getErrorMessage() + "\r\n");
					failed++;
				}
			}
		}
		decomp.dispose();
		println("Exported " + ok + " functions (" + failed + " failed) to " + out);
	}
}
