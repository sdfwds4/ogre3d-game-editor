

#include "stdafx.h"
#include "surface.h"

surface::surface(const Ogre::Vector3& _pos, 
				 const Ogre::Vector3& _n, 
				 int _segmentNumX, 
				 int _segmentNumY, 
				 Ogre::Camera *_cam,
				 parameterhandler *_prm)
{
	mInitialized = true;
	mNormal = _n.normalisedCopy();
	mPos = _pos;
	mBasePlane = Ogre::Plane(mNormal,mPos);

	mPrm = _prm;
	mSegmentNumX = _segmentNumX+1;
	mSegmentNumY = _segmentNumY+1;
	mRenderingCamera = _cam;
	mProjectingCamera = NULL;

	set_displacement_amplitude(0.0f);

	if (!initbuffers())
		mInitialized = false;		// init vertex & indexbuffers

	mSoftwareNoiseMaker = new software_noisemaker(mSegmentNumX,mSegmentNumX, mPrm, mBasePlane);

}

surface::~surface()
{
}

bool surface::initbuffers()
{
	mSurfBuffer = new SurfaceMesh("OceanSurface");
	mSurfBuffer->initializeVertexBuffer(mSegmentNumX*mSegmentNumX,sizeof(SOFTWARESURFACEVERTEX),NULL);

	unsigned int *indexbuffer = new unsigned int[6 * (mSegmentNumX-1)*(mSegmentNumX-1)];
	int i = 0;
	{
		for(int v=0; v<mSegmentNumX-1; v++){
			for(int u=0; u<mSegmentNumX-1; u++){
				// face 1 |/
				indexbuffer[i++]	= v*mSegmentNumX + u;
				indexbuffer[i++]	= v*mSegmentNumX + u + 1;
				indexbuffer[i++]	= (v+1)*mSegmentNumX + u;

				// face 2 /|
				indexbuffer[i++]	= (v+1)*mSegmentNumX + u;
				indexbuffer[i++]	= v*mSegmentNumX + u + 1;
				indexbuffer[i++]	= (v+1)*mSegmentNumX + u + 1;
			}
		}
	}
	mSurfBuffer->initializeIndexBuffer(6 * (mSegmentNumX-1)*(mSegmentNumX-1), sizeof(unsigned int),indexbuffer);
	delete []indexbuffer;

	return true;
}

bool surface::prepareForHeightMap()
{
	if(!mInitialized) return false;
	
	mPlane_Within_Frustum = this->getMinMax(&mRange);
	
	if (mPlane_Within_Frustum)
	{
		mSoftwareNoiseMaker->render_geometry(mRange,mProjectingCamera->getViewMatrix());
		mSurfBuffer->updateMesh(mSegmentNumX*mSegmentNumX,sizeof(SOFTWARESURFACEVERTEX),mSoftwareNoiseMaker->vertices);
	}
	
	return true;
}

bool surface::prepareForNormalMap(){
	if (mPlane_Within_Frustum)
	{
		mSoftwareNoiseMaker->generate_normalmap();
	}
	return true;
}

bool surface::getMinMax(Ogre::Matrix4 *range)
{
	set_displacement_amplitude(mPrm->params[p_fStrength].fData);
	float x_min,y_min,x_max,y_max;
	Ogre::Vector3 frustum[8],proj_points[24];		// frustum to check the camera against

	int n_points=0;
	int cube[] = 
	{	0,1,	0,2,	2,3,	1,3,
		0,4,	2,6,	3,7,	1,5,
		4,6,	4,5,	5,7,	6,7  };

	Ogre::Matrix4 invviewproj;
	invviewproj = (mRenderingCamera->getProjectionMatrixWithRSDepth()*mRenderingCamera->getViewMatrix()).inverse();
	frustum[0] = invviewproj * Ogre::Vector3(-1,-1,0);
	frustum[1] = invviewproj * Ogre::Vector3(+1,-1,0);
	frustum[2] = invviewproj * Ogre::Vector3(-1,+1,0);
	frustum[3] = invviewproj * Ogre::Vector3(+1,+1,0);
	frustum[4] = invviewproj * Ogre::Vector3(-1,-1,+1);
	frustum[5] = invviewproj * Ogre::Vector3(+1,-1,+1);
	frustum[6] = invviewproj * Ogre::Vector3(-1,+1,+1);
	frustum[7] = invviewproj * Ogre::Vector3(+1,+1,+1);

	// check intersections with upper_bound and lower_bound	
	for(int i=0; i<12; i++)
	{
		int src=cube[i*2], dst=cube[i*2+1];
		Ogre::Vector3 _testLine = frustum[dst]-frustum[src];
		Ogre::Real _dist = _testLine.normalise();
		Ogre::Ray _ray( frustum[src], _testLine);
		std::pair<bool,Ogre::Real> _result = Ogre::Math::intersects(_ray,mUpperBoundPlane);
		if( (_result.first) && (_result.second<_dist+0.00001) )
			proj_points[n_points++] = frustum[src] + _result.second * _testLine;
		_result = Ogre::Math::intersects(_ray,mLowerBoundPlane);
		if( (_result.first) && (_result.second<_dist+0.00001) )
			proj_points[n_points++] = frustum[src] + _result.second * _testLine;
	}
	// check if any of the frustums vertices lie between the upper_bound and lower_bound planes
	for(int i=0; i<8; i++)
	{	
		if(mUpperBoundPlane.getDistance(frustum[i])/mLowerBoundPlane.getDistance(frustum[i]) < 0)
		{
			proj_points[n_points++] = frustum[i];
		}		
	}	

	// create the camera the grid will be projected from
	delete mProjectingCamera;
	mProjectingCamera = new Ogre::Camera("projecting",NULL);
	Ogre::Vector2 _offset = mRenderingCamera->getFrustumOffset();
	mProjectingCamera->setFrustumOffset(_offset.x,_offset.y);
	mProjectingCamera->setAspectRatio(mRenderingCamera->getAspectRatio());
	mProjectingCamera->setDirection(mRenderingCamera->getRealDirection());
	mProjectingCamera->setFarClipDistance(mRenderingCamera->getFarClipDistance());
	mProjectingCamera->setFOVy(mRenderingCamera->getFOVy());
	mProjectingCamera->setNearClipDistance(mRenderingCamera->getNearClipDistance());
	mProjectingCamera->setOrientation(mRenderingCamera->getRealOrientation());
	mProjectingCamera->setPosition(mRenderingCamera->getRealPosition());
	// make sure the camera isn't too close to the plane
	float height_in_plane = mBasePlane.getDistance(mProjectingCamera->getRealPosition());

	bool keep_it_simple = false;
	bool underwater=false;

	if (height_in_plane < 0.0f) underwater = true;

	if(keep_it_simple)
	{
		mProjectingCamera->setDirection(mRenderingCamera->getRealDirection());
	}
	else
	{
		Ogre::Vector3 aimpoint, aimpoint2;		

		if (height_in_plane < (mPrm->params[p_fStrength].fData+mPrm->get_float(p_fElevation)))
		{					
			if(underwater)
				mProjectingCamera->setPosition(mRenderingCamera->getRealPosition()+Ogre::Vector3(mLowerBoundPlane.normal.x,mLowerBoundPlane.normal.y,mLowerBoundPlane.normal.z)*(mPrm->params[p_fStrength].fData + mPrm->get_float(p_fElevation) - 2*height_in_plane));
			else
				mProjectingCamera->setPosition(mRenderingCamera->getRealPosition()+Ogre::Vector3(mLowerBoundPlane.normal.x,mLowerBoundPlane.normal.y,mLowerBoundPlane.normal.z)*(mPrm->params[p_fStrength].fData + mPrm->get_float(p_fElevation) - height_in_plane));
		} 
		
		// aim the projector at the point where the camera view-vector intersects the plane
		// if the camera is aimed away from the plane, mirror it's view-vector against the plane
		if( ((mBasePlane.normal).dotProduct(mRenderingCamera->getRealDirection()) < 0.0f) || ((mBasePlane.normal).dotProduct(mRenderingCamera->getRealPosition()) < 0.0f ) )
		{				
			Ogre::Ray _ray( mRenderingCamera->getRealPosition(), mRenderingCamera->getRealDirection());
			std::pair<bool,Ogre::Real> _result = Ogre::Math::intersects(_ray,mBasePlane);
			if(_result.first)
				aimpoint = mRenderingCamera->getRealPosition() + _result.second * mRenderingCamera->getRealDirection();
		}
		else
		{
			Ogre::Vector3 flipped;
			flipped = mRenderingCamera->getRealDirection() - 2*mNormal* (mRenderingCamera->getRealDirection()).dotProduct(mNormal);
			flipped.normalise();
			Ogre::Ray _ray( mRenderingCamera->getRealPosition(), flipped);
			std::pair<bool,Ogre::Real> _result = Ogre::Math::intersects(_ray,mBasePlane);
			if(_result.first)
				aimpoint = mRenderingCamera->getRealPosition() + _result.second * flipped;
		}

		// force the point the camera is looking at in a plane, and have the projector look at it
		// works well against horizon, even when camera is looking upwards
		// doesn't work straight down/up
		float af = fabs( (mBasePlane.normal).dotProduct(mRenderingCamera->getRealDirection()) );
		aimpoint2 = mRenderingCamera->getRealPosition() + 10.0*mRenderingCamera->getRealDirection();
		aimpoint2 = aimpoint2 - mNormal* (aimpoint2.dotProduct(mNormal));

		// fade between aimpoint & aimpoint2 depending on view angle
		aimpoint = aimpoint*af + aimpoint2*(1.0f-af);

		mProjectingCamera->setDirection(aimpoint-mProjectingCamera->getRealPosition());
	}


	for(int i=0; i<n_points; i++)
	{
		// project the point onto the surface plane
		proj_points[i] = proj_points[i] - mBasePlane.normal*mBasePlane.getDistance(proj_points[i]);
		proj_points[i] = mProjectingCamera->getViewMatrix() * proj_points[i];
		proj_points[i] = mProjectingCamera->getProjectionMatrixWithRSDepth() * proj_points[i];
	}

	// get max/min x & y-values to determine how big the "projection window" must be
	if (n_points > 0)
	{
		x_min = proj_points[0].x;
		x_max = proj_points[0].x;
		y_min = proj_points[0].y;
		y_max = proj_points[0].y;
		for(int i=1; i<n_points; i++)
		{
			if (proj_points[i].x > x_max) x_max = proj_points[i].x;
			if (proj_points[i].x < x_min) x_min = proj_points[i].x;
			if (proj_points[i].y > y_max) y_max = proj_points[i].y;
			if (proj_points[i].y < y_min) y_min = proj_points[i].y;
		}		

		// build the packing matrix that spreads the grid across the "projection window"
		Ogre::Matrix4 pack(	x_max-x_min,	0,				0,		x_min,
							0,				y_max-y_min,	0,		y_min,
							0,				0,				1,		0,	
							0,				0,				0,		1);
		Ogre::Matrix4 invviewproj;
		invviewproj = (mProjectingCamera->getProjectionMatrixWithRSDepth()*mProjectingCamera->getViewMatrix()).inverse();
		*range = invviewproj * pack;
		return true;
	}
	return false;
}

void surface::set_displacement_amplitude(float amplitude)
{
	mUpperBoundPlane = Ogre::Plane( mNormal, mPos + amplitude * mNormal);
	mLowerBoundPlane = Ogre::Plane( mNormal, mPos - amplitude * mNormal);
}