

#include "stdafx.h"
#include "Editor.h"
#include "EntityEx.h"


EntityEx::EntityEx() 
	: m_eEntityType(E_NULL_ENTITY_TYPE),
	mpSceneNode(NULL),
	mpEntity(NULL),
	mpSceneMgr(NULL),
	mbVisible(false),
	//////////////////////////////////////////////////////////////////////////
	mpTipNode(NULL),
	mpTipBoard(NULL)
{
	msEntityName = "";
	msBillboardName	= "";
}

EntityEx::~EntityEx()
{

}


E_ENTITY_TYPE EntityEx::GetEntityType() const
{
	return m_eEntityType;
}

void EntityEx::SetEntityType( E_ENTITY_TYPE type )
{
	m_eEntityType = type;
}


void EntityEx::Create( const String& entityName, const String& meshName, const String& mtlName )
{
	mpSceneMgr = GetEditor()->GetSceneManager();
	// create main model
	msEntityName = entityName;
	mpSceneNode = mpSceneMgr->getRootSceneNode()->createChildSceneNode();
	mpEntity = mpSceneMgr->createEntity( msEntityName, meshName );
	mpEntity->setUserAny( Ogre::Any(this) );
	mbVisible = false;

	mpTipNode = static_cast<SceneNode*>(mpSceneNode->createChild());
	msBillboardName = entityName + "bbinfo";
	mpTipBoard = mpSceneMgr->createBillboardSet(msBillboardName);
	Billboard* pTip = mpTipBoard->createBillboard(Vector3(0, 50, 0));
	pTip->setDimensions( 20.0f, 20.0f );

	if ( mtlName != "NULL" )
	{
		mpEntity->setMaterialName(mtlName);
		mpTipBoard->setMaterialName(mtlName);
	}
}

void EntityEx::SetVisible(bool bShow)
{
	if ( !mpSceneNode || bShow == mbVisible )
		return;

	if ( bShow )
	{
		mpSceneNode->attachObject(mpEntity);
		mpTipNode->attachObject(mpTipBoard);

	}
	else
	{
		mpSceneNode->detachObject(mpEntity);
		mpTipNode->detachObject(mpTipBoard);

	}
	mbVisible = bShow;
}

void EntityEx::Destroy()
{
	mpTipNode->detachObject(mpTipBoard);
	mpSceneMgr->destroyBillboardSet( mpTipBoard );
	mpSceneMgr->destroySceneNode( mpTipNode );

	mpSceneNode->detachObject(mpEntity);
	mpSceneMgr->destroyEntity( mpEntity );
	mpSceneMgr->destroySceneNode( mpSceneNode );

	mpSceneNode = NULL;
	mpTipNode = NULL;
}

void EntityEx::setPosition( const Vector3& pos )
{
	mpSceneNode->setPosition(pos);
}

String EntityEx::GetName()
{
	return msEntityName;
}

Entity* EntityEx::GetEntity() const
{
	return mpEntity;
}

SceneNode* EntityEx::GetSceneNode() const
{
	return mpSceneNode;
}