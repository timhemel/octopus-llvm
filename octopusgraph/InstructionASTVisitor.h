#ifndef GRAPHBUILDER_VISITOR_H
#define GRAPHBUILDER_VISITOR_H

#include <stack>

#include "llvm/IR/InstVisitor.h"
#include "llvm/IR/Constants.h"
#include "InstructionNode.h"

using namespace llvm;

namespace Octopus {

	class OctopusGraph;

	class InstructionASTVisitor {
	public:
		void visit(Instruction &instruction);
		void visit(Instruction *instruction) { visit(*instruction); }

		virtual void preVisitInstruction(Instruction &instruction) { }
		virtual void postVisitInstruction(Instruction &instruction) { }

		virtual void visitInstructionOpcode(Instruction &instruction) { }

		virtual void visitNullOperand() { }
		virtual void visitNamedOperand(const Value *operand) { }
		virtual void visitConstant(const Constant *constant) { }
		virtual void visitConstantInt(const ConstantInt *constant_int) { return visitConstant(constant_int); }
		virtual void visitConstantFloat(const ConstantFP *constant_float) { return visitConstant(constant_float); }
		virtual void visitConstantAggregateZero(const Constant *constant) { return visitConstant(constant); }
		virtual void visitBlockAddress(const BlockAddress *block_address) { return visitConstant(block_address); }
		virtual void visitConstantArray(const ConstantArray *constant_array) { return visitConstant(constant_array); }
		virtual void visitConstantDataArray(const ConstantDataArray *constant_data_array) { return visitConstant(constant_data_array); }
		virtual void visitConstantStruct(const ConstantStruct *constant_struct) { return visitConstant(constant_struct); }
		virtual void visitConstantVector(const Constant *constant) { return visitConstant(constant); }
		virtual void visitConstantDataVector(const Constant *constant) { return visitConstant(constant); }
		virtual void visitConstantPointerNull(const Constant *constant) { return visitConstant(constant); }
		virtual void visitConstantTokenNone(const Constant *constant) { return visitConstant(constant); }
		virtual void visitConstantExpr(const ConstantExpr *constant_expr) { return visitConstant(constant_expr); }

		virtual void visitLocalValue(const Value *operand) { }

	protected:
		virtual void _visitInstruction(Instruction &instruction);
		virtual void _visitOperands(Instruction &instruction);
		virtual void _visitOperand(const Value *operand);
		virtual void _visitConstant(const Constant *constant);
		Node * getParentNode() { return node_stack.top(); }
		int getChildNum() { return current_child_num; }
		std::stack<Node *> node_stack;
		std::stack<int> child_num_stack;
		int current_child_num;
	};


	class IRASTBuilderVisitor : public InstructionASTVisitor {
	public:
		IRASTBuilderVisitor(OctopusGraph *graph) : octopus_graph(graph) { }

		void build(InstructionNode *instruction_node);

		virtual void preVisitInstruction(Instruction &instruction);
		virtual void postVisitInstruction(Instruction &instruction);
		virtual void visitInstructionOpcode(Instruction &instruction);

		virtual void visitNamedOperand(const Value *operand);
		virtual void visitConstant(const Constant *constant);

		virtual void visitLocalValue(const Value *operand);

	private:
		OctopusGraph *octopus_graph;
		InstructionNode *instruction_node;
	};

}


#endif
