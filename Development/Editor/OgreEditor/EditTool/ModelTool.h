

#pragma once

class CModelToolRenderTargetListener : public RenderTargetListener
{
public:
	CModelToolRenderTargetListener();
	void preRenderTargetUpdate(const RenderTargetEvent& evt);
	void postRenderTargetUpdate(const RenderTargetEvent& evt);

	SceneNode* mpSkyBoxNode;
};

class CModelTool : public CEditTool
{
public:
	DECLARE_DYNCREATE(CModelTool)

	CModelTool();
	~CModelTool();

	void BeginTool();
	void EndTool();

	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	void FromName( const CString& fileName )
	{
		CString name = fileName;
		LoadModel(name);
	}

	bool LoadModel( CString& modelFileName );

	void SetParent( CWnd* pWnd )
	{
		mpParent = pWnd;
	}

	CWnd* mpParent;
	Ogre::RenderWindow* mRenderWindow;
	Ogre::Camera* mCamera;
	Ogre::Viewport* mViewport;
	Ogre::SceneManager* mpSceneMgr;
	HWND mhWnd;

	Ogre::SceneNode* mpModelNode;
	Ogre::Entity* mpEntity;
	Ogre::Light* mpLight;
	Ogre::String mModelName;

	CModelToolRenderTargetListener *mpModelToolRTListner;
};