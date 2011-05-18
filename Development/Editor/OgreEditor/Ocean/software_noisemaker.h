#ifndef _software_noisemaker_
#define _software_noisemaker_


#include "parameterhandler.h"
#include "OgreVector2.h"
#include "OgreVector3.h"
#include "OgreVector4.h"
#include "OgreMatrix4.h"
#include "Ogre.h"
#include "OgreTextureManager.h"
using namespace Ogre;

#define n_bits				5
#define n_size				(1<<(n_bits-1))
#define n_size_m1			(n_size - 1)
#define n_size_sq			(n_size*n_size)
#define n_size_sq_m1		(n_size_sq - 1)


#define n_packsize			4

#define np_bits				(n_bits+n_packsize-1)
#define np_size				(1<<(np_bits-1))
#define np_size_m1			(np_size-1)
#define np_size_sq			(np_size*np_size)
#define np_size_sq_m1		(np_size_sq-1)

#define n_dec_bits			12
#define n_dec_magn			4096
#define n_dec_magn_m1		4095

#define max_octaves			32

#define noise_frames		256
#define noise_frames_m1		(noise_frames-1)

#define noise_decimalbits	15
#define noise_magnitude		(1<<(noise_decimalbits-1))

#define scale_decimalbits	15
#define scale_magnitude		(1<<(scale_decimalbits-1))

#define nmapsize_x	512
#define nmapsize_y	1024

struct SOFTWARESURFACEVERTEX
{
	float x,y,z;
	float nx,ny,nz;
	float tu,tv;
};

class software_noisemaker
{
public:
	software_noisemaker(int sizeX, int sizeY, parameterhandler *prm,const Ogre::Plane& _noiseBasePlane);
	~software_noisemaker();

public:
	bool render_geometry(const Ogre::Matrix4& m,const Ogre::Matrix4& _viewMat );
	void generate_normalmap();
	SOFTWARESURFACEVERTEX *vertices;

private:
	float get_height_dual(int, int);
	Ogre::Vector4 calc_worldpos(Ogre::Vector2 uv, const Ogre::Matrix4& m,const Ogre::Matrix4& _viewMat);
	//used for caculating normals by CPU
	void calc_normals();
	void init_noise();
	void calc_noise();
	int readtexel_linear(int u, int v, int);
	int readtexel_linear_dual(int u, int v, int);
	int mapsample(int u, int v, int level, int octave);

	int sizeX, sizeY;	// framebuffer size
	int noise[n_size_sq*noise_frames];
	int o_noise[n_size_sq*max_octaves];
	int p_noise[np_size_sq*(max_octaves>>(n_packsize-1))];	
	int *r_noise;
	int octaves;

	parameterhandler *prm;
	unsigned long last_time;
	double time;

	Ogre::Vector4 t_corners0,t_corners1,t_corners2,t_corners3;
	Ogre::Plane mNoiseBasePlane;
	Ogre::TexturePtr packed_noise_texture[2];
	Ogre::TexturePtr heightmap;
	Ogre::TexturePtr normalmap;

	void init_textures();
	void upload_noise();

	Ogre::GpuProgramParametersSharedPtr parameters;
	Ogre::GpuProgramParametersSharedPtr Fparameters;

};

inline int software_noisemaker::readtexel_linear_dual(int u, int v, int o)
{
	int iu, iup, iv, ivp, fu, fv;
	iu = (u>>n_dec_bits)&np_size_m1;
	iv = ((v>>n_dec_bits)&np_size_m1)*np_size;
	
	iup = ((u>>n_dec_bits) + 1)&np_size_m1;
	ivp = (((v>>n_dec_bits) + 1)&np_size_m1)*np_size;
	
	fu = u & n_dec_magn_m1;
	fv = v & n_dec_magn_m1;
		
	int ut01 = ((n_dec_magn-fu)*r_noise[iv + iu] + fu*r_noise[iv + iup])>>n_dec_bits;
	int ut23 = ((n_dec_magn-fu)*r_noise[ivp + iu] + fu*r_noise[ivp + iup])>>n_dec_bits;
	int ut = ((n_dec_magn-fv)*ut01 + fv*ut23) >> n_dec_bits;
	return ut;
}

inline float software_noisemaker::get_height_dual(int u, int v)
{	
	int value=0;	
	r_noise = p_noise;	// pointer to the current noise source octave
	int hoct = octaves / n_packsize;
	for(int i=0; i<hoct; i++)
	{		
		value += readtexel_linear_dual(u,v,0);
		u = u << n_packsize;
		v = v << n_packsize;
		r_noise += np_size_sq;
	}		
	return value*prm->params[p_fStrength].fData/noise_magnitude;
}

#endif