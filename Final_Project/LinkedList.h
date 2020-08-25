#pragma once
#include "DataStructure.h"
#include<iostream>	
using namespace std;
template<class T>
class LNode {
public:
	T data;
	LNode<T>* next;
	LNode<T>* prev;

	LNode() {
		data = T();
		next=prev = NULL;
	}
};
template<class T>
class LinkedList:public DataStructure<T>{
	LNode<T>* head, *tail;
	int total_elements;
public:
	LinkedList() {
		head = tail = nullptr;
		total_elements = 0;
	}
	LNode<T>* getHead() {
		return head;
	}
	int ElementInList() { return total_elements; }
	LinkedList(LinkedList<T>& temp) {
		*this = temp;
	}
	bool isEmpty() {
		return head == NULL;
	}
	void Insert(T value) {
		if (!head) {
			head = new LNode<T>;
			head->data = value;
			tail = head;
		}
		else {
			LNode<T>* temp = new LNode<T>;
			temp->data = value;
			temp->prev = tail;
			tail->next = temp;
			tail = temp;
		}
		++total_elements;
	}
	void Delete(T value) {
		if (!head) {
			cout << "Empty List" << endl;
		}
		else {
			LNode<T>* curr = head;
			int found = false;
			while (curr && !found) {
				if (curr->data == value) {
					if(curr->prev)
						curr->prev->next = curr->next;
					else 
						head = curr->next;
					if(curr->next)
						curr->next->prev = curr->prev;
					else 
						tail = curr->prev;
					delete curr;
					found = true;
					break;
				}
				curr = curr->next;
			}
			if (!found) {
				cout << value<<" Not found" << endl;
			}
		}
	}
	void MakeNull() {
		LNode<T>* curr = head;
		while (head) {
			curr = head->next;
			delete head;
			head = curr;
		}
		tail = head = nullptr;
	}
	~LinkedList() {
		MakeNull();
	}
	T Find(T value) {
		LNode<T>* curr = head;	
		while (curr) {
			if (curr->data == value) {
				return curr->data;
			}
			curr = curr->next;
		}
		return T();
	}

	const LinkedList<T>& operator = (LinkedList<T>& temp) {
		if (this == &temp) {
			return *this;
		}
		MakeNull();
		if (!temp.isEmpty()) {
			LNode<T>* curr = temp.head;
			while (curr) {
				Insert(curr->data);
				curr = curr->next;
			}
		}
		return *this;
	}
};
template <class T>
ostream& operator << (ostream& out, LinkedList<T>& list) {
	LNode<T>* curr = list.getHead();
	while (curr) {
		out << " " << curr->data;
		curr = curr->next;
	}
	return out;
}

