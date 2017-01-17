#include <sstream>
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/ModuleSlotTracker.h"
#include "OctopusGraph.h"

namespace Octopus {

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

	OctopusGraph::node_iterator OctopusGraph::node_begin()
	{
		return nodes.begin();
	}

	OctopusGraph::node_iterator OctopusGraph::node_end()
	{
		return nodes.end();
	}


	InstructionNode* OctopusGraph::createInstructionNode(Instruction *instruction)
	{
		InstructionNode *instruction_node = instruction_map[instruction];
		if (instruction_node == 0) {
			errs() << "instruction node for " << instruction << " does NOT exist!\n";
			instruction_node = new InstructionNode(instruction);
			nodes.push_back(instruction_node);
			instruction_map[instruction] = instruction_node;
		} else {
			errs() << "instruction node for " << instruction << " exists!\n";
		}
		// errs() << "instruction " << instruction << "\n";
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

	Edge::Edge(std::string label, Node *source_node, Node *destination_node) : label(label), source_node(source_node), destination_node(destination_node)
	{
		errs() << "edge " << source_node << " -[" << label << "]-> " << destination_node << "\n";
	}

	InstructionNode::InstructionNode(Instruction *instruction)
	{
		llvm_instruction = instruction;
		instruction->dump();
	}

	std::string InstructionNode::getCode()
	{
		llvm_instruction->print(errs());
		errs() << "\n";

		ModuleSlotTracker MST(llvm_instruction->getModule(),true);
		errs() << "module " << llvm_instruction->getModule() << "\n";
		// MST.getMachine();//->initialize();

		std::ostringstream ost;
		if (llvm_instruction->hasName()) {
			ost << '%' << llvm_instruction->getName().str() << " = ";
		} else if (!llvm_instruction->getType()->isVoidTy()) {
			ost << '%' << MST.getLocalSlot(llvm_instruction) << " = ";
		}
		ost << llvm_instruction->getOpcodeName();
		return ost.str();
		
		// AssemblyAnnotationWriter aw;
		// aw.emitInstructionAnnot(llvm_instruction, errs());
		// this is more or less a re-implementation of
		// AssemblyWriter::printInstruction
		//
		// to get slot, use Machine object
	}

}
