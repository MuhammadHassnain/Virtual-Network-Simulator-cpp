#pragma once
#include<iostream>	
using namespace std;
template<class T>
class QNode {
public:
	T value;
	QNode* next;

	QNode() {
		value = T();
		next = NULL;
	}
	~QNode() {

	}
};
template<class T>
class Queue {
	QNode<T>* front;
	QNode<T>* rear;
public:
	Queue() {
		front = NULL;
		rear = NULL;
	}
	Queue(Queue<T>& temp) {
		*this = temp;
	}
	bool isEmpty() {
		return front == NULL;
	}
	void enqueue(T v) {
		QNode<T>* newNode;
		newNode = new QNode<T>;
		newNode->value = v;
		newNode->next = NULL;
		if (isEmpty()) {
			front = newNode;
			rear = newNode;
		}
		else {
			rear->next = newNode;
			rear = newNode;
		}
	}
	T dequeue() {
		QNode<T>* temp;
		T v = T();
		if (isEmpty()) {
			return v;
		}
		else {
			v = front->value;
			temp = front->next;
			delete front;
			front = temp;
			return v;
		}
	}
	~Queue() {
		MakeNull();
	}
	void MakeNull(){
		QNode<T>* curr = front;
		while (curr) {
			dequeue();
			curr = front;
		}
		front = NULL;
		rear = front;
	}
	T Top() {
		return front->value;
	}
	 Queue<T>& operator = (Queue<T>& temp) {
		if (this == &temp)
			return *this;
		MakeNull();
		if (!temp.isEmpty()) {
			QNode<T>* curr = temp.top();
			while (curr) {
				enqueue(curr->value);
				curr = curr->next;
			}
		}
		return *this;
	}

};