// Dump the section map and, for a named data symbol, the bytes with
// the relocations resolved (the module's own relocations are already
// applied by the loader for absolute targets that live in the same
// image, so a raw dump is enough for most tables).
//
// Ghidra headless script:
//   ghidra_headless.cmd <proj> <name> -process \
//     -scriptPath <dir> -postScript DumpSecs.java [sym ...]
//
// @category VXTux

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolIterator;
import java.io.File;
import java.io.PrintWriter;

public class DumpSecs extends GhidraScript {
    @Override
    public void run() throws Exception {
        File out = new File("E:\\vxtux_build_repo\\donors\\ghidra_out");
        out.mkdirs();
        File f = new File(out, currentProgram.getName() + ".secs.txt");
        try (PrintWriter pw = new PrintWriter(f, "UTF-8")) {
            pw.println("### image base = " + currentProgram.getImageBase());
            for (MemoryBlock b : currentProgram.getMemory().getBlocks()) {
                pw.println(String.format("BLOCK %-28s %s - %s  size=0x%x",
                                         b.getName(), b.getStart(), b.getEnd(),
                                         b.getSize()));
            }
            pw.println();
            pw.println("### data symbols");
            SymbolIterator it = currentProgram.getSymbolTable()
                                    .getAllSymbols(false);
            while (it.hasNext()) {
                Symbol s = it.next();
                String t = s.getSymbolType().toString();
                if (t.equals("LABEL") || t.equals("GLOBAL")) {
                    pw.println(s.getAddress() + " " + t + " " + s.getName()
                               + " size=" + s.getSize());
                }
            }
        }
        System.out.println("wrote " + f.getAbsolutePath());

        // dump the requested symbols' bytes
        String[] a = getScriptArgs();
        for (String name : a) {
            SymbolIterator si = currentProgram.getSymbolTable()
                                    .getSymbolIterator(name, true);
            while (si.hasNext()) {
                Symbol s = si.next();
                Address ad = s.getAddress();
                System.out.println("=== " + name + " @ " + ad
                                   + " size=" + s.getSize());
                try {
                    Memory mem = currentProgram.getMemory();
                    int len = (int) s.getSize();
                    if (len <= 0 || len > 4096) {
                        len = 256;
                    }
                    byte[] buf = new byte[len];
                    int n = mem.getBytes(ad, buf);
                    for (int i = 0; i < n; i += 8) {
                        StringBuilder sb = new StringBuilder();
                        for (int j = 0; j < 8 && i + j < n; j++) {
                            sb.append(String.format("%02x", buf[i + j]));
                        }
                        long v = 0;
                        for (int j = 7; j >= 0 && i + j < n; j--) {
                            v = (v << 8) | (buf[i + j] & 0xff);
                        }
                        System.out.println(String.format("  +0x%04x  %s  0x%016x",
                                                         i, sb, v));
                    }
                }
                catch (Exception e) {
                    System.out.println("  error: " + e.getMessage());
                }
            }
        }
    }
}
