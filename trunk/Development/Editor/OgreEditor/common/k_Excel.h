

/*
简单的表格Excel结构
格式要求字段用 空格 分开
第一行为标题 之后内容为正文
2008 8 16
*/

#pragma once

//表格元素的基本数据类型
enum E_Excel_Node_Type
{
	eInt,
	eString,
	eFloat,
	eVector,
	eChar,
	eMax_Excel_Node_Type,
};

//数据表格的起始行
#define THE_FIRST_LINE 1

struct sExcelNode
{
	void* mpNode;
	E_Excel_Node_Type meType;
};

class CExcelLine
{
public:
	CExcelLine()
	{
	}

	~CExcelLine()
	{
	}

	int GetExcelNodeNum()
	{
		return mDataList.GetLength();
	}
	void PushParam( k_String* pData );
	void PushParam( char* pData );
	void PushParam( int* pData );
	void PushParam( float* pData );

	k_List<sExcelNode> mDataList;

};

inline
void CExcelLine::PushParam( int* pData )
{
	*pData = 0;
	k_Node<sExcelNode>* pNode = mDataList.AddNode();
	pNode->mData.mpNode = pData;
	pNode->mData.meType = eInt;
}

inline
void CExcelLine::PushParam( float* pData )
{
	*pData = 0.0f;
	k_Node<sExcelNode>* pNode = mDataList.AddNode();
	pNode->mData.mpNode = pData;
	pNode->mData.meType = eFloat;
}

inline
void CExcelLine::PushParam( char* pData )
{
	k_Node<sExcelNode>* pNode = mDataList.AddNode();
	pNode->mData.mpNode = pData;
	pNode->mData.meType = eChar;
}

inline
void CExcelLine::PushParam( k_String* pData )
{
	k_Node<sExcelNode>* pNode = mDataList.AddNode();
	pNode->mData.mpNode = pData;
	pNode->mData.meType = eString;
}

template<class T>
class CExcelTable
{
public:
	CExcelTable() : mLineNum( 0 ), mpExcelData(NULL)
	{

	}

	~CExcelTable()
	{
		if ( mpExcelData )
		{
			delete[] mpExcelData;
		}
	}

	//解析Excel文件
	bool LoadExcel( const char* pFileName );

	int GetFileLineNum()
	{
		return mLineNum;
	}

	int CalFileLineNum( char* pBuffer );

	//取出Excel表格行的指针
	T* GetExcelLine( int num );

protected:
	void SetExcelLine( T* pTable )
	{
		mpExcelData = pTable;
	}

	char* parseExcelLine( char* pBuffer, int line );
	char* parseExcelTitle( char* pBuffer );
	//创建一个新的字符串
	int GetWordLen( char* pBuffer, char token );

	CExcelLine mExcelTitle;
	T* mpExcelData;

	int mLineNum;
};

//取出Excel表格行的指针
template<class T>
T* CExcelTable<T>::GetExcelLine( int num )
{
	if ( num < mLineNum )
	{
		return &mpExcelData[num - 1];
	}

	return NULL;
}

template<class T>
bool CExcelTable<T>::LoadExcel( const char* pFileName )
{
	FILE* fp = fopen(pFileName, "rb");
	if (!fp)
		return false;


	fseek(fp,0L, SEEK_END );
	int iFileSize = ftell( fp );
	fseek( fp,0, SEEK_SET );

	char* pBuffer = new char[iFileSize + 1];
	fread(pBuffer, iFileSize, 1, fp);
	pBuffer[iFileSize] = '\0';

	fclose(fp);
	//---------------------------------------------------

	mLineNum = CalFileLineNum( pBuffer );

	SetExcelLine( new T[mLineNum - 1 ] );

	char* pCursorPos = pBuffer;

	pCursorPos = parseExcelTitle( pCursorPos );

	int ilinePos = 1;

	while ( ilinePos < mLineNum )
	{
		//解析表格的数据行
		pCursorPos = parseExcelLine( pCursorPos, ilinePos );
		ilinePos++;
	}



	//---------------------------------------------------
	if( pBuffer )
	{
		delete[] pBuffer;
	}

	return true;
}

template<class T>
int CExcelTable<T>::CalFileLineNum( char* pBuffer )
{
	char* tmp = pBuffer;
	int iLineNum = 0;
	bool bBadLine = true;

	while ( *tmp != '\0' )
	{
		if ( *tmp == '\n' )
		{
			if ( !bBadLine )
			{
				iLineNum++;
				bBadLine = true;
			}
		}
		else
		{
			if ( bBadLine &&( *tmp != ' ' && *tmp != '\n' && *tmp != 13 ) )
			{
				bBadLine = false;
			}
		}

		tmp++;
	}
	if ( !bBadLine )
		iLineNum++;

	return iLineNum;
}

static char* pText = new char[512];

//解析每行的表格数据
template<class T>
char* CExcelTable<T>::parseExcelLine( char* pBuffer, int line )
{
	char* titlePos = pBuffer;

	int iCursorPos = GetWordLen( titlePos, '\t' );
	CExcelLine* pData = (CExcelLine*)(mpExcelData+(line - 1));
	k_Node<sExcelNode>* pNode = pData->mDataList.First();

	while ( iCursorPos != 0 )
	{
		if ( iCursorPos > 511 )
		{
			delete[] pText;
			pText = new char[iCursorPos + 1];
		}

		memcpy( pText, titlePos, iCursorPos );
		pText[iCursorPos] = 0;
		if ( pText[iCursorPos - 1] == 13 )
		{
			pText[iCursorPos - 1] = 0;
		}

		switch( pNode->mData.meType )
		{
		case eInt:
			{
				int* pValue = (int*)pNode->mData.mpNode;
				*pValue = atoi(pText);
			}
			break;
		case eString:
			{
				k_String* pValue = (k_String*)pNode->mData.mpNode;
				*pValue = pText;
			}
			break;
		case eFloat:
			{
				float* pValue = (float*)pNode->mData.mpNode;
				*pValue = (float)atof(pText);
			}
			break;
		default:
			break;
		}

		if ( line == mLineNum )
		{
			if ( *(titlePos + iCursorPos) == '\0' )
			{
				titlePos += iCursorPos + 1;
				break;
			}
		}


		if ( *(titlePos + iCursorPos) == '\n' )
		{
			titlePos += iCursorPos + 1;
			break;
		}

		titlePos += iCursorPos + 1;

		pNode = pNode->mpNext;
		if ( !pNode )
		{
			iCursorPos = GetWordLen( titlePos, '\n' );
			titlePos += iCursorPos + 1;
			break;
		}

		iCursorPos = GetWordLen( titlePos, '\t' );
	}

	return titlePos;
}

template<class T>
char* CExcelTable<T>::parseExcelTitle( char* pBuffer )
{
	char* titlePos = pBuffer;

	int iCursorPos = GetWordLen( titlePos, '\t' );

	while ( iCursorPos != 0 )
	{
		if ( iCursorPos > 511 )
		{
			delete[] pText;
			pText = new char[iCursorPos + 1];
		}

		memcpy( pText, titlePos, iCursorPos );
		pText[iCursorPos] = 0;
		mExcelTitle.PushParam( pText );

		if ( *(titlePos + iCursorPos) == '\n' )
		{
			titlePos += iCursorPos + 1;
			break;
		}
		titlePos += iCursorPos + 1;

		iCursorPos = GetWordLen( titlePos, '\t' );
	}

	return titlePos;
}

//创建一个新的字符串
template<class T>
int CExcelTable<T>::GetWordLen( char* pBuffer, char token )
{
	int iSize = 0;
	while( *pBuffer != token && *pBuffer != '\n' )
	{
		iSize++;
		pBuffer++;
	}
	return iSize;
}