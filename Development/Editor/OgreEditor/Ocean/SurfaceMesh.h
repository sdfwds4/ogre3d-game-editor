#pragma once

#include "OgrePlatform.h"
#include "Ogre.h"

using namespace Ogre ;

class SurfaceMesh
{
private:
	MeshPtr mesh ;
	SubMesh *subMesh ; 
	String meshName ;
	int numFaces ;
	int numVertices ;

	HardwareVertexBufferSharedPtr m_VertexBuffer ;
	HardwareIndexBufferSharedPtr m_indexBuffer ;

public:
	SurfaceMesh(const String& meshName) ;
    
    virtual ~SurfaceMesh ();

	void initializeVertexBuffer(int numVer, int typeSize, void* verArray);
	void initializeIndexBuffer(int numEle, int typeSize, void* eleArray);

	/** updates mesh */
	void updateMesh(int numVer, int typeSize, void* verArray) ;

} ;