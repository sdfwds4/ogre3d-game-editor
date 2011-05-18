#pragma once
#include "Ogre.h"
#include "Surface.h"

class Ocean;

class OceanHeightGenerator : public RenderTargetListener
{
public:
	OceanHeightGenerator(Ocean *_parent);

    void preRenderTargetUpdate(const RenderTargetEvent& evt);
    void postRenderTargetUpdate(const RenderTargetEvent& evt);

	Ocean *m_Parent;
};

class OceanNormalGenerator : public RenderTargetListener
{
public:
	OceanNormalGenerator(Ocean *_parent);

    void preRenderTargetUpdate(const RenderTargetEvent& evt);
    void postRenderTargetUpdate(const RenderTargetEvent& evt);

	Ocean *m_Parent;
};

class Ocean  : public FrameListener
{
	friend class OceanHeightGenerator;
	friend class OceanNormalGenerator;
public:
	Ocean(Ogre::Real _oceanBaseHeight, Ogre::SceneManager *_mgr, Ogre::Camera *_cam);
	~Ocean(void);
	void update(Ogre::Vector3 _pos,Ogre::ColourValue _col);

	bool frameStarted (const Ogre::FrameEvent &e);

private:
	OceanHeightGenerator *m_HeightGenerator;
	OceanNormalGenerator *m_NormalGenerator;
	parameterhandler *prm;
	surface *m_SeaSurface;
	Ogre::Camera *mCamera;
	Ogre::Camera *mProjCamera;
	Ogre::Entity *mOceanEnt;
	Ogre::SceneManager *mSceneMgr;

	MaterialPtr matOcean;
	Ogre::Pass *passOcean;
	bool m_Slerp;
	float m_SlerpProgress;
	float m_SlerpFactor;
	Ogre::ColourValue m_Color;
	Ogre::Real mOceanBaseHeight;
};
