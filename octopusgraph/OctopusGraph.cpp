#include "OctopusGraph.h"

namespace OctopusGraph {

	void OctopusGraph::createEntryAndExitNodesForFunction(Function &F)
	{
	}

	void OctopusGraph::createAndConnectInstructionNodesForBasicBlock(BasicBlock &B)
	{
		// Instruction &prev_instruction = dyn_cast<Instruction>(*B.begin());
		Instruction *prev_instruction = 0;
		for(BasicBlock::iterator i = B.begin(), ie = B.end(); i != ie; ++i) {
			// create instruction node
			createInstructionNode(*i);
			// link instruction node with predecessor
			linkInstructionWithPredecessor(prev_instruction,*i);
			prev_instruction = dyn_cast<Instruction>(&*i);
		}
	}

	void OctopusGraph::linkBasicBlock(BasicBlock &B)
	{
	}

	void OctopusGraph::createInstructionNode(Instruction &I)
	{
		I.dump();
	}

	void OctopusGraph::linkInstructionWithPredecessor(Instruction *prev_instruction, Instruction &current_instruction)
	{
	}

}
