#pragma once
#include<iostream>
#include<cmath>
#include "DataStructure.h"
using namespace std;

// A BTree node
template <class T>
class BTreeNode{
public:
	T* keys;      //An array of keys
	BTreeNode** childs;   //An array of pointer to childs
	int MaxKeys;  //Store the Maxmimum number of keys a Tree has.
	int currKeys; //Current Keys in the node
	bool leaf;
	//constructor
	//l tell that node is leaf or not
	//n tell about number of keys in node
	BTreeNode(int n, bool l) {
		leaf = l;
		MaxKeys = n - 1;
		keys = new T[n];
		childs = new BTreeNode<T>*[n + 1];
		for (int i = 0; i < n; ++i) keys[i] = T();
		for (int i = 0; i <= n; ++i)childs[i] = NULL;
	}
	void Insert(T value) {
		int i = 0;
		for (i = currKeys - 1; i >= 0 && keys[i] > value; --i)
			keys[i + 1] = keys[i];
		keys[i + 1] = value;
	}
	void Remove(T value) {
		int i = 0;
		bool found = false;
		for (; i < currKeys; ++i) {
			if (keys[i] == value) {
				found = true;
			}
			if (found) {
				keys[i] = key[i + 1];
			}
		}
		if (!found) {
			cout << "Key Not Found" << endl;
		}
	}
	//return biggest value in left subtree of index
	T GetPrevious(int index) {
		BTreeNode<T>* curr = childs[index];
		while (!curr->leaf)
			curr = curr->childs[curr->currKeys];
		return curr->keys[curr->currKeys - 1];
	}
	//return smallest value in right subtree of index
	T GetNext(int index) {
		BTreeNode<T>* curr = childs[index + 1];
		while (!curr->leaf)
			curr = curr->childs[0];
		return curr->keys[0];
	}
	//function to remove value from leaf
	void RemoveFromLeaf(int index) {
		for (int i = index + 1; i < currKeys; ++i)
			keys[i - 1] = keys[i];
		currKeys--;
	}
	//function to remove value from non-leaf node
	void RemoveFromNonLeaf(int index) {
		T k = keys[index]; //store the value
		if (childs[index]->currKeys >= ((int)(ceil((double)(MaxKeys + 1) / 2.0)))) {  //check where we are going to
																					//find value if there is voilation in that node
			T prev = GetPrevious(index);											//swap value wiht hightest value in left subtree 
			keys[index] = prev;
			childs[index]->Delete(prev);											//remove heights value in left subtre
		}
		else if (childs[index + 1]->currKeys >= ((int)(ceil((double)(MaxKeys + 1) / 2.0)))) {
			T next = GetNext(index);
			keys[index] = next;																//swap value with height with lowest value in right subtree
			childs[index + 1]->Delete(next);												//
		}
		else {
			Merge(index);																	//Merge the two sub child if both chiles has minimum childs
			childs[index]->Delete(k);
		}
	}
	//function take index and
	//merge both its subtee
	void Merge(int index) {
		BTreeNode<T>* LeftChilds = childs[index];
		BTreeNode<T>* RightChilds = childs[index + 1];
		//mov a key from parent to children
		LeftChilds->keys[LeftChilds->currKeys] = keys[index];
		int i = 0, j = LeftChilds->currKeys + 1;
		//copy right key to left
		for (; i < RightChilds->currKeys; ++i, ++j) {
			LeftChilds->keys[j] = RightChilds->keys[i];
		}
		//if node is not leaf
		//copy right childs to left
		if (LeftChilds->leaf == false) {
			i = 0;
			j = LeftChilds->currKeys + 1;
			for (; i <= RightChilds->currKeys; ++i, ++j) {
				LeftChilds->childs[j] = RightChilds->childs[i];
			}
		}
		//removing moved key from parent
		i = index + 1;
		for (; i < currKeys; ++i)
			keys[i - 1] = keys[i];
		//shifting childs 
		i = index + 2;
		for (; i <= currKeys; ++i)
			childs[i - 1] = childs[i];
		currKeys--;
		//updateing numbe of keys
		LeftChilds->currKeys += RightChilds->currKeys + 1;
		//deltign the copied right subtree
		delete RightChilds;
	}

	//delete value from tree
	void Delete(T value) {
		//find index of value equal or greater then find value
		int index = FindKey(value);
		//if value found 
		if (index < currKeys && keys[index] == value) {
			//check if it is leaf node
			//if leaf then call removefromleaf else call removefrom non leaf
			if (leaf)
				RemoveFromLeaf(index);
			else
				RemoveFromNonLeaf(index);
		}
		else {
			bool is_in_last_child; //variable to check then value we are finding is in 
									//last subtree of node
			if (currKeys != index)
				is_in_last_child = false;
			else
				is_in_last_child = true;
			if (childs[index]) {
				//if there is childs 
				if (childs[index]->currKeys < ((int)(ceil((double)(MaxKeys) / 2.0)))) {
					FillLessKeyChilds(index);//check child  for violation of btree
				}

				if (is_in_last_child && index > currKeys)
					childs[index - 1]->Delete(value);
				else
					childs[index]->Delete(value);
			}
		}
	}
	//function used to check if a childs has less number
	//of key then it will borrow from previous childs 
	//or from next chids
	void FillLessKeyChilds(int index) {
		if (index != 0 && childs[index - 1]->currKeys > ((int)(ceil((double)(MaxKeys) / 2.0)))) {
			BorrowFromPrevious(index);
		}
		else if (index != currKeys && childs[index + 1]->currKeys > ((int)(ceil((double)(MaxKeys) / 2.0)))) {
			BorrowFromNext(index);
		}
		else {
			if (index != currKeys)
				Merge(index);
			else
				Merge(index - 1);
		}
	}
	//take a key from left child
	void BorrowFromPrevious(int index) {
		BTreeNode<T>* current = childs[index];
		BTreeNode<T>* previous = childs[index - 1];

		int i = current->currKeys - 1;
		//make space for keys
		for (; i >= 0; --i) {
			current->keys[i + 1] = current->keys[i];
		}
		//if not a leaf node then copy childs also
		if (current->leaf == false) {
			i = current->currKeys;
			for (; i >= 0; --i) {
				current->childs[i + 1] = current->childs[i];
			}
		}
		//mov keys to current node arr[0]
		current->keys[0] = keys[index - 1];
		//if this is not leaf then mov childs also
		if (!leaf) {
			childs[0] = previous->childs[previous->currKeys];
		}
		keys[index - 1] = previous->keys[previous->currKeys];
		current->currKeys += 1;
		previous->currKeys--;
	}
	//take a keyfrom next subtree
	void BorrowFromNext(int index) {
		BTreeNode<T>* current = childs[index];
		BTreeNode<T>* next = childs[index + 1];
		//mov key of parent ot current childs
		current->keys[current->currKeys] = keys[index];
		//if not leaf then also link childs
		if (!(current->leaf))
			current->childs[current->currKeys + 1] = next->childs[0];
		//mov first key of next tp parent
		keys[index] = next->keys[0];
		int i = 1;
		//shifting next childs key to lef
		for (; i < next->currKeys; ++i)
			next->keys[i - 1] = next->keys[i];
		//shifting next childs childs to lef
		if (!next->leaf) {
			int i = 1;
			for (; i <= next->currKeys; ++i) {
				next->childs[i - 1] = next->childs[i];
			}
		}
		current->currKeys++;
		next->currKeys--;
	}
	//return index of value which is equal to greater then argument
	int FindKey(T value) {
		int idx = 0;
		while (idx < currKeys && keys[idx] < value)
			++idx;
		return idx;
	}
	~BTreeNode() {
		if (MaxKeys > 0) delete[] keys;
		if (MaxKeys >= 0)	delete[] childs;
	}
};

//BTree Class
template<class T>
class BTree:public DataStructure<T> {
	BTreeNode<T>* root;
	int degree;
	//T value is Value which is to be inserted in tree
	//C_Node is pointer to Current Node
	//P_Pointer in Pointer to Parent
	void InsertInTree(T value, BTreeNode<T>* C_Node, BTreeNode<T>* P_Pointer) {
		//check if current Node is leaf then simply insert in it
		if (C_Node->leaf) {
			C_Node->Insert(value);
			//increase no of keys
			C_Node->currKeys++;
		}
		else {
			//find the position where the new keys is going to insert
			int i;
			for (i = C_Node->currKeys - 1; i >= 0 && C_Node->keys[i] > value;) {
				--i;
			}
			InsertInTree(value, C_Node->childs[i + 1], C_Node);
		}
		//Check if C_Node keys exceed from maxKeys
		//then split it
		if (C_Node->currKeys == degree) {
			int mid_index = degree / 2;
			BTreeNode<T>* newNode = new BTreeNode<T>(degree, C_Node->leaf);
			//copy last half keys to the newly created node
			int i;
			for (i = mid_index + 1; i < C_Node->currKeys; ++i) {
				newNode->Insert(C_Node->keys[i]);
				C_Node->keys[i] = T();
				newNode->currKeys++;
			}
			//if Current Node is not leaf then copy half childs to newly Created Node childs
			if (!C_Node->leaf) {
				int j = 0;
				for (i = mid_index + 1; i <= C_Node->currKeys; ++i) {
					newNode->childs[j] = C_Node->childs[i];
					C_Node->childs[i] = NULL;
					++j;
				}
			}
			//If current Node is Root then Create new Node and 
			// Move Middle key  to Parent
			if (!P_Pointer) {//if Parent Doesn't Exits
				//Create A new Node for Root
				BTreeNode<T>* newRoot = new BTreeNode<T>(degree, false);
				newRoot->Insert(C_Node->keys[mid_index]);
				newRoot->currKeys++;
				//Link the seperated childs
				newRoot->childs[0] = C_Node;
				newRoot->childs[1] = newNode;
				root = newRoot;
			}
			else {
				//find position of middle key in parent
				//and new child
				for (i = P_Pointer->currKeys - 1; i >= 0 && P_Pointer->keys[i] > C_Node->keys[mid_index]; --i) {
					P_Pointer->keys[i + 1] = P_Pointer->keys[i];
					P_Pointer->childs[i + 2] = P_Pointer->childs[i + 1];
				}
				P_Pointer->keys[i + 1] = C_Node->keys[mid_index];
				P_Pointer->currKeys++;
				P_Pointer->childs[i + 2] = newNode;
			}
			//adjust current childern in seperated childs
			C_Node->currKeys = degree / 2;
		}
	}
	
	void display(BTreeNode<T>* ptr) {
		if (ptr) {
			if (ptr->leaf) {
				for (int i = 0; i < ptr->currKeys; ++i) {
					cout << ptr->keys[i] << " ";
				}
				return;
			}
			else {
				int i = 0;
				for (; i <= ptr->currKeys; ++i) {
					display(ptr->childs[i]);
					if (i < ptr->currKeys)
						cout << ptr->keys[i] << " ";
				}
			}
		}
	}
	//return T() if not found
	T Search(BTreeNode<T>*& temp,T value){
		int index=temp->FindKey(value);//get index
		if (temp->keys[index ] == value) //if value is at that index then return
			return temp->keys[index];
		else if (temp->childs[index])//else recusively call searchon childs till childs is null
			return Search(temp->childs[index],value);
		else{
			return T();
		}
	}
public:
	BTree(int d) {
		if (d > 2)
			degree = d;
		else
			degree = 3;
		root = NULL;
	}
	void Insert(T value) {
		if (!root) {
			root = new BTreeNode<T>(degree, true);
			root->Insert(value);
			root->currKeys++;
		}
		else {
			InsertInTree(value, root, NULL);
		}

	}
	void Delete(T value) {
		//if root is leaf then immediatly remove from it
		if (!root) {
			return;
		}
		root->Delete(value);
		if (root->currKeys == 0) {
			BTreeNode<T>* temp = root;
			if (root->leaf)
				root = NULL;
			else
				root = root->childs[0];
		}
	}
	T Find(T value) {
		if (root)
			return Search(root, value);
		else {
			return T();
		}
	}
	void MakeNull() {
		while (root)
			Delete(root->keys[0]);
	}
	void Display() {
		display(root);
	}
	~BTree() {
		MakeNull();
	}
};