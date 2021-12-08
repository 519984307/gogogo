#ifndef _CRAVUNIX_INIFILE_H_
#define _CRAVUNIX_INIFILE_H_

#include <fstream>
#include <string>
#include <map>

	
typedef  std::map<std::string,std::string> KEYVALMAP;
typedef  std::map<std::string,KEYVALMAP >SECTIONMAP;
 
class CINIFile
{
public:
	enum INIERROR
	{
		INIOK=0x0,
		INIFILENOTEXIST,
		ININOTREGULARFILE,
		INIPERMISSIONDENIED,
		INIOTHERERROR
	};
	class CIniFileException
	{
		const char* msg;
		public:
		CIniFileException(const char* msg) : msg(msg) {}
		const char* what() const { return msg; }
	};

	CINIFile(const std::string&strFile,bool bCaseSentive);
	~CINIFile();
	
	bool readitemlong(const std::string &strSection,const std::string &strKey,long &lVal,long lDefautVal=0); 
	bool readitemlong(const std::string &strSection,const std::string &strKey,int &lVal,int lDefautVal=0);
	bool readitemlong(const std::string &strSection,const std::string &strKey,bool &lVal,bool bDefautVal=false);

	bool readitemfloat(const std::string &strSection,const std::string &strKey,float &flVal,float flDefaultValue=0.0);
	bool readitemstr(const std::string &strSection,const std::string &strKey,std::string &strVal,const std::string &strDefaultValue="");

	bool writeitemlong(const std::string &strSection,const std::string &strKey,long lVal);
	bool writeitemfloat(const std::string &strSection,const std::string &strKey,float flVal);
	bool writeitemstr(const std::string &strSection,const std::string &strKey,const std::string &strVal);

	bool close();
	bool openfile(const std::string &strFilename);
private:
	bool readstr(const std::string &strSection,const std::string &strKey,std::string &strVal);
	bool readlong(const std::string &strSection,const std::string &strKey,long &lVal); 

private:
	SECTIONMAP m_strSection;
	KEYVALMAP m_strComment;
	std::string m_strIniFile;
	INIERROR m_elasterror;

    bool m_bCaseSensitive;
	bool m_bdirty;
};
#endif
