

#include "stdafx.h"
#include "PhysicsManager.h"
#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldShape.h>
#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldBaseCinfo.h>
#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>
#include <Common/Base/Ext/hkBaseExt.h>

#include "Editor.h"

class MySampledHeightFieldShape: public hkpSampledHeightFieldShape
{
public:
	MySampledHeightFieldShape( const hkpSampledHeightFieldBaseCinfo& ci, hkUint16* data )
		:	hkpSampledHeightFieldShape(ci),
		m_data(data)
	{
	}

	// Generate a rough terrain
	HK_FORCE_INLINE hkReal getHeightAtImpl( int x, int z ) const
	{
		// Lookup data and return a float
		// We scale the data artificially by 5 to make it look interesting
		return hkReal(m_data[x * m_zRes + z]);
	}

	// Assuming each heightfield quad is defined as four points { 00, 01, 11, 10 },
	// this should return true if the two triangles share the edge p00-p11.
	// Otherwise it should return false if the triangles share the edge p01-p10
	HK_FORCE_INLINE hkBool getTriangleFlipImpl() const
	{	
		return false;
	}

	virtual void collideSpheres( const CollideSpheresInput& input, SphereCollisionOutput* outputArray) const
	{
		hkSampledHeightFieldShape_collideSpheres(*this, input, outputArray);
	}

private:

	hkUint16* m_data;
};

static void HK_CALL errorReport(const char* msg, void* userArgGivenToInit)
{
	CString strLog;
	strLog = msg;
	GetEditor()->Log(strLog);
}

template<> CPhysicsManager* Singleton<CPhysicsManager>::ms_Singleton = 0;
CPhysicsManager* CPhysicsManager::getSingletonPtr(void)
{
	return ms_Singleton;
}
CPhysicsManager& CPhysicsManager::getSingleton(void)
{  
	assert( ms_Singleton );  return ( *ms_Singleton );  
}

CPhysicsManager::CPhysicsManager() : m_heightData(NULL), last_time(0)
			, threadPool(NULL), jobQueue(NULL), physicsWorld(NULL)
			, context(NULL), vdb(NULL)
{

}

CPhysicsManager::~CPhysicsManager()
{

}

void CPhysicsManager::Create()
{
	timestep = 1.0f / 60.0f;

	hkMemoryRouter* memoryRouter = hkMemoryInitUtil::initDefault(hkMallocAllocator::m_defaultMallocAllocator, hkMemorySystem::FrameInfo(6*1024*1024));

	extAllocator::initDefault();

	if ( hkBaseSystem::init( memoryRouter, errorReport ) != HK_SUCCESS)
		return;

	{
		// We can cap the number of threads used - here we use the maximum for whatever multithreaded platform we are running on. This variable is
		// set in the following code sections.
		int totalNumThreadsUsed;

		// Get the number of physical threads available on the system
		hkHardwareInfo hwInfo;
		hkGetHardwareInfo(hwInfo);
		totalNumThreadsUsed = hwInfo.m_numThreads;

		// We use one less than this for our thread pool, because we must also use this thread for our simulation
		hkCpuJobThreadPoolCinfo threadPoolCinfo;
		threadPoolCinfo.m_numThreads = totalNumThreadsUsed - 1;

		// This line enables timers collection, by allocating 200 Kb per thread.  If you leave this at its default (0),
		// timer collection will not be enabled.
		threadPoolCinfo.m_timerBufferPerThreadAllocation = 200000;
		threadPool = new hkCpuJobThreadPool( threadPoolCinfo );

		// We also need to create a Job queue. This job queue will be used by all Havok modules to run multithreaded work.
		// Here we only use it for physics.
		hkJobQueueCinfo info;
		info.m_jobQueueHwSetup.m_numCpuThreads = totalNumThreadsUsed;
		jobQueue = new hkJobQueue(info);

		// Monitors have been enabled for thread pool threads already (see above comment).
		hkMonitorStream::getInstance().resize(200000);

		{
			// The world cinfo contains global simulation parameters, including gravity, solver settings etc.
			hkpWorldCinfo worldInfo;
			worldInfo.setBroadPhaseWorldSize( 100.0f );
			worldInfo.m_collisionTolerance = 0.03f;
			physicsWorld = new hkpWorld(worldInfo);

			// Disable deactivation, so that you can view timers in the VDB. This should not be done in your game.
			physicsWorld->m_wantDeactivation = false;

			// When the simulation type is SIMULATION_TYPE_MULTITHREADED, in the debug build, the sdk performs checks
			// to make sure only one thread is modifying the world at once to prevent multithreaded bugs. Each thread
			// must call markForRead / markForWrite before it modifies the world to enable these checks.
			physicsWorld->markForWrite();

			// Register all collision agents, even though only box - box will be used in this particular example.
			// It's important to register collision agents before adding any entities to the world.
			hkpAgentRegisterUtil::registerAllAgents( physicsWorld->getCollisionDispatcher() );

			// We need to register all modules we will be running multi-threaded with the job queue
			physicsWorld->registerWithJobQueue( jobQueue );

			// Create all the physics rigid bodies
			//setupPhysics( physicsWorld );
		}

		{
			context = new hkpPhysicsContext();
			hkpPhysicsContext::registerAllPhysicsProcesses(); // all the physics viewers
			context->addWorld(physicsWorld); // add the physics world so the viewers can see it
			contexts.pushBack(context);

			// Now we have finished modifying the world, release our write marker.
			physicsWorld->unmarkForWrite();
		}

		vdb = new hkVisualDebugger(contexts);
		vdb->serve();

		last_time = Ogre::Root::getSingletonPtr()->getTimer()->getMilliseconds();
	}
}

void CPhysicsManager::Destroy()
{
	if( m_heightData )
		hkDeallocate<hkUint16>( m_heightData );
	// <PHYSICS-ONLY>: cleanup physics
	{
		physicsWorld->markForWrite();
		physicsWorld->removeReference();
	}
	vdb->removeReference();


	// Contexts are not reference counted at the base class level by the VDB as
	// they are just interfaces really. So only delete the context after you have
	// finished using the VDB.
	context->removeReference();

	delete jobQueue;

	hkMonitorStream::getInstance().quit();

	threadPool->removeReference();
}

hkpWorld* CPhysicsManager::GetPhysicsWorld() const
{
	return physicsWorld;
}

void CPhysicsManager::Update()
{
	unsigned long now_time = Ogre::Root::getSingletonPtr()->getTimer()->getMilliseconds();

	if ( now_time - last_time > timestep*1000.0f )
	{
		physicsWorld->stepMultithreaded( jobQueue, threadPool, timestep );

		// Step the visual debugger. We first synchronize the timer data
		context->syncTimers( threadPool );
		vdb->step();

		last_time = now_time;
	}
}

void CPhysicsManager::SetHeightfieldData()
{
	physicsWorld->lock();
	// Here we create an array of shorts that our heightfield will lookup
	// This array could easily have been loaded from a texture.
	Terrain* pTerrain = GetEditor()->GetTerrainGroup()->getTerrain(0,0);
	const int xRes = pTerrain->getSize();
	const int zRes = pTerrain->getSize();
	m_heightData = hkAllocate<hkUint16>(xRes * zRes, HK_MEMORY_CLASS_DEMO);
	{
		for (int x = 0; x < xRes; x++)
		{
			for (int z = 0; z < zRes; z++)
			{
				// Convert to 16 bit 
				m_heightData[x*zRes + z] = static_cast<hkUint16> ( pTerrain->getHeightAtPoint( x, zRes - z ) );
			}
		}
	}

	{
		hkpSampledHeightFieldBaseCinfo ci;
		ci.m_xRes = xRes;
		ci.m_zRes = zRes;
		hkReal scale = pTerrain->getWorldSize() /(pTerrain->getSize() - 1);
		ci.m_scale = hkVector4( scale, 1.0f, scale );

		MySampledHeightFieldShape* heightFieldShape = new MySampledHeightFieldShape( ci , m_heightData);

		// Now that we have a shape all we need is the fixed body to represent it in the 
		// the simulation. Standard rigid body setup.
		{
			hkpRigidBodyCinfo rci;
			rci.m_motionType = hkpMotion::MOTION_FIXED;
			rci.m_position.setMul4( -0.5f, heightFieldShape->m_extents ); // center the heightfield
			rci.m_shape = heightFieldShape;
			rci.m_friction = 0.2f;

			hkpRigidBody* body = new hkpRigidBody( rci );

			physicsWorld->addEntity(body);
			body->removeReference();
		}

		// Just need to remove the reference we hold to the shape, and that is it.
		heightFieldShape->removeReference();
	}

	physicsWorld->unlock();

}
