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

	InstructionNode* OctopusGraph::createInstructionNode(Instruction *instruction)
	{
		InstructionNode *instruction_node = instruction_map[instruction];
		if (instruction_node == 0) {
			errs() << "instruction node for " << instruction << " does NOT exist!\n";
			instruction_node = new InstructionNode(instruction);
			instruction_map[instruction] = instruction_node;
		} else {
			errs() << "instruction node for " << instruction << " exists!\n";
		}
		errs() << "instruction " << instruction << "\n";
		return instruction_node;
	}

	void OctopusGraph::linkInstructionWithPredecessor(Instruction *previous_instruction, Instruction *current_instruction)
	{
		if (previous_instruction == 0) return;
		InstructionNode *previous_instruction_node = createInstructionNode(previous_instruction);
		InstructionNode *current_instruction_node = createInstructionNode(current_instruction);
		createEdge("FLOWS_TO",previous_instruction_node,current_instruction_node);
	}

	void OctopusGraph::createEdge(std::string label, Node *source_node, Node *destination_node)
	{
		Edge edge(label, source_node, destination_node);
		edges.push_back(edge);
	}

	Edge::Edge(std::string label, Node *source_node, Node *destination_node) : label(label), source_node(source_node), destination_node(destination_node)
	{
		errs() << "edge " << source_node << " -[" << label << "]-> " << destination_node << "\n";
	}

	InstructionNode::InstructionNode(Instruction *instruction)
	{
		instruction->dump();
	}

}
