#ifndef PRODUCT_H
#define PRODUCT_H
#include <iostream>
#include <sstream>
#include <list>

using namespace std;

class Product
{
public:
	Product();
};

class BaseDelegate
{
public:
	BaseDelegate()
	{

	}
	virtual ~BaseDelegate()
	{

	}
	virtual string getName() = 0;
	virtual void run()
	{
		cout << "I am  " << getName() << endl;
	}


};


class DelegateA: public BaseDelegate
{
public:
	DelegateA()
	{

	}
	virtual ~DelegateA()
	{

	}
	string getName()
	{
		return "Delegate A";
	}
};

class DelegateB: public BaseDelegate
{
public:
	DelegateB()
	{

	}
	virtual ~DelegateB()
	{

	}
	string getName()
	{
		return "Delegate B";
	}

};

class DelegateC: public BaseDelegate
{
public:
	DelegateC()
	{

	}
	virtual ~DelegateC()
	{

	}
	string getName()
	{
		return "Delegate C";
	}

};

class DelegateD: public BaseDelegate
{
public:
	DelegateD()
	{

	}
	virtual ~DelegateD()
	{

	}
	string getName()
	{
		return "Delegate D";
	}

};


class  BaseObject
{
public:
	BaseObject();
	virtual ~BaseObject();

	virtual string getName() = 0;
	virtual void run()
	{
		cout << "I am  " << getName()<< endl;
	}

};



class  ObjectA: public BaseObject
{
public:
	ObjectA();
	virtual ~ObjectA();

	string getName()
	{
		return "Object A";
	}

	string Test() const
	{
		cout << "Const Test: "  << endl;
	}
};

class  ObjectB: public BaseObject
{
public:
	ObjectB();
	virtual ~ObjectB();

	string getName()
	{
		return "Object B";
	}
};

class  ObjectC: public BaseObject
{
public:
	ObjectC();
	virtual ~ObjectC();

	string getName()
	{
		return "Object C";
	}
};

class  ObjectD: public BaseObject
{
public:
	ObjectD();
	virtual ~ObjectD();

	string getName()
	{
		return "Object D";
	}
};




#endif // PRODUCT_H
