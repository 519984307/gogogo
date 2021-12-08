#ifndef IPTABLES_H
#define IPTABLES_H

#include <QVector>
#include <QString>
#include <QWidget>
#include <iostream>
#include <string>
#include <QDebug>

#include <sqlite3.h>

#include "application.h"

typedef struct port_text{
	QString str="";
	QString protocol="tcp";
	QString direction="in";
}port_Text;

class IpSqlData : public QWidget
{
    Q_OBJECT

public:
	IpSqlData(std::string, QWidget * parent=0);
	void SqlCmd(std::string);
	void SqlQueryIp(std::string, int flag);
	void CloseSqldb();

	QVector<QString> m_IpWhiteDB;
	QVector<QString> m_IpBlackDB;
	QVector<port_Text> m_PortDB;
	
private:
	sqlite3 *db;
	char *errmsg;
	char **resultp;
	std::string databasename;
	std::string dbtablename;
	bool sqlflag;

};

class IpTables : public QWidget
{
    Q_OBJECT
public:
	IpTables(std::string, QWidget *parent=0);

	void setIpWhite();
	void setIpBlack();
	void setPort();
	bool IsNumStr(QString str);
	bool IsIpValid(QString str);
	bool IsPortValid(QString str);
	QString ChangePortStyle(QString str);
	void InitAll();
	void DisableAll();
	void EnableAll();
	void GLTXcmd(std::string);

	QVector<QString> m_IpWhiteData;
	QVector<QString> m_IpBlackData;
	QVector<port_Text> m_PortData;
	IpSqlData *ipsqldata;
	AppConfig *mAppConfig;


private:

	void Enablecmd(QString str);
	int Ipflag;
	int Portflag;

};


#endif
