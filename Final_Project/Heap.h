#pragma once
#include<iostream>	
using namespace std;
template<class T>
class Heap {
	T* heapArr;
	int capacity;
	int heap_size;
	//utility function for swapping
	void swap(T* a, T* b) {
		T temp = *a;
		*a = *b;
		*b = temp;
	}
	void heapOrder(int i) {
		//i is index of element
		int left = leftChild(i);//taking index of left child of i
		int right = rightChild(i);//taking index of right child of i
		int max = i;//current max is i
		//check for end of array and updating max index to left child index if left child is greater then parent
		if (left < heap_size && heapArr[left] > heapArr[i])
			max = left;
		//check for end of array and updating max index to right child index if right child is greater then parent
		if (right < heap_size && heapArr[right] > heapArr[max])
			max = right;
		//if updated index is not parent then swap value with the greater value index 
		//and again heaporder at updated index
		if (max != i) {
			swap(&heapArr[i], &heapArr[max]);
			heapOrder(max);
		}
	}
public:
	Heap();
	int parent(int);
	int leftChild(int);
	int rightChild(int);
	void Insert(T);
	T getMin() { if (heap_size > 0)return heapArr[0]; else return T(); }
	int currSize() {return heap_size;}
	void print();
	T deleteMin();
	bool is_empty();
	~Heap();
};

template<class T>
Heap<T>::Heap() {
	capacity = 4;
	heap_size = 0;
	heapArr = new T[capacity];
}
template<class T>
int Heap<T>::parent(int i) {
	return (i - 1) / 2;
}
template<class T>
int Heap<T>::leftChild(int i) {
	return 2 * i + 1;
}
template<class T>
int Heap<T>::rightChild(int i) {
	return 2 * i + 2;
}
template<class T>
void Heap<T>::Insert(T obj) {
	if (heap_size == capacity) {
		capacity += 4;
		T* temp=new T[capacity];
		for (int i = 0; i < heap_size; ++i) {
			temp[i] = heapArr[i];
		}
		delete[] heapArr;
		heapArr = new T[capacity]; //allocating memory
		for (int i = 0; i < heap_size; ++i) {
			heapArr[i] = temp[i];
		}
		this->Insert(obj);
		delete[] temp;
		return;
	}
	heapArr[heap_size++] = obj;
	int i = heap_size - 1;
	while (i != 0 && heapArr[i]>heapArr[parent(i)] ) {
		swap(&heapArr[i], &heapArr[parent(i)]);
		i = parent(i);
	}
}
template<class T>
T Heap<T>::deleteMin() {
	if (heap_size <= 0) {
		cout << "Nothing in heap"<<endl;
		return T();
	}
	if (heap_size == 1) {
		heap_size--;
		return heapArr[0];
	}
	T topElement = heapArr[0];
	heapArr[0] = heapArr[--heap_size];
	heapOrder(0);
	return topElement;
}
template<class T>
void Heap<T>::print() {
	for (int i = 0; i < heap_size; ++i) {
		cout << heapArr[i] << endl;
	}
}
template<class T>
bool Heap<T>::is_empty() {
	return heap_size == 0;
}
template<class T>
Heap<T>::~Heap() {
	if (capacity != 0) {
		delete []heapArr;
	}
}


