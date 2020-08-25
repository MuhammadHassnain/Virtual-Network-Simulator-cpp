#pragma once
#include<fstream>
#include<sstream>
#include<string>
#include "Device.h"
#include "Router.h"
#include "Machine.h"
#include<string.h>
#include<limits.h>
#include "Stack.h"
class Network {
	int** graph;
	int graph_size;
	Device** dev_arr;
	int total_devices;
	int curr_devices;
	int RoutingTableType;
	void InstallDevices() {
		string firstline, device_name;
		fstream Network_file("network.csv");
		//get first line
		if (Network_file.is_open()) {
			getline(Network_file, firstline, '\n');
			istringstream deviceline(firstline);
			//dismiss first entry
			getline(deviceline, device_name, ',');
			//count total devices in file
			while (getline(deviceline, device_name, ',')) {
				total_devices++;
			}
			//intialize dev_arr
			dev_arr = new Device*[total_devices];
			for (int i = 0; i < total_devices; ++i) {
				dev_arr[i] = nullptr;
			}
			istringstream deviceline1(firstline);
			getline(deviceline1, device_name, ',');
			//Insert Devics in dev_arr
			while (getline(deviceline1, device_name, ',')) {
				//if device is Machine then Creater machine Object
				if ((device_name[0] == 'M') || (device_name[0] == 'm')) {
					dev_arr[curr_devices] = new Machine(device_name);
					curr_devices++;
				}
				else {
					//if device is Router then Creater Router Object
					dev_arr[curr_devices] = new Router(RoutingTableType, device_name);
					curr_devices++;
				}
			}
		}
		else {
			cout << "File Not found, Can't Install Devices" << endl;
		}
		Network_file.close();
	}
	//return index of minimum edge
	int MinDist(int*& distance, bool*& Visited)
	{
		// Initialize min value
		int min = INT_MAX, min_index;

		for (int i = 0; i < graph_size; i++) {
			if (distance[i] <= min && Visited[i] == false)
				min = distance[i], min_index = i;
		}
		return min_index;
	}
	void dijkstra(int source) {
		int* distance = new int[graph_size];//store shortest distance from source
											//distance[i]
		bool* Visited = new bool[graph_size]; //value of those index are true if 
												//they have been visited
				
		int* parent = new int[graph_size];		//used to store parent of edges 
											//parent [0]=6 means parent of 0 is 6
		//sent all parent to zero
		//visited to false
		//and distance maximum
		for (int i = 0; i < graph_size; ++i) {
			parent[i] = -1;
			Visited[i] = false;
			distance[i] = INT_MAX;
		}
		//set source distance to 0
		//source is edge where you want to apply dijkestra
		distance[source] = 0;

		for (int i = 0; i < graph_size - 1; ++i) {
			int u = MinDist(distance, Visited);   //get index of minimum edge
			Visited[u] = true;						//mark it visited
			//update its parent and distance
			//to connected router and devices
			for (int v = 0; v < graph_size; ++v) {
				if (!Visited[v] && graph[u][v] && distance[u] + graph[u][v] < distance[v]) {
					parent[v] = u;
					distance[v] = distance[u] + graph[u][v];
				}
			}
		}
		//print all path from src to each other vertiex
		printSolution(distance, graph_size, parent, source);
		delete[]distance;
		delete[]Visited;
		delete[]parent;
	}
	//recursivily call the function to get path of devices
	//parent is array of parent 
	//stack is used to store path top is path of next 
	//best router
	void printPath(int*& parent, int j, Stack<int>*& s)
	{
		// Base Case : If j is source
		if (parent[j] == -1)
			return;
		s->Push(j);
		printPath(parent, parent[j], s);
		cout << dev_arr[j]->GetName() << " ";
	}
	void displayGraph() {
		for (int i = 0; i < graph_size; ++i) {
			for (int j = 0; j < graph_size; ++j) {
				cout << graph[i][j] << " ";
			}
			cout << endl;
		}

	}
	//go through all devices 
	//if device is router then it 
	//destory previous routing table
	//and create new routing table 
	void ReDijkstra() {
		string n;
		for (int i = 0; i < graph_size; ++i) {
			n = dev_arr[i]->GetName();
			if (n[0] == 'R') {
				dev_arr[i]->DestroyRoutingTable();
				dijkstra(i);
			}
		}
	}

	void printSolution(int*& dist, int n, int*& parent, int src)
	{
		Stack<int>* path = new Stack<int>;
		printf("Vertex\t  Distance\tPath");
		for (int i = 0; i < graph_size; i++)
		{
			if (i != src) {
				cout << "\n" << dev_arr[src]->GetName() << "--->" << dev_arr[i]->GetName() << "\t" << dist[i] << "\t" << dev_arr[src]->GetName() << " ";
				printPath(parent, i, path);
				dev_arr[src]->AddRoutingEntry(i, path->peek());//stack top is next best router
				path->MakeNull();
			}
		}
		cout << endl;
		delete path;
	}
	void IntializeGraph() {
		graph = new int*[graph_size];
		for (int i = 0; i < graph_size; ++i) {
			graph[i] = new int[graph_size];
			for (int j = 0; j < graph_size; ++j) {
				graph[i][j] = 0;
			}
		}
		string s;
		fstream network("network.csv");
		if (network.is_open()) {
			//neglect first line
			getline(network, s, '\n');
			//setting edge between devices
			int i = 0;
			for (; i < graph_size; ++i) {
				getline(network, s, ',');
				int j = 0;
				for (; j < graph_size - 1; ++j) {
					getline(network, s, ',');
					graph[i][j] = atoi(s.c_str());
					cout << graph[i][j] << " ";
				}
				getline(network, s, '\n');
				graph[i][j] = atoi(s.c_str());
				cout << graph[i][j] << endl;
			}
		}
		else {
			cout << "File can't be open" << endl;
		}
		network.close();
	}
public:
	Network(int a) {
		RoutingTableType = a;
		curr_devices = 0;
		InstallDevices();
		graph_size = total_devices;
		IntializeGraph();
		string s;
		for (int i = 0; i < graph_size; ++i) {
			s = dev_arr[i]->GetName();
			if (s[0] == 'R') {
				//if a device is router then apply dijkstran on it
				dijkstra(i);
				cout << endl;
			}
		}
	}
	//return router which is connected to 
	//given machine id
	int GetConnectedRouter(int MachineId) {
		for (int i = 0; i < graph_size; ++i) {
			if (graph[MachineId][i] > 0)
				return i;
		}
		return -1;
	}
	//return id of device
	int GetDeviceId(string s) {
		for (int i = 0; i < graph_size; ++i) {
			if (dev_arr[i]->GetName() == s)
				return i;
		}
		return -1;
	}
	//a procedutre ot send msg
	//input a file name
	//open it 
	//and send the msg according to data
	void SendMsg(string f) {
		fstream file(f, ios::in);
		if (file.is_open()) {
			string id, priority, src, dest, payload;
			getline(file, id, '\n');
			while (getline(file, id, ':')) {
				getline(file, priority, ':');
				getline(file, src, ':');
				getline(file, dest, ':');
				getline(file, payload, '\n');
				Message msg(atoi(id.c_str()), atoi(priority.c_str()), src, dest, payload);
				int sendingdevice = GetDeviceId(msg.source);
				int receiver = GetDeviceId(msg.dest);
				string source = msg.source;
				string dest = msg.dest;
				if (sendingdevice >= 0 && source[0] == 'M' && receiver >= 0 && dest[0] == 'M') {
					dev_arr[sendingdevice]->SendMsg(msg);
					cout << "Sending" << endl;
				}
			}
		}
	}
	void Routing() {
		for (int i = 0; i < graph_size; ++i) {
			//transfer all the message of outgoing queue to the next 
			//appropriate router or maching
			Message OutMsg;
			string n;

			if (!dev_arr[i]->OutGoingEmpty()) {
				dev_arr[i]->OutGoingMsg(OutMsg);	//get outgoing msg of dev[i]
				n = dev_arr[i]->GetName();			//get device 
				if (n[0] == 'M')					//if device is machine then getconnected router to it and put msg in its incoming
					dev_arr[GetConnectedRouter(i)]->InComingMsg(OutMsg);
				else if (n[0] == 'R') {
					int NextDevice, destination = GetDeviceId(OutMsg.dest);
					//if device exists
					if (destination >= 0) {
						dev_arr[i]->GetRoutingEntry(destination, NextDevice); //GetRoutingEntry Match destination in Routing Table 
																				//and return NextDevice
						dev_arr[NextDevice]->InComingMsg(OutMsg);				//add msg to incoming of that device
					}
				}

			}

		}
		//transfer a the message from incoming queue to the outgoing queue
		for (int i = 0; i < graph_size; ++i)
			dev_arr[i]->RouterInnerTransfer();

	}
	void AddNewDevice(string name) {

		Device** temp = new Device*[graph_size];
		//copy previous graph
		int** temp_graph = new int*[graph_size];
		for (int i = 0; i < graph_size; ++i) {
			temp_graph[i] = new int[graph_size];
			for (int j = 0; j < graph_size; ++j) {
				temp_graph[i][j] = graph[i][j];
			}
		}
		//copying previous devices
		for (int i = 0; i < graph_size; ++i) {
			temp[i] = dev_arr[i];
		}

		delete[] dev_arr;//deallocate memory of previous devices
		for (int i = 0; i < graph_size; ++i) {
			delete[] graph[i];
		}
		delete[] graph;
		//allocate memory for prev+1 devices
		dev_arr = new Device*[graph_size + 1];
		//copying previous devices into dev_arr
		int i, j;
		for (i = 0; i < graph_size; ++i) {
			dev_arr[i] = temp[i];
		}
		//inserting new device
		if (name[0] == 'M')
			dev_arr[i] = new Machine(name);
		else if (name[0] == 'R')
			dev_arr[i] = new Router(RoutingTableType, name);
		//allocating memory for graph_size+1 size of graph
		graph = new int*[graph_size + 1];
		for (int i = 0; i < graph_size + 1; ++i) {
			graph[i] = new int[graph_size + 1];
			for (int j = 0; j < graph_size + 1; ++j) {
				if (i >= graph_size || j >= graph_size)
					graph[i][j] = 0;
				else
					graph[i][j] = temp_graph[i][j];
			}
		}
		//de allocating temporary memory
		delete[] temp;
		for (int i = 0; i < graph_size; ++i)
			delete[] temp_graph[i];
		delete[] temp_graph;
		graph_size++;
		curr_devices++;
		displayGraph();
		ReDijkstra();
	}
	void printDeviceArray() {
		for (int i = 0; i < curr_devices; ++i) {
			cout << (*(dev_arr[i])).GetName() << endl;
		}
	}
	void PrintPath(string src, string dest) {
		fstream file;
		string id, s, d, t, msg;
		string line, line1;
		file.open("path.csv", ios::in);
		if (file.is_open()) {
			getline(file, line, '\n');
			while (getline(file, line, '\n')) {
				istringstream myline(line);
				int i = 0;
				while (getline(myline, line1, ',')) {
					if (i == 0)id = line1;
					else if (i == 1)s = line1;
					else if (i == 2)d = line1;
					else if (i == 3)t = line1;
					else if (i == 4)msg = line1;

					i++;
				}
				if (src == "*") {
					if (dest == d)
						cout << t << endl;

				}
				else if (dest == "*") {
					if (src == s)
						cout << t << endl;
				}
				else {
					if (dest == d && src == s)
						cout << t << endl;
				}
			}
		}
		else {
			cout << "Path file can't be opened" << endl;
		}
		file.close();
	}
	void ChangeEdge(string src, string dest, int wei) {
		int s = GetDeviceId(src);
		int d = GetDeviceId(dest);
		if (s == -1) {   //if new device is to be added
			AddNewDevice(src);
		}
		if (d == -1) {
			AddNewDevice(dest);
		}
		s = GetDeviceId(src);
		d = GetDeviceId(dest);
		if (s >= 0 && d >= 0) {
			graph[s][d] = wei;
			graph[d][s] = wei;
			ReDijkstra();
		}
	}
	void ChangeEdge(string f) {
		fstream file(f, ios::in);
		string l;
		const int arrsize = 50;
		int currentsize = 0;
		if (file.is_open()) {
			getline(file, l, '\n');
			istringstream firstline(l);
			//breaking first line
			string arr[arrsize];
			string name;
			getline(firstline, name, ',');//descarding first line
			while (getline(firstline, name, ',')) {
				arr[currentsize] = name;
				int deviceid = GetDeviceId(name);
				if (deviceid == -1)
					AddNewDevice(name);
				currentsize++;
			}
			while (getline(file, l, '\n')) {
				istringstream f(l);
				getline(f, name, ',');
				int srcid = GetDeviceId(name);
				if (srcid == -1)
					AddNewDevice(name);
				string weight;
				int i = 0;
				while (getline(f, weight, ',')) {
					int src = GetDeviceId(name);
					string s = arr[i];
					int dest = GetDeviceId(s);
					if (src >= 0 && dest >= 0) {
						graph[src][dest] = atoi(weight.c_str());
						graph[dest][src] = graph[src][dest];
					}
					++i;
				}	
			}

		}
		else {
			cout << "Edge file can't be opened" << endl;
		}
		file.close();
		ReDijkstra();
	}
	void AddInRoutingTable(string r,string d, string n) {
		int router, dest, next;
		if (r[0] == 'R') {
			router = GetDeviceId(r);
			dest = GetDeviceId(d);
			next = GetDeviceId(n);
			if (router >= 0 && dest >= 0 && next >= 0) {
				dev_arr[router]->AddRoutingEntry(dest, next);
			}
		}
	}
	void AddInRoutingTable(string r,string f) {
		fstream file(f, ios::in);
		string next, dest;
		if (file.is_open()) {
			getline(file, next, '\n');
			while (getline(file, dest, ',')) {
				getline(file, next, ',');
				AddInRoutingTable(r, dest, next);
			}
		}
		else {
			cout << "Can't Open File" << r << endl;
		}
	}
	void DeleteFromRoutingTable(string r, string d, string n) {
		int router, dest, next;
		if (r[0] == 'R') {
			router = GetDeviceId(r);
			dest = GetDeviceId(d);
			next = GetDeviceId(n);
			if (router >= 0 && dest >= 0 && next >= 0) {
				dev_arr[router]->DeleteRoutingEntry(dest, next);
			}
		}
	}
	void DeleteFromRoutingTable(string r, string f) {
		fstream file(f, ios::in);
		string next, dest;
		if (file.is_open()) {
			getline(file, next, '\n');
			while (getline(file, dest, ',')) {
				getline(file, next, ',');
				DeleteFromRoutingTable(r, dest, next);
			}
		}
		else {
			cout << "Can't Open File:" <<r<< endl;
		}
	}
	~Network() {
		if (graph_size > 0){
			for (int i = 0; i < graph_size; ++i) {
				delete dev_arr[i];
				delete []graph[i];
			}
			delete[]dev_arr;
			delete[]graph;
		}
	}
};