#include "SemanticAnalyzer.h"
#include <sstream>

SemanticAnalyzer::SemanticAnalyzer() {}

// ==========================================
// Main Analysis Entry Point
// Triggers AST traversal for semantic checks
// ==========================================
void SemanticAnalyzer::analyze(const Program* program) {
    symbolTable.clear();
    errors.clear();
    for (const auto& stmt : program->statements) {
        analyzeStatement(stmt.get());
    }
}

// ==========================================
// Statement Analysis
// Validates variable declarations, scopes, assignments, and control flows
// ==========================================
void SemanticAnalyzer::analyzeStatement(const Statement* stmt) {
    if (!stmt) return;
    // Exit immediately if statement is null

    if (auto varDecl = dynamic_cast<const VarDecl*>(stmt)) {
        // Check if the statement is a variable declaration
        VarType exprType = VarType::UNKNOWN;
        analyzeExpression(varDecl->value.get(), exprType);

        if (symbolTable.count(varDecl->varName)) {
            // Check if the variable is already declared in the current scope
            std::stringstream ss;
            ss << "Line " << stmt->line << ": Variable '" << varDecl->varName << "' redeclared (previously declared at line "
               << symbolTable[varDecl->varName].lineDeclared << ").";
            errors.push_back(ss.str());
        } else {
            declareVariable(varDecl->varName, exprType, stmt->line);
        }
        return;
    }

    if (auto inputStmt = dynamic_cast<const InputStmt*>(stmt)) {
        // Check if the statement is an input statement
        if (!isVariableDeclared(inputStmt->varName)) {
            // Ensure the variable being read into is automatically declared
            declareVariable(inputStmt->varName, VarType::NUMBER, stmt->line);
        }
        return;
    }

    if (auto outputStmt = dynamic_cast<const OutputStmt*>(stmt)) {
        // Check if the statement is an output statement
        VarType exprType = VarType::UNKNOWN;
        analyzeExpression(outputStmt->value.get(), exprType);
        return;
    }

    if (auto binOp = dynamic_cast<const BinOpStmt*>(stmt)) {
        // Check if the statement is a binary operation
        VarType leftType = VarType::UNKNOWN;
        VarType rightType = VarType::UNKNOWN;

        if (!isVariableDeclared(binOp->left)) {
            // Verify the left operand variable exists
            errors.push_back("Line " + std::to_string(stmt->line) + ": Variable '" + binOp->left + "' not declared.");
        } else {
            leftType = getVariableType(binOp->left);
        }

        if (!isVariableDeclared(binOp->right)) {
            // Verify the right operand variable exists
            errors.push_back("Line " + std::to_string(stmt->line) + ": Variable '" + binOp->right + "' not declared.");
        } else {
            rightType = getVariableType(binOp->right);
        }

        if (leftType != VarType::NUMBER || rightType != VarType::NUMBER) {
            // Ensure both operands in the binary operation are numbers
            std::stringstream ss;
            ss << "Line " << stmt->line << ": Cannot perform binary operation on types ";
            ss << varTypeToString(leftType) << " and " << varTypeToString(rightType) << ".";
            errors.push_back(ss.str());
        }

        if (!isVariableDeclared(binOp->result)) {
            // Automatically declare the destination result variable if it's new
            declareVariable(binOp->result, VarType::NUMBER, stmt->line);
        }

        return;
    }

    if (auto ifStmt = dynamic_cast<const IfStmt*>(stmt)) {
        // Check if the statement is an 'if' control flow construct
        VarType condType = VarType::UNKNOWN;
        analyzeExpression(ifStmt->condition.get(), condType);
        for (const auto& s : ifStmt->thenBranch) analyzeStatement(s.get());
        for (const auto& s : ifStmt->elseIfBranches) analyzeStatement(s.get());
        for (const auto& s : ifStmt->elseBranch) analyzeStatement(s.get());
        return;
    }

    if (auto repeatStmt = dynamic_cast<const RepeatStmt*>(stmt)) {
        // Check if the statement is a 'repeat' loop
        if (!repeatStmt->varName.empty() && !isVariableDeclared(repeatStmt->varName)) {
            // Auto-declare loop counter variable if provided
            declareVariable(repeatStmt->varName, VarType::NUMBER, stmt->line);
        }
        if (repeatStmt->start) {
            // Analyze the starting value expression for the loop if it exists
            VarType t = VarType::UNKNOWN;
            analyzeExpression(repeatStmt->start.get(), t);
        }
        if (repeatStmt->end) {
            // Analyze the ending value expression for the loop if it exists
            VarType t = VarType::UNKNOWN;
            analyzeExpression(repeatStmt->end.get(), t);
        }
        if (repeatStmt->jump) {
            // Analyze the jump/step expression for the loop if it exists
            VarType t = VarType::UNKNOWN;
            analyzeExpression(repeatStmt->jump.get(), t);
        }
        if (repeatStmt->untilCondition) {
            // Analyze the 'until' termination condition if provided
            VarType t = VarType::UNKNOWN;
            analyzeExpression(repeatStmt->untilCondition.get(), t);
        }
        for (const auto& s : repeatStmt->body) analyzeStatement(s.get());
        return;
    }
}

// ==========================================
// Expression Analysis
// Resolves and asserts types for variables, literals, and operations
// ==========================================
void SemanticAnalyzer::analyzeExpression(const Expression* expr, VarType& outType) {
    if (!expr) {
        // Return UNKNOWN type immediately for null expressions
        outType = VarType::UNKNOWN;
        return;
    }

    if (auto id = dynamic_cast<const Identifier*>(expr)) {
        // Check if the expression is an identifier
        if (!isVariableDeclared(id->name)) {
            // Check if the identifier evaluates an undeclared variable
            errors.push_back("Line " + std::to_string(expr->line) + ": Variable '" + id->name + "' not declared.");
            outType = VarType::UNKNOWN;
        } else {
            outType = getVariableType(id->name);
        }
        return;
    }

    if (auto num = dynamic_cast<const NumberLiteral*>(expr)) {
        // Check if the expression is a numeric literal
        outType = VarType::NUMBER;
        return;
    }

    if (auto str = dynamic_cast<const StringLiteral*>(expr)) {
        // Check if the expression is a string literal
        outType = VarType::STRING;
        return;
    }

    if (auto rel = dynamic_cast<const RelOpExpr*>(expr)) {
        // Check if the expression is a relational operator
        VarType leftType = VarType::UNKNOWN;
        VarType rightType = VarType::UNKNOWN;

        analyzeExpression(rel->left.get(), leftType);
        analyzeExpression(rel->right.get(), rightType);

        bool valid = false;
        if (rel->op == "==" || rel->op == "!=") {
            // Ensure equality operations match exact types
            valid = (leftType == rightType) &&
                    (leftType == VarType::NUMBER || leftType == VarType::STRING);
        } else {
            // <, >, <=, >=
            valid = (leftType == VarType::NUMBER && rightType == VarType::NUMBER);
        }
        if (!valid) {
            // Log an error if relational types are incompatible
            std::stringstream ss;
            ss << "Line " << expr->line << ": Cannot compare " << varTypeToString(leftType)
               << " and " << varTypeToString(rightType) << " using relational operator '" << rel->op << "'.";
            errors.push_back(ss.str());
        }

        outType = VarType::BOOLEAN;
        return;
    }

    outType = VarType::UNKNOWN;
}

// ==========================================
// Symbol Table Operations
// Manage and verify variable scope, memory, and types
// ==========================================
void SemanticAnalyzer::declareVariable(const std::string& name, VarType type, int line) {
    symbolTable[name] = {type, line};
}

bool SemanticAnalyzer::isVariableDeclared(const std::string& name) const {
    return symbolTable.count(name) > 0;
}

VarType SemanticAnalyzer::getVariableType(const std::string& name) const {
    auto it = symbolTable.find(name);
    if (it != symbolTable.end()) return it->second.type;
    // Return the variable's type if found in the symbol table
    return VarType::UNKNOWN;
}

// ==========================================
// Error Handling
// Returns any type-mismatches collected
// ==========================================
const std::vector<std::string>& SemanticAnalyzer::getErrors() const {
    return errors;
}

// ==========================================
// Utility
// Converts VarType Enum to String representation
// ==========================================
std::string varTypeToString(VarType type) {
    switch (type) {
        case VarType::NUMBER: return "NUMBER";
        case VarType::STRING: return "STRING";
        case VarType::BOOLEAN: return "BOOLEAN";
        case VarType::UNKNOWN: return "UNKNOWN";
        default: return "INVALID";
    }
}
