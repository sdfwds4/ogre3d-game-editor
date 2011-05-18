

/*
3Î¬ÏòÁ¿
2008 8 16
*/

#pragma once

#include "stdafx.h"

//  LLvector3 = |x y z w|


int GetWordLen( char* pBuffer, char token );

class k_Vector3
{
public:
	k_Vector3()
	{
		mV[0] = 0.f;
		mV[1] = 0.f;
		mV[2] = 0.f;
	}

	k_Vector3( const float x, const float y, const float z );

	~k_Vector3()
	{
	}

	k_Vector3& operator =( const float& value )
	{
		mV[0] = value;
		mV[1] = value;
		mV[2] = value;

		return *this;
	}

	void InitFromString( char* strOriVector3 )
	{
		if( strOriVector3 )
		{
			char buffer[32];
			char* pCursorPos = strOriVector3;

			int size = GetWordLen( pCursorPos, ',' );
			if ( size > 0 )
			{
				memcpy( buffer, pCursorPos, size );
				buffer[size] = 0;
				mV[0] = (float)atof(buffer);
			}
			pCursorPos = pCursorPos + (size + 1);

			size = GetWordLen( pCursorPos, ',' );
			if ( size > 0 )
			{
				memcpy( buffer, pCursorPos, size );
				buffer[size] = 0;
				mV[1] = (float)atof(buffer);
			}
			pCursorPos = pCursorPos + (size + 1);

			if ( size > 0 )
			{
				strcpy( buffer, pCursorPos );
				mV[2] = (float)atof(buffer);
			}
		}
	}

	k_Vector3& operator =( const k_String& strOriVector3 )
	{
		this->InitFromString( strOriVector3.GetBuffer() );
		return *this;
	}

	k_Vector3& operator =( char* strOriVector3 )
	{
		this->InitFromString( strOriVector3 );
		return *this;
	}

	k_Vector3& operator =( const k_Vector3& vec )
	{
		if ( this != &vec )
		{
			mV[0] = vec.mV[0];
			mV[1] = vec.mV[1];
			mV[2] = vec.mV[2];
		}

		return *this;
	}


	float mV[3];
};

inline 
k_Vector3::k_Vector3( const float x, const float y, const float z )
{
	mV[0] = x;
	mV[1] = y;
	mV[2] = z;
}

inline
int
GetWordLen( char* pBuffer, char token )
{
	int iSize = 0;
	while( *pBuffer != token && *pBuffer != '\n' )
	{
		iSize++;
		pBuffer++;
	}
	return iSize;
}

//class ÉùÒô
//{
//public:
//	ÉùÒô()
//	{
//		MessageBox( 0, "ÉùÒô","ÉùÒô", 0 );
//	}
//
//	void µ¯³öÒ»¸ö´°¿Ú()
//	{
//		MessageBox( 0, "ÉùÒô","ÉùÒô", 0 );
//	}
//
//protected:
//private:
//};