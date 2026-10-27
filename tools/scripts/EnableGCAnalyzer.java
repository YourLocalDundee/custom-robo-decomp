// Pre-script: turn on the GameCube loader's SDA/GQR analyzer, which stays off by default
// because its format-name check no longer matches the loader's name.
import ghidra.app.script.GhidraScript;

public class EnableGCAnalyzer extends GhidraScript {
	@Override
	public void run() throws Exception {
		setAnalysisOption(currentProgram, "(GameCube/Wii) Program Analyzer", "true");
		println("Enabled (GameCube/Wii) Program Analyzer");
	}
}
