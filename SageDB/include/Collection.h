#ifndef COLLECTION_H
#define COLLECTION_H

#include <map>
#include <vector>
#include <fstream>
#include <string>
#include "Row.h"
#include "Expression.h"

namespace SageDB{

#define COLLECTION_HEADER "*SAGEDB*"

enum CollectionSortFlag{
	SORT_ASC=0,
	SORT_DESC
};

class Collection{
public:
	
	Collection(std::string p_sName);
	~Collection();

	static void SetSortFlag(CollectionSortFlag p_eSortFlag);
	static bool Sorter(const Cell *p_pxO0,const Cell *p_pxO1);

	friend Collection & Where(Collection & p_xCollection,Expression & p_xExpr);

	ResultSet *Get(const std::string & p_sIndices);
	bool Insert(Expression & p_xExpr);
	bool Update(Expression & p_xExpr);

protected:

private:
	Collection();
	bool CollectionExists();
	bool Create();
	bool Open();

	Expression m_xSearchPattern;
	static CollectionSortFlag m_eSortPattern;
	std::string m_sCollectionName;

	std::fstream m_xFileStream;
};

}
#endif




