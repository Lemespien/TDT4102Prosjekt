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

using namespace leme_sim;

class SimulationController {
    private:
        static const std::vector<std::string> boolSettings;
        static const std::vector<std::string> doubleSettings;
        std::vector<std::unique_ptr<Particle>> particles;
        std::vector<std::unique_ptr<Particle>> particlesMarkedForRemoval;
        int bWidth, bHeight;
        Stopwatch stopwatch;
        
        static constexpr double gravCalcDistTol = 1.01;
        static constexpr int everyX = 50;
        static constexpr int partCount = 00;
        int currentCount = 0;
        int everyCount = 0;
        
        double timestepScaling = 1;

        bool useConstantGravity = false;
        bool useGravitationAttraction = true;
        bool isBouncy = true;
        bool useBoundingBox = true;
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
        void step();
        void toggleRunState();
        void toggleConstantGravity();
        void load(std::string& path);
};
