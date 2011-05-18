

#pragma once

#include "GuiManager.h"
#include "OgreTerrain.h"
#include "OgreTerrainGroup.h"
#include "OgreTerrainQuadTreeNode.h"
#include "OgreTerrainMaterialGeneratorA.h"
#include "OgreTerrainPaging.h"
#include "Controls/RollupCtrl.h"
#include "EditTool.h"
#include "DatabaseDialog.h"
#include "ParticleDialog.h"
#include "StatusDialog.h"
#include "smartptr.h"
#include "EntityManager.h"
#include "PhysicsManager.h"

enum E_ROLLUP_CTRLS
{
	E_OBJECT_CTRL,
	E_TERRAIN_CTRL,
	E_DISPLAY_CTRL,
	E_LAYER_CTRL,
	E_ROLLUP_CTRL_NUM
};

/** Possible types of editor tools enumeration */
enum EDITORTOOLS 
{
	TOOL_SELECT = 0,    /** Selection mode, allowing for selection of object(s) */
	TOOL_MOVE,          /** Displacement mode, allowing for moving object(s) */
	TOOL_ROTATE,        /** Rotation mode, allowing for changing object(s) orientation */
	TOOL_SCALE         /** Scale mode, allowing for scaling of object(s) */
};

enum E_NOTIFY_EVENTS
{
	NE_EDITTOOL_CHANGED,
	NE_TOOLMODE_CHANGED
};

#define TERRAIN_FILE_PREFIX String("ogreTerrain")
#define TERRAIN_FILE_SUFFIX String("dat")
#define TERRAIN_WORLD_SIZE 12000.0f
#define TERRAIN_SIZE 513

class IEditorEventListener
{
public:
	virtual void Notify( E_NOTIFY_EVENTS event, UINT nflag = 0 ) = 0;
};

class CEditor:public Ogre::FrameListener,
	public Ogre::WindowEventListener,
	public OgreBites::GuiManagerListener
{
public:
	CEditor();
	~CEditor();

	void Create( HWND hWnd,int width,int height );
	void locateResources();
	void loadResources();
	void PreLoadResources();
	//void SetupInput();

	void UpDate();
	void Destroy();
	void Resize(int width,int height);
	void CreateHydrax();

	void CreateLevel();
	BOOL Load(LPCTSTR lpszPathName);
	void SaveLevel();

	void DefineTerrain(long x, long y, bool flat = false);
	void GetTerrainImage(bool flipX, bool flipY, Ogre::Image& img);

	/*-----------------------------------------------------------------------------
	| Processes frame started events.
	-----------------------------------------------------------------------------*/
	virtual bool frameStarted(const Ogre::FrameEvent& evt)
	{
		// manually call sample callback to ensure correct order
		return true;
	}

	/*-----------------------------------------------------------------------------
	| Processes rendering queued events.
	-----------------------------------------------------------------------------*/
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

	/*-----------------------------------------------------------------------------
	| Processes frame ended events.
	-----------------------------------------------------------------------------*/
	virtual bool frameEnded(const Ogre::FrameEvent& evt)
	{
		// quit if window was closed
		if (mRenderWindow->isClosed()) 
			return false;

		return true;
	}

	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnRButtonDown(UINT nFlags, CPoint point);
	void OnRButtonUp(UINT nFlags, CPoint point);
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	Ogre::SceneManager* GetSceneManager()
	{
		return mSceneManager;
	}

	Ogre::RenderWindow* GetRenderWindow()
	{
		return mRenderWindow;
	}

	Ogre::Camera* GetCamera()
	{
		return mCamera;
	}

	Ogre::Viewport* GetViewport()
	{
		return mViewport;
	}

	Ogre::TerrainGroup* GetTerrainGroup()
	{
		return mTerrainGroup;
	}

	Ogre::TerrainGlobalOptions* GetTerrainGlobalOptions()
	{
		return mTerrainGlobals;
	}

	bool IsLevelLoaded()
	{
		return mHaveLevel;
	}

	Ogre::Ray getCameraToViewportRay( CPoint& point )
	{
		Ogre::Ray ray;
		Ogre::Vector2 fPt = Ogre::Vector2(point.x, point.y);
		ray = mCamera->getCameraToViewportRay(fPt.x/mRenderWindow->getWidth(), fPt.y/mRenderWindow->getHeight());
		return ray;
	}

	Ogre::TerrainGroup::RayResult TerrainHitTest( CPoint& point )
	{
		Ogre::Ray ray;
		Ogre::Vector2 fPt = Ogre::Vector2(point.x, point.y);
		ray = mCamera->getCameraToViewportRay(fPt.x/mRenderWindow->getWidth(), fPt.y/mRenderWindow->getHeight());
		return mTerrainGroup->rayIntersects(ray);
	}

	void SetToolMode( EDITORTOOLS toolMode )
	{
		mToolMode = toolMode;

		Notify( NE_TOOLMODE_CHANGED );
	}

	EDITORTOOLS GetToolMode()
	{
		return mToolMode;
	}

	void RegisterEditorListener( IEditorEventListener* listener );
	void unRegisterEditorListener( IEditorEventListener* listener );

	void SetEditTool( CEditTool* pTool );
	CEditTool* GetCurrentEditTool();
	void SetStatusText( CString& test );
	void Notify( E_NOTIFY_EVENTS event, UINT nflag = 0 );

	void RestoreCurrentDirectory();

	void ShowDatabaseDialog( bool bShow );
	void DestroyDatabaseDialog()
	{
		if ( mpDatabaseDialog )
		{
			mpDatabaseDialog = NULL;
		}
	}

	void ShowParticleDialog( bool bShow );
	void DestroyParticleDialog()
	{
		if ( mpParticleDialog )
		{
			mpParticleDialog = NULL;
		}
	}


	void SetRollupCtrl( E_ROLLUP_CTRLS id, CRollupCtrl* pCtrl )
	{
		m_pRollupCtrl[id] = pCtrl;
	}

	CRollupCtrl* GetRollupCtrl( E_ROLLUP_CTRLS id )
	{
		return m_pRollupCtrl[id];
	}

	void SetStatusDialog( CStatusDialog* pDialog )
	{
		mpStatusDialog = pDialog;
	}

	CDialog* GetStatusDialog() const
	{
		return mpStatusDialog;
	}

	void SetEditPos( Ogre::Vector3& pos )
	{
		return mpStatusDialog->SetEditPos(pos);
	}

	void SetLogWindow(CEdit* pEdit);
	void Log( CString& log );

	void SetTopSpeed( Ogre::Real speed )
	{
		mTopSpeed = speed;
	}

protected:
	Ogre::Root* mRoot;
	Ogre::RenderWindow* mRenderWindow;
	Ogre::SceneManager* mSceneManager;
	Ogre::SceneNode* mBaseSceneNode;
	Ogre::SceneNode* mStaticSceneNode;
	Ogre::Camera* mCamera;
	Ogre::Viewport* mViewport;
	OgreBites::GuiManager* mGuiMgr;

	CPoint mRButtonDownPos;
	HWND mhWnd;

	CEntityManager* mpEntityMgr;
	CPhysicsManager* mpPhysicsMgr;

	Ogre::TerrainGlobalOptions* mTerrainGlobals;
	Ogre::TerrainGroup* mTerrainGroup;
	Ogre::Light* mTerrainLight;
	Ogre::Vector3 mTerrainPos;
	bool mTerrainsImported;

	Ogre::Real mTopSpeed;
	Ogre::Vector3 mVelocity;
	bool mGoingForward;
	bool mGoingBack;
	bool mGoingLeft;
	bool mGoingRight;
	bool mGoingUp;
	bool mGoingDown;
	bool mHaveLevel;

	_smart_ptr<CEditTool> mpEditTool;

	EDITORTOOLS mToolMode;

	std::list<IEditorEventListener*> mEditorListenerList;
	
	char mCurDir[MAX_PATH];

	CDatabaseDialog* mpDatabaseDialog;
	CParticleDialog* mpParticleDialog;
	CStatusDialog* mpStatusDialog;

	CRollupCtrl* m_pRollupCtrl[E_ROLLUP_CTRL_NUM];
	CString mLogTest;
	CEdit* mpLogEdit;
};

CEditor* GetEditor();
