

#include "stdafx.h"
#include "Editor.h"
#include "EntityManager.h"

//-----------------------------------------------------------------------
template<> CEntityManager* Singleton<CEntityManager>::ms_Singleton = 0;
CEntityManager* CEntityManager::getSingletonPtr(void)
{
	return ms_Singleton;
}
CEntityManager& CEntityManager::getSingleton(void)
{  
	assert( ms_Singleton );  return ( *ms_Singleton );  
}

CEntityManager::CEntityManager() 
	: mHeightUpdateCountDown(0),mTimeLastFrame(0)
{
	// Update terrain at max 20fps
	mPageUpdateRate = 100;
	mTimeLastFrame = Ogre::Root::getSingletonPtr()->getTimer()->getMilliseconds();
}

CEntityManager::~CEntityManager()
{

}

void CEntityManager::Create()
{
	bool bLoad = mBrushTable.LoadExcel( "..//Editor//brushtable.txt" );
	if ( bLoad )
	{
		for ( int i = 1; i < mBrushTable.GetFileLineNum(); i++ )
		{
			CBrushLine* pBrush = mBrushTable.GetExcelLine(i);
			pBrush->mpBrushEntity = CreateBrush(pBrush->mstrName.GetBuffer(),pBrush->mstrMesh.GetBuffer(),pBrush->mstrMtl.GetBuffer());
		}
	}

	//-------------------------------------- LOAD TREES --------------------------------------
	//Create and configure a new PagedGeometry instance
	//mpPageManager = new PagedGeometry();
	//mpPageManager->setCamera(GetEditor()->GetCamera());	//Set the camera so PagedGeometry knows how to calculate LODs
	//mpPageManager->setPageSize(80);	//Set the size of each page of geometry
	//mpPageManager->setInfinite();		//Use infinite paging mode
	//mpPageManager->addDetailLevel<BatchPage>(2500, 50);		//Use batches up to 150 units away, and fade for 30 more units
	//mpPageManager->addDetailLevel<ImpostorPage>(5000, 50);

	////Create a new TreeLoader3D object
	//mpTreeLoader = new TreeLoader3D(mpPageManager, TBounds(0, 0, TERRAIN_WORLD_SIZE, TERRAIN_WORLD_SIZE));
	//mpPageManager->setPageLoader(mpTreeLoader);	//Assign the "treeLoader" to be used to load geometry for the PagedGeometry instance
}

void CEntityManager::Destroy()
{
	//if ( mpPageManager )
	//{
	//	//Delete the TreeLoader3D instance
	//	delete mpPageManager->getPageLoader();

	//	//Delete the PagedGeometry instance
	//	delete mpPageManager;
	//}


	for ( int i = 1; i < mBrushTable.GetFileLineNum(); i++ )
	{
		CBrushLine* pBrush = mBrushTable.GetExcelLine(i);
		CEntityManager::getSingletonPtr()->DestroyBrush( pBrush->mpBrushEntity->GetName() );
	}
}

void CEntityManager::Update()
{
	//[NOTE] PagedGeometry::update() is called every frame to keep LODs, etc. up-to-date
	//unsigned long newTime = Ogre::Root::getSingletonPtr()->getTimer()->getMilliseconds();

	//unsigned long timeSinceLastFrame = newTime - mTimeLastFrame;
	//if (mHeightUpdateCountDown > 0)
	//{
	//	mHeightUpdateCountDown -= timeSinceLastFrame;
	//	if (mHeightUpdateCountDown <= 0)
	//	{
	//		if( mpPageManager )
	//			mpPageManager->update();
	//		mHeightUpdateCountDown = 0;
	//	}
	//}
	//mTimeLastFrame = newTime;
}

CBrushEntity* CEntityManager::CreateBrush( const String& entityName, const String& meshName, const String& mtlName )
{
	CBrushEntity* pBrush = new CBrushEntity;
	String newName = GetUniqueEntityName( entityName );
	pBrush->Create( newName, meshName, mtlName );
	mBrushList.push_back(pBrush);
	return pBrush;
}

void CEntityManager::DestroyBrush( const String& entityName )
{
	std::vector<CBrushEntity*>::iterator it = mBrushList.begin();
	for ( ; it != mBrushList.end(); it++ )
	{
		CBrushEntity* pBrush = *it;
		if ( pBrush->GetName() == entityName )
		{
			pBrush->Destroy();
			delete pBrush;
			mBrushList.erase(it);
			break;;
		}
	}
}

void CEntityManager::AddTree( Entity *entity, const Vector3 &position, Degree yaw, Real scale, void* userData )
{
	//mpTreeLoader->addTree(entity, position, yaw, scale);
	//mpPageManager->update();
}

String CEntityManager::GetUniqueEntityName( const String& base )
{
	int i = 0;
	String name = base;
	while(GetEditor()->GetSceneManager()->hasEntity(name))
	{
		name = base+StringConverter::toString(++i);
	}
	return name;
}
