#include "llvm/Support/raw_ostream.h"
#include "OctopusGraph.h"

namespace OctopusGraph {

	void OctopusGraph::createEntryAndExitNodesForFunction(Function &F)
	{
	}

	void OctopusGraph::createAndConnectInstructionNodesForBasicBlock(BasicBlock &B)
	{
		Instruction *prev_instruction = 0;
		for(BasicBlock::iterator i = B.begin(), ie = B.end(); i != ie; ++i) {
			Instruction *current_instruction = dyn_cast<Instruction>(&*i);
			createInstructionNode(current_instruction);
			linkInstructionWithPredecessor(prev_instruction,current_instruction);
			prev_instruction = current_instruction;
		}
	}

	void OctopusGraph::linkBasicBlock(BasicBlock &B)
	{
	}

	void OctopusGraph::createInstructionNode(Instruction *instruction)
	{
		InstructionNode *instruction_node = new InstructionNode(instruction);
		errs() << "instruction " << instruction << "\n";
	}

	void OctopusGraph::linkInstructionWithPredecessor(Instruction *previous_instruction, Instruction *current_instruction)
	{
		errs() << "edge " << previous_instruction << " -[FLOWS_TO]-> " << current_instruction << "\n";
	}


	InstructionNode::InstructionNode(Instruction *instruction)
	{
		instruction->dump();
	}

}
