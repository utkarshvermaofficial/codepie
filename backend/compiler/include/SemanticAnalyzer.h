#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "Parser.h"
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

// Basic primitive variables supported by language bindings.
enum class VarType { NUMBER, STRING, BOOLEAN, UNKNOWN };


// Keeps contextual history corresponding to a declared memory reference.
struct VariableInfo {
    VarType type;      // Resolved logical variable scope type
    int lineDeclared;  // Identifies original statement insertion point
};

// Responsible for type-checking and structural correctness logic.
class SemanticAnalyzer {
public:
    // Sets up analysis dictionary mapping.
    SemanticAnalyzer();

    // Invokes top-level structural traversal verifying the AST program.
    void analyze(const Program* program);

    // Returns any type-mismatches or scope errors caught during evaluation.
    const std::vector<std::string>& getErrors() const;

private:
    std::unordered_map<std::string, VariableInfo> symbolTable; // Persistent bindings table
    std::vector<std::string> errors;                           // Log tracking fault sequences

    // Recursively analyzes logical compatibility inside nested statements/blocks.
    void analyzeStatement(const Statement* stmt);

    // Deduences type logic returning its structural validity format.
    void analyzeExpression(const Expression* expr, VarType& outType);

    // Records a fresh variable against current execution limits.
    void declareVariable(const std::string& name, VarType type, int line);

    // Queries pre-existence of variable labels across maps.
    bool isVariableDeclared(const std::string& name) const;

    // Locates internal stored bindings for designated textual mappings.
    VarType getVariableType(const std::string& name) const;
};

// Converts the enum representation into a printable descriptive string.
std::string varTypeToString(VarType type);

#endif
