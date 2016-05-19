/**
 * Our entry point to the module
 */
#include "NanGraph.h"

using v8::FunctionTemplate;

NAN_MODULE_INIT(InitAll) {
  NanGraph::Init(target);
}

NODE_MODULE(nangraph, InitAll)
