#ifndef OCTOPUS_GRAPH_H
#define OCTOPUS_GRAPH_H

#include <map>
#include <list>
#include <set>
#include <string>
#include <ostream>
#include "llvm/IR/Function.h"
#include "SimpleSlotTracker.h"
#include "OctopusNode.h"
#include "InstructionNode.h"

using namespace llvm;

namespace Octopus {

	class Edge {
	public:
		Edge(std::string label, Node *source_node, Node *destination_node);
		void setProperty(std::string key, std::string value);
		typedef std::map<std::string,std::string>::const_iterator property_iterator;
		property_iterator properties_begin() const { return properties.begin(); }
		property_iterator properties_end() const { return properties.end(); }
		
		std::string label;
		Node *source_node;
		Node *destination_node;
	private:	
		std::map<std::string,std::string> properties;
	};

	struct edge_compare {
		bool operator()(const Edge* e1, const Edge* e2) const {
			bool b = e1->label == e2->label
				? ( e1->source_node == e2->source_node
					? e1->destination_node < e2->destination_node
					: e1->source_node < e2->source_node )
				: e1->label < e2->label;
			return b;
		}
	};

	// lookup tables for DIInfo -> LocationNode
	class OctopusGraph {
	public:
		void initializeFunction(Function &F);
		void finalizeFunction(Function &F);
		SimpleSlotTracker &getSlotTracker() { return slot_tracker; }
		void createEntryAndExitNodesForFunction(Function &F);
		void addBlockLabel(BasicBlock &B);
		void createAndConnectInstructionNodesForBasicBlock(BasicBlock &B);
		void linkBasicBlock(BasicBlock &B);
		void createDataDependenceEdges();

		typedef std::list<Node *>::iterator node_iterator;
		node_iterator node_begin();
		node_iterator node_end();

		typedef std::set<Edge*,edge_compare>::iterator edge_iterator;
		edge_iterator edge_begin();
		edge_iterator edge_end();

	private:
		InstructionNode* findOrCreateInstructionNode(Instruction *instruction);
		InstructionNode *createInstructionNode(Instruction *instruction);
		void linkInstructionWithPredecessor(Instruction *previous_instruction, Instruction *current_instruction);
		void linkBasicBlockInstructions(BasicBlock &source_block, BasicBlock &destination_block);
		void createInstructionChildren(InstructionNode *instruction);
		IROperandNode *createOperandNode(const Value *operand);

		void storeNode(Node *node) { nodes.push_back(node); }
		Edge *createAndStoreEdge(std::string label, Node *source_node, Node *destination_node);
		LocationNode* findOrCreateLocationAndFileNodes(InstructionNode *instruction);
		FileNode* findOrCreateFileNode(DILocation *location);
		LocationNode* findOrCreateLocationNode(DILocation *location);

		std::map<const Instruction *,InstructionNode *> instruction_map;
		std::map<Function *,CFGEntryNode *> entry_nodes_map;
		std::map<Function *,CFGExitNode *> exit_nodes_map;
		std::map<std::string,FileNode *> file_map;
		std::map<DILocation*,LocationNode *> location_map;
		std::list<Node*> nodes;
		std::set<Edge *,edge_compare> edges;

		SimpleSlotTracker slot_tracker;
	};

}

#endif
