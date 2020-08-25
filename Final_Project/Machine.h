#pragma once
#include "Device.h"
#include "Heap.h"
#include "Queue.h"
#include "Message.h"
#include "LinkedList.h"
#include<fstream>
class Machine :public Device {
	Heap<Message> OutGoing;   //Outgoing to store Message going from that Machine
	fstream file;				//used to store path of received Message in file
public:
	Machine(string s=""):Device(s){
	}

	virtual void OutGoingMsg(Message& M) {
		M = OutGoing.deleteMin();
	}
	virtual void InComingMsg(Message M) {
		M.trace += GetName();
		file.open("path.csv", ios::app);
		if (file.is_open()) {
			file << M.msg_id <<","<<M.source<<","<<M.dest<<","<<M.trace<<","<<M.msg<<"\n";
		}
		file.close();
	}
	virtual void SendMsg(Message M) {
		M.trace += GetName();
		OutGoing.Insert(M);
	}
	virtual bool OutGoingEmpty() {	
		return OutGoing.is_empty();
	}
	virtual bool InComingEmpty() {
		return true;
	}

	virtual void AddRoutingEntry(int,int) {}
	virtual ~Machine() {}
};