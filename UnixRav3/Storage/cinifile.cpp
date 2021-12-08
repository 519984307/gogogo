#include <fstream>
#include <string>
#include <string.h>
#include <map>

#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <cctype>

#include <sys/stat.h>

#include <iostream>
#include <iomanip>

#include "cinifile.h"

// Make an uppercase copy of s
inline std::string upperCase(const std::string& s)
{
	std::string upper(s);
	for(size_t i = 0; i < s.length(); ++i)
		upper[i] = toupper(upper[i]);
	return upper;
}

inline bool iscomment(std::string& line)
{
	if(line=="")
		return true;

	std::string s;
	std::size_t nStInx=0;
	for(size_t i = 0; i < line.length(); ++i)
	{
		if(line[i]=='\t' || line[i]==' ')
			continue;
		nStInx=i;
		break;
	}

    // current line is blank comment
	if(nStInx==line.length())
	{
		line="\n";
		return true;
	}

	s=line.substr(nStInx,line.length()-nStInx);
	line=s;
	if(s[0]==';' || s[0]=='#')
		return true;
	return false;
}

CINIFile::CINIFile(const std::string&strFile,bool bCaseSensitive)
{
	if(strFile=="")
	{
		throw CIniFileException("File Name empty");
	}

	m_bCaseSensitive=bCaseSensitive;
	m_elasterror=INIOK;
	m_bdirty=false;
	if(!openfile(strFile))
	{
		if(INIFILENOTEXIST!=m_elasterror)
		{
			printf(" we throw bad\n");
			switch(m_elasterror)
			{
				case ININOTREGULARFILE:
					throw CIniFileException("Not a regular file");
					break;
				case INIPERMISSIONDENIED:
					throw CIniFileException("Permission denied!");
					break;
				case INIOTHERERROR:
					throw CIniFileException(" File open error!");
					break;
				default:
					break;
			}
		}
	}
}

CINIFile::~CINIFile()
{
	if(m_bdirty)
		close();
}

bool CINIFile::close()
{
	if(!m_bdirty)
		return true;

	std::ofstream fileout(m_strIniFile.c_str());
	if(!fileout)
		return false;

	std::string line;
	
	KEYVALMAP::iterator keypos,keytmp;
	SECTIONMAP::iterator sectionpos;

	for(sectionpos=m_strSection.begin();sectionpos!=m_strSection.end();++sectionpos)
	{
		// 处section注释
		std::string strCommName="##"+sectionpos->first+"##";
	 	keytmp=m_strComment.find(strCommName);
		if(keytmp!=m_strComment.end())
		{
			fileout<<keytmp->second;
		}

		fileout<<"["<<sectionpos->first<<"]"<<std::endl;
		for(keypos=sectionpos->second.begin();keypos!=sectionpos->second.end();++keypos)
		{
			// 处key注释
			std::string strCommName="##"+sectionpos->first+"##"+keypos->first+"##";
	 		keytmp=m_strComment.find(strCommName);
			if(keytmp!=m_strComment.end())
			{
				fileout<<keytmp->second;
			}
			fileout<<keypos->first<<"="<<keypos->second<<std::endl;
		}
	}
	 
    // 处理file tail comment
	std::string strCommName="##FILETAIL##";
	keytmp=m_strComment.find(strCommName);
	if(keytmp!=m_strComment.end())
	{
		fileout<<keytmp->second;
	}
	return true;
}

bool CINIFile::openfile(const std::string &strFilename)
{

	m_strIniFile=strFilename;
	
	struct stat stBuf;
	if(stat(m_strIniFile.c_str(),&stBuf))
	{
		switch(errno)
		{
			case ENOENT:
				m_elasterror=INIFILENOTEXIST;
				break;
			case EACCES:
				m_elasterror=INIPERMISSIONDENIED;
				break;
			default:
				m_elasterror=INIOTHERERROR;
				break;
		}
		return false;
	}

	if( !(stBuf.st_mode&S_IFREG )) 
	{
		m_elasterror=ININOTREGULARFILE;
		return false;
	}

	std::ifstream filein(m_strIniFile.c_str());
	if(!filein)
	{
		m_elasterror=INIOTHERERROR;
		return false;
	}

    // 打开文件的时候分析文件内容，建立map表
	std::string::size_type begIdx,endIdx;
        
	std::string line;
	
	KEYVALMAP keyvalmap;
	
	std::string strSectionName;
	bool bHasSection=false;
	bool bWrite=true;

	std::string strcommline;
	
	while(getline(filein, line))
	{
		//处理注释语句
		if(::iscomment(line))
		{
			
			strcommline+=line;
			strcommline+="\n";
			continue;
		}
		
		begIdx=line.find_first_of("[");
		if(begIdx!=std::string::npos)
		{
			endIdx=line.find_first_of("]",begIdx);
			if(endIdx==std::string::npos)
				continue;
			
			
			if(bHasSection && keyvalmap.size())
			{
				m_strSection[strSectionName]=keyvalmap;
			}

			if(m_bCaseSensitive)
				strSectionName=line.substr(begIdx+1,endIdx-begIdx-1);
			else
				strSectionName=upperCase(line.substr(begIdx+1,endIdx-begIdx-1));
			if(strcommline!="")
			{
				std::string strCommName="##"+strSectionName+"##";
				m_strComment[strCommName]=strcommline;
				strcommline="";
			}

			keyvalmap.clear();
			bHasSection=true;
			bWrite=false;
			continue;
		}
		
		
		if(!bHasSection)
			continue;

		begIdx=line.find_first_of("=");
		if(begIdx==std::string::npos)
			continue;

		std::string strKey;
		if(m_bCaseSensitive)
			strKey=line.substr(0,begIdx);
		else
			strKey=upperCase(line.substr(0,begIdx));
		endIdx=line.length();
		std::string strVal=line.substr(begIdx+1,endIdx);
		
		keyvalmap[strKey]=strVal;
		if(strcommline!="")
		{
			std::string strCommName="##"+strSectionName+"##"+strKey+"##";
			m_strComment[strCommName]=strcommline;
			strcommline="";
		}
		
	}

	if(!bWrite &&keyvalmap.size() )
	{
		m_strSection[strSectionName]=keyvalmap;
	}
	if(strcommline!="")
	{
		std::string strCommName="##FILETAIL##";
		m_strComment[strCommName]=strcommline;
		strcommline="";
	}
	
//	KEYVALMAP::iterator keypos;
//	SECTIONMAP::iterator sectionpos;

	return true;	
}

bool CINIFile::readitemlong(const std::string &strSection,const std::string &strKey,long &nVal,long lDefaultValue)
{
	long lVal;
	if(readlong(strSection,strKey,lVal))
	{
		nVal=int(lVal);
		return true;
	}
	nVal=lDefaultValue;
	return false;
}

bool CINIFile::readitemlong(const std::string &strSection,const std::string &strKey,int &nVal,int nDefaultValue)
{
	long lVal;
	if(readlong(strSection,strKey,lVal))
	{
		nVal=int(lVal);
		return true;
	}
	nVal=nDefaultValue;
	return false;
}

bool CINIFile::readitemlong(const std::string &strSection,const std::string &strKey,
	bool &bVal,bool bDefault)
{
	long lVal;
	if(readlong(strSection,strKey,lVal))
	{
		if(lVal==0)
			bVal=false;
		else
			bVal=true;
		return true;
	}
	bVal=bDefault;
	return false;
}

bool CINIFile::readlong(const std::string &strSection,const std::string &strKey,long &lVal)
{
	std::string strVal;
	if(readstr(strSection,strKey,strVal))
	{
		lVal=atol(strVal.c_str());
		return true;
	}
	
	return false;
	
}

bool CINIFile::readitemfloat(const std::string &strSection,const std::string &strKey,float &flVal,float flDefaultValue)
{
	std::string strVal;
	if(readstr(strSection,strKey,strVal))
	{
		flVal=atof(strVal.c_str());
		return true;
	}
    flVal = flDefaultValue;
	return false;
}

bool CINIFile::readitemstr(const std::string &strSection,const std::string &strKey,
	std::string &strVal,const std::string &strDefault)
{
	if(readstr(strSection,strKey,strVal))
		return true;
	strVal=strDefault;
	return false;
}

bool CINIFile::readstr(const std::string &strSection,const std::string &strKey,
	std::string &strVal)
{
	std::string strUpSec,strUpKey;
	if(m_bCaseSensitive)
	{
		strUpSec=strSection;
		strUpKey=strKey;
	}
	else
	{
		strUpSec=upperCase(strSection);
		strUpKey=upperCase(strKey);
	}


	KEYVALMAP::iterator keypos;
	SECTIONMAP::iterator sectionpos;

	 sectionpos=m_strSection.find(strUpSec);
	 if(sectionpos==m_strSection.end())
	 	return false;
	 
	 keypos=sectionpos->second.find(strUpKey);
	 
	 if(keypos==sectionpos->second.end())
	 	return false;

	strVal= keypos->second;

    // trim all
    const std::string &space =" \f\n\t\r\v" ;
    strVal = strVal.erase(0,strVal.find_first_not_of(space));
    strVal = strVal.erase(strVal.find_last_not_of(space)+1);

	if(strVal=="")
		return false;
	return true;
#if 0
	char szBuf[1024];
	strcpy(szBuf,strVal.c_str());
	int nLen=strlen(szBuf);
	int nSaveLen=nLen;

	while (nLen>0)
	{
		
		if(szBuf[nLen-1]=='\n' || szBuf[nLen-1]=='\r')
		{
			szBuf[nLen-1]='\0';
			nLen--;
		}
		else
			break;
	}

	if(nLen<nSaveLen)
		strVal=szBuf;
#endif
	return true;
}


bool CINIFile::writeitemlong(const std::string &strSection,const std::string &strKey,long lVal)
{
	char szBuf[256];
	sprintf(szBuf,"%ld",lVal);
	writeitemstr(strSection,strKey,szBuf);
	return true;
}

bool CINIFile::writeitemfloat(const std::string &strSection,const std::string &strKey,float flVal)
{
	char szBuf[256];
	sprintf(szBuf,"%f",flVal);
	writeitemstr(strSection,strKey,szBuf);
	return true;
}

bool CINIFile::writeitemstr(const std::string &strSection,const std::string &strKey,const std::string &strVal)
{

	m_bdirty=true;

	std::string strUpSec,strUpKey;

	if(m_bCaseSensitive)
	{
		strUpSec=strSection;
		strUpKey=strKey;
	}
	else
	{
		strUpSec=upperCase(strSection);
		strUpKey=upperCase(strKey);
	}


	KEYVALMAP::iterator keypos;
	SECTIONMAP::iterator sectionpos;

	 sectionpos=m_strSection.find(strUpSec);
	 if(sectionpos==m_strSection.end())
	 {
	 	KEYVALMAP valmap;
	 	valmap[strUpKey]=strVal;
	 	m_strSection[strUpSec]=valmap;
	 	return true;
	 }
	 
	 keypos=sectionpos->second.find(strUpKey);
	 
	 if(keypos==sectionpos->second.end())
	 {
		sectionpos->second[strUpKey]=strVal;
	 	return false;
	 }

	std::string strTmpVal=strVal;

	keypos->second=strTmpVal;
	return true;
}



#ifdef MAIN_TEST
int main(int argc,char *argv[])
{
	CINIFile *pIni;
	try
	{
		pIni=new CINIFile (argv[1]);
	}
	catch(CINIFile::CIniFileException &cExpect)
        {
                std::cout<<cExpect.what()<<std::endl;
		return -1;
        }

	std::string strVal;
	long lVa;

	if(pIni->readitemstr("xzl","file",strVal))
				std::cout<<"val-="<<strVal<<std::endl;
	else
		std::cout <<" not found"<<std::endl;

#if 0
//	pIni->writeitemstr("xzl","file","zdgfh/sdjjf");
	if(pIni->readitemlong("xzl","AA",lVa))
				std::cout<<"val-="<<lVa<<std::endl;
	else
		std::cout <<" not found"<<std::endl;
	pIni->writeitemlong("xzl","AA",10);

	float flVal;
	if(pIni->readitemfloat("xzl","bB",flVal))
				std::cout<<"fval-="<<flVal<<std::endl;
	else
		std::cout <<" not found"<<std::endl;

	pIni->writeitemfloat("Axzl","bB",3.141567);

#endif
	pIni->close();
	delete pIni;
}
#endif

