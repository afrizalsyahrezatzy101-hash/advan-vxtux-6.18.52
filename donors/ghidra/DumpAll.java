// Dump every function's decompilation into one file.
//
// Ghidra headless script:
//   ghidra_headless.cmd <proj> <name> -import <file.ko> \
//     -postScript DumpAll.java
//
// @category VXTux

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import java.io.File;
import java.io.PrintWriter;

public class DumpAll extends GhidraScript {
    @Override
    public void run() throws Exception {
        File out = new File("E:\\vxtux_build_repo\\donors\\ghidra_out");
        out.mkdirs();
        File f = new File(out, currentProgram.getName() + ".all.c");
        System.out.println("writing " + f.getAbsolutePath());

        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);

        try (PrintWriter pw = new PrintWriter(f, "UTF-8")) {
            FunctionIterator it = currentProgram.getFunctionManager()
                                      .getFunctions(true);
            int n = 0;
            while (it.hasNext()) {
                Function fn = it.next();
                pw.println("====================" + fn.getName()
                           + "@" + fn.getEntryPoint() + "====================");
                pw.println("// signature: " + fn.getSignature());
                DecompileResults r = di.decompileFunction(fn, 90, monitor);
                if (r.decompileCompleted()) {
                    pw.println(r.getDecompiledFunction().getC());
                } else {
                    pw.println("// decompile FAILED: " + r.getErrorMessage());
                }
                pw.println();
                n++;
                if ((n % 25) == 0) {
                    System.out.println("  " + n + " functions");
                }
            }
            System.out.println("wrote " + n + " functions");
        }
        di.dispose();
    }
}
