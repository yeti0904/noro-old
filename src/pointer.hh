#pragma once
#include "_components.hh"

class AllocatedPointer {
	public:
		// variables
		void*  ptr;
		size_t size;

		// functions
		AllocatedPointer();
		AllocatedPointer(void* p_ptr, size_t p_size);
		~AllocatedPointer();
		void Alloc(size_t p_size);
		void Resize(size_t p_size);
};
