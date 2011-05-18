

#include "stdafx.h"
#include "Editor.h"
#include "DecalEntity.h"

CDecalEntity::CDecalEntity() : mDecal(NULL), mScale(1.0f), mSize(4)
{
	mpSceneMgr = GetEditor()->GetSceneManager();
}

CDecalEntity::~CDecalEntity()
{

}

void CDecalEntity::Create( const String& decalName, const String& mtlName, int size )
{
	mDecal = new Ogre::ManualObject(decalName);
	mpSceneMgr->getRootSceneNode()->attachObject(mDecal);

	mSize = size;
	int x_size = mSize;  // number of polygons
	int z_size = mSize;

	mDecal->begin(mtlName, Ogre::RenderOperation::OT_TRIANGLE_LIST);
	for (int i=0; i<=x_size; i++)
	{
		for (int j=0; j<=z_size; j++)
		{
			mDecal->position(Ogre::Vector3(i, 0, j));
			mDecal->textureCoord((float)i / (float)x_size, (float)j / (float)z_size);
		}
	}

	for (int i=0; i<x_size; i++)
	{
		for (int j=0; j<z_size; j++)
		{
			mDecal->quad( i * (x_size+1) + j,
				i * (x_size+1) + j + 1,
				(i + 1) * (x_size+1) + j + 1,
				(i + 1) * (x_size+1) + j);
		}
	}
	mDecal->end();
}

void CDecalEntity::Destroy()
{
	if ( mDecal )
	{
		mpSceneMgr->getRootSceneNode()->detachObject(mDecal);
		delete mDecal;
		mDecal = NULL;
	}
}

void CDecalEntity::setPosition( Real x, Real z, Real offset )
{
	Ogre::Real x1 = x - mScale;
	Ogre::Real z1 = z - mScale;

	int x_size = mSize;  // number of polygons
	int z_size = mSize;

	Ogre::Real x_step = mScale * 2/ x_size;
	Ogre::Real z_step = mScale * 2/ z_size;

	mDecal->beginUpdate(0);
	// redefine vertices
	for (int i=0; i<=x_size; i++)
	{
		for (int j=0; j<=z_size; j++)
		{
			Real height = GetEditor()->GetTerrainGroup()->getHeightAtWorldPosition( x1, 0, z1 );
			mDecal->position(Ogre::Vector3(x1, height + offset, z1));
			mDecal->textureCoord((float)i / (float)x_size, (float)j / (float)z_size);
			z1 += z_step;
		}
		x1 += x_step;
		z1 = z - mScale;
	}
	// redefine quads
	for (int i=0; i<x_size; i++)
	{
		for (int j=0; j<z_size; j++)
		{
			mDecal->quad( i * (x_size+1) + j,
				i * (x_size+1) + j + 1,
				(i + 1) * (x_size+1) + j + 1,
				(i + 1) * (x_size+1) + j);
		}
	}
	mDecal->end();
}


void CDecalEntity::SetScale( Real scale )
{
	mScale = scale;
}
