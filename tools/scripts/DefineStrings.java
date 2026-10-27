// Define null-terminated ASCII strings (>= 4 chars) in non-executable initialized memory,
// so pointer tables and code that reference them show names like s_chara_bin_801b6a66.
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.mem.MemoryBlock;

public class DefineStrings extends GhidraScript {
	private static boolean printable(int b) {
		return (b >= 0x20 && b < 0x7f) || b == '\n' || b == '\t' || b == '\r';
	}

	@Override
	public void run() throws Exception {
		int made = 0;
		for (MemoryBlock block : currentProgram.getMemory().getBlocks()) {
			if (!block.isInitialized() || block.isExecute()) {
				continue;
			}
			byte[] data = new byte[(int) block.getSize()];
			block.getBytes(block.getStart(), data);
			int i = 0;
			while (i < data.length) {
				int start = i;
				while (i < data.length && printable(data[i] & 0xff)) {
					i++;
				}
				if (i < data.length && data[i] == 0 && i - start >= 4) {
					Address a = block.getStart().add(start);
					if (getDataAt(a) == null && getInstructionAt(a) == null) {
						try {
							clearListing(a, a.add(i - start));
							createAsciiString(a);
							made++;
						}
						catch (Exception e) {
							// overlapping defined data; leave it
						}
					}
				}
				i++;
			}
		}
		println("Defined " + made + " strings");
	}
}
