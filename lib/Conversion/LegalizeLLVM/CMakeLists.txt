
add_circt_conversion_library(CIRCTLegalizeLLVM
  LegalizeLLVM.cpp


  LINK_LIBS PUBLIC
  # Link against the core CIRCT and MLIR libraries.
  CIRCTSupport
  CIRCTTransforms
  MLIRIR
  MLIRPass

  # Link against the dialects this pass uses.
  MLIRArithDialect  # For the output arith.constant
  MLIRFuncDialect   # Since the pass runs on func.func
  MLIRLLVMDialect   # For the input llvm.mlir.undef
)
