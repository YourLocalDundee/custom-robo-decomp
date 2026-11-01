// Name the GC/2.6 scheduler functions of mwcceppc.exe and export their decompiled C (docs/sched_model.md).
// Usage (headless, on the analysis copy tools/sched/bin/mwcceppc.exe):
//   -postScript SchedExport.java <output.c> <addr:name> ...
// Every named function and every function it calls (one level) is decompiled into <output.c>; the output is the
// compiler's code and stays local (tools/sched/ghidra_export/ is gitignored).
import java.io.PrintWriter;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.Map;
import java.util.Set;

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.SourceType;

public class SchedExport extends GhidraScript {
	@Override
	public void run() throws Exception {
		String[] args = getScriptArgs();
		String out = args[0];
		Map<Address, String> names = new LinkedHashMap<>();
		for (int i = 1; i < args.length; i++) {
			String[] kv = args[i].split(":", 2);
			names.put(toAddr(Long.parseLong(kv[0].replace("0x", ""), 16)), kv[1]);
		}
		Set<Function> todo = new LinkedHashSet<>();
		for (Map.Entry<Address, String> e : names.entrySet()) {
			Function f = getFunctionAt(e.getKey());
			if (f == null) {
				disassemble(e.getKey());
				f = createFunction(e.getKey(), e.getValue());
			}
			if (f == null) {
				println("no function at " + e.getKey());
				continue;
			}
			f.setName(e.getValue(), SourceType.USER_DEFINED);
			todo.add(f);
		}
		Set<Function> all = new LinkedHashSet<>(todo);
		for (Function f : todo) {
			all.addAll(f.getCalledFunctions(monitor));
		}
		DecompInterface d = new DecompInterface();
		d.openProgram(currentProgram);
		try (PrintWriter w = new PrintWriter(out, "UTF-8")) {
			for (Function f : all) {
				DecompileResults r = d.decompileFunction(f, 120, monitor);
				w.print("\r\n// " + f.getEntryPoint() + "  " + f.getName() + "\r\n");
				if (r.decompileCompleted()) {
					w.print(r.getDecompiledFunction().getC().replace("\r\n", "\n").replace("\n", "\r\n"));
				}
				else {
					w.print("// decompile failed: " + r.getErrorMessage() + "\r\n");
				}
			}
		}
		d.dispose();
		println("SchedExport: " + all.size() + " functions to " + out);
	}
}
