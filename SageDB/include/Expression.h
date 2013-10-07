#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <vector>
#include "KeyPair.h"
#include "Cell.h"


namespace SageDB{

	typedef std::vector<KeyPair*> KeyPairSet;

class Expression{
public:
	Expression();
	Expression(const Expression & p_xO);
	Expression & operator=(const Expression & p_xO);
	virtual  ~Expression();

	void Add(const std::string & p_sPart);
	void Add(const std::string & p_sKey,const std::string & p_sVal);
	void Remove(const std::string & p_sPart);

	bool Match(Cell *p_xCell);
	//bool Match(const std::string & p_sMatch);

	bool operator==(Cell *p_xCell);
	//bool operator==(const std::string & p_sMatch);

	KeyPairSet & KeyPairs(){return m_xKeyPairs;}

private:
	KeyPairSet m_xKeyPairs;
};

}


#endif