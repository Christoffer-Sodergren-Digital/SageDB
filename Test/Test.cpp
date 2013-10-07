// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include "Collection.h"
#include "Expression.h"

using namespace SageDB;

int _tmain(int argc, _TCHAR* argv[]){
	
	Collection xCol("db1");
	Expression xE;

	/*xE.Add("name=Jon");
	xE.Add("age","37");
	xE.Add("stone","many");*/

	//xCol.Insert(xE);

	//xCol.SetSortFlag(SORT_ASC);
	ResultSet *xResult=Where(xCol,xE).Get("name,stone");

	ResultSet::iterator it;
	for(it=xResult->begin();it!=xResult->end();++it){
		Cell *pxC=(*it);
		std::cout<<pxC->Key()<<": "<<pxC->Val()<<std::endl;
	}
	
	std::cin.get();
	delete xResult;
}

