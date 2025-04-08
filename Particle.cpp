#include "Particle.h"

Particle::Particle(Vector2 spawn_location, Vector2 vel) {
    position = spawn_location;
    velocity = vel;
}

Particle::Particle(double x, double y, double hVel, double vVel) : Particle::Particle(Vector2{x, y}, Vector2(hVel, vVel)){}

Particle::Particle(){}

Vector2& Particle::getPosition(){
    return position;
}

void Particle::setPosition(const double x, const double y) {
    position.x = x;
    position.y = y;
}

void Particle::setPosition(const Vector2& newPos) {
    setPosition(newPos.x, newPos.y);
}

TDT4102::Point Particle::getIntPosition() {
    return TDT4102::Point{static_cast<int>(position.x), static_cast<int>(position.y)};
}

void Particle::setVisibility(bool visible) {
    isVisible = visible;
}

void Particle::setVelocity(const double hor, const double vert) {
    velocity.x = hor;
    velocity.y = vert;
}

void Particle::applyForce(Vector2 forceVec) {
    acceleration = forceVec/mass;
}

Vector2& Particle::calculateFuturePos(double timestep) {
    futurePos = position + velocity * timestep;
    // futurePos = Vector2(position.x + velocity.x * timestep, position.y + velocity.y * timestep);
    return futurePos;
}