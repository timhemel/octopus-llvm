#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constants.h"
#include "InstructionNode.h"
#include "OctopusGraph.h"

namespace Octopus {

	std::string getValueString(const Value *value)
	{
		std::string s;
		raw_string_ostream ost(s);
		value->print(ost);
		return ost.str();
	}

	std::string getValueString(const Type *type)
	{
		std::string s;
		raw_string_ostream ost(s);
		type->print(ost);
		return ost.str();
	}

	InstructionNode::InstructionNode(Instruction *instruction) : Node()
	{
		llvm_instruction = instruction;
		setProperty("code", _getCode());
		setProperty("type", "llvm_ir_instruction");
		setProperty("isCFGNode", "True");
		setProperty("instr", llvm_instruction->getOpcodeName());
		setProperty("operands", ""); // ?
	}

	bool InstructionNode::needsSlot()
	{
		return (!llvm_instruction->hasName() && !llvm_instruction->getType()->isVoidTy());
	}

	std::string InstructionNode::_getCode()
	{
		return getValueString(llvm_instruction);
	}

	IROpcodeNode::IROpcodeNode(Instruction *instruction,int child_num) : Node()
	{
		setProperty("type", "llvm_ir_opcode");
		setProperty("code",instruction->getOpcodeName());
		setProperty("childNum",std::to_string(child_num));
	}

	IROperandNode::IROperandNode(const Value *operand) : Node()
	{
	}

	IROperandNamedVariableNode::IROperandNamedVariableNode(const Value *operand, int child_num) : IROperandNode(operand)
	{
		setProperty("type", "llvm_ir_operand_variable");
		setProperty("code","%" + operand->getName().str());
		// setProperty("code",getValueString(operand));
		setProperty("childNum", std::to_string(child_num));
	}

	IROperandConstantNode::IROperandConstantNode(const Constant *operand, int child_num) : IROperandNode(operand)
	{
		setProperty("type", "llvm_ir_operand_constant");
		setProperty("llvm_type", getValueString(operand->getType()));
		// TODO remove type info from "code" property
		setProperty("code", getValueString(operand));
		setProperty("childNum", std::to_string(child_num));
	}

	IROperandUnnamedVariableNode::IROperandUnnamedVariableNode(OctopusGraph *octopus_graph, const Value *operand, int child_num) : IROperandNode(operand)
	{
		setProperty("type", "llvm_ir_operand_variable_unnamed");
		// setProperty("code","%" + operand->getName().str());
		// setProperty("code",getValueString(operand));
		// get slot number from moduletracker
		int slot = octopus_graph->getSlotTracker().getSlotIndex(operand);
		setProperty("code", "%" + std::to_string(slot));
		setProperty("childNum", std::to_string(child_num));
	}

}

