import { Terminal as XTerminal } from "@xterm/xterm";
import { useEffect, useRef } from "react";
import socket from "../socket";

import "@xterm/xterm/css/xterm.css";

export const TerminalA = () => {
  const terminalRef = useRef();
  const isRendered = useRef(false);

  useEffect(() => {
    if (isRendered.current) return;
    isRendered.current = true;

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
    });

    term.open(terminalRef.current);

    term.onData((data) => {
      socket.emit("terminal:write", data);
    });

    function onTerminalData(data) {
      term.write(data);
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
