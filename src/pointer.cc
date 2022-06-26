#include "pointer.hh"

AllocatedPointer::AllocatedPointer() {
	ptr  = nullptr;
	size = 0;
}

AllocatedPointer::AllocatedPointer(void* p_ptr, size_t p_size) {
	ptr  = p_ptr;
	size = p_size;
}

AllocatedPointer::~AllocatedPointer() {
	if (ptr != nullptr) {
		free(ptr);
	}
}

void AllocatedPointer::Alloc(size_t p_size) {
	ptr  = malloc(size);
	size = p_size;
}

void AllocatedPointer::Resize(size_t p_size) {
	ptr  = realloc(ptr, size);
	size = p_size;
}
