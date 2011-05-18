

#include "stdafx.h"
#include "k_Log.h"

enum LogType
{
	LogNormal = 0,
	LogDetails,
	LogDebug,
	LogError,
	LogType_Max
};

const int LogType_count = int(LogType_Max);

void CLog::InitColors( )
{
	mColors[0] = LogColor(13);
	mColors[1] = LogColor(7);
	mColors[2] = LogColor(11);
	mColors[3] = LogColor(9);

	mbColored = true;
}

void CLog::SetColor(bool stdout_stream, LogColor color)
{
	static WORD WinColorFG[COLOR_MAX] =
	{
		0,                                                  // BLACK
		FOREGROUND_RED,                                     // RED
		FOREGROUND_GREEN,                                   // GREEN
		FOREGROUND_RED | FOREGROUND_GREEN,                  // BROWN
		FOREGROUND_BLUE,                                    // BLUE
		FOREGROUND_RED |                    FOREGROUND_BLUE,// MAGENTA
		FOREGROUND_GREEN | FOREGROUND_BLUE,                 // CYAN
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,// WHITE
		// YELLOW
		FOREGROUND_RED | FOREGROUND_GREEN |                   FOREGROUND_INTENSITY,
		// RED_BOLD
		FOREGROUND_RED |                                      FOREGROUND_INTENSITY,
		// GREEN_BOLD
		FOREGROUND_GREEN |                   FOREGROUND_INTENSITY,
		FOREGROUND_BLUE | FOREGROUND_INTENSITY,             // BLUE_BOLD
		// MAGENTA_BOLD
		FOREGROUND_RED |                    FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		// CYAN_BOLD
		FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		// WHITE_BOLD
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
	};

	HANDLE hConsole = GetStdHandle(stdout_stream ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE );
	SetConsoleTextAttribute(hConsole, WinColorFG[color]);
}

void CLog::ResetColor(bool stdout_stream)
{
	HANDLE hConsole = GetStdHandle(stdout_stream ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE );
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED );
}

void CLog::Initialize()
{
	k_String logsDir( "logs" );

	if ( logsDir.GetLength() )
	{
		if ( logsDir[ logsDir.GetLength() - 1 ] != '/' && logsDir[logsDir.GetLength() - 1 ] != '\\' )
		{
			logsDir.Append( "/" );
		}
	}
	k_String logTimestamp( "_" );
	logTimestamp.Append( GetTimestampStr() );

	k_String logfn;
	logfn += logsDir;
	logfn += "ServerLog";
	logfn += logTimestamp;
	logfn += ".log";
	mpServerFile = fopen(( logfn ).GetBuffer(), "w");

	k_String netLog;
	netLog += logsDir;
	netLog += "NetWorkLog";
	netLog += logTimestamp;
	netLog += ".log";
	mpNetworkFile = fopen((netLog).GetBuffer(), "w");

	k_String errorLog;
	errorLog += logsDir;
	errorLog += "ErrorLog";
	errorLog += logTimestamp;
	errorLog += ".log";
	mpErrorFile = fopen((errorLog).GetBuffer(), "w");

	InitColors( );
}

void CLog::outTimestamp(FILE* file)
{
	time_t t = time(NULL);
	tm* aTm = localtime(&t);
	//       YYYY   year
	//       MM     month (2 digits 01-12)
	//       DD     day (2 digits 01-31)
	//       HH     hour (2 digits 00-23)
	//       MM     minutes (2 digits 00-59)
	//       SS     seconds (2 digits 00-59)
	fprintf(file,"%-4d-%02d-%02d %02d:%02d:%02d ",aTm->tm_year+1900,aTm->tm_mon+1,aTm->tm_mday,aTm->tm_hour,aTm->tm_min,aTm->tm_sec);
}

void CLog::outTime()
{
	time_t t = time(NULL);
	tm* aTm = localtime(&t);
	//       YYYY   year
	//       MM     month (2 digits 01-12)
	//       DD     day (2 digits 01-31)
	//       HH     hour (2 digits 00-23)
	//       MM     minutes (2 digits 00-59)
	//       SS     seconds (2 digits 00-59)
	printf("%02d:%02d:%02d ",aTm->tm_hour,aTm->tm_min,aTm->tm_sec);
}

k_String CLog::GetTimestampStr()
{
	time_t t = time(NULL);
	tm* aTm = localtime(&t);
	//       YYYY   year
	//       MM     month (2 digits 01-12)
	//       DD     day (2 digits 01-31)
	//       HH     hour (2 digits 00-23)
	//       MM     minutes (2 digits 00-59)
	//       SS     seconds (2 digits 00-59)
	char buf[20];
	_snprintf(buf,20,"%04d-%02d-%02d_%02d-%02d-%02d",aTm->tm_year+1900,aTm->tm_mon+1,aTm->tm_mday,aTm->tm_hour,aTm->tm_min,aTm->tm_sec);
	return k_String(buf);
}

void CLog::outString()
{
	if(mbIncludeTime)
		outTime();
	printf( "\n" );
	if(mpServerFile)
	{
		outTimestamp(mpServerFile);
		fprintf(mpServerFile, "\n" );
		fflush(mpServerFile);
	}
	fflush(stdout);
}

#define PRINTF(OUT,FRM,RESERR)                      \
{                                                       \
	static char temp_buf[1000];                         \
	va_list ap;                                         \
	va_start(ap, FRM);                                  \
	vsnprintf(temp_buf,1000,FRM,ap);                    \
	va_end(ap);                                         \
	fprintf(OUT,temp_buf);                              \
}

void CLog::outString( const char * str, ... )
{
	if( !str )
		return;

	MAKE_CriticalSection;

	if(mbColored)
		SetColor(true,mColors[LogNormal]);

	if(mbIncludeTime)
		outTime();

	PRINTF(stdout,str,);

	if(mbColored)
		ResetColor(true);

	printf( "\n" );
	if(mpServerFile)
	{
		outTimestamp(mpServerFile);

		va_list ap;
		va_start(ap, str);
		vfprintf(mpServerFile, str, ap);
		fprintf(mpServerFile, "\n" );
		va_end(ap);

		fflush(mpServerFile);
	}
	fflush(stdout);
}

void CLog::outError( const char * err, ... )
{
	if( !err )
		return;

	MAKE_CriticalSection;


	if(mbColored)
		SetColor(false,mColors[LogError]);

	if(mbIncludeTime)
		outTime();

	PRINTF(stderr,err,);

	if(mbColored)
		ResetColor(false);

	fprintf( stderr, "\n" );
	if(mpErrorFile)
	{
		outTimestamp(mpErrorFile);
		fprintf(mpErrorFile, "ERROR:" );

		va_list ap;
		va_start(ap, err);
		vfprintf(mpErrorFile, err, ap);
		va_end(ap);

		fprintf(mpErrorFile, "\n" );
		fflush(mpErrorFile);
	}
	fflush(stderr);
}

void CLog::outBasic( const char * str, ... )
{
	if( !str )
		return;

	MAKE_CriticalSection;

	va_list ap;

	if(mbColored)
		SetColor(true,mColors[LogDetails]);

	if(mbIncludeTime)
		outTime();

	PRINTF(stdout,str,);

	if(mbColored)
		ResetColor(true);

	printf( "\n" );

	if( mpServerFile )
	{
		outTimestamp(mpServerFile);
		va_start(ap, str);
		vfprintf(mpServerFile, str, ap);
		fprintf(mpServerFile, "\n" );
		va_end(ap);
		fflush(mpServerFile);
	}
	fflush(stdout);
}

void CLog::outDetail( const char * str, ... )
{
	if( !str )
		return;

	MAKE_CriticalSection;

	va_list ap;

	if(mbColored)
		SetColor(true,mColors[LogDetails]);

	if(mbIncludeTime)
		outTime();

	PRINTF(stdout,str,);

	if(mbColored)
		ResetColor(true);

	printf( "\n" );

	if( mpServerFile )
	{
		outTimestamp(mpServerFile);
		va_start(ap, str);
		vfprintf(mpServerFile, str, ap);
		fprintf(mpServerFile, "\n" );
		va_end(ap);
		fflush(mpServerFile);
	}

	fflush(stdout);
}

void CLog::outDebug( const char * str, ... )
{
	if( !str )
		return;

	MAKE_CriticalSection;


	if(mbColored)
		SetColor(true,mColors[LogDebug]);

	if(mbIncludeTime)
		outTime();

	PRINTF(stdout,str,);

	if(mbColored)
		ResetColor(true);

	printf( "\n" );

	if( mpServerFile )
	{
		outTimestamp(mpServerFile);

		va_list ap;
		va_start(ap, str);
		vfprintf(mpServerFile, str, ap);
		va_end(ap);

		fprintf(mpServerFile, "\n" );
		fflush(mpServerFile);
	}
	fflush(stdout);
}

void CLog::outNetInfo( const char * str, ... )
{
	if( !str )
		return;

	MAKE_CriticalSection;


	va_list ap;

	if(mbColored)
		SetColor(true,mColors[LogDetails]);

	if(mbIncludeTime)
		outTime();

	PRINTF(stdout,str,);

	if(mbColored)
		ResetColor(true);

	printf( "\n" );

	if( mpNetworkFile )
	{
		outTimestamp(mpNetworkFile);
		va_start(ap, str);
		vfprintf(mpNetworkFile, str, ap);
		fprintf(mpNetworkFile, "\n" );
		va_end(ap);
		fflush(mpNetworkFile);
	}
	fflush(stdout);
}

void CLog::fprintNetInfo( const char * str, ... )
{
	if( !str )
		return;

	MAKE_CriticalSection;

	va_list ap;

	if( mpNetworkFile )
	{
		outTimestamp(mpNetworkFile);
		va_start(ap, str);
		vfprintf(mpNetworkFile, str, ap);
		fprintf(mpNetworkFile, "\n" );
		va_end(ap);
		fflush(mpNetworkFile);
	}
}

CLog* LogManage()
{
	return CLog::getInstance();
}
