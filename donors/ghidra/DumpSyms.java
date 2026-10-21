// Dump the data symbols and the function list of the program.
//
// Ghidra headless script:
//   ghidra_headless.cmd <proj> <name> -process \
//     -postScript DumpSyms.java <mode> [arg]
//
// modes:
//   list          every function name + address
//   data <name>   the bytes of a data symbol, as a C array
//   xrefs <name>  the callers and callees of a symbol
//
// @category VXTux

import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.Listing;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolIterator;
import ghidra.program.model.symbol.SymbolTable;
import java.io.File;
import java.io.PrintWriter;

public class DumpSyms extends GhidraScript {
    private static final String OUT_DIR = "E:\\vxtux_build_repo\\donors\\ghidra_out";

    @Override
    public void run() throws Exception {
        String[] a = getScriptArgs();
        if (a.length == 0) {
            println("usage: DumpSyms list | data <name> | xrefs <name>");
            return;
        }
        new File(OUT_DIR).mkdirs();
        String mode = a[0];
        SymbolTable st = currentProgram.getSymbolTable();

        if ("list".equals(mode)) {
            File f = new File(OUT_DIR, currentProgram.getName() + ".funcs.txt");
            try (PrintWriter pw = new PrintWriter(f, "UTF-8")) {
                FunctionIterator it = currentProgram.getFunctionManager().getFunctions(true);
                int n = 0;
                while (it.hasNext()) {
                    Function fn = it.next();
                    pw.println(fn.getEntryPoint() + " " + fn.getName() + " "
                              + fn.getSymbol().getSymbolType());
                    n++;
                }
                println("wrote " + n + " functions to " + f.getAbsolutePath());
            }
            // and the data symbols
            File d = new File(OUT_DIR, currentProgram.getName() + ".data.txt");
            try (PrintWriter pw = new PrintWriter(d, "UTF-8")) {
                SymbolIterator it = st.getAllSymbols(false);
                int n = 0;
                while (it.hasNext()) {
                    Symbol s = it.next();
                    if (s.getSymbolType() == ghidra.program.model.symbol.SymbolType.LABEL
                        || s.getSymbolType() == ghidra.program.model.symbol.SymbolType.GLOBAL) {
                        pw.println(s.getAddress() + " " + s.getName() + " " + s.getSymbolType());
                        n++;
                    }
                }
                println("wrote " + n + " data symbols to " + d.getAbsolutePath());
            }
        }
        else if ("data".equals(mode) && a.length > 1) {
            dumpData(st, a[1]);
        }
        else if ("xrefs".equals(mode) && a.length > 1) {
            dumpXrefs(st, a[1]);
        }
    }

    private void dumpData(SymbolTable st, String name) {
        SymbolIterator it = st.getSymbolIterator(name, true);
        while (it.hasNext()) {
            Symbol s = it.next();
            Address ad = s.getAddress();
            println("=== " + name + " @ " + ad + " type=" + s.getSymbolType());
            try {
                Memory mem = currentProgram.getMemory();
                byte[] buf = new byte[128];
                int n = mem.getBytes(ad, buf);
                StringBuilder sb = new StringBuilder();
                for (int i = 0; i < n; i++) {
                    if (i % 8 == 0) {
                        sb.append("\n  ");
                    }
                    sb.append(String.format("%02x ", buf[i]));
                }
                println(sb.toString());
                // is there a defined data item there?
                Listing l = currentProgram.getListing();
                var obj = l.getDataAt(ad);
                if (obj != null) {
                    println("  data item: " + obj.getClass().getSimpleName()
                            + " len=" + obj.getLength());
                }
            }
            catch (Exception e) {
                println("  error: " + e.getMessage());
            }
        }
    }

    private void dumpXrefs(SymbolTable st, String name) {
        SymbolIterator it = st.getSymbolIterator(name, true);
        while (it.hasNext()) {
            Symbol s = it.next();
            Address ad = s.getAddress();
            println("=== " + name + " @ " + ad);
            ReferenceIterator ri = currentProgram.getReferenceManager()
                                    .getReferencesTo(ad);
            while (ri.hasNext()) {
                Reference r = ri.next();
                println("  ref from " + r.getFromAddress() + " "
                       + r.getReferenceType());
            }
            // who calls it
            Function f = currentProgram.getFunctionManager().getFunctionAt(ad);
            if (f != null) {
                for (Function c : f.getCallingFunctions(monitor)) {
                    println("  called by " + c.getName() + " @ " + c.getEntryPoint());
                }
                for (Function c : f.getCalledFunctions(monitor)) {
                    println("  calls " + c.getName() + " @ " + c.getEntryPoint());
                }
            }
        }
    }
}
