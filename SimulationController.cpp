#include "SimulationController.h"

SimulationController::SimulationController(int width, int height){
    bWidth = width;
    bHeight = height;
}

std::vector<std::unique_ptr<Particle>> const& SimulationController::getParticles() const {
    return particles;
}

Particle& SimulationController::createParticle(Vector2 spawn_position, Vector2 vel, double mass, int radius) {
    std::unique_ptr<Particle> ptr_particle = std::make_unique<Particle>(spawn_position, vel);
    ptr_particle->mass = mass;
    ptr_particle->massInv = 1/mass;
    ptr_particle->radius = radius;
    particles.push_back(std::move(ptr_particle));
    return *particles.at(particles.size()-1);
}

void SimulationController::randomSpawn() {
    std::random_device dev;
    std::mt19937 rng(dev());

    std::uniform_int_distribution<std::mt19937::result_type> distW((uint_fast32_t)25, (uint_fast32_t)bWidth-25);//PosX
    std::uniform_int_distribution<std::mt19937::result_type> distH((uint_fast32_t)25, (uint_fast32_t)bHeight-25);//PosY
    std::uniform_int_distribution<std::mt19937::result_type> distV((uint_fast32_t)minVel, (uint_fast32_t)maxVel); //Velocity
    std::uniform_int_distribution<std::mt19937::result_type> distD((uint_fast32_t)0, (uint_fast32_t)1); //Direction
    std::uniform_int_distribution<std::mt19937::result_type> distR((uint_fast32_t)minRadius, (uint_fast32_t)maxRadius); //Radius
    std::uniform_int_distribution<std::mt19937::result_type> distM((uint_fast32_t)minMass, (uint_fast32_t)maxMass); //Mass

    std::vector<Vector2> positions{
        Vector2(bWidth/2, bHeight/2), 
        Vector2(3*bWidth/4, 3*bHeight/4)
    };
    // int insideCount = 0;
    for (int i = 0; i < count; i++){
        double radius = distR(rng);
        bool isInside = true;
        Vector2 pos;
        int counter = 0;
        do {
            pos = Vector2(distW(rng), distH(rng));
            for (auto posi : positions) {
                if (posi.x - radius*1.5 < pos.x && pos.x < posi.x + radius*1.5) {
                    // INSIDE ANOTHER!!!!
                    isInside = true;
                    // insideCount++;
                    break;
                }
                if (posi.y - radius < pos.y && pos.y < posi.y + radius) {
                    // INSIDE ANOTHER!!!!
                    isInside = true;
                    // insideCount++;
                    break;
                }
                isInside = false;
            }
            counter++;
        }
        while (isInside && counter < 5000);

        positions.push_back(pos);
        Vector2 dir = Vector2(distD(rng) == 1 ? 1 : -1, distD(rng) == 1 ? 1 : -1 );
        Vector2 vel = Vector2(distV(rng), distV(rng))*dir;
        double radiusRatio = double(radius)/double(maxRadius);
        std::cout << "pos: " << pos << " | vel: " << vel << " | radius: " << radius <<  " | mass: " << maxMass*radiusRatio << std::endl;
        createParticle(pos, vel, maxMass*radiusRatio, static_cast<int>(radius));

    }
}

void SimulationController::markParticleForRemoval(std::unique_ptr<Particle> ptr_particle) {
    particlesMarkedForRemoval.push_back(std::move(ptr_particle));
    ptr_particle->setVisibility();
    //TODO: disable interactions
}

void SimulationController::toggleRunState() {
    isPaused = isPaused ? false : true;
}


void SimulationController::toggleConstantGravity() {
    useConstantGravity = useConstantGravity ? false: true;
}

template <typename T> bool SimulationController::loadingLoop(std::ifstream& f, 
    std::string& line,
    const std::string toFind, 
    const std::vector<std::string>& settings,
    std::map<std::string, T>& settingVarMap
) {

    if (line.find(toFind) == std::string::npos) {
        return false;
    }
    for (auto& s : settings) {
        std::getline(f, line); // Get setting name
        if (s == line) {
            std::getline(f, line); // Get setting value
            std::istringstream iss2(line);
            iss2 >> this->*settingVarMap[s];
        } else if (s == "</>") {
            return true;
        }
    }
    return true;
}


void SimulationController::load(const std::string& path) {
    
    reset(); // we always want to reset everything before we load.
    std::ifstream f(path);
    std::map<std::string, bool SimulationController::*> boolMap {
        {"useConstantGravity", &SimulationController::useConstantGravity},
        {"useBoundingBox", &SimulationController::useBoundingBox},
        {"isBouncy", &SimulationController::isBouncy},
        {"useGravitationAttraction", &SimulationController::useGravitationAttraction},
        {"random", &SimulationController::random}
    };

    std::map<std::string, double SimulationController::*> doubleMap {
        {"timestepScaling", &SimulationController::timestepScaling},
        {"floorBounciness", &SimulationController::floorBounciness},
    };

    std::map<std::string, int SimulationController::*> intMap {
        {"count", &SimulationController::count},
        {"interval", &SimulationController::interval}
    };

    std::string line;
    bool loadingParticles = false;
    bool loadedBool = false;
    bool loadedDouble = false;
    bool loadedInt = false;
    try
    {
        while (std::getline(f, line)) {
            if (!loadedBool) {
                loadedBool = loadingLoop<bool SimulationController::*>(f, line, std::string("<bool>"), boolSettings, boolMap);
            }
            if (!loadedDouble) {
                loadedDouble = loadingLoop<double SimulationController::*>(f, line, std::string("<double>"), doubleSettings, doubleMap);
            }
            if (!loadedInt) {
                loadedInt = loadingLoop<int SimulationController::*>(f, line, std::string("<int>"), intSettings, intMap);
            }
           
            if (loadingParticles) {
                std::istringstream iss(line);
                double mass, radius;
                Vector2 pos;
                Vector2 vel;
                if (!(iss >> pos >> vel >> mass >> radius)) { continue; }
                createParticle(pos, vel, mass, static_cast<int>(radius));
            }

            if (line.find("<particles>") != std::string::npos) {
                loadingParticles = true;
                continue;
            }
            if (line.find("</particles>") != std::string::npos) {
                loadingParticles = false;
                continue;
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void SimulationController::loadDefault() {
    load(defaultConfig);
    std::cout << "Loading default config." << std::endl;
}

void SimulationController::reset() {
    particles.clear();
}

// void SimulationController::removeParticle(std::unique_ptr<Particle> ptr_particle) {
//     // TODO: implement removal
// }
void SimulationController::step(double timestep) {

    if (isPaused) {
        return;
    }
    
    // Basic spawning functionality
    if (false && countTracker <= count && intervalTracker >= interval) {
        createParticle(Vector2(10 + countTracker*25 % bWidth, 20), Vector2(0, gravity_acc_e));
        countTracker++;
        intervalTracker = 0;
    } else {
        intervalTracker++;
    }

    double timestepScaled = timestep * timestepScaling;
    for (auto& particle : particles) {
    
        calculateVelocity(particle, timestepScaled);
        Vector2 futurePos = particle->calculateFuturePos(timestepScaled);
        // std::cout << "vel: " << particle->velocity << std::endl;
        particle->setPosition(futurePos);
    }
}


Vector2 SimulationController::calculateGravAttAcceleration(std::unique_ptr<Particle>& particle) {
    Vector2& pos = particle->getPosition();
    Vector2 totalAccVec = Vector2();
    for (auto& otherP : particles) {
        if (particle == otherP) continue;
        Vector2& otherPos = otherP->getPosition();
        double distance = pos.distanceTo(otherPos);
        double radii = particle->radius + otherP->radius;
        Vector2 dir = pos.directionTo(otherPos, distance);
        
        double accDirect = (G_const*otherP->mass)/(distance*distance);
        if (distance < radii*gravCalcDistTol) {
            // dir * distance
            // particle->setPosition(pos + dir*(distance - otherP->radius - particle->radius));
            // particle->velocity 
            // totalAccVec -= accDirect * dir;
        } else if (distance > radii*gravCalcDistTol) {
            totalAccVec += accDirect * dir;
        }
         
    }
    particle->totalAcc = totalAccVec;
    // std::cout << "Total Force: " << totalForceVec << std::endl;
    return totalAccVec;
}

void SimulationController::resolveCollision(std::unique_ptr<Particle>& particle, double const& timestepScaled) {
    // Vector2 pos = particle->calculateFuturePos(timestepScaled);
    Vector2 pos = particle->getPosition();
    
    for (auto& otherP : particles) {
        if (particle == otherP) continue;
        double radii = (particle->radius + otherP->radius);
        double distance = pos.distanceToSq(otherP->getPosition());
        if (distance > radii * radii) {
            continue;
        }
        Vector2 colDir = pos.directionTo(otherP->getPosition());
        double velDir = (otherP->velocity - particle->velocity).dot(colDir);
        if (velDir > 0) {
            return;
        }
        double impulseScalar = -(1 - particleBounciness) * velDir;
        // double impulseScalar = -(1 - particleBounciness) * velDir / (particle->massInv + otherP->massInv);
        
        Vector2 impulse = impulseScalar * colDir;
        
        double massSum = particle->massInv + otherP->massInv;
        double massRatio = particle->massInv / massSum;

        particle->velocity -= massRatio * impulse;

        // std::cout << "velDir: " << velDir << " | mass: " << particle->mass
        // << " | massRatio: " << massRatio << " | impulse: " << impulse << std::endl;

        massRatio = otherP->massInv / massSum;
        otherP->velocity += massRatio * impulse;

        double percent = 0.5;
        Vector2 correction = ((radii - distance) * massSum) * percent * colDir;
        pos -= particle->massInv * correction;
        otherP->getPosition() += otherP->massInv * correction;
        // std::cout << "correction: " << correction << " | mass: " << particle->mass
        // << " | objA: " << particle->massInv * correction << " | objB: " << otherP->massInv * correction << std::endl;
    }
    return;
}

void SimulationController::calculateVelocity(std::unique_ptr<Particle>& particle, const double& timestepScaled) {
    // Vector2 colVec = resolveCollision(particle, timestepScaled);
    // particle->velocity = colVec;
    resolveCollision(particle, timestepScaled);

    if (useGravitationAttraction) {
        Vector2 gravAttAcc = calculateGravAttAcceleration(particle);
        particle->velocity += gravAttAcc * timestepScaled;
    }

    Vector2 pos = particle->getPosition();
    bool isGrounded = particle->getPosition().y + particle->radius >= bHeight;

    if (useConstantGravity && (!useBoundingBox or !isGrounded)) {
        particle->velocity.y += leme_sim::gravity_acc_e * timestepScaled;
    }
    
    if (!useBoundingBox) {
        return;
    }

    if (isGrounded) {
        if (particle->velocity.y > 0) {
            particle->velocity.y = -floorBounciness * particle->velocity.y;
            particle->getPosition().y = bHeight - particle->radius;
        } else if (abs(particle->velocity.y) < 0.1) {
            particle->velocity.y = 0;
        }
    } else if (pos.y - particle->radius < 0) {
        if (particle->velocity.y < 0) {
            particle->velocity.y = -floorBounciness * particle->velocity.y;
            particle->getPosition().y = particle->radius;
        } else if (abs(particle->velocity.y) < 0.1) {
            particle->velocity.y = 0;
        }
    }

    if (pos.x - particle->radius < 0) {
        if (particle->velocity.x < 0) {
            particle->velocity.x = -floorBounciness * particle->velocity.x;
            particle->getPosition().x = particle->radius;
        } else if (abs(particle->velocity.x) < 0.1) {
            particle->velocity.x = 0;
        }
    } else if (pos.x + particle->radius > bWidth) {
        if (particle->velocity.x > 0) {
            particle->velocity.x = -floorBounciness * particle->velocity.x;
            particle->getPosition().x = bWidth - particle->radius;
        } else if (abs(particle->velocity.x) < 0.1) {
            particle->velocity.x = 0;
        }
    }
}