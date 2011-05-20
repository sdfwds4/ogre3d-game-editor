

#pragma once

#include "DecalEntity.h"

class hkpShapePhantom;
class hkpCharacterProxy;
class hkpCharacterContext;
class MyCharacterListener;

using namespace Ogre;

#define NUM_ANIMS 13           // number of animations the character has
#define CHAR_HEIGHT 5          // height of character's center of mass above ground
#define CAM_HEIGHT 2           // height of camera above character's center of mass
#define TURN_SPEED 1000.0f      // character turning in degrees per second
#define ANIM_FADE_SPEED 7.5f   // animation crossfade speed in % of full weight per second
#define JUMP_ACCEL 30.0f       // character jump acceleration in upward units per squared second
#define GRAVITY 90.0f          // gravity in downward units per squared second

class CNpc
{
public:
	CNpc();
	~CNpc();

	void Create();
	void Updaye(Real deltaTime);
	void Destroy();
	void setGoalDirection(Vector3& pos);
	void setPosition( Vector3& pos, bool fixCamera = false );

	// all the animations our character has, and a null ID
	// some of these affect separate body parts and will be blended together
	enum AnimID
	{
		ANIM_IDLE_BASE,
		ANIM_IDLE_TOP,
		ANIM_RUN_BASE,
		ANIM_RUN_TOP,
		ANIM_HANDS_CLOSED,
		ANIM_HANDS_RELAXED,
		ANIM_DRAW_SWORDS,
		ANIM_SLICE_VERTICAL,
		ANIM_SLICE_HORIZONTAL,
		ANIM_DANCE,
		ANIM_JUMP_START,
		ANIM_JUMP_LOOP,
		ANIM_JUMP_END,
		ANIM_NONE
	};

	void setBaseAnimation(AnimID id, bool reset = false);
	void setTopAnimation(AnimID id, bool reset = false);

	void updateBody(Real deltaTime);
	void updateAnimations(Real deltaTime);
	void fadeAnimations(Real deltaTime);

	void updatePhysics(Ogre::Real deltaTime);
	void CreateCharacterProxy();
	void DestroyCharacterProxy();
protected:
	SceneNode* mBodyNode;
	Entity* mBodyEnt;
	AnimationState* mAnims[NUM_ANIMS];    // master animation list
	AnimID mBaseAnimID;                   // current base (full- or lower-body) animation
	AnimID mTopAnimID;                    // current top (upper-body) animation
	bool mFadingIn[NUM_ANIMS];            // which animations are fading in
	bool mFadingOut[NUM_ANIMS];           // which animations are fading out
	bool mSwordsDrawn;
	Vector3 mGoalPosition;     // actual intended direction in world-space
	Real mVerticalVelocity;     // for jumping
	Real mTimer;                // general timer to see how long animations have been playing
	Light* mpLight;
	SceneManager* mpSceneMgr;
	CDecalEntity mShadowDecal;
	/** Controller for time update*/
	Controller<Ogre::Real>* mTimeController;

	hkpCharacterProxy* m_characterProxy;

	hkpCharacterContext* m_characterContext;

	hkpShapePhantom* m_phantom;

	hkpShape* m_standShape;

	MyCharacterListener* m_listener;

	hkpWorld* m_world;

	hkReal m_timestep;

};