#ifndef KEYPAIR_H
#define KEYPAIR_H

#include <cstring>
#include <cassert>
#include <fstream>
#include <string>

namespace SageDB{

class KeyPair{
public:
	KeyPair(const char *p_sExpr){
		m_bValid=false;
		m_sKey=NULL;
		m_sVal=NULL;
		if(p_sExpr==NULL){return;}
		char *pch=strtok((char*)p_sExpr,"=");
		if(pch==NULL){return;}
		int iL=strlen(pch);
		m_sKey=new char[iL+1];
		strcpy(m_sKey,pch);
		m_sKey[iL]=0;

		pch=strtok(NULL,"=");
		if(pch==NULL){return;}
		iL=strlen(pch);
		m_sVal=new char[iL+1];
		strcpy(m_sVal,pch);
		m_sVal[iL]=0;

		m_bValid=true;
	}
	KeyPair(const char *p_sKey,const char *p_sVal){
		m_bValid=false;
		m_sKey=NULL;
		m_sVal=NULL;
		if(p_sKey==NULL||p_sVal==NULL){return;}
		int iL=strlen(p_sKey);
		if(iL<=0){return;}
		m_sKey=new char[iL+1];
		strcpy(m_sKey,p_sKey);
		m_sKey[iL]=0;

		iL=strlen(p_sVal);
		if(iL<=0){return;}
		m_sVal=new char[iL+1];
		strcpy(m_sVal,p_sVal);
		m_sVal[iL]=0;

		m_bValid=true;
	}

	virtual ~KeyPair(){
		delete [] m_sKey;
		delete [] m_sVal;
	}

	KeyPair(const KeyPair & p_xO){
		if(p_xO.m_sKey==NULL||p_xO.m_sVal==NULL){return;}
		int iL=strlen(p_xO.m_sKey);
		m_sKey=new char[iL+1];
		strcpy(m_sKey,p_xO.m_sKey);
		m_sKey[iL]=0;

		iL=strlen(p_xO.m_sVal);
		m_sVal=new char[iL+1];
		strcpy(m_sVal,p_xO.m_sVal);
		m_sVal[iL]=0;

		m_bValid=true;
	}
	KeyPair & operator=(const KeyPair & p_xO){
		assert(p_xO.m_sKey==NULL||p_xO.m_sVal==NULL);
		int iL=strlen(p_xO.m_sKey);
		m_sKey=new char[iL+1];
		strcpy(m_sKey,p_xO.m_sKey);
		m_sKey[iL]=0;

		iL=strlen(p_xO.m_sVal);
		m_sVal=new char[iL+1];
		strcpy(m_sVal,p_xO.m_sVal);
		m_sVal[iL]=0;

		m_bValid=true;
		return *this;
	}

	friend std::ostream & operator<<(std::ostream & p_xO,KeyPair & p_xKP){
		p_xO<<p_xKP.Key()<<"="<<p_xKP.Val()<<" ";
		return p_xO;
	}

	char *Key(){return m_sKey;}
	char *Val(){return m_sVal;}

	bool Match(const char *p_sP0, const char *p_sP1){
		if(p_sP0==NULL||p_sP1==NULL){return false;}
		int iM=strcmp(p_sP0,m_sKey);
		if(iM!=0){return false;}
		iM=strcmp(p_sP1,m_sVal);
		return (iM==0);
	}
	
	bool Valid()const{return m_bValid;}

private:
	KeyPair(){}
	char *m_sKey;
	char *m_sVal;
	bool m_bValid;
};

}


#endif