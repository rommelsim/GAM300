#pragma once
/*!***************************************************************************
	\file			Particle.h
	\author			Rommel Sim
	\par			zhenpengrommel.sim@digipen.edu
	\date			Oct 1, 2023
	\brief			This header file consists of the function declarations to
					manage Paritcle
	Copyright (C) 2023 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the
	prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************************/
#include "Core/Log.h"
#include "Core/Singleton.hpp"

#include "ECS/Architecture/ECS.hpp"
#include "ECS/Components/IComponent.h"
#include "ECS/Components/Transform.h"

#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <rttr/registration.h>

#include "Resource/Material.h"
#include "Resource/Model.h"

namespace AeonCore
{
	struct Particle
	{
		glm::vec3 position, velocity;
		glm::vec3 colour;
		glm::vec3 scale;

		float life;

		Particle() : position(0.0f), velocity(0.0f), colour(1.0f), scale(0.1f), life(0.0f)
		{
		}
	};

	class AEON_API Particles : public IComponent
	{
	public:

		///		CONSTRUCTOR	//////////////////////////////////////////////

		Particles(EntityID entityId);
		Particles(EntityID entityId, unsigned int _MaxParticles, unsigned int _SpawnParticles);

		///		DESTRUCTOR	//////////////////////////////////////////////

		~Particles() = default;

		///		FUNCTIONS	//////////////////////////////////////////////

		void Update(float _dt, Transform& _transform);

		///		GET FUNCTIONS	//////////////////////////////////////////////

		inline std::vector<Particle>	GetParticleContainer()		{ return mParticles; }
		inline glm::vec3				GetParticleGravity()		{ return mParticleGravity; }
		inline glm::vec3 				GetMinVelocity()			{ return minVelocity; }
		inline glm::vec3 				GetMaxVelocity()			{ return maxVelocity; }
		inline glm::vec3				GetParticleOffset()			{ return mOffset; }

		inline unsigned int				GetMaximumParticles()		{ return mMaxParticles; }
		inline unsigned int				GetSpawnParticleRate()		{ return mSpawnParticle; }
		inline float					GetParticleLife()			{ return mInitialLife; }


		// Material Getter
		std::string		GetDiffuseTexture()		{ return m_material.GetDiffuseTexture(); }
		std::string		GetSpecularTexture()	{ return m_material.GetSpecularTexture(); }
		Material*		GetMaterialPtr()		{ return &m_material; }
		Model*			GetModelPtr()			{ return &m_model; }
		std::string		GetModelPath()			{ return m_model.GetDirectory(); }

		///		SET FUNCTIONS	//////////////////////////////////////////////

		inline void		SetParticleMinVelocity	(const glm::vec3 _minVelocity)	{ minVelocity = _minVelocity; }
		inline void		SetParticleMaxVelocity	(const glm::vec3 _maxVelocity)	{ maxVelocity = _maxVelocity; }
		inline void		SetParticleGravity		(const glm::vec3 _gravity)		{ mParticleGravity = _gravity; }
		//inline void		SetMinVelocity			(const glm::vec3 _minVel)		{ minVelocity = _minVel; }
		//inline void		SetMaxVelocity			(const glm::vec3 _MaxVel)		{ maxVelocity = _MaxVel; }
		
		inline void		SetSpawnParticleRate	(unsigned int _SpawnParticles)	{ mSpawnParticle = _SpawnParticles; }
		inline void		SetParticleLife			(float _life)					{ mInitialLife = _life; }
		inline void		SetParticleOffset		(const glm::vec3 _offset)		{ mOffset = _offset; }

		void			SetParticleScale		(const glm::vec3& _scale);
		void			SetMaximumParticles		(unsigned int _maxParticles);

		// Material Setter
		void			SetDiffuseTexture(std::string _path);
		void			SetSpecularTexture(std::string _path);
		void			SetModelName(std::string _name) { m_model.SetName(_name); }
		void			SetModel(std::string _path);

	private:
		RTTR_ENABLE(IComponent)

		///		VARIABLES	//////////////////////////////////////////////

		std::vector<Particle> mParticles;
		glm::vec3 minVelocity		= glm::vec3(1.0f);
		glm::vec3 maxVelocity		= glm::vec3(5.0f);
		glm::vec3 mParticleGravity  = glm::vec3(0.0f, -9.8f, 0.0f);	// gravity (default is 9.8 m/s^2 downwards))
		glm::vec3 mOffset			= glm::vec3(0.f);
		unsigned int mMaxParticles;
		unsigned int mlastUsedParticle = 0;	// stores the index of the last particle used (for quick access to next dead particle)
		unsigned int mSpawnParticle;
		float mInitialLife = 1.0f;
		// Properties
		Material		m_material;
		Model			m_model;
		std::string		m_text;
		EntityID m_EntID;

		///		FUNCTIONS	//////////////////////////////////////////////

		void			init();
		unsigned int	firstUnusedParticle();
		void			respawnParticle(Particle& _particle, Transform& _transform, glm::vec3 minVel, glm::vec3 maxVel, float _life, glm::vec3 _offset = glm::vec3(0.f));

	};



	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//namespace particles
	//{

	//	// Class to store data for ONE particle
	//	class Particle
	//	{
	//	public:
	//		std::unique_ptr<glm::vec4[]> m_pos;
	//		std::unique_ptr<glm::vec4[]> m_col;
	//		std::unique_ptr<glm::vec4[]> m_startCol;
	//		std::unique_ptr<glm::vec4[]> m_endCol;
	//		std::unique_ptr<glm::vec4[]> m_vel;
	//		std::unique_ptr<glm::vec4[]> m_acc;
	//		std::unique_ptr<glm::vec4[]> m_time;
	//		std::unique_ptr<bool[]>  m_alive;

	//		size_t m_count{ 0 };
	//		size_t m_countAlive{ 0 };
	//	public:
	//		Particle() {}
	//		explicit Particle(size_t maxCount) { generate(maxCount); }
	//		~Particle() { }

	//		Particle(const Particle&) = delete;
	//		Particle& operator=(const Particle&) = delete;

	//		void generate(size_t maxSize);
	//		void kill(size_t id);
	//		void wake(size_t id);
	//		void swapData(size_t a, size_t b);

	//	};

	//	// Generate particle
	//	class ParticleGenerator
	//	{
	//	public:
	//		ParticleGenerator() { }
	//		virtual ~ParticleGenerator() { }

	//		virtual void generate(double dt, Particle* p, size_t startId, size_t endId) = 0;
	//	};


	//	// Emits particle
	//	class ParticleEmitter
	//	{
	//	protected:
	//		std::vector<std::shared_ptr<particles::ParticleGenerator>> m_generators;
	//	public:
	//		float m_emitRate{ 0.0 };
	//	public:
	//		ParticleEmitter() { }
	//		virtual ~ParticleEmitter() { }

	//		// calls all the generators and at the end it activates (wakes) particle
	//		virtual void emit(double dt, particles::Particle* p);

	//		void addGenerator(std::shared_ptr<particles::ParticleGenerator> gen) { m_generators.push_back(gen); }
	//	};

	//	// Update each particle using DT
	//	class ParticleUpdater
	//	{
	//	public:
	//		ParticleUpdater() { }
	//		virtual ~ParticleUpdater() { }

	//		virtual void update(double dt, particles::Particle* p) = 0;
	//	};

	//	// Particle System
	//	// Handles generating of a particle, emitting particles, and updating each particle.
	//	/*
	//		SRP principle: particle system contains a particle container, list of emitters, list of updaters.
	//		It does only basic stuff like initialization, cleanup and manages the update procedure.
	//	*/
	//	class ParticleSystem
	//	{
	//	protected:
	//		particles::Particle m_particles;
	//		particles::Particle m_aliveParticles;

	//		size_t m_count;

	//		std::vector<std::shared_ptr<particles::ParticleEmitter>> m_emitters;
	//		std::vector<std::shared_ptr<particles::ParticleUpdater>> m_updaters;

	//	public:
	//		explicit ParticleSystem(size_t maxCount);
	//		virtual ~ParticleSystem() { }

	//		ParticleSystem(const ParticleSystem&) = delete;
	//		ParticleSystem& operator=(const ParticleSystem&) = delete;

	//		virtual void update(double dt);
	//		virtual void reset();

	//		virtual size_t numAllParticles() const { return m_particles.m_count; }
	//		virtual size_t numAliveParticles() const { return m_particles.m_countAlive; }

	//		void addEmitter(std::shared_ptr<particles::ParticleEmitter> em) { m_emitters.push_back(em); }
	//		void addUpdater(std::shared_ptr<particles::ParticleUpdater> up) { m_updaters.push_back(up); }

	//		particles::Particle* finalData() { return &m_particles; }

	//		static size_t computeMemoryUsage(const ParticleSystem& p);
	//	};
	//}
	//namespace generators
	//{
	//	class BoxPosGen : public particles::ParticleGenerator
	//	{
	//	public:
	//		glm::vec4 m_pos{ 0.0 };
	//		glm::vec4 m_maxStartPosOffset{ 0.0 };
	//	public:
	//		BoxPosGen() { }

	//		virtual void generate(double dt, particles::Particle* p, size_t startId, size_t endId) override;
	//	};

	//	class RoundPosGen : public particles::ParticleGenerator
	//	{
	//	public:
	//		glm::vec4 m_center{ 0.0 };
	//		float m_radX{ 0.0 };
	//		float m_radY{ 0.0 };
	//	public:
	//		RoundPosGen() { }
	//		RoundPosGen(const glm::vec4& center, double radX, double radY)
	//			: m_center(center)
	//			, m_radX((float)radX)
	//			, m_radY((float)radY)
	//		{ }

	//		virtual void generate(double dt, particles::Particle* p, size_t startId, size_t endId) override;
	//	};

	//	class BasicColorGen : public particles::ParticleGenerator
	//	{
	//	public:
	//		glm::vec4 m_minStartCol{ 0.0 };
	//		glm::vec4 m_maxStartCol{ 0.0 };
	//		glm::vec4 m_minEndCol{ 0.0 };
	//		glm::vec4 m_maxEndCol{ 0.0 };
	//	public:
	//		BasicColorGen() { }

	//		virtual void generate(double dt, particles::Particle* p, size_t startId, size_t endId) override;
	//	};

	//	class BasicVelGen : public particles::ParticleGenerator
	//	{
	//	public:
	//		glm::vec4 m_minStartVel{ 0.0 };
	//		glm::vec4 m_maxStartVel{ 0.0 };
	//	public:
	//		BasicVelGen() { }

	//		virtual void generate(double dt, particles::Particle* p, size_t startId, size_t endId) override;
	//	};

	//	class SphereVelGen : public particles::ParticleGenerator
	//	{
	//	public:
	//		float m_minVel{ 0.0f };
	//		float m_maxVel{ 0.0f };
	//	public:
	//		SphereVelGen() { }

	//		virtual void generate(double dt, particles::Particle* p, size_t startId, size_t endId) override;
	//	};

	//	class BasicTimeGen : public particles::ParticleGenerator
	//	{
	//	public:
	//		float m_minTime{ 0.0 };
	//		float m_maxTime{ 0.0 };
	//	public:
	//		BasicTimeGen() { }

	//		virtual void generate(double dt, particles::Particle* p, size_t startId, size_t endId) override;
	//	};
	//}
	//namespace updaters
	//{
	//	class EulerUpdater : public particles::ParticleUpdater
	//	{
	//	public:
	//		glm::vec4 m_globalAcceleration{ 0.0f };
	//	public:
	//		virtual void update(double dt, particles::Particle* p) override;
	//	};

	//	// can bounce particle off the floor.
	//	class FloorUpdater : public particles::ParticleUpdater
	//	{
	//	public:
	//		float m_floorY{ 0.0f };
	//		float m_bounceFactor{ 0.5f };
	//	public:
	//		virtual void update(double dt, particles::Particle* p) override;
	//	};

	//	// attractors in a gravity system.
	//	class AttractorUpdater : public particles::ParticleUpdater
	//	{
	//	protected:
	//		std::vector<glm::vec4> m_attractors; // .w is force
	//	public:
	//		virtual void update(double dt, particles::Particle* p) override;

	//		size_t collectionSize() const { return m_attractors.size(); }
	//		void add(const glm::vec4& attr) { m_attractors.push_back(attr); }
	//		glm::vec4& get(size_t id) { return m_attractors[id]; }
	//	};

	//	// generate current particle color based on time and min and max color.
	//	class BasicColorUpdater : public particles::ParticleUpdater
	//	{
	//	public:
	//		virtual void update(double dt, particles::Particle* p) override;
	//	};

	//	// current color comes from position.
	//	class PosColorUpdater : public particles::ParticleUpdater
	//	{
	//	public:
	//		glm::vec4 m_minPos{ 0.0 };
	//		glm::vec4 m_maxPos{ 1.0 };
	//	public:
	//		virtual void update(double dt, particles::Particle* p) override;
	//	};

	//	// current color comes from velocity.
	//	class VelColorUpdater : public particles::ParticleUpdater
	//	{
	//	public:
	//		glm::vec4 m_minVel{ 0.0 };
	//		glm::vec4 m_maxVel{ 1.0 };
	//	public:
	//		virtual void update(double dt, particles::Particle* p) override;
	//	};

	//	// measures the time of life of a particle. It kills a particle if its time is over.
	//	class BasicTimeUpdater : public particles::ParticleUpdater
	//	{
	//	public:
	//		virtual void update(double dt, particles::Particle* p) override;
	//	};
	//}
}


