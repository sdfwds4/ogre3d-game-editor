#ifndef _surface_
#define _surface_

#include "OgreVector2.h"
#include "OgreVector3.h"
#include "OgreVector4.h"
#include "OgreMatrix4.h"
#include "OgreRay.h"
#include "SurfaceMesh.h"
#include "parameterhandler.h"
#include "software_noisemaker.h"
//#include <windows.h>

class surface
{
public:
	surface(const Ogre::Vector3& _pos, const Ogre::Vector3& _n,int _segmentNumX, int _segmentNumY,Ogre::Camera *_cam, parameterhandler *_prm);
	~surface();

public:
	bool prepareForHeightMap();
	bool prepareForNormalMap();
	void set_displacement_amplitude(float _amplitude);
	
private:
	bool initbuffers();
	bool getMinMax(Ogre::Matrix4 *range);

	bool mInitialized;
	software_noisemaker *mSoftwareNoiseMaker;
	SurfaceMesh *mSurfBuffer;
	parameterhandler *mPrm;

	Ogre::Matrix4 mRange;
	Ogre::Plane	mBasePlane, mUpperBoundPlane, mLowerBoundPlane;
	Ogre::Camera *mProjectingCamera,	// the camera that does the actual projection
		         *mRenderingCamera;		// the camera whose frustum the projection is created for

	Ogre::Vector3	mNormal,mPos;
	int mSegmentNumX, mSegmentNumY;
	bool mPlane_Within_Frustum;
};

#endif