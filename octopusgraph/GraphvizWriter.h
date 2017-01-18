#include <ostream>

namespace GraphWriter {

	class GraphvizWriter {
	public:
		GraphvizWriter(std::ostream &ost) : ost(ost) { }
		void writeOctopusGraph(Octopus::OctopusGraph &octopus_graph);
	private:
		void writeHeader();
		void writeFooter();
		void writeNode(Octopus::Node *n);
		void writeNodes(Octopus::OctopusGraph &octopus_graph);
		void writeEdge(const Octopus::Edge &e);
		void writeEdges(Octopus::OctopusGraph &octopus_graph);

		std::ostream &ost;

	};

}

