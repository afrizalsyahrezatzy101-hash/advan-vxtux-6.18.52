// Dump the decompilation and the call graph of a function set.
//
// Ghidra headless script: run with
//   ghidra_headless.cmd <proj_dir> <proj> -import <file.ko> \
//     -postScript DumpFuncs2.java <symbol> [<symbol>...]
//
// Writes into <outdir>, which is the first argument, so several blobs
// can be dumped without clobbering each other.
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

public class DumpFuncs2 extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) {
            println("usage: DumpFuncs2 <outdir> <symbol> [<symbol>...]");
            return;
        }
        File out = new File(args[0]);
        out.mkdirs();

        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);

        for (int i = 1; i < args.length; i++) {
            String want = args[i];
            Function f = findFunction(want);
            if (f == null) {
                println("NOT FOUND: " + want);
                continue;
            }
            println("=== " + f.getName() + " @ " + f.getEntryPoint() + " ===");
            DecompileResults r = di.decompileFunction(f, 120, monitor);
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

        if (name.startsWith("0x") || name.startsWith("0X")) {
            try {
                long off = Long.parseLong(name.substring(2), 16);
                Address a = currentProgram.getAddressFactory()
                                   .getDefaultAddressSpace()
                                   .getAddress(off);
                Function f = fm.getFunctionAt(a);
                if (f == null) {
                    f = createFunction(a, null);
                    if (f != null)
                        println("  created function at " + a);
                }
                return f;
            }
            catch (Exception e) {
                println("  bad address: " + e.getMessage());
                return null;
            }
        }

        SymbolIterator it = st.getSymbolIterator(name, true);
        while (it.hasNext()) {
            Symbol s = it.next();
            Function f = fm.getFunctionAt(s.getAddress());
            if (f != null)
                return f;
        }
        SymbolIterator all = st.getAllSymbols(false);
        while (all.hasNext()) {
            Symbol s = all.next();
            if (!s.getName().equals(name))
                continue;
            Function f = fm.getFunctionAt(s.getAddress());
            if (f != null)
                return f;
        }
        return null;
    }
}
