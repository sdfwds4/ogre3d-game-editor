

#pragma once

using namespace Ogre;

static void HK_CALL errorReport(const char* msg, void* userArgGivenToInit);

class CPhysicsManager : public Singleton<CPhysicsManager>
{
public:
	CPhysicsManager();
	~CPhysicsManager();

	void Create();
	void Destroy();
	void Update();
	void SetHeightfieldData();

	hkpWorld* GetPhysicsWorld() const;

	static CPhysicsManager& getSingleton(void);
	static CPhysicsManager* getSingletonPtr(void);

	hkMallocAllocator mallocBase;
	hkJobThreadPool* threadPool;
	hkJobQueue* jobQueue;
	hkpWorld* physicsWorld;
	hkArray<hkProcessContext*> contexts;
	hkpPhysicsContext* context;
	hkVisualDebugger* vdb;

private:

	hkUint16* m_heightData;
	unsigned long last_time;
	hkReal timestep;
};