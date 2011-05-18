

#pragma once

using namespace Ogre;

class CDecalEntity
{
public:
	CDecalEntity();
	~CDecalEntity();

	void Create( const String& decalName, const String& mtlName, int size = 4 );
	void Destroy();
	void setPosition( Real x, Real z, Real offset = 0.05f );

	void SetScale( Real scale );
protected:
	SceneManager* mpSceneMgr;
	ManualObject* mDecal;
	Real mScale;
	int mSize;
};