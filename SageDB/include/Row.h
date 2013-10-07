#ifndef ROW_H
#define ROW_H

#include <fstream>
#include <list>
#include "Cell.h"
#include "KeyPair.h"

namespace SageDB{

class Row{
public:
	Row(std::fstream & p_xStream);
	virtual ~Row(){}

	ResultSet *Next();


	void Serialize(KeyPair *p_pxKP);

protected:
private:

	std::fstream *m_pxStream;

};

}

#endif