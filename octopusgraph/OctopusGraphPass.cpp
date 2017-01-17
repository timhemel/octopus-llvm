#include "llvm/Pass.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace llvm;

namespace {
	struct OctopusGraphPass : public FunctionPass {
		static char ID;
		// implement destructor

		OctopusGraphPass() : FunctionPass(ID) { }

		virtual bool runOnFunction(Function &F);

	};
}

char OctopusGraphPass::ID = 0;

bool OctopusGraphPass::runOnFunction(Function &F) {
}


static void registerOctopusGraphPass(const PassManagerBuilder &, legacy::PassManagerBase &PM) {
	PM.add(new OctopusGraphPass());
}

// rename?
static RegisterStandardPasses RegisterOctopusGraphPass(PassManagerBuilder::EP_EarlyAsPossible, registerOctopusGraphPass);

