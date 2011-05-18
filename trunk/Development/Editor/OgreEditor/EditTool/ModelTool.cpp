

#include "stdafx.h"
#include "../EditTool.h"
#include "../Editor.h"
#include "ModelTool.h"

CModelToolRenderTargetListener::CModelToolRenderTargetListener()
{
	mpSkyBoxNode = GetEditor()->GetSceneManager()->getSkyBoxNode();
}

void CModelToolRenderTargetListener::preRenderTargetUpdate(const RenderTargetEvent& evt)
{
	mpSkyBoxNode->setVisible(false);
}

void CModelToolRenderTargetListener::postRenderTargetUpdate(const RenderTargetEvent& evt)
{
	//mpSkyBoxNode->setVisible(true);
}

IMPLEMENT_DYNCREATE(CModelTool, CEditTool)

CModelTool::CModelTool() 
		: mpParent(NULL), 
		mpModelNode(NULL), 
		mRenderWindow(NULL),
		mCamera(NULL),
		mViewport(NULL),
		mpEntity(NULL),
		mpLight(NULL),
		mpModelToolRTListner(NULL)
{
	mModelName = "";
}

CModelTool::~CModelTool()
{

}

void CModelTool::BeginTool()
{
	// TODO: 在此添加控件通知处理程序代码
	Ogre::NameValuePairList parms;
	parms["externalWindowHandle"] = Ogre::StringConverter::toString((long)mpParent->GetSafeHwnd());
	CRect r; 
	mpParent->GetClientRect(r);
	mRenderWindow = Ogre::Root::getSingletonPtr()->createRenderWindow("ModelView", r.right, r.bottom, false, &parms);
	mhWnd = mpParent->GetSafeHwnd();

	mpSceneMgr = GetEditor()->GetSceneManager();
	//// Create the camera
	mCamera = GetEditor()->GetSceneManager()->createCamera("ModelCam");

	mCamera->setPosition(Ogre::Vector3(100, -10000, 100));
	mCamera->lookAt(Ogre::Vector3(0, -10000, 0));
	mCamera->setNearClipDistance(1);
	//mCamera->setFarClipDistance(50000);

	mViewport = mRenderWindow->addViewport(mCamera);
	mViewport->setBackgroundColour(Ogre::ColourValue(0.3f,0.3f,0.3f));
	mViewport->setSkiesEnabled(false);
	mViewport->setOverlaysEnabled(false);

	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(
		Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));

	//mpModelToolRTListner = new CModelToolRenderTargetListener;
	//mRenderWindow->addListener(mpModelToolRTListner);
	mpSceneMgr->setAmbientLight(Ogre::ColourValue(0.3f, 0.3f, 0.3f));
	mpLight = mpSceneMgr->createLight();
	mpLight->setPosition(0.0f, 80.0f, 0.0f);

	mpModelNode = mpSceneMgr->getRootSceneNode()->createChildSceneNode( "ModelNode" );
	mpModelNode->attachObject(mpLight);
}

void CModelTool::EndTool()
{
	if ( mModelName.size() != 0 )
	{
		mpModelNode->detachObject(mpEntity);
		mpModelNode->detachObject(mpLight);
		mpSceneMgr->destroyEntity( "ModelTest" );
	}

	mpSceneMgr->destroyLight(mpLight);
	mpSceneMgr->destroySceneNode( mpModelNode );

	mRenderWindow->removeAllViewports( );
	GetEditor()->GetSceneManager()->destroyCamera( "ModelCam" );
	mRenderWindow->destroy();
	Ogre::Root::getSingletonPtr()->getRenderSystem()->destroyRenderWindow( "ModelView" );

	mpParent->Invalidate();
}

BOOL CModelTool::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
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
	return TRUE;
}

bool CModelTool::LoadModel( CString& modelFileName )
{
	if ( mModelName.size() != 0 )
	{
		mpModelNode->detachObject(mpEntity);
		mpSceneMgr->destroyEntity( "ModelTest" );
	}
	mModelName = modelFileName.GetBuffer();

	std::string::size_type pos = mModelName.rfind('.');
	if ( pos > mModelName.size() )
	{
		mModelName = "";
		return false;
	}

	mpEntity = mpSceneMgr->createEntity("ModelTest", modelFileName.GetBuffer() );
	mpModelNode->attachObject(mpEntity);
	mpModelNode->setPosition(Ogre::Vector3(0.0f, -10000.0f, 0.0f));

	return true;
}
