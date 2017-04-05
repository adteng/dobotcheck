/*------------------------------------------------------------------------
File name:             utility.h
Author:                linhu
Version:               1.0
Date:                  2006 - 07 - 14
Description:           通用类
Others:       
Revision history:      
   1:
     Date:
     Author:
     Modifiacation:
   2：……
------------------------------------------------------------------------*/
#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "config.h"
//#include <streamb.h>

using namespace std;

#ifdef __cplusplus
	extern "C"
#endif

#define	CONFIGITEMLEN	32
#define uchar unsigned char

class appLog;
class ConfigTable;

//fxh_begin
int timecheck(string);//6位时间HHMMSS

string& ToUpper(string& strValue);
string& ToLower(string& strValue);
string IToString(const int&,int nLen=0);
string LToString(const long&,int nLen=0);
string FToString(const float&,int nLen=0);
string& strLTrim(string& strValue);
string& strRTrim(string& strValue);
string& strTrim(string& strValue);
string& strTrimAll(string& strValue);
string& strLTrim0(string& strValue);
string& strRTrim0(string& strValue);
string& strTrim0(string& strValue);
int strHexToInt(const string& strHex);
string LongTostrHex(const long nInt,int nLen=0);
string IntTostrHex(const int nInt,int nLen=0);
long BCDToLong(uchar ucBCD[],int nBCDLen);
int BCDToInt(uchar ucBCD[],int nBCDLen);
string BCDToString(uchar ucBCD[],int nBCDLen);
long HEXToLong(uchar ucHEX[],int nHEXLen);
int HEXToInt(uchar ucHEX[],int nHEXLen);
string HEXToString(uchar ucHEX[],int nHEXLen);
void LongToBCD(const long lNum,uchar* ucBCD);
void charToHEX(char c,int& ucTmp);
void stringToBCD(const string& strTmp,uchar* ucBCD);
void stringToHEX(const string& strTmp,uchar* ucHEX,int nstrLen=0);
bool isNumber(const string& strIN);	//判断字符串是否含非数字字符
bool isABC(const string& strIN);	//判断是否是英文字符，而不是数字或其他字符
bool isNumOrABC(const string& strIN);	//判断是否是英文字符或数字
void DivKey(byte* byRand,byte* bySK2,byte* byTriDesKey);//用随机数对SK2分散
int BitExclusiveOR(const string& strInput,byte* byteOutPut);//异或
//设置成读卡器数据
int SetDataForReader(const string& strInput,const string& strCMD,byte* byteOutPut);
//校验读卡器数据
int CheckDataFromReader(byte* byteData,int nDataSize);

string CharToString(const char *cHEX,int nHEXLen);
void StringToBCDChar(const string& strTmp,char* cBCD);
void StringToHEXChar(const string& strTmp,char* cHEX);
string SubChar(const char* cp,int nBegin,int nLen);		//按字节转换为字符
string strHexSubChar(const char* cp,int nBegin,int nLen);//按半字节转换为字符
string HEXToFString(uchar ucHEX[],int nHEXLen,int nDecimalLen=2);//转为含小数点的字符串
string IToFString(const int&,int nDecimalLen=2);//转为含小数点的字符串
string& strTrimLQuotation(string& strValue);
string& strTrimRQuotation(string& strValue);
string& strTrimQuotation(string& strValue);
string& strTrimLExcept(string& strValue);//剔除教育局的学生资料的0x09
string& strTrimLTab(string& strValue);
string& strTrimRTab(string& strValue);
string& strTrimTab(string& strValue);
string& strTrimROtherChar(string& strValue);

int MakeCardCheckCode(const string& strNO,
		const string& strCardType,int nIssueVer,int nPurseID);//计算卡号校验码
/*
	BYTE byKeyA[8],byKeyB[8],byDKey[8];
	stringToHEX("A0A1A2A3A4A5",byKeyA);
	stringToHEX("B0B1B2B3B4B5",byKeyB);
	CalMF_PASSWORD(byKeyA,byKeyB,byDKey);
*/
int CalMF_PASSWORD(unsigned char ucInA[],unsigned char ucInB[],
				   unsigned char* ucMF_PASSWORD);
extern ConfigTable theCFG;
extern appLog	theLog;

#define ende ('\001')  //八进制 相当于^A
#define endw ('\002')  //八进制 相当于^B
#define endi ('\003')  //八进制 相当于^C
#define endt ('\004')  //八进制 相当于^D


class Time
{
public:
	typedef enum { STD, FILENAME, SHORT ,SHORTYEAR } TIMEFORMAT;
private:
	tm m_tm;
	TIMEFORMAT m_tf;
	char * m_strTF;
public:
	Time(TIMEFORMAT = STD);
	~Time();
	string getCurTime();

};

#define MAXLOGLINE	100000

class appLog : public streambuf, public ostream
{
private:
	ofstream m_ofs;
	string m_strBuf,m_strFileName;
	string m_strSource;
	bool m_bOutput;
	int m_nLoglines,m_nOpenMode;
public:
	typedef enum { _INFO,_WARNING,_ERROR,_IMPORTANT,_UNKOWN } INFO_TYPE;
	static bool ParseImportantInfo(const string &strInfo, string &strAppCode, string &strType,
		string &strContent)
	{
		strType.assign(strInfo.data() + 31, 4);
		if ( !(strType == "归档" || strType == "警告" || strType == "出错" ) ) return false;
		strAppCode.assign(strInfo.data() + 18, 11); strTrim(strAppCode); strAppCode.erase(strAppCode.end() - 1);
		strContent.assign(strInfo.data() + 37, strInfo.size() - 37);
		return true;
	};

	appLog(bool bOutput = true);

	void SetOutput(bool bOutput){m_bOutput = bOutput;};
	int overflow(int nCh);
	int underflow() { return 0;};

	appLog& operator ()(const string& strSource)
	{
		// Max appcode len = 10
		string tmp;
		unsigned long nSize = strSource.size();
		if (nSize > 10)
			m_strSource = "[" + string(strSource.data(), 10) + "]";
		else if (nSize == 10)
			m_strSource = "[" + strSource + "]";
		else
			m_strSource = "[" + strSource + "]" + string(10 - nSize, ' ');
		return *this;
	};

	appLog::~appLog()
	{
		Close();
	}
	int Close();
	int Open(const string& strFile, int nMode = ios::out|ios::app);

};



class ConfigItem
{
public:
	string	m_strSection;
	string	m_strKey;
	string	m_strValue;

	ConfigItem(const string strSection,const string strKey,const string strValue) 
	{
		m_strSection = strSection;
		m_strKey = strKey;
		m_strValue = strValue;
	};
	ConfigItem() {};
	bool	operator == (const ConfigItem &item) const
	{
		string strSection,strKey,strtmpSection,strtmpKey;
		
		strSection = m_strSection;
		strKey = m_strKey;
		strtmpSection = item.m_strSection;
		strtmpKey = item.m_strKey;
		return ((stricmp(strSection.c_str() , strtmpSection.c_str()) == 0) &&
				(stricmp(strKey.c_str() , strtmpKey.c_str()) == 0));
	};
};

class ConfigTable
{

public:
	ConfigTable(){};
	~ConfigTable();
	int Open(const string strFile);
	int Close();

	int GetValue(const string& strSection, const string& strKey, string& strValue);
	int printit(int nRow=0);

	typedef list<ConfigItem> CONFIGITEMLIST;
//	typedef CONFIGITEMLIST::iterator iterator;
//	typedef CONFIGITEMLIST::const_iterator const_iterator;
	CONFIGITEMLIST m_itemList;

};

//strEnvName：环境变量名
bool OpenDefaultConfig(const string& strEnvName="AUTOCHARGE_CONFIG",bool bEnv=true);

void FloatToByte(float floatNum,unsigned char* byteArry);////浮点数到十六进制转换2
float Hex_To_Decimal(unsigned char *Byte,int num);//十六进制到浮点数

#endif
