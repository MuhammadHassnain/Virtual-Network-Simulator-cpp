// FinalProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
// A C / C++ program for Dijkstra's single source shortest
// path algorithm. The program is for adjacency matrix
// representation of the graph.
#include <stdio.h>
#include <limits.h>
#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include <sstream>
#include <string>
#include "Stack.h"
#include "Network.h"
#include "Message.h"
#include <Windows.h>
#include <process.h>
using namespace std;

HANDLE h_mainThread, h_timer = NULL;
bool ST = false;
Network* mynetwork;
int Device::device_count = 0;
void timer(void *) {
	while (!ST) {
		//Keep main thread alive for 1 sec.
		Sleep(    // minutes to sleep
			1 *   // seconds to a minute
			1000); // milliseconds to a second;
		mynetwork->Routing();
	}
}
//function to check that file has extension are not
bool is_file(string s) {
	if(s.size()>=4)
		return s[s.size() - 4] == '.';
	return false;
}
// function to change routing table
void Change_RT() {
	cout << "1.ADD!" << endl;
	cout << "2.REMOVE!" << endl;
	int num;
	cin >> num;
	if (num == 1) {
		bool loop = true;
		do {
			cout << "1. ENTER DATA DIRECTLY" << endl;
			cout << "2.ENTER BY FILE" << endl;
			int type;
			cin >> type;
			if (type == 1) {
				string router, dest, next;
				cout << "ENTER ROUTER:";
				cin >> router;
				cout << "ENTER DESTINATION:";
				cin >> dest;
				cout << "ENTER NEXT MACHINE";
				cin >> next;
				mynetwork->AddInRoutingTable(router, dest, next);
			}
			else if (type == 2) {
				string router, file;
				cout << "ENTER ROUTER NAME!" << endl;
				cin >> router;
				cout << "ENTER FILE NAME!" << endl;
				cin >> file;
				mynetwork->AddInRoutingTable(router, file);
			}
			char add;
			cout << "DO YOU WANT TO DO ADD ANOTHER Y/N" << endl;
			cin >> add;

			if (add == 'Y' || add == 'y') {}
			else loop = false;
		} while (loop);
	}
	else if (num == 2) {
		bool loop = true;
		do {
			cout << "1. ENTER DATA DIRECTLY" << endl;
			cout << "2.ENTER BY FILE" << endl;
			int type;
			cin >> type;
			if (type == 1) {
				string router, dest, next;
				cout << "ENTER ROUTER:";
				cin >> router;
				cout << "ENTER DESTINATION:";
				cin >> dest;
				cout << "ENTER NEXT MACHINE";
				cin >> next;
				mynetwork->DeleteFromRoutingTable(router, dest, next);
			}
			else if (type == 2) {
				string router, file;
				cout << "ENTER ROUTER NAME!" << endl;
				cin >> router;
				cout << "ENTER FILE NAME!" << endl;
				cin >> file;
				mynetwork->DeleteFromRoutingTable(router, file);
			}
			char add;
			cout << "DO YOU WANT TO DO ADD ANOTHER Y/N" << endl;
			cin >> add;

			if (add == 'Y' || add == 'y') {}
			else loop = false;
		} while (loop);
	}
}
// function to change edge weight
void Change_Edge_Weight() {
	bool loop = true;
	do {
		cout << "1. ENTER DATA DIRECTLY" << endl;
		cout << "2.ENTER BY FILE" << endl;
		int type;
		cin >> type;
		if (type == 1) {
			string rout1, rout2;
			int weight;
			cout << "ENTER NAME OF FIRST ROUTER!" << endl;
			cin >> rout1;
			cout << "ENTER NAME OF SECOND ROUTER!" << endl;
			cin >> rout2;
			cout << "ENTER THE NEW WEIGHT BETWEEN ABOVE ROUTERS!" << endl;
			cin >> weight;
			mynetwork->ChangeEdge(rout1, rout2, weight);
		}
		else if (type == 2) {
			string filename;
			cin >> filename;
			mynetwork->ChangeEdge(filename);
		}
		char edge;
		cout << "DO YOU WANT TO DO CHANGE ANOTHER EDGE Y/N" << endl;
		cin >> edge;
		if (edge == 'Y' || edge == 'y') {}
		else loop = false;
	} while (loop);
}
void PrintPath() {
	string m1, m2;
	cout << "Enter First Machine Name:";
	cin >> m1;
	cout << "Enter Second Machine Name:";
	cin >> m2;
	mynetwork->PrintPath(m1, m2);
}

// Number of vertices in the graph



int main() {
	int datastructure;
	cout << "Enter 1. LinkedList\n2. Splay Tree\n3. B Tree:";
	cin >> datastructure;
	if (datastructure > 0 && datastructure < 4)
		mynetwork = new Network(datastructure);
	else
		return 0;
	_beginthread(timer, 0, NULL); // start timer thread
	//creater a file to store path
	fstream file;
	file.open("path.csv", ios::out);
	file << "MessageID," << "Source," << "Destination," << "Trace," << "Message\n";
	file.close();
	int choice;
	cout << "1.FOR OPTION BASED INTERFACE!" << endl;
	cout << "2.FOR COMMAND LINE INTERFACE!" << endl;
	cin >> choice;
	if (choice == 1) {
		bool loop = true;
		do {
			int num;
			cout << "1.SEND MESSAGE!" << endl;
			cout << "2.CHANGE ROUTING TABLE!" << endl;
			cout << "3.CHANGE EDGE WEIGHT!" << endl;
			cout << "4.PRINT PATH!" << endl;
			cout << "-1.TO EXIT!" << endl;
			cin >> num;
			if (num == 1) {
				string file;
				cout << "Enter file Name:" << endl;
				cin >> file;
				mynetwork->SendMsg(file);
				cin.ignore();
			}
			else if (num == 2) {
				Change_RT();
			}
			else if (num == 3) {
				Change_Edge_Weight();
			}
			else if (num == 4) {
				PrintPath();
			}
			else if (num == -1) {
				loop = false;
			}
		} while (loop);
	}
	else if (choice == 2) {
		bool loop = true;
		cin.ignore();
		do {
			bool perform = true;
			string str, com;
			cout << "Enter Your Command:";
			getline(cin, str);
			cout << str << endl;
			istringstream a(str);
			int count = 0;
			string op;
			getline(a, com, ' ');
			if (com == "print") {
				string r1, r2;
				getline(a, r1, ' ');
				getline(a, r1, ' ');
				getline(a, r2, ' ');
				getline(a, r2, ' ');
				cout << r1 << " " << r2 << endl;
				perform = false;
				mynetwork->PrintPath(r1, r2);
			}
			else if (com == "change") {
				string com1;
				getline(a, com1, ' ');
				if (com1 == "RT") {
					string router, operation;
					getline(a, router, ' ');
					getline(a, operation, ' ');
					if (operation == "add") {
						string filename;
						getline(a, filename, ' ');
						if (is_file(filename)) {
							perform = false;
							mynetwork->AddInRoutingTable(router, filename);
						}
						else {
							string next;
							getline(a, next, ' ');
							perform = false;
							mynetwork->AddInRoutingTable(router, filename, next);
						}
					}
					else if (operation == "remove") {
						string filename;
						getline(a, filename, ' ');
						if (is_file(filename)) {
							perform = false;
							mynetwork->DeleteFromRoutingTable(router, filename);
						}
						else {
							string next;
							getline(a, next, ' ');
							perform = false;
							mynetwork->DeleteFromRoutingTable(router, filename, next);
						}
					}
				}
				else if (com1 == "edge") {
					string filename;
					getline(a, filename, ',');
					if (is_file(filename)) {
						perform = false;
						mynetwork->ChangeEdge(filename);
					}
					else {
						string machine2, weight;
						if(getline(a, machine2, ',') ){
							if(getline(a, weight, ','))
								mynetwork->ChangeEdge(filename, machine2, atoi(weight.c_str()));
							perform = false;
						}
					}
				}
			}
			else if (com == "send") {
				ST = TRUE;
				Sleep(1000);
				string com1;
				getline(a, com1, ' ');
				if (com1 == "msg") {
					getline(a, com1, ' ');
					mynetwork->SendMsg(com1);
					perform = false;
					cout << com1 << endl;
				}
				ST = FALSE;
				_beginthread(timer, 0, NULL);
				
			}
			else if (com == "exit") {
				loop = false;
				perform = false;
			}
			if (perform)cout << "YOU ENTER THE WRONGE COMMAND!" << endl;
		} while (loop);
	}
	ST = TRUE;
	Sleep(2000);
	delete mynetwork;
	system("Pause");
	return 0;
}