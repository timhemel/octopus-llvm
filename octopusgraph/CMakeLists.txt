add_library(OctopusGraphPass MODULE
	OctopusGraphPass.cpp
	OctopusGraph.cpp
	GraphvizWriter.cpp
	DGSWriter.cpp
	InstructionNode.cpp
	OctopusNode.cpp
	CommandLineOptions.cpp
)

target_compile_features(OctopusGraphPass PRIVATE cxx_range_for cxx_auto_type)

set_target_properties(OctopusGraphPass PROPERTIES
	COMPILE_FLAGS "-fno-rtti"
)

if(APPLE)
	set_target_properties(OctopusGraphPass PROPERTIES
		LINK_FLAGS "-undefined dynamic_lookup"
	)
endif(APPLE)
