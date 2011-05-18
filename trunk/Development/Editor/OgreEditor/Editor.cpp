

#include "stdafx.h"
#include "Editor.h"
#include "Ocean/Ocean.h"
#include "EditTool/ObjectSelectTool.h"

CEditor::CEditor(): mRoot(NULL)
				, mRenderWindow(NULL)
				, mSceneManager(NULL)
				, mBaseSceneNode(NULL)
				, mStaticSceneNode(NULL)
				, mCamera(NULL)
				, mViewport(NULL)
				, mGuiMgr(NULL)
				, mTerrainLight(NULL)
				, mTerrainPos(0.0f,0.0f,0.0f)
				, mTerrainGlobals(NULL)
				, mTerrainGroup(NULL)
				, mTerrainsImported(false)
				, mTopSpeed(180)
				, mVelocity(Ogre::Vector3::ZERO)
				, mGoingForward(false)
				, mGoingBack(false)
				, mGoingLeft(false)
				, mGoingRight(false)
				, mGoingUp(false)
				, mGoingDown(false)
				, mHaveLevel(false)
				, mpEditTool(NULL)
				, mpDatabaseDialog( NULL )
				, mpParticleDialog( NULL )
				, mpEntityMgr( NULL )
				, mToolMode( TOOL_SELECT )
				, mpStatusDialog(NULL)
				, mpLogEdit(NULL)
				, mRButtonDownPos(CPoint(0,0))
{
	for ( int i = 0; i < E_ROLLUP_CTRL_NUM; i++ )
	{
		m_pRollupCtrl[i] = NULL;
	}

	memset(mCurDir, 0, MAX_PATH);

	mLogTest = "";
}

CEditor::~CEditor()
{

}

void CEditor::Create( HWND hWnd,int width,int height )
{
	GetCurrentDirectory(MAX_PATH, mCurDir);

	mpEntityMgr = new CEntityManager;
	mpPhysicsMgr = new CPhysicsManager;

	Ogre::NameValuePairList parms;
	parms["externalWindowHandle"] = Ogre::StringConverter::toString((long)hWnd);
	mRenderWindow = mRoot->createRenderWindow("TerrainEditor", width, height, false, &parms);
	mhWnd = hWnd;
	mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC, "MainScene");

	// Create the camera
	mCamera = mSceneManager->createCamera("PlayerCam");

	mCamera->setPosition(mTerrainPos + Ogre::Vector3(200, 400, 0));
	mCamera->lookAt(Ogre::Vector3(0, 50, 0));
	mCamera->setNearClipDistance(1);
	mCamera->setFarClipDistance(12000);

	mViewport = mRenderWindow->addViewport(mCamera);
	mViewport->setBackgroundColour(Ogre::ColourValue(0.5f,0.5f,0.5f));

	//SetupInput();
	loadResources();

	mGuiMgr = new OgreBites::GuiManager("BrowserControls", mRenderWindow, this);
	//mGuiMgr->showBackdrop("SdkTrays/Bands");
	mGuiMgr->getTrayContainer(OgreBites::TL_NONE)->hide();

	Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
	Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(7);

	mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();

	Ogre::Light* l = mSceneManager->createLight("terrainLight");
	l->setType(Ogre::Light::LT_DIRECTIONAL);
	Ogre::Vector3 lightdir(0.1f, -0.3f, 0.1f);
	lightdir.normalise();
	l->setDirection(lightdir);
	l->setDiffuseColour(Ogre::ColourValue::White);
	l->setSpecularColour(Ogre::ColourValue(0.4f, 0.4f, 0.4f));
	mTerrainLight = l;
	mSceneManager->setAmbientLight(Ogre::ColourValue(0.2f, 0.2f, 0.2f));

	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	mRoot->addFrameListener(this);

	mGuiMgr->showFrameStats(OgreBites::TL_TOPRIGHT);

	mpEntityMgr->Create();
	mpPhysicsMgr->Create();

	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(
		Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));

	SetEditTool(NULL);
}

void CEditor::CreateHydrax()
{
	// Hydrax initialization code ---------------------------------------------
	// ------------------------------------------------------------------------

	//// Create Hydrax object
	//mHydrax = new Hydrax::Hydrax(mSceneManager, mCamera, mViewport);

	//// Create our projected grid module  
	//Hydrax::Module::ProjectedGrid *mModule 
	//	= new Hydrax::Module::ProjectedGrid(// Hydrax parent pointer
	//	mHydrax,
	//	// Noise module
	//	new Hydrax::Noise::Perlin(/*Generic one*/),
	//	// Base plane
	//	Ogre::Plane(Ogre::Vector3(0,1,0), Ogre::Vector3(0,0,0)),
	//	// Normal mode
	//	Hydrax::MaterialManager::NM_VERTEX,
	//	// Projected grid options
	//	Hydrax::Module::ProjectedGrid::Options(/*264 /*Generic one*/));

	//// Set our module
	//mHydrax->setModule(static_cast<Hydrax::Module::Module*>(mModule));

	//// Load all parameters from config file
	//// Remarks: The config file must be in Hydrax resource group.
	//// All parameters can be set/updated directly by code(Like previous versions),
	//// but due to the high number of customizable parameters, since 0.4 version, Hydrax allows save/load config files.
	//mHydrax->loadCfg("HydraxDemo.hdx");

	//// Create water
	//mHydrax->create();

	// Hydrax initialization code end -----------------------------------------
	// ------------------------------------------------------------------------
}


void CEditor::locateResources()
{
	// load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load( "resources_d.cfg" );

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	Ogre::String sec, type, arch;

	// go through all specified resource groups
	while (seci.hasMoreElements())
	{
		sec = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap* settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;

		// go through all resource paths
		for (i = settings->begin(); i != settings->end(); i++)
		{
			type = i->first;
			arch = i->second;

			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch, type, sec);
		}
	}
}

void CEditor::loadResources()
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void CEditor::PreLoadResources()
{
#ifdef _DEBUG
	mRoot = new Ogre::Root("plugins_d.cfg", "Ogre.cfg", "Ogre.log");
#else
	mRoot = new Ogre::Root("plugins.cfg", "Ogre.cfg", "Ogre.log");
#endif

	locateResources();

	mRoot->setRenderSystem(mRoot->getRenderSystemByName("Direct3D9 Rendering Subsystem"));

	mRoot->initialise(false, Ogre::StringUtil::BLANK);
}

void CEditor::UpDate()
{
	if ( !mpEditTool )
		return;

	mpEditTool->Update();

	mpEntityMgr->Update();

	mpPhysicsMgr->Update();

	mRoot->renderOneFrame();
}

void CEditor::Destroy()
{
	mGuiMgr->clearAllTrays();

	if (mGuiMgr)
	{
		delete mGuiMgr;
		mGuiMgr = NULL;
	}

	if ( mpEntityMgr )
	{
		mpEntityMgr->Destroy();
		delete mpEntityMgr;
		mpEntityMgr = NULL;
	}

	if ( mpPhysicsMgr )
	{
		mpPhysicsMgr->Destroy();
		delete mpPhysicsMgr;
		mpPhysicsMgr = NULL;
	}

	if (mRoot)
	{
		if ( mTerrainGroup )
		{
			OGRE_DELETE mTerrainGroup;
			OGRE_DELETE mTerrainGlobals;
		}

		if (mSceneManager)
		{
			mSceneManager->clearScene();
			mRoot->destroySceneManager(mSceneManager);
		}

		delete mRoot;
		mRoot = NULL;
	}
}

void CEditor::Resize(int width,int height)
{
	if (mRenderWindow) 
	{ 
		// notify "render window" instance 
		mRenderWindow->windowMovedOrResized(); 

		// Adjust camera's aspect ratio, too 
		if (height != 0 && mCamera != 0) 
			mCamera->setAspectRatio((Ogre::Real)mRenderWindow->getWidth() / (Ogre::Real)mRenderWindow->getHeight()); 

		mCamera->yaw(Ogre::Radian(0));
	} 
}

void CEditor::CreateLevel()
{
	if(!mTerrainLight)
		return;
	SetCursor(LoadCursor(NULL, IDC_WAIT));

	mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneManager, Ogre::Terrain::ALIGN_X_Z, TERRAIN_SIZE, TERRAIN_WORLD_SIZE);
	mTerrainGroup->setFilenameConvention(TERRAIN_FILE_PREFIX, TERRAIN_FILE_SUFFIX);
	mTerrainGroup->setOrigin(mTerrainPos);

	//matProfile->setLightmapEnabled(false);
	// Important to set these so that the terrain knows what to use for derived (non-realtime) data
	mTerrainGlobals->setLightMapDirection(mTerrainLight->getDerivedDirection());
	mTerrainGlobals->setCompositeMapAmbient(mSceneManager->getAmbientLight());
	//mTerrainGlobals->setCompositeMapAmbient(ColourValue::Red);
	mTerrainGlobals->setCompositeMapDiffuse(mTerrainLight->getDiffuseColour());

	//Configure default import settings for if we use imported image
	Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
	defaultimp.terrainSize = TERRAIN_SIZE;
	defaultimp.worldSize = TERRAIN_WORLD_SIZE;
	defaultimp.inputScale = 600;
	defaultimp.minBatchSize = 33;
	defaultimp.maxBatchSize = 65;
	// textures
	defaultimp.layerList.resize(1);
	defaultimp.layerList[0].worldSize = 100;
	defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
	defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");

	DefineTerrain(0, 0, true);
	// sync load since we want everything in place when we start
	mTerrainGroup->loadAllTerrains(true);
	mTerrainGroup->freeTemporaryResources();

	mSceneManager->setSkyBox(true, "Examples/CloudyNoonSkyBox");

	SetCursor(LoadCursor(NULL, IDC_ARROW));

	mHaveLevel = true;
}

void CEditor::SaveLevel()
{
	if(!mHaveLevel)
		return;

	SetCursor(LoadCursor(NULL, IDC_WAIT));

	mTerrainGroup->saveAllTerrains(true);

	RestoreCurrentDirectory();

	SetCursor(LoadCursor(NULL, IDC_ARROW));
}

BOOL CEditor::Load(LPCTSTR lpszPathName)
{
	SetCursor(LoadCursor(NULL, IDC_WAIT));

	RestoreCurrentDirectory();

	mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(mSceneManager, Ogre::Terrain::ALIGN_X_Z, TERRAIN_SIZE, TERRAIN_WORLD_SIZE);
	mTerrainGroup->setFilenameConvention(TERRAIN_FILE_PREFIX, TERRAIN_FILE_SUFFIX);
	mTerrainGroup->setOrigin(mTerrainPos);

	// Important to set these so that the terrain knows what to use for derived (non-realtime) data
	mTerrainGlobals->setLightMapDirection(mTerrainLight->getDerivedDirection());
	mTerrainGlobals->setCompositeMapAmbient(mSceneManager->getAmbientLight());
	mTerrainGlobals->setCompositeMapDiffuse(mTerrainLight->getDiffuseColour());

	//Ogre::TerrainMaterialGeneratorA::SM2Profile* matProfile = 
	//	static_cast<Ogre::TerrainMaterialGeneratorA::SM2Profile*>(mTerrainGlobals->getDefaultMaterialGenerator()->getActiveProfile());
	//matProfile->setLightmapEnabled(false);

	//Configure default import settings for if we use imported image
	Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
	defaultimp.terrainSize = TERRAIN_SIZE;
	defaultimp.worldSize = TERRAIN_WORLD_SIZE;
	defaultimp.inputScale = 600;
	defaultimp.minBatchSize = 33;
	defaultimp.maxBatchSize = 65;
	// textures
	defaultimp.layerList.resize(1);
	defaultimp.layerList[0].worldSize = 100;
	defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
	defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");


	DefineTerrain(0, 0, false);
	// sync load since we want everything in place when we start
	mTerrainGroup->loadAllTerrains(true);
	mTerrainGroup->freeTemporaryResources();

	mSceneManager->setSkyBox(true, "Examples/CloudyNoonSkyBox");
	//Ocean *_Ocean = new Ocean( 60.0f,mSceneManager,mCamera );

	SetCursor(LoadCursor(NULL, IDC_ARROW));

	mHaveLevel = true;
	return TRUE;
}

void CEditor::DefineTerrain(long x, long y, bool flat )
{
	// if a file is available, use it
	// if not, generate file from import

	// Usually in a real project you'll know whether the compact terrain data is
	// available or not; I'm doing it this way to save distribution size

	if (flat)
	{
		mTerrainGroup->defineTerrain(x, y, 0.0f);
	}
	else
	{
		Ogre::String filename = mTerrainGroup->generateFilename(x, y);
		if (Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename))
		{
			mTerrainGroup->defineTerrain(x, y);
		}
		else
		{
			Ogre::Image img;
			GetTerrainImage(x % 2 != 0, y % 2 != 0, img);
			mTerrainGroup->defineTerrain(x, y, &img);
			bool mTerrainsImported = true;
		}

	}
}

void CEditor::GetTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
	img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	if (flipX)
		img.flipAroundY();
	if (flipY)
		img.flipAroundX();
}

bool CEditor::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	// manually call sample callback to ensure correct order
	mGuiMgr->frameRenderingQueued(evt);

	if ( mHaveLevel )
	{
		// build our acceleration vector based on keyboard input composite
		Ogre::Vector3 accel = Ogre::Vector3::ZERO;
		if (mGoingForward) 
			accel += mCamera->getDirection();
		if (mGoingBack) 
			accel -= mCamera->getDirection();
		if (mGoingRight)
			accel += mCamera->getRight();
		if (mGoingLeft)
			accel -= mCamera->getRight();
		if (mGoingUp) 
			accel += mCamera->getUp();
		if (mGoingDown) 
			accel -= mCamera->getUp();

		// if accelerating, try to reach top speed in a certain time
		Ogre::Real topSpeed = mTopSpeed;
		if (accel.squaredLength() != 0)
		{
			accel.normalise();
			mVelocity += accel * topSpeed * evt.timeSinceLastFrame * 10;
		}
		// if not accelerating, try to stop in a certain time
		else mVelocity -= mVelocity * evt.timeSinceLastFrame * 10;

		Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

		// keep camera velocity below top speed and above epsilon
		if (mVelocity.squaredLength() > topSpeed * topSpeed)
		{
			mVelocity.normalise();
			mVelocity *= topSpeed;
		}
		else if (mVelocity.squaredLength() < tooSmall * tooSmall)
			mVelocity = Ogre::Vector3::ZERO;

		if (mVelocity != Ogre::Vector3::ZERO)
		{
			mCamera->move(mVelocity * evt.timeSinceLastFrame);
		}
		else
		{
			mpEntityMgr->UpdatePageManager();
		}
	}

	return true;
}

void CEditor::OnLButtonDown(UINT nFlags, CPoint point)
{
	if ( !mHaveLevel )
		return;
	mpEditTool->OnLButtonDown(nFlags,point);
}

void CEditor::OnLButtonUp(UINT nFlags, CPoint point)
{
	if ( !mHaveLevel )
		return;
	mpEditTool->OnLButtonUp(nFlags,point);
}


void CEditor::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ( !mHaveLevel )
		return;
	//处理键盘响应
	switch(nChar)
	{
	case 'R':
		{
			Ogre::String newVal;
			Ogre::PolygonMode pm;

			switch (mCamera->getPolygonMode())
			{
			case Ogre::PM_SOLID:
				newVal = "Wireframe";
				pm = Ogre::PM_WIREFRAME;
				break;
			case Ogre::PM_WIREFRAME:
				newVal = "Points";
				pm = Ogre::PM_POINTS;
				break;
			default:
				newVal = "Solid";
				pm = Ogre::PM_SOLID;
			}

			mCamera->setPolygonMode(pm);
		}
		break;
	case 'W':
		{
			mGoingForward = true;
		}
		break;
	case 'S':
		{
			mGoingBack = true;
		}
		break;
	case 'A':
		{
			mGoingLeft = true;
		}
		break;
	case 'D':
		{
			mGoingRight = true;
		}
		break;
	}

	mpEditTool->OnKeyDown(nChar,nRepCnt,nFlags);
}

void CEditor::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ( !mHaveLevel )
		return;
	switch(nChar)
	{
	case 'W':
		{
			mGoingForward = false;
		}
		break;
	case 'S':
		{
			mGoingBack = false;
		}
		break;
	case 'A':
		{
			mGoingLeft = false;
		}
		break;
	case 'D':
		{
			mGoingRight = false;
		}
		break;
	}

	//mGoingForward = false;
	//mGoingBack = false;
	//mGoingLeft = false;
	//mGoingRight = false;
	//mGoingUp = false;
	//mGoingDown = false;
	//mVelocity = Ogre::Vector3::ZERO;

	mpEditTool->OnKeyUp(nChar,nRepCnt,nFlags);
}

void CEditor::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( !mHaveLevel )
		return;

	if ( mRButtonDownPos == point )
		return;

	if(nFlags & MK_RBUTTON)
	{
		CPoint pt = mRButtonDownPos;
		::ClientToScreen( mhWnd, &pt );
		::SetCursorPos( pt.x, pt.y );

		CPoint mouseOffset = point - mRButtonDownPos;

		mCamera->yaw(-Degree(mouseOffset.x) * 0.2f);   
		mCamera->pitch(-Degree(mouseOffset.y) * 0.2f);   
	}   

	mpEditTool->OnMouseMove(nFlags,point);
}

void CEditor::OnRButtonDown(UINT nFlags, CPoint point)
{
	if ( !mHaveLevel )
		return;
	mRButtonDownPos = point;
	mpEditTool->OnRButtonDown(nFlags,point);
}

void CEditor::OnRButtonUp(UINT nFlags, CPoint point)
{
	if ( !mHaveLevel )
		return;
	mpEditTool->OnRButtonUp(nFlags,point);
}

BOOL CEditor::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if ( !mHaveLevel )
		return TRUE;
	Ogre::Vector3 accel = Ogre::Vector3::ZERO;

	if ( zDelta > 0 )
	{
		accel += mCamera->getDirection();
	}
	else if ( zDelta < 0 )
	{
		accel -= mCamera->getDirection();
	}
	mCamera->move(accel * abs(zDelta)*0.1f);

	return mpEditTool->OnMouseWheel( nFlags, zDelta, pt);
}

void CEditor::SetEditTool( CEditTool* pTool )
{
	if ( !pTool )
	{
		if ( mpEditTool && mpEditTool->IsKindOf( RUNTIME_CLASS(CObjectSelectTool) ) )
			return;
		else
			pTool = new CObjectSelectTool;
	}

	if ( mpEditTool != pTool && mpEditTool != 0 )
	{
		mpEditTool->EndTool();
	}
	mpEditTool = pTool;
	if( mpEditTool )
		mpEditTool->BeginTool();

	mpStatusDialog->SetToolName( mpEditTool->GetName() );
	Notify( NE_EDITTOOL_CHANGED );
}

CEditTool* CEditor::GetCurrentEditTool()
{
	return mpEditTool;
}

void CEditor::SetLogWindow(CEdit* pEdit)
{
	mpLogEdit = pEdit;
}

void CEditor::Log( CString& log )
{
	if ( !mpLogEdit )
	{
		LogManager::getSingleton().logMessage( log.GetBuffer() );

		return;
	}
	mpLogEdit->GetWindowText( mLogTest );
	mLogTest += log;
	mLogTest += "\r\n";
	mpLogEdit->SetWindowText( mLogTest );
	mpLogEdit->LineScroll( mpLogEdit->GetLineCount() - 1 );
	//int pos = mpLogEdit->GetScrollPos( SB_VERT );
	//mpLogEdit->SetScrollPos( SB_VERT, pos );
}
void CEditor::SetStatusText( CString& test )
{

}

void CEditor::ShowDatabaseDialog( bool bShow )
{
	if ( !mpDatabaseDialog )
	{
		//AfxGetApp()->GetMainWnd()->GetSafeHwnd();
		mpDatabaseDialog = new CDatabaseDialog;
		mpDatabaseDialog->Create( "DatabaseDialog", "资源浏览器", WS_POPUPWINDOW | WS_DLGFRAME, CRect( 0, 0, 1024, 768 ), 
			CWnd::FromHandle ( AfxGetApp()->GetMainWnd()->GetSafeHwnd() ));

		mpDatabaseDialog->CenterWindow( CWnd::FromHandle ( AfxGetApp()->GetMainWnd()->GetSafeHwnd() ) );
	}
	mpDatabaseDialog->ShowWindow( bShow ? SW_SHOW : SW_HIDE );
}

void CEditor::ShowParticleDialog( bool bShow )
{
	if ( !mpParticleDialog )
	{
		//AfxGetApp()->GetMainWnd()->GetSafeHwnd();
		mpParticleDialog = new CParticleDialog;
		mpParticleDialog->Create( "ParticleDialog", "特效编辑器", WS_POPUPWINDOW | WS_DLGFRAME, CRect( 0, 0, 1024, 768 ), 
			CWnd::FromHandle ( AfxGetApp()->GetMainWnd()->GetSafeHwnd() ));

		mpParticleDialog->CenterWindow( CWnd::FromHandle ( AfxGetApp()->GetMainWnd()->GetSafeHwnd() ) );
	}
	mpParticleDialog->ShowWindow( bShow ? SW_SHOW : SW_HIDE );
}

void CEditor::RestoreCurrentDirectory()
{
	SetCurrentDirectory( mCurDir );
}

void CEditor::Notify( E_NOTIFY_EVENTS event, UINT nflag )
{
	std::list<IEditorEventListener*>::iterator it = mEditorListenerList.begin();

	for ( ; it != mEditorListenerList.end(); it++ )
	{
		(*it)->Notify(event, nflag);
	}
}

void CEditor::RegisterEditorListener( IEditorEventListener* listener )
{
	mEditorListenerList.push_back( listener );
}

void CEditor::unRegisterEditorListener( IEditorEventListener* listener )
{
	std::list<IEditorEventListener*>::iterator it = mEditorListenerList.begin();

	for ( ; it != mEditorListenerList.end(); it++ )
	{
		if ( listener == *it )
		{
			mEditorListenerList.erase(it);
			break;
		}
	}
}

CEditor* GetEditor()
{
	static CEditor* g_pEditor = NULL;
	if ( !g_pEditor )
	{
		g_pEditor = new CEditor;
	}
	return g_pEditor;
}