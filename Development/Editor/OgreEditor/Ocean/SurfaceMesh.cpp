

#include "stdafx.h"
#include "SurfaceMesh.h"
#include "software_noisemaker.h"

SurfaceMesh::SurfaceMesh(const String& meshName)
{
	this->meshName = meshName ;

}

SurfaceMesh::~SurfaceMesh ()
{

}

void SurfaceMesh::initializeVertexBuffer(int numVer, int typeSize, void* verArray)
{
	numVertices = numVer ;

	// create mesh and submesh
	mesh = MeshManager::getSingleton().createManual(meshName,
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	subMesh = mesh->createSubMesh();
	subMesh->useSharedVertices=false;

	// Vertex buffers
	subMesh->vertexData = new VertexData();
	subMesh->vertexData->vertexStart = 0;
	subMesh->vertexData->vertexCount = numVertices;

	VertexDeclaration* vdecl = subMesh->vertexData->vertexDeclaration;
	VertexBufferBinding* vbind = subMesh->vertexData->vertexBufferBinding;

	size_t offset = 0;
	vdecl->addElement(0, 0, VET_FLOAT3, VES_POSITION);
	offset += VertexElement::getTypeSize(VET_FLOAT3);
	vdecl->addElement(0, offset, VET_FLOAT3, VES_NORMAL);
	offset += VertexElement::getTypeSize(VET_FLOAT3);
	vdecl->addElement(0, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES);

	// Prepare buffer for positions - todo: first attempt, slow
	m_VertexBuffer =
         HardwareBufferManager::getSingleton().createVertexBuffer(
            sizeof(SOFTWARESURFACEVERTEX),
			numVertices,
			HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
	vbind->setBinding(0, m_VertexBuffer);

	if(verArray)
		m_VertexBuffer->writeData(0,
			m_VertexBuffer->getSizeInBytes(), // size
			verArray, // source
			true); // discard?

}

void SurfaceMesh::initializeIndexBuffer(int numEle, int typeSize, void* eleArray)
{
	// Prepare buffer for indices
	m_indexBuffer =
		HardwareBufferManager::getSingleton().createIndexBuffer(
			HardwareIndexBuffer::IT_32BIT,
			numEle,
			HardwareBuffer::HBU_STATIC, true);
	m_indexBuffer->writeData(0,
		m_indexBuffer->getSizeInBytes(), // size
		eleArray, // source
		true); // discard?

	// Set index buffer for this submesh
	subMesh->indexData->indexBuffer = m_indexBuffer;
	subMesh->indexData->indexStart = 0;
	subMesh->indexData->indexCount = numEle;

	AxisAlignedBox meshBounds(-1000000,-1000000,-1000000,
		1000000,1000000, 1000000);
	//AxisAlignedBox meshBounds;
	//meshBounds.setInfinite();
	mesh->_setBounds(meshBounds);

    mesh->load();
    mesh->touch();

}

/* ========================================================================= */
void SurfaceMesh::updateMesh(int numVer, int typeSize, void* verArray)
{
	if(verArray)
		m_VertexBuffer->writeData(0,
			m_VertexBuffer->getSizeInBytes(), // size
			verArray, // source
			true); // discard?
}
