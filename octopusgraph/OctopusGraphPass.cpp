#include <iostream>
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include "CommandLineOptions.h"
#include "OctopusGraph.h"
#include "GraphvizWriter.h"
#include "DGSWriter.h"

using namespace llvm;


namespace {
	struct OctopusGraphPass : public FunctionPass {
		static char ID;

		OctopusGraphPass();

		virtual bool runOnFunction(Function &F) override;

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
	octopus_graph.initializeFunction();
	octopus_graph.createEntryAndExitNodesForFunction(F);
	for(Function::iterator b = F.begin(), be = F.end(); b != be; ++b) {
		octopus_graph.addBlockLabel(*b);
		octopus_graph.createAndConnectInstructionNodesForBasicBlock(*b);
	}
	for(Function::iterator b = F.begin(), be = F.end(); b != be; ++b) {
		octopus_graph.linkBasicBlock(*b);
	}
	if (!optionNoDataDependenceEdges) {
		octopus_graph.createDataDependenceEdges();
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
	if (optionOutputGraphviz) {
		GraphWriter::GraphvizWriter w(std::cout);
		w.writeOctopusGraph(octopus_graph);
	}
	if (optionOutputGraphstream) {
		GraphWriter::DGSWriter w(std::cout);
		w.writeOctopusGraph(octopus_graph);
	}
	return false;
}


static void registerOctopusGraphPass(const PassManagerBuilder &, legacy::PassManagerBase &PM)
{
	PM.add(new OctopusGraphPass());
}

static RegisterStandardPasses RegisterOctopusGraphPass(PassManagerBuilder::EP_EarlyAsPossible, registerOctopusGraphPass);

