#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include <memory>
#include <vector>
#include <string>

// Foundational descriptor tracking source line coordinates.
struct ASTNode {
    virtual ~ASTNode() = default;
    int line;
    int column;
};

// Top-level differentiators mapping valid code blocks.
struct Statement : public ASTNode {};
struct Expression : public ASTNode {};

// Binds variable allocations handling assigned expressions.
struct VarDecl : public Statement {
    std::string varName;
    std::unique_ptr<Expression> value;
};

using Assignment = VarDecl;

// Denotes an ingestion query awaiting user console value.
struct InputStmt : public Statement {
    std::string varName;
};

// Represents printing an enclosed value chunk to system out.
struct OutputStmt : public Statement {
    std::unique_ptr<Expression> value;
};

// Mathematical operator bindings bridging expression terms.
enum class BinOpType { ADD, SUBTRACT, MULTIPLY, DIVIDE };
struct BinOpStmt : public Statement {
    BinOpType op;
    std::string left;
    std::string right;
    std::string result;
};

// Constructs traditional `if-else` logical branching boundaries.
struct IfStmt : public Statement {
    std::unique_ptr<Expression> condition;
    std::vector<std::unique_ptr<Statement>> thenBranch;
    std::vector<std::unique_ptr<Statement>> elseIfBranches;
    std::vector<std::unique_ptr<Statement>> elseBranch;
};

// Organizes iterations simulating bounded loops.
struct RepeatStmt : public Statement {
    std::string varName;
    std::unique_ptr<Expression> start;
    std::unique_ptr<Expression> end;
    std::unique_ptr<Expression> jump;
    std::vector<std::unique_ptr<Statement>> body;

    std::unique_ptr<Expression> untilCondition;
};

// Primitive mapping plain labeled reference strings.
struct Identifier : public Expression {
    std::string name;
};

// Primitive literal representation for constant numbers.
struct NumberLiteral : public Expression {
    std::string value;
};

// Primitive literal retaining static character bounds.
struct StringLiteral : public Expression {
    std::string value;
};

// Binds complex evaluating expressions testing Boolean properties.
struct RelOpExpr : public Expression {
    std::unique_ptr<Expression> left;
    std::string op; 
    std::unique_ptr<Expression> right;
};

// Outer-most container collecting independent line statements.
struct Program : public ASTNode {
    std::vector<std::unique_ptr<Statement>> statements;
};

// Parser responsible for converting a Token stream into an Abstract Syntax Tree (AST).
class Parser {
public:
    // Binds lexed tokens against current analysis context.
    Parser(const std::vector<Token>& tokens);
    
    // Core builder generating the absolute AST payload hierarchy.
    std::unique_ptr<Program> parse();
    
    // Obtains encountered structural irregularities logged during construction.
    std::vector<std::string> getErrors() const;

private:
    const std::vector<Token>& tokens; // The incoming sequence of grammatical tokens
    size_t pos;                       // Offset keeping progression bounds
    std::vector<std::string> errors;  // Saved failures stopping flawless compilation

    // Helper returning upcoming sequence token silently.
    const Token& peek() const;

    // Helper capturing sequence token directly advancing state.
    const Token& get();

    // Validates targeted type existence, bumping progression optionally.
    bool match(TokenType type);

    // Validates logical keyword existence similarly omitting parameters.
    bool matchKeyword(TokenType type);

    // Mandates explicit token verification injecting strict failures if missing.
    void expect(TokenType type, const std::string& errorMsg);

    // Dispatches tree sub-creations analyzing contextual statement variants.
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Statement> parseVarDecl();
    std::unique_ptr<Statement> parseInput();
    std::unique_ptr<Statement> parseOutput();
    std::unique_ptr<Statement> parseBinOp();
    std::unique_ptr<Statement> parseIf();
    std::unique_ptr<Statement> parseRepeat();

    // Breaks down math or literal terms filling deeper expression levels.
    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> parseRelOpExpr();
    std::unique_ptr<Expression> parsePrimary();
};

#endif 
