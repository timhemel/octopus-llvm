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
		IROpcodeNode(Instruction *instruction, int child_num);
	};

	class IROperandNode : public Node {
	public:
		IROperandNode(const Value *operand);
	};

	class IRLocalNamedVariableNode : public IROperandNode {
	public:
		IRLocalNamedVariableNode(const Value *operand, int child_num);
	};

	class IRGlobalNamedVariableNode : public IROperandNode {
	public:
		IRGlobalNamedVariableNode(const Value *operand, int child_num);
	};

	class IROperandConstantNode : public IROperandNode {
	public:
		IROperandConstantNode(const Constant *operand, int child_num);
	};

	class IROperandUnnamedVariableNode : public IROperandNode {
	public:
		IROperandUnnamedVariableNode(OctopusGraph *octopus_graph, const Value *operand, int child_num);
	};

	class IRASTLabelNode : public IROperandNode {
	public:
		IRASTLabelNode(OctopusGraph *octopus_graph, const BasicBlock *basic_block, int child_num);
	};

}

#endif
