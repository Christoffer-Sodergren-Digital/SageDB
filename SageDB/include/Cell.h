#ifndef CELL_H
#define CELL_H

#include <fstream>
#include <string>
#include <list>

namespace SageDB{

class Cell{
public:
	Cell(){}
	Cell(std::string p_sKey, std::string p_xVal) : m_sKey(p_sKey), m_xVal(p_xVal){}
	virtual ~Cell(){}

	const std::string & Key(){return m_sKey;}
	const std::string & Val()const{return m_xVal;}
	void SetVal(const std::string & p_xVal){m_xVal=p_xVal;}

	friend std::ostream & operator<<(std::ostream & p_xStream, Cell & p_xC){
		p_xStream<<p_xC.m_sKey<<"="<<p_xC.m_xVal<<" ";
		return p_xStream;
	}

	int GetInt(){return atoi(m_xVal.c_str());}
	float GetFloat(){return (float)atof(m_xVal.c_str());}
	bool GetBoolean(){return (m_xVal=="true"||m_xVal=="1");}

private:
	std::string m_sKey;
	std::string m_xVal;
};

typedef std::list<Cell*> ResultSet;

}

#endif