#ifndef VECTOR
#define VECTOR

#include<iostream>
#include "allocator.hpp"
using namespace std;

template<typename T, typename Alloc = Allocator<T>>
class Vector {

public:
	Vector(int capacity = 1) {
		_first = _alloc.allocate(capacity);
		_last = _first;
		_end = _first + capacity;
	}

	Vector(const Vector<T>& v) {
		cout << "Vector(const Vector<T>& v)\n";
		if (this == &v)
			return;
		int size = v._last - v._first;
		int capacity = v._end - v._first;
		_first = _alloc.allocate(capacity);
		_last = _first + size;
		_end = _first + capacity;
		for (T* p = _first; p != _last; p++) {
			_alloc.construct(p, (*(v._first + (p - _first))));
		}
	}

	Vector(Vector<T>&& v) {
		cout << "Vector(Vector<T>&& v)\n";
		if (this == &v) return;
		_first = v._first;
		_last = v._last;
		_end = v._end;
		v._first = v._end = v._last = nullptr;
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
		return *this;
	}
	Vector<T>& operator=(Vector<T>&& v) {
		cout << "Vector<T>& operator=(Vector<T>&& v)\n";
		if (this == &v) return *this;
		this->~Vector();
		_first = v._first;
		_last = v._last;
		_end = v._end;
		v._first = v._last = v._end = nullptr;
		return *this;
	}
	T& operator[](int index) {
		if (index >= _last - _first||index < 0) throw "vector operator[] error";
		return *(_first + index);
	}
	const T& operator[](int index) const {
		if (index >= _last - _first || index < 0) throw "vector operator[] error";
		return *(_first + index);
	}
	void push_back(const T& t) {
		if (full()) expend();
		_alloc.construct(_last, t);
		_last++;
	}

	void pop_back() {
		if (empty()) return;
		verify(_last - 2, _last);
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
		if (_first == nullptr) return;
		for (T* p = _first; p != _last; p++) {
			_alloc.destroy((*p));
		}
		_alloc.dealloc(_first);
		_first = _last = _end = nullptr;
		while (_head._next != nullptr) {
			Iterator_Base* t = _head._next;
			_head._next = t->_next;
			delete t;
		}
	}
	class iterator {
	public:
		friend class Vector<T, Alloc>;
		iterator(T* ptr = nullptr, Vector<T, Alloc>* pVec = nullptr):_p(ptr), _pVec(pVec){
			Iterator_Base* itBase = new Iterator_Base(this, _pVec->_head._next);
			_pVec->_head._next = itBase;
		}
		iterator(const iterator& t) {
			_p = t._p;
			_pVec = t._pVec;
		}
		iterator& operator=(const iterator& t) {
			_p = t._p;
			_pVec = t._pVec;
			return *this;
		}
		bool operator!= (const iterator& t) const {
			if (_pVec == nullptr||t._pVec == nullptr||_pVec != t._pVec) {
				throw "iterator incompatable";
			}
			return _p != t._p;
		}
		iterator& operator++() {
			if (_pVec == nullptr)
				throw "iterator invaild";
			this->_p++;
			return *this;
		}
		iterator operator++(int) {
			if (_pVec == nullptr)
				throw "iterator invaild";
			iterator tmp = *this;
			this->_p++;
			return tmp;
		}
		T& operator*() {
			if (_pVec == nullptr)
				throw "iterator invaild";
			return *_p;
		}
		const T& operator*() const {
			if (_pVec == nullptr)
				throw "iterator invaild";
			return *_p;
		}
	private:
		T* _p;
		Vector<T, Alloc>* _pVec;
	};
	iterator begin() { return iterator(_first, this); }
	iterator end() { return iterator(_last, this); }
	iterator insert(const iterator& it, const T& val) {


		verify(it._p - 1, _last);
		T* p = _last;
		_last++;
		while (p > it._p) {
			_alloc.destroy(*p);
			_alloc.construct(p, *(p - 1));
			p--;
		}
		_alloc.destroy(*p);
		_alloc.construct(p, val);
		return iterator(p, this);
	}

	void verify(T* first, T* last) {
		Iterator_Base* pre = &_head;
		Iterator_Base* p = _head._next;
		while (p != nullptr) {
			Iterator_Base* t = p;
			p = t->_next;
			if (t->_it->_p > first && t->_it->_p <= last) {
				t->_it->_pVec = nullptr;
				delete t;
				pre->_next = p;
			}
			else pre = t;
		}
	}
private:
	T* _first;
	T* _last;
	T* _end;
	Alloc _alloc;


	struct Iterator_Base {
		Iterator_Base(iterator* it = nullptr, Iterator_Base* next = nullptr):_it(it), _next(next){}
		iterator* _it;
		Iterator_Base* _next;
	};
	Iterator_Base _head;

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
#endif