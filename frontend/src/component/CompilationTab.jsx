import { useState } from "react";

export const CompilationTabs = ({ data }) => {
  const [activeTab, setActiveTab] = useState("tokens");

  if (!data) {
    return (
      <div className="flex h-full items-center justify-center bg-slate-950/40 text-slate-400">
        <div className="text-center">
          <p className="mb-2 text-lg font-medium">No compilation data</p>
          <p className="text-sm">
            Click "Compile & Run" to see the compilation pipeline
          </p>
        </div>
      </div>
    );
  }

  const tabs = [
    { id: "tokens", label: "Tokens" },
    { id: "errors", label: "Errors" },
    { id: "ir", label: "IR" },
    { id: "optimized_ir", label: "Optimized IR" },
    { id: "c_code", label: "C Code" },
    { id: "output", label: "Output" },
  ];

  const errorsCount = data.errors?.filter((entry) => entry !== "No errors.").length ?? 0;

  return (
    <div className="flex h-full flex-col bg-slate-950/40">
      {/* Tabs List */}
      <div className="flex h-12 items-center gap-1 border-b border-slate-600/40 bg-slate-900/70 px-2">
        {tabs.map(({ id, label }) => (
          <button
            key={id}
            onClick={() => setActiveTab(id)}
            className={`rounded-md px-3 py-1.5 text-sm font-medium select-none transition-colors ${
              activeTab === id
                ? "bg-slate-200/15 text-slate-100"
                : "text-slate-400 hover:bg-slate-700/50 hover:text-slate-100"
            }`}
            type="button"
            role="tab"
            aria-selected={activeTab === id}
            aria-controls={`${id}-tab`}
            id={`${id}-tab-button`}
          >
            {label}
            {id === "errors" && errorsCount > 0 ? (
              <span className="ml-2 rounded bg-rose-500/20 px-1.5 py-0.5 text-xs text-rose-300">
                {errorsCount}
              </span>
            ) : null}
          </button>
        ))}
      </div>

      {/* Tabs Content */}
      <div className="flex-1 overflow-auto p-4">
        {activeTab === "tokens" && (
          <div className="space-y-2">
            {data.tokens.length > 0 ? (
              data.tokens.map((token, i) => (
                <div
                  key={i}
                  className="rounded-lg border border-slate-700/60 bg-slate-900/80 p-2 font-mono text-sm text-sky-300"
                >
                  {token}
                </div>
              ))
            ) : (
              <p className="text-slate-500">No tokens available.</p>
            )}
          </div>
        )}

        {activeTab === "errors" && (
          <div className="space-y-2">
            {errorsCount > 0 ? (
              data.errors.map((error, i) => (
                <div
                  key={i}
                  className="rounded-lg border border-rose-700/70 bg-rose-950/50 p-3 font-mono text-sm text-rose-200"
                >
                  {error}
                </div>
              ))
            ) : (
              <p className="text-emerald-300">No errors found.</p>
            )}
          </div>
        )}

        {activeTab === "ir" && (
          <pre className="overflow-x-auto whitespace-pre-wrap rounded-lg border border-slate-700/60 bg-slate-900/85 p-4 font-mono text-sm text-slate-200">
            {data.ir || "No IR available."}
          </pre>
        )}

        {activeTab === "optimized_ir" && (
          <pre className="overflow-x-auto whitespace-pre-wrap rounded-lg border border-slate-700/60 bg-slate-900/85 p-4 font-mono text-sm text-slate-200">
            {data.optimized_ir || "No optimized IR available."}
          </pre>
        )}

        {activeTab === "c_code" && (
          <pre className="overflow-x-auto whitespace-pre-wrap rounded-lg border border-slate-700/60 bg-slate-900/85 p-4 font-mono text-sm text-slate-200">
            {data.c_code || "No C code available."}
          </pre>
        )}

        {activeTab === "output" && (
          <pre className="overflow-x-auto whitespace-pre-wrap rounded-lg border border-emerald-700/40 bg-emerald-950/30 p-4 font-mono text-sm text-emerald-300">
            {data.output || "No output available."}
          </pre>
        )}
      </div>
    </div>
  );
};
