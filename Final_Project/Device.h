#pragma once
#include<string>
#include<iostream>
#include "RoutingEntry.h"
#include "Message.h"
using namespace std;
class Device {
	string name;  //device name
			
	int id;			//unique id
public:
	static int device_count;
	Device(string s = "") {
		name = s;
		id = device_count;
		device_count++;
	}
	int GetDeviceId() {
		return id;
	}
	void SetName(string s) {
		name = s;
	}
	string GetName() {
		return name;
	}
	virtual void OutGoingMsg(Message&) {}
	virtual void InComingMsg(Message) {}
	virtual void RouterInnerTransfer() {} // only for router
	virtual void SendMsg(Message) {}//only for machine
	virtual void viewRouter() {}
	virtual bool OutGoingEmpty() = 0;
	virtual bool InComingEmpty() = 0;
	virtual void AddRoutingEntry(int, int) {}//only for router
	virtual void GetRoutingEntry(int, int&) {}//only for router
	virtual void DestroyRoutingTable(){}
	virtual void DeleteRoutingEntry(int,int){}//only for router
	friend ostream& operator << (ostream& out, Device& obj) {
		cout << obj.GetName() << "\t" << obj.GetDeviceId();
		return out;
	}
	virtual ~Device() {}
};