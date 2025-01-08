#include "acpch.h"
/*!***************************************************************************
	\file			PhysicsSystem.h
	\author			Ng Yuwen Felicia & Chua Yip Xuan 
	\par			yuwenfelicia.ng\@digipen.edu & yipxuan.chua\@digipen.edu 
	\date			Oct 1, 2023
	\brief			This header file consists of the Physics System
					and its functions definitions, to initialise, update and
					destroy the system.
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/

#include "ECS/Systems/PhysicsSystem.h"
#include "ECS/Components/Transform.h"
#include "ECS/Architecture/Coordinator.hpp"
#include "Core/TimeStep.h"
#include "Physics/ACPhysicsInternal.hpp"

#include <PxPhysics.h>
#include <PxPhysicsAPI.h>

#include "Camera/CameraManager.h"
#include "ECS/Components/Collider.h"
#include "ECS/Components/RigidBody.h"
#include "Input/Input.h"
#include "Rendering/Rendering.h"

namespace AeonCore
{
	void PhysicsSystem::Init()
	{
		mPhysicsScene = &ACPhysics::PhysicsInternal::GetInstance().GetPhysicsScene();
	}
	void PhysicsSystem::CreateAllActors()
	{
		Coordinator &gCoord = Coordinator::GetInstance();
		for ( Entity entity :  mEntities)
		{
			//Collider *col = gCoord.HasComponent<Collider>(entity) ? &gCoord.GetComponent<Collider>(entity) : nullptr;
			//RigidBody *rb = gCoord.HasComponent<RigidBody>(entity) ? &gCoord.GetComponent<RigidBody>(entity) : nullptr;
			//Transform *trans = gCoord.HasComponent<Transform>(entity) ? &gCoord.GetComponent<Transform>(entity) : nullptr;

			Transform* trans = &gCoord.GetComponent<Transform>(entity);
			Collider* col = &gCoord.GetComponent<Collider>(entity);  // nullptr if not existent
			RigidBody* rb = &gCoord.GetComponent<RigidBody>(entity); 
			if ( !col || !rb ) continue;
			if (rb->GetPhysxActor()) continue;
			CreateActor(entity, trans, rb, col);
		}
		mAreActorsCreated = true;
	}

	void PhysicsSystem::AfterSceneLoaded()
	{
		CreateAllActors();
	}

	void PhysicsSystem::DeleteActor(Entity _entity)
	{
		Coordinator &gCoord = Coordinator::GetInstance();
		RigidBody *rb = gCoord.HasComponent<RigidBody>(_entity) ? &gCoord.GetComponent<RigidBody>(_entity) : nullptr;
		Collider *col = gCoord.HasComponent<Collider>(_entity) ? &gCoord.GetComponent<Collider>(_entity) : nullptr;
		if ( col && col->GetPhysxShape() )
		{
			col->GetPhysxShape()->release();
			col->SetPhysxShape(nullptr);
		}
		if ( col && col->GetPhysxMaterial() )
		{
			col->GetPhysxMaterial()->release();
			col->SetPhysxMaterial(nullptr);
		}
		if ( rb && rb->GetPhysxActor() )
		{
			mPhysicsScene->removeActor(*rb->GetPhysxActor());
			rb->GetPhysxActor()->release();
			rb->SetPhysxActor(nullptr);
			AeonCore_INFO("Actor has been deleted!");
		}
	}

	void PhysicsSystem::CreateActor(Entity _entity, Transform* _trans, RigidBody* _rb, Collider* _col)
	{
		ZoneScopedN("CreateActor");

		auto &physicsInternal = ACPhysics::PhysicsInternal::GetInstance();
		_col->SetPhysxMaterial(physicsInternal.CreateMaterial(_col->GetMaterial()));
		switch ( _col->GetColliderType() )
		{
			case Collider::ColliderType::CUBE:
				{
					auto *geom = dynamic_cast<ACPhysics::CubeGeometry *>(_col->GetGeometry());
					geom->GetPhysxGeometry().halfExtents = ACPhysicsUtility::ToPhysXVector(geom->GetHalfExtents());
					if ( geom->GetPhysxGeometry().halfExtents.x == 0.0f || geom->GetPhysxGeometry().halfExtents.y == 0.0f || geom->GetPhysxGeometry().halfExtents.z == 0.0f )
					{
						AeonCore_ERROR("Half Extents cannot be 0!");
						break;
					}
					_col->SetPhysxShape(physicsInternal.GetPhysics().createShape(geom->GetPhysxGeometry(), *_col->GetPhysxMaterial(), true));
					(_col &&_col->GetIsShowingBounds()) ?
						_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eVISUALIZATION, true)
						:
						_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eVISUALIZATION, false);

					_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
					if ( _col->IsTrigger() )
					{
						_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
						_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
					}
					else
					{
						_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
						_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
					}
					_col->GetPhysxShape()->setContactOffset(0.02f);
					break;
				}
			case Collider::ColliderType::SPHERE:
				{
					auto geom = dynamic_cast<ACPhysics::SphereGeometry *>(_col->GetGeometry());
					geom->GetPhysxGeometry().radius = geom->GetRadius();

					if ( geom->GetPhysxGeometry().radius == 0.0f )
					{
						AeonCore_ERROR("Radius cannot be 0!");
						break;
					}
					_col->SetPhysxShape(physicsInternal.GetPhysics().createShape(geom->GetPhysxGeometry(), *_col->GetPhysxMaterial(), true));
					(_col &&_col->GetIsShowingBounds()) ?
						_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eVISUALIZATION, true)
						:
						_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eVISUALIZATION, false);

					_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
					if ( _col->IsTrigger() )
					{
						_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
						_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
					}
					else
					{
						_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
						_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
					}
					_col->GetPhysxShape()->setContactOffset(0.02f);
					break;
				}
		
		case Collider::ColliderType::NONE:
				{
					AeonCore_ERROR("Collider Type is NONE!");
					break;
				}
			default:
				AeonCore_ERROR("Collider Type does not exist!");
		}

		switch ( _rb->GetRigidBodyType() )
		{
			case RigidBody::RigidBodyType::STATIC:
				{
					_rb->SetPhysxActor(physicsInternal.GetPhysics().createRigidStatic(ACPhysicsUtility::ToPhysXTransform(*_trans)));
					(_col && _col->GetIsShowingBounds()) ?
						_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true)
					:
						_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eVISUALIZATION, false);
					_rb->GetPhysxActor()->attachShape(*_col->GetPhysxShape());
					_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eSEND_SLEEP_NOTIFIES, true);
					physx::PxRigidDynamic *dynamicActor = _rb->GetPhysxActor()->is<physx::PxRigidDynamic>();
					if (dynamicActor )
					{
						dynamicActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, false);
						physx::PxRigidBodyExt::setMassAndUpdateInertia(*dynamicActor, _rb->GetMass());
					}
					mActorsMap[_entity.GetEntityID()] = _rb;
					break;
				}
			case RigidBody::RigidBodyType::DYNAMIC:
				{
					_rb->SetPhysxActor(physicsInternal.GetPhysics().createRigidDynamic(ACPhysicsUtility::ToPhysXTransform(*_trans)));
					(_col &&_col->GetIsShowingBounds() )?
						_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true)
						:
						_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eVISUALIZATION, false);

					_rb->GetPhysxActor()->attachShape(*_col->GetPhysxShape());
					_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eSEND_SLEEP_NOTIFIES, true);
					physx::PxRigidDynamic *dynamicActor = _rb->GetPhysxActor()->is<physx::PxRigidDynamic>();
					if ( dynamicActor )
					{
						dynamicActor->setSleepThreshold(0.000001f);
						dynamicActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, false);
						physx::PxRigidBodyExt::setMassAndUpdateInertia(*dynamicActor, _rb->GetMass());
					}
					mActorsMap[_entity.GetEntityID()] = _rb;
					break;
				}
			default:
				AeonCore_ERROR("RigidBody Type does not exist!");
		}

		_rb->GetPhysxActor()->setGlobalPose(ACPhysicsUtility::ToPhysXTransform(*_trans));
		//_rb->GetPhysxActor()->userData = Coordinator::GetInstance().GetEntity(_entity.GetEntityID());
		_rb->GetPhysxActor()->userData = reinterpret_cast<void*>(_entity.GetEntityID());
		mPhysicsScene->addActor(*_rb->GetPhysxActor());
	}

	void PhysicsSystem::Update()
	{
		ZoneScopedN("Physics Update");
		//if ( !mAreActorsCreated )
		//{
		//	CreateAllActors();
		//}
		//PreSimulationUpdate();

		//if ( Application::GetApplication().GetRuntimeState() == RuntimeState::Start )
		//{
		//	mPhysicsScene->simulate(1.0f / 60.0f);
		//	
		//}
		//else
		//{
		//	mPhysicsScene->simulate(0.00000000001f);
		//}
		//mPhysicsScene->fetchResults(true);
		////After simulate update ECS components
		//PostSimulationUpdate();
		Application& app = Application::GetApplication();

		if (app.GetLoadStatus() != LoadStatus::NotRunning)
			return;

		if (!mAreActorsCreated)
		{
			CreateAllActors();
		}

		if (app.IsPaused())
			return;

		PreSimulationUpdate();
		if (Application::GetApplication().GetRuntimeState() == RuntimeState::Start)
		{
			TimerStep *time = &TimerStep::GetInstance();
			mPhysicsScene->simulate(time->get_delta_time());
			//mPhysicsScene->simulate(1.f/60.0f);
			mPhysicsScene->fetchResults(true);
		}
		PostSimulationUpdate();

	}

	void PhysicsSystem::PreSimulationUpdate()
	{
		ZoneScopedN("PreSimulationUpdate");

		Coordinator &gCoord = Coordinator::GetInstance();
		auto &physicsInternal = ACPhysics::PhysicsInternal::GetInstance();
		for ( auto &actor : mActorsMap )
		{
			Collider *col = gCoord.HasComponent<Collider>(actor.first) ? &gCoord.GetComponent<Collider>(actor.first) : nullptr;
			RigidBody *rb = gCoord.HasComponent<RigidBody>(actor.first) ? &gCoord.GetComponent<RigidBody>(actor.first) : nullptr;
			if ( !col || !rb )
			{
				DeleteActor(actor.first); 
			}
		}

		for ( auto const &entity : mEntities )
		{
			Collider *col = gCoord.HasComponent<Collider>(entity) ? &gCoord.GetComponent<Collider>(entity) : nullptr;
			RigidBody *rb = gCoord.HasComponent<RigidBody>(entity) ? &gCoord.GetComponent<RigidBody>(entity) : nullptr;
			Transform *trans = gCoord.HasComponent<Transform>(entity) ? &gCoord.GetComponent<Transform>(entity) : nullptr;
			if ( col && rb )
			{
				if ( !rb->GetPhysxActor() ) 
				{
					CreateActor(entity,trans, rb, col); 
				}
				else
				{
					if ( col && col->GetHasChanged() )
					{
						UpdatePhysxCol(rb, col);
						col->SetHasChanged(false);
					}
					if ( rb && (rb->GetHasChanged() || mNeedToUpdateShape) )
					{
						UpdatePhysxRb(entity, trans, rb, col);
						rb->SetHasChanged(false);
					}
					if ( trans && trans->GetHasChangedFlag() )
					{
						UpdatePhysxTrans(entity, trans, rb);
						trans->SetHasChangedFlag(false);
					}
					if ( rb && mNeedToCreateActor )
					{
						rb->GetPhysxActor()->userData = reinterpret_cast<void*>(entity);
						physicsInternal.GetPhysicsScene().addActor(*rb->GetPhysxActor());
						mNeedToCreateActor = false;
					}
				}
			}
		}
	}

	void PhysicsSystem::PostSimulationUpdate()
	{
		ZoneScopedN("PostSimulationUpdate");
		Coordinator &gCoord = Coordinator::GetInstance();
		for ( auto &entity : mEntities)
		{
			Collider *col = gCoord.HasComponent<Collider>(entity) ? &gCoord.GetComponent<Collider>(entity) : nullptr;
			RigidBody *rb = gCoord.HasComponent<RigidBody>(entity) ? &gCoord.GetComponent<RigidBody>(entity) : nullptr;
			Transform *trans = gCoord.HasComponent<Transform>(entity) ? &gCoord.GetComponent<Transform>(entity) : nullptr;
			if ( rb && trans )
			{
				UpdateTrans(trans, rb);
				UpdatePhysxTrans(entity, trans, rb);
			}
			if (rb && col )
			{
				UpdatePhysxCol(rb, col);
				UpdatePhysxRb(entity, trans, rb, col);
			}
		}
	}
	
	void PhysicsSystem::UpdatePhysxRb(Entity _entity, Transform *_trans, RigidBody *_rb, Collider *_col)
	{
		ZoneScopedN("UpdatePhysxRb");
		auto &physicsInternal = ACPhysics::PhysicsInternal::GetInstance();
		if ( (_rb->GetRigidBodyType() == RigidBody::RigidBodyType::DYNAMIC && !_rb->GetPhysxActor()->is<physx::PxRigidDynamic>()) ||
			(_rb->GetRigidBodyType() == RigidBody::RigidBodyType::STATIC && !_rb->GetPhysxActor()->is<physx::PxRigidStatic>()) )
		{
			mNeedToCreateActor = true;
			if ( _rb->GetPhysxActor() )
			{
				mPhysicsScene->removeActor(*_rb->GetPhysxActor());
				_rb->GetPhysxActor()->release();  
				_rb->SetPhysxActor(nullptr);
				mActorsMap.erase(_entity.GetEntityID());
			}
			switch ( _rb->GetRigidBodyType() )
			{
				case RigidBody::RigidBodyType::STATIC:
					{
						_rb->SetPhysxActor(physicsInternal.GetPhysics().createRigidStatic(ACPhysicsUtility::ToPhysXTransform(*_trans)));

						(_col &&_col->GetIsShowingBounds()) ?
							_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true)
							:
							_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eVISUALIZATION, false);

						_rb->GetPhysxActor()->attachShape(*_col->GetPhysxShape());
						_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eSEND_SLEEP_NOTIFIES, true);
						physx::PxRigidDynamic *dynamicActor = _rb->GetPhysxActor()->is<physx::PxRigidDynamic>();
						if ( dynamicActor )
						{
							dynamicActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, false);
							physx::PxRigidBodyExt::setMassAndUpdateInertia(*dynamicActor, _rb->GetMass());
						}
						mActorsMap[_entity.GetEntityID()] = _rb;
						break;
					}
				case RigidBody::RigidBodyType::DYNAMIC:
					{
						_rb->SetPhysxActor(physicsInternal.GetPhysics().createRigidDynamic(ACPhysicsUtility::ToPhysXTransform(*_trans)));

						(_col &&_col->GetIsShowingBounds() )?
							_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true)
							:
							_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eVISUALIZATION, false);

						_rb->GetPhysxActor()->attachShape(*_col->GetPhysxShape());
						_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eSEND_SLEEP_NOTIFIES, true);
						
						physx::PxRigidDynamic *dynamicActor = _rb->GetPhysxActor()->is<physx::PxRigidDynamic>();
						if ( dynamicActor )
						{
							AeonCore_WARN("Dynamic RB Update");
							dynamicActor->setSleepThreshold(0.000001f);
							dynamicActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, false);
							physx::PxRigidBodyExt::setMassAndUpdateInertia(*dynamicActor, _rb->GetMass());
						}
						mActorsMap[_entity.GetEntityID()] = _rb;
						break;
					}
				default:
					AeonCore_WARN("Selected Rigidbody Type is not registered!");
			}

			physx::PxRigidDynamic *dynamicActor = _rb->GetPhysxActor()->is<physx::PxRigidDynamic>();
			if ( dynamicActor )
			{
				if ( std::fabs(dynamicActor->getMass() - _rb->GetMass()) > std::numeric_limits<float>::epsilon() )
				{
					AeonCore_WARN("Fabs Dynamic RB Update");
					dynamicActor->setMass(_rb->GetMass());
					dynamicActor->setMassSpaceInertiaTensor(physx::PxVec3(_rb->GetMass()));
				}

				if ( std::fabs(dynamicActor->getLinearDamping() - _rb->GetLinearDrag()) > std::numeric_limits<float>::epsilon() )
				{
					dynamicActor->setLinearDamping(_rb->GetLinearDrag());
				}

				if ( std::fabs(dynamicActor->getAngularDamping() - _rb->GetAngularDrag()) > std::numeric_limits<float>::epsilon() )
				{
					dynamicActor->setAngularDamping(_rb->GetAngularDrag());
				}

				bool disableGravity = !_rb->GetUseGravity();
				if ( dynamicActor->getActorFlags().isSet(physx::PxActorFlag::eDISABLE_GRAVITY) != disableGravity )
				{
					dynamicActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, disableGravity);
				}

				bool isKinematic = _rb->GetIsKinematic();
				if ( dynamicActor->getRigidBodyFlags().isSet(physx::PxRigidBodyFlag::eKINEMATIC) != isKinematic )
				{
					dynamicActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, isKinematic);
				}
			}
		}
		else
		{
			if (mNeedToUpdateShape )
			{
				switch ( _rb->GetRigidBodyType() )
				{
				case RigidBody::RigidBodyType::STATIC:
					{
						(_col &&_col->GetIsShowingBounds()) ?
							_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true)
							:
							_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eVISUALIZATION, false);
						_rb->GetPhysxActor()->attachShape(*_col->GetPhysxShape());
						_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eSEND_SLEEP_NOTIFIES, true);
						physx::PxRigidDynamic *dynamicActor = _rb->GetPhysxActor()->is<physx::PxRigidDynamic>();
						if ( dynamicActor )
						{
							dynamicActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, false);
							physx::PxRigidBodyExt::setMassAndUpdateInertia(*dynamicActor, _rb->GetMass());
						}
						mActorsMap[_entity.GetEntityID()] = _rb;
						break;
					}
				case RigidBody::RigidBodyType::DYNAMIC:
					{
						(_col &&_col->GetIsShowingBounds()) ?
							_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true)
							:
							_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eVISUALIZATION, false);
						_rb->GetPhysxActor()->attachShape(*_col->GetPhysxShape());
						_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eSEND_SLEEP_NOTIFIES, true);
						
						physx::PxRigidDynamic *dynamicActor = _rb->GetPhysxActor()->is<physx::PxRigidDynamic>();
						if ( dynamicActor )
						{
							AeonCore_WARN("ShapeUpdate Dynamic RB Update");
							dynamicActor->setSleepThreshold(0.000001f);
							dynamicActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eENABLE_CCD, false);
							physx::PxRigidBodyExt::setMassAndUpdateInertia(*dynamicActor, _rb->GetMass());
						}
						mActorsMap[_entity.GetEntityID()] = _rb;
						break;
					}
				default:
					{
						AeonCore_WARN("Selected Rigidbody Type is not registered!");
					}
				}
				mNeedToUpdateShape = false;
			}

			physx::PxRigidDynamic *dynamicActor = _rb->GetPhysxActor()->is<physx::PxRigidDynamic>();
			if ( dynamicActor )
			{
				if ( std::fabs(dynamicActor->getMass() - _rb->GetMass()) > std::numeric_limits<float>::epsilon() )
				{
					AeonCore_WARN("ShapeUpdate fabs Dynamic RB Update");
					//dynamicActor->setMass(_rb->GetMass());
					physx::PxRigidBodyExt::setMassAndUpdateInertia(*dynamicActor, _rb->GetMass());
				}

				if ( std::fabs(dynamicActor->getLinearDamping() - _rb->GetLinearDrag()) > std::numeric_limits<float>::epsilon() )
				{
					dynamicActor->setLinearDamping(_rb->GetLinearDrag());
				}

				if ( std::fabs(dynamicActor->getAngularDamping() - _rb->GetAngularDrag()) > std::numeric_limits<float>::epsilon() )
				{
					dynamicActor->setAngularDamping(_rb->GetAngularDrag());
				}

				bool disableGravity = !_rb->GetUseGravity();
				if ( dynamicActor->getActorFlags().isSet(physx::PxActorFlag::eDISABLE_GRAVITY) != disableGravity )
				{
					dynamicActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, disableGravity);
				}

				bool isKinematic = _rb->GetIsKinematic();
				if ( dynamicActor->getRigidBodyFlags().isSet(physx::PxRigidBodyFlag::eKINEMATIC) != isKinematic )
				{
					dynamicActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, isKinematic);
				}
			}
		}
	}

	void PhysicsSystem::UpdatePhysxCol(RigidBody *_rb, Collider *_col)
	{
		ZoneScopedN("UpdatePhysxCol");

		auto &physicsInternal = ACPhysics::PhysicsInternal::GetInstance();

		bool geometryTypeChanged = false;
		switch ( _col->GetColliderType() )
		{
			case Collider::ColliderType::CUBE:
				geometryTypeChanged = dynamic_cast<ACPhysics::CubeGeometry *>(_col->GetGeometry()) == nullptr;
				break;
			case Collider::ColliderType::SPHERE:
				geometryTypeChanged = dynamic_cast<ACPhysics::SphereGeometry *>(_col->GetGeometry()) == nullptr;
				break;
			case Collider::ColliderType::CAPSULE:
				geometryTypeChanged = dynamic_cast<ACPhysics::CapsuleGeometry *>(_col->GetGeometry()) == nullptr;
				break;
		}

		if ( geometryTypeChanged )
		{
			mNeedToUpdateShape = true;
			physx::PxShape *oldShape = _col->GetPhysxShape();
			if ( oldShape )
			{
				_rb->GetPhysxActor()->detachShape(*oldShape);
				physx::PxMaterial *currentMaterials;
				physx::PxU32 materialCount = oldShape->getMaterials(&currentMaterials, 1);

				_col->SetPhysxShape(nullptr);
				_col->SetPhysxMaterial(nullptr);

				if ( materialCount > 0 )
				{
					currentMaterials[0].release();
				}
				oldShape->release();

				if ( _col->GetGeometry() )
				{
					delete _col->GetGeometry();
					_col->SetGeometry(nullptr);
				}

				if ( !_col->GetPhysxMaterial() )
				{
					_col->SetPhysxMaterial(physicsInternal.CreateMaterial(_col->GetMaterial()));
				}
				switch ( _col->GetColliderType() )
				{
					case Collider::ColliderType::CUBE:
						{
							_col->SetGeometry(new ACPhysics::CubeGeometry());
							auto geom = dynamic_cast<ACPhysics::CubeGeometry *>(_col->GetGeometry());
							geom->GetPhysxGeometry().halfExtents = ACPhysicsUtility::ToPhysXVector(geom->GetHalfExtents());
							if ( geom->GetPhysxGeometry().halfExtents.x == 0.0f || geom->GetPhysxGeometry().halfExtents.y == 0.0f || geom->GetPhysxGeometry().halfExtents.z == 0.0f )
							{
								AeonCore_ERROR("Half Extents cannot be 0!");
								break;
							}
							(_col && _col->GetIsShowingBounds()) ?
								_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true)
								:
								_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eVISUALIZATION, false);
							_col->SetPhysxShape(physicsInternal.GetPhysics().createShape(geom->GetPhysxGeometry(), *_col->GetPhysxMaterial(), true));
							_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
							if ( _col->IsTrigger() )
							{
								_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
								_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
							}
							else
							{
								_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
								_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
							}
							_col->GetPhysxShape()->setContactOffset(0.02f);
							break;
						}
					case Collider::ColliderType::SPHERE:
						{
							_col->SetGeometry(new ACPhysics::SphereGeometry());
							auto geom = dynamic_cast<ACPhysics::SphereGeometry *>(_col->GetGeometry());
							geom->GetPhysxGeometry().radius = geom->GetRadius();
							if ( geom->GetPhysxGeometry().radius == 0.0f )
							{
								AeonCore_ERROR("Radius cannot be 0!");
								break;
							}
							(_col && _col->GetIsShowingBounds()) ?
								_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true)
								:
								_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eVISUALIZATION, false);
							_col->SetPhysxShape(physicsInternal.GetPhysics().createShape(geom->GetPhysxGeometry(), *_col->GetPhysxMaterial(), true));
							_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
							if ( _col->IsTrigger() )
							{
								_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
								_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
							}
							else
							{
								_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
								_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
							}
							_col->GetPhysxShape()->setContactOffset(0.02f);
							break;
						}
					case Collider::ColliderType::CAPSULE:
						{
							_col->SetGeometry(new ACPhysics::CapsuleGeometry());
							auto geom = dynamic_cast<ACPhysics::CapsuleGeometry *>(_col->GetGeometry());
							geom->GetPhysxGeometry().halfHeight = geom->GetHalfHeight();
							geom->GetPhysxGeometry().radius = geom->GetRadius();
							if ( geom->GetPhysxGeometry().radius == 0.0f || geom->GetPhysxGeometry().halfHeight == 0.0f )
							{
								AeonCore_ERROR("Radius or Half Height cannot be 0!");
								break;
							}
							(_col && _col->GetIsShowingBounds()) ?
								_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true)
								:
								_rb->GetPhysxActor()->setActorFlag(physx::PxActorFlag::eVISUALIZATION, false);
							_col->SetPhysxShape(physicsInternal.GetPhysics().createShape(geom->GetPhysxGeometry(), *_col->GetPhysxMaterial(), true));
							_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
							if ( _col->IsTrigger() )
							{
								_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
								_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
							}
							else
							{
								_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
								_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
							}
							_col->GetPhysxShape()->setContactOffset(0.02f);
							break;
						}
					default:
						{
							AeonCore_ERROR("Collider Type does not exist!");
						}
				}
			}
		}
		else 
		{
			physx::PxShape *shape = _col->GetPhysxShape();
			if ( shape )
			{
				auto material = _col->GetMaterial();
				if ( !_col->GetPhysxMaterial() )
				{
					_col->SetPhysxMaterial(physicsInternal.CreateMaterial(material));
					physx::PxMaterial *physxMat = _col->GetPhysxMaterial();
					shape->setMaterials(&physxMat, 1);
				}
				bool visualizationFlag = _col->GetIsShowingBounds();
				bool triggerFlag = _col->IsTrigger();
				physx::PxTransform localPose(ACPhysicsUtility::ToPhysXVector(_col->GetOffset()));
				physx::PxMaterial *physxMat = _col->GetPhysxMaterial();
				{
					auto tmpcube = dynamic_cast<ACPhysics::CubeGeometry *>(_col->GetGeometry());
					auto tmpsphere = dynamic_cast<ACPhysics::SphereGeometry *>(_col->GetGeometry());
					auto tmpcapsule = dynamic_cast<ACPhysics::CapsuleGeometry *>(_col->GetGeometry());

					if ( tmpcube )
					{                                                                                     
						tmpcube->SetHalfExtents(ACPhysicsUtility::FromPhysXVector(tmpcube->GetPhysxGeometry().halfExtents));
					}                                                                                                                
					else if ( tmpsphere )
					{
						tmpsphere->SetRadius(tmpsphere->GetPhysxGeometry().radius);
					}
					else if ( tmpcapsule )
					{
						tmpcapsule->SetRadius(tmpcapsule->GetPhysxGeometry().radius);
						tmpcapsule->SetHalfHeight(tmpcapsule->GetPhysxGeometry().halfHeight);
					}
				}
				
				if ( shape->getFlags().isSet(physx::PxShapeFlag::eVISUALIZATION) != visualizationFlag )
				{
					shape->setFlag(physx::PxShapeFlag::eVISUALIZATION, visualizationFlag);
				}
				else if ( shape->getFlags().isSet(physx::PxShapeFlag::eTRIGGER_SHAPE) != triggerFlag )
				{
					if ( triggerFlag )
					{
						_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
						_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
					}
					else
					{
						_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
						_col->GetPhysxShape()->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
					}
				}
				else if ( shape->getLocalPose() != localPose )
				{
					shape->setLocalPose(localPose);
				}
				else if ( std::fabs(physxMat->getStaticFriction() - material.GetStaticFriction()) > std::numeric_limits<float>::epsilon() )
				{
					physxMat->setStaticFriction(material.GetStaticFriction());
					shape->setMaterials(&physxMat, 1);
				}
				else if ( std::fabs(physxMat->getDynamicFriction() - material.GetDynamicFriction()) > std::numeric_limits<float>::epsilon() )
				{
					physxMat->setDynamicFriction(material.GetDynamicFriction());
					shape->setMaterials(&physxMat, 1);
				}
				else if ( std::fabs(physxMat->getRestitution() - material.GetRestitution()) > std::numeric_limits<float>::epsilon() )
				{
					physxMat->setRestitution(material.GetRestitution());
					shape->setMaterials(&physxMat, 1);
				}
				else
				{
					switch (_col->GetColliderType())
					{
					case Collider::ColliderType::CUBE:
						{
							auto tmp = static_cast<ACPhysics::CubeGeometry *>(_col->GetGeometry());
							shape->setGeometry(tmp->GetPhysxGeometry());
							break;
						}
					case Collider::ColliderType::SPHERE:
						{
							auto tmp = static_cast<ACPhysics::SphereGeometry *>(_col->GetGeometry());
							shape->setGeometry(tmp->GetPhysxGeometry());
							break;
						}
					case Collider::ColliderType::CAPSULE:
						{
							auto tmp = static_cast<ACPhysics::CapsuleGeometry *>(_col->GetGeometry());
							shape->setGeometry(tmp->GetPhysxGeometry());
							break;
						}
					}
				}
				
			}
		}
	}

	void PhysicsSystem::UpdatePhysxTrans(Entity _entity, Transform *_trans, RigidBody *_rb)
	{
		_rb->GetPhysxActor()->setGlobalPose(ACPhysicsUtility::ToPhysXTransform(*_trans));
	}

	void PhysicsSystem::UpdateTrans(Transform *_trans, RigidBody *_rb)
	{
		ZoneScopedN("UpdateTrans(Post)");

		if ( _rb->GetPhysxActor() )
		{
			physx::PxTransform actorPose = _rb->GetPhysxActor()->getGlobalPose();
			glm::vec3 p = ACPhysicsUtility::FromPhysXVector(actorPose.p);
			glm::quat q = ACPhysicsUtility::FromPhysXQuat(actorPose.q);
			_trans->PhysicsSyncSetPosition(p);
			_trans->PhysicsSyncSetRotation(q);
		}
	}

	void PhysicsSystem::Destroy()
	{
		Coordinator &gCoord = Coordinator::GetInstance();

		for ( EntityID entity : mEntities )
		{
			Collider *col = gCoord.HasComponent<Collider>(entity) ? &gCoord.GetComponent<Collider>(entity) : nullptr;
			RigidBody *rb = gCoord.HasComponent<RigidBody>(entity) ? &gCoord.GetComponent<RigidBody>(entity) : nullptr;
			if ( col->GetPhysxShape() )
			{
				col->GetPhysxShape()->release();
				col->SetPhysxShape(nullptr);
			}
			if ( col->GetPhysxMaterial() )
			{
				col->GetPhysxMaterial()->release();
				col->SetPhysxMaterial(nullptr);
			}
			if (rb->GetPhysxActor() )
			{
				ACPhysics::PhysicsInternal::GetInstance().GetPhysicsScene().removeActor(*rb->GetPhysxActor());
				rb->GetPhysxActor()->release();
				rb->SetPhysxActor(nullptr);
			}
			mActorsMap.erase(entity);
		}
		//mActorsMap.clear();

		// update PhysX scene state after actors were destroyed
		/*TimerStep *time = &TimerStep::GetInstance();
		mPhysicsScene->simulate(time->get_delta_time());
		//mPhysicsScene->simulate(1.f/60.0f);
		mPhysicsScene->fetchResults(true);*/
	}

	EntityID PhysicsSystem::GetEntityIdFromActor(physx::PxRigidActor *_actor)
	{
		for ( const auto &pair : mActorsMap )
		{
			if ( pair.second->GetPhysxActor() == _actor )
			{
				return pair.first;
			}
		}
		return 0;
	}

}
