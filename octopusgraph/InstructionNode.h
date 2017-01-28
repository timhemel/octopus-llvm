#ifndef INSTRUCTION_NODE_H
#define INSTRUCTION_NODE_H

#include <ostream>
#include "llvm/IR/Function.h"
#include "OctopusNode.h"

namespace Octopus {

	class OctopusGraph;

	// TODO: make template function?
	std::string getValueString(const Value *value);
	std::string getValueString(const Type *type);

	class InstructionNode : public Node {
	public:
		InstructionNode(Instruction *instruction);
		Instruction *getLLVMInstruction() { return llvm_instruction; }
		virtual bool isInstruction() { return true; }
		bool needsSlot();

	private:
		virtual std::string _getCode();

		Instruction *llvm_instruction;
	};

	class IROpcodeNode : public Node {
	public:
		IROpcodeNode(Instruction *instruction);
	};

	class IROperandNode : public Node {
	public:
		IROperandNode(const Value *operand);
	};

	class IROperandNamedVariableNode : public IROperandNode {
	public:
		IROperandNamedVariableNode(const Value *operand, int operand_no);
	};

	class IROperandConstantNode : public IROperandNode {
	public:
		IROperandConstantNode(const Constant *operand, int operand_no);
	};

	class IROperandUnnamedVariableNode : public IROperandNode {
	public:
		IROperandUnnamedVariableNode(OctopusGraph *octopus_graph, const Value *operand, int operand_no);
	};

}

#endif
