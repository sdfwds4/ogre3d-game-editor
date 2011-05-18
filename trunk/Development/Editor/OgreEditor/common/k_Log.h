

/*
日志系统 
时间显示 支持多种颜色日志
分类 字符串 正常 调试 错误等

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

//日志颜色列表
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
	// 打印网络包信息
	void outNetInfo( const char * str, ... ); 
	// 打印网络包信息到文件
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


//得到日志结构的指针
CLog* LogManage();

#ifndef SERVER_ERRORS_
#define SERVER_ERRORS_

//输出错误信息 包含错误所在代码页面
#define GetError( assertion, errmsg )\
	if( ! (assertion) ) \
{\
LogManage()->outError( "%s:%i ERROR:\n  %s\n", __FILE__, __LINE__, (char *)errmsg ); \
}

#endif