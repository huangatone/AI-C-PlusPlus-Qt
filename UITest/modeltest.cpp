#include "modeltest.h"
#include "factory.h"
ModelTest::ModelTest()
{
	///
	/// \brief Test simple factory
	///
	list<BaseObject*> lt;
	SimpleFactory f;
	lt.push_back( f.createObject(0));
	lt.push_back( f.createObject(3));
	lt.push_back( f.createObject(1));
	lt.push_back( f.createObject(2));


	///
	/// \brief Test  factory mode
	///
	FactoryA a;
	lt.push_back(a.createObject());
	FactoryB b;
	lt.push_back(b.createObject());
	FactoryC c;
	lt.push_back(c.createObject());
	FactoryD d;
	lt.push_back(d.createObject());

	///abstractly factory;
	///
	///
	AbstrctFactoryA aa;
	lt.push_back(aa.createObject());
	aa.createDelegate()->run();


	AbstrctFactoryC cc;
	lt.push_back(cc.createObject());
	cc.createDelegate()->run();


	BaseAbstractFactory<ObjectA,DelegateA> ff;
	lt.push_back(ff.createA());

	BaseAbstractFactory<ObjectA,DelegateA> fd;
	lt.push_back(fd.createA());

	ObjectA* oa = new ObjectA ();
	ObjectB* ob = new ObjectB ();
	ObjectC* oc = new ObjectC ();
	ObjectD* od = new ObjectD ();

	lt.push_back(oa);
	lt.push_back(ob);
	lt.push_back(oc);
	lt.push_back(od);

	for(auto obj: lt)
		obj->run();

	ModelFactory fff;
	fff.obj = oa;
	fff.delegate = new DelegateA();
	fff.run();

	const ObjectA* ddd = oa;
	//ddd->Test();


}

Single::Single()
{
	_instance = nullptr;
}

Single *Single::getInstance()
{
	if(_instance == nullptr)
		_instance = new Single () ;
	return _instance;
}

Single::~Single()
{

}




