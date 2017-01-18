#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/CFG.h"
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
		CFGEntryNode *cfg_entry_node = new CFGEntryNode(&F);
		entry_nodes_map[&F] = cfg_entry_node;
		nodes.push_back(cfg_entry_node);

		CFGExitNode *cfg_exit_node = new CFGExitNode(&F);
		exit_nodes_map[&F] = cfg_exit_node;
		nodes.push_back(cfg_exit_node);
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
			findOrCreateInstructionNode(current_instruction);
			linkInstructionWithPredecessor(prev_instruction,current_instruction);
			prev_instruction = current_instruction;
		}
	}

	void OctopusGraph::linkBasicBlock(BasicBlock &B)
	{
		if (pred_begin(&B) == pred_end(&B)) {
			CFGEntryNode *source_node = entry_nodes_map[B.getParent()];
			InstructionNode *destination_node = findOrCreateInstructionNode(&B.front());
			createEdge("BB_TO",source_node,destination_node);
		}
		for (pred_iterator PI = pred_begin(&B), E = pred_end(&B); PI != E; ++PI) {
			BasicBlock &Pred = **PI;
			linkBasicBlockInstructions(Pred,B);
		}
		// if no successors, link with CFGExit
		if (succ_begin(&B) == succ_end(&B)) {
			InstructionNode *source_node = findOrCreateInstructionNode(&B.back());
			CFGExitNode *destination_node = exit_nodes_map[B.getParent()];
			createEdge("BB_TO",source_node,destination_node);
		}
		for (succ_iterator PI = succ_begin(&B), E = succ_end(&B); PI != E; ++PI) {
			BasicBlock &Succ = **PI;
			linkBasicBlockInstructions(B,Succ);
		}
	}

	void OctopusGraph::linkBasicBlockInstructions(BasicBlock &source_block, BasicBlock &destination_block)
	{
		InstructionNode *source_instruction_node = findOrCreateInstructionNode(&source_block.back());
		InstructionNode *destination_instruction_node = findOrCreateInstructionNode(&destination_block.front());
		createEdge("BB_TO",source_instruction_node,destination_instruction_node);
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


	InstructionNode* OctopusGraph::findOrCreateInstructionNode(Instruction *instruction)
	{
		InstructionNode *instruction_node = instruction_map[instruction];
		if (instruction_node == 0) {
			instruction_node = createInstructionNode(instruction);
			nodes.push_back(instruction_node);
			instruction_map[instruction] = instruction_node;
			updateSlotMap(instruction_node);
		}
		return instruction_node;
	}

	void OctopusGraph::linkInstructionWithPredecessor(Instruction *previous_instruction, Instruction *current_instruction)
	{
		if (previous_instruction == 0) return;
		InstructionNode *previous_instruction_node = findOrCreateInstructionNode(previous_instruction);
		InstructionNode *current_instruction_node = findOrCreateInstructionNode(current_instruction);
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

	InstructionNode *OctopusGraph::createInstructionNode(Instruction *instruction)
	{
		return new InstructionNode(*this,instruction);
	}

	Edge::Edge(std::string label, Node *source_node, Node *destination_node) : label(label), source_node(source_node), destination_node(destination_node)
	{
	}

}
