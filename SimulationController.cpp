#include "SimulationController.h"

const std::vector<std::string> SimulationController::boolSettings {
    "useConstantGravity",
    "useBoundingBox",
    "isBouncy",
    "useGravitationAttraction",
};

const std::vector<std::string> SimulationController::doubleSettings {
    "timestepScaling",
    "floorBounciness"
};

const std::vector<std::string> SimulationController::intSettings {
    "count",
    "interval",
    "random"
};

SimulationController::SimulationController(int width, int height){
    bWidth = width;
    bHeight = height;
    
    // std::string loadPath = "save1.lemesave";
    // load(loadPath);
    // useGravitationAttraction = false;
    // isBouncy = false;
    // useConstantGravity = false;
    // useBoundingBox = false;
    isPaused = true;
    randomSpawn();
    // createParticle(Vector2(width/2, height/2), Vector2(0, 0), leme_sim::mass_sun, 32);
    // createParticle(Vector2(width/2 -856, height/2), Vector2(0, 200), leme_sim::mass_earth, 4);
    // createParticle(Vector2(width/2 +856, height/2), Vector2(0, -200), leme_sim::mass_earth*4, 8);
    // createParticle(Vector2(width/2, height/4), Vector2(400, 0), leme_sim::mass_earth, 4);
    // createParticle(Vector2(width/2+200, height*3/4), Vector2(-400, 0), leme_sim::mass_earth, 4);
    // createParticle(Vector2(width/3, height/3), Vector2(5, 5), 10000, 60);
    // createParticle(Vector2(2*width/3, 2*height/3), Vector2(-5, -5), 5000, 40);
    
    // createParticle(Vector2(50, 50), Vector2(20, 0), 10);
    // createParticle(Vector2(50, 500), Vector2(0, 0), 10);
    // createParticle(Vector2(width-100, 50), Vector2(-20, 0), 10);
    // createParticle(Vector2(width-100, 500), Vector2(0, 0), 10);
}

std::vector<std::unique_ptr<Particle>> const& SimulationController::getParticles() const {
    return particles;
}

Particle& SimulationController::createParticle(Vector2 spawn_position, Vector2 vel, double mass, int radius) {
    std::unique_ptr<Particle> ptr_particle = std::make_unique<Particle>(spawn_position, vel);
    ptr_particle->mass = mass;
    ptr_particle->radius = radius;
    particles.push_back(std::move(ptr_particle));
    return *particles.at(particles.size()-1);
}

void SimulationController::randomSpawn() {
    std::random_device dev;
    std::mt19937 rng(dev());

    std::uniform_int_distribution<std::mt19937::result_type> distW(25, bWidth-25);//PosX
    std::uniform_int_distribution<std::mt19937::result_type> distH(25, bHeight-25);//PosY
    std::uniform_int_distribution<std::mt19937::result_type> distV(1, 5); //Velocity
    std::uniform_int_distribution<std::mt19937::result_type> distD(0, 1); //Direction
    std::uniform_int_distribution<std::mt19937::result_type> distR(2, 6); //Radius
    std::uniform_int_distribution<std::mt19937::result_type> distM(50, 100); //Mass

    std::vector<Vector2> positions{
        Vector2(bWidth/2, bHeight/2), 
        Vector2(3*bWidth/4, 3*bHeight/4)
    };
    int insideCount = 0;
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
                    insideCount++;
                    break;
                }
                if (posi.y - radius < pos.y && pos.y < posi.y + radius) {
                    // INSIDE ANOTHER!!!!
                    isInside = true;
                    insideCount++;
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
        std::cout << "pos: " << pos << " | vel: " << vel << " | inside count: " << insideCount << std::endl;

        auto testy = createParticle(pos, Vector2(0, 0), distM(rng), radius);

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

void SimulationController::load(std::string& path) {
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
            std::istringstream iss(line);
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
                double posX, posY, velX, velY, mass;
                double radius;
                Vector2 pos;
                Vector2 vel;
                if (!(iss >> pos >> vel >> mass >> radius)) { continue; }
                createParticle(pos, vel, mass, radius);
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
void SimulationController::reset() {
    particles.clear();
}

// void SimulationController::removeParticle(std::unique_ptr<Particle> ptr_particle) {
//     // TODO: implement removal
// }

void SimulationController::step() {

    if (!stopwatch.isRunning) {
        stopwatch.start();
        return;
    }

    double timestep = stopwatch.stop();

    if (isPaused) {
        return;
    }
    
    // Basic spawning functionality
    if (false && currentCount <= count && everyCount >= interval) {
        createParticle(Vector2(10 + currentCount*25 % bWidth, 20), Vector2(0, gravity_acc_e));
        currentCount++;
        everyCount = 0;
    } else {
        everyCount++;
    }

    double timestepScaled = timestep * timestepScaling;
    double frameRate = 1 / timestep;
    // std::cout << "Framerate: " << frameRate << " | Timestep: " << timestep << std::endl;
    for (auto& particle : particles) {
        Vector2 totalForceVec = Vector2(0, 0);
    //     /*
    //     Calculate gravitational pull towards other particles
    //     */
    //    if (useGravitationAttraction) {
    //        totalForceVec = calculateGravitationalForces(particle);
    //     }
    //     particle->applyForce(totalForceVec);
    //     particle->velocity += particle->acceleration * timestepScaled;
        
    //     /*
    //     Check for collisions with other particles
    //     */
    //    particle->velocity = calculateCollisions(particle, timestepScaled);
    //    auto futurePos = particle->calculateFuturePos(timestepScaled);
    //     //std::cout << "Vel: " << particle->velocity << std::endl;

    // // Collision detection for bounding box
    //     if (useBoundingBox) {
    //         if (futurePos.x - particle->radius > 0 && futurePos.x + particle->radius < bWidth) {
    //             // newPos.x = futurePos.x;
    //         } else {
    //             if (futurePos.x - particle->radius <= 0) {
    //                 futurePos.x = particle->radius;
    //             } else {
    //                 futurePos.x = bWidth - particle->radius;
    //             }
    //             if (isBouncy) {
    //                 particle->velocity.x *= -1;
    //             }
    //         }
    
    //         if ((futurePos.y - particle->radius > 0) && (futurePos.y + particle->radius < bHeight)) {
    //             futurePos.y = futurePos.y;
    //         } else {
    //             if (futurePos.y - particle->radius <= 0) {
    //                 futurePos.y = particle->radius;
    //             } else {
    //                 futurePos.y = bHeight - particle->radius;
    //             }
    //             if (isBouncy) {
    //                 particle->velocity.y *= -1;
    //             }
    //         }
    //     }
        
    
        calculateVelocity(particle, timestepScaled);
        // particle->applyForce(totalForceVec);
        // particle->velocity += particle->acceleration * timestepScaled;

        Vector2 futurePos = particle->calculateFuturePos(timestepScaled);


        particle->setPosition(futurePos);
    }
    stopwatch.start();
}


Vector2 SimulationController::calculateGravitationalForces(std::unique_ptr<Particle>& particle) {
    Vector2& pos = particle->getPosition();
    Vector2 totalForceVec = Vector2(0, 0);
    bool shouldAddGravity = true;
    for (auto& otherP : particles) {
        if (particle == otherP) continue;
        Vector2& otherPos = otherP->getPosition();
        double distance = pos.distanceTo(otherPos);
        double radii = particle->radius + otherP->radius;
        Vector2 dir = pos.directionTo(otherPos, distance);
        if ((radii/gravCalcDistTol < distance) 
            && (distance < radii*gravCalcDistTol)) {
                // N force + G force = 0
                // std::cout << "No force" << std::endl;
                if (dir.y > 0.85) {
                    shouldAddGravity = false;
                }
                continue;
        } else {
            double force = (G_const*particle->mass*otherP->mass)/(distance*distance);
            if (distance < radii*gravCalcDistTol) {
                totalForceVec -= force * dir;
                if (dir.y > 0.85) {
                    shouldAddGravity = false;
                }
                // std::cout << "Dir: " << dir << "Normal: " << -force * dir << " | Gravity: " <<  leme_sim::gravity_acc_e*particle->mass << std::endl;
            } else {
                totalForceVec += force * dir;
                // std::cout << "Force: " << force * dir << std::endl;
            }
        }
         
    }
    if (useConstantGravity) {
        if (shouldAddGravity) {
            totalForceVec += Vector2(0, leme_sim::gravity_acc_e*particle->mass);
            // std::cout << "Adding gravity | Mass: " << particle->mass << std::endl;
        }
        if ((pos.y + particle->radius)*gravCalcDistTol > bHeight) {
            // Normalforce from ground. Cancels gravity.
    
            totalForceVec -= Vector2(0, leme_sim::gravity_acc_e*particle->mass);
        }
    }

    // std::cout << "Total Force: " << totalForceVec << std::endl;
    return totalForceVec;
}

Vector2 SimulationController::calculateCollisions(std::unique_ptr<Particle>& particle, double const& timestepScaled) {
    Vector2 futurePos = particle->calculateFuturePos(timestepScaled);
    Vector2 collisionVel = particle->velocity;
    for (auto& otherP : particles) {
        if (particle == otherP) continue;
        double radii = (particle->radius + otherP->radius) *gravCalcDistTol;
        if (futurePos.distanceTo(otherP->getPosition()) < radii) {
            Vector2 v3 = Vector2(0, 0);
            if (isBouncy) {
                Vector2 dir = particle->getPosition().directionTo(otherP->getPosition());
                v3 = (2 * collisionVel*particle->mass + otherP->velocity*(otherP->mass - particle->mass))/(particle->mass + otherP->mass);
                collisionVel += otherP->mass/particle->mass*(otherP->velocity - v3);
                collisionVel = collisionVel.magnitude()*-dir;
                otherP->velocity = v3.magnitude()*dir;
            }
        }
    }
    // if (futurePos.y + particle->radius > bHeight && collisionVel.y > 0) {
    //     collisionVel.y = 0;
    // }
    return collisionVel;
}

void SimulationController::calculateVelocity(std::unique_ptr<Particle>& particle, const double& timestepScaled) {

    Vector2 colVec = calculateCollisions(particle, timestepScaled);
    particle->velocity = colVec;

    Vector2 pos = particle->getPosition();
    bool isGrounded = particle->getPosition().y + particle->radius >= bHeight;

    if (useConstantGravity && (!useBoundingBox or !isGrounded)) {
        particle->velocity.y += leme_sim::gravity_acc_e * timestepScaled;
    }
    
    if (!useBoundingBox) {
        return;
    }

    if (isGrounded) {
        // If on the 'ground', should deaccelerate
        if (particle->velocity.y > 0) {
            particle->velocity.y = -floorBounciness * particle->velocity.y;
        } else if (abs(particle->velocity.y) < 0.1) {
            particle->velocity.y = 0;
        }
    } else if (pos.y - particle->radius < 0) {
        if (particle->velocity.y < 0) {
            particle->velocity.y = -floorBounciness * particle->velocity.y;
        } else if (abs(particle->velocity.y) < 0.1) {
            particle->velocity.y = 0;
        }
    }

    if (pos.x - particle->radius < 0) {
        if (particle->velocity.x < 0) {
            particle->velocity.x = -floorBounciness * particle->velocity.x;
        } else if (abs(particle->velocity.x) < 0.1) {
            particle->velocity.x = 0;
        }
    } else if (pos.x + particle->radius > bWidth) {
        if (particle->velocity.x > 0) {
            particle->velocity.x = -floorBounciness * particle->velocity.x;
        } else if (abs(particle->velocity.x) < 0.1) {
            particle->velocity.x = 0;
        }
    }
}