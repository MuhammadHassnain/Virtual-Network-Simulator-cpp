#pragma once
//this file store a string and priority of that string
#include<iostream>
#include<string>
using namespace std;
struct Message {
	std::string msg;
	int priority;
	int msg_id;
	std::string source;
	std::string dest;
	std::string trace;
	//construct
	Message() {
		msg = "";
		priority = -1;
		msg_id = -1;
		source = "";
		dest = "";

		trace = "";
	}
	Message(int i,int pri,string s,string d,string p) {
		msg = p;
		priority = pri;
		source = s;
		dest = d;
		trace = "";
		msg_id = i;
	}
	bool operator !=(const Message& m) {
		return this->priority != m.priority;
	}
	bool operator !=(const Message& m)const {
		return this->priority != m.priority;
	}

	bool operator < (const Message& m) {
		return this->priority < m.priority;
	}
	bool operator < (const Message& m) const{
		return this->priority < m.priority;
	}
	bool operator >(const Message& m) {
		return this->priority > m.priority;
	}
	bool operator == (const Message& m) {
		return this->priority == m.priority;
	}
	bool operator <=(const Message& m) {
		return (*this < m) || (*this == m);
	}
	bool operator >=(const Message& m) {
		return (*this > m) || (*this == m);
	}
	friend ostream& operator << (ostream& out, const Message& m) {
		out << "Message-->" << m.msg << endl;
		out << "Priority-->" << m.priority;
		return out;
	}
	~Message() {}

};