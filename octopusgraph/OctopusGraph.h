#include <map>
#include <list>
#include <set>
#include <string>
#include <ostream>
#include "llvm/IR/Function.h"
#include "llvm/IR/ModuleSlotTracker.h"

using namespace llvm;

namespace Octopus {

	class OctopusGraph;

	class Node {
	public:
		virtual std::string getCode() = 0;
	};

	struct Edge {
		Edge(std::string label, Node *source_node, Node *destination_node);
		std::string label;
		Node *source_node;
		Node *destination_node;
	};

	struct edge_compare {
		bool operator()(const Edge& e1, const Edge& e2) const {
			return e1.label == e2.label
				? ( e1.source_node == e2.source_node
					? e1.destination_node < e1.destination_node
					: e1.source_node < e2.source_node )
				: e1.label < e2.label;
		}
	};

	class InstructionNode : public Node {
	public:
		InstructionNode(OctopusGraph &ograph, Instruction *instruction);
		virtual std::string getCode();
		Instruction *getLLVMInstruction() { return llvm_instruction; }
		virtual bool needsSlot();

	protected:

		virtual void renderLHS(std::ostream &ost);
		virtual void renderOpcode(std::ostream &ost);
		virtual void renderOperands(std::ostream &ost);
	private:
		OctopusGraph &octopus_graph;
		Instruction *llvm_instruction;
		std::string code;
	};

	class SlotTracker {
	public:
		SlotTracker() : slot_count(0) {}
		void add(Value *v) { slot_map[v] = slot_count++; }
		int getSlotIndex(Value *v) { return slot_map[v]; }
		void reset() { slot_count = 0; }
	private:
		int slot_count;
		std::map<Value *,int> slot_map;
	};

	// lookup tables for Instruction -> InstructionNode
	// lookup tables for DIInfo -> LocationNode
	class OctopusGraph {
	public:
		void initializeFunction();
		void finalizeFunction();
		void createEntryAndExitNodesForFunction(Function &F);
		void addBlockLabel(BasicBlock &B);
		void createAndConnectInstructionNodesForBasicBlock(BasicBlock &B);
		void linkBasicBlock(BasicBlock &B);

		typedef std::list<Node *>::iterator node_iterator;
		node_iterator node_begin();
		node_iterator node_end();

		typedef std::set<Edge,edge_compare>::iterator edge_iterator;
		edge_iterator edge_begin();
		edge_iterator edge_end();

		SlotTracker slot_tracker;

	private:
		InstructionNode* createInstructionNode(Instruction *instruction);
		void linkInstructionWithPredecessor(Instruction *previous_instruction, Instruction *current_instruction);

		void createEdge(std::string label, Node *source_node, Node *destination_node);
		void updateSlotMap(InstructionNode *instruction);

		std::map<Instruction *,InstructionNode *> instruction_map;
		std::list<Node*> nodes;
		std::set<Edge,edge_compare> edges;
	};

}

