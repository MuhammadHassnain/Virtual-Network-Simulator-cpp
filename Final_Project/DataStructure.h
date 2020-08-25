#pragma once
template <class T>
class DataStructure {
public:
	DataStructure() {}
	virtual void Insert(T)=0;
	virtual void Delete(T) = 0;
	virtual T Find(T) = 0;
	virtual void MakeNull() = 0;
	virtual ~DataStructure() {}
};