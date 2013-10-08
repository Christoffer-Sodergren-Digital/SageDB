#include "Collection.h"
#include <algorithm>
#include <functional>
#include <iomanip>
#include <sstream>
#include "boost/filesystem.hpp"
#include "boost/algorithm/string.hpp"

namespace SageDB{

CollectionSortFlag Collection::m_eSortPattern=SORT_ASC;
Collection::Collection(){
	m_iRowLength=256;
	m_iContentOffset=0;
}

Collection::~Collection(){
	if(m_xFileStream.is_open()){
		m_xFileStream.close();
	}
}

Collection::Collection(std::string p_sName,int p_iRowLength){
	m_sCollectionName=p_sName;
	m_iRowLength=p_iRowLength;
	m_iContentOffset=0;
	if(!CollectionExists()){
		Create();
	}else{
		Open();
		ReadHeader();
	}
}

void Collection::SetSortFlag(CollectionSortFlag p_eFlag){
	m_eSortPattern=p_eFlag;
}

bool Collection::Sorter(const Cell *p_xO0,const Cell *p_xO1){
	if(m_eSortPattern==SORT_ASC){
		return p_xO0->Val()<p_xO1->Val();
	}else if(m_eSortPattern==SORT_DESC){
		return p_xO0->Val()>p_xO1->Val();
	}
	return 0;
}

bool Collection::CollectionExists(){
	std::string sLink=m_sCollectionName+".sdb";
	boost::filesystem::path xPath(sLink.c_str());
	return boost::filesystem::exists(xPath);
}

void Collection::ReadHeader(){
	Row *pxRow=new Row(m_xFileStream);
	int iBytesRead=0;
	ResultSet *pxRS=pxRow->Next(iBytesRead);
	m_iContentOffset=iBytesRead;
	delete pxRow;
	if(pxRS->size()<=0){m_xFileStream.close();}

	Cell *pxFirst=(*pxRS->begin());
	m_iRowLength=pxFirst->GetInt();
	Rewind();
}

void Collection::Rewind(){
	m_xFileStream.clear();
	m_xFileStream.seekg(m_iContentOffset,std::ios_base::beg);
	m_xFileStream.seekp(m_iContentOffset,std::ios_base::beg);
}

bool Collection::Open(){
	if(m_xFileStream.is_open()){m_xFileStream.close();}
	std::string sLink=m_sCollectionName+".sdb";
	m_xFileStream.open(sLink.c_str(),std::ios_base::in|std::ios_base::out);
	if(!m_xFileStream.is_open()){
		m_xFileStream.open(sLink.c_str(),std::ios_base::in|std::ios_base::out|std::ios_base::trunc);
	}


	return m_xFileStream.is_open();
}

bool Collection::Create(){
	
	if(!Open()){return false;}
	Expression xExp;
	std::string sKey="rowsize";
	std::string sVal=std::to_string(m_iRowLength);
	xExp.Add(sKey,sVal);
	m_iContentOffset=sKey.size()+sVal.length()+1;//equal sign

	m_xFileStream<<xExp<<std::endl;
	m_xFileStream.flush();
	m_xFileStream.clear();

	return true;
}

Collection & Where(Collection & p_xCollection, Expression & p_xExpr){
	p_xCollection.m_xSearchPattern=p_xExpr;
	return p_xCollection;
}

ResultSet *Collection::Get(const std::string & p_sIndices){
	if(!m_xFileStream.is_open()){return NULL;}
	Rewind();
	ResultSet *pxRet=new ResultSet();
	Row *pxRow=new Row(m_xFileStream);

	std::vector<std::string> xTokens;
	boost::split(xTokens,p_sIndices,boost::is_any_of(","));

	KeyPairSet xCmp=m_xSearchPattern.KeyPairs();
	ResultSet *pxSet=pxRow->Next();
	while(pxSet!=NULL){
		KeyPairSet::iterator it;
		ResultSet::iterator jt;
		int iNumMatch=0;
		for(it=xCmp.begin();it!=xCmp.end();++it){
			for(jt=pxSet->begin();jt!=pxSet->end();++jt){
				KeyPair *pxKP=(*it);
				Cell *pxC=(*jt);
				if(pxKP->Match(pxC->Key().c_str(),pxC->Val().c_str())){
					iNumMatch++;
				}
			}
		}
		if(iNumMatch==xCmp.size()){
			for(jt=pxSet->begin();jt!=pxSet->end();++jt){
				Cell *pxC=(*jt);
				if(std::find(xTokens.begin(),xTokens.end(),pxC->Key())!=xTokens.end()){
					pxRet->push_back(pxC);
				}
			}
		}
		delete pxSet;
		pxSet=pxRow->Next();
	}

	delete pxRow;
	//pxRet->sort(&Collection::Sorter);

	return pxRet;
}
bool Collection::Insert(Expression & p_xExpr){
	if(!m_xFileStream.is_open()){return false;}
	Rewind();
	m_xFileStream.seekp(0,std::ios_base::end);
	m_xFileStream.clear();
	
	
	KeyPairSet::iterator it;
	KeyPairSet xKP=p_xExpr.KeyPairs();
	std::stringstream xStream;
	for(it=xKP.begin();it!=xKP.end();++it){
		KeyPair *pxKP=(*it);
		xStream<<(*pxKP);
	}

	int iP=(int)xStream.tellp();
	if(iP>=m_iRowLength){return false;}

	m_xFileStream<<std::endl;
	m_xFileStream<<xStream.str();
	m_xFileStream.flush();

	return true;
}

bool Collection::Update(Expression & p_xExpr){
	if(!m_xFileStream.is_open()){return false;}
	Rewind();
	ResultSet *pxRet=new ResultSet();
	Row *pxRow=new Row(m_xFileStream);

	KeyPairSet xCmp=m_xSearchPattern.KeyPairs();
	int iBytesRead=0;
	ResultSet *pxSet=pxRow->Next(iBytesRead);
	while(pxSet!=NULL){
		KeyPairSet::iterator it;
		ResultSet::iterator jt;
		int iNumMatch=0;
		for(it=xCmp.begin();it!=xCmp.end();++it){
			for(jt=pxSet->begin();jt!=pxSet->end();++jt){
				KeyPair *pxKP=(*it);
				Cell *pxC=(*jt);
				if(pxKP->Match(pxC->Key().c_str(),pxC->Val().c_str())){
					iNumMatch++;
				}
			}
		}
		if(iNumMatch==xCmp.size()){
			KeyPairSet xKPS=p_xExpr.KeyPairs();
			KeyPairSet::iterator nt;
			
			for(jt=pxSet->begin();jt!=pxSet->end();++jt){
				for(nt=xKPS.begin();nt!=xKPS.end();++nt){
					if((*jt)->Key().compare((*nt)->Key())==0){
						(*jt)->SetVal((*nt)->Val());
					}
				}
			}
			m_xFileStream.clear();
			std::stringstream xStream;
			for(jt=pxSet->begin();jt!=pxSet->end();++jt){
				Cell *pxC=(*jt);
				xStream<<(*pxC);
			}
			int iP=(int)xStream.tellp();
			if(iP>=m_iRowLength){return false;}

			iP=(int)m_xFileStream.tellp();
			m_xFileStream.seekp(-(iBytesRead),std::ios_base::cur);
			std::string sTmp=xStream.str();
			if(iBytesRead>sTmp.length()){
				m_xFileStream<<sTmp<<std::setw(m_iRowLength-sTmp.length())<<std::setfill(' ')<<std::left<<std::endl;
			}else{
				m_xFileStream<<sTmp<<std::endl;//<<std::setw(m_iRowLength-sTmp.length())<<std::setfill(' ')<<std::left;
			}
		}
		delete pxSet;
		pxSet=pxRow->Next(iBytesRead);
	}
	
	delete pxRow;
	return true;
}



}