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

		void createInstructionNode(Instruction *instruction);
		void linkInstructionWithPredecessor(Instruction *previous_instruction, Instruction *current_instruction);
	};


	class InstructionNode {
	public:
		InstructionNode(Instruction *instruction);
	};

}

