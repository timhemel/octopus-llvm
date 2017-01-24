#ifndef COMMAND_LINE_OPTIONS_H
#define COMMAND_LINE_OPTIONS_H

#include "llvm/Support/CommandLine.h"

using namespace llvm;

#if !defined(COMMAND_LINE_OPTIONS_CPP)
#define EXTERN extern
#else
#define EXTERN 
#endif

namespace OctopusCLI {

EXTERN bool optionOutputGraphviz;
EXTERN bool optionOutputGraphstream;
EXTERN bool optionNoDataDependenceEdges;
EXTERN bool optionNoLocationNodesAndEdges;

}

using namespace OctopusCLI;

#endif
