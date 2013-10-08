// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include "Collection.h"
#include "Expression.h"

using namespace SageDB;

int _tmain(int argc, _TCHAR* argv[]){
	
	Collection xCol("db3");
	Expression xE;

	xE.Add("name","kalle");
	xE.Add("points","125");

	xCol.Insert(xE);

	Expression xUpdate;
	xUpdate.Add("points","1");

	////xCol.SetSortFlag(SORT_ASC);
	bool bResult=Where(xCol,xE).Update(xUpdate);
	ResultSet *xResult=Where(xCol,xE).Get("name");

	if(xResult!=NULL){
		ResultSet::iterator it;
		for(it=xResult->begin();it!=xResult->end();++it){
			Cell *pxC=(*it);
			std::cout<<pxC->Key()<<": "<<pxC->Val()<<std::endl;
		}
	}
	
	std::cin.get();
	delete xResult;
	
}

