import { useEffect, useState } from "react";
import { CodeEditor } from "./component/CodeEditor";
import { CompilationTabs } from "./component/CompilationTab";
// import { TerminaA}
import { TerminalA } from "./component/Terminal2";
import { LoaderCircle, Play, Save, WandSparkles } from "lucide-react";
import { useToast } from "./hooks/use-toast";
import socket from "./socket";

const Index = () => {
  const [code, setCode] = useState("");
  const [compilationData, setCompilationData] = useState(null);
  // const [terminalOutput, setTerminalOutput] = useState("");
  const [isCompiling, setIsCompiling] = useState(false);
  const { toast } = useToast();

  useEffect(() => {
    const savedCode = localStorage.getItem("customLangCode");
    if (savedCode) setCode(savedCode);
    else
      setCode(
        `let a be 10
output a`
      );
  }, []);

  const runCompiledCCode = () => {
    socket.emit("run:c");
  };

  const handleCompile = async () => {
    setIsCompiling(true);
    // setTerminalOutput("Compiling...\n");

    try {
      const response = await fetch("http://localhost:9900/compile", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({ code }),
      });

      if (!response.ok) {
        const errorData = await response.json();
        // setTerminalOutput(
        //   `Compilation failed: ${errorData.error || "Unknown error"}`
        // );
        toast({
          title: "Compilation Failed",
          description: errorData.error || "Unknown error occurred.",
          variant: "destructive",
        });
        setIsCompiling(false);
        return;
      }

      const compilationData = await response.json();
      setCompilationData(compilationData);

      // setTerminalOutput(
      //   "Compilation successful!\nExecuting program...\n" +
      //     (compilationData.output || "No output from program.")
      // );

      toast({
        title: "Compilation Complete",
        description: "Your code has been successfully compiled and executed.",
      });
    } catch (error) {
      // setTerminalOutput("Compilation failed due to network or server error.");
      toast({
        title: "Compilation Failed",
        description: error.message || "An error occurred.",
        variant: "destructive",
      });
    } finally {
      setIsCompiling(false);
    }
  };

  const handleSave = () => {
    localStorage.setItem("customLangCode", code);
    toast({
      title: "Code Saved",
      description: "Your code has been saved locally.",
    });
  };

  return (
    <div className="min-h-screen p-3 md:p-5 lg:p-6">
      <div className="mx-auto max-w-[1600px]">
        <header className="fade-up mb-4 rounded-2xl border border-[var(--surface-border)] bg-[var(--surface)] p-4 shadow-[0_18px_40px_rgba(0,0,0,0.34)] backdrop-blur-sm md:p-5">
          <div className="flex flex-col gap-4 md:flex-row md:items-center md:justify-between">
            <div>
              <h1 className="text-xl font-semibold tracking-tight text-[var(--text)] md:text-2xl">
                Codepie Studio
              </h1>
              <p className="text-sm text-[var(--muted)]">
                Edit, compile, inspect pipeline output, and execute generated C
                code.
              </p>
            </div>

            <div className="flex flex-wrap gap-2">
          <button
            onClick={handleCompile}
            disabled={isCompiling}
            className={`inline-flex items-center gap-2 rounded-lg px-3 py-2 text-sm font-medium text-white transition-colors ${
              isCompiling
                ? "cursor-not-allowed bg-emerald-800/70"
                : "bg-[var(--primary)] hover:bg-[var(--primary-strong)]"
            }`}
          >
            {isCompiling ? (
              <LoaderCircle size={16} className="animate-spin" />
            ) : (
              <WandSparkles size={16} />
            )}
            {isCompiling ? "Compiling..." : "Compile"}
          </button>

          <button
            onClick={runCompiledCCode}
            className="inline-flex items-center gap-2 rounded-lg bg-sky-600 px-3 py-2 text-sm font-medium text-white transition-colors hover:bg-sky-500"
          >
            <Play size={16} />
            Run C Code
          </button>

          <button
            onClick={handleSave}
            className="inline-flex items-center gap-2 rounded-lg border border-slate-500/60 px-3 py-2 text-sm font-medium text-slate-200 transition-colors hover:bg-slate-700/40"
          >
            <Save size={16} />
            Save
          </button>
        </div>
          </div>
        </header>

        <main className="flex min-h-[calc(100vh-170px)] flex-col gap-4 lg:flex-row">
          <section className="fade-up fade-delay-1 min-h-[420px] overflow-hidden rounded-2xl border border-[var(--surface-border)] bg-[var(--surface)] shadow-[0_16px_32px_rgba(0,0,0,0.26)] backdrop-blur-sm lg:w-1/2">
            <CodeEditor code={code} onChange={setCode} />
          </section>

          <section className="flex min-h-[420px] flex-col gap-4 lg:w-1/2">
            <div className="fade-up fade-delay-2 min-h-[260px] flex-1 overflow-hidden rounded-2xl border border-[var(--surface-border)] bg-[var(--surface)] shadow-[0_16px_32px_rgba(0,0,0,0.26)] backdrop-blur-sm">
              <CompilationTabs data={compilationData} />
            </div>
            <div className="fade-up fade-delay-3 h-64 overflow-hidden rounded-2xl border border-[var(--surface-border)] bg-[var(--surface)] shadow-[0_16px_32px_rgba(0,0,0,0.26)] backdrop-blur-sm">
              <TerminalA />
            </div>
          </section>
        </main>
      </div>
    </div>
  );
};

export default Index;
