


/*
字符串结构
2008 8 16
*/


#pragma once


#pragma warning( disable : 4996 4267 )

//初始化字符大小
#define PRELOAD_BUFFER_SIZE 16

class k_String
{
public:
	k_String() : mBuffer(NULL), miStringLen(0), miBufferSize( 0 )
	{
		mPreBuffer[0] = 0;
	}

	k_String( char* pString ) : mBuffer( 0 ), miStringLen( 0 ), miBufferSize( 0 )
	{
		SetBuffer( pString );
	}

	~k_String()
	{
		if ( miStringLen >= PRELOAD_BUFFER_SIZE )
		{
			delete[] mBuffer;
		}
	}

	int GetLength() const
	{
		return miStringLen;
	}

	k_String& operator =( const char* strOri )
	{
		ClearBuffer();
		if( strOri )
			SetBuffer( strOri );

		return *this;
	}

	k_String& operator =( const k_String& strOri )
	{
		if ( this != &strOri )
		{
			ClearBuffer();
			if( strOri.GetLength() )
				SetBuffer( strOri.GetBuffer() );
		}

		return *this;
	}

	//取出索引字节的字符代码
	const char operator [] (int index) const
	{
		if( miStringLen >= PRELOAD_BUFFER_SIZE )
			return mBuffer[index];
		else
			return mPreBuffer[index];
	}

	k_String operator +( const char* strSecond )
	{
		k_String tmp;
		tmp.Append( GetBuffer() );
		if( strSecond )
			tmp.Append( strSecond );

		return tmp;
	}

	k_String operator +( const k_String& strSecond )
	{
		k_String tmp;
		tmp.Append( GetBuffer() );
		tmp.Append( strSecond );

		return tmp;
	}

	k_String& operator +=( const char* strSecond )
	{
		if ( strSecond )
		{
			Append( strSecond );
		}
		return *this;
	}

	k_String& operator +=( const char strSecond )
	{
		char tmp[2];
		tmp[0] = strSecond;
		tmp[1] = 0;
		Append( tmp );

		return *this;
	}


	k_String& operator +=( const k_String& strSecond )
	{
		Append( strSecond );
		return *this;
	}

	//等号判断
	int operator ==( const char* strSecond )
	{
		if ( strSecond )
		{
			//Append( strSecond );
		}
		return -1;
	}

	void ClearBuffer();
	void SetBuffer( const char* bufferOri );
	char* GetBuffer() const;

	int Append( const char* strBuffer );
	int Append( const k_String& strBuffer )
	{
		return Append( strBuffer.GetBuffer() );
	}

protected:
	char mPreBuffer[PRELOAD_BUFFER_SIZE];
	char* mBuffer;
	int miBufferSize;
	int miStringLen;
};

inline void k_String::ClearBuffer()
{
	miStringLen = 0;
}

inline char* k_String::GetBuffer() const
{
	if( miStringLen >= PRELOAD_BUFFER_SIZE )
		return mBuffer;
	else
		return (char*)mPreBuffer;
}

inline void k_String::SetBuffer( const char* bufferOri )
{
	miStringLen = strlen( bufferOri );
	if( miStringLen >= PRELOAD_BUFFER_SIZE )
	{
		if( miBufferSize < miStringLen )
		{
			if( mBuffer )
				delete[] mBuffer;
			miBufferSize = 2;
			while ( miBufferSize <= miStringLen )
				miBufferSize *= 2;

			mBuffer = new char[miBufferSize];
		}

		strcpy( mBuffer, bufferOri );
	}
	else
	{
		strcpy( mPreBuffer, bufferOri );
	}
}

inline
int k_String::Append( const char* strBuffer )
{
	if( !strBuffer )
		return miStringLen;

	int len = miStringLen + strlen( strBuffer );

	if( len >= PRELOAD_BUFFER_SIZE )
	{
		if( miBufferSize < len )
		{
			miBufferSize = 2;
			while ( miBufferSize <= len )
				miBufferSize *= 2;
			char* tmpBuffer = new char[miBufferSize];

			if( mBuffer )
			{
				sprintf( tmpBuffer, "%s%s", mBuffer, strBuffer );
				delete[] mBuffer;
			}
			else
			{
				sprintf( tmpBuffer, "%s%s", mPreBuffer, strBuffer );
			}

			mBuffer = tmpBuffer;
		}
		else
			strcat( mBuffer, strBuffer );
	}
	else
	{
		strcat( mPreBuffer, strBuffer );
	}

	miStringLen = len;
	return miStringLen;
}