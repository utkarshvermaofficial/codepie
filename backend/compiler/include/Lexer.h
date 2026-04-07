#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <memory>

// Enumerates all the recognized token types by the language syntax.
enum class TokenType {
    LET, BE, INPUT, OUTPUT,
    ADD, SUBTRACT, MULTIPLY, DIVIDE, STORE, IN, AND,
    IF, ELSE, OTHERWISE, THEN,
    REPEAT, FROM, TO, JUMP, UNTIL,
    ASSIGN, 
    REL_OP, 
    IDENTIFIER, NUMBER, STRING,
    END_OF_LINE,
    END_OF_FILE,
    INVALID
};

// Represents a fundamental lexical token.
struct Token {
    TokenType type;       // Type classification of the token
    std::string lexeme;   // Text content mapped to the token
    int line;             // Line number indicating where the token starts
    int column;           // Column number indicating where the token starts

    // Initializes the token.
    Token(TokenType t, const std::string& l, int ln, int col)
        : type(t), lexeme(l), line(ln), column(col) {}
};

// Lexer module corresponding to lexical analysis.
class Lexer {
public:
    // Initializes the lexer with the provided source code input.
    Lexer(const std::string& source);

    // Starts processing and returns the collection of generated tokens.
    std::vector<Token> tokenize();

private:
    std::string source; // The entire given source string.
    size_t pos;         // Current reading offset index.
    int line;           // Tracks present traversal line.
    int column;         // Tracks present traversal column.

    // Views the pending character without moving the cursor.
    char peek() const;

    // Pulls the current character and moves the cursor forward.
    char get();

    // Advances the string offset ignoring all spaces, tabs, and newlines.
    void skipWhitespace();

    // Advances the string offset ignoring arbitrary textual comments.
    void skipComment();

    // Resolves textual substrings into valid identifiers or keywords.
    Token identifierOrKeyword();

    // Identifies positive/negative numeric combinations.
    Token number();

    // Detects bounded string constants.
    Token stringLiteral();

    // Resolves matching relational operators.
    Token relOp();
};

#endif 
