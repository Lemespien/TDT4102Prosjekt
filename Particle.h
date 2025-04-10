#pragma once
#include "Point.h"
#include <vector>
#include "Vector2.h"

class Particle {
    private:
        Vector2 position;
        bool isVisible = true;
        TDT4102::Point intPosition;
    public:
        int radius = 10;
        double mass = 10;
        double massInv = 1/10;
        Vector2 velocity = Vector2(0, 0);
        Vector2 acceleration = Vector2(0, 0);
        Vector2 totalAcc = Vector2(0, 0);
        Vector2 futurePos = Vector2(0,0);
        Particle(Vector2 spawn_location, Vector2 vel = Vector2{0, 0});
        Particle(double x, double y, double p_hVel = 0, double p_vVel = 0);
        Particle();
        Vector2& getPosition();
        void setPosition(const double x, const double y);
        void setPosition(const Vector2& newPos);

        TDT4102::Point getIntPosition();
        void setVisibility(bool visible = false);
        void setVelocity(double hor, double vert);
        void applyForce(Vector2 forceVec);
        Vector2& calculateFuturePos(double timestep);
};