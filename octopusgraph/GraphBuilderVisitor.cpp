#include "llvm/Support/raw_ostream.h"

#include "OctopusGraph.h"
#include "GraphBuilderVisitor.h"

namespace Octopus {

	void InstructionASTVisitor::visit(Instruction &instruction)
	{
		_visitInstruction(instruction);
	}

	void InstructionASTVisitor::_visitInstruction(Instruction &instruction)
	{
		current_child_num = 0;
		preVisitInstruction(instruction);
		visitInstructionOpcode(instruction);
		current_child_num++;
		_visitOperands(instruction);
		postVisitInstruction(instruction);
	}


	void InstructionASTVisitor::_visitOperands(Instruction &instruction)
	{

		for(int operand_no = 0, E = instruction.getNumOperands(); operand_no != E; ++operand_no) {
			const Value *operand = instruction.getOperand(operand_no);
			child_num_stack.push(current_child_num);
			_visitOperand(operand);
			current_child_num = child_num_stack.top();
			current_child_num++;
			child_num_stack.pop();
		}
	}

	void InstructionASTVisitor::_visitOperand(const Value *operand)
	{
		errs() << "OPERAND: " << *operand << " num " << current_child_num << "\n";
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
		const InlineAsm *inline_asm = dyn_cast<InlineAsm>(operand);
		if (inline_asm) {
			// TODO: return visitiInlineAsm(...)
			return;
		}
		auto *meta_data = dyn_cast<MetadataAsValue>(operand);
		if (meta_data) {
			// TODO: metadata
			return;
		}
		// unnamed variable
		const GlobalValue *global_value = dyn_cast<GlobalValue>(operand);
		if (global_value) {
			// TODO: global value
			return;
		}
		// local value
		visitLocalValue(operand);
	}

	void InstructionASTVisitor::_visitConstant(const Constant *constant)
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

	void IRASTBuilderVisitor::build(InstructionNode *instr_node)
	{
		instruction_node = instr_node;
		visit(instruction_node->getLLVMInstruction());
	}


	void IRASTBuilderVisitor::preVisitInstruction(Instruction &instruction)
	{
		node_stack.push(instruction_node);
	}

	void IRASTBuilderVisitor::postVisitInstruction(Instruction &instruction)
	{
		node_stack.pop();
	}

	void IRASTBuilderVisitor::visitInstructionOpcode(Instruction &instruction)
	{
		errs() << "OPCODE: " << instruction.getOpcodeName() << "\n";
		IROpcodeNode *opcode_node = new IROpcodeNode(&instruction, getChildNum());
		// connect with parent
		octopus_graph->storeNode(opcode_node);
		octopus_graph->createAndStoreEdge("IS_AST_PARENT", getParentNode(), opcode_node);
	}

	void IRASTBuilderVisitor::visitNamedOperand(const Value *operand)
	{
		IROperandNamedVariableNode *named_operand_node = new IROperandNamedVariableNode(operand, getChildNum());
		octopus_graph->storeNode(named_operand_node);
		octopus_graph->createAndStoreEdge("IS_AST_PARENT", getParentNode(), named_operand_node);
	}

	void IRASTBuilderVisitor::visitConstant(const Constant *constant)
	{
		IROperandConstantNode *constant_node = new IROperandConstantNode(constant, getChildNum());
		octopus_graph->storeNode(constant_node);
		octopus_graph->createAndStoreEdge("IS_AST_PARENT", getParentNode(), constant_node);
	}

	void IRASTBuilderVisitor::visitLocalValue(const Value *operand)
	{
		IROperandUnnamedVariableNode *unnamed_variable_node = new IROperandUnnamedVariableNode(octopus_graph, operand, getChildNum());
		octopus_graph->storeNode(unnamed_variable_node);
		octopus_graph->createAndStoreEdge("IS_AST_PARENT", getParentNode(), unnamed_variable_node);
	}

}
