/// \file Object.h
/// \brief Interface for the game object class CObject.

#pragma once

#include "GameDefines.h"
#include "Renderer.h"
#include "Common.h"
#include "Component.h"
#include "SpriteDesc.h"

/// \brief The game object. 
///
/// CObject is the abstract representation of an object.

class CWeapon;

class CObject:
  public CCommon,
  public CComponent,
  public CSpriteDesc2D
{
  friend class CObjectManager; ///< The object manager needs access to manage objects.

protected:
	BoundingSphere m_Sphere; ///< Bounding sphere.
	BoundingBox m_Aabb; ///< Axially aligned bounding box.

	float m_fSpeed = 0.0f; ///< Speed.
	float m_fOrthoSpeed = 0.0f; ///<Speed in the direction orthogonal to the objects direction.
	float m_fRotSpeed = 0.0f; ///< Rotational speed.

	bool m_bStrafeLeft = false; ///< Strafe left.
	bool m_bStrafeRight = false; ///< Strafe right.
	bool m_bStrafeForward = false; ///< Strafe forward.
	bool m_bStrafeBack = false; ///< Strafe back.

	//int spriteIndex = 0; ///< Index for the sprite, if necessary. this is m_nCurrentFrame?
	float distance = 0.0f; ///< Distance traveled, for animation.

	Vector2 m_vVelocity = Vector2::Zero; ///< Velocity.

	Vector2 m_vOldPos = Vector2::Zero; ///< Last position.

	Vector2 m_vRadius = Vector2::Zero; ///< Half width and height of object sprite.

public:
	CObject(); ///< Default constructor.
	CObject(eSpriteType t, const Vector2& p); ///< Constructor.

	bool m_bDead = false; ///< Is dead or not.

	void kill(); ///< Kill me.
	bool IsDead(); ///< Query whether dead.
	virtual void DeathFX(); ///< Death special effects.
	
	//Deals With Movement
	void SetSpeed(float speed); ///< Set speed.
	void SetOrthoSpeed(float speed); ///< Set orthogonal speed.
	float GetSpeed(); ///< Get speed.
	void SetRotSpeed(float speed); ///< Set rotational velocity.

	void StrafeLeft(); ///< Strafe left.
	void StrafeRight(); ///< Strafe right.
	void StrafeForward(); ///<Strafe forward.
	void StrafeBack(); ///< Strafe back.

	Vector2 GetViewVector(); //Get view vector.
	float GetOrientation(); ///< Get orientation.
	void SetOrientation(float angle); ///< Set orientation.

	const Vector2& GetVelocity(); ///< Get orientation.
	void SetVelocity(const Vector2& v); ///< Set orientation.

	

	const BoundingBox& GetBoundingBox(); ///< Get AABB.
	const BoundingSphere& GetBoundingSphere(); ///< Get bounding sphere.
	const Vector2& GetPos(); ///< Get position.

	virtual void move(); ///< Move object.

	/// For inheritance only
	virtual void CollisionResponseWall(); ///< Collision response for hitting a wall.
	virtual void CollisionResponseObject(CObject* p); ///< Collision response for hitting another object.
	void EntityCollision(CObject* p); ///< How to respond when hitting a moving object.

	virtual void Damage(int d);

	virtual void PrimaryAction();
	virtual void SecondaryAction();

	virtual void equipWeapon(CWeapon* w, CObject* o);

	virtual void animate(); ///< does what it says on the tin

	virtual CObject* getOwner(); ///< Returns the owner of the object, only for projectiles & weaps
}; //CObject