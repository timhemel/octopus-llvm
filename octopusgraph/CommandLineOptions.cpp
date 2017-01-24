#define COMMAND_LINE_OPTIONS_CPP
#include "CommandLineOptions.h"


static cl::opt<bool,true> OutputGraphviz("graphviz", cl::desc("Write graph in graphviz format"),cl::location(optionOutputGraphviz));
static cl::opt<bool,true> OutputGraphstream("graphstream", cl::desc("Write graph in graphstream format"),cl::location(optionOutputGraphstream));
static cl::opt<bool,true> NoDataDependenceEdges("noddg", cl::desc("Do not generate DDG edges"),cl::location(optionNoDataDependenceEdges));
static cl::opt<bool,true> NoLocationNodesAndEdges("noloc", cl::desc("Do not generate location nodes and edges"),cl::location(optionNoLocationNodesAndEdges));

