#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/DebugInfoMetadata.h"
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
			createAndStoreEdge("BB_TO",source_node,destination_node);
		}
		for (pred_iterator PI = pred_begin(&B), E = pred_end(&B); PI != E; ++PI) {
			BasicBlock &Pred = **PI;
			linkBasicBlockInstructions(Pred,B);
		}
		if (succ_begin(&B) == succ_end(&B)) {
			InstructionNode *source_node = findOrCreateInstructionNode(&B.back());
			CFGExitNode *destination_node = exit_nodes_map[B.getParent()];
			createAndStoreEdge("BB_TO",source_node,destination_node);
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
		createAndStoreEdge("BB_TO",source_instruction_node,destination_instruction_node);
	}

	bool isInstructionNode(Node *n)
	{
		return n->isInstruction();
	}

	void OctopusGraph::createDataDependenceEdges()
	{
		node_iterator node = find_if(node_begin(), node_end(), isInstructionNode);
		while (node != node_end()) {
			InstructionNode *instruction_node = (InstructionNode *) *node;
			Instruction *llvm_instruction = ((InstructionNode *) *node)->getLLVMInstruction();
			for(int operand_no = 0, E = llvm_instruction->getNumOperands(); operand_no != E; ++operand_no) {
				const Value *operand = llvm_instruction->getOperand(operand_no);
				if (isa<Instruction>(operand)) {
					const Instruction *ins = (const Instruction *) operand;
					InstructionNode *defining_node = instruction_map[ins];
					Edge *e = createAndStoreEdge("USE",*node, defining_node);
					e->setProperty("operand_no",std::to_string(operand_no));
				}
			}
			node = find_if(++node, node_end(), isInstructionNode);
		}
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
			LocationNode *location_node = findOrCreateLocationAndFileNodes(instruction_node);
			if (location_node != 0) {
				createAndStoreEdge("LOCATED_AT",instruction_node,location_node);
			}
		}
		return instruction_node;
	}

	void OctopusGraph::linkInstructionWithPredecessor(Instruction *previous_instruction, Instruction *current_instruction)
	{
		if (previous_instruction == 0) return;
		InstructionNode *previous_instruction_node = findOrCreateInstructionNode(previous_instruction);
		InstructionNode *current_instruction_node = findOrCreateInstructionNode(current_instruction);
		createAndStoreEdge("FLOWS_TO",previous_instruction_node,current_instruction_node);
	}

	Edge* OctopusGraph::createAndStoreEdge(std::string label, Node *source_node, Node *destination_node)
	{
		Edge *edge = new Edge(label, source_node, destination_node);
		edges.insert(edge);
		return edge;
	}

	void OctopusGraph::updateSlotMap(InstructionNode *instruction_node)
	{
		if (instruction_node->needsSlot()) {
			slot_tracker.add(instruction_node->getLLVMInstruction());
		}
	}

	LocationNode* OctopusGraph::findOrCreateLocationAndFileNodes(InstructionNode *instruction)
	{
		Instruction *llvm_instruction = instruction->getLLVMInstruction();
		if (llvm_instruction->hasMetadata()) {
			MDNode *metadata = llvm_instruction->getMetadata("dbg");
			DILocation *location = (DILocation *) metadata;
			FileNode *file_node = findOrCreateFileNode(location);
			LocationNode *location_node = findOrCreateLocationNode(location);
			createAndStoreEdge("IN_FILE",location_node,file_node);
			return location_node;
		}
		return 0;
	}

	FileNode* OctopusGraph::findOrCreateFileNode(DILocation *location)
	{
		// mapping of location to filenode
		FileNode *file_node = file_map[location->getFilename().str()];
		if (file_node == 0) {
			file_node = new FileNode(location->getFilename().str());
			nodes.push_back(file_node);
			file_map[location->getFilename().str()] = file_node;
		}
		return file_node;
	}

	LocationNode* OctopusGraph::findOrCreateLocationNode(DILocation *location)
	{
		LocationNode *location_node = location_map[location];
		if (location_node == 0) {
			location_node = new LocationNode(location);
			nodes.push_back(location_node);
			location_map[location] = location_node;
		}
		return location_node;
	}

	InstructionNode *OctopusGraph::createInstructionNode(Instruction *instruction)
	{
		return new InstructionNode(*this,instruction);
	}

	Edge::Edge(std::string label, Node *source_node, Node *destination_node) : label(label), source_node(source_node), destination_node(destination_node)
	{
	}

	void Edge::setProperty(std::string key, std::string value)
	{
		properties[key] = value;
	}


}
