// Dump the decompilation and the call graph of a function set.
//
// Ghidra headless script: run with
//   ghidra_headless.cmd <proj_dir> <proj> -import <file.ko> \
//     -postScript DumpFuncs.java <symbol> [<symbol>...]
//
// The symbol names are looked up in the symbol table, so a vendor
// symbol like mbox_start_send_tx_fifo can be passed directly.
//
// @category VXTux

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolIterator;
import ghidra.program.model.symbol.SymbolTable;
import java.io.File;
import java.io.PrintWriter;

public class DumpFuncs extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length == 0) {
            println("usage: DumpFuncs <symbol> [<symbol>...]");
            return;
        }
        File out = new File("E:\\vxtux_build_repo\\donors\\ghidra_out");
        out.mkdirs();

        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);

        for (String want : args) {
            Function f = findFunction(want);
            if (f == null) {
                println("NOT FOUND: " + want);
                continue;
            }
            println("=== " + f.getName() + " @ " + f.getEntryPoint() + " ===");
            DecompileResults r = di.decompileFunction(f, 60, monitor);
            if (r.decompileCompleted()) {
                File cf = new File(out, f.getName() + ".c");
                try (PrintWriter pw = new PrintWriter(cf, "UTF-8")) {
                    pw.println("// " + f.getName() + " @ " + f.getEntryPoint());
                    pw.println("// signature: " + f.getSignature());
                    pw.println("// calling convention: " + f.getCallingConventionName());
                    pw.println();
                    pw.println(r.getDecompiledFunction().getC());
                }
                println("  -> " + cf.getAbsolutePath());
            } else {
                println("  decompile FAILED: " + r.getErrorMessage());
            }
        }
        di.dispose();
    }

    private Function findFunction(String name) {
        SymbolTable st = currentProgram.getSymbolTable();
        FunctionManager fm = currentProgram.getFunctionManager();

        // a raw address: 0x104368
        if (name.startsWith("0x") || name.startsWith("0X")) {
            try {
                long off = Long.parseLong(name.substring(2), 16);
                Address a = currentProgram.getAddressFactory()
                                   .getDefaultAddressSpace()
                                   .getAddress(off);
                Function f = fm.getFunctionAt(a);
                if (f == null) {
                    // the .ko is stripped, Ghidra did not make a function
                    // here. Create one from the bytes at that address.
                    f = createFunction(a, null);
                    if (f != null) {
                        println("  created function at " + a);
                    }
                }
                return f;
            }
            catch (Exception e) {
                println("  bad address: " + e.getMessage());
                return null;
            }
        }

        // exact name, any namespace
        SymbolIterator it = st.getSymbolIterator(name, true);
        while (it.hasNext()) {
            Symbol s = it.next();
            Function f = fm.getFunctionAt(s.getAddress());
            if (f != null) {
                return f;
            }
        }
        // any symbol with that name
        SymbolIterator all = st.getAllSymbols(false);
        while (all.hasNext()) {
            Symbol s = all.next();
            if (!s.getName().equals(name)) {
                continue;
            }
            Function f = fm.getFunctionAt(s.getAddress());
            if (f != null) {
                return f;
            }
        }
        return null;
    }
}
