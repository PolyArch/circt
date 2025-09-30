//this file declares passes for legalizing the scf dialect before lowering to 

#ifndef CIRCT_CONVERSION_LEGALIZELLVM_H
#define CIRCT_CONVERSION_LEGALIZELLVM_H

#include "mlir/Pass/Pass.h"

namespace mlir {
class ModuleOp;
template <typename T>
class OperationPass;
} // namespace mlir

namespace circt {
#define GEN_PASS_DECL_LEGALIZELLVM
#include "circt/Conversion/Passes.h.inc"


/// Creates a pass to legalize the LLVM dialect for CIRCT hardware lowering.
std::unique_ptr<mlir::Pass> createLegalizeLLVMPass();

} // namespace circt

#endif // CIRCT_CONVERSION_LEGALIZELLVM_H