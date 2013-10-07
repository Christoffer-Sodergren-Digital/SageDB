#include "Collection.h"
#include <algorithm>
#include <functional>
#include "boost/filesystem.hpp"
#include "boost/algorithm/string.hpp"

namespace SageDB{

CollectionSortFlag Collection::m_eSortPattern=SORT_ASC;
Collection::Collection(){

}

Collection::~Collection(){
	if(m_xFileStream.is_open()){
		m_xFileStream.close();
	}
}

Collection::Collection(std::string p_sName){
	m_sCollectionName=p_sName;
	if(!CollectionExists()){
		Create();
	}else{
		Open();
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

bool Collection::Open(){
	if(m_xFileStream.is_open()){m_xFileStream.close();}
	std::string sLink=m_sCollectionName+".sdb";
	m_xFileStream.open(sLink.c_str(),std::ios_base::in|std::ios_base::out);

	return m_xFileStream.is_open();
}

bool Collection::Create(){
	if(!Open()){return false;}
	m_xFileStream<<COLLECTION_HEADER;
	
	return true;
}

Collection & Where(Collection & p_xCollection, Expression & p_xExpr){
	p_xCollection.m_xSearchPattern=p_xExpr;
	return p_xCollection;
}

ResultSet *Collection::Get(const std::string & p_sIndices){
	if(!m_xFileStream.is_open()){return NULL;}
	m_xFileStream.seekg(0,std::ios_base::beg);
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
	m_xFileStream.seekp(0,std::ios_base::end);
	m_xFileStream<<std::endl;
	Row *pxRow=new Row(m_xFileStream);
	KeyPairSet::iterator it;
	KeyPairSet xKP=p_xExpr.KeyPairs();
	for(it=xKP.begin();it!=xKP.end();++it){
		KeyPair *pxKP=(*it);
		pxRow->Serialize(pxKP);
	}
	delete pxRow;
	return true;
}

bool Collection::Update(Expression & p_xExpr){
	if(!m_xFileStream.is_open()){return NULL;}
	m_xFileStream.seekg(0,std::ios_base::beg);
	ResultSet *pxRet=new ResultSet();
	Row *pxRow=new Row(m_xFileStream);

//	std::vector<std::string> xTokens;
	//boost::split(xTokens,p_sIndices,boost::is_any_of(","));

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
			KeyPairSet xKPS=p_xExpr.KeyPairs();
			for(it=xKPS.begin();it!=xKPS.end();++it){

			}
		}
		delete pxSet;
		pxSet=pxRow->Next();
	}
	
	delete pxRow;
}



}