#ifndef ALLOCATOR
#define ALLOCATOR


template<typename T>
struct Allocator {
	T* allocate(int size) {
		return (T*)malloc(size * sizeof(T));
	}
	void dealloc(T* p) {
		free(p);
	}
	void construct(T* p, const T& t) {
		new (p) T(t);
	}
	void destroy(T& t) {
		t.~T();
	}
};


#endif
