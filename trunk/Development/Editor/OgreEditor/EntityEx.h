

#pragma once

using namespace Ogre;

enum E_ENTITY_TYPE
{
	E_NULL_ENTITY_TYPE = 0,
	E_BRUSH_ENTITY_TYPE,
	E_NPC_ENTITY_TYPE,
	E_ENTITY_TYPE_NUM
};

class EntityEx : public UserObjectBindings
{
public:
	EntityEx();
	~EntityEx();

	E_ENTITY_TYPE GetEntityType() const;
	void SetEntityType( E_ENTITY_TYPE type );

	void Create( const String& entityName, const String& meshName, const String& mtlName );
	void Destroy();
	void setPosition( const Vector3& pos );
	Entity* GetEntity() const;
	SceneNode* GetSceneNode() const;
	String GetName();
	void SetVisible(bool bShow);
protected:

	E_ENTITY_TYPE m_eEntityType;
	SceneNode* mpSceneNode;
	Entity* mpEntity;
	String msEntityName;
	SceneManager* mpSceneMgr;
	bool mbVisible;

	//////////////////////////////////////////////////////////////////////////
	SceneNode* mpTipNode;
	BillboardSet* mpTipBoard;
	String msBillboardName;
};