

#pragma once

#include "NpcEntity.h"
#include "BrushEntity.h"

class CBrushLine : public CExcelLine
{
public:
	CBrushLine() : mpBrushEntity(NULL)
	{
		PushParam( &miIndex );
		PushParam( &mstrName );
		PushParam( &mstrMesh );
		PushParam( &mstrMtl );
	}

	~CBrushLine()
	{

	}

	int miIndex;
	k_String mstrName;
	k_String mstrMesh;
	k_String mstrMtl;

	CBrushEntity* mpBrushEntity;
};

class CEntityManager : public Singleton<CEntityManager>
{
public:
	CEntityManager();
	~CEntityManager();

	void Create();
	void Destroy();
	void Update();

	static CEntityManager& getSingleton(void);
	static CEntityManager* getSingletonPtr(void);

	CBrushEntity* CreateBrush( const String& entityName, const String& meshName, const String& mtlName );
	void DestroyBrush( const String& entityName );

	void AddTree(Entity *entity, const Vector3 &position, Degree yaw = Degree(0), Real scale = 1.0f, void* userData = NULL);
	CExcelTable<CBrushLine>* GetBrushTable()
	{
		return &mBrushTable;
	}

	void UpdatePageManager()
	{
		if (mHeightUpdateCountDown == 0)
			mHeightUpdateCountDown = mPageUpdateRate;
	}

	String GetUniqueEntityName( const String& base );

protected:
	std::vector<CNpc*> mNpcList;
	std::vector<CBrushEntity*> mBrushList;

	CExcelTable<CBrushLine> mBrushTable;

	int mHeightUpdateCountDown;
	unsigned long mPageUpdateRate;
	unsigned long mTimeLastFrame;
};