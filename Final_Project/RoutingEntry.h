#pragma once
#include<iostream>
using namespace std;
struct  RoutingEntry{
	int destination;//destinaiton of Message
	int nextDevice;//Next bext Router

	RoutingEntry(int d = -1, int n = -1) {
		destination = d;
		nextDevice = n;
	}
	bool operator == (RoutingEntry& r) {
		if (r.nextDevice < 0)
			return destination == r.destination;
		else
			return destination == r.destination && nextDevice == r.nextDevice;
	}
	bool operator < (RoutingEntry& r) {
		
		return destination < r.destination;
	}
	bool operator > (RoutingEntry& r) {
		return destination > r.destination;
	}
	bool operator!=(RoutingEntry& r) {
		return destination != r.destination;
	}
	friend ostream& operator << (ostream& out, RoutingEntry& obj) {
		cout << obj.destination << " " << obj.nextDevice << endl;
		return out;
	}
};