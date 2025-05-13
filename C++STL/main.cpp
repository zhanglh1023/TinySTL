#include <iostream>
#include <new>
using namespace std;

class Test {
public:
	int _id;
	Test(int id = 10) :_id(id) {
		cout << "Construct\n";
	}
	Test(const Test& t) {
		this->_id = t._id;
		cout << "copy construct\n";
	}
	~Test() {
		cout << "destroy\n";
	}
	friend ostream& operator<< (ostream& cout, const Test& t);
};
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
template<typename T, typename Alloc = Allocator<T>>
class Vector {

public:
	Vector(int capacity = 1) {
		_first = _alloc.allocate(capacity);
		_last = _first;
		_end = _first + capacity;
	}

	Vector(const Vector<T>& v) {
		if (this == &v)
			return;
		int size = v._last - v._first;
		int capacity = v.end - v._first;
		_first = _alloc.allocate(capacity);
		_last = _first + size;
		_end = _first + capacity;
		for (T* p = _first; p != _last; p++) {
			_alloc.construct(p, (*(v._first + p - _first)));
		}
	}
	Vector<T>& operator=(const Vector<T>& v) {
		if (this == &v) return *this;
		int size = v._last - v._first;
		int capacity = v._end - v._first;
		this->~Vector();
		_first = _alloc.allocate(capacity);
		_last = _first + size;
		_end = _first + capacity;
		for (T* p = _first; p != _last; p++) {
			_alloc.construct(p, (*(v._first + p - _first)));
		}
	}
	T& operator[](int index) {
		if (index >= _last - _first) throw "operator[] error";
		return *(_first + index);
	}
	void push_back(const T& t) {
		if (full()) expend();
		_alloc.construct(_last, t);
		_last++;
	}

	void pop_back() {
		if (empty()) return;
		_alloc.destroy(*(_last - 1));
		_last--;
	}
	bool empty() const {
		return _last == _first;
	}
	bool full() const {
		return _last == _end;
	}
	int size() const {
		return _last - _first;
	}
	~Vector() {
		for (T* p = _first; p != _last; p++) {
			_alloc.destroy((*p));
		}
		_alloc.dealloc(_first);
		_first = _last = _end = nullptr;
	}
private:
	T* _first;
	T* _last;
	T* _end;
	Alloc _alloc;
	void expend() {
		int size = _last - _first;
		T* tmp = _alloc.allocate(size * 2);
		for (int i = 0; i < size; i++) {
			_alloc.construct(tmp + i, *(_first + i));
			_alloc.destroy(*(_first + i));
		}
		_alloc.dealloc(_first);
		_first = tmp;
		_last = tmp + size;
		_end = tmp + size * 2;
	}
};

ostream& operator<< (ostream& cout, const Test& t) {
	return cout << t._id;
}
int main()
{
	Test t1(1), t2(2), t3(3);
	cout << t1 << "\n";
	Vector<Test> v;
	v.push_back(t1);
	v.push_back(t2);
	v.push_back(t3);
	cout << v[0] << "\n";
	return 0;
}