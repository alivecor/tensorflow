#ifndef OPS_TO_REGISTER
#define OPS_TO_REGISTER


constexpr int strcmpc(const char* a, const char* b)
{
  return *a == 0 && *b == 0 ? 0 :
  *a == ' ' ? strcmpc(a+1,b) :
  *b == ' ' ? strcmpc(a,b+1) :
  *a == 0 ? -1 :
  *b == 0 ? 1 :
  *a < *b ? -1 :
  *a > *b ? 1 :
  *a == *b ? strcmpc(a+1, b+1) :
  0;
}

 
constexpr const char * strstrc( const char *str, const char *substr)
{
return ( 0 == str )? NULL : ( true == strcmpc( str, substr ) )? str : strstrc( str + 1, substr );
}


constexpr inline bool ShouldRegisterOp(const char op[]) {
  return false
     || (strcmpc(op, "Add") == 0)
     || (strcmpc(op, "BiasAdd") == 0)
     || (strcmpc(op, "Concat") == 0)
     || (strcmpc(op, "Const") == 0)
     || (strcmpc(op, "Conv2D") == 0)
     || (strcmpc(op, "Div") == 0)
     || (strcmpc(op, "Enter") == 0)
     || (strcmpc(op, "Exit") == 0)
     || (strcmpc(op, "Exp") == 0)
     || (strcmpc(op, "Fill") == 0)
     || (strcmpc(op, "Floor") == 0)
     || (strcmpc(op, "Identity") == 0)
     || (strcmpc(op, "Less") == 0)
     || (strcmpc(op, "Log") == 0)
     || (strcmpc(op, "LoopCond") == 0)
     || (strcmpc(op, "MatMul") == 0)
     || (strcmpc(op, "Maximum") == 0)
     || (strcmpc(op, "MaxPool") == 0)
     || (strcmpc(op, "Mean") == 0)
     || (strcmpc(op, "Merge") == 0)
     || (strcmpc(op, "Mul") == 0)
     || (strcmpc(op, "Neg") == 0)
     || (strcmpc(op, "NextIteration") == 0)
     || (strcmpc(op, "NoOp") == 0)
     || (strcmpc(op, "OneHot") == 0)
     || (strcmpc(op, "Pack") == 0)
     || (strcmpc(op, "Placeholder") == 0)
     || (strcmpc(op, "RandomUniform") == 0)
     || (strcmpc(op, "Range") == 0)
     || (strcmpc(op, "Relu") == 0)
     || (strcmpc(op, "Reshape") == 0)
     || (strcmpc(op, "Rsqrt") == 0)
     || (strcmpc(op, "Shape") == 0)
     || (strcmpc(op, "Sigmoid") == 0)
     || (strcmpc(op, "Slice") == 0)
     || (strcmpc(op, "Softmax") == 0)
     || (strcmpc(op, "SoftmaxCrossEntropyWithLogits") == 0)
     || (strcmpc(op, "Split") == 0)
     || (strcmpc(op, "Square") == 0)
     || (strcmpc(op, "Sqrt") == 0)
     || (strcmpc(op, "StridedSlice") == 0)
     || (strcmpc(op, "Sub") == 0)
     || (strcmpc(op, "Sum") == 0)
     || (strcmpc(op, "Switch") == 0)
     || (strcmpc(op, "Tanh") == 0)
     || (strcmpc(op, "TensorArrayGatherV2") == 0)
     || (strcmpc(op, "TensorArrayReadV2") == 0)
     || (strcmpc(op, "TensorArrayScatterV2") == 0)
     || (strcmpc(op, "TensorArraySizeV2") == 0)
     || (strcmpc(op, "TensorArrayV2") == 0)
     || (strcmpc(op, "TensorArrayWriteV2") == 0)
     || (strcmpc(op, "Transpose") == 0)
     || (strcmpc(op, "_Recv") == 0)
     || (strcmpc(op, "_Send") == 0)
  ;
}
#define SHOULD_REGISTER_OP(op) ShouldRegisterOp(op)

const char kNecessaryOpKernelClasses[] = ","
"BiasAddOp<CPUDevice, float>,"
"BinaryOp< CPUDevice, functor::add<float>>,"
"BinaryOp< CPUDevice, functor::add<int32>>,"
"ConcatOp<CPUDevice, ::tensorflow::int32>,"
"ConcatOp<CPUDevice, float>,"
"ConstOp<CPUDevice, float>,"  // Not sure if this is right.
"ConstantOp,"
"Conv2DOp<CPUDevice, float>,"
"BinaryOp< CPUDevice, functor::div<float>>,"
"EnterOp,"
"ExitOp,"
"FillOp<CPUDevice, float>,"
"UnaryOp< CPUDevice, functor::floor<float>>,"
"IdentityOp,"
"BinaryOp< CPUDevice, functor::less<int32>>,"
"LoopCondOp,"
"MatMulOp<CPUDevice, float, false >,"
"MaxPoolingOp<CPUDevice, float>,"
"ReductionOp<CPUDevice, float, Eigen::internal::MeanReducer<float>>,"
"MergeOp,"
"BinaryOp< CPUDevice, functor::mul<float>>,"
"BinaryOp< CPUDevice, functor::mul<int32>>,"
"NextIterationOp,"
"NoOp,"
"OneHotOp<CPUDevice, float, int32>,"
"PackOp<CPUDevice, ::tensorflow::int32>,"
"PackOp<CPUDevice, float>,"
"PlaceholderOp,"
"PhiloxRandomOp<CPUDevice, random::UniformDistribution< random::PhiloxRandom, float> >,"
"RangeOp<::tensorflow::int32>,"
"ReluOp<CPUDevice, float>,"
"ReshapeOp,"
"ShapeOp<int32>,"
"UnaryOp< CPUDevice, functor::sigmoid<float>>,"
"SliceOp<CPUDevice, ::tensorflow::int32>,"
"SliceOp<CPUDevice, float>,"
"SoftmaxOp<CPUDevice, float>,"
"SoftmaxXentWithLogitsOp<CPUDevice, float>,"
"SplitOpCPU<float>,"
"StridedSliceOp<CPUDevice, ::tensorflow::int32>,"
"BinaryOp< CPUDevice, functor::sub<float>>,"
"BinaryOp< CPUDevice, functor::sub<int32>>,"
"SwitchOp,"
"UnaryOp< CPUDevice, functor::tanh<float>>,"
"TensorArrayPackOrGatherOp<CPUDevice, float, false >,"
"TensorArrayReadOp<CPUDevice, float>,"
"TensorArrayUnpackOrScatterOp<CPUDevice, float, false >,"
"TensorArraySizeOp,"
"TensorArrayOp,"
"TensorArrayWriteOp<CPUDevice, float>,"
"TransposeCpuOp,"
"RecvOp,"
"SendOp,"
;
#define SHOULD_REGISTER_OP_KERNEL(clz) (strstrc(kNecessaryOpKernelClasses, "," clz ",") != nullptr)

#define SHOULD_REGISTER_OP_GRADIENT false
#endif
