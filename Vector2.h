#pragma once
#include <math.h>
#include <iostream>

struct Vector2 {
    double x = 0;
    double y = 0;
    Vector2(double x = 0, double y = 0) {
        this->x = x;
        this->y = y;
    }
    double distanceTo(Vector2& otherVec) {
        return std::sqrt(pow(otherVec.x - x, 2) + pow(otherVec.y - y, 2));
    }

    double distanceToSq(Vector2& otherVec) {
        return pow(otherVec.x - x, 2) + pow(otherVec.y - y, 2);
    }

    Vector2 directionTo(Vector2& otherVec, double magnitude = -1.001) {
        auto dir = Vector2(otherVec.x - x, otherVec.y - y);
        if (magnitude == -1.001) {
            magnitude = distanceTo(otherVec);
        }
        return dir/magnitude;
    }

    bool isNonZero() {
        return x != 0 || y != 0;
    }

    double mag_squared() {
        return pow(x, 2) + pow(y, 2);
    }

    double magnitude() {
        return sqrt(pow(x, 2) + pow(y, 2));
    }

    double dot(Vector2& rhs) const {
        return x + rhs.x + y + rhs.y;
    }

    Vector2 operator+(double a) const {
        return Vector2(x + a, y + a);
    }

    Vector2 operator-() const {
        return Vector2(-x, -y);
    }

    Vector2 operator-(Vector2 const& rhs) const {
        return Vector2(x - rhs.x, y - rhs.y);
    }

    Vector2 operator-(double a) const {
        return Vector2(x - a, y - a);
    }

    // Not proper vector multiplication
    Vector2 operator*(Vector2 const& rhs) {
        return Vector2(x*rhs.x, y*rhs.y);
    }
    
    Vector2 operator*(double val) {
        return Vector2(val*x, val*y);
    }
    
    Vector2 operator/(double val) {
        return Vector2(x/val, y/val);
    }

    Vector2 operator/(Vector2 const& rhs) {
        return Vector2(x/rhs.x, y/rhs.y);
    }

};

inline Vector2 operator*(Vector2 const& lhs, double val) {
    return Vector2(lhs.x*val, lhs.y*val);
}
inline Vector2 operator*(double val, Vector2 const& rhs) {
    return Vector2(val*rhs.x, val*rhs.y);
}

inline Vector2 operator+(Vector2 const& lhs, Vector2 const& rhs) {
    return Vector2(lhs.x + rhs.x, lhs.y + rhs.y);
}

inline Vector2 operator+(double lhs, Vector2 const& rhs) {
    return Vector2(lhs + rhs.x, lhs + rhs.y);
}

inline Vector2& operator+=(Vector2& lhs, Vector2 const& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

inline Vector2& operator-=(Vector2& lhs, Vector2 const& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

inline Vector2& operator*=(Vector2& lhs, double const rhs) {
    lhs.x *= rhs;
    lhs.y *= rhs;
    return lhs;
}

inline Vector2 operator-(double lhs, Vector2 const& rhs) {
    return Vector2(lhs - rhs.x, lhs - rhs.y);
}

inline std::ostream& operator<<(std::ostream& os, const Vector2& rhs) {
    os << "(" << rhs.x << ", " << rhs.y << ")";
    return os;
}

inline std::istream& operator>>(std::istream& is, Vector2& rhs) {
    is >> rhs.x >> rhs.y;
    return is;
}
