#ifndef ONEFLOW_CORE_COMPILER_OF2XLA_XLA_GRAPH_COMPILER_H_
#define ONEFLOW_CORE_COMPILER_OF2XLA_XLA_GRAPH_COMPILER_H_

#include "tensorflow/compiler/xla/client/xla_builder.h"
#include "oneflow/core/graph/op_graph.h"

namespace oneflow {
namespace mola {

class XlaGraphCompiler {
 public:
  XlaGraphCompiler(OpGraph *graph, xla::XlaBuilder *builder, bool force_compile = false)
      : graph_(graph), builder_(builder), force_compile_(force_compile) {}

  void Compile();

 private:
  OpGraph *graph_;
  xla::XlaBuilder *builder_;

  bool force_compile_;
};

}  // namespace mola
}  // namespace oneflow

#endif  // ONEFLOW_CORE_COMPILER_OF2XLA_XLA_GRAPH_COMPILER_H_
