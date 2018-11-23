#include "../lib/Target/CBackend/CBackend.h"
#include "llvm/IR/TypeBuilder.h"
#include "gtest/gtest.h"

using namespace llvm;
using namespace llvm::types;

namespace llvm_cbe {

class CWriterTestHelper {
public:
  static std::string getTypeName(Type *ty, bool isSigned = false,
                                 std::pair<AttributeList, CallingConv::ID> PAL =
                                     std::make_pair(AttributeList(),
                                                    CallingConv::C)) {
    raw_null_ostream NullStm;
    CWriter Writer(NullStm);

    std::string Str;
    raw_string_ostream StrStm(Str);
    Writer.printTypeName(StrStm, ty, isSigned, PAL);
    StrStm.flush();

    return Str;
  }
};

TEST(TypeNameTest, Void) {
  LLVMContext ctx;
  Type *ty = Type::getVoidTy(ctx);
  EXPECT_EQ(CWriterTestHelper::getTypeName(ty), "void");
}

TEST(TypeNameTest, Bool) {
  LLVMContext ctx;
  EXPECT_EQ(CWriterTestHelper::getTypeName(Type::getInt1Ty(ctx)), "bool");
}

TEST(TypeNameTest, SimpleNumeric) {
  LLVMContext ctx;
  EXPECT_EQ(CWriterTestHelper::getTypeName(Type::getInt8Ty(ctx)), "uint8_t");
  EXPECT_EQ(CWriterTestHelper::getTypeName(Type::getInt16Ty(ctx)), "uint16_t");
  EXPECT_EQ(CWriterTestHelper::getTypeName(Type::getInt32Ty(ctx)), "uint32_t");
  EXPECT_EQ(CWriterTestHelper::getTypeName(Type::getInt64Ty(ctx)), "uint64_t");
  EXPECT_EQ(CWriterTestHelper::getTypeName(Type::getInt128Ty(ctx)),
            "uint128_t");
  EXPECT_EQ(CWriterTestHelper::getTypeName(Type::getFloatTy(ctx)), "float");
  EXPECT_EQ(CWriterTestHelper::getTypeName(Type::getDoubleTy(ctx)), "double");
}

TEST(TypeNameTest, BoolPtr) {
  LLVMContext ctx;
  EXPECT_EQ(
      CWriterTestHelper::getTypeName(Type::getInt1Ty(ctx)->getPointerTo()),
      "bool*");
}

TEST(TypeNameTest, SimpleNumericPtr) {
  LLVMContext ctx;
  EXPECT_EQ(
      CWriterTestHelper::getTypeName(Type::getInt8Ty(ctx)->getPointerTo()),
      "uint8_t*");
  EXPECT_EQ(
      CWriterTestHelper::getTypeName(Type::getInt16Ty(ctx)->getPointerTo()),
      "uint16_t*");
  EXPECT_EQ(
      CWriterTestHelper::getTypeName(Type::getInt32Ty(ctx)->getPointerTo()),
      "uint32_t*");
  EXPECT_EQ(
      CWriterTestHelper::getTypeName(Type::getInt64Ty(ctx)->getPointerTo()),
      "uint64_t*");
  EXPECT_EQ(
      CWriterTestHelper::getTypeName(Type::getInt128Ty(ctx)->getPointerTo()),
      "uint128_t*");
  EXPECT_EQ(
      CWriterTestHelper::getTypeName(Type::getFloatTy(ctx)->getPointerTo()),
      "float*");
  EXPECT_EQ(
      CWriterTestHelper::getTypeName(Type::getDoubleTy(ctx)->getPointerTo()),
      "double*");
}

TEST(TypeNameTest, SimpleFunctionPtr) {
  LLVMContext ctx;
  EXPECT_EQ(CWriterTestHelper::getTypeName(
                TypeBuilder<void (*)(void), true>::get(ctx)),
            "void (*)(void)");
  EXPECT_EQ(CWriterTestHelper::getTypeName(
                TypeBuilder<void (*)(i<32>), true>::get(ctx)),
            "void (*)(uint32_t)");
  EXPECT_EQ(CWriterTestHelper::getTypeName(
                TypeBuilder<i<32> (*)(void), true>::get(ctx)),
            "uint32_t (*)(void)");
  EXPECT_EQ(CWriterTestHelper::getTypeName(
                TypeBuilder<void (*)(i<8> *), true>::get(ctx)),
            "void (*)(uint8_t*)");
}

TEST(TypeNameTest, VarArgsFunctionPtr) {
  LLVMContext ctx;
  EXPECT_EQ(CWriterTestHelper::getTypeName(
                TypeBuilder<void (*)(i<32>, ...), true>::get(ctx)),
            "void (*)(uint32_t, ...)");
  EXPECT_EQ(CWriterTestHelper::getTypeName(
                TypeBuilder<void (*)(i<8> *, ...), true>::get(ctx)),
            "void (*)(uint8_t*, ...)");
}

TEST(TypeNameTest, NestedFunctionPtr) {
  LLVMContext ctx;
  typedef i<32> (*func1_t)(i<32>);
  typedef i<32> (*func2_t)(func1_t, i<32>);
  EXPECT_EQ(
      CWriterTestHelper::getTypeName(TypeBuilder<func2_t, true>::get(ctx)),
      "uint32_t (*)(uint32_t (*)(uint32_t), uint32_t)");
}

TEST(TypeNameTest, CallingConvFunctionPtr) {
  LLVMContext ctx;
  typedef void (*func_ptr_t)(void);
  Type *Ty = TypeBuilder<func_ptr_t, true>::get(ctx);
  EXPECT_EQ(
      CWriterTestHelper::getTypeName(
          Ty, false, std::make_pair(AttributeList(), CallingConv::X86_StdCall)),
      // TODO: is this valid C?
      "void (*)(void) __stdcall");
  EXPECT_EQ(CWriterTestHelper::getTypeName(
                Ty, false,
                std::make_pair(AttributeList(), CallingConv::X86_FastCall)),
            // TODO: is this valid C?
            "void (*)(void) __fastcall");
  EXPECT_EQ(CWriterTestHelper::getTypeName(
                Ty, false,
                std::make_pair(AttributeList(), CallingConv::X86_ThisCall)),
            // TODO: is this valid C?
            "void (*)(void) __thiscall");
}

} // namespace llvm_cbe
