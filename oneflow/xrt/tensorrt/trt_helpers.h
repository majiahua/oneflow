#ifndef ONEFLOW_XRT_TENSORRT_TRT_HELPERS_H_
#define ONEFLOW_XRT_TENSORRT_TRT_HELPERS_H_

#include "oneflow/xrt/tensorrt/ops/op_context.h"
#include "oneflow/xrt/tensorrt/trt_shape.h"

namespace oneflow {
namespace xrt {
namespace tensorrt {

namespace helpers {

bool DimsEqual(const nvinfer1::Dims &dim1, const nvinfer1::Dims &dim2);

nvinfer1::ITensor *Reshape(TrtOpContext *ctx, nvinfer1::ITensor *in,  // NOLINT
                           const Shape &shape);

nvinfer1::ITensor *Reshape(TrtOpContext *ctx, nvinfer1::Weights in,  // NOLINT
                           const Shape &shape);

nvinfer1::ITensor *Transpose(TrtOpContext *ctx, nvinfer1::ITensor *in,
                             const std::vector<int> &permute);

nvinfer1::ITensor *Transpose(TrtOpContext *ctx, nvinfer1::Weights in, const Shape &shape,
                             const std::vector<int> &permute);

}  // namespace helpers

}  // namespace tensorrt
}  // namespace xrt
}  // namespace oneflow

#endif  // ONEFLOW_XRT_TENSORRT_TRT_HELPERS_H_
