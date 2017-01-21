#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constants.h"
#include "InstructionNode.h"
#include "OctopusGraph.h"

namespace Octopus {

	InstructionNode::InstructionNode(OctopusGraph &ograph, Instruction *instruction) : Node(), octopus_graph(ograph)
	{
		llvm_instruction = instruction;
		setProperty("code", _getCode());
		setProperty("isCFGNode", "True");
		setProperty("type", ""); // ?
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

}

