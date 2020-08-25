#pragma once
#include<iostream>
using namespace std;
template<class T>
struct SNode {
	T value;
	SNode<T>* next;

	SNode() {
		value = T();
		next = nullptr;
	}
};
template <class T> class Stack {
	SNode<T>* top;
public:
	Stack() {
		top = NULL;
	}
	Stack(Stack<T>& rht) {
		*this = rht;
	}
	void Push(T v) {
		SNode<T>* temp = new SNode<T>;
		temp->value = v;
		temp->next = top;
		top = temp;
	}
	T Pop() {
		if (top==NULL) {
			cout << "Stack Empty" << endl;
			return T();
		}
		T value = top->value;
		SNode<T>* temp = top;
		top = top->next;
		delete temp;
		return value;
	}
	T peek() {
		if (top == nullptr)
			return T();
		return top->value;
	}
	bool isEmpty() {
		return top == NULL;
	}
	~Stack() {
		MakeNull();
	}
	void MakeNull() {
		while (!isEmpty())
			Pop();
	}
	void MakeCopy(SNode<T>*& temp) {
		if (temp->next) {
			MakeCopy(temp->next);
		}
		Push(temp->value);
	}
	Stack<T>& operator = (Stack<T>& rht) {
		if (this == &rht)
			return *this;
		MakeNull();
		SNode<T>* curr = rht.top;
		MakeCopy(curr);
		return *this;
	}
};