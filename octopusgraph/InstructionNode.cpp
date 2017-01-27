#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constants.h"
#include "InstructionNode.h"
#include "OctopusGraph.h"

namespace Octopus {

	InstructionNode::InstructionNode(Instruction *instruction) : Node()
	{
		llvm_instruction = instruction;
		setProperty("code", _getCode());
		setProperty("type", "llvm_ir_instruction");
		setProperty("isCFGNode", "True");
		setProperty("instr", llvm_instruction->getOpcodeName());
		setProperty("operands", ""); // ?
	}

	std::string InstructionNode::_getCode()
	{
		std::string s;
		raw_string_ostream ost(s);
		llvm_instruction->print(ost);
		return ost.str();
	}

	IROpcodeNode::IROpcodeNode(Instruction *instruction) : Node()
	{
		setProperty("type", "llvm_ir_opcode");
		setProperty("code",instruction->getOpcodeName());
	}

	IROperandNode::IROperandNode(const Value *operand) : Node()
	{
	}

	IROperandNamedVariableNode::IROperandNamedVariableNode(const Value *operand) : IROperandNode(operand)
	{
		setProperty("type", "llvm_ir_operand_variable");
		setProperty("code","%" + operand->getName().str());
	}

}

