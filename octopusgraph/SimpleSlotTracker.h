#ifndef SIMPLE_SLOT_TRACKER_H
#define SIMPLE_SLOT_TRACKER_H

using namespace llvm;

namespace Octopus {

	class SimpleSlotTracker {
	public:
		SimpleSlotTracker() : slot_count(0) { }
		void add(const Value *v) { slot_map[v] = slot_count++; }
		int getSlotIndex(const Value *v) { return slot_map[v]; }
		void reset() { slot_count = 0; }
	private:
		int slot_count;
		std::map<const Value *, int> slot_map;
	};

}


#endif
