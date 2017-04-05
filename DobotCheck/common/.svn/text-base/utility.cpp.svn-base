#include "stdafx.h"
#include "utility.h"


//VC6.0的bug
//编译器内部生成标识符的长度超过255时将对其进行截断到255字符，因此出现了这个警告
//#pragma warning(disable : 4786)


//////////////////////////////////////////////////////////////////////////////////
// class Time
//////////////////////////////////////////////////////////////////////////////////
Time::Time(TIMEFORMAT tf)
{
	m_tf = tf;
/*	switch(tf)
	{
	case SHORT:
		m_strTF = "%04d%02d%02d";
		break;
	case STD:
		m_strTF = "%04d-%02d-%02d %02d:%02d:%02d";
		break;
	case FILENAME:
		m_strTF = "%04d%02d%02d%02d%02d%02d";
		break;
	case SHORTYEAR:
		m_strTF = "%02d%02d%02d%02d%02d%02d";
		break;
	}*/
}

Time::~Time()
{
}

string Time::getCurTime()
{
	char strTime[20];
	string m_strTime;
	_getsystime(&m_tm);
	switch(m_tf)
	{
	case STD:
		sprintf(strTime,"%04d-%02d-%02d %02d:%02d:%02d",m_tm.tm_year+1900,m_tm.tm_mon+1,m_tm.tm_mday,
			m_tm.tm_hour,m_tm.tm_min,m_tm.tm_sec);
		break;
	case SHORT:
		sprintf(strTime,"%04d%02d%02d",m_tm.tm_year+1900,m_tm.tm_mon+1,m_tm.tm_mday);
		break;
	case FILENAME:
		sprintf(strTime,"%04d%02d%02d%02d%02d%02d",m_tm.tm_year+1900,m_tm.tm_mon+1,m_tm.tm_mday,
			m_tm.tm_hour,m_tm.tm_min,m_tm.tm_sec);
		break;
	case SHORTYEAR:
		sprintf(strTime,"%02d%02d%02d%02d%02d%02d",m_tm.tm_year-100,m_tm.tm_mon+1,m_tm.tm_mday,
			m_tm.tm_hour,m_tm.tm_min,m_tm.tm_sec);
		break;
	}
	m_strTime = strTime;
	return m_strTime;
}

//////////////////////////////////////////////////////////////////////////////////
// class ConfigTable
//////////////////////////////////////////////////////////////////////////////////

appLog::appLog(bool bOutput): streambuf(), ostream((streambuf*) this)
{
	//operator () ("应用");
	m_bOutput = bOutput;
};

int appLog::overflow(int nCh = EOF)
{
	if (nCh == ende || nCh == endw || nCh == endi || nCh == endt)
	{
		string strType;
		switch(nCh)
		{
		case endi:
			strType = "[信息]";
			break;
		case endw:
			strType = "[警告]";
			break;
		case ende:
			strType = "[出错]";
			break;
		case endt:
			strType = "[归档]";
			break;
		}
		string strTime;  strTime = Time().getCurTime();
		strTime = "[" + string(strTime.data() + 5, strTime.size() - 5) + "]";
		m_ofs << strTime << " " //<< m_strSource << " "
			<< strType << " " << m_strBuf << endl;
		m_ofs.flush();
		if (m_bOutput)
		{
			std::cout << strTime << " " //<< m_strSource << " "
				<< m_strBuf.substr(0,800) << endl;
		}
		m_strBuf = "";
		m_nLoglines ++;
		if(m_nLoglines == MAXLOGLINE)
		{
			m_nLoglines = 0;
			m_ofs.close();
			m_ofs.clear();
			string strCMD="rename " + m_strFileName;
			string strTime;  strTime = Time().getCurTime();
			strTime.erase(16,1);strTime.erase(13,1);strTime.erase(10,1);
			strTime.erase(7,1);strTime.erase(4,1);
			strCMD.append(" " + m_strFileName+strTime);
			system(strCMD.c_str());
			m_ofs.open(m_strFileName.c_str(), m_nOpenMode);
		}
		//operator () ("");
		//operator () ("应用");
	}
	else
	{
		m_strBuf += nCh;
	}
	return nCh;
};

int appLog::Open(const string& strFile, int nMode)
{
	m_nLoglines = 0;
	m_ofs.close();
	m_ofs.clear();
	m_strFileName = strFile;
	m_nOpenMode = nMode;
	m_ofs.open(m_strFileName.c_str(), m_nOpenMode);
	if (!m_ofs.good()) return -1;
	return 1;
};

int appLog::Close()
{
	if (m_ofs)
		int i = 0;
	m_ofs.close();
	m_ofs.clear();
	return 0;
};

appLog theLog;

//////////////////////////////////////////////////////////////////////////////////
// class ConfigTable
//////////////////////////////////////////////////////////////////////////////////

ConfigTable::~ConfigTable()
{
	Close();
};

int ConfigTable::Open(const string strFile)
{
	char tmp[1024];
	int nLineLen;
	std::ifstream ifs;
	string strSection;
	string strLine;
	int pos;

	// Close previous table if needed
	Close();

	ifs.open(strFile.c_str(), ios::in);
	if (!ifs) return -1;
	while(ifs.getline(tmp, 1023))
	{
		strLine = tmp;
		strTrim(strLine);
		nLineLen = strLine.size();
		if (nLineLen < 1) continue;

		char cLead = strLine[0];
		// '#' mean remark
		if (cLead == '#')
			continue;
		// '[' mean new section
		else if (cLead == '[')
		{
			strncpy(tmp,&strLine[1], nLineLen - 2);
			tmp[nLineLen - 2] = '\0';
			strSection = tmp;
		}
		// error format 
		else if ((pos = strcspn(strLine.c_str(),"=")) >= nLineLen)
			continue;
		else
		{
			char ctmp[1024];
			ConfigItem item;
			item.m_strSection = strSection;
			strncpy(ctmp , &strLine[0], pos); 
			ctmp[pos] = '\0';
			item.m_strKey = ctmp;
			strTrim(item.m_strKey);
			memset(ctmp,0,1024);
			strncpy(ctmp , &strLine[pos+1],(nLineLen - pos - 1)); 
			ctmp[nLineLen - pos - 1] = '\0';
			item.m_strValue = ctmp;
			strTrim(item.m_strValue);
			m_itemList.push_back(item);//realkobe
		}
	};
	ifs.close();
	return 1;
};

int ConfigTable::Close()
{
	m_itemList.clear();
	return 1;
};

int ConfigTable::printit(int nRow)
{
	CONFIGITEMLIST::iterator it, begin, end;
	begin = m_itemList.begin();
	end = m_itemList.end();

	it = begin;
	if(nRow > 0)
	{
		for(int i=0;i<nRow;i++)
			it ++;
		theLog<<it->m_strSection.c_str()<<"|"
			<<it->m_strKey.c_str()<<"|"<<it->m_strValue.c_str()<<endi;
		return 0;
	}
	while(1)
	{
		theLog<<it->m_strSection.c_str()<<"|"
			<<it->m_strKey.c_str()<<"|"<<it->m_strValue.c_str()<<endi;
		it ++;
		if(it == end)
			break;
	}
	return 0;
};

int ConfigTable::GetValue(const string& strSection, const string& strKey, string& strValue)
{
	CONFIGITEMLIST::iterator it, begin, end;
	begin = m_itemList.begin();
	end = m_itemList.end();

//	it = std::find(begin, end, ConfigItem(strSection, strKey, ""));
//	if (it == end)
//	{
//		strValue = "";
//		return 0;
//	}

	it = begin;
	while(1)
	{
		if(*it == ConfigItem(strSection, strKey, ""))
		{
			strValue = it->m_strValue;
			return 0;
		}
		it ++;
		if(it == end)
			break;
	}

//	strValue = it->m_strValue;
	return 1;
};


ConfigTable theCFG;


string& ToUpper(string& strValue)
{
	for (int i = 0; i < strValue.size(); i++)
	{
		strValue[i] = toupper(strValue[i]);
	}
	return strValue;
};

string& ToLower(string& strValue)
{
	for (int i = 0; i < strValue.size(); i++)
	{
		strValue[i] = tolower(strValue[i]);
	}
	return strValue;
};

string IToString(const int& nTmp,int nLen)
{
	string strTmp;
	char tmp[20];
	sprintf(tmp, "%d", nTmp);
	strTmp = tmp;
	if(nLen > strTmp.size())
		strTmp.insert(0,"0000000000000000000",nLen - strTmp.size());
	return strTmp;
};

string LToString(const long& nTmp,int nLen)
{
	string strTmp;
	char tmp[20];
	sprintf(tmp, "%ld", nTmp);
	strTmp = tmp;
	if(nLen > strTmp.size())
		strTmp.insert(0,"0000000000000000000",nLen - strTmp.size());
	return strTmp;
};

string FToString(const float& fTmp,int nLen)
{
	string strTmp;
	char tmp[20];
	sprintf(tmp, "%.2f", fTmp);
	strTmp = tmp;
	if(nLen > strTmp.size())
		strTmp.insert(0,"0000000000000000000",nLen - strTmp.size());
	return strTmp;
};

string& strLTrim(string& strValue)
{
	while (strValue.begin() != strValue.end() && *strValue.begin() == ' ')
	{
		strValue.erase(strValue.begin());
	}
	return strValue;
};

string& strRTrim(string& strValue)
{
	while (strValue.begin() != strValue.end() && *(strValue.end() - 1) == ' ')
	{
		strValue.erase(strValue.end() - 1);
	}
	return strValue;
};

string& strTrimAll(string& strValue)
{
	string::iterator it = strValue.begin();
	while (it != strValue.end())
	{
		if(*it == ' ')
			strValue.erase(it);
		else it++;
	}
	return strValue;
};

string& strTrim(string& strValue)
{
	return strRTrim(strLTrim(strValue));
};

string& strLTrim0(string& strValue)
{
	while (strValue.begin() != strValue.end() && *strValue.begin() == '0')
	{
		strValue.erase(strValue.begin());
	}
	return strValue;
};

string& strRTrim0(string& strValue)
{
	while (strValue.begin() != strValue.end() && *strValue.end() == '0')
	{
		strValue.erase(strValue.end());
	}
	return strValue;
};

string& strTrim0(string& strValue)
{
	return strRTrim0(strLTrim0(strValue));
};

int strHexToInt(const string& strHex)
{
	int nRet = 0;
	char cRet[2];

	if(strHex.size() > 8)
		return 0;
	for(int i = 0; i < strHex.size() ; i ++)
	{
//		strncpy(cRet , (strHex.end() - (i + 1)),1);//lyle
		strncpy(cRet, (const char*)&(*(strHex.end() - (i + 1))), 1);
		cRet[1] = '\0';
		switch(*(strHex.end() - (i + 1)))
		{
		case 'A':
		case 'a':
			nRet += pow((float)16,i) * 10;
			break;
		case 'B':
		case 'b':
			nRet += pow((float)16,i) * 11;
			break;
		case 'C':
		case 'c':
			nRet += pow((float)16,i) * 12;
			break;
		case 'D':
		case 'd':
			nRet += pow((float)16,i) * 13;
			break;
		case 'E':
		case 'e':
			nRet += pow((float)16,i) * 14;
			break;
		case 'F':
		case 'f':
			nRet += pow((float)16,i) * 15;
			break;
		default:
			nRet += pow((float)16,i) * atoi(cRet);
			break;
		}
	}
	return nRet;
};

string LongTostrHex(const long nInt,int nLen)
{
	string strTmp;
	char cBuf[21];

	memset(cBuf,0,21);
	ltoa(nInt,cBuf,16);
	strTmp = cBuf;
	if(nLen > strTmp.size())
		strTmp.insert(0,"0000000000000000000",nLen - strTmp.size());
	return ToUpper(strTmp);
};

string IntTostrHex(const int nInt,int nLen)
{
	string strTmp;
	char cBuf[21];

	memset(cBuf,0,21);
	itoa(nInt,cBuf,16);
	strTmp = cBuf;
	if(nLen > strTmp.size())
		strTmp.insert(0,"0000000000000000000",nLen - strTmp.size());
	return ToUpper(strTmp);
};

long BCDToLong(uchar ucBCD[],int nBCDLen)
{
	long lTmp = 0;
	int nTmp;
	uchar ucTmp[2],ucTmp1 = 0xF0 , ucTmp2 = 0x0F;
	memset(ucTmp,0,2);
	for(int i=0;i<nBCDLen;i++)
	{
		memcpy(ucTmp , &ucBCD[i],1);
		nTmp = ucTmp[0] & ucTmp1;
		nTmp = nTmp >> 4;
		if(nTmp > 9) return -1;
		lTmp = (10*lTmp) + nTmp;
		nTmp = ucTmp[0] & ucTmp2;
		if(nTmp > 9) return -1;
		lTmp = (10*lTmp) + nTmp;
	}
	return lTmp;
};

int BCDToInt(uchar ucBCD[],int nBCDLen)
{
	int lTmp = 0;
	int nTmp;
	uchar ucTmp[2],ucTmp1 = 0xF0 , ucTmp2 = 0x0F;
	memset(ucTmp,0,2);
	for(int i=0;i<nBCDLen;i++)
	{
		memcpy(ucTmp , &ucBCD[i],1);
		nTmp = ucTmp[0] & ucTmp1;
		nTmp = nTmp >> 4;
		if(nTmp > 9) return -1;
		lTmp = (10*lTmp) + nTmp;
		nTmp = ucTmp[0] & ucTmp2;
		if(nTmp > 9) return -1;
		lTmp = (10*lTmp) + nTmp;
	}
	return lTmp;
};

string BCDToString(uchar ucBCD[],int nBCDLen)
{
/*  不含前面的0
	char cBuf[17];
	long lTmp;
	memset(cBuf,0,17);
	lTmp = BCDToLong(ucBCD,nBCDLen);
	ltoa(lTmp,cBuf,10);
	return cBuf;
*/
	string strTmp = "";
	int nTmp;
	uchar ucTmp[2],ucTmp1 = 0xF0 , ucTmp2 = 0x0F;
	memset(ucTmp,0,2);
	for(int i=0;i<nBCDLen;i++)
	{
		memcpy(ucTmp , &ucBCD[i],1);
		nTmp = ucTmp[0] & ucTmp1;
		nTmp = nTmp >> 4;
		if(nTmp > 9) nTmp=0;//return "";
		strTmp.append(LongTostrHex(nTmp));
		nTmp = ucTmp[0] & ucTmp2;
		if(nTmp > 9) nTmp=0;//return "";
		strTmp.append(LongTostrHex(nTmp));
	}
	return strTmp;
};

long HEXToLong(uchar ucHEX[],int nHEXLen)
{
	long lTmp = 0;
	int nTmp;
	uchar ucTmp[2],ucTmp1 = 0xF0 , ucTmp2 = 0x0F;
	memset(ucTmp,0,2);
	for(int i=0;i<nHEXLen;i++)
	{
		memcpy(ucTmp , &ucHEX[i],1);
		nTmp = ucTmp[0] & ucTmp1;
		nTmp = nTmp >> 4;
		lTmp = (16*lTmp) + nTmp;
		nTmp = ucTmp[0] & ucTmp2;
		lTmp = (16*lTmp) + nTmp;
	}
	return lTmp;
};

int HEXToInt(uchar ucHEX[],int nHEXLen)
{
	int lTmp = 0;
	int nTmp;
	uchar ucTmp[2],ucTmp1 = 0xF0 , ucTmp2 = 0x0F;
	memset(ucTmp,0,2);
	for(int i=0;i<nHEXLen;i++)
	{
		memcpy(ucTmp , &ucHEX[i],1);
		nTmp = ucTmp[0] & ucTmp1;
		nTmp = nTmp >> 4;
		lTmp = (16*lTmp) + nTmp;
		nTmp = ucTmp[0] & ucTmp2;
		lTmp = (16*lTmp) + nTmp;
	}
	return lTmp;
};

string HEXToString(uchar ucHEX[],int nHEXLen)
{
	string strTmp = "";
	int nTmp;
	uchar ucTmp[2],ucTmp1 = 0xF0 , ucTmp2 = 0x0F;
	memset(ucTmp,0,2);
	for(int i=0;i<nHEXLen;i++)
	{
		memcpy(ucTmp , &ucHEX[i],1);
		nTmp = ucTmp[0] & ucTmp1;
		nTmp = nTmp >> 4;
		strTmp.append(LongTostrHex(nTmp));
		nTmp = ucTmp[0] & ucTmp2;
		strTmp.append(LongTostrHex(nTmp));
	}
	return strTmp;
};

string HEXToFString(uchar ucHEX[],int nHEXLen,int nDecimalLen)
{
	int nTmp = HEXToInt(ucHEX,nHEXLen);
	string strTmp = IToString(nTmp);
	if(nTmp <= 9) return "0.0" + strTmp;
	if(nTmp <= 99) return "0." + strTmp;
	strTmp.insert(strTmp.size()-2,".",1);
	return strTmp;
};

string IToFString(const int& nTmp,int nDecimalLen)
{
	string strTmp = IToString(nTmp);
	if(nTmp <= 9) return "0.0" + strTmp;
	if(nTmp <= 99) return "0." + strTmp;
	strTmp.insert(strTmp.size()-2,".",1);
	return strTmp;
};

string CharToString(const char *cHEX,int nHEXLen)
{
	string strTmp = "";
	int nTmp;
	uchar ucTmp[2],ucTmp1 = 0xF0 , ucTmp2 = 0x0F;
	memset(ucTmp,0,2);
	for(int i=0;i<nHEXLen;i++)
	{
		memcpy(ucTmp , &cHEX[i],1);
		nTmp = ucTmp[0] & ucTmp1;
		nTmp = nTmp >> 4;
		strTmp.append(LongTostrHex(nTmp));
		nTmp = ucTmp[0] & ucTmp2;
		strTmp.append(LongTostrHex(nTmp));
	}
	return strTmp;
};

string SubChar(const char* cp,int nBegin,int nLen)
{
	string strTmp = cp;
	return strTrim(strTmp.substr(nBegin,nLen));
};

string strHexSubChar(const char* cp,int nBegin,int nLen)
{
	string strTmp = CharToString(cp+nBegin,nLen);
	return strTmp;//.substr(nBegin,nLen);
};

void LongToBCD(const long lNum,uchar* ucBCD)
{
	uchar ucTmp,ucRet[8];
	int nTmp1,nTmp2,nPoint=7;
	long lTmp;

	lTmp = lNum;
	while(1)
	{
		nTmp2 = lTmp % 10;
		lTmp -= nTmp2;
		lTmp = lTmp/10;
		nTmp1 = lTmp % 10;
		lTmp = lTmp/10;
		nTmp1 = nTmp1 << 4;
		ucTmp = nTmp1 + nTmp2;
		ucRet[nPoint] = ucTmp;
		nPoint --;
		if(nPoint < 0 || lTmp <= 0)
			break;
	}
	for(int i=nPoint;i >= 0;i--)
	{
		ucRet[i] = 0x00;
	}
	memcpy(ucBCD,&ucRet,8);
	return ;
};

void stringToBCD(const string& strTmp,uchar* ucBCD)
{
	uchar ucTmp;
	int nTmp=0,nTmp1,nTmp2=0;
	int nSize = strTmp.size();
	if(nSize <= 0)
		return ;

	uchar *ucRet = new uchar[nSize];
	memset(ucRet,0,nSize);
	if(nSize == 1)
	{
		ucRet[0] = atoi(strTmp.substr(0,1).c_str());
		memcpy(ucBCD,&ucRet,1);
		delete [] ucRet;
		ucRet = NULL;
		return;
	}
	for(int i=0;i<nSize;i++)
	{
		nTmp1 = atoi(strTmp.substr(i,1).c_str());
		i++;
		nTmp2 = atoi(strTmp.substr(i,1).c_str());
		nTmp1 = nTmp1 << 4;
		ucTmp = nTmp1 + nTmp2;
		ucRet[nTmp] = ucTmp;
		nTmp ++;
	}
	memcpy(ucBCD,ucRet,nTmp);
	delete [] ucRet;
	ucRet = NULL;

	return ;
};

void charToHEX(char c,int& ucTmp)
{
	char a=c;
	switch(a)
	{
	case 'a':
	case 'A':
		ucTmp = 10;//'A'
		break;
	case 'b':
	case 'B':
		ucTmp = 11;//'B'
		break;
	case 'c':
	case 'C':
		ucTmp = 12;//'C'
		break;
	case 'd':
	case 'D':
		ucTmp = 13;//'D'
		break;
	case 'e':
	case 'E':
		ucTmp = 14;//'E'
		break;
	case 'f':
	case 'F':
		ucTmp = 15;//'F'
		break;
	default:
		ucTmp = atoi(&c);
		break;
	}
};

void StringToBCDChar(const string& strTmp,char* cBCD)
{
	stringToBCD(strTmp,(uchar*)cBCD);
};

void StringToHEXChar(const string& strTmp,char* cHEX)
{
	stringToHEX(strTmp,(uchar*)cHEX);
};

void stringToHEX(const string& strTmp,uchar* ucHEX,int nstrLen)
{
	uchar ucTmp;
	char c;
	int nTmp=0,nTmp1,nTmp2=0;
	int nSize ;
	if(nstrLen == 0)
		nSize = strTmp.size();
	else
		nSize = nstrLen;
	if(nSize <= 0)
		return ;

	uchar *ucRet = new uchar[nSize];
	memset(ucRet,0,nSize);
	if(nSize == 1)
	{
		memcpy(&c,strTmp.substr(0,1).c_str(),1);
		charToHEX(c,nTmp);
		ucRet[0] = nTmp;
		memcpy(ucHEX,&ucRet,1);
		delete [] ucRet;
		ucRet = NULL;
		return;
	}
	for(int i=0;i<nSize;i++)
	{
		memcpy(&c,strTmp.substr(i,1).c_str(),1);
		charToHEX(c,nTmp1);
		i++;
		memcpy(&c,strTmp.substr(i,1).c_str(),1);
		charToHEX(c,nTmp2);
		nTmp1 = nTmp1 << 4;
		ucTmp = nTmp1 + nTmp2;
		ucRet[nTmp] = ucTmp;
		nTmp ++;
	}
	memcpy(ucHEX,ucRet,nTmp);
	delete [] ucRet;
	ucRet = NULL;
	return ;
};

bool isNumber(const string& strIN)
{
	char uc[2];

	for(int i=0;i<strIN.size();i++)
	{
		strcpy(uc , strIN.substr(i,1).c_str());
		if(uc[0] > 0x39 || uc[0] < 0x30) 
			return false;
	}
	return true;
};

bool isABC(const string& strIN)
{
	char uc[2];

	for(int i=0;i<strIN.size();i++)
	{
		strcpy(uc , strIN.substr(i,1).c_str());
		if((uc[0] < 0x41) || 
			(uc[0] > 0x7A) || 
			(uc[0] >= 0x5B && uc[0] <= 0x60)) 
			return false;
	}
	return true;
};

bool isNumOrABC(const char cIN)
{
	if((cIN >= 0x30 && cIN <=0x39) || 
		(cIN >= 0x41 && cIN <=0x5A) || 
		(cIN >= 0x61 && cIN <=0x7A)) 
		return true;
	else
		return false;
};

void DivKey(byte* byRand,byte* bySK2,byte* byTriDesKey)
{
	BYTE byKey[16],byKeyA[8],byKeyB[8],m_Rand1[8],byInit[8];

	memset(byInit,0,8);
	my3des(byKeyA,byRand,bySK2,0);//KeyA:随机数用基础密钥做３ＤＥＳ加密
	for(int i=0;i<8;i++)
	{
		m_Rand1[i] = ~byRand[i];
	}
	my3des(byKeyB,m_Rand1,bySK2,0);//KeyB:随机数按位取反后用基础密钥做３ＤＥＳ加密
	memcpy(byKey,byKeyA,8);
	memcpy(byKey+8,byKeyB,8);//Key=KeyA+KeyB
//	my3des(byMACKey,byInit,byKey,0);//Key做3DES加密8字节0做为MAC_key
	memcpy(byTriDesKey,byKey,16);
};

int BitExclusiveOR(const string& strInput,byte* byteOutPut)
{
	int nSize = strInput.size()/2 + 2;
	byte byteCheck;
	byte *byteTmp = new byte[nSize];
	byteTmp[0] = 0xBA;
	stringToHEX(IntTostrHex(nSize-1,2),byteTmp+1,2);
	stringToHEX(strInput,byteTmp+2);
	memcpy(byteOutPut,byteTmp,nSize);
	byteCheck = byteTmp[0]^0x00;
	for(int i=1;i<nSize;i++)
	{
		byteCheck = byteTmp[i]^byteCheck;
	}
	byteOutPut[nSize] = byteCheck;
	delete [] byteTmp;
	byteTmp = NULL;
	return 0;
};

int SetDataForReader(const string& strInput,const string& strCMD,byte* byteOutPut)
{
	int nSize = strInput.size()/2 + 2;
	byte byteCheck;
	byte *byteTmp = new byte[nSize+2];
	byteTmp[0] = 0xBA;
	stringToHEX(IntTostrHex(nSize,2),byteTmp+1,2);
	stringToHEX(strCMD,byteTmp+2,2);
	stringToHEX(strInput,byteTmp+3,strInput.size());
	memcpy(byteOutPut,byteTmp,nSize+2);
	byteCheck = byteTmp[0]^0x00;
	for(int i=1;i<nSize+1;i++)
	{
		byteCheck = byteTmp[i]^byteCheck;
	}
	byteOutPut[nSize+1] = byteCheck;
	delete [] byteTmp;
	byteTmp = NULL;
	return nSize+2;
};

int CheckDataFromReader(byte* byteData,int nDataSize)
{
	byte byteCheck;
	byteCheck = byteData[0]^0x00;
	for(int i=1;i<(nDataSize-1);i++)
	{
		byteCheck = byteData[i]^byteCheck;
	}
	if(byteData[nDataSize-1] != byteCheck)
	{
		theLog<<"CheckDataFromReader error!!!"<<ende;
		return -1;
	}
	if(byteData[0] != 0xBD)
	{
		theLog<<"CheckDataFromReader header error!!!"<<ende;
		return -2;
	}
	if(byteData[3] != 0x00)
	{
		theLog<<"CheckDataFromReader status error!!!"<<byteData[2]<<ende;
		return -3;
	}
	return 0;
};

bool OpenDefaultConfig(const string& strEnvName,bool bEnv)
{
	char    *str;
	string config_fname;
	if(bEnv)
	{
		if(!(str = getenv(strEnvName.c_str())))
		{
			std::cout<<"can't find environment varible: "<<strEnvName<<endl;
			return false;
		}
		config_fname = str;
	}else
		config_fname = strEnvName;
//	char config_fname[] = "C:\\项目\\自动充值\\program\\AutoCharge\\AutoChargeConsole\\AutoChargeConsole.ini";
	if(theCFG.Open(config_fname) < 1)
	{
		std::cout<<"can't open config file:"<<config_fname<<endl;
		return false;
	}

	return true;
};

string& strTrimLQuotation(string& strValue)
{
	while (strValue.begin() != strValue.end() && 
		(*strValue.begin() == '"' || (*strValue.begin() == (char)0x0D)))
	{
		strValue.erase(strValue.begin());
	}
	return strValue;
};

string& strTrimRQuotation(string& strValue)
{
	while (strValue.begin() != strValue.end() && *(strValue.end() - 1) == '"')
	{
		strValue.erase(strValue.end() - 1);
	}
	return strValue;
};

string& strTrimQuotation(string& strValue)
{
	return strTrimRQuotation(strTrimLQuotation(strValue));
};

string& strTrimLTab(string& strValue)
{
	while ((strValue.begin() != strValue.end()) && 
		(*strValue.begin() == '	'))
	{
		strValue.erase(strValue.begin());
	}
	return strValue;
};

string& strTrimRTab(string& strValue)
{
	while (strValue.begin() != strValue.end() && *(strValue.end() - 1) == '	')
	{
		strValue.erase(strValue.end() - 1);
	}
	return strValue;
};

string& strTrimTab(string& strValue)
{
	return strTrimRTab(strTrimLTab(strValue));
};

//剔除教育局学生资料的0x09
string& strTrimLExcept(string& strValue)
{
	while (strValue.begin() != strValue.end() && 
		(*strValue.begin() == '"' || (*strValue.begin() == (char)0x09)))
	{
		strValue.erase(strValue.begin());
	}
	return strValue;
};

//剔除教育局学籍号多余的字符(如??)
string& strTrimROtherChar(string& strValue)
{
	while (strValue.begin() != strValue.end() && 
		(*strValue.begin() == '"' || (!isNumOrABC(*(strValue.end() - 1)))))
	{
		strValue.erase(strValue.end() - 1);
	}
	return strValue;
};

int timecheck(string time)//fangxh
{

//检查是否非法字符
const char* ch;
ch=time.c_str();
int n=0;
   for(int i=0;i<=5;i++)
   {
   n=ch[i];
     if (n>57||n<48)
	 { 
		 return 1;}
      }

//检查范围
   string stemp;
   char ch_t[10];
   int m=0;
   stemp=time.substr(0,2);
   strcpy(ch_t,stemp.c_str());
   sscanf(ch_t,"%d",&m);
         if(m<0 ||m>23 )
		 {    
	       return 2;
		 }
   stemp=time.substr(2,2);
   strcpy(ch_t,stemp.c_str());
   sscanf(ch_t,"%d",&m);
         if(m<0 ||m>59 )
		 {    
	       return 2;
		 }

   stemp=time.substr(4,2);
   strcpy(ch_t,stemp.c_str());
   sscanf(ch_t,"%d",&m);
         if(m<0 ||m>59 )
		 {    
	       return 2;
		 }

return 0;
};

int MakeCardCheckCode(const string& strNO,
		const string& strCardType,int nIssueVer,int nPurseID)
{
	int nTmp=0,nVer;
	nVer = atoi(strCardType.substr(0,1).c_str()) + 
		atoi(strCardType.substr(1,1).c_str()) + nIssueVer + nPurseID;
	for(int j=0;j<15;j++)
	{
		nTmp = atoi(strNO.substr(j,1).c_str()) + nTmp;
	}
	nTmp += nVer;
	nTmp = nTmp % 10;
	return nTmp;
};

int CalMF_PASSWORD(unsigned char ucInA[],unsigned char ucInB[],unsigned char* ucMF_PASSWORD)
{
	unsigned char byKey[16],byInit[8];
	unsigned char ucKeyA[6],ucDKeyA[8],byTmpA;
	unsigned char ucKeyB[6],ucDKeyB[8],byTmpB;

	//Cal DkeyA
	memcpy(ucKeyA,ucInA,6);
	memset(ucDKeyA,0,8);

	byTmpA = ((ucKeyA[5] & 0x80) >> 6) |	
		((ucKeyA[4] & 0x80) >> 5) |	
		((ucKeyA[3] & 0x80) >> 4) |	
		((ucKeyA[2] & 0x80) >> 3) |	
		((ucKeyA[1] & 0x80) >> 2) |	
		((ucKeyA[0] & 0x80) >> 1) ;
	byTmpA = byTmpA & 0x7E;
	memset(ucDKeyA+1,byTmpA,1);

	byTmpA = ucKeyA[5] & 0x7F;
	byTmpA = byTmpA << 1;
	memset(ucDKeyA+2,byTmpA,1);
	byTmpA = ucKeyA[4] & 0x7F;
	byTmpA = byTmpA << 1;
	memset(ucDKeyA+3,byTmpA,1);
	byTmpA = ucKeyA[3] & 0x7F;
	byTmpA = byTmpA << 1;
	memset(ucDKeyA+4,byTmpA,1);
	byTmpA = ucKeyA[2] & 0x7F;
	byTmpA = byTmpA << 1;
	memset(ucDKeyA+5,byTmpA,1);
	byTmpA = ucKeyA[1] & 0x7F;
	byTmpA = byTmpA << 1;
	memset(ucDKeyA+6,byTmpA,1);
	byTmpA = ucKeyA[0] & 0x7F;
	byTmpA = byTmpA << 1;
	memset(ucDKeyA+7,byTmpA,1);

	//Cal DkeyB
	memcpy(ucKeyB,ucInB,6);
	memset(ucDKeyB,0,8);

	byTmpB = ucKeyB[5] & 0x7F;
	byTmpB = byTmpB << 1;
	memset(ucDKeyB,byTmpB,1);
	byTmpB = ucKeyB[4] & 0x7F;
	byTmpB = byTmpB << 1;
	memset(ucDKeyB+1,byTmpB,1);
	byTmpB = ucKeyB[3] & 0x7F;
	byTmpB = byTmpB << 1;
	memset(ucDKeyB+2,byTmpB,1);
	byTmpB = ucKeyB[2] & 0x7F;
	byTmpB = byTmpB << 1;
	memset(ucDKeyB+3,byTmpB,1);
	byTmpB = ucKeyB[1] & 0x7F;
	byTmpB = byTmpB << 1;
	memset(ucDKeyB+4,byTmpB,1);
	byTmpB = ucKeyB[0] & 0x7F;
	byTmpB = byTmpB << 1;
	memset(ucDKeyB+5,byTmpB,1);

	byTmpB = ((ucKeyB[5] & 0x80) >> 1) |	
		((ucKeyB[4] & 0x80) >> 2) |	
		((ucKeyB[3] & 0x80) >> 3) |	
		((ucKeyB[2] & 0x80) >> 4) |	
		((ucKeyB[1] & 0x80) >> 5) |	
		((ucKeyB[0] & 0x80) >> 6) ;
	byTmpB = byTmpB & 0x7E;
	memset(ucDKeyB+6,byTmpB,1);

	//CalDKey
	memset(byInit,0,8);
	memcpy(byKey,ucDKeyA,8);
	memcpy(byKey+8,ucDKeyB,8);
	my3des(byInit,byInit,byKey,0);
	memcpy(ucMF_PASSWORD,byInit+7,1);
	memcpy(ucMF_PASSWORD+1,byInit+6,1);
	memcpy(ucMF_PASSWORD+2,byInit+5,1);
	memcpy(ucMF_PASSWORD+3,byInit+4,1);
	memcpy(ucMF_PASSWORD+4,byInit+3,1);
	memcpy(ucMF_PASSWORD+5,byInit+2,1);
	memcpy(ucMF_PASSWORD+6,byInit+1,1);
	memcpy(ucMF_PASSWORD+7,byInit,1);
	return 0;
};


void FloatToByte(float floatNum, unsigned char* byteArry)////浮点数到十六进制转换2
{
	char* pchar = (char*)&floatNum;
	for (int i = 0;i<sizeof(float);i++)
	{
		*byteArry = *pchar;
		pchar++;
		byteArry++;
	}
}

float Hex_To_Decimal(unsigned char *Byte, int num)//十六进制到浮点数
{
	// char cByte[4];//方法一
	// for (int i=0;i<num;i++)
	// {
	// cByte[i] = Byte[i];
	// }
	// 
	// float pfValue=*(float*)&cByte;
	//return pfValue;

	return *((float*)Byte);//方法二

}


