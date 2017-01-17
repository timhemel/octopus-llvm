#include <map>
#include <list>
#include "llvm/IR/Function.h"

using namespace llvm;

namespace OctopusGraph {

	class Node {
	};

	struct Edge {
		Edge(std::string label, Node *source_node, Node *destination_node);
		std::string label;
		Node *source_node;
		Node *destination_node;
	};

	class InstructionNode : public Node {
	public:
		InstructionNode(Instruction *instruction);
	};

	// lookup tables for Instruction -> InstructionNode
	// lookup tables for DIInfo -> LocationNode
	class OctopusGraph {
	public:
		void createEntryAndExitNodesForFunction(Function &F);
		void createAndConnectInstructionNodesForBasicBlock(BasicBlock &B);
		void linkBasicBlock(BasicBlock &B);

	private:
		InstructionNode* createInstructionNode(Instruction *instruction);
		void linkInstructionWithPredecessor(Instruction *previous_instruction, Instruction *current_instruction);

		void createEdge(std::string label, Node *source_node, Node *destination_node);

		std::map<Instruction *,InstructionNode *> instruction_map;
		std::list<Edge> edges;
	};

}

