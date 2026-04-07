#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "IntermediateCodeGen.h"
#include <vector>
#include <string>

// Optimizer phase targeting performance improvements on generated IR streams.
class Optimizer {
public:
    // Constructor.
    Optimizer();

    // Applies multiple optimization strategies on provided initial IR.
    void optimize(const std::vector<IRInstruction>& inputIR);

    // Grabs resulting instructions after optimizations conclude.
    const std::vector<IRInstruction>& getOptimizedIR() const;

private:
    std::vector<IRInstruction> optimizedIR; // Buffered end sequence map.

    // Passes arithmetic consolidations on static value instructions.
    void constantFolding();

    // Detaches assigned logic entries proving unutilized ahead.
    void removeRedundantAssignments();

    // Basic checker filtering pure numerical patterns.
    bool isNumber(const std::string& s) const;
};

#endif 
