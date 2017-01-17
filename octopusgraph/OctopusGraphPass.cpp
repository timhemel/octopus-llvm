#include "llvm/Pass.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include "OctopusGraph.h"

using namespace llvm;

namespace {
	struct OctopusGraphPass : public FunctionPass {
		static char ID;

		OctopusGraphPass();

		virtual bool runOnFunction(Function &F);

	private:

		OctopusGraph::OctopusGraph octopus_graph;

	};
}

char OctopusGraphPass::ID = 0;

OctopusGraphPass::OctopusGraphPass() : FunctionPass(ID)
{
}


bool OctopusGraphPass::runOnFunction(Function &F)
{
	return false;
}


static void registerOctopusGraphPass(const PassManagerBuilder &, legacy::PassManagerBase &PM)
{
	PM.add(new OctopusGraphPass());
}

static RegisterStandardPasses RegisterOctopusGraphPass(PassManagerBuilder::EP_EarlyAsPossible, registerOctopusGraphPass);

