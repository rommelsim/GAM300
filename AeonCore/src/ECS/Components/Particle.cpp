#include "acpch.h"
#include "ECS/Components/Particle.h"
#include <corecrt_math_defines.h>

#include "Core/Application.h"
#include "Core/TimeStep.h"
#include "Serialization/RegisterManager.hpp"

namespace AeonCore
{
	Particles::Particles(EntityID entityId)
		: mMaxParticles(200), mSpawnParticle(2), m_EntID(entityId)
	{
		this->init();
	}

	Particles::Particles(EntityID entityId, unsigned int _MaxParticles, unsigned int _SpawnParticles)
		: mMaxParticles(_MaxParticles), mSpawnParticle(_SpawnParticles), m_EntID(entityId)
	{
		this->init();
	}

	void Particles::Update(float _dt, Transform& _transform)
	{
		// add new particles 
		for (unsigned int i = 0; i < mSpawnParticle; ++i)
		{
			int unusedParticle = this->firstUnusedParticle();
			this->respawnParticle(this->mParticles[unusedParticle], _transform, minVelocity, maxVelocity, mInitialLife, mOffset);
		}

		// update all particles
		for (unsigned int i = 0; i < this->mMaxParticles; ++i)
		{
			Particle& p = this->mParticles[i];

			p.life -= _dt; // reduce life

			if (p.life >= 0.0f)
			{
				//	update particle position and velocity with gravity
				{
					p.velocity += mParticleGravity * _dt;
					p.position += p.velocity * _dt;
				}
			}
		}
	}

	void Particles::SetParticleScale(const glm::vec3& _scale)
	{
		for (auto& particle : mParticles)
			particle.scale = _scale;
	}

	void Particles::SetMaximumParticles(unsigned int _maxParticles)
	{
		mMaxParticles = _maxParticles;
		mParticles.resize(_maxParticles);
	}

	void Particles::SetDiffuseTexture(std::string _path) {
		// m_material.SetDiffuseTexture(_path);
		
		Application& app = Application::GetApplication();
		if (app.GetLoadStatus() == LoadStatus::NotRunning || app.GetLoadStatus() == LoadStatus::LoadResources) {
			m_material.SetDiffuseTexture(_path);
		} else {
			app.SubmitToLoadResourcesQueue([entID = m_EntID, texturePath = _path]() {
				Coordinator::GetInstance().GetComponent<Particles>(entID).SetDiffuseTexture(texturePath);
			});
		}
	}

	void Particles::SetSpecularTexture(std::string _path) {
		// m_material.SetSpecularTexture(_path);
		
		Application& app = Application::GetApplication();
		if (app.GetLoadStatus() == LoadStatus::NotRunning || app.GetLoadStatus() == LoadStatus::LoadResources) {
			m_material.SetSpecularTexture(_path);
		} else {
			app.SubmitToLoadResourcesQueue([entID = m_EntID, texturePath = _path]() {
				Coordinator::GetInstance().GetComponent<Particles>(entID).SetSpecularTexture(texturePath);
			});
		}
	}

	void Particles::SetModel(std::string _path) {
		 //m_model.SetModel(_path, m_EntID);
		
		Application& app = Application::GetApplication();
		if (app.GetLoadStatus() == LoadStatus::NotRunning || app.GetLoadStatus() == LoadStatus::LoadResources) {
			m_model.SetModel(_path, m_EntID);
		} else {
			app.SubmitToLoadResourcesQueue([entID = m_EntID, modelPath = _path]() {
				Coordinator::GetInstance().GetComponent<Particles>(entID).SetModel(modelPath);
			});
		}
	}

	void Particles::init()
	{
		// create this->amount default particle instances
		for (unsigned int i = 0; i < this->mMaxParticles; ++i)
			this->mParticles.push_back(Particle());
	}

	unsigned int Particles::firstUnusedParticle()
	{
		//	check spawn rate does not exceed the maximum amount of particles
		if (mSpawnParticle > mMaxParticles)
			mSpawnParticle = mMaxParticles;
		
		// first search from last used particle, this will usually return almost instantly
		for (unsigned int i = mlastUsedParticle; i < this->mMaxParticles; ++i) {
			if (this->mParticles[i].life <= 0.0f) {
				mlastUsedParticle = i;
				return i;
			}
		}
		// otherwise, do a linear search
		for (unsigned int i = 0; i < mlastUsedParticle; ++i) {
			if (this->mParticles[i].life <= 0.0f) {
				mlastUsedParticle = i;
				return i;
			}
		}
		// all particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
		mlastUsedParticle = 0;
		return 0;
	}

	void Particles::respawnParticle(Particle& _particle, Transform& _transform, glm::vec3 _minVel, glm::vec3 _maxVel, float _life, glm::vec3 _offset)
	{
		// Set particle position with random offsets and the specified offset
		_particle.position = _transform.GetPosition() + _offset;

		// Set particle life
		_particle.life = _life;

		// Set particle velocity with random values within the specified range
		float randomVelX = glm::mix(_minVel.x, _maxVel.x, static_cast<float>(rand()) / RAND_MAX);
		float randomVelY = glm::mix(_minVel.y, _maxVel.y, static_cast<float>(rand()) / RAND_MAX);
		float randomVelZ = glm::mix(_minVel.z, _maxVel.z, static_cast<float>(rand()) / RAND_MAX);

		// Set particle velocity
		_particle.velocity = glm::vec3(randomVelX, randomVelY, randomVelZ);
	}
}

RTTR_REGISTRATION
{
	using namespace AeonCore;
	RegisterManager::Instance().AddRegister([]() {
		rttr::registration::class_<Particles>("Particles")
		.constructor<EntityID, unsigned int&, unsigned int&>()
		.property_readonly("id",&Particles::GetComponentID)
		//texture
		.property("diffuse_texture",&Particles::GetDiffuseTexture, &Particles::SetDiffuseTexture)
		.property("specular_texture",&Particles::GetSpecularTexture, &Particles::SetSpecularTexture)
		//number
		.property("max_particles", &Particles::GetMaximumParticles, &Particles::SetMaximumParticles)
		.property("spawn_rate", &Particles::GetSpawnParticleRate, &Particles::SetSpawnParticleRate)
		.property("initial_life", &Particles::GetParticleLife, &Particles::SetParticleLife)
		.property("gravity", &Particles::GetParticleGravity, &Particles::SetParticleGravity)
		.property("offset", &Particles::GetParticleOffset, &Particles::SetParticleOffset)
		.property("min_velocity", &Particles::GetMinVelocity, &Particles::SetParticleMinVelocity)
		.property("max_velocity", &Particles::GetMaxVelocity, &Particles::SetParticleMaxVelocity)
		//model 
		.property("model", &Particles::GetModelPath, &Particles::SetModel); }, 14);
}

//////////////////////////////////////////////////////////////////////////////////
//// Particle class 
//////////////////////////////////////////////////////////////////////////////////
//namespace particles
//{
//    void Particle::generate(size_t maxSize)
//    {
//        m_count = maxSize;
//        m_countAlive = 0;

//        m_pos.reset(new glm::vec4[maxSize]);
//        m_col.reset(new glm::vec4[maxSize]);
//        m_startCol.reset(new glm::vec4[maxSize]);
//        m_endCol.reset(new glm::vec4[maxSize]);
//        m_vel.reset(new glm::vec4[maxSize]);
//        m_acc.reset(new glm::vec4[maxSize]);
//        m_time.reset(new glm::vec4[maxSize]);
//        m_alive.reset(new bool[maxSize]);
//    }

//    void Particle::kill(size_t id)
//    {
//        if (m_countAlive > 0)
//        {
//            m_alive[id] = false;
//            swapData(id, m_countAlive - 1);
//            m_countAlive--;
//        }
//    }

//    void Particle::wake(size_t id)
//    {
//        if (m_countAlive < m_count)
//        {
//            m_alive[id] = true;
//            swapData(id, m_countAlive);
//            m_countAlive++;
//        }
//    }

//    void Particle::swapData(size_t a, size_t b)
//    {
//        std::swap(m_pos[a], m_pos[b]);
//        std::swap(m_col[a], m_col[b]);
//        std::swap(m_startCol[a], m_startCol[b]);
//        std::swap(m_endCol[a], m_endCol[b]);
//        std::swap(m_vel[a], m_vel[b]);
//        std::swap(m_acc[a], m_acc[b]);
//        std::swap(m_time[a], m_time[b]);
//        std::swap(m_alive[a], m_alive[b]);
//    }


//    ////////////////////////////////////////////////////////////////////////////////
//    // ParticleEmitter class 
//    ////////////////////////////////////////////////////////////////////////////////

//    void ParticleEmitter::emit(double dt, particles::Particle* p)
//    {
//        const size_t maxNewParticles = static_cast<size_t>(dt * m_emitRate);
//        const size_t startId = p->m_countAlive;
//        const size_t endId = std::min(startId + maxNewParticles, p->m_count - 1);

//        for (auto& gen : m_generators)            // << gen loop
//            gen->generate(dt, p, startId, endId);

//        for (size_t i = startId; i < endId; ++i)  // << wake loop
//            p->wake(i);
//    }

//    ////////////////////////////////////////////////////////////////////////////////
//    // ParticleEmitter System 
//    ////////////////////////////////////////////////////////////////////////////////
//    ParticleSystem::ParticleSystem(size_t maxCount)
//    {
//        m_count = maxCount;
//        m_particles.generate(maxCount);
//        m_aliveParticles.generate(maxCount);

//        for (size_t i = 0; i < maxCount; ++i)
//            m_particles.m_alive[i] = false;
//    }

//    void ParticleSystem::update(double dt)
//    {
//        for (auto& em : m_emitters)
//        {
//            em->emit(dt, &m_particles);
//        }

//        for (size_t i = 0; i < m_count; ++i)
//        {
//            m_particles.m_acc[i] = glm::vec4(0.0f);
//        }

//        for (auto& up : m_updaters)
//        {
//            up->update(dt, &m_particles);
//        }

//        //ParticleData::copyOnlyAlive(&m_particles, &m_aliveParticles);
//    }

//    void ParticleSystem::reset()
//    {
//        m_particles.m_countAlive = 0;
//    }
//}

//namespace generators
//{
//    void BoxPosGen::generate(double dt, particles::Particle* p, size_t startId, size_t endId)
//    {
//        glm::vec4 posMin{ m_pos.x - m_maxStartPosOffset.x, m_pos.y - m_maxStartPosOffset.y, m_pos.z - m_maxStartPosOffset.z, 1.0 };
//        glm::vec4 posMax{ m_pos.x + m_maxStartPosOffset.x, m_pos.y + m_maxStartPosOffset.y, m_pos.z + m_maxStartPosOffset.z, 1.0 };

//        for (size_t i = startId; i < endId; ++i)
//        {
//            p->m_pos[i] = glm::linearRand(posMin, posMax);
//        }
//    }

//    void RoundPosGen::generate(double dt, particles::Particle* p, size_t startId, size_t endId)
//    {
//        for (size_t i = startId; i < endId; ++i)
//        {
//            double ang = glm::linearRand(0.0, M_PI * 2.0);
//            p->m_pos[i] = m_center + glm::vec4(m_radX * sin(ang), m_radY * cos(ang), 0.0, 1.0);
//        }
//    }

//    void BasicColorGen::generate(double dt, particles::Particle* p, size_t startId, size_t endId)
//    {
//        for (size_t i = startId; i < endId; ++i)
//        {
//            p->m_startCol[i] = glm::linearRand(m_minStartCol, m_maxStartCol);
//            p->m_endCol[i] = glm::linearRand(m_minEndCol, m_maxEndCol);
//        }
//    }

//    void BasicVelGen::generate(double dt, particles::Particle* p, size_t startId, size_t endId)
//    {
//        for (size_t i = startId; i < endId; ++i)
//        {
//            p->m_vel[i] = glm::linearRand(m_minStartVel, m_maxStartVel);
//        }
//    }

//    void SphereVelGen::generate(double dt, particles::Particle* p, size_t startId, size_t endId)
//    {
//        float phi, theta, v, r;
//        for (size_t i = startId; i < endId; ++i)
//        {
//            phi = glm::linearRand(-M_PI, M_PI);
//            theta = glm::linearRand(-M_PI, M_PI);
//            v = glm::linearRand(m_minVel, m_maxVel);

//            r = v * sinf(phi);
//            p->m_vel[i].z = v * cosf(phi);
//            p->m_vel[i].x = r * cosf(theta);
//            p->m_vel[i].y = r * sinf(theta);
//        }
//    }

//    void BasicTimeGen::generate(double dt, particles::Particle* p, size_t startId, size_t endId)
//    {
//        for (size_t i = startId; i < endId; ++i)
//        {
//            p->m_time[i].x = p->m_time[i].y = glm::linearRand(m_minTime, m_maxTime);
//            p->m_time[i].z = (float)0.0;
//            p->m_time[i].w = (float)1.0 / p->m_time[i].x;
//        }
//    }
//}

//namespace updaters
//{
//    void EulerUpdater::update(double dt, particles::Particle* p)
//    {
//        const glm::vec4 globalA{ dt * m_globalAcceleration.x, dt * m_globalAcceleration.y, dt * m_globalAcceleration.z, 0.0 };
//        const float localDT = (float)dt;

//        const unsigned int endId = p->m_countAlive;
//        for (size_t i = 0; i < endId; ++i)
//            p->m_acc[i] += globalA;

//        for (size_t i = 0; i < endId; ++i)
//            p->m_vel[i] += localDT * p->m_acc[i];

//        for (size_t i = 0; i < endId; ++i)
//            p->m_pos[i] += localDT * p->m_vel[i];
//    }

//    void FloorUpdater::update(double dt, particles::Particle* p)
//    {
//        const float localDT = (float)dt;

//        const size_t endId = p->m_countAlive;
//        for (size_t i = 0; i < endId; ++i)
//        {
//            if (p->m_pos[i].y < m_floorY)
//            {
//                glm::vec4 force = p->m_acc[i];
//                float normalFactor = glm::dot(force, glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
//                if (normalFactor < 0.0f)
//                    force -= glm::vec4(0.0f, 1.0f, 0.0f, 0.0f) * normalFactor;

//                float velFactor = glm::dot(p->m_vel[i], glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
//                //if (velFactor < 0.0)
//                p->m_vel[i] -= glm::vec4(0.0f, 1.0f, 0.0f, 0.0f) * (1.0f + m_bounceFactor) * velFactor;

//                p->m_acc[i] = force;
//            }
//        }
//    }
//    void AttractorUpdater::update(double dt, particles::Particle* p)
//    {
//        const float localDT = (float)dt;

//        const size_t endId = p->m_countAlive;
//        const size_t countAttractors = m_attractors.size();
//        glm::vec4 off;
//        float dist;
//        size_t a;
//        for (size_t i = 0; i < endId; ++i)
//        {
//            for (a = 0; a < countAttractors; ++a)
//            {
//                off.x = m_attractors[a].x - p->m_pos[i].x;
//                off.y = m_attractors[a].y - p->m_pos[i].y;
//                off.z = m_attractors[a].z - p->m_pos[i].z;
//                dist = glm::dot(off, off);

//                //if (fabs(dist) > 0.00001)
//                dist = m_attractors[a].w / dist;

//                p->m_acc[i] += off * dist;
//            }
//        }
//    }
//    void BasicColorUpdater::update(double dt, particles::Particle* p)
//    {
//        const size_t endId = p->m_countAlive;
//        for (size_t i = 0; i < endId; ++i)
//            p->m_col[i] = glm::mix(p->m_startCol[i], p->m_endCol[i], p->m_time[i].z);
//    }
//    void PosColorUpdater::update(double dt, particles::Particle* p)
//    {
//        const size_t endId = p->m_countAlive;
//        float scaler, scaleg, scaleb;
//        float diffr = m_maxPos.x - m_minPos.x;
//        float diffg = m_maxPos.y - m_minPos.y;
//        float diffb = m_maxPos.z - m_minPos.z;
//        for (size_t i = 0; i < endId; ++i)
//        {
//            scaler = (p->m_pos[i].x - m_minPos.x) / diffr;
//            scaleg = (p->m_pos[i].y - m_minPos.y) / diffg;
//            scaleb = (p->m_pos[i].z - m_minPos.z) / diffb;
//            p->m_col[i].r = scaler;// glm::mix(p->m_startCol[i].r, p->m_endCol[i].r, scaler);
//            p->m_col[i].g = scaleg;// glm::mix(p->m_startCol[i].g, p->m_endCol[i].g, scaleg);
//            p->m_col[i].b = scaleb;// glm::mix(p->m_startCol[i].b, p->m_endCol[i].b, scaleb);
//            p->m_col[i].a = glm::mix(p->m_startCol[i].a, p->m_endCol[i].a, p->m_time[i].z);
//        }
//    }
//    void VelColorUpdater::update(double dt, particles::Particle* p)
//    {
//        const size_t endId = p->m_countAlive;
//        float scaler, scaleg, scaleb;
//        float diffr = m_maxVel.x - m_minVel.x;
//        float diffg = m_maxVel.y - m_minVel.y;
//        float diffb = m_maxVel.z - m_minVel.z;
//        for (size_t i = 0; i < endId; ++i)
//        {
//            scaler = (p->m_vel[i].x - m_minVel.x) / diffr;
//            scaleg = (p->m_vel[i].y - m_minVel.y) / diffg;
//            scaleb = (p->m_vel[i].z - m_minVel.z) / diffb;
//            p->m_col[i].r = scaler;// glm::mix(p->m_startCol[i].r, p->m_endCol[i].r, scaler);
//            p->m_col[i].g = scaleg;// glm::mix(p->m_startCol[i].g, p->m_endCol[i].g, scaleg);
//            p->m_col[i].b = scaleb;// glm::mix(p->m_startCol[i].b, p->m_endCol[i].b, scaleb);
//            p->m_col[i].a = glm::mix(p->m_startCol[i].a, p->m_endCol[i].a, p->m_time[i].z);
//        }
//    }
//    void BasicTimeUpdater::update(double dt, particles::Particle* p)
//    {
//        unsigned int endId = p->m_countAlive;
//        const float localDT = (float)dt;

//        if (endId == 0) return;

//        for (size_t i = 0; i < endId; ++i)
//        {
//            p->m_time[i].x -= localDT;
//            // interpolation: from 0 (start of life) till 1 (end of life)
//            p->m_time[i].z = (float)1.0 - (p->m_time[i].x * p->m_time[i].w); // .w is 1.0/max life time		

//            if (p->m_time[i].x < (float)0.0)
//            {
//                p->kill(i);
//                endId = p->m_countAlive < p->m_count ? p->m_countAlive : p->m_count;
//            }
//        }
//    }
//}




