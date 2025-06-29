#pragma once
#include <iostream>
#include <stdexcept>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Vector {
private:
    friend class Projectile;
    double x, y;
    double magnitude;
    void setMagnitude();

    class ComponentProxy {
        Vector& parent;
        int index;
    public:
        ComponentProxy(Vector& p, int i);
        operator double() const;
        double& operator=(double value);
    };

public:
    Vector();
    Vector(double x, double y);

    double getMagnitude() const;
    double getX()const;
    void setX(double x_comp);
    void setY(double y_comp);
    double getY()const;

    double DotProduct(const Vector& v) const;

    Vector Normalize();

    Vector operator+(const Vector& v);
    Vector& operator+=(const Vector& v);
    Vector operator-(const Vector& v);
    Vector operator!();

    Vector operator*(double scalar);
    Vector operator/(double a);

    bool operator==(const Vector& v);

    ComponentProxy operator[](int index);
    const double& operator[](int index) const;

    friend Vector operator*(double scalar, const Vector& v);
    friend std::ostream& operator<<(std::ostream& COUT, const Vector v);
    friend std::istream& operator>>(std::istream& CIN, Vector& v);

    friend double FindAngle(Vector v1, Vector v2);
};
