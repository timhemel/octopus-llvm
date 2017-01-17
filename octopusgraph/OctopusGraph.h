#include "llvm/IR/Function.h"

using namespace llvm;

namespace OctopusGraph {

	// lookup tables for Instruction -> InstructionNode
	// lookup tables for DIInfo -> LocationNode
	class OctopusGraph {
	public:
		void createEntryAndExitNodesForFunction(Function &F);
		void createAndConnectInstructionNodesForBasicBlock(BasicBlock &B);
		void linkBasicBlock(BasicBlock &B);

		void createInstructionNode(Instruction &I);
		void linkInstructionWithPredecessor(Instruction *prev_instruction, Instruction &current_instruction);
	};
}

