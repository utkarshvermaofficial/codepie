#include "Optimizer.h"
#include <sstream>
#include <unordered_map>

Optimizer::Optimizer() {}

// ==========================================
// Central Optimizer Function
// Orchestrates optimization techniques over raw IR
// ==========================================
void Optimizer::optimize(const std::vector<IRInstruction>& inputIR) {
    optimizedIR = inputIR;
    constantFolding();
    removeRedundantAssignments();
}

const std::vector<IRInstruction>& Optimizer::getOptimizedIR() const {
    return optimizedIR;
}

// ==========================================
// Numeric Type Checker
// Checks if string value can be evaluated to a number
// ==========================================
bool Optimizer::isNumber(const std::string& s) const {
    if (s.empty()) return false;
    std::istringstream iss(s);
    double d;
// ==========================================
// Constant Folding Optimization
// Evaluates literal arithmetic expressions at compile time
// ==========================================
    iss >> d;
    return iss.eof() && !iss.fail();
}

void Optimizer::constantFolding() {
    for (auto& instr : optimizedIR) {
        if ((instr.opcode == "ADD" || instr.opcode == "SUB" ||
             instr.opcode == "MUL" || instr.opcode == "DIV") &&
            instr.operands.size() == 3) {
            
            if (isNumber(instr.operands[0]) && isNumber(instr.operands[1])) {
                double left = std::stod(instr.operands[0]);
                double right = std::stod(instr.operands[1]);
                double result = 0.0;
                if (instr.opcode == "ADD") result = left + right;
                else if (instr.opcode == "SUB") result = left - right;
                else if (instr.opcode == "MUL") result = left * right;
                else if (instr.opcode == "DIV") result = (right != 0.0) ? left / right : 0.0;
                instr.opcode = "ASSIGN";
                instr.operands = { std::to_string(result), instr.operands[2] };
            }
        }
        
        if ((instr.opcode == "LT" || instr.opcode == "LE" || 
             instr.opcode == "GT" || instr.opcode == "GE" ||
             instr.opcode == "EQ" || instr.opcode == "NE") &&
            instr.operands.size() == 3) {
            
            if (isNumber(instr.operands[0]) && isNumber(instr.operands[1])) {
                double left = std::stod(instr.operands[0]);
                double right = std::stod(instr.operands[1]);
                bool result = false;
                
                if (instr.opcode == "LT") result = (left < right);
                else if (instr.opcode == "LE") result = (left <= right);
                else if (instr.opcode == "GT") result = (left > right);
                else if (instr.opcode == "GE") result = (left >= right);
                else if (instr.opcode == "EQ") result = (left == right);
                else if (instr.opcode == "NE") result = (left != right);
// ==========================================
// Redundant Assignment Removal
// Prunes self-assignments and back-to-back duplicate assignments
// ==========================================
                
                instr.opcode = "ASSIGN";
                instr.operands = { std::to_string(result ? 1 : 0), instr.operands[2] };
            }
        }
    }
}

void Optimizer::removeRedundantAssignments() {
    std::vector<IRInstruction> filtered;
    for (const auto& instr : optimizedIR) {
        bool isRedundant = (instr.opcode == "ASSIGN" && 
                            instr.operands.size() == 2 &&
                            instr.operands[0] == instr.operands[1]);
        
        if (!isRedundant && !filtered.empty() &&
            instr.opcode == "ASSIGN" &&
            filtered.back().opcode == "ASSIGN" &&
            filtered.back().operands.size() == 2 &&
            filtered.back().operands[0] == instr.operands[0]) {
            isRedundant = true;
        }
        
        if (!isRedundant) filtered.push_back(instr);
    }
    optimizedIR = std::move(filtered);
}
