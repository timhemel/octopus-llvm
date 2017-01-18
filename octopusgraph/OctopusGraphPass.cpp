#include <iostream>
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include "llvm/IR/ModuleSlotTracker.h"

#include "OctopusGraph.h"
#include "GraphvizWriter.h"

using namespace llvm;

namespace {
	struct OctopusGraphPass : public FunctionPass {
		static char ID;

		OctopusGraphPass();

		virtual bool runOnFunction(Function &F);

		virtual bool doInitialization(Module &M);
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
	octopus_graph.initializeFunction();
	octopus_graph.createEntryAndExitNodesForFunction(F);
	for(Function::iterator b = F.begin(), be = F.end(); b != be; ++b) {
		octopus_graph.addBlockLabel(*b);
		// create all instruction nodes and link them
		octopus_graph.createAndConnectInstructionNodesForBasicBlock(*b);
		// link basic block with predecessors and successors?
		octopus_graph.linkBasicBlock(*b);
	}
	octopus_graph.finalizeFunction();
	return false;
}

bool OctopusGraphPass::doInitialization(Module &M)
{
	return false;
}

bool OctopusGraphPass::doFinalization(Module &M)
{
	GraphWriter::GraphvizWriter w(std::cout);
	w.writeOctopusGraph(octopus_graph);
	return false;
}


static void registerOctopusGraphPass(const PassManagerBuilder &, legacy::PassManagerBase &PM)
{
	PM.add(new OctopusGraphPass());
}

static RegisterStandardPasses RegisterOctopusGraphPass(PassManagerBuilder::EP_EarlyAsPossible, registerOctopusGraphPass);

