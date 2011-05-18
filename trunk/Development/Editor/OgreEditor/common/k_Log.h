

/*
��־ϵͳ 
ʱ����ʾ ֧�ֶ�����ɫ��־
���� �ַ��� ���� ���� �����

2008 8 18
*/

#pragma once

// bitmask
enum LogFilters
{
	LOG_FILTER_TRANSPORT_MOVES    = 1,
	LOG_FILTER_CREATURE_MOVES     = 2,
	LOG_FILTER_VISIBILITY_CHANGES = 4
};

//��־��ɫ�б�
enum LogColor
{
	BLACK,
	RED,
	GREEN,
	BROWN,
	BLUE,
	MAGENTA,
	CYAN,
	GREY,
	YELLOW,
	LRED,
	LGREEN,
	LBLUE,
	LMAGENTA,
	LCYAN,
	WHITE,
	COLOR_MAX
};


//const int Color_count = int(MAX_COLOR);

class CLog : public CSingleton<CLog>
{
public:
	CLog() : mpServerFile(NULL), mpNetworkFile(NULL), mpErrorFile(NULL), 
		mbColored(true), mbIncludeTime(true)
	{
		Initialize();
	}

	~CLog()
	{
		if (mpServerFile != NULL)
			fclose(mpServerFile);
		mpServerFile = NULL;

		if( mpNetworkFile != NULL )
			fclose(mpNetworkFile);
		mpNetworkFile = NULL;

		if( mpErrorFile != NULL )
			fclose(mpErrorFile);
		mpErrorFile = NULL;
	}

public:
	void Initialize();
	void InitColors( );


	void outString();
	void outString( const char * str, ... );

	void outError( const char * err, ... );       

	void outBasic( const char * str, ... );       

	void outDetail( const char * str, ... );      
	// ��ӡ�������Ϣ
	void outNetInfo( const char * str, ... ); 
	// ��ӡ�������Ϣ���ļ�
	void fprintNetInfo( const char * str, ... ); 

	void outDebug( const char * str, ... );       

	void SetColor(bool stdout_stream, LogColor color);
	void ResetColor(bool stdout_stream);
	void outTime();
	static void outTimestamp(FILE* file);
	static k_String GetTimestampStr();

private:
	FILE* mpServerFile;
	FILE* mpNetworkFile;
	FILE* mpErrorFile;

	// log/console control

	bool mbColored;
	bool mbIncludeTime;
	LogColor mColors[4];

	CriticalSection_Object;
};


//�õ���־�ṹ��ָ��
CLog* LogManage();

#ifndef SERVER_ERRORS_
#define SERVER_ERRORS_

//���������Ϣ �����������ڴ���ҳ��
#define GetError( assertion, errmsg )\
	if( ! (assertion) ) \
{\
LogManage()->outError( "%s:%i ERROR:\n  %s\n", __FILE__, __LINE__, (char *)errmsg ); \
}

#endif