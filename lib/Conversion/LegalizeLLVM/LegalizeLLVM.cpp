#include "circt/Conversion/LegalizeLLVM.h"
#include "mlir/Dialect/Arith/IR/Arith.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/Dialect/LLVMIR/LLVMDialect.h"
#include "mlir/IR/PatternMatch.h"
#include "mlir/Pass/Pass.h"
#include "mlir/Transforms/DialectConversion.h"
#include "mlir/IR/Builders.h"

namespace circt {
#define GEN_PASS_DEF_LEGALIZELLVM
#include "circt/Conversion/Passes.h.inc"
}



namespace {

// Pattern to lower llvm.undef to a zero constant.
struct UndefOpLowering : public mlir::OpConversionPattern<mlir::LLVM::UndefOp> {
  using OpConversionPattern<mlir::LLVM::UndefOp>::OpConversionPattern;

  mlir::LogicalResult
  matchAndRewrite(mlir::LLVM::UndefOp op, OpAdaptor adaptor,
                  mlir::ConversionPatternRewriter &rewriter) const override {
    //create the new constantOp
    mlir::TypedAttr zeroAttr = rewriter.getZeroAttr(op.getType());
    if (!zeroAttr) {return mlir::failure();}
    auto newOp = rewriter.create<mlir::arith::ConstantOp>(
        op.getLoc(),             // Location
        op.getType(),                  // Type
        zeroAttr //value is the zeroAttr
    );
    rewriter.replaceOp(op,newOp);
    return mlir::success();
  }
};
} // namespace
// The new pass definition.
struct LegalizeLLVMPass
    : public circt::impl::LegalizeLLVMBase<LegalizeLLVMPass> {
  void runOnOperation() override {
    mlir::ConversionTarget target(getContext());
    mlir::RewritePatternSet patterns(&getContext());

    // This pass will eliminate llvm.undef and leave arith.constant.
    target.addIllegalOp<mlir::LLVM::UndefOp>();
    target.addLegalOp<mlir::arith::ConstantOp>();

    patterns.add<UndefOpLowering>(&getContext());

    if (failed(applyPartialConversion(getOperation(), target,
                                      std::move(patterns))))
      signalPassFailure();
  }
};


// Function to create the pass.
std::unique_ptr<mlir::Pass> circt::createLegalizeLLVMPass() {
  return std::make_unique<LegalizeLLVMPass>();
}