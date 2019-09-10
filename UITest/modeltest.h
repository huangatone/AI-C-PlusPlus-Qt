#ifndef MODELTEST_H
#define MODELTEST_H

#include <iostream>
#include <sstream>
#include <list>
#include <assert.h>
using namespace std;

class ModelTest
{
	enum T_Type
	{
		NUMBER,
		STRING
	};
	union U_Tyoe{
		int number;
		string str;
	};

public:
	ModelTest();
};


class Single
{

	Single();
	Single* _instance;
public:
	Single* getInstance();
	virtual ~Single();
};




#endif // MODELTEST_H
