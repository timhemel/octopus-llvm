#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include "OctopusGraph.h"
#include "GraphvizWriter.h"

using namespace llvm;

namespace {
	struct OctopusGraphPass : public FunctionPass {
		static char ID;

		OctopusGraphPass();

		virtual bool runOnFunction(Function &F);

		virtual bool doFinalization(Module &M);

	private:

		Octopus::OctopusGraph octopus_graph;

	};
}

char OctopusGraphPass::ID = 0;

OctopusGraphPass::OctopusGraphPass() : FunctionPass(ID)
{
}


bool OctopusGraphPass::runOnFunction(Function &F)
{
	// create CFGEntry and CFGExit nodes
	octopus_graph.createEntryAndExitNodesForFunction(F);
	for(Function::iterator b = F.begin(), be = F.end(); b != be; ++b) {
		// create all instruction nodes and link them
		octopus_graph.createAndConnectInstructionNodesForBasicBlock(*b);
		// link basic block with predecessors and successors?
		octopus_graph.linkBasicBlock(*b);
	}
	return false;
}

bool OctopusGraphPass::doFinalization(Module &M)
{
	GraphvizWriter::writeOctopusGraph(octopus_graph);
	return false;
}


static void registerOctopusGraphPass(const PassManagerBuilder &, legacy::PassManagerBase &PM)
{
	PM.add(new OctopusGraphPass());
}

static RegisterStandardPasses RegisterOctopusGraphPass(PassManagerBuilder::EP_EarlyAsPossible, registerOctopusGraphPass);

