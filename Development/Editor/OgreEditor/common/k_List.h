


/*
双向链表及节点结构
2008 8 16
*/

#pragma once

template<class T>
class k_Node
{
public:
	k_Node(): mpPrev(NULL), mpNext(NULL)
	{

	}
	~k_Node()
	{

	}

	void InsertAfter( k_Node<T>* prevNode );

	//使节点离开链表 注 头节点 和 尾节点要特殊处理
	void ExitList();

	k_Node<T>& operator =( const T& data )
	{
		if ( &mData != &data )
		{
			mData = data
		}

		return *this;
	}

	T mData;
	k_Node<T>* mpPrev;
	k_Node<T>* mpNext;
};

//在指定节点后插入
template<class T>
void k_Node<T>::InsertAfter( k_Node<T>* prevNode )
{
	this->mpNext = prevNode->mpNext;
	prevNode->mpNext = this;
	this->mpPrev = prevNode;
}

//使节点离开链表 注 头节点 和 尾节点要特殊处理
template<class T>
void k_Node<T>::ExitList()
{
	if( this->mpPrev )
	{
		this->mpPrev->mpNext = this->mpNext;
	}

	if( this->mpNext )
	{
		this->mpNext->mpPrev = this->mpPrev;
	}

	this->mpPrev = NULL;
	this->mpNext = NULL;
}

template<class T>
class k_List
{
public:
	k_List():mpFirst(NULL),mpLast(NULL),miLength(0)
	{

	}

	~k_List()
	{
		Clear();
	}

	//增加节点 分配内存
	k_Node<T>* AddNode( );

	//返回指定节点的指针
	k_Node<T>* GetNode( T& otherNode )
	{
		k_Node<T>* pNode = First();
		while( pNode )
		{
			if ( pNode->mData == otherNode )
			{
				return pNode;
			}
			pNode = pNode->mpNext;
		}

		return NULL;
	}


	//在指定节点后插入节点
	k_Node<T>* InsertAfter( k_Node<T>* prevNode, k_Node<T>* pNode );

	//删除节点 收回内存
	void DeleteNode( k_Node<T>* pDeleteNode );

	//插入节点
	k_Node<T>* PushBack( k_Node<T>* pNode );

	//插入节点 分配内存
	k_Node<T>* PushBack( T& data );

	//取出头节点 注 需要手动释放内存
	k_Node<T>* PopFirst();

	//取出尾节点 注 需要手动释放内存
	k_Node<T>* PopLast();

	//返回头节点的指针
	k_Node<T>* First() const
	{
		return mpFirst;
	}

	//清空所有节点 同时释放资源
	void Clear();

	//返回链表长度
	int GetLength()
	{
		return miLength;
	}

protected:

	int miLength;
	k_Node<T>* mpFirst;
	k_Node<T>* mpLast;
};

template<class T>
k_Node<T>* k_List<T>::AddNode( )
{
	k_Node<T>* pNode = new k_Node<T>;
	this->PushBack( pNode );
	return pNode;
}

template<class T>
k_Node<T>* k_List<T>::InsertAfter( k_Node<T>* prevNode, k_Node<T>* pNode )
{
	if ( prevNode == mpLast )
	{
		this->PushBack( pNode );
	}
	else
	{
		pNode->InsertAfter( prevNode );
	}
}

template<class T>
void k_List<T>::DeleteNode( k_Node<T>* pDeleteNode )
{
	k_Node<T>* pNode = NULL;

	if ( pDeleteNode == mpFirst )
	{
		pNode = PopFirst();
		delete pNode;
		return;
	}

	if ( pDeleteNode == mpLast )
	{
		pNode = PopLast();
		delete pNode;
		return;
	}


	if ( GetLength() > 3 )
	{
		pNode = mpFirst->mpNext;

		while ( pNode != mpLast )
		{
			if ( pNode == pDeleteNode )
			{
				pNode->ExitList();
				delete pNode;
				miLength--;
				return;
			}

			pNode = pNode->mpNext;
		}
	}
}

template<class T>
k_Node<T>* k_List<T>::PushBack( k_Node<T>* pNode )
{
	if ( !pNode )
		return NULL;

	if ( !mpLast )
	{
		mpFirst = pNode;
		mpLast = pNode;
	}
	else
	{
		pNode->InsertAfter( mpLast );
		mpLast = pNode;
	}

	miLength++;

	return pNode;
}

template<class T>
k_Node<T>* k_List<T>::PushBack( T& data )
{
	k_Node<T>* pNode = new k_Node<T>;
	pNode->mData = data;
	PushBack( pNode );

	return pNode;
}


template<class T>
k_Node<T>* k_List<T>::PopFirst()
{
	if( !mpFirst )
		return NULL;

	k_Node<T>* pNode = mpFirst;
	k_Node<T>* pNodeNext = mpFirst->mpNext;
	mpFirst->ExitList();
	mpFirst = pNodeNext;

	if ( !pNodeNext )
		mpLast = NULL;
	miLength--;
	return pNode;
}

template<class T>
k_Node<T>* k_List<T>::PopLast()
{
	if( !mpFirst )
		return NULL;

	k_Node<T>* pNode = mpLast;
	k_Node<T>* pNodePrev = mpLast->mpPrev;
	pNode->ExitList();
	mpLast = pNodePrev;
	if ( !pNodePrev )
		mpFirst = NULL;

	miLength--;
	return pNode;
}

template<class T>
void k_List<T>::Clear()
{
	k_Node<T>* pNode = PopFirst();

	while ( pNode )
	{
		delete pNode;
		pNode = PopFirst();
	}
}