#include <sstream>
#include "llvm/Support/raw_ostream.h"
#include "OctopusGraph.h"

namespace Octopus {

	void OctopusGraph::initializeFunction()
	{
		slot_tracker.reset();
	}

	void OctopusGraph::finalizeFunction()
	{
	}


	void OctopusGraph::createEntryAndExitNodesForFunction(Function &F)
	{
	}

	void OctopusGraph::addBlockLabel(BasicBlock &B)
	{
		slot_tracker.add(&B);
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

	OctopusGraph::node_iterator OctopusGraph::node_begin()
	{
		return nodes.begin();
	}

	OctopusGraph::node_iterator OctopusGraph::node_end()
	{
		return nodes.end();
	}

	OctopusGraph::edge_iterator OctopusGraph::edge_begin()
	{
		return edges.begin();
	}

	OctopusGraph::edge_iterator OctopusGraph::edge_end()
	{
		return edges.end();
	}


	InstructionNode* OctopusGraph::createInstructionNode(Instruction *instruction)
	{
		InstructionNode *instruction_node = instruction_map[instruction];
		if (instruction_node == 0) {
			// TODO: replace with factory method
			instruction_node = new InstructionNode(*this,instruction);
			nodes.push_back(instruction_node);
			instruction_map[instruction] = instruction_node;
			updateSlotMap(instruction_node);
		}
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
		edges.insert(edge);
	}

	void OctopusGraph::updateSlotMap(InstructionNode *instruction_node)
	{
		if (instruction_node->needsSlot()) {
			slot_tracker.add(instruction_node->getLLVMInstruction());
		}
	}

	Edge::Edge(std::string label, Node *source_node, Node *destination_node) : label(label), source_node(source_node), destination_node(destination_node)
	{
	}

	InstructionNode::InstructionNode(OctopusGraph &ograph, Instruction *instruction) : octopus_graph(ograph)
	{
		llvm_instruction = instruction;
	}

	std::string InstructionNode::getCode()
	{
		std::ostringstream ost;

		renderLHS(ost);
		renderOpcode(ost);
		renderOperands(ost);

		return ost.str();
	}

	bool InstructionNode::needsSlot()
	{
		return (!llvm_instruction->hasName() && !llvm_instruction->getType()->isVoidTy());
	}

	void InstructionNode::renderLHS(std::ostream &ost)
	{
		if (llvm_instruction->hasName()) {
			ost << '%' << llvm_instruction->getName().str() << " = ";
		} else if (!llvm_instruction->getType()->isVoidTy()) {
			int slot = octopus_graph.slot_tracker.getSlotIndex(llvm_instruction);
			ost << '%' << slot << " = ";
		}
	}

	void InstructionNode::renderOpcode(std::ostream &ost)
	{
		ost << llvm_instruction->getOpcodeName() << ' ';
	}

	void InstructionNode::renderOperands(std::ostream &ost)
	{
		for (unsigned i=0, E = llvm_instruction->getNumOperands(); i != E; ++i) {
			if (i) ost << ", ";
			Value *op = llvm_instruction->getOperand(i);
			if (op->hasName()) {
				ost << '%' << op->getName().str();
			} else {
				int slot = octopus_graph.slot_tracker.getSlotIndex(op);
				if (slot != 0) {
					ost << '%' << slot;
				} else {
					ost << op;
				}
			}
		}
	}

}
