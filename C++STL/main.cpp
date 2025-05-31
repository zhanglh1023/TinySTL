#include <iostream>
#include "include/vector.hpp"
#include "include/string.hpp"
using namespace std;

class Test {
public:
	int _id;
	Test(int id = 10) :_id(id) {
		//cout << "Test Construct\n";
	}
	Test(const Test& t) {
		this->_id = t._id;
		//cout << "Test copy construct\n";
	}
	~Test() {
		//cout << "Test destroy\n";
	}
	friend ostream& operator<< (ostream& cout, const Test& t);
};
ostream& operator<< (ostream& cout, const Test& t) {
	return cout << t._id;
}
int main()
{
	/*String s("aaaa");
	cout << s << "\n";
	String t("tttttt");
	cout << s + t << "\n";
	cout << (s < t) << "\n";
	const String tmp = s;
	tmp[0];
	String::iterator it = t.begin();
	(*it) = 'a';
	for (; it != t.end(); it++) {
		cout << (*it) << "\n";
	}*/
	Test t1(1), t2(2), t3(3);
	cout << t1 << "\n";
	Vector<Test> v;
	v.push_back(t1);
	v.push_back(t2);
	v.push_back(t3);
	cout << v[0] << "\n";
	Vector<Test> vv = move(v);
	cout << v.empty() << "\n";
	//vv = move(v);
	cout << vv[0] << "\n";
	auto it0 = vv.end();
	//vv.pop_back();
	auto it1 = vv.end();
	cout << (it0 != it1) << "\n";
	for (Test t : vv) {
		cout << t << " ";
	}
	cout << "\n";
	Vector<Test>::iterator it = vv.begin();
	for (; it != vv.end(); ++it) {
		if ((*it)._id % 2 == 0) {
			Test t = Test();
			it = vv.insert(it, t);
			it++;
		}
	}
	for (Test t : vv) {
		cout << t << " ";
	}
	cout << "\n";
	return 0;
}