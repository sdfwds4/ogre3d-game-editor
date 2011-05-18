

#include "stdafx.h"
#include "Editor.h"
#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>
#include <Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>
#include <Physics/Collide/Filter/Group/hkpGroupFilter.h>
#include <Physics/Utilities/Collide/Filter/GroupFilter/hkpGroupFilterUtil.h>

#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Dynamics/Phantom/hkpSimpleShapePhantom.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h>
#include <Physics/Collide/Query/Collector/PointCollector/hkpAllCdPointCollector.h>
#include <Physics/ConstraintSolver/Simplex/hkpSimplexSolver.h>
#include <Physics/Collide/Query/CastUtil/hkpLinearCastInput.h>
#include "NpcEntity.h"


#define HK_OBJECT_IS_LADDER 1234

class MyCharacterListener : public hkReferencedObject, public hkpCharacterProxyListener
{
public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO);

	MyCharacterListener( ) 
		: m_atLadder(false)
	{}

	// Ladder handling code goes here
	void contactPointAddedCallback( const hkpCharacterProxy* proxy, const hkpRootCdPoint& point )
	{
		hkpRigidBody* body = hkpGetRigidBody(point.m_rootCollidableB);

		if ( body->hasProperty(HK_OBJECT_IS_LADDER) )
		{
			m_atLadder = true;
			m_ladderNorm = point.m_contact.getNormal();
			body->getPointVelocity(point.m_contact.getPosition(), m_ladderVelocity);
		}
	}

	void contactPointRemovedCallback( const hkpCharacterProxy* proxy, const hkpRootCdPoint& point)
	{
		hkpRigidBody* body = hkpGetRigidBody(point.m_rootCollidableB);

		if ( body->hasProperty(HK_OBJECT_IS_LADDER) )
		{
			m_atLadder = false;
		}
	}

public:

	hkBool m_atLadder;

	hkVector4 m_ladderNorm;

	hkVector4 m_ladderVelocity;

};

//-----------------------------------------------------------------------
// Local class for updating based on time
class NpcUpdateValue : public ControllerValue<Real>
{
protected:
	CNpc* mTarget;
public:
	NpcUpdateValue(CNpc* target) : mTarget(target) {}

	Real getValue(void) const { return 0; } // N/A

	void setValue(Real value) { mTarget->Updaye(value); }

};

CNpc::CNpc()
	:mTimeController(NULL), 
	mBodyNode(NULL), 
	mBodyEnt(NULL),
	mpLight(NULL),
	mpSceneMgr(NULL),
	m_timestep(0.016f)
{

}

CNpc::~CNpc()
{

}

void CNpc::Create()
{
	mpSceneMgr = GetEditor()->GetSceneManager();
	// create main model
	mBodyNode = mpSceneMgr->getRootSceneNode()->createChildSceneNode(Vector3::UNIT_Y * CHAR_HEIGHT);
	mBodyEnt = mpSceneMgr->createEntity("SinbadBody", "Sinbad.mesh");
	mBodyNode->attachObject(mBodyEnt);

	mpLight = mpSceneMgr->createLight();
	mpLight->setPosition(0.0f, 160.0f, 0.0f);
	mBodyNode->attachObject(mpLight);


	// this is very important due to the nature of the exported animations
	mBodyEnt->getSkeleton()->setBlendMode(ANIMBLEND_CUMULATIVE);

	String animNames[] =
	{"IdleBase", "IdleTop", "RunBase", "RunTop", "HandsClosed", "HandsRelaxed", "DrawSwords",
	"SliceVertical", "SliceHorizontal", "Dance", "JumpStart", "JumpLoop", "JumpEnd"};

	// populate our animation list
	for (int i = 0; i < NUM_ANIMS; i++)
	{
		mAnims[i] = mBodyEnt->getAnimationState(animNames[i]);
		mAnims[i]->setLoop(true);
		mFadingIn[i] = false;
		mFadingOut[i] = false;
	}

	// start off in the idle state (top and bottom together)
	setBaseAnimation(ANIM_IDLE_BASE);
	setTopAnimation(ANIM_IDLE_TOP);

	// relax the hands since we're not holding anything
	mAnims[ANIM_HANDS_RELAXED]->setEnabled(true);

	mGoalPosition = Vector3::ZERO;
	mVerticalVelocity = 0;

	mShadowDecal.Create( "DecalShadow", "DecalShadow" );
	mShadowDecal.SetScale(2.0f);

	// Create controller
	if (!mTimeController)
	{
		Ogre::ControllerManager& controllerManager = Ogre::ControllerManager::getSingleton(); 
		Ogre::ControllerValueRealPtr npcUpdateValue(OGRE_NEW NpcUpdateValue(this));
		mTimeController = controllerManager.createFrameTimePassthroughController(npcUpdateValue);
	}

	CreateCharacterProxy();
}

void CNpc::setGoalDirection(Vector3& pos)
{
	mGoalPosition = pos;
	mGoalPosition.y += CHAR_HEIGHT;

	if (!mGoalPosition.isZeroLength() && mBaseAnimID == ANIM_IDLE_BASE)
	{
		// start running if not already moving and the player wants to move
		setBaseAnimation(ANIM_RUN_BASE, true);
		if (mTopAnimID == ANIM_IDLE_TOP) 
			setTopAnimation(ANIM_RUN_TOP, true);
	}
}

void CNpc::setPosition( Vector3& pos, bool fixCamera )
{
	pos.y += CHAR_HEIGHT;
	mBodyNode->setPosition(pos);
	mShadowDecal.setPosition( pos.x, pos.z );
	if ( fixCamera )
	{
		Camera* pCamera = GetEditor()->GetCamera();
		pCamera->setPosition( pos.x + 100.0f, pos.y + 100.0f, pos.z + 100.0f );
		pCamera->lookAt(mBodyNode->getPosition());
	}
}

void CNpc::setBaseAnimation(AnimID id, bool reset )
{
	if (mBaseAnimID >= 0 && mBaseAnimID < NUM_ANIMS)
	{
		// if we have an old animation, fade it out
		mFadingIn[mBaseAnimID] = false;
		mFadingOut[mBaseAnimID] = true;
	}

	mBaseAnimID = id;

	if (id != ANIM_NONE)
	{
		// if we have a new animation, enable it and fade it in
		mAnims[id]->setEnabled(true);
		mAnims[id]->setWeight(0);
		mFadingOut[id] = false;
		mFadingIn[id] = true;
		if (reset) 
			mAnims[id]->setTimePosition(0);
	}
}

void CNpc::setTopAnimation(AnimID id, bool reset )
{
	if (mTopAnimID >= 0 && mTopAnimID < NUM_ANIMS)
	{
		// if we have an old animation, fade it out
		mFadingIn[mTopAnimID] = false;
		mFadingOut[mTopAnimID] = true;
	}

	mTopAnimID = id;

	if (id != ANIM_NONE)
	{
		// if we have a new animation, enable it and fade it in
		mAnims[id]->setEnabled(true);
		mAnims[id]->setWeight(0);
		mFadingOut[id] = false;
		mFadingIn[id] = true;
		if (reset) 
			mAnims[id]->setTimePosition(0);
	}
}

//-----------------------------------------------------------------------
void CNpc::Updaye(Ogre::Real deltaTime)
{
	updatePhysics();
	updateBody(deltaTime);
	updateAnimations(deltaTime);
}

void CNpc::updatePhysics()
{
	hkVector4 up;
	hkQuaternion orient;

	m_world->lock();

	//	Get user input data
	int m_upAxisIndex = 2;
	up.setZero4();
	up(m_upAxisIndex) = 1;

	hkReal posX = 0.f;
	hkReal posY = 0.f;
	{
		float deltaAngle = 0.f;
		//CharacterUtils::getUserInputForCharacter(m_env, deltaAngle, posX, posY);
		m_currentAngle += deltaAngle;
		orient.setAxisAngle(up, m_currentAngle);
	}

	hkpCharacterInput input;
	hkpCharacterOutput output;
	{
		input.m_inputLR = posX;
		input.m_inputUD = posY;

		input.m_wantJump = false;
		input.m_atLadder = m_listener->m_atLadder;

		input.m_up = up;
		input.m_forward.set(1,0,0);
		input.m_forward.setRotatedDir( orient, input.m_forward );

		input.m_stepInfo.m_deltaTime = m_timestep;
		input.m_stepInfo.m_invDeltaTime = 1.0f / m_timestep;
		input.m_characterGravity.set(0,0,-16);
		input.m_velocity = m_characterProxy->getLinearVelocity();
		input.m_position = m_characterProxy->getPosition();

		hkVector4 down;	down.setNeg4(up);
		m_characterProxy->checkSupport(down, input.m_surfaceInfo);

		// Only climb the ladder when the character is either unsupported or wants to go up.
		if ( m_listener->m_atLadder && ( ( input.m_inputUD < 0 ) || ( input.m_surfaceInfo.m_supportedState != hkpSurfaceInfo::SUPPORTED ) ) )
		{
			hkVector4 right, ladderUp;
			right.setCross( up, m_listener->m_ladderNorm );
			ladderUp.setCross( m_listener->m_ladderNorm, right );
			// Calculate the up vector for the ladder
			if (ladderUp.lengthSquared3() > HK_REAL_EPSILON)
			{
				ladderUp.normalize3();
			}

			// Reorient the forward vector so it points up along the ladder
			input.m_forward.addMul4( -m_listener->m_ladderNorm.dot3(input.m_forward), m_listener->m_ladderNorm);
			input.m_forward.add4( ladderUp );
			input.m_forward.normalize3();

			input.m_surfaceInfo.m_supportedState = hkpSurfaceInfo::UNSUPPORTED;
			input.m_surfaceInfo.m_surfaceNormal = m_listener->m_ladderNorm;
			input.m_surfaceInfo.m_surfaceVelocity = m_listener->m_ladderVelocity;
		}
	}

	// Apply the character state machine
	{
		HK_TIMER_BEGIN( "update character state", HK_NULL );

		m_characterContext->update(input, output);

		HK_TIMER_END();
	}

	//Apply the player character controller
	{
		HK_TIMER_BEGIN( "simulate character", HK_NULL );

		// Feed output from state machine into character proxy
		m_characterProxy->setLinearVelocity(output.m_velocity);

		hkStepInfo si;
		si.m_deltaTime = m_timestep;
		si.m_invDeltaTime = 1.0f/m_timestep;
		m_characterProxy->integrate( si, m_world->getGravity() );

		HK_TIMER_END();
	}

	m_world->unlock();
}

//-----------------------------------------------------------------------
void CNpc::updateBody(Real deltaTime)
{
	if (mGoalPosition != Vector3::ZERO && mBaseAnimID != ANIM_DANCE)
	{
		// calculate actually goal direction in world based on player's key directions
		Vector3 goalPos = mGoalPosition;
		Vector3 pos = mBodyNode->getPosition();
		goalPos -= pos;
		Real distance;
		distance = goalPos.normalise();

		Quaternion toGoal = mBodyNode->getOrientation().zAxis().getRotationTo(goalPos);

		// calculate how much the character has to turn to face goal direction
		Real yawToGoal = toGoal.getYaw().valueDegrees();
		// this is how much the character CAN turn this frame
		Real yawAtSpeed = yawToGoal / Math::Abs(yawToGoal) * deltaTime * TURN_SPEED;
		// reduce "turnability" if we're in midair
		if (mBaseAnimID == ANIM_JUMP_LOOP) 
			yawAtSpeed *= 0.2f;

		// turn as much as we can, but not more than we need to
		if (yawToGoal < 0) 
			yawToGoal = std::min<Real>(0, std::max<Real>(yawToGoal, yawAtSpeed)); 
		//yawToGoal = Math::Clamp<Real>(yawToGoal, yawAtSpeed, 0);
		else if (yawToGoal > 0) 
			yawToGoal = std::max<Real>(0, std::min<Real>(yawToGoal, yawAtSpeed)); 
		//yawToGoal = Math::Clamp<Real>(yawToGoal, 0, yawAtSpeed);

		mBodyNode->yaw(Degree(yawToGoal));

		Real move = deltaTime * RUN_SPEED * mAnims[mBaseAnimID]->getWeight();
		distance -= move;
		goalPos *= move;
		goalPos.y = 0;

		if (distance <= 0.0f)
		{
			mBodyNode->setPosition(mGoalPosition);
			mShadowDecal.setPosition( mGoalPosition.x, mGoalPosition.z );
			mGoalPosition = Vector3::ZERO;

			// stop running if already moving and the player doesn't want to move
			setBaseAnimation(ANIM_IDLE_BASE);
			if (mTopAnimID == ANIM_RUN_TOP) 
				setTopAnimation(ANIM_IDLE_TOP);
		}
		else
		{
			// move in current body direction (not the goal direction)
			mBodyNode->translate( goalPos,Node::TS_WORLD);

			TerrainGroup* pTerrainGroup = GetEditor()->GetTerrainGroup();
			pos = mBodyNode->getPosition();
			pos.y = pTerrainGroup->getHeightAtWorldPosition( pos ) + CHAR_HEIGHT;
			mBodyNode->setPosition(pos);
			mShadowDecal.setPosition( pos.x, pos.z );
		}
	}

	if (mBaseAnimID == ANIM_JUMP_LOOP)
	{
		// if we're jumping, add a vertical offset too, and apply gravity
		mBodyNode->translate(0, mVerticalVelocity * deltaTime, 0, Node::TS_LOCAL);
		mVerticalVelocity -= GRAVITY * deltaTime;

		Vector3 pos = mBodyNode->getPosition();
		if (pos.y <= CHAR_HEIGHT)
		{
			// if we've hit the ground, change to landing state
			pos.y = CHAR_HEIGHT;
			mBodyNode->setPosition(pos);
			setBaseAnimation(ANIM_JUMP_END, true);
			mTimer = 0;
		}
	}
}

//-----------------------------------------------------------------------
void CNpc::updateAnimations(Real deltaTime)
{
	Real baseAnimSpeed = 1;
	Real topAnimSpeed = 1;

	mTimer += deltaTime;

	if (mTopAnimID == ANIM_SLICE_VERTICAL || mTopAnimID == ANIM_SLICE_HORIZONTAL)
	{
		if (mTimer >= mAnims[mTopAnimID]->getLength())
		{
			// animation is finished, so return to what we were doing before
			if (mBaseAnimID == ANIM_IDLE_BASE) setTopAnimation(ANIM_IDLE_TOP);
			else
			{
				setTopAnimation(ANIM_RUN_TOP);
				mAnims[ANIM_RUN_TOP]->setTimePosition(mAnims[ANIM_RUN_BASE]->getTimePosition());
			}
		}

		// don't sway hips from side to side when slicing. that's just embarrasing.
		if (mBaseAnimID == ANIM_IDLE_BASE) baseAnimSpeed = 0;
	}
	else if (mBaseAnimID == ANIM_JUMP_START)
	{
		if (mTimer >= mAnims[mBaseAnimID]->getLength())
		{
			// takeoff animation finished, so time to leave the ground!
			setBaseAnimation(ANIM_JUMP_LOOP, true);
			// apply a jump acceleration to the character
			mVerticalVelocity = JUMP_ACCEL;
		}
	}
	else if (mBaseAnimID == ANIM_JUMP_END)
	{
		if (mTimer >= mAnims[mBaseAnimID]->getLength())
		{
			// safely landed, so go back to running or idling
			//if (mKeyDirection == Vector3::ZERO)
			//{
			//	setBaseAnimation(ANIM_IDLE_BASE);
			//	setTopAnimation(ANIM_IDLE_TOP);
			//}
			//else
			//{
			//	setBaseAnimation(ANIM_RUN_BASE, true);
			//	setTopAnimation(ANIM_RUN_TOP, true);
			//}
		}
	}

	// increment the current base and top animation times
	if (mBaseAnimID != ANIM_NONE) 
		mAnims[mBaseAnimID]->addTime(deltaTime * baseAnimSpeed);
	if (mTopAnimID != ANIM_NONE) 
		mAnims[mTopAnimID]->addTime(deltaTime * topAnimSpeed);

	// apply smooth transitioning between our animations
	fadeAnimations(deltaTime);
}

//-----------------------------------------------------------------------
void CNpc::fadeAnimations(Real deltaTime)
{
	for (int i = 0; i < NUM_ANIMS; i++)
	{
		if (mFadingIn[i])
		{
			// slowly fade this animation in until it has full weight
			Real newWeight = mAnims[i]->getWeight() + deltaTime * ANIM_FADE_SPEED;
			mAnims[i]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));
			if (newWeight >= 1) mFadingIn[i] = false;
		}
		else if (mFadingOut[i])
		{
			// slowly fade this animation out until it has no weight, and then disable it
			Real newWeight = mAnims[i]->getWeight() - deltaTime * ANIM_FADE_SPEED;
			mAnims[i]->setWeight(Math::Clamp<Real>(newWeight, 0, 1));
			if (newWeight <= 0)
			{
				mAnims[i]->setEnabled(false);
				mFadingOut[i] = false;
			}
		}
	}
}

//-----------------------------------------------------------------------
void CNpc::Destroy()
{
	DestroyCharacterProxy();

	if (mTimeController)
	{
		// Destroy controller
		Ogre::ControllerManager::getSingleton().destroyController(mTimeController);
		mTimeController = 0;
	}

	mBodyNode->detachObject(mpLight);
	mpSceneMgr->destroyLight(mpLight);
	mBodyNode->detachObject(mBodyEnt);
	mpSceneMgr->destroyEntity( "SinbadBody" );
	mpSceneMgr->destroySceneNode( mBodyNode );

	mShadowDecal.Destroy();
}

void CNpc::CreateCharacterProxy()
{
	m_world = CPhysicsManager::getSingletonPtr()->GetPhysicsWorld();

	m_world->lock();

	//
	//	Create a character proxy object
	//
	{
		// Construct a shape

		hkVector4 vertexA(0,0, 0.4f);
		hkVector4 vertexB(0,0,-0.4f);		

		// Create a capsule to represent the character standing
		m_standShape = new hkpCapsuleShape(vertexA, vertexB, .6f);

		// Create a capsule to represent the character crouching
		// Note that we create the smaller capsule with the base at the same position as the larger capsule.
		// This means we can simply swap the shapes without having to reposition the character proxy,
		// and if the character is standing on the ground, it will still be on the ground.
		vertexA.setZero4();
		m_crouchShape = new hkpCapsuleShape(vertexA, vertexB, .6f);


		// Construct a Shape Phantom
		m_phantom = new hkpSimpleShapePhantom( m_standShape, hkTransform::getIdentity(), hkpGroupFilter::calcFilterInfo(0,2) );

		// Add the phantom to the world
		m_world->addPhantom(m_phantom);
		m_phantom->removeReference();

		// Construct a character proxy
		hkpCharacterProxyCinfo cpci;
		cpci.m_position.set(-9.1f, 35, .4f);
		cpci.m_staticFriction = 0.0f;
		cpci.m_dynamicFriction = 1.0f;
		cpci.m_up.setNeg4( m_world->getGravity() );
		cpci.m_up.normalize3();	
		cpci.m_userPlanes = 4;
		cpci.m_maxSlope = HK_REAL_PI / 3.f;

		cpci.m_shapePhantom = m_phantom;
		m_characterProxy = new hkpCharacterProxy( cpci );
	}

	//
	// Add in a custom friction model
	//
	{
		hkVector4 up( 0.f, 0.f, 1.f );
		m_listener = new MyCharacterListener();
		m_characterProxy->addCharacterProxyListener(m_listener);
	}

	//
	// Create the Character state machine and context
	//
	{
		hkpCharacterState* state;
		hkpCharacterStateManager* manager = new hkpCharacterStateManager();

		state = new hkpCharacterStateOnGround();
		manager->registerState( state,	HK_CHARACTER_ON_GROUND);
		state->removeReference();

		state = new hkpCharacterStateInAir();
		manager->registerState( state,	HK_CHARACTER_IN_AIR);
		state->removeReference();

		state = new hkpCharacterStateJumping();
		manager->registerState( state,	HK_CHARACTER_JUMPING);
		state->removeReference();

		state = new hkpCharacterStateClimbing();
		manager->registerState( state,	HK_CHARACTER_CLIMBING);
		state->removeReference();

		m_characterContext = new hkpCharacterContext(manager, HK_CHARACTER_ON_GROUND);
		manager->removeReference();
	}

	m_world->unlock();

	// Current camera angle about up
	m_currentAngle = HK_REAL_PI * 0.5f;
}

void CNpc::DestroyCharacterProxy()
{
	m_listener->removeReference();

	m_world->lock();

	m_world->removePhantom(m_characterProxy->getShapePhantom());

	m_characterProxy->removeReference();

	m_standShape->removeReference();

	m_crouchShape->removeReference();

	m_world->unlock();

	delete m_characterContext;
}