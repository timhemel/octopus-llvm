#ifndef DGS_WRITER_H
#define DGS_WRITER_H

#include <ostream>

namespace GraphWriter {

	class DGSWriter {
	public:
		DGSWriter(std::ostream &ost) : ost(ost) { }
		void writeOctopusGraph(Octopus::OctopusGraph &octopus_graph);
	private:
		void writeHeader();
		void writeFooter();
		void writeEscaped(std::string const &s);
		void writeNodePropertyValues(std::list<std::string> &properties);
		void writeNodeProperties(Octopus::Node *n);
		void writeNode(Octopus::Node *n);
		void writeNodes(Octopus::OctopusGraph &octopus_graph);
		void writeEdgeProperties(const Octopus::Edge *e);
		void writeEdge(const Octopus::Edge *e);
		void writeEdges(Octopus::OctopusGraph &octopus_graph);

		std::ostream &ost;
	};

}

#endif
