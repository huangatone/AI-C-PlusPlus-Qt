#include "factory.h"

Factory::Factory()
{

}
BaseFactory::BaseFactory()
{

}

BaseFactory::~BaseFactory()
{

}

SimpleFactory::SimpleFactory(){}

SimpleFactory::~SimpleFactory(){}

BaseObject *SimpleFactory::createObject(int type)
{
	switch (type) {
	case 0:
		return new ObjectA();
	case 1:
		return new ObjectB();
	case 2:
		return new ObjectC();
	case 3:
		return new ObjectD();
	default:
		return nullptr;
	}
}

FactoryA::FactoryA()
{

}

FactoryA::~FactoryA()
{

}

BaseObject *FactoryA::createObject()
{
	return new ObjectA();
}

FactoryB::FactoryB()
{

}

FactoryB::~FactoryB()
{

}

BaseObject *FactoryB::createObject()
{
	return new ObjectB();
}

FactoryC::FactoryC()
{

}

FactoryC::~FactoryC()
{

}

BaseObject *FactoryC::createObject()
{
	return new ObjectC();
}

FactoryD::FactoryD()
{

}

FactoryD::~FactoryD()
{

}

BaseObject *FactoryD::createObject()
{
	return new ObjectD();
}
