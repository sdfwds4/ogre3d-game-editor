

#include "stdafx.h"
#include "software_noisemaker.h"
#include <xmmintrin.h>


#define SSE false	// SSE is broken atm
#define quadpipe false
#define packednoise true

software_noisemaker::software_noisemaker(int sX, int sY, parameterhandler *prm,const Ogre::Plane& _noiseBasePlane)
{
	this->sizeX = sX;
	this->sizeY = sY;
	this->prm = prm;
	time = 0.0;
	last_time = Ogre::Root::getSingletonPtr()->getTimer()->getMilliseconds();
	octaves = 0;	// don't want to have the noise accessed before it's calculated

	// reset normals
	vertices	= new SOFTWARESURFACEVERTEX[sizeX*sizeY];	
	for(int v=0; v<sizeY; v++)
	{
		for(int u=0; u<sizeX; u++)
		{
			vertices[v*sizeX + u].nx =	0.0f;
			vertices[v*sizeX + u].ny =	1.0f;
			vertices[v*sizeX + u].nz =	0.0f;
			vertices[v*sizeX + u].tu = (float)u/(sizeX-1);
			vertices[v*sizeX + u].tv = (float)v/(sizeY-1);
		}
	}	
	this->init_noise();	
	this->init_textures();
	mNoiseBasePlane = _noiseBasePlane;
}

void software_noisemaker::init_noise()
{	
	// create noise (uniform)
	float tempnoise[n_size_sq*noise_frames];
	for(int i=0; i<(n_size_sq*noise_frames); i++)
	{
		float temp = (float) rand()/RAND_MAX;		
		tempnoise[i] = 4*(temp - 0.5f);	
	}	

	for(int frame=0; frame<noise_frames; frame++)
	{
		for(int v=0; v<n_size; v++)
		{
			for(int u=0; u<n_size; u++)
			{	
				int v0 = ((v-1)&n_size_m1)*n_size,
					v1 = v*n_size,
					v2 = ((v+1)&n_size_m1)*n_size,
					u0 = ((u-1)&n_size_m1),
					u1 = u,
					u2 = ((u+1)&n_size_m1),					
					f  = frame*n_size_sq;
				float temp = (1.0f/14.0f) * (	tempnoise[f + v0 + u0] + tempnoise[f + v0 + u1] + tempnoise[f + v0 + u2] +
										tempnoise[f + v1 + u0] + 6.0f*tempnoise[f + v1 + u1] + tempnoise[f + v1 + u2] +
										tempnoise[f + v2 + u0] + tempnoise[f + v2 + u1] + tempnoise[f + v2 + u2]);
									  
				this->noise[frame*n_size_sq + v*n_size + u] = noise_magnitude*temp;
			}
		}
	}	
	
}

void software_noisemaker::calc_noise()
{
	#define min(a,b)            (((a) < (b)) ? (a) : (b))
	int multitable[max_octaves];
	float f_multitable[max_octaves];

	octaves = min(prm->params[p_iOctaves].iData, max_octaves);		

	// calculate the strength of each octave
	float sum=0.0f;
	for(int i=0; i<octaves; i++)
	{
		f_multitable[i] = powf(prm->get_float(p_fFalloff),1.0f*i);
		sum += f_multitable[i];
	}

	{
	for(int i=0; i<octaves; i++)
	{
		f_multitable[i] /= sum;
	}}
	
	{
	for(int i=0; i<octaves; i++)
	{
		multitable[i] = scale_magnitude*f_multitable[i];
	}}
	
	//Ogre::Root::getSingletonPtr()->getTimer()->getMilliseconds()
	DWORD this_time = Ogre::Root::getSingletonPtr()->getTimer()->getMilliseconds();
	double itime = this_time - last_time;
	static double lp_itime=0.0;	
	last_time = this_time;
	itime *= 0.001 * prm->get_float( p_fAnimspeed );
	lp_itime = 0.99*lp_itime + 0.01 * itime;
	if ( !prm->get_bool(p_bPaused) )
		time += lp_itime;			

	
	double	r_timemulti = 1.0;

	for(int o=0; o<octaves; o++)
	{		
		unsigned int image[3];
		int amount[3];
		double dImage, fraction = modf(time*r_timemulti,&dImage);
		int iImage = (int)dImage;
		amount[0] = scale_magnitude*f_multitable[o]*(pow(sin((fraction+2)*Ogre::Math::PI/3),2)/1.5);
		amount[1] = scale_magnitude*f_multitable[o]*(pow(sin((fraction+1)*Ogre::Math::PI/3),2)/1.5);
		amount[2] = scale_magnitude*f_multitable[o]*(pow(sin((fraction)*Ogre::Math::PI/3),2)/1.5);
		image[0] = (iImage) & noise_frames_m1;
		image[1] = (iImage+1) & noise_frames_m1;
		image[2] = (iImage+2) & noise_frames_m1;
		{	
			for(int i=0; i<n_size_sq; i++)
			{
				o_noise[i + n_size_sq*o] =	(	((amount[0] * noise[i + n_size_sq * image[0]])>>scale_decimalbits) + 
												((amount[1] * noise[i + n_size_sq * image[1]])>>scale_decimalbits) + 
												((amount[2] * noise[i + n_size_sq * image[2]])>>scale_decimalbits));
			}
		}

		r_timemulti *= prm->get_float( p_fTimemulti );
	}

	if(packednoise)
	{
		int octavepack = 0;
		for(int o=0; o<octaves; o+=n_packsize)
		{
			for(int v=0; v<np_size; v++)
			for(int u=0; u<np_size; u++)
			{
				p_noise[v*np_size+u+octavepack*np_size_sq] = o_noise[(o+3)*n_size_sq + (v&n_size_m1)*n_size + (u&n_size_m1)];
				p_noise[v*np_size+u+octavepack*np_size_sq] += mapsample( u, v, 3, o);
				p_noise[v*np_size+u+octavepack*np_size_sq] += mapsample( u, v, 2, o+1);
				p_noise[v*np_size+u+octavepack*np_size_sq] += mapsample( u, v, 1, o+2);				
			}
			octavepack++;
		}
	}
}

inline int software_noisemaker::mapsample(int u, int v, int upsamplepower, int octave)
{
	int magnitude = 1<<upsamplepower;
	int pu = u >> upsamplepower;
	int pv = v >> upsamplepower;	
	int fu = u & (magnitude-1);
	int fv = v & (magnitude-1);
	int fu_m = magnitude - fu;
	int fv_m = magnitude - fv;

	int o = fu_m*fv_m*o_noise[octave*n_size_sq + ((pv)&n_size_m1)*n_size + ((pu)&n_size_m1)] +
			fu*fv_m*o_noise[octave*n_size_sq + ((pv)&n_size_m1)*n_size + ((pu+1)&n_size_m1)] +
			fu_m*fv*o_noise[octave*n_size_sq + ((pv+1)&n_size_m1)*n_size + ((pu)&n_size_m1)] +
			fu*fv*o_noise[octave*n_size_sq + ((pv+1)&n_size_m1)*n_size + ((pu+1)&n_size_m1)];

	return o >> (upsamplepower+upsamplepower);
}

software_noisemaker::~software_noisemaker()
{
	delete [] vertices;
	packed_noise_texture[0].setNull();
	packed_noise_texture[1].setNull();
	heightmap.setNull();
	normalmap.setNull();
}

bool software_noisemaker::render_geometry(const Ogre::Matrix4& m,const Ogre::Matrix4& _viewMat)
{
	this->calc_noise();

	float magnitude = n_dec_magn * prm->get_float(p_fScale);

	t_corners0 = calc_worldpos(Ogre::Vector2(0.0f,0.0f),m,_viewMat);
	t_corners1 = calc_worldpos(Ogre::Vector2(+1.0f,0.0f),m,_viewMat);
	t_corners2 = calc_worldpos(Ogre::Vector2(0.0f,+1.0f),m,_viewMat);
	t_corners3 = calc_worldpos(Ogre::Vector2(+1.0f,+1.0f),m,_viewMat);

	float	du = 1.0f/float(sizeX-1),
		dv = 1.0f/float(sizeY-1),
		u,v=0.0f;
	Ogre::Vector4 result;
	int i=0;
	for(int iv=0; iv<sizeY; iv++)
	{
		u = 0.0f;		
		for(int iu=0; iu<sizeX; iu++)
		{				

			result.x = (1.0f-v)*( (1.0f-u)*t_corners0.x + u*t_corners1.x ) + v*( (1.0f-u)*t_corners2.x + u*t_corners3.x );				
			result.z = (1.0f-v)*( (1.0f-u)*t_corners0.z + u*t_corners1.z ) + v*( (1.0f-u)*t_corners2.z + u*t_corners3.z );				
			result.w = (1.0f-v)*( (1.0f-u)*t_corners0.w + u*t_corners1.w ) + v*( (1.0f-u)*t_corners2.w + u*t_corners3.w );				

			float divide = 1.0f/result.w;				
			result.x *= divide;
			result.z *= divide;

			vertices[i].x = result.x;
			vertices[i].z = result.z;
			vertices[i].y = -mNoiseBasePlane.d + get_height_dual(magnitude*result.x, magnitude*result.z );

			i++;
			u += du;
		}
		v += dv;			
	}

	// smooth the heightdata
	if(prm->params[p_bSmooth].bData)
	{
		for(int v=1; v<(sizeY-1); v++)
		{
			for(int u=1; u<(sizeX-1); u++)
			{				
				vertices[v*sizeX + u].y =	0.2f * (vertices[v*sizeX + u].y +
					vertices[v*sizeX + (u+1)].y + 
					vertices[v*sizeX + (u-1)].y + 
					vertices[(v+1)*sizeX + u].y + 
					vertices[(v-1)*sizeX + u].y);															
			}
		}
	}

	if(!prm->params[p_bDisplace].bData)
	{
		// reset height to 0
		for(int u=0; u<(sizeX*sizeY); u++)
		{
			vertices[u].y = 0;
		}

	}

	this->upload_noise();

	return true;
}



// check the point of intersection with the plane (0,1,0,0) and return the position in homogenous coordinates 
Ogre::Vector4 software_noisemaker::calc_worldpos(Ogre::Vector2 uv, const Ogre::Matrix4& m,const Ogre::Matrix4& _viewMat)
{	
	// this is hacky.. this does take care of the homogenous coordinates in a correct way, 
	// but only when the plane lies at y=0
	//Ogre::Vector4	origin(uv.x,uv.y,0,1);
	//Ogre::Vector4	direction(uv.x,uv.y,1,1);

	//origin = m*origin;
	//direction = m*direction;
	//direction -= origin;

	//float	l = -origin.y / direction.y;	// assumes the plane is y=0可以这样假设吗？(RenCheng)

	//Ogre::Vector4 worldPos = origin + direction*l;    
	//return worldPos;
	Ogre::Vector4	origin(uv.x,uv.y,-1,1);
	Ogre::Vector4	direction(uv.x,uv.y,1,1);

	origin = m*origin;
	direction = m*direction;

	Ogre::Vector3 _org(origin.x/origin.w,origin.y/origin.w,origin.z/origin.w);
	Ogre::Vector3 _dir(direction.x/direction.w,direction.y/direction.w,direction.z/direction.w);
	_dir -= _org;
	_dir.normalise();

	Ogre::Ray _ray(_org,_dir);
	std::pair<bool,Ogre::Real> _result = _ray.intersects(mNoiseBasePlane);
	float l = _result.second;
	Ogre::Vector3 worldPos = _org + _dir*l;
	Ogre::Vector4 _tempVec = _viewMat*Ogre::Vector4(worldPos);
	float _temp = -_tempVec.z/_tempVec.w;
	Ogre::Vector4 retPos(worldPos);
	retPos /= _temp;

	return Ogre::Vector4(retPos);
}

void software_noisemaker::calc_normals()
{
	for(int v=1; v<(sizeY-1); v++)
	{
		for(int u=1; u<(sizeX-1); u++)
		{
			Ogre::Vector3 vec1(	vertices[v*sizeX + u + 1].x-vertices[v*sizeX + u - 1].x,
				vertices[v*sizeX + u + 1].y-vertices[v*sizeX + u - 1].y, 
				vertices[v*sizeX + u + 1].z-vertices[v*sizeX + u - 1].z);

			Ogre::Vector3 vec2(	vertices[(v+1)*sizeX + u].x - vertices[(v-1)*sizeX + u].x,
				vertices[(v+1)*sizeX + u].y - vertices[(v-1)*sizeX + u].y,
				vertices[(v+1)*sizeX + u].z - vertices[(v-1)*sizeX + u].z);
			Ogre::Vector3 normal = vec2.crossProduct(vec1);
			vertices[v*sizeX + u].nx = normal.x;
			vertices[v*sizeX + u].ny = normal.y;
			vertices[v*sizeX + u].nz = normal.z;

		}
	}
}

inline int software_noisemaker::readtexel_linear(int u, int v, int offset)
{
	int iu, iup, iv, ivp, fu, fv;
	iu = (u>>n_dec_bits)&n_size_m1;
	iv = ((v>>n_dec_bits)&n_size_m1)*n_size;

	iup = (((u>>n_dec_bits) + 1)&n_size_m1);
	ivp = (((v>>n_dec_bits) + 1)&n_size_m1)*n_size;

	fu = u & n_dec_magn_m1;
	fv = v & n_dec_magn_m1;

	int ut01 = ((n_dec_magn-fu)*o_noise[offset + iv + iu] + fu*o_noise[offset + iv + iup]) >> n_dec_bits;
	int ut23 = ((n_dec_magn-fu)*o_noise[offset + ivp + iu] + fu*o_noise[offset + ivp + iup]) >> n_dec_bits ;
	int ut = ((n_dec_magn-fv)*ut01 + fv*ut23) >> n_dec_bits;
	return ut;
}


void software_noisemaker::init_textures()
{
	// the noise textures. currently two of them (= 8 levels)
	// Create the texture
	packed_noise_texture[0] = TextureManager::getSingleton().createManual(
		"Noise0", // name
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		TEX_TYPE_2D,      // type
		np_size, np_size,         // width & height
		0,
		PF_L16,     // pixel format
		TU_DYNAMIC_WRITE_ONLY);      // usage; should be TU_DYNAMIC_WRITE_ONLY_DISCARDABLE for

	packed_noise_texture[1] = TextureManager::getSingleton().createManual(
		"Noise1", // name
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		TEX_TYPE_2D,      // type
		np_size, np_size,         // width & height
		0,
		PF_L16,     // pixel format
		TU_DYNAMIC_WRITE_ONLY);      // usage; should be TU_DYNAMIC_WRITE_ONLY_DISCARDABLE for

	heightmap = TextureManager::getSingleton().createManual(
		"heightmap", // name
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		TEX_TYPE_2D,      // type
		nmapsize_x, nmapsize_y,         // width & height
		0,                // number of mipmaps
		PF_FLOAT16_RGB,     // pixel format
		TU_RENDERTARGET);      // usage; should be TU_DYNAMIC_WRITE_ONLY_DISCARDABLE for

	normalmap = TextureManager::getSingleton().createManual(
		"normalmap", // name
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		TEX_TYPE_2D,      // type
		nmapsize_x, nmapsize_y,         // width & height
		0,                // number of mipmaps
		PF_FLOAT16_RGB,     // pixel format
		TU_RENDERTARGET);      // usage; should be TU_DYNAMIC_WRITE_ONLY_DISCARDABLE for

	MaterialPtr mat = MaterialManager::getSingleton().getByName("ProjOceanNormal");
	Ogre::Pass *pass = mat->getTechnique (0)->getPass (0);
	parameters = pass->getVertexProgramParameters();
	Fparameters = pass->getFragmentProgramParameters();
}

void software_noisemaker::upload_noise()
{
	//uint8* data;
	unsigned short *data;
	for(int t=0; t<2; t++)
	{
		int offset = np_size_sq*t;

		HardwarePixelBufferSharedPtr pixelBuffer = packed_noise_texture[t]->getBuffer(0,0);

		pixelBuffer->lock(HardwareBuffer::HBL_DISCARD); // for best performance use HBL_DISCARD!
		const PixelBox& pixelBox = pixelBuffer->getCurrentLock();
		data = static_cast<unsigned short *>(pixelBox.data);
		for(int i=0; i<np_size_sq; i++)
			data[i] = 32768+p_noise[i+offset];
		pixelBuffer->unlock();
	}
}

void software_noisemaker::generate_normalmap( )
{
    	parameters->setNamedConstant ("corner00", t_corners0);
		parameters->setNamedConstant ("corner01", t_corners1);
		parameters->setNamedConstant ("corner10", t_corners2);
		parameters->setNamedConstant ("corner11", t_corners3);
		parameters->setNamedConstant("inv_mapsize_x", 1.0f/nmapsize_x);
		parameters->setNamedConstant("inv_mapsize_y", 1.0f/nmapsize_y);
		Fparameters->setNamedConstant("amplitude",prm->params[p_fStrength].fData);
		Fparameters->setNamedConstant("inv_mapsize_x", 1.0f/nmapsize_x);
		Fparameters->setNamedConstant("inv_mapsize_y", 1.0f/nmapsize_y);
}
