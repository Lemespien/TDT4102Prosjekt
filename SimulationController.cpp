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
    // std::cout << "randomSpawn" << std::endl;
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
        double radiusRatio = 1;
        if (massBasedRadius) {
            radiusRatio = double(radius)/double(maxRadius);
        }
        // std::cout << "pos: " << pos << " | vel: " << vel << " | radius: " << radius <<  " | mass: " << maxMass*radiusRatio << std::endl;
        createParticle(pos, vel, maxMass*radiusRatio, static_cast<int>(radius));

    }
    // std::cout << "finished spawning: " << particles.size() << std::endl;
}

void SimulationController::markParticleForRemoval(std::unique_ptr<Particle> ptr_particle) {
    particlesMarkedForRemoval.push_back(std::move(ptr_particle));
    ptr_particle->setVisibility();
    //TODO: disable interactions
}

void SimulationController::toggleRunState() {
    isPaused = !isPaused;
}


void SimulationController::toggleConstantGravity() {
    useConstantGravity = useConstantGravity ? false: true;
}

template <typename T> bool SimulationController::loadingLoop(std::ifstream& f, 
    std::string& line,
    const std::string toFind,
    std::map<std::string, T>& settingVarMap
) {

    if (line.find(toFind) == std::string::npos) {
        return false;
    }
    do
    {
        std::getline(f, line); // Get setting name
        
        for (auto s : settingVarMap) {
            if (s.first == line) {
                std::getline(f, line); // Get setting value
                std::istringstream iss2(line);
                iss2 >> this->*settingVarMap[s.first];
            } else if (line == "</>") {
                return true;
            }
        }
    } while (line != "</>");
    
    return true;
}

void SimulationController::load(const std::string& path, bool isDefault) {
    if (!isDefault) {
        reset(); // we always want to reset everything before we load.
    }
    // std::cout << "loading: " << path << std::endl;
    std::ifstream f(path);

    std::string line;
    bool loadingParticles = false;
    bool loadedBool = false;
    bool loadedDouble = false;
    bool loadedInt = false;
    try
    {
        while (std::getline(f, line)) {
            if (!loadedBool) {
                loadedBool = loadingLoop<bool SimulationController::*>(f, line, std::string("<bool>"), boolMap);
                // std::cout << "loading bool" << std::endl;
            }
            if (!loadedDouble) {
                loadedDouble = loadingLoop<double SimulationController::*>(f, line, std::string("<double>"), doubleMap);
                // std::cout << "loading double" << std::endl;
            }
            if (!loadedInt) {
                loadedInt = loadingLoop<int SimulationController::*>(f, line, std::string("<int>"), intMap);
                // std::cout << "loading int" << std::endl;
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
        randomSpawn();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

void SimulationController::loadDefault() {
    load(defaultConfig, true);
    std::cout << "Loading default config." << std::endl;
}

void SimulationController::reset() {
    particles.clear();
    loadDefault();
}

// void SimulationController::removeParticle(std::unique_ptr<Particle> ptr_particle) {
//     // TODO: implement removal
// }
double prevTimestep = 0;
void SimulationController::step(double timestep) {

    if (isPaused) {
        return;
    }
    double timestepScaled = timestep * timestepScaling;
    for (auto& particle : particles) {
        // if (isnan(particle->getPosition().x)) {
        //     isPaused = true;
        //     break;
        // }
        calculateVelocity(particle, timestepScaled);
        Vector2 futurePos = particle->calculateFuturePos(timestepScaled);
        // std::cout << "vel: " << particle->velocity << std::endl;
        particle->setPosition(futurePos);
    }
    prevTimestep = timestepScaled;
}


Vector2 SimulationController::calculateGravAttAcceleration(std::unique_ptr<Particle>& particle) {
    Vector2& pos = particle->getPosition();
    Vector2 totalAccVec = Vector2(0, 0);
    for (auto& otherP : particles) {
        if (particle == otherP) continue;
        Vector2& otherPos = otherP->getPosition();
        double distance = pos.distanceTo(otherPos);
        double radii = particle->radius + otherP->radius;
        
        if (distance < radii*gravCalcDistTol) {
            if (distance < radii) {
                double percent = 0.2;
                double massSum = (particle->massInv + otherP->massInv);
                Vector2 correction = -((pos.distanceTo(otherP->getPosition()) - radii)) / massSum * percent * pos.directionTo(otherP->getPosition());
                pos -= particle->massInv * correction;
                otherP->getPosition() += otherP->massInv * correction;
            }
            continue;
        }
        double accDirect = (G_const*otherP->mass)/(distance*distance);
        Vector2 dir = pos.directionTo(otherPos, distance);
        totalAccVec += accDirect * dir;
    }
    return totalAccVec;
}
void SimulationController::resolveCollision(std::unique_ptr<Particle>& particle, double const& timestepScaled) {
    Vector2 pos = particle->calculateFuturePos(timestepScaled);
    
    for (auto& otherP : particles) {
        if (particle == otherP) continue;
        double radii = (particle->radius + otherP->radius);
        
        if (pos.distanceToSq(otherP->getPosition()) > radii * radii) {
            continue;
        }
        Vector2 colDir = pos.directionTo(otherP->getPosition());
        double massSum = (particle->mass + otherP->mass);
        Vector2 vel_3 = (2 * particle->velocity * particle->mass + otherP->velocity*(otherP->mass - particle->mass))/massSum;
        Vector2 vel_2 = particle->velocity + otherP->mass/particle->mass*(otherP->velocity - vel_3);
        // std::cout << "vel_2: " << vel_2 << " | vel_3: " << vel_3 << std::endl;
        particle->velocity = vel_2;
        otherP->velocity = vel_3;
    }
}

void SimulationController::calculateVelocity(std::unique_ptr<Particle>& particle, const double& timestepScaled) {
    // Vector2 colVec = resolveCollision(particle, timestepScaled);
    // particle->velocity = colVec;
    resolveCollision(particle, timestepScaled);

    if (useGravitationAttraction) {
        particle->acceleration = calculateGravAttAcceleration(particle);
        particle->velocity += particle->acceleration * timestepScaled;
        // std::cout << "acc: " << particle->acceleration << " | velocity: " << particle->velocity << " | vel_d: " << particle->acceleration * timestepScaled << std::endl;
    }

    Vector2 pos = particle->getPosition();
    bool isGrounded = particle->getPosition().y + particle->radius >= bHeight;

    if (useConstantGravity && (!useBoundingBox or !isGrounded)) {
        particle->velocity.y += leme_sim::gravity_acc_e * timestepScaled;
        // std::cout << "grav: " << leme_sim::gravity_acc_e << " | velocity: " << particle->velocity << " | vel_d: " << leme_sim::gravity_acc_e * timestepScaled << std::endl;
    }
    
    if (!useBoundingBox) {
        return;
    }

    if (isGrounded) {
        if (particle->velocity.y > 0) {
            particle->velocity.y = -floorBounciness * particle->velocity.y;
            particle->getPosition().y = bHeight - particle->radius;
        } 
        // else if (abs(particle->velocity.y) < 5.0) {
        //     particle->velocity.y = 0;
        // }
    } else if (pos.y - particle->radius < 0) {
        if (particle->velocity.y < 0) {
            particle->velocity.y = -floorBounciness * particle->velocity.y;
            particle->getPosition().y = particle->radius;
        } 
        // else if (abs(particle->velocity.y) < 5.0) {
        //     particle->velocity.y = 0;
        // }
    }

    if (pos.x - particle->radius < 0) {
        if (particle->velocity.x < 0) {
            particle->velocity.x = -floorBounciness * particle->velocity.x;
            particle->getPosition().x = particle->radius;
        } 
        // else if (abs(particle->velocity.x) < 5.0) {
        //     particle->velocity.x = 0;
        // }
    } else if (pos.x + particle->radius > bWidth) {
        if (particle->velocity.x > 0) {
            particle->velocity.x = -floorBounciness * particle->velocity.x;
            particle->getPosition().x = bWidth - particle->radius;
        } 
        // else if (abs(particle->velocity.x) < 5.0) {
        //     particle->velocity.x = 0;
        // }
    }
}