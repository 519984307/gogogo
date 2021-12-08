#include <stdio.h>
#include <stdlib.h>
#include <QStringList>
#include <QFile>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "iptables.h"
#include "../../../BaseController/appconfig.h"

#define DATABASENAME "/opt/BDFZ/RAV/RJJHGLTX/mytest.db"
#define SYSTEMBF "/home/dong/rising/10.19/UnixRav3/RavQt/Config/common/system "

using namespace std;

//init sql(open db and create table)
IpSqlData::IpSqlData(std::string dbtablename, QWidget *parent)
	:QWidget(parent)
{
	databasename = DATABASENAME;

#if 1

	try{
		if(sqlite3_open(databasename.c_str(), &db) != SQLITE_OK){
			throw sqlite3_errmsg(db);
		}
		sqlflag = true;
	}catch(const char* temp){
		if(db)
			sqlite3_close(db);
		sqlflag = false;

		return;
	}

	dbtablename = "create table if not exists " + dbtablename + ";";

	if(!sqlflag)
	{
		if(sqlite3_exec(db, dbtablename.c_str(), NULL, NULL, &errmsg) != SQLITE_OK){
			qDebug() << sqlite3_errmsg(db);
			return;
		}else{
			qDebug() << "Create or open table success.";
		}

	}

#else

	if(sqlite3_open(databasename.c_str(), &db) != SQLITE_OK){
		qDebug() << sqlite3_errmsg(db);
		return;
	}else{
		qDebug() << "open DATABASE success.";
	}

	dbtablename = "create table if not exists " + dbtablename + ";";

	if(sqlite3_exec(db, dbtablename.c_str(), NULL, NULL, &errmsg) != SQLITE_OK){
		qDebug() << sqlite3_errmsg(db);
		return;
	}else{
		qDebug() << "Create or open table success.";
	}
#endif
}

//exec sql cmd
void IpSqlData::SqlCmd(std::string insetcmd)
{
	if(!sqlflag)
		return;

	if(sqlite3_exec(db, insetcmd.c_str(), NULL, NULL, &errmsg) != SQLITE_OK){
		qDebug() << sqlite3_errmsg(db);
		return;
	}else{
		qDebug() << "success.";
	}
}

//get sql db data
void IpSqlData::SqlQueryIp(std::string dbtablename, int flag)
{
	int row=0;
	int column=0;
	dbtablename = "select * from " + dbtablename;

	if(!sqlflag)
		return;

	if(sqlite3_get_table(db, dbtablename.c_str(), &resultp, &row, &column, &errmsg) != SQLITE_OK){
		qDebug() << sqlite3_errmsg(db);
		return;
	}else{
		qDebug() << "Query done.";
	}

	int i=0;
	int j=0;
	int index = column;
	port_Text text;


	if(flag == 1){
		for(i = 0; i < row; i++){
			for(j = 0; j < column; j++){
				m_IpWhiteDB.push_back(QString(QLatin1String(resultp[index++])));
			}
		}
	}else if(flag == 2){
		for(i = 0; i < row; i++){
			for(j = 0; j < column; j++){
				m_IpBlackDB.push_back(QString(QLatin1String(resultp[index++])));
			}
		}

	}else if(flag == 3){
		for(i = 0; i < row; i++){
			for(j = 0; j < column; j++){
				if(j == 0)
					text.str = QString(QLatin1String(resultp[index++]));
				if(j == 1)
					text.protocol = QString(QLatin1String(resultp[index++]));
				if(j == 2)
					text.direction = QString(QLatin1String(resultp[index++]));
			}
			m_PortDB.push_back(text);
		}
	}

}

void IpSqlData::CloseSqldb()
{
	if(!sqlflag)
		return;
	sqlite3_close(db);
}



IpTables::IpTables(std::string dbtablename, QWidget *parent)
	:QWidget(parent)
{
	ipsqldata = new IpSqlData(dbtablename);
	std::string appconfig = Utils::Path::JoinPath("/etc", RAV_CONFIG_FILE, NULL);
	if(!Utils::Path::FileAvailed(appconfig)){
		appconfig = Utils::Path::JoinPath(RAV_DEFAULT_PATH, FOLDER_RJJH_ETC, RAV_CONFIG_FILE, NULL);
	}

	if(!Utils::Path::FileAvailed(appconfig)){
		std::string InstallPath = "/opt/BDFZ/RAV";
	}else if(Utils::Path::FileAvailed(appconfig)){
		mAppConfig = AppConfig::NEW(appconfig);
	}
}

void IpTables::GLTXcmd(std::string str)
{
	CommandBundle command;
	int ret = 0;
	//command.mCommandID = RSSUDO_EXECUTE;
	command.mCommandID = 200;
	command.mStringValue = str;

	RStreamPack<CommandBundle> pack(RStreamPack<CommandBundle>::STRING, command);
	pack.Externalize(Utils::Path::JoinPath(mAppConfig->GetInstallPath().c_str(), FOLDER_RJJH_GLTX, GLTX_RJJH_SOCKET, NULL).c_str(), ret);
}

//(new chain or clear customizing chain rule) and clear sql db data
void IpTables::InitAll()
{
	std::string str = "iptables -N IN_RISING";
	GLTXcmd(str);
	str = "iptables -N OUT_RISING";
	GLTXcmd(str);
	str = "iptables -F IN_RISING";
	GLTXcmd(str);
	str = "iptables -F OUT_RISING";
	GLTXcmd(str);
	str = "iptables -D INPUT -j IN_RISING";
	GLTXcmd(str);
	str = "iptables -D OUTPUT -j OUT_RISING";
	GLTXcmd(str);
	//system(str.c_str());
	ipsqldata->SqlCmd("delete from ipwhite");
	ipsqldata->SqlCmd("delete from ipblack");
	ipsqldata->SqlCmd("delete from portblack");
}

//clear all iptables rules but not delete sql data
void IpTables::DisableAll()
{

	std::string str = "iptables -F IN_RISING";
	GLTXcmd(str);
	str = "iptables -F OUT_RISING";
	GLTXcmd(str);
	str = "iptables -D INPUT -j IN_RISING";
	GLTXcmd(str);
	str = "iptables -D OUTPUT -j OUT_RISING";
	GLTXcmd(str);
	str = "iptables -X IN_RISING";
	GLTXcmd(str);
	str = "iptables -X OUT_RISING";
	GLTXcmd(str);
}

//enable customizing chain rule
void IpTables::EnableAll()
{
	std::string str = "iptables -I INPUT -j IN_RISING";
	GLTXcmd(str);
	str = "iptables -I OUTPUT -j OUT_RISING";
	GLTXcmd(str);
}

//exec iptables cmd
void IpTables::Enablecmd(QString str)
{
	if(str == "ls")
		return;

	GLTXcmd(str.toStdString());
	
}

//if str is numstr return false or return true;
bool IpTables::IsNumStr(QString str)
{
	bool a = false;

	QByteArray text = str.toLocal8Bit();
	char *s = new char[text.size()+1];
	s[text.size()] = '\0';
	char *p = s;
	strcpy(s, text.data());

	while(*p && *p>='0' && *p<='9')
		p++;

	a = (*p == '\0')? false:true;

	delete [] s;

	return a;
}

QString IpTables::ChangePortStyle(QString str)
{
	QStringList list1 = str.split('-');
	if(list1.size() == 1){
		return str;
	}
	else{
		str = list1[0] + ":" + list1[1];
	}

	return str;
}

//if port is valid return true or return false
bool IpTables::IsPortValid(QString str)
{

	QStringList list1 = str.split('-');
	int num = list1.size();
	//qDebug() <<"isportvalid" << list1;

	if(list1.size() > 2)
		return false;

	for(int i=0; i<list1.size() ; i++)
	{
		if(list1[i] == "")
		{
			num = 1;
			continue;
		}

		if(IsNumStr(list1[i]) || !(list1[i].length() <=5) || !(list1[i].toInt() >= 0 && list1[i].toInt() <= 65535))
			return false;

	}

	//if(list1.size() == 2)
	if(num == 2)
		return list1[0].toInt() < list1[1].toInt();

	return true;
}

//if ip is valid return true or return false
bool IpTables::IsIpValid(QString str)
{
	Ipflag = 0;
	QStringList list1 = str.split('-', QString::SkipEmptyParts);
	QString str2 = "";

	if(list1.size() != str.split('-').size())
		return false;

	if(list1.size() == 1 )
	{
		Ipflag =4;
	}
	else if(list1.size() == 2)
	{
		Ipflag =8;
	}
	else{
		return false;
	}
		
	for(int i = 0; i < list1.size(); i++)
	{
		str2 += list1[i];
		str2 += ".";
	}

	QStringList list2 = str2.split('.', QString::SkipEmptyParts);
	if(list2.size() != Ipflag || list2.size() != str2.split('.').size()-1)
		return false;
	for(int i = 0; i < Ipflag; i++)
	{
		if(IsNumStr(list2[i]) || !(list2[i].length() <= 3) || !(list2[i].toInt() >= 0 && list2[i].toInt() <= 255))
			return false;

	}

	return true;
}

//Concatenated cmd string and set ipwhitelist
void IpTables::setIpWhite()
{
	if(!m_IpWhiteData.size())
		return;

	QString QScmdin = "ls";
	QString QScmdout= "ls";
	for (int i = 0; i < m_IpWhiteData.size(); i++) {
		if(IsIpValid(m_IpWhiteData[i]));
		{
			QString ipwhitecmd = "insert into ipwhite values(\"";
			ipwhitecmd = ipwhitecmd + m_IpWhiteData[i] +"\")";
			ipsqldata->SqlCmd(ipwhitecmd.toStdString());

			if(Ipflag == 4)
			{
				QScmdin = "iptables -A IN_RISING -s " + m_IpWhiteData[i] + " -j ACCEPT";
				QScmdout = "iptables -A OUT_RISING -d " + m_IpWhiteData[i] + " -j ACCEPT";
			}
			else if(Ipflag == 8)
			{
				QScmdin = "iptables -A IN_RISING -m iprange --src-range " + m_IpWhiteData[i] + " -j ACCEPT";
				QScmdout = "iptables -A OUT_RISING -m iprange --dst-range " + m_IpWhiteData[i] + " -j ACCEPT";
			}
			else 
				return;
			Enablecmd(QScmdin);
			Enablecmd(QScmdout);
		}
	}

}

//Concatenated cmd string and set ipblacklist
void IpTables::setIpBlack()
{
	if(!m_IpBlackData.size())
		return;

	QString QScmdin = "ls";
	QString QScmdout= "ls";
	for (int i = 0; i < m_IpBlackData.size(); i++) {
		if(IsIpValid(m_IpBlackData[i]));
		{
			QString ipblackcmd = "insert into ipblack values(\"";
			ipblackcmd = ipblackcmd + m_IpBlackData[i] +"\")";
			ipsqldata->SqlCmd(ipblackcmd.toStdString());
			if(Ipflag == 4)
			{
				QScmdin = "iptables -A IN_RISING -s " + m_IpBlackData[i] + " -j DROP";
				QScmdout = "iptables -A OUT_RISING -d " + m_IpBlackData[i] + " -j DROP";
			}
			else if(Ipflag == 8)
			{
				QScmdin = "iptables -A IN_RISING -m iprange --src-range " + m_IpBlackData[i] + " -j DROP";
				QScmdout = "iptables -A OUT_RISING -m iprange --dst-range " + m_IpBlackData[i] + " -j DROP";
			}
			else 
				return;

			Enablecmd(QScmdin);
			Enablecmd(QScmdout);
		}
	}

}

//Concatenated cmd string and set portblacklist
void IpTables::setPort()
{
	Portflag = 0;
	if(!m_PortData.size())
		return;

	QString QScmdtcpin = "ls";
	QString QScmdtcpout= "ls";
	QString QScmdudpin = "ls";
	QString QScmdudpout= "ls";
	QString QScmdin = "ls";
	QString QScmdout= "ls";

	for (int i = 0; i < m_PortData.size(); i++) {
		QString portblackcmd = "insert into portblack values(\"";
		portblackcmd = portblackcmd + m_PortData[i].str + "\", \"" + m_PortData[i].protocol +"\", \"" + m_PortData[i].direction +"\")";
		ipsqldata->SqlCmd(portblackcmd.toStdString());
		QStringList plist = m_PortData[i].protocol.split('_', QString::SkipEmptyParts);
		QStringList dlist = m_PortData[i].direction.split('_', QString::SkipEmptyParts);
		if(plist.size() == 1){
			if(dlist.size() == 1){
				if(m_PortData[i].direction == "in"){
					QScmdin = "iptables -A IN_RISING -p " + m_PortData[i].protocol + " --dport " + ChangePortStyle(m_PortData[i].str) + " -j DROP";
				}else{
					QScmdout = "iptables -A OUT_RISING -p " + m_PortData[i].protocol + " --dport " + ChangePortStyle(m_PortData[i].str) + " -j DROP";
				}
			}
			else{
				QScmdin = "iptables -A IN_RISING -p " + m_PortData[i].protocol + " --dport " + ChangePortStyle(m_PortData[i].str) + " -j DROP";
				QScmdout = "iptables -A OUT_RISING -p " + m_PortData[i].protocol + " --dport " + ChangePortStyle(m_PortData[i].str) + " -j DROP";
			}
		}else{
			if(dlist.size() == 1){
				if(m_PortData[i].direction == "in"){
					QScmdtcpin = "iptables -A IN_RISING -p tcp --dport " + ChangePortStyle(m_PortData[i].str) + " -j DROP";
					QScmdudpin = "iptables -A IN_RISING -p udp --dport " + ChangePortStyle(m_PortData[i].str) + " -j DROP";
				}
				else{
					QScmdtcpout = "iptables -A OUT_RISING -p tcp --dport " + ChangePortStyle(m_PortData[i].str) + " -j DROP";
					QScmdudpout = "iptables -A OUT_RISING -p udp --dport " + ChangePortStyle(m_PortData[i].str) + " -j DROP";
				}
			}else{
				QScmdtcpin = "iptables -A IN_RISING -p tcp --dport " + ChangePortStyle(m_PortData[i].str) + " -j DROP";
				QScmdudpin = "iptables -A IN_RISING -p udp --dport " + ChangePortStyle(m_PortData[i].str) + " -j DROP";
				QScmdtcpout = "iptables -A OUT_RISING -p tcp --dport " + ChangePortStyle(m_PortData[i].str) + " -j DROP";
				QScmdudpout = "iptables -A OUT_RISING -p udp --dport " + ChangePortStyle(m_PortData[i].str) + " -j DROP";
			}

		}

		Enablecmd(QScmdin);
		Enablecmd(QScmdout);
		Enablecmd(QScmdtcpin);
		Enablecmd(QScmdtcpout);
		Enablecmd(QScmdudpin);
		Enablecmd(QScmdudpout);
	}

	ipsqldata->CloseSqldb();
}
