#include "Row.h"
#include <vector>
#include "boost/algorithm/string.hpp"



namespace SageDB{

Row::Row(std::fstream & p_xStream){
	m_pxStream=&p_xStream;
}

std::list<Cell*> *Row::Next(){
	ResultSet *pxRet=new std::list<Cell*>();
	if(!m_pxStream->is_open()||m_pxStream->eof()){return NULL;}
	std::string sBuffer;
	std::getline(*m_pxStream,sBuffer);
	boost::trim(sBuffer);

	if(sBuffer.length()>0&&sBuffer[0]=='*'){return pxRet;}

	std::vector<std::string> xTokens;
	boost::split(xTokens,sBuffer,boost::is_any_of(" "));
	std::vector<std::string>::iterator it;
	for(it=xTokens.begin();it!=xTokens.end();++it){
		std::vector<std::string> xTmp;
		boost::split(xTmp,(*it),boost::is_any_of("="));

		if(xTmp.size()<2){continue;}
		Cell *pxNC=new Cell(xTmp[0],xTmp[1]);
		pxRet->push_back(pxNC);
	}
	return pxRet;
}

void Row::Serialize(KeyPair *p_pxKP){
	if(!m_pxStream->is_open()){return;}
	//m_pxStream->seekp(0,std::ios_base::end);
	(*m_pxStream)<<(*p_pxKP);
	m_pxStream->flush();
}

}