import { Terminal as XTerminal } from "@xterm/xterm";
import { useEffect, useRef } from "react";
import socket from "../socket";
import "@xterm/xterm/css/xterm.css";

// ==========================================
// Interactive Web Terminal
// Pipes stdout and stdin between Socket.io backend server and Xterm.js UI
// ==========================================
export const TerminalA = () => {
  const terminalRef = useRef();
  const isRendered = useRef(false);

  useEffect(() => {
    if (isRendered.current) return;
    isRendered.current = true;

    // ==========================================
    // XTerminal Initialization & Theming
    // Sets up dimensions and a custom dark palette
    // ==========================================
    const term = new XTerminal({
      rows: 20,
      cursorBlink: true,
      theme: {
        background: "#0a121d",
        foreground: "#c9e5ff",
        cursor: "#2ed0b8",
        black: "#0b1220",
        brightBlack: "#41566f",
        red: "#f87171",
        brightRed: "#fb7185",
        green: "#34d399",
        brightGreen: "#6ee7b7",
        yellow: "#fbbf24",
        brightYellow: "#fcd34d",
        blue: "#60a5fa",
        brightBlue: "#93c5fd",
        magenta: "#c084fc",
        brightMagenta: "#d8b4fe",
        cyan: "#22d3ee",
        brightCyan: "#67e8f9",
        white: "#dbeafe",
        brightWhite: "#eff6ff",
      },
    // ==========================================
    // Socket Handlers
    // Binds input events emitting to backend, and writes incoming server logs
    // ==========================================
    });

    term.open(terminalRef.current);

    term.onData((data) => {
      socket.emit("terminal:write", data);
    });

    function onTerminalData(data) {
      term.write(data);
    // ==========================================
    // Teardown Hooks
    // Cleans up event listeners and disposes instance to prevent memory leaks
    // ==========================================
    }

    socket.on("terminal:data", onTerminalData);

    return () => {
      socket.off("terminal:data", onTerminalData);
      term.dispose();
      isRendered.current = false;
    };
  }, []);

  return (
    <div
      ref={terminalRef}
      id="terminal"
      className="h-full w-full bg-[#0a121d] text-white font-mono"
    />
  );
};

// export default TerminalA;
