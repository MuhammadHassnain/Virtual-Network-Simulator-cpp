#pragma once
#include "Device.h"
#include "LinkedList.h"
#include "Heap.h"
#include "Queue.h"
#include "Message.h"
#include "RoutingEntry.h"
#include "DataStructure.h"
#include "Splay_Tree.h"
#include "BTree.h"
class Router :public Device {
	Heap<Message> Incoming;						//Priority QUeue Used to store msg on base of high Priority
	Queue<Message> OutGoing;					//Simple First in First OUt QUeue Used to store Outgoing Msg
	DataStructure<RoutingEntry>* RoutingTable;  //Data structure for Routing Table
	int tabletype;								//type of data structure used in routing table
public:
	Router(int a, string s = "") :Device(s) {
		tabletype = a;
		if (tabletype == 1)RoutingTable = new LinkedList<RoutingEntry>;			
		else if (tabletype == 2)RoutingTable = new SplayTree < RoutingEntry >;
		else if (tabletype == 3)RoutingTable = new BTree<RoutingEntry>(5);
		else RoutingTable = new SplayTree<RoutingEntry>;
		tabletype = 2;
	}
	//Function to delete A Routing Entry from Routing Table
	//dest is destination and next is next best Router
	virtual void DeleteRoutingEntry(int dest, int next) {
		RoutingTable->Delete(RoutingEntry(dest, next));
	}
	//Function to Add A Routing Entry in ROuting table 
	//dest is destination and next is next best Router
	virtual void AddRoutingEntry(int dest, int next) {
		RoutingEntry r(dest, next);
		RoutingTable->Insert(r);
	}
	virtual void viewRouter() {
		cout << RoutingTable << endl;
	}
	// Procedure take A msg from Heap 
	//add trace to it 
	//transfer it to outgoing Queuw
	virtual void RouterInnerTransfer() {
		if (!Incoming.is_empty()) {
			Message m = Incoming.deleteMin();
			m.trace += GetName();
			OutGoing.enqueue(m);
		}
	}
	//Get Outgoing Message in M
	virtual void OutGoingMsg(Message& M) {
		M = OutGoing.dequeue();
	}
	//Insert incoming msg to heap
	virtual void InComingMsg(Message M) {
		Incoming.Insert(M);
	}

	virtual bool OutGoingEmpty() {
		return OutGoing.isEmpty();
	}
	virtual bool InComingEmpty() {
		return Incoming.is_empty();
	}
	//Dest is Provided 
	//Function Find Next Router 
	//and return in NextRouter&
	virtual void GetRoutingEntry(int dest, int& NextRouter) {
		RoutingEntry r(dest);
		r = RoutingTable->Find(r);
		NextRouter = r.nextDevice;
	}
	//Delete All Routing Entries of Router
	virtual void DestroyRoutingTable() {
		if(RoutingTable)
			RoutingTable->MakeNull();
	}
	virtual ~Router() {
		DestroyRoutingTable();
	}
};