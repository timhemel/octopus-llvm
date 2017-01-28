#include "llvm/Support/raw_ostream.h"

#include "OctopusGraph.h"
#include "GraphBuilderVisitor.h"

namespace Octopus {

	void GraphBuilderVisitor::_visitOperands(InstructionNode *instruction_node, Instruction &instruction)
	{

		node_stack.push(instruction_node);
		errs() << "OPERANDS\n";
		for(int operand_no = 0, E = instruction.getNumOperands(); operand_no != E; ++operand_no) {
			const Value *operand = instruction.getOperand(operand_no);
			_visitOperand(operand);
		}
		node_stack.pop();
	}

	void GraphBuilderVisitor::_visitOperand(const Value *operand)
	{
		errs() << "OPERAND: " << *operand << "\n";
		if (!operand) {
			return visitNullOperand();
		}
		if (operand->hasName()) {
			return visitNamedOperand(operand);
		}
		const Constant *constant_value = dyn_cast<Constant>(operand);
		if (constant_value && !isa<GlobalValue>(constant_value)) {
			return visitConstant(constant_value);
		}
	}

	void GraphBuilderVisitor::_visitConstant(const Constant *constant)
	{
		errs() << "CONSTANT: " << *constant << "\n";
		const ConstantInt *constant_int = dyn_cast<ConstantInt>(constant);
		if (constant_int) {
			return visitConstantInt(constant_int);
		}
		const ConstantFP *constant_float = dyn_cast<ConstantFP>(constant);
		if (constant_float) {
			return visitConstantFloat(constant_float);
		}
		if (isa<ConstantAggregateZero>(constant)) {
			return visitConstantAggregateZero(constant); // zero initializer
		}
		const BlockAddress *block_address = dyn_cast<BlockAddress>(constant);
		if (block_address) {
			return visitBlockAddress(block_address);
		}
		const ConstantArray *constant_array = dyn_cast<ConstantArray>(constant);
		if (constant_array) {
			return visitConstantArray(constant_array);
		}
		const ConstantDataArray *constant_data_array = dyn_cast<ConstantDataArray>(constant);
		if (constant_data_array) {
			return visitConstantDataArray(constant_data_array);
		}
		const ConstantStruct *constant_struct = dyn_cast<ConstantStruct>(constant);
		if (constant_struct) {
			return visitConstantStruct(constant_struct);
		}
		if (isa<ConstantVector>(constant)) {
			return visitConstantVector(constant);
		}
		if (isa<ConstantDataVector>(constant)) {
			return visitConstantDataVector(constant);
		}
		if (isa<ConstantPointerNull>(constant)) {
			return visitConstantPointerNull(constant);
		}
		if (isa<ConstantTokenNone>(constant)) {
			return visitConstantTokenNone(constant);
		}
		const ConstantExpr *constant_expr = dyn_cast<ConstantExpr>(constant);
		if (constant_expr) {
			return visitConstantExpr(constant_expr);
		}
		// placeholder or erroneous constant
	}

	void InstructionBuilderVisitor::visitInstruction(Instruction &instruction)
	{
		errs() << "visiting:" << instruction << "\n";
		instruction_node = new InstructionNode(&instruction);
	}

	void InstructionBuilderVisitor::visitReturnInst(ReturnInst &instruction)
	{
		errs() << "RETURN!" << instruction << "\n";
		instruction_node = new InstructionNode(&instruction);
	}

	void InstructionBuilderVisitor::visitBinaryOperator(BinaryOperator &instruction)
	{
		errs() << "BINOP!" << instruction << "\n";
		instruction_node = new InstructionNode(&instruction);
		_visitOperands(instruction_node,instruction);
	}

	void InstructionBuilderVisitor::visitUnaryInstruction(UnaryInstruction &instruction)
	{
		errs() << "UNARY INS!" << instruction << "\n";
		instruction_node = new InstructionNode(&instruction);
		_visitOperands(instruction_node,instruction);
	}

	// void InstructionBuilderVisitor::visitConstant

	void InstructionBuilderVisitor::visitNamedOperand(const Value *operand)
	{
		IROperandNamedVariableNode *named_operand_node = new IROperandNamedVariableNode(operand);
		octopus_graph->storeNode(named_operand_node);
		octopus_graph->createAndStoreEdge("IS_AST_PARENT", getParentNode(), named_operand_node);
	}

	void InstructionBuilderVisitor::visitConstant(const Constant *constant)
	{
		IROperandConstantNode *constant_node = new IROperandConstantNode(constant);
		octopus_graph->storeNode(constant_node);
		octopus_graph->createAndStoreEdge("IS_AST_PARENT", getParentNode(), constant_node);
	}

}
