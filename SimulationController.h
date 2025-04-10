#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include <random>
#include <fstream>

#include "Particle.h"
#include "Stopwatch.h"
#include "PhysicsConstants.h"

#include <map>
#include <variant>

using namespace leme_sim;

class SimulationController {
    private:
        static const std::vector<std::string> boolSettings;
        static const std::vector<std::string> doubleSettings;
        static const std::vector<std::string> intSettings;
        std::vector<std::unique_ptr<Particle>> particles;
        std::vector<std::unique_ptr<Particle>> particlesMarkedForRemoval;
        int bWidth, bHeight;
        Stopwatch stopwatch;
        
        // Simulation settings - Should probably be a settings file
        static constexpr double gravCalcDistTol = 1.01;
        double timestepScaling = 1;
        double floorBounciness = 0.55;
        bool useConstantGravity = false;
        bool useGravitationAttraction = true;
        bool isBouncy = true;
        bool useBoundingBox = true;

        // Spawner - Move to seperate file.
        int interval = 50;
        int count = 0; // Amount to spawn
        int currentCount = 0; // tracks 
        int everyCount = 0;
        bool random = false;

    public:
        bool isPaused = false;
        SimulationController(int width, int height);
        std::vector<std::unique_ptr<Particle>> const& getParticles() const;
        Particle& createParticle(Vector2 spawn_position, Vector2 vel = Vector2{0, 0}, double mass = 10, int radius = 10);
        void randomSpawn();
        void markParticleForRemoval(std::unique_ptr<Particle> ptr_particle);
        void removeParticle(std::unique_ptr<Particle> ptr_particle);
        Vector2 calculateGravitationalForces(std::unique_ptr<Particle>& particle);
        Vector2 calculateCollisions(std::unique_ptr<Particle>& particle, double const& timestepScaled);
        void calculateVelocity(std::unique_ptr<Particle>& particle, const double& timestepScaled);
        void step();
        void toggleRunState();
        void toggleConstantGravity();
        void load(std::string& path);
        void reset();
        
        template <typename T> 
        bool loadingLoop(std::ifstream& f, 
            std::string& line,
            const std::string toFind, 
            const std::vector<std::string>& settings,
            std::map<std::string, T>& settingVarMap);
};
