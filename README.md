# **Compiler for Custom Programming Language**

## **Description**
This project is a Compiler for our own Programming language named "Codepie". The compiler is built using c++. It also includes a fully functional Graphical User Interface (GUI) built using Vite and React, allowing users to write code in Codepie and view tokens, errors, Intermediate Representation(IR), Optimized IR, Corresponding C code and the output in different sections all within the GUI environment. The Compiler also allows the Generated code to be executed directly from the interface.

## **Features**

- Custom Programming language: A new simple to use and understand language "Codepie".
- Lexical Analysis: Tokenization of the Codepie code.
- Syntax Analysis: Parsing of the Codepie code which detects syntax errors with line numbers.
- Semantic Analysis: Checks for semantic errors such as undefined variables, type mismatches etc.
- Intermediate Representation (IR): Shows Intermediate Representation of the Codepie code.
- Code Optimization: Optimizes the IR for better performance.
- Code Generation: Generates C code from the optimized IR.
- Execution Support: Executes the generated C code directly from the GUI interface.
- User Interface: A fully functional GUI built using Vite and React for interacting with the user.

## **Project Structure**

```
codepie/
├── backend/
│   ├── compiler/                  # C++ Compiler source code and executable
│   │   ├── include/               # Header files defining compiler components
│   │   │   ├── Lexer.h            # Tokenizer definitions for lexical analysis
│   │   │   ├── CodeGenerator.h    # Target C code generation logic
│   │   │   ├── IntermediateCodeGen.h # Logic for handling IR generation
│   │   │   ├── Optimizer.h        # Defines IR optimization routines
│   │   │   ├── Parser.h           # Syntax analysis definitions (AST building)
│   │   │   ├── SemanticAnalyzer.h # Semantic analysis & type checking logic
│   │   │   ├── UI.h               # Console and file output utilities
│   │   │   └── Utils.h            # Shared helper functions
│   │   ├── src/                   # Source files implementing compiler logic
│   │   │   ├── CodeGenerator.cpp  # Implements translation from IR to C code
│   │   │   ├── IntermediateCodeGen.cpp # Builds intermediate representation
│   │   │   ├── Lexer.cpp          # Converts source text stream into tokens
│   │   │   ├── main.cpp           # Compiler executable entry point
│   │   │   ├── Optimizer.cpp      # Optimizes generated IR for performance
│   │   │   ├── Parser.cpp         # Parses tokens into Abstract Syntax Tree
│   │   │   └── SemanticAnalyzer.cpp # Validates language semantics
│   │   ├── compiler               # The resulting compiled C++ executable
│   │   └── readme.md              # Compiler-specific documentation
│   ├── user/
│   │   └── code.c                 # Output directory for the generated C code
│   ├── server.js                  # Node.js/Socket.io backend bridging frontend and compiler
│   └── package.json               # Backend Node.js dependencies
├── frontend/                      # User Interface (Vite + React)
│   ├── public/                    # Static UI assets
│   ├── src/
│   │   ├── assets/                # App-specific images and icons
│   │   ├── components/            # Reusable React UI components
│   │   │   ├── CodeEditor.jsx     # Handles the main IDE code input interface
│   │   │   ├── CompilationTabs.jsx # Manages tabs mapping (Errors, IR, C Code, Output)
│   │   │   └── Terminal.jsx       # Read-only UI terminal showing program output
│   │   ├── hooks/
│   │   │   └── use-toast.jsx      # Custom notifications/toast hook
│   │   ├── App.jsx                # Main application entry component
│   │   ├── index.css              # Global styles (Tailwind configured)
│   │   ├── main.jsx               # React DOM rendering entry point
│   │   └── socket.js              # WebSocket client linking UI directly to backend
│   ├── eslint.config.js           # Code linting rules and format checks
│   ├── package.json               # Frontend dependencies and run scripts
│   ├── vite.config.js             # Vite bundler and dev server configuration
│   └── index.html                 # Main frontend DOM container
└── README.md                      # Global documentation (This file)
```

## **Getting Started**

### **Prerequisites**
Make sure the following are installed on your system:

- For Backend (C++ Compiler)
  - C++17 or higher
  - Generic compiler stack like g++ or clang

- For Server (Node.js Bridge)
  - Node.js (v18 or above recommended)
  - npm (Node Package Manager)

- For Frontend (Vite + React UI)
  - Node.js & npm (same as above)

- Modern Browser (e.g., Chrome, Edge, Firefox)

### **How To Run On Local**

1. **Clone the repository:**
   ```bash
   git clone https://github.com/utkarshvermaofficial/codepie.git
   cd codepie
   ```

2. **Compile the Local Compiler (Required if running for the first time):**
   ```bash
   cd backend/compiler
    # Example build command using clang++
    clang++ -std=c++17 -I./include src/*.cpp -o compiler
   cd ../..
   ```

3. **Start the Backend Node.js Server:**
   Open a terminal window and navigate to the backend directory.
   ```bash
   cd backend
   npm install
   npm run start  # or you can use: nodemon server.js
   ```

4. **Start the Frontend UI Server:**
   Open a *new* terminal window and navigate to the frontend directory.
   ```bash
   cd frontend
   npm install
   npm run dev
   ```

5. **Interact via GUI:**
   Open the browser tab displaying the local url shown by Vite (e.g. `http://localhost:5173`) and use the UI to write, view tokens/IR, and run the `codepie` language codes.

## **Language Specification**

This section defines the syntax and semantics of codepie supported by our compiler.

### **Keywords**
| Category                  | Keywords                                                      |
| ------------------------- | ------------------------------------------------------------- |
| **Variable Declaration**  | `let`, `be`                                                   |
| **Input/Output**          | `input`, `output`                                             |
| **Arithmetic Operations** | `add`, `subtract`, `multiply`, `divide`, `store`, `in`, `and` |
| **Control Flow**          | `if`, `then`, `else`, `otherwise`                             |
| **Looping**               | `repeat`, `from`, `to`, `jump`, `until`                       |


### **Datatypes**
Codepie supports type interference so you don't need to explicitly declare the type of a variable.

### **Statements Supported**
➤ Variable Declaration
```
    let x be 10
    let name be "Gautam"
```

➤ Input Statement
```
    input value
```

➤ Output Statement
```
    output x
    output "Hello, World"
```

➤ Arithmetic Operations
```
    add a and b store in result
    subtract x and y store in diff
    multiply p and q store in product
    divide total and count store in avg
```

#### **Control Flow**
➤ Conditional Statements
```
if x < y then
    output "x is less"
otherwise
    output "x is not less"
```

Supports:
- Simple if
- otherwise as else
- else if as else if chain with another if

➤ Loops
1. Counting Loop (like a for loop):
    ```
    repeat from i = 1 to 10 jump 1
        output i
    ```

2. Conditional Loop (like a while loop):
    ```
    repeat until x < 5
        output x
    ```

### **Expressions**
- Relational Operators: <, >, <=, >=, ==, !=
- Operands: Identifiers, Numbers, Strings

Example:
```
if a == b then ...
```

### **Error Handling**
- Unexpected tokens and syntax mismatches are reported with line numbers.
- Unterminated strings, missing keywords, and invalid expressions generate descriptive error messages

## **Sample Programs**

### **Sample 1**
    let a be "Hello"
    let b be 10
    let c be 10.5
    output a
    output b
    output c

### **Sample 2**
    let a be 10.5
    let b be 20.5
    add a and b store in sum
    subtract a and b store in diff
    multiply a and b store in c
    divide a and b store in d
    output sum
    output diff
    output c
    output d

### **Sample 3**
    let a be "Hello"
    let b be "World"
    add a and b store in sum
    output sum

### **Sample 4**
    let a be "Hello"
    let b be 20
    add a and b store in sum
    output sum

### **Sample 5**
    let a be 10
    let b be 5
    if a>b then
        output a
    otherwise
        output b

### **Sample 6**
    repeat from i=1 to 15 jump 3

## **Troubleshooting**

Here are some common issues you might run into and how to fix them:

- **Missing `compiler` file error when executing code via frontend:** 
  You haven't compiled the C++ backend logic yet. Go to `backend/compiler/` and run the g++ command shown in the Setup Instructions snippet. Ensure the compiled output file is specifically named `compiler` or `compiler.exe`. If permissions are denied, grant execution access using `chmod +x backend/compiler/compiler`.
  
- **Dependencies installation errors on npm install:** 
  There may be a node version conflict. Clear the node module caching by removing `node_modules` and `package-lock.json` completely from the failing directory (i.e., frontend or backend). Retrying `npm install` afterwards frequently fixes peer dependency failures.
  
- **Port 5173 / Port 3000 conflicts:**
  Ensure no rogue instances of the servers are already running on these default ports. Check utilizing `lsof -i :3000` or simply modify your node `server.js` listener / Vite configuration files respectively as an alternative.
  
- **UI displays disconnected/WebSocket errors:** 
  It generally happens if the backend application is off. Leave the frontend running and make sure your server `backend/server.js` was effectively booted in a side-by-side terminal outputting its "Server running" console log.
        output i

## **Technologies Used**
- C++ - Core compiler Logic
- Node.js - Server to bridge frontend and compiler
- React+Vite - Fast and mordern UI for code editing and output display
- HTML/CSS/JS – UI styling and interactions
- npm - Dependency management for server and frontend

## **Output Sections (in UI)**
- Tokens
- Errors
- Intermediate Representation
- Optimized IR
- Generated C Code
- Program Output

