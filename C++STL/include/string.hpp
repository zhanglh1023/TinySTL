#ifndef STRING_H
#define STRING_H

#include<iostream>
using namespace std;

class String {
public:
	String(const char *t = nullptr) {
		if (t != nullptr) {
			str = new char[strlen(t) + 1];
			for (int i = 0; i < (int)strlen(t); i++) str[i] = t[i];
			str[strlen(t)] = '\0';
		}
		else {
			str = new char[1];
			str[0] = '\0';
		}
	}
	String(const String& t) {
		
		int len = strlen(t.str);
		if (len == 0) {
			str = new char[1];
			str[0] = '\0';
			return;
		}
		str = new char[len + 1];
		for (int i = 0; i < len; i++) str[i] = t.str[i];
		str[len] = '\0';
	}

	String& operator= (const String& t) {
		if (this == &t) return *this;
		if (str != nullptr) {
			delete[] str;
		}
		int len = strlen(t.str);
		str = new char[len + 1];
		for (int i = 0; i < len; i++) {
			str[i] = t[i];
		}
		str[len] = '\0';
		return *this;
	}

	char& operator[] (int index) {
		if (index < 0 || index >= (int)strlen(str)) throw "String operator[] error";
		return str[index];
	}
	const char& operator[] (int index) const {
		if (index < 0 || index >= (int)strlen(str)) throw "String operator[] error";
		return str[index];
	}
	bool operator< (const String& t) const {
		return strcmp(str, t.str) < 0;
	}
	bool operator> (const String& t) const {
		return strcmp(str, t.str) > 0;
	}
	bool operator== (const String& t) const {
		return strcmp(str, t.str) == 0;
	}
	int length() const {
		return strlen(str);
	}
	const char* c_str() const { return str; }
	~String() {
		if (str == nullptr) return;
		delete[]str;
		str = nullptr;
	}
	class iterator {
	public:
		iterator(char* t = nullptr):p(t){}
		iterator(const iterator& it) {
			p = it.p;
		}
		iterator& operator=(const iterator& it) {
			p = it.p;
			return *this;
		}
		bool operator!=(const iterator& it) const {
			return p != it.p;
		}
		iterator& operator++() {
			this->p++;
			return *this;
		}
		iterator operator++(int) {
			iterator t = *this;
			this->p++;
			return t;
		}
		char& operator*() {
			return (*p);
		}
		const char& operator*() const {
			//cout << "const char& operator*() const\n";
			return *p;
		}
	private:
		char* p;
	};
	iterator begin() { return iterator(str); }
	iterator end() { return iterator(str + strlen(str)); }
private:
	char* str;

	friend ostream& operator<< (ostream& cout, const String& s);
	friend String operator+ (const String& t1, const String& t2);

};

String operator+ (const String& t1, const String& t2) {
	int len1 = t1.length();
	int len2 = t2.length();
	String tmp;
	if(tmp.str != nullptr)
		delete[] tmp.str;
	tmp.str = new char[len1 + len2 + 1];
	for (int i = 0; i < len1; i++) tmp.str[i] = t1[i];
	for (int i = 0; i < len2; i++) tmp.str[i + len1] = t2[i];
	tmp.str[len1 + len2] = '\0';
	return tmp;
}

ostream& operator<< (ostream& cout, const String& s) {
	return cout << s.str;
}

/*istream& operator>> (istream& cin, String& s) {
	
	return cin;
}*/
#endif