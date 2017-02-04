#include "llvm/Support/raw_ostream.h"

#include "OctopusGraph.h"
#include "InstructionASTVisitor.h"

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
		errs() << "OPERAND: " << *operand << " childnum " << current_child_num << "\n";
		if (!operand) {
			return visitNullOperand();
		}
		const BasicBlock *basic_block = dyn_cast<BasicBlock>(operand);
		if (basic_block) {
			errs() << "LABEL!\n";
			return visitLabel(basic_block);
		}

		if (operand->hasName()) {
			if (isa<GlobalValue>(operand)) {
				return visitGlobalNamedOperand(operand);
			}
			return visitLocalNamedOperand(operand);
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

	void InstructionASTVisitor::visitConstantArray(const ConstantArray *constant_array)
	{
		errs() << "CONSTANTARRAY\n";
		return visitConstant(constant_array);
	}

	void InstructionASTVisitor::visitConstantDataArray(const ConstantDataArray *constant_data_array)
	{
		errs() << "CONSTANTDATAARRAY\n";
		return visitConstant(constant_data_array);
	}

	void InstructionASTVisitor::visitConstantStruct(const ConstantStruct *constant_struct)
	{
		errs() << "CONSTANTSTRUCT\n";
		return visitConstant(constant_struct);
	}

	void InstructionASTVisitor::visitConstantVector(const Constant *constant)
	{
		errs() << "CONSTANTVECTOR\n";
		return visitConstant(constant);
	}

	void InstructionASTVisitor::visitConstantDataVector(const Constant *constant)
	{
		errs() << "CONSTANTDATAVECTOR\n";
		return visitConstant(constant);
	}

	void IRASTBuilderVisitor::build(InstructionNode *instr_node)
	{
		instruction_node = instr_node;
		visit(instruction_node->getLLVMInstruction());
	}


	void IRASTBuilderVisitor::preVisitInstruction(Instruction &instruction)
	{
		errs() << "INSTR:" << instruction << "\n";
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

	void IRASTBuilderVisitor::visitLocalNamedOperand(const Value *operand)
	{
		IRLocalNamedVariableNode *named_operand_node = new IRLocalNamedVariableNode(operand, getChildNum());
		octopus_graph->storeNode(named_operand_node);
		octopus_graph->createAndStoreEdge("IS_AST_PARENT", getParentNode(), named_operand_node);
	}

	void IRASTBuilderVisitor::visitGlobalNamedOperand(const Value *operand)
	{
		IRGlobalNamedVariableNode *named_operand_node = new IRGlobalNamedVariableNode(operand, getChildNum());
		octopus_graph->storeNode(named_operand_node);
		octopus_graph->createAndStoreEdge("IS_AST_PARENT", getParentNode(), named_operand_node);
	}

	void IRASTBuilderVisitor::visitConstant(const Constant *constant)
	{
		IROperandConstantNode *constant_node = new IROperandConstantNode(constant, getChildNum());
		octopus_graph->storeNode(constant_node);
		octopus_graph->createAndStoreEdge("IS_AST_PARENT", getParentNode(), constant_node);
	}

	void IRASTBuilderVisitor::visitLabel(const BasicBlock *basic_block)
	{
		errs() << "create label node\n";
		IRASTLabelNode *label_node = new IRASTLabelNode(octopus_graph, basic_block, getChildNum());
		octopus_graph->storeNode(label_node);
		octopus_graph->createAndStoreEdge("IS_AST_PARENT", getParentNode(), label_node);
	}

	void IRASTBuilderVisitor::visitLocalValue(const Value *operand)
	{
		// if label?
		IROperandUnnamedVariableNode *unnamed_variable_node = new IROperandUnnamedVariableNode(octopus_graph, operand, getChildNum());
		octopus_graph->storeNode(unnamed_variable_node);
		octopus_graph->createAndStoreEdge("IS_AST_PARENT", getParentNode(), unnamed_variable_node);
	}

}
