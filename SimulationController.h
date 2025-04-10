#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include <random>
#include <fstream>
#include <sstream>
#include <map>

#include "Particle.h"
#include "PhysicsConstants.h"

using namespace leme_sim;

class SimulationController {
    private:
        const std::vector<std::string> boolSettings {
            "useConstantGravity",
            "useBoundingBox",
            "isBouncy",
            "useGravitationAttraction",
        };
        
        const std::vector<std::string> doubleSettings {
            "timestepScaling",
            "floorBounciness"
        };
        
        const std::vector<std::string> intSettings {
            "count",
            "interval",
            "random"
        };
        std::vector<std::unique_ptr<Particle>> particles;
        std::vector<std::unique_ptr<Particle>> particlesMarkedForRemoval;
        const std::string defaultConfig = "default.lemesave";
        int bWidth, bHeight;

        static constexpr double gravCalcDistTol = 1.05;
        /* ----------------------------
        NOTE:

        Most of these settings can be (and are) changed 
        in .lemesave files

        -------------------------------
        */

        // Simulation settings - Should probably be a settings file
        double timestepScaling = 1;
        double floorBounciness = 0.55;
        bool useConstantGravity = false;
        bool useGravitationAttraction = true;
        bool isBouncy = true;
        bool useBoundingBox = true;

        // Spawner - Move to seperate file.
        int interval = 50;
        int intervalTracker = 0; 
        int count = 100; // Amount to spawn
        int countTracker = 0; // Tracker 
        int minRadius = 3;
        int maxRadius = 10;
        double minVel = 0;
        double maxVel = 0;
        double minMass = 1e3;
        double maxMass = 2.5e3;
        bool random = false;

    public:
        bool isPaused = false;
        SimulationController(int width, int height);
        std::vector<std::unique_ptr<Particle>> const& getParticles() const;
        Particle& createParticle(Vector2 spawn_position, Vector2 vel = Vector2{0, 0}, double mass = 10, int radius = 10);
        void randomSpawn();
        void markParticleForRemoval(std::unique_ptr<Particle> ptr_particle);
        void removeParticle(std::unique_ptr<Particle> ptr_particle);
        Vector2 calculateGravAttAcceleration(std::unique_ptr<Particle>& particle, const double& timestepScaled);
        Vector2 calculateCollisions(std::unique_ptr<Particle>& particle, double const& timestepScaled);
        void calculateVelocity(std::unique_ptr<Particle>& particle, const double& timestepScaled);
        void step(double timestep = -1);
        void toggleRunState();
        void toggleConstantGravity();
        void load(const std::string& path);
        void loadDefault();
        void reset();
        
        template <typename T> 
        bool loadingLoop(std::ifstream& f, 
            std::string& line,
            const std::string toFind, 
            const std::vector<std::string>& settings,
            std::map<std::string, T>& settingVarMap);
};
