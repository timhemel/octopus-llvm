#include <map>
#include <list>
#include <set>
#include <string>
#include "llvm/IR/Function.h"

using namespace llvm;

namespace Octopus {

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
		InstructionNode(Instruction *instruction);
		virtual std::string getCode();
	private:
		Instruction *llvm_instruction;
		std::string code;
	};

	// lookup tables for Instruction -> InstructionNode
	// lookup tables for DIInfo -> LocationNode
	class OctopusGraph {
	public:
		void createEntryAndExitNodesForFunction(Function &F);
		void createAndConnectInstructionNodesForBasicBlock(BasicBlock &B);
		void linkBasicBlock(BasicBlock &B);

		typedef std::list<Node *>::iterator node_iterator;
		node_iterator node_begin();
		node_iterator node_end();

	private:
		InstructionNode* createInstructionNode(Instruction *instruction);
		void linkInstructionWithPredecessor(Instruction *previous_instruction, Instruction *current_instruction);

		void createEdge(std::string label, Node *source_node, Node *destination_node);

		std::map<Instruction *,InstructionNode *> instruction_map;
		std::list<Node*> nodes;
		std::set<Edge,edge_compare> edges;
	};

}

