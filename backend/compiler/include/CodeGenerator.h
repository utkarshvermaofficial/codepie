#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include "IntermediateCodeGen.h"
#include <string>
#include <vector>
#include <unordered_map>

// Class responsible for generating target C code from Intermediate Representation (IR).
class CodeGenerator {
public:
    // Constructor
    CodeGenerator();

    // Generates C code from a list of IR instructions.
    void generate(const std::vector<IRInstruction>& ir);

    // Returns the generated C code as a string.
    const std::string& getCCode() const;

private:
    // Stores the generated C code string.
    std::string cCode;

    // Checks if a string represents a valid identifier.
    bool isIdentifier(const std::string& s) const;

    // Maps variable names to their declared types.
    std::unordered_map<std::string, std::string> declaredVars;

    // Helper to declare a variable and optionally initialize it.
    void declareVar(const std::string& var, const std::string& value);

    // Helpers to determine the data type from a string.
    bool isInteger(const std::string& s) const;
    bool isDouble(const std::string& s) const;
  
    // Emits C code for a single IR instruction into a string builder.
    void emitSingleStatement(const IRInstruction& instr, std::ostringstream& oss);
};

#endif 
