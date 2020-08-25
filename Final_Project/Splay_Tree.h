 
 #pragma once
#include "Queue.h"
#include "DataStructure.h"
#include<iostream>
using namespace std;
template<class T>
struct SplayNode {
	T key;
	SplayNode<T>* left, *right;
	//constructor
	SplayNode() {
		key = T();
		left = right = NULL;
	}
};
template<class T>
class SplayTree:public DataStructure<T> {
	SplayNode<T>* root;
	//funtion bring the value at top of tree
	//if value not found then it will bring 
	//closet valut to top
	void Splay(T value, SplayNode<T>*& curr) {
		//node left pointer store tree whose
		//values are less then node which are 
		//finding....
		int count=0;
		SplayNode<T>* temp = new SplayNode<T>;
		SplayNode<T>* leftMax = temp, *rightMin = temp;//leftmax pointer to max node
												//rightmin pointer to min node
		while (curr && curr->key != value) {
			count++;
			//loop end when key found or curr is null
			if (value < curr->key) {			//go to left if value is less then curr value
				if (!curr->left) break;			//if left is empty then break
				if (curr->key < curr->left->key)//if current node key is less then left of curr key then Rotate right  
					RotateRight(curr);
				rightMin->left = curr;			//Update left of righMin
				rightMin = curr;				//Now update rightmin to rightmin->left
				curr = curr->left;
			}
			else if (value > curr->key) {		//go to right if value isgreater then current value
				if (!curr->right)break;			//if right is empty then break
				if (curr->key > curr->right->key)//if current node key is less then right of curr key then Rotate left
					RotateLeft(curr);
				leftMax->right = curr;				//Update right of leftMax
				leftMax = curr;
				curr = curr->right;
			}
		}
		//reassembling the node
		leftMax->right = curr->left;
		rightMin->left = curr->right;
		curr->left = temp->right;
		curr->right = temp->left;
		temp->right = temp->left = NULL;
		cout << "SPLay" << count<<endl;
		delete temp;
	}
	//curr is reference of point whose 
	//child is to rotate left or right
	void RotateRight(SplayNode<T>*& curr) {
		SplayNode<T>* temp = curr->left;
		curr->left = temp->right;
		temp->right = curr;
		curr = temp;
	}
	void RotateLeft(SplayNode<T>*& curr) {
		SplayNode<T>* temp = curr->right;
		curr->right = temp->left;
		temp->left = curr;
		curr = temp;
	}
	//copy a tree to calling tree
	SplayNode<T>* MakeCopy(SplayNode<T>* temp) {
		SplayNode<T>* curr = new SplayNode<T>;
		curr->key = temp->key;
		if (temp->left)
			curr->left = MakeCopy(temp->left);
		if (temp->right)
			curr->right = MakeCopy(temp->right);
		return curr;
	}

public:
	SplayTree() {
		root = NULL;
	}
	SplayNode<T>* GetRoot() { return root; }
	SplayTree(SplayTree<T>& temp) {
		*this = temp;
	}
	void Insert(T value) {
		//if no root then add value to root
		if (root == nullptr) {
			root = new SplayNode<T>;
			root->key = value;
		}
		else {
			Splay(value, root);//splay value to top
			SplayNode<T>* newNode = new SplayNode<T>;
			newNode->key = value;
			if (value < root->key) {
				newNode->right = root;
				newNode->left = root->left;
				root->left = NULL;
				root = newNode;
			}
			else /*if (value > root->key)*/ {
				newNode->left = root;
				newNode->right = root->right;
				root->right = NULL;
				root = newNode;
			}
		}
	}
	void Delete(T value) {
		SplayNode<T>* newRoot;
		Splay(value, root);//Splay the value to top of tree
		if (root->key == value) {
			if (root->left) {
				newRoot = root->left;
				Splay(value, newRoot);
				newRoot->right = root->right;
			}
			else {
				newRoot = root->right;
			}
		}
		else {
			cout << "Value Not Found" << endl;
			return;
		}
		delete root;
		root = newRoot;
	}
	T Find(T value) {
		//if value is already at root
		if (root->key == value) {
			return root->key;
		}
		else {
			//splay the value at top of tree
			Splay(value, root);
			//if found then return pointer to root
			if (root->key == value) {
				return root->key;
			}
			//else return null pointer
			else {
				cout << "Value Not Found" << endl;
				return T();
			}
		}
	}
	//assignment operator
	const SplayTree& operator = (SplayTree& temp) {
		if (this == &temp) {
			//if assign value to itself
			return *this;
		}
		//Delete curr tree
		MakeNull();
		if (temp.GetRoot())
			root = MakeCopy(temp.GetRoot());
		else
			root = NULL;
		return *this;
	}
	void Display() {
		//Level Order Display
		Queue<SplayNode<T>*> a;
		a.enqueue(root);
		while (!a.isEmpty()) {
			if (a.Top()->left)a.enqueue(a.Top()->left);
			if (a.Top()->right)a.enqueue(a.Top()->right);
			cout << a.dequeue()->key << " ";
		}
		cout << endl;
	}
	void MakeNull() {
		while (root)
			Delete(root->key);
		root = NULL;
	}
	~SplayTree() {
		MakeNull();
	}
};