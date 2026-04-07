#ifndef INTERMEDIATE_CODE_GEN_H
#define INTERMEDIATE_CODE_GEN_H

#include "Parser.h"
#include <vector>
#include <string>
#include <memory>

// Represents a single Intermediate Representation (IR) instruction.
struct IRInstruction {
    std::string opcode;             // Operation code (e.g., ADD, JUMP)
    std::vector<std::string> operands; // List of operands
    int line;                       // Source code line number for debugging

    // Initializes an IR instruction with its opcode, operands, and line number.
    IRInstruction(const std::string& op, const std::vector<std::string>& ops, int ln)
        : opcode(op), operands(ops), line(ln) {}
};

// Class responsible for constructing Intermediate Representation from the AST.
class IntermediateCodeGen {
public:
    // Constructor
    IntermediateCodeGen();

    // Generates IR from the provided AST Program.
    void generate(const Program* program);

    // Retrieves the generated IR instructions.
    const std::vector<IRInstruction>& getIR() const;

private:
    // Sequence of generated IR instructions.
    std::vector<IRInstruction> ir;

    // Converts a relational operator text into an IR opcode.
    std::string relOpToOpcode(const std::string& op);

    // Recursively processes AST statements into IR.
    void genStatement(const Statement* stmt);

    // Evaluates an AST expression and stores the location instruction result.
    void genExpression(const Expression* expr, std::string& result);

    // Keeps track of temporary variables generated during IR pass.
    int tempVarCounter;

    // Creates and returns a new unique temporary variable name.
    std::string newTemp();
};

#endif 
