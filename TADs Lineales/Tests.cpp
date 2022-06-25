#include <iostream>
using namespace std;

#include "Stack.h"
#include "LinkedListStack.h"
#include "Queue.h"
#include "List.h"

void testStack(){
	char op;
	int n;
	Stack<int> s;
	cout << s;
	do{
		cout << "Choose option ((p)ush, p(o)p, (t)op, (s)ize, (e)xit)" << endl;
		cin >> op;
		if (op == 'p'){
			cin >> n;
			s.push(n);
		} else if (op == 'o') {
			try {
				s.pop();
			} catch (ExcepcionTAD& e) {
				cout << e.msg() << endl;
			}
		} else if (op == 't') {
			try {
				cout << "Top is " << s.top() << endl;
			} catch (ExcepcionTAD& e) {
				cout << e.msg() << endl;
			}
		} else if (op == 's') {
			cout << "Size is " << s.size() << endl;
		}
		cout << s << endl;
	} while (op != 'e');
	cout << "Program finished" << endl;
}

void testLinkedListStack(){
	char op;
	int n;
	LinkedListStack<int> s;
	cout << s;
	do{
		cout << "Choose option ((p)ush, p(o)p, (t)op, (s)ize, (e)xit)" << endl;
		cin >> op;
		if (op == 'p'){
			cin >> n;
			s.push(n);
		} else if (op == 'o') {
			try {
				s.pop();
			} catch (ExcepcionTAD& e) {
				cout << e.msg() << endl;
			}
		} else if (op == 't') {
			try {
				cout << "Top is " << s.top() << endl;
			} catch (ExcepcionTAD& e) {
				cout << e.msg() << endl;
			}
		} else if (op == 's') {
			cout << "Size is " << s.size() << endl;
		}
		cout << s << endl;
	} while (op != 'e');
	cout << "Program finished" << endl;
}

void testQueue(){
	char op;
	int n;
	Queue<int> q;
	cout << q;
	do{
		cout << "Choose option ((p)ush back, p(o)p front, (f)ront, (s)ize, (e)xit)" << endl;
		cin >> op;
		if (op == 'p'){
			cin >> n;
			q.push_back(n);
		} else if (op == 'o') {
			try {
				q.pop_front();
			} catch (ExcepcionTAD& e) {
				cout << e.msg() << endl;
			}
		} else if (op == 'f') {
			try {
				cout << "Front is " << q.front() << endl;
			} catch (ExcepcionTAD& e) {
				cout << e.msg() << endl;
			}
		} else if (op == 's') {
			cout << "Size is " << q.size() << endl;
		}
		cout << q << endl;
	} while (op != 'e');
	cout << "Program finished" << endl;
}

// Esta vez se redefine el operador << usando un iterador
template<class T>
std::ostream& operator<<(std::ostream& sOut, List<T>& l) {
	List<int>::ConstIterator it = l.cbegin();
	List<int>::ConstIterator itEnd = l.cend();
	while (it != itEnd){
		sOut << it.elem() << " ";
		it.next();
	}
	sOut << endl;
	return sOut;
}

template<class T>
void incList(List<T>& l) {
	List<int>::Iterator it = l.begin();
	List<int>::Iterator itEnd = l.end();
	while (it != itEnd){
		it.set(it.elem() + 1);
		it.next();
	}
}

template<class T>
void removeOdd(List<T>& l) {
	List<int>::Iterator it = l.begin();
	List<int>::Iterator itEnd = l.end();
	while (it != itEnd){
		if (it.elem()%2 == 1) l.erase(it);
		else it.next();
	}
}

void testList(){
	string op;
	int n;
	List<int> l;
	cout << l;
	do{
		cout << "Choose option ((pb) push back, (pf) push front, (ob) pop back, (of) pop front,"
				" (b) back, (f) front, (a) at, (i) increment, (r) remove odd, (e) exit)" << endl;
		cin >> op;
		if (op == "pb"){
			cin >> n;
			l.push_back(n);
		} else if (op == "pf"){
			cin >> n;
			l.push_front(n);
		} else if (op == "ob") {
			try {
				l.pop_back();
			} catch (ExcepcionTAD& e) {
				cout << e.msg() << endl;
			}
		} else if (op == "of") {
			try {
				l.pop_front();
			} catch (ExcepcionTAD& e) {
				cout << e.msg() << endl;
			}
		} else if (op == "b") {
			try {
				cout << "Back is " << l.back() << endl;
			} catch (ExcepcionTAD& e) {
				cout << e.msg() << endl;
			}
		} else if (op == "f") {
			try {
				cout << "Front is " << l.front() << endl;
			} catch (ExcepcionTAD& e) {
				cout << e.msg() << endl;
			}
		} else if (op == "a") {
			int i;
			cin >> i;
			try {
				cout << "Element at position " << i << " is " << l.at(i) << endl;
			} catch (ExcepcionTAD& e) {
				cout << e.msg() << endl;
			}
		} else if (op == "i") {
			incList(l);
		} else if (op == "r") {
			removeOdd(l);
		}
		cout << l << endl;
	} while (op != "e");
	cout << "Program finished" << endl;
}
