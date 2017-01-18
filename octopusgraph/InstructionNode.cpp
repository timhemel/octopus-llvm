#include <sstream>
#include "InstructionNode.h"
#include "OctopusGraph.h"

namespace Octopus {

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

