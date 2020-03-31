#include "oneflow/core/kernel/reduce_min_kernel.h"
#include "oneflow/core/common/device_type.pb.h"
#include "oneflow/core/ndarray/ndarray_util.h"

namespace oneflow {

template<DeviceType device_type, typename T>
void ReduceMinKernel<device_type, T>::ForwardDataContent(
    const KernelCtx& ctx, std::function<Blob*(const std::string&)> BnInOp2Blob) const {
  const Blob* in_blob = BnInOp2Blob("in");
  Blob* out_blob = BnInOp2Blob("out");
  Blob* fw_tmp_blob = BnInOp2Blob("fw_tmp");
  const ReduceMinOpConf& conf = this->op_conf().reduce_min_conf();
  const Shape& reduced_shape =
      conf.axis().empty()
          ? Shape::Ones(in_blob->shape().NumAxes())
          : CreateReducedShape(in_blob->shape(), {conf.axis().begin(), conf.axis().end()});
  NdarrayUtil<device_type, T>::ReduceMin(
      ctx.device_ctx, XpuVarNdarray<T>(reduced_shape, out_blob->mut_dptr<T>()),
      XpuVarNdarray<const T>(in_blob, in_blob->shape().NumAxes()),
      XpuVarNdarray<T>(fw_tmp_blob, in_blob->shape().NumAxes()));
}

ADD_DEFAULT_KERNEL_CREATOR_WITH_GPU_HALF(OperatorConf::kReduceMinConf, ReduceMinKernel,
                                         ARITHMETIC_DATA_TYPE_SEQ);

}  // namespace oneflow