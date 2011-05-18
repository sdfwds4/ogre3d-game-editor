

#pragma once

#include "MovableText.h"

/** The Axis enumeration */
enum AXISTYPE 
{
	AXIS_X = 1,     /** X Axis */
	AXIS_Y = 2,     /** Y Axis */
	AXIS_XY = 3,    /** XY Axis */
	AXIS_Z = 4,     /** Z Axis */
	AXIS_XZ = 5,    /** XZ Axis */
	AXIS_YZ = 6,    /** YZ Axis */
	AXIS_ALL = 7    /** XYZ Axis */
};

using namespace Ogre;

//! Object-oriented axis gizmo class
/*!  
An object-oriented axis gizmo class implementation
*/
class AxisGizmo
{
public:
    /**
    * Constructor
    */
    AxisGizmo();
    /**
    * Destructor
	*/
	virtual ~AxisGizmo();

	/**
	* Creates gizmo objects
	*/
	void CreateGizmo();
	/**
	* Destroys gizmo objects
	* @return
	*/
	void DestroyGizmo();
	/**
	* Updates position of gizmo objects
	* @return
	*/
	void UpdateGizmo();

	/**
	* Highlights gizmo(s)
	* @param ID gizmo axis(i) flag(s) to highlight 
	* @see AXISTYPE
	*/
	void HighlightGizmo(int ID);
	/**
	* Sets new gizmo mode 
	* @param mode new gizmo mode flag(s) 
	* @see EDITORTOOLS
	*/
	void SetGizmoMode(int mode);

	/**
	* Sets Gizmo scaling factor
	* @param names of auto track targets in the scene
	*/
	void SetGizmoScale(Real value);

	/**
	* Fetches gizmo scaling factor
	* @return gizmo's current scaling factor
	*/
	Real GetGizmoScale() {return mGizmoScale;};

	void setPosition( Vector3& pos )
	{
		mGizmoNode->setPosition(pos);
	}

	void setVisible( bool bShow )
	{
		mGizmoNode->setVisible(bShow);
	}

	/**
	* Attempts to pick X, Y or Z axis gizmo(s) presented when object is moved, scaled etc
	* @param ray ray cast from the mouse  
	* @see COgitorsRoot::GetMouseRay
	* @param Axis axis of the selected gizmo
	* @return true if intersected with any of gizmo(s), otherwise false
	*/
	bool PickGizmos(Ogre::Ray &ray, int &Axis);

	SceneManager * GetSceneManager()
	{
		return mSceneMgr;
	}

	void setOrientation( const Quaternion& q );
	const Quaternion & getOrientation() const;
	/**
    * Sets up gizmos
    */
    void createMesh(Ogre::SceneManager *manager, Ogre::String name);
    /**
    * Sets up plane gizmos
	*/
	void createPlaneMesh(Ogre::SceneManager *manager, Ogre::String name);

protected:
	SceneManager* mSceneMgr;
	Real          mGizmoScale;                                /** Gizmo scaling factor */
	SceneNode    *mGizmoNode;                                 /** Gizmo widget handle */
	SceneNode    *mGizmoX;                                    /** X axis widget node handle */
	SceneNode    *mGizmoY;                                    /** Y axis widget node handle */
	SceneNode    *mGizmoZ;                                    /** Z axis widget node handle */
	SceneNode    *mTextX;                                    /** X axis Text */
	SceneNode    *mTextY;                                    /** Y axis Text */
	SceneNode    *mTextZ;                                    /** Z axis Text */
	Entity       *mGizmoEntities[6];                          /** Gizmo handles */
	bool         mWorldSpaceGizmoOrientation;                /** Is the Gizmo Orientation in World Space? */ 
	int          mOldGizmoMode;                              /** Previous gizmo mode (@see EDITORTOOLS) */
	int          mOldGizmoAxis;                              /** Previous gizmo axis along which transformation has occured */

	MovableText* mAxisText[3];
	/**
	* Fetches information about OGRE-based mesh
	* @param entity entity to retrieve information about
	* @param vertex_count number of vertices to retrieve
	* @param vertices raw Ogre::Vector3 array pointer to mesh' vertices
	* @param index_count number of indices to retrieve
	* @param indices raw ulong array pointer to mesh' indices
	* @param position mesh position to retrieve
	* @param orient mesh orientation to retrieve
	* @param scale mesh scale to retrieve
	*/
	void GetMeshDataEx(const Ogre::Entity *entity, size_t &vertex_count, size_t &index_count,
		const Ogre::Vector3 &position, const Ogre::Quaternion &orient, const Ogre::Vector3 &scale);

	void GetBuffers(Ogre::Vector3 **VertexBuffer, unsigned long **IndexBuffer)
	{
		*VertexBuffer = mVertexBuffer;
		*IndexBuffer = mIndexBuffer;
	}

	unsigned int   mVertexBufferSize;
	unsigned int   mIndexBufferSize;
	Ogre::Vector3 *mVertexBuffer;
	unsigned long *mIndexBuffer;
};

