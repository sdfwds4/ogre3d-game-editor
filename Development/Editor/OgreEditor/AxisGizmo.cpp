

#include "stdafx.h"
#include "AxisGizmo.h"
#include "Editor.h"

const unsigned int QUERYFLAG_WIDGET = 1;

AxisGizmo::AxisGizmo() : mSceneMgr(NULL),mGizmoScale(1.0f), 
mGizmoNode(0), mGizmoX(0), mGizmoY(0), mGizmoZ(0), 
mWorldSpaceGizmoOrientation(false), 
mOldGizmoMode(256), mOldGizmoAxis(256), mVertexBufferSize(0), 
mIndexBufferSize(0), mVertexBuffer(0), mIndexBuffer(0)
{
	mGizmoEntities[0] = mGizmoEntities[1] = mGizmoEntities[2] = 0;
	mGizmoEntities[3] = mGizmoEntities[4] = mGizmoEntities[5] = 0;
	mAxisText[0] = mAxisText[1] = mAxisText[2] = 0;
	mTextX = mTextY = mTextZ = 0;
}

AxisGizmo::~AxisGizmo()
{
}

//-----------------------------------------------------------------------------------------
void AxisGizmo::CreateGizmo()
{
	Ogre::MaterialPtr gizmoMaterial = MaterialManager::getSingletonPtr()->createOrRetrieve("AxisGizmo_Material", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME).first;
	gizmoMaterial->setReceiveShadows(false);
	gizmoMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false); 
	gizmoMaterial->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
	gizmoMaterial->getTechnique(0)->getPass(0)->setCullingMode(CULL_NONE);
	gizmoMaterial->getTechnique(0)->getPass(0)->setVertexColourTracking(Ogre::TVC_DIFFUSE);

	mSceneMgr = GetEditor()->GetSceneManager();
	AxisGizmo::createMesh(GetSceneManager(), "OgitorAxisGizmoMesh");
	AxisGizmo::createPlaneMesh(GetSceneManager(), "OgitorAxisPlaneMesh");

	mGizmoNode = GetSceneManager()->getRootSceneNode()->createChildSceneNode("scbWidgetNode",Vector3(0,0,0),Quaternion::IDENTITY);

	mGizmoX = mGizmoNode->createChildSceneNode("scbnwx",Vector3(0,0,0),Quaternion::IDENTITY);
	mGizmoY = mGizmoNode->createChildSceneNode("scbnwy",Vector3(0,0,0),Quaternion::IDENTITY);
	mGizmoZ = mGizmoNode->createChildSceneNode("scbnwz",Vector3(0,0,0),Quaternion::IDENTITY);

	mTextX = mGizmoNode->createChildSceneNode("textx",Vector3(4.5f,-0.5f,0),Quaternion::IDENTITY);
	mTextY = mGizmoNode->createChildSceneNode("texty",Vector3(0,3.2f,0),Quaternion::IDENTITY);
	mTextZ = mGizmoNode->createChildSceneNode("textz",Vector3(0,-0.5f,3.5f),Quaternion::IDENTITY);

	mAxisText[0] = new MovableText("TXT_X", "x");
	// Center horizontally and display above the node
	mAxisText[0]->setTextAlignment(MovableText::H_CENTER, MovableText::V_ABOVE); 
	//msg->setAdditionalHeight( 2.0f ); //msg->setAdditionalHeight( ei.getRadius() )
	mAxisText[0]->showOnTop(true);
	mAxisText[0]->setRenderQueueGroup(RENDER_QUEUE_SKIES_LATE);
	mAxisText[0]->setQueryFlags(QUERYFLAG_WIDGET);
	mTextX->attachObject(mAxisText[0]);

	mAxisText[1] = new MovableText("TXT_Y", "y");
	// Center horizontally and display above the node
	mAxisText[1]->setTextAlignment(MovableText::H_CENTER, MovableText::V_ABOVE); 
	//msg->setAdditionalHeight( 2.0f ); //msg->setAdditionalHeight( ei.getRadius() )
	mAxisText[1]->showOnTop(true);
	mAxisText[1]->setRenderQueueGroup(RENDER_QUEUE_SKIES_LATE);
	mAxisText[1]->setQueryFlags(QUERYFLAG_WIDGET);
	mTextY->attachObject(mAxisText[1]);

	mAxisText[2] = new MovableText("TXT_Z", "z");
	// Center horizontally and display above the node
	mAxisText[2]->setTextAlignment(MovableText::H_CENTER, MovableText::V_ABOVE); 
	//msg->setAdditionalHeight( 2.0f ); //msg->setAdditionalHeight( ei.getRadius() )
	mAxisText[2]->showOnTop(true);
	mAxisText[2]->setRenderQueueGroup(RENDER_QUEUE_SKIES_LATE);
	mAxisText[2]->setQueryFlags(QUERYFLAG_WIDGET);
	mTextZ->attachObject(mAxisText[2]);

	Quaternion q1;
	Quaternion q2;

	q1.FromAngleAxis(Degree(90), Vector3(0,0,1));
	q2.FromAngleAxis(Degree(90), Vector3(1,0,0));
	mGizmoY->setOrientation(q1 * q2);

	q1.FromAngleAxis(Degree(-90), Vector3(0,1,0));
	q2.FromAngleAxis(Degree(-90), Vector3(1,0,0));
	mGizmoZ->setOrientation(q1 * q2);

	//Entities
	mGizmoEntities[0] = GetSceneManager()->createEntity("scbwx", "OgitorAxisGizmoMesh");
	mGizmoEntities[1] = GetSceneManager()->createEntity("scbwy", "OgitorAxisGizmoMesh");
	mGizmoEntities[2] = GetSceneManager()->createEntity("scbwz", "OgitorAxisGizmoMesh");
	mGizmoEntities[3] = GetSceneManager()->createEntity("scbwt", "OgitorAxisPlaneMesh");
	mGizmoEntities[4] = GetSceneManager()->createEntity("scbwu", "OgitorAxisPlaneMesh");
	mGizmoEntities[5] = GetSceneManager()->createEntity("scbwv", "OgitorAxisPlaneMesh");

	//XX arrows
	mGizmoEntities[0]->setCastShadows(false);
	mGizmoEntities[0]->setMaterialName("MAT_GIZMO_X");
	mGizmoEntities[0]->setRenderQueueGroup(RENDER_QUEUE_SKIES_LATE);
	mGizmoEntities[0]->setQueryFlags(QUERYFLAG_WIDGET);
	mGizmoX->attachObject(mGizmoEntities[0]);

	//YY arrows
	mGizmoEntities[1]->setCastShadows(false);
	mGizmoEntities[1]->setMaterialName("MAT_GIZMO_Y");
	mGizmoEntities[1]->setRenderQueueGroup(RENDER_QUEUE_SKIES_LATE);
	mGizmoEntities[1]->setQueryFlags(QUERYFLAG_WIDGET);
	mGizmoY->attachObject(mGizmoEntities[1]);

	//ZZ arrows
	mGizmoEntities[2]->setCastShadows(false);
	mGizmoEntities[2]->setMaterialName("MAT_GIZMO_Z");
	mGizmoEntities[2]->setRenderQueueGroup(RENDER_QUEUE_SKIES_LATE);
	mGizmoEntities[2]->setQueryFlags(QUERYFLAG_WIDGET);
	mGizmoZ->attachObject(mGizmoEntities[2]);

	//XY Plane
	mGizmoEntities[3]->setCastShadows(false);
	mGizmoEntities[3]->setMaterialName("MAT_GIZMO_XY");
	mGizmoEntities[3]->setRenderQueueGroup(RENDER_QUEUE_SKIES_LATE);
	mGizmoEntities[3]->setQueryFlags(QUERYFLAG_WIDGET);
	mGizmoX->attachObject(mGizmoEntities[3]);

	//YZ Plane
	mGizmoEntities[4]->setCastShadows(false);
	mGizmoEntities[4]->setMaterialName("MAT_GIZMO_YZ");
	mGizmoEntities[4]->setRenderQueueGroup(RENDER_QUEUE_SKIES_LATE);
	mGizmoEntities[4]->setQueryFlags(QUERYFLAG_WIDGET);
	mGizmoY->attachObject(mGizmoEntities[4]);

	//ZX Plane
	mGizmoEntities[5]->setCastShadows(false);
	mGizmoEntities[5]->setMaterialName("MAT_GIZMO_ZX");
	mGizmoEntities[5]->setRenderQueueGroup(RENDER_QUEUE_SKIES_LATE);
	mGizmoEntities[5]->setQueryFlags(QUERYFLAG_WIDGET);
	mGizmoZ->attachObject(mGizmoEntities[5]);

	mGizmoNode->setVisible(false);
}

//-----------------------------------------------------------------------------------------
void AxisGizmo::UpdateGizmo()
{
	//CViewportEditor *viewport = GetViewport();

	if(mGizmoEntities[0])
	{
		Ogre::Vector3 cameraPos = GetEditor()->GetCamera()->getPosition();
		float dist1 = (cameraPos - mGizmoEntities[0]->getWorldBoundingBox().getCenter()).length();
		float dist2 = (cameraPos - mGizmoEntities[1]->getWorldBoundingBox().getCenter()).length();
		float dist3 = (cameraPos - mGizmoEntities[2]->getWorldBoundingBox().getCenter()).length();

		int orders[3];
		int increment = -1;

		//if(viewport->GetEditorTool() == TOOL_ROTATE)
		//{
		//	increment = 1;
		//	orders[0] = orders[1] = orders[2] = 93;
		//}
		//else
		{
			increment = -1;
			orders[0] = orders[1] = orders[2] = 95;
		}

		if(dist1 > dist2) 
			orders[0] += increment;
		else
			orders[1] += increment;

		if(dist1 > dist3) 
			orders[0] += increment;
		else
			orders[2] += increment;

		if(dist2 > dist3) 
			orders[1] += increment;
		else
			orders[2] += increment;

		mGizmoEntities[0]->setRenderQueueGroup(orders[0]);
		mGizmoEntities[1]->setRenderQueueGroup(orders[1]);
		mGizmoEntities[2]->setRenderQueueGroup(orders[2]);
	}
}

//-----------------------------------------------------------------------------------------
void AxisGizmo::HighlightGizmo(int ID)
{
	if(mOldGizmoAxis == ID) 
		return;

	mOldGizmoAxis = ID;
	Entity* wx = mGizmoEntities[0];
	Entity* wy = mGizmoEntities[1];
	Entity* wz = mGizmoEntities[2];

	if(ID & AXIS_X)
		wx->setMaterialName("MAT_GIZMO_X_L");
	else
		wx->setMaterialName("MAT_GIZMO_X");
	if(ID & AXIS_Y)
		wy->setMaterialName("MAT_GIZMO_Y_L");
	else
		wy->setMaterialName("MAT_GIZMO_Y");
	if(ID & AXIS_Z)
		wz->setMaterialName("MAT_GIZMO_Z_L");
	else
		wz->setMaterialName("MAT_GIZMO_Z");

	if((ID & AXIS_XY) == AXIS_XY)
		mGizmoEntities[3]->setMaterialName("MAT_GIZMO_XY_L");
	else
		mGizmoEntities[3]->setMaterialName("MAT_GIZMO_XY");
	if((ID & AXIS_YZ) == AXIS_YZ)
		mGizmoEntities[4]->setMaterialName("MAT_GIZMO_YZ_L");
	else
		mGizmoEntities[4]->setMaterialName("MAT_GIZMO_YZ");
	if((ID & AXIS_XZ) == AXIS_XZ)
		mGizmoEntities[5]->setMaterialName("MAT_GIZMO_ZX_L");
	else
		mGizmoEntities[5]->setMaterialName("MAT_GIZMO_ZX");
}
//-----------------------------------------------------------------------------------------
void AxisGizmo::SetGizmoScale(Ogre::Real value)
{
	Vector3 pos = GetEditor()->GetCamera()->getPosition();
	float distance = (mGizmoNode->getPosition() - pos).length();

	Ogre::Vector4 rect;
	int iLeft,iTop,iWidth,iHeight;
	GetEditor()->GetViewport()->getActualDimensions(iLeft,iTop,iWidth,iHeight);
	rect.x = iLeft;
	rect.y = iTop;
	rect.z = iWidth;
	rect.w = iHeight;
	float minsize = std::min(rect.z, rect.w);
	distance /= (minsize / 20.0f);

	mGizmoNode->setScale(distance, distance, distance);
}
//-----------------------------------------------------------------------------------------
void AxisGizmo::SetGizmoMode(int mode)
{
	if(mOldGizmoMode == mode) 
		return;
	mOldGizmoMode = mode;
	Entity* wx = mGizmoEntities[0];
	Entity* wy = mGizmoEntities[1];
	Entity* wz = mGizmoEntities[2];

	mGizmoEntities[3]->getSubEntity(0)->setVisible(false);
	mGizmoEntities[4]->getSubEntity(0)->setVisible(false);
	mGizmoEntities[5]->getSubEntity(0)->setVisible(false);

	wx->getSubEntity(0)->setVisible(true);
	wx->getSubEntity(1)->setVisible(false);
	wx->getSubEntity(2)->setVisible(false);
	wx->getSubEntity(3)->setVisible(false);
	wx->getSubEntity(4)->setVisible(false);
	wy->getSubEntity(0)->setVisible(true);
	wy->getSubEntity(1)->setVisible(false);
	wy->getSubEntity(2)->setVisible(false);
	wy->getSubEntity(3)->setVisible(false);
	wy->getSubEntity(4)->setVisible(false);
	wz->getSubEntity(0)->setVisible(true);
	wz->getSubEntity(1)->setVisible(false);
	wz->getSubEntity(2)->setVisible(false);
	wz->getSubEntity(3)->setVisible(false);
	wz->getSubEntity(4)->setVisible(false);
	switch(mode)
	{
	case TOOL_MOVE:
		{
			wx->getSubEntity(2)->setVisible(true);
			wy->getSubEntity(2)->setVisible(true);
			wz->getSubEntity(2)->setVisible(true);
			mGizmoEntities[3]->getSubEntity(0)->setVisible(true);
			mGizmoEntities[4]->getSubEntity(0)->setVisible(true);
			mGizmoEntities[5]->getSubEntity(0)->setVisible(true);
			break;
		}
	case TOOL_ROTATE:
		{
			wx->getSubEntity(0)->setVisible(false);
			wy->getSubEntity(0)->setVisible(false);
			wz->getSubEntity(0)->setVisible(false);
			wx->getSubEntity(1)->setVisible(true);
			wy->getSubEntity(1)->setVisible(true);
			wz->getSubEntity(1)->setVisible(true);
			wx->getSubEntity(3)->setVisible(true);
			wy->getSubEntity(3)->setVisible(true);
			wz->getSubEntity(3)->setVisible(true);
			break;
		}
	case TOOL_SCALE:
		{
			wx->getSubEntity(4)->setVisible(true);
			wy->getSubEntity(4)->setVisible(true);
			wz->getSubEntity(4)->setVisible(true);
			mGizmoEntities[3]->getSubEntity(0)->setVisible(true);
			mGizmoEntities[4]->getSubEntity(0)->setVisible(true);
			mGizmoEntities[5]->getSubEntity(0)->setVisible(true);
			break;
		}
	}
}
//-----------------------------------------------------------------------------------------
void AxisGizmo::DestroyGizmo()
{
	if(!mGizmoNode) 
		return;

	for(int i= 0;i < 6;i++)
	{
		mGizmoEntities[i]->detachFromParent();
		mGizmoEntities[i]->_getManager()->destroyEntity(mGizmoEntities[i]);
	}

	mGizmoNode->removeAndDestroyChild(mGizmoX->getName());
	mGizmoNode->removeAndDestroyChild(mGizmoY->getName());
	mGizmoNode->removeAndDestroyChild(mGizmoZ->getName());

	mGizmoNode->getParentSceneNode()->removeAndDestroyChild(mGizmoNode->getName());
	mGizmoNode = 0;
	mGizmoX = 0;
	mGizmoY = 0;
	mGizmoZ = 0;
	mGizmoEntities[0] = mGizmoEntities[1] = mGizmoEntities[2] = mGizmoEntities[3] = mGizmoEntities[4] = mGizmoEntities[5] = 0;
}

//-----------------------------------------------------------------------------------------
void AxisGizmo::createMesh(Ogre::SceneManager *manager, Ogre::String name)
{
    Ogre::ManualObject *mMesh = manager->createManualObject("OgitorAxisGizmoManualObject");

    mMesh->begin("AxisGizmo_Material", RenderOperation::OT_LINE_LIST);
    mMesh->position(0, 0, 0);
    mMesh->position(3, 0, 0);

    mMesh->index(0);
    mMesh->index(1);
    mMesh->end();

    float const radius = 0.22f;
    float const accuracy = 8;
    float MPI = Math::PI;

    float division = (MPI / 2.0f) / 16.0f;
    float start = division * 3;
    float end = division * 14;


    int index_pos = 0;

    mMesh->begin("AxisGizmo_Material", RenderOperation::OT_LINE_STRIP);

    for(float theta = start; theta < end; theta += division) 
    {
        mMesh->position(0, 3.0f * cos(theta), 3.0f * sin(theta));
        mMesh->index(index_pos++);
    }

    mMesh->end();

    mMesh->begin("AxisGizmo_Material", RenderOperation::OT_TRIANGLE_LIST);

    mMesh->position(2.85f,     0,     0);

    for(float theta = 0; theta < 2 * MPI; theta += MPI / accuracy) 
    {
        mMesh->position(2.95f, radius * cos(theta), radius * sin(theta));
    }
    mMesh->position(3.45f,     0,     0);

    for(int inside = 1;inside < 16;inside++)
    {
        mMesh->index(0);
        mMesh->index(inside);
        mMesh->index(inside + 1);
    }
    mMesh->index(0);
    mMesh->index(16);
    mMesh->index(1);

    for(int outside = 1;outside < 16;outside++)
    {
        mMesh->index(17);
        mMesh->index(outside);
        mMesh->index(outside + 1);
    }
    mMesh->index(17);
    mMesh->index(16);
    mMesh->index(1);
    
    mMesh->end();

    //ROTATE GIZMO

    mMesh->begin("AxisGizmo_Material", RenderOperation::OT_TRIANGLE_LIST);

    Quaternion q1;
    q1.FromAngleAxis(Degree(-90), Ogre::Vector3(0,0,1));
    Quaternion q2;
    q2.FromAngleAxis(Degree(90), Ogre::Vector3(0,1,0));
    
    Ogre::Vector3 translate1(0, 3.0f * cos(end), 3.0f * sin(end));
    Ogre::Vector3 translate2(0, 3.0f * cos(start), 3.0f * sin(start) - 0.25f);

    Ogre::Vector3 pos(-0.3f,     0,     0);
    mMesh->position(q1 * pos + translate1);

    for(float theta = 0; theta < 2 * MPI; theta += MPI / accuracy) 
    {
        pos = Ogre::Vector3(-0.3f, radius * cos(theta), radius * sin(theta));
        mMesh->position(q1 * pos + translate1);
    }
    pos = Ogre::Vector3(0.3f, 0 , 0);
    mMesh->position(q1 * pos + translate1);

    pos = Ogre::Vector3(-0.3f,     0,     0);
    mMesh->position(q2 * pos + translate2);

    for(float theta = 0; theta < 2 * MPI; theta += MPI / accuracy) 
    {
        pos = Ogre::Vector3(-0.3f, radius * cos(theta), radius * sin(theta));
        mMesh->position(q2 * pos + translate2);
    }
    pos = Ogre::Vector3(0.3f, 0 , 0);
    mMesh->position(q2 * pos + translate2);

    for(int inside = 1;inside < 16;inside++)
    {
        mMesh->index(0);
        mMesh->index(inside);
        mMesh->index(inside + 1);
    }
    mMesh->index(0);
    mMesh->index(16);
    mMesh->index(1);

    for(int outside = 1;outside < 16;outside++)
    {
        mMesh->index(17);
        mMesh->index(outside);
        mMesh->index(outside + 1);
    }
    mMesh->index(17);
    mMesh->index(16);
    mMesh->index(1);
    
    for(int inside = 19;inside < 34;inside++)
    {
        mMesh->index(18);
        mMesh->index(inside);
        mMesh->index(inside + 1);
    }
    mMesh->index(18);
    mMesh->index(34);
    mMesh->index(19);

    for(int outside = 19;outside < 34;outside++)
    {
        mMesh->index(35);
        mMesh->index(outside);
        mMesh->index(outside + 1);
    }
    mMesh->index(35);
    mMesh->index(34);
    mMesh->index(19);

    mMesh->end();

    //SCALE GIZMO

    mMesh->begin("AxisGizmo_Material", RenderOperation::OT_TRIANGLE_LIST);

    mMesh->position(2.85f,     0,     0);

    for(float theta = 0; theta < 2 * MPI; theta += MPI / accuracy) 
    {
        mMesh->position(2.85f, radius * cos(theta), radius * sin(theta));
    }
    mMesh->position(3.45f,     0,     0);

    mMesh->position(3.40f,  0.20f,  0.20f);
    mMesh->position(3.40f,  0.20f, -0.20f);
    mMesh->position(3.40f, -0.20f, -0.20f);
    mMesh->position(3.40f, -0.20f,  0.20f);
    mMesh->position(3.50f,  0.20f,  0.20f);
    mMesh->position(3.50f,  0.20f, -0.20f);
    mMesh->position(3.50f, -0.20f, -0.20f);
    mMesh->position(3.50f, -0.20f,  0.20f);

    for(int inside = 1;inside < 16;inside++)
    {
        mMesh->index(0);
        mMesh->index(inside);
        mMesh->index(inside + 1);
    }
    mMesh->index(0);
    mMesh->index(16);
    mMesh->index(1);

    for(int outside = 1;outside < 16;outside++)
    {
        mMesh->index(17);
        mMesh->index(outside);
        mMesh->index(outside + 1);
    }
    mMesh->index(17);
    mMesh->index(16);
    mMesh->index(1);

    mMesh->index(18);
    mMesh->index(19);
    mMesh->index(20);
    mMesh->index(18);
    mMesh->index(20);
    mMesh->index(21);

    mMesh->index(22);
    mMesh->index(23);
    mMesh->index(24);
    mMesh->index(22);
    mMesh->index(24);
    mMesh->index(25);

    mMesh->index(18);
    mMesh->index(22);
    mMesh->index(25);
    mMesh->index(18);
    mMesh->index(25);
    mMesh->index(21);

    mMesh->index(19);
    mMesh->index(23);
    mMesh->index(24);
    mMesh->index(19);
    mMesh->index(24);
    mMesh->index(20);

    mMesh->index(18);
    mMesh->index(22);
    mMesh->index(23);
    mMesh->index(18);
    mMesh->index(23);
    mMesh->index(19);

    mMesh->index(21);
    mMesh->index(20);
    mMesh->index(24);
    mMesh->index(21);
    mMesh->index(24);
    mMesh->index(25);

    mMesh->end();

    mMesh->convertToMesh(name);

    manager->destroyManualObject(mMesh);
}

void AxisGizmo::createPlaneMesh(Ogre::SceneManager *manager, Ogre::String name)
{
    Ogre::ManualObject *mMesh = manager->createManualObject("OgitorAxisPlaneGizmoManualObject");

    mMesh->begin("AxisGizmo_Material", RenderOperation::OT_TRIANGLE_LIST);

    mMesh->position( 0, 1, 0);
    mMesh->position( 1, 1, 0);
    mMesh->position( 1, 0, 0);
    mMesh->position( 0, 0, 0);

    mMesh->index(0);
    mMesh->index(1);
    mMesh->index(2);
    mMesh->index(0);
    mMesh->index(2);
    mMesh->index(3);

    mMesh->end();

    mMesh->convertToMesh(name);

    manager->destroyManualObject(mMesh);
}

//-----------------------------------------------------------------------------------------
void AxisGizmo::GetMeshDataEx(const Ogre::Entity *entity, size_t &vertex_count, size_t &index_count,
								 const Ogre::Vector3 &position, const Ogre::Quaternion &orient, const Ogre::Vector3 &scale)
{
	bool added_shared = false;
	size_t current_offset = 0;
	size_t shared_offset = 0;
	size_t next_offset = 0;
	size_t index_offset = 0;

	vertex_count = index_count = 0;

	Ogre::MeshPtr mesh = entity->getMesh();

	// Calculate how many vertices and indices we're going to need
	for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		Ogre::SubMesh* submesh = mesh->getSubMesh( i );

		// We only need to add the shared vertices once
		if(submesh->useSharedVertices)
		{
			if( !added_shared )
			{
				vertex_count += mesh->sharedVertexData->vertexCount;
				added_shared = true;
			}
		}
		else
		{
			vertex_count += submesh->vertexData->vertexCount;
		}

		// Add the indices
		index_count += submesh->indexData->indexCount;
	}


	if(vertex_count > mVertexBufferSize)
	{
		OGRE_FREE(mVertexBuffer, Ogre::MEMCATEGORY_GEOMETRY);
		mVertexBuffer = OGRE_ALLOC_T(Ogre::Vector3,vertex_count, Ogre::MEMCATEGORY_GEOMETRY);
		mVertexBufferSize = vertex_count;
	}

	if(index_count > mIndexBufferSize)
	{
		OGRE_FREE(mIndexBuffer, Ogre::MEMCATEGORY_GEOMETRY);
		mIndexBuffer = OGRE_ALLOC_T(unsigned long,index_count, Ogre::MEMCATEGORY_GEOMETRY);
		mIndexBufferSize = index_count;
	}

	added_shared = false;

	// Run through the submeshes again, adding the data into the arrays
	for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	{
		if(!entity->getSubEntity(i)->isVisible())
			continue;

		Ogre::SubMesh* submesh = mesh->getSubMesh(i);

		Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

		if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
		{
			if(submesh->useSharedVertices)
			{
				added_shared = true;
				shared_offset = current_offset;
			}

			const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

			Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

			unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

			// There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
			//  as second argument. So make it float, to avoid trouble when Ogre::Real will
			//  be comiled/typedefed as double:
			//      Ogre::Real* pReal;
			float* pReal;

			for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
			{
				posElem->baseVertexPointerToElement(vertex, &pReal);

				Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

				mVertexBuffer[current_offset + j] = (orient * (pt * scale)) + position;
			}

			vbuf->unlock();
			next_offset += vertex_data->vertexCount;
		}


		Ogre::IndexData* index_data = submesh->indexData;
		size_t numTris = index_data->indexCount / 3;
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

		bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

		unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);

		size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

		if ( use32bitindexes )
		{
			for ( size_t k = 0; k < numTris*3; ++k)
			{
				mIndexBuffer[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
			}
		}
		else
		{
			for ( size_t k = 0; k < numTris*3; ++k)
			{
				mIndexBuffer[index_offset++] = static_cast<unsigned long>(pShort[k]) + static_cast<unsigned long>(offset);
			}
		}

		ibuf->unlock();
		current_offset = next_offset;
	}
	index_count = index_offset;
}

//-----------------------------------------------------------------------------------------
bool AxisGizmo::PickGizmos(Ogre::Ray &ray, int &Axis)
{
	if(!mGizmoEntities[0] || !mGizmoEntities[0]->isVisible())
	{
		Axis = 0;
		return false;
	}

	Axis = 0;
	Ogre::Real closest_distance = -1.0f;
	Ogre::Vector3 closest_result;

	for(int widx = 0;widx < 6;widx++)
	{
		// get the entity to check
		Ogre::Entity *pentity = mGizmoEntities[widx];

		std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray, pentity->getWorldBoundingBox());
		if(!hit.first)
			continue;

		size_t vertex_count;
		size_t index_count;
		Ogre::Vector3 *vertices;
		unsigned long *indices;

		// get the mesh information
		GetMeshDataEx(pentity, vertex_count, index_count, 
			pentity->getParentNode()->_getDerivedPosition(),
			pentity->getParentNode()->_getDerivedOrientation(),
			pentity->getParentNode()->_getDerivedScale());

		GetBuffers(&vertices, &indices);
		// test for hitting individual triangles on the mesh
		bool new_closest_found = false;
		for (int i = 0; i < static_cast<int>(index_count); i += 3)
		{
			// check for a hit against this triangle
			hit = Ogre::Math::intersects(ray, vertices[indices[i]],
				vertices[indices[i+1]], vertices[indices[i+2]], true, true);
			// if it was a hit check if its the closest
			if (hit.first)
			{
				if ((closest_distance < 0.0f) || (hit.second < closest_distance))
				{
					// this is the closest so far, save it off
					closest_distance = hit.second;
					new_closest_found = true;
				}
			}
		}

		// if we found a new closest raycast for this object, update the
		// closest_result before moving on to the next object.
		if (new_closest_found)
		{
			closest_result = ray.getPoint(closest_distance);
			switch(widx)
			{
			case 0:
				Axis = AXIS_X;
				break;
			case 1:
				Axis = AXIS_Y;
				break;
			case 2:
				Axis = AXIS_Z;
				break;
			case 3:
				Axis = AXIS_XY;
				break;
			case 4:
				Axis = AXIS_YZ;
				break;
			case 5:
				Axis = AXIS_XZ;
				break;
			}
		}
	}

	return (closest_distance >= 0.0f);
}

void AxisGizmo::setOrientation( const Quaternion& q )
{
	mGizmoNode->setOrientation(q);
}

const Quaternion & AxisGizmo::getOrientation() const
{
	return mGizmoNode->getOrientation();
}
