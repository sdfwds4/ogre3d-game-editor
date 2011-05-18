
#include "stdafx.h"
#include "Ocean.h"

OceanHeightGenerator::OceanHeightGenerator(Ocean *_parent)
:m_Parent(_parent)
{
}

void OceanHeightGenerator::preRenderTargetUpdate(const RenderTargetEvent& evt)
{
	Ogre::Vector2 _offset = m_Parent->mCamera->getFrustumOffset();
	m_Parent->mProjCamera->setFrustumOffset(_offset.x,_offset.y);
	m_Parent->mProjCamera->setAspectRatio(m_Parent->mCamera->getAspectRatio());
	m_Parent->mProjCamera->setDirection(m_Parent->mCamera->getRealDirection());
	m_Parent->mProjCamera->setFarClipDistance(m_Parent->mCamera->getFarClipDistance());
	m_Parent->mProjCamera->setFOVy(m_Parent->mCamera->getFOVy());
	m_Parent->mProjCamera->setNearClipDistance(m_Parent->mCamera->getNearClipDistance());
	m_Parent->mProjCamera->setOrientation(m_Parent->mCamera->getRealOrientation());
	m_Parent->mProjCamera->setPosition(m_Parent->mCamera->getRealPosition());
	m_Parent->m_SeaSurface->prepareForHeightMap();

	m_Parent->mOceanEnt->setMaterialName("ProjOcean");
}
void OceanHeightGenerator::postRenderTargetUpdate(const RenderTargetEvent& evt)
{
}


OceanNormalGenerator::OceanNormalGenerator(Ocean *_parent)
:m_Parent(_parent)
{
}

void OceanNormalGenerator::preRenderTargetUpdate(const RenderTargetEvent& evt)
{
	m_Parent->mOceanEnt->setMaterialName("ProjOceanNormal");
	m_Parent->m_SeaSurface->prepareForNormalMap();
}
void OceanNormalGenerator::postRenderTargetUpdate(const RenderTargetEvent& evt)
{
	m_Parent->mOceanEnt->setMaterialName("OceanFresnel");
}

Ocean::Ocean(Ogre::Real _oceanBaseHeight, Ogre::SceneManager *_mgr, Ogre::Camera *_cam)
{
	assert(_mgr);
	assert(_cam);
	
	mOceanBaseHeight = _oceanBaseHeight;
	mCamera = _cam;
	mSceneMgr = _mgr;
	mProjCamera = mSceneMgr->createCamera("UseForProj");

	//mProjCamera->setAspectRatio(mCamera->getAspectRatio());
	mProjCamera->setDirection(mCamera->getRealDirection());
	mProjCamera->setFarClipDistance(mCamera->getFarClipDistance());
	mProjCamera->setFOVy(mCamera->getFOVy());
	mProjCamera->setNearClipDistance(mCamera->getNearClipDistance());
	mProjCamera->setOrientation(mCamera->getRealOrientation());
	mProjCamera->setPosition(mCamera->getRealPosition());

	prm = new parameterhandler();
	//prm->set_float( p_fStrength,	0.9f );
	prm->set_float( p_fStrength,	11.3f );
	prm->set_bool(	p_bDisplace,	true );
	prm->set_int(	p_iOctaves,		8 );
	prm->set_float( p_fScale,		0.18f );
	prm->set_float( p_fFalloff,		0.607f );
	prm->set_float( p_fAnimspeed,	1.4f );
	prm->set_float( p_fTimemulti,	1.27f );
	prm->set_bool(	p_bPaused,		false );

	prm->set_float(	p_fLODbias,		0.0f);
	prm->set_bool(	p_bDisplayTargets, false );
	prm->set_float(	p_fElevation,	17.0f );

	//prm->set_float( p_fSunPosAlpha,		2.7f );
	//prm->set_float( p_fSunPosTheta,		0.1f );
	prm->set_float( p_fSunPosAlpha,		1.38f );
	prm->set_float( p_fSunPosTheta,		1.09f );
	prm->set_float( p_fSunShininess,	84.0f );
	prm->set_float( p_fSunStrength,		12.0f );

	prm->set_bool(	p_bSmooth,			true );

	prm->set_float(	p_bReflRefrStrength,0.1f );

	prm->set_float( p_fWaterColourR,	0.17f );
	prm->set_float( p_fWaterColourG,	0.27f );
	prm->set_float( p_fWaterColourB,	0.26f );

	prm->set_bool(	p_bAsPoints,		false );
	prm->set_bool(	p_bDrawDuckie,		true );
	prm->set_bool(	p_bDrawIsland,		false );
	prm->set_bool(	p_bDiffuseRefl,		false );

	prm->set_active_parameter(p_fStrength);

	m_SeaSurface = new surface(Ogre::Vector3(0,mOceanBaseHeight,0),Ogre::Vector3(0,1,0),128,256,mProjCamera,prm);

	mOceanEnt = mSceneMgr->createEntity("OceanEnt", "OceanSurface");
	mOceanEnt->setVisibilityFlags(0x00000003);
	Ogre::SceneNode *_node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	_node->attachObject(mOceanEnt);

	Ogre::TexturePtr mTexture = TextureManager::getSingleton().getByName("heightmap");
	Ogre::RenderTexture *rttTex = mTexture->getBuffer()->getRenderTarget();
	rttTex->setPriority(1);
	rttTex->setAutoUpdated(true);
	{
		Viewport *v = rttTex->addViewport( mProjCamera );
		v->setVisibilityMask(0x00000002);
		v->setClearEveryFrame(true);
		v->setOverlaysEnabled(false);
		m_HeightGenerator = new OceanHeightGenerator(this);
		rttTex->addListener(m_HeightGenerator);
	}
	Ogre::TexturePtr mTexture1 = TextureManager::getSingleton().getByName("normalmap");
	Ogre::RenderTexture *rttTex1 = mTexture1->getBuffer()->getRenderTarget();
	rttTex1->setPriority(2);
	rttTex1->setAutoUpdated(true);
	{
		Viewport *v = rttTex1->addViewport( mProjCamera );
		v->setVisibilityMask(0x00000002);
		v->setClearEveryFrame(true);
		v->setDimensions(0,0,1,1);
		v->setOverlaysEnabled(false);
		m_NormalGenerator = new OceanNormalGenerator(this);
		rttTex1->addListener(m_NormalGenerator);
	}

	matOcean = MaterialManager::getSingleton().getByName("OceanFresnel");
	passOcean = matOcean->getTechnique(0)->getPass(0);
}

Ocean::~Ocean(void)
{
}

bool Ocean::frameStarted (const Ogre::FrameEvent &e)
{
	return true;
}

void Ocean::update(Ogre::Vector3 _pos,Ogre::ColourValue _col)
{
	passOcean->getFragmentProgramParameters()->setNamedConstant("sunDirection",_pos);
	//Ogre::String str = passOcean->getTextureUnitState("Reflection")->getTextureName();
	//passOcean->getTextureUnitState("Reflection")->setTextureName("oceancube1.bmp",Ogre::TextureType::TEX_TYPE_CUBE_MAP);

	Vector3 pro = Vector3(_pos.x,0,_pos.z);
	pro.normalise();
	_pos.normalise();
	float angle = pro.dotProduct(_pos);

    if( angle >= 0.96 )
	{
		m_SlerpProgress = 0;
		passOcean->getFragmentProgramParameters()->setNamedConstant("sunColor",Ogre::ColourValue(1.2,0.4,0.1,1));
	}
	else if( (0.96>angle) && (angle>0.8) )
	{
		passOcean->getFragmentProgramParameters()->setNamedConstant("sunColor",Ogre::ColourValue(1,1,1,1) + m_Color*(angle - 0.8)/0.16 );
	}
	else
	{
		passOcean->getFragmentProgramParameters()->setNamedConstant("sunColor",Ogre::ColourValue(1,1,1,1));
	}
}