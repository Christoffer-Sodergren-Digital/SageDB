#include "Expression.h"


namespace SageDB{

	Expression::Expression(){
	}

	Expression::~Expression(){
		std::vector<KeyPair*>::iterator it;
		for(it=m_xKeyPairs.begin();it!=m_xKeyPairs.end();++it){
			KeyPair *pxKP=(*it);
			delete pxKP;
			pxKP=NULL;
		}
		m_xKeyPairs.clear();
	}

	Expression::Expression(const Expression & p_xO){
		KeyPairSet::const_iterator it;
		for(it=p_xO.m_xKeyPairs.begin();it!=p_xO.m_xKeyPairs.end();++it){
			KeyPair *pxNKP=new KeyPair(*(*it));
			m_xKeyPairs.push_back(pxNKP);
		}
	}
	Expression & Expression::operator=(const Expression & p_xO){
		KeyPairSet::const_iterator it;
		for(it=p_xO.m_xKeyPairs.begin();it!=p_xO.m_xKeyPairs.end();++it){
			KeyPair *pxNKP=new KeyPair(*(*it));
			m_xKeyPairs.push_back(pxNKP);
		}
		return *this;
	}
	void Expression::Add(const std::string & p_sPart){
		KeyPair *pxNKP=new KeyPair(p_sPart.c_str());
		m_xKeyPairs.push_back(pxNKP);
	}
	void Expression::Add(const std::string & p_sKey, const std::string & p_sVal){
		KeyPair *pxNKP=new KeyPair(p_sKey.c_str(),p_sVal.c_str());
		m_xKeyPairs.push_back(pxNKP);
	}

	void Expression::Remove(const std::string & p_sPart){
		std::string sP0;
		std::string sP1;
		size_t iP=p_sPart.find("=");
		if(iP==std::string::npos){return;}
		sP0=p_sPart.substr(0,iP);
		sP1=p_sPart.substr(iP+1);
		std::vector<KeyPair*>::iterator it;
		for(it=m_xKeyPairs.begin();it!=m_xKeyPairs.end();++it){
			if((*it)->Match(sP0.c_str(),sP1.c_str())){
				delete (*it);
				it=m_xKeyPairs.erase(it);
				if(it==m_xKeyPairs.end()){break;}
			}
		}
	}


	bool Expression::Match(Cell *p_pxCell){
		if(p_pxCell==NULL){return false;}
		const char *sK=p_pxCell->Key().c_str();
		const char *sV=p_pxCell->Val().c_str();

		std::vector<KeyPair*>::iterator it;
		for(it=m_xKeyPairs.begin();it!=m_xKeyPairs.end();++it){
			if((*it)->Match(sK,sV)){
				return true;
			}
		}
	}
	bool Expression::operator==(Cell *p_pxCell){
		return Match(p_pxCell);
	}

	/*bool Expression::Match(const std::string & p_sMatch){

	}*/

}