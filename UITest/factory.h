#ifndef FACTORY_H
#define FACTORY_H

#include <iostream>
#include <sstream>
#include <list>

#include "product.h"

using namespace std;
class Factory
{
public:
	Factory();
};

///
/// \brief 简单工厂
///
class SimpleFactory
{
public:
	SimpleFactory();
	~SimpleFactory();

	BaseObject* createObject(int type);

};

///
/// \brief 工厂模式
///
class BaseFactory
{
public:
	BaseFactory();
	virtual ~BaseFactory();
	virtual BaseObject* createObject() = 0;
};


class FactoryA: public BaseFactory
{
public:
	FactoryA();
	virtual ~FactoryA();
	virtual BaseObject* createObject();
};



class FactoryB: public BaseFactory
{
public:
	FactoryB();
	virtual ~FactoryB();
	virtual BaseObject* createObject();
};

class FactoryC: public BaseFactory
{
public:
	FactoryC();
	virtual ~FactoryC();
	virtual BaseObject* createObject();
};


class FactoryD: public BaseFactory
{
public:
	FactoryD();
	virtual ~FactoryD();
	virtual BaseObject* createObject();
};


///
///抽象工厂模式
///
///
class AbstrctFactory
{
public:
	AbstrctFactory()
	{

	}
	virtual ~AbstrctFactory()
	{

	}

	virtual BaseObject* createObject() = 0;
	virtual BaseDelegate* createDelegate() = 0;
};


class AbstrctFactoryA: public AbstrctFactory
{
public:
	AbstrctFactoryA()
	{

	}
	virtual ~AbstrctFactoryA()
	{

	}

	virtual BaseObject* createObject() { return new ObjectA();}
	virtual BaseDelegate* createDelegate() { return new DelegateA(); }
};


class AbstrctFactoryB: public AbstrctFactory
{
public:
	AbstrctFactoryB()
	{

	}
	virtual ~AbstrctFactoryB()
	{

	}

	virtual BaseObject* createObject() { return new ObjectB();}
	virtual BaseDelegate* createDelegate() { return new DelegateB(); }
};


class AbstrctFactoryC: public AbstrctFactory
{
public:
	AbstrctFactoryC()
	{

	}
	virtual ~AbstrctFactoryC()
	{

	}

	virtual BaseObject* createObject() { return new ObjectC();}
	virtual BaseDelegate* createDelegate() { return new DelegateC(); }
};


class AbstrctFactoryD: public AbstrctFactory
{
public:
	AbstrctFactoryD()
	{

	}
	virtual ~AbstrctFactoryD()
	{

	}

	virtual BaseObject* createObject() { return new ObjectD();}
	virtual BaseDelegate* createDelegate() { return new DelegateD(); }
};


template<class A, class B>
class BaseAbstractFactory
{
public:
	BaseAbstractFactory()
	{

	}
	virtual ~BaseAbstractFactory()
	{

	}
	A* createA() { return new A();}
	B* createB() { return new B();}
};

class ModelFactory
{
public:
	ModelFactory()
	{

	}
	BaseObject* obj;
	BaseDelegate* delegate;
	void run()
	{
		cout << endl << "ModelFactory running" <<endl;
		obj->run();
		delegate->run();
	}
};


#endif // FACTORY_H
