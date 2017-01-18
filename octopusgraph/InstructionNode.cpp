#include <sstream>
#include "llvm/IR/Constants.h"
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
		if (llvm_instruction->getNumOperands() > 0) ost << ' ';
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
		ost << llvm_instruction->getOpcodeName();
	}

	void InstructionNode::renderConstant(std::ostream &ost, const Constant *CV)
	{
		if (const ConstantInt *CI = dyn_cast<ConstantInt>(CV)) {
			if (CI->getType()->isIntegerTy(1)) {
				ost << (CI->getZExtValue() ? "true" : "false");
				return;
			}
			ost << CI->getValue().toString(10,true);
			return;
		}
		if (const ConstantFP *CFP = dyn_cast<ConstantFP>(CV)) {
			ost << CFP->getValueAPF().convertToDouble();
			return;
		}
		if (isa<ConstantAggregateZero>(CV)) {
			ost << "zeroinitializer";
			return;
		}
		// block address
		// constant array
		// constant data array
		// constant struct
		// constant vector
		// constant pointer null
		// constant token none
		// undef value
		// constant expr
	}

	void InstructionNode::renderOperand(std::ostream &ost, const Value *OP)
	{
		if (OP->hasName()) {
			ost << '%' << OP->getName().str();
			return;
		}
		const Constant *CV = dyn_cast<Constant>(OP);
		if (CV && !isa<GlobalValue>(CV)) {
			renderConstant(ost, CV);
			return;
		}
		// inline asm
		// metadataasvalue
		// otherwise, slot tracker
		int slot = octopus_graph.slot_tracker.getSlotIndex(OP);
		if (slot != 0) {
			ost << '%' << slot;
		} else {
			ost << OP;
		}
	}

	void InstructionNode::renderOperands(std::ostream &ost)
	{
		for (unsigned i=0, E = llvm_instruction->getNumOperands(); i != E; ++i) {
			if (i) ost << ", ";
			renderOperand(ost,llvm_instruction->getOperand(i));
/*
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
*/
		}
	}

}

