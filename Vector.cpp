#include "Vector.h"
#include <cmath>
#include <iostream>

using namespace std;

// Private
void Vector::setMagnitude() {
    magnitude = sqrt(x * x + y * y);
}

// ComponentProxy implementations
Vector::ComponentProxy::ComponentProxy(Vector& p, int i) : parent(p), index(i) {}

Vector::ComponentProxy::operator double() const {
    return const_cast<const Vector&>(parent)[index];
}

double& Vector::ComponentProxy::operator=(double value) {
    double& ref = (index == 0) ? parent.x : parent.y;
    ref = value;
    parent.setMagnitude();
    return ref;
}

// Constructors
Vector::Vector() : x(0), y(0), magnitude(0) {}

Vector::Vector(double x, double y) : x(x), y(y) {
    setMagnitude();
}

// Public methods
double Vector::getMagnitude() const {
    return magnitude;
}

double Vector::getX() const { return x; }

void Vector::setX(double x_comp) { x = x_comp; setMagnitude(); }

void Vector::setY(double y_comp) { y = y_comp; setMagnitude(); }

double Vector::getY() const { return y; }

double Vector::DotProduct(const Vector& v) const {
    return v.x * x + v.y * y;
}

Vector Vector::Normalize() {
    try {
        if (magnitude == 0)
            throw runtime_error("Vector's magnitude is 0; cannot be normalized.");
        return Vector((x / magnitude), (y / magnitude));
    }
    catch (const runtime_error& e) {
        cerr << "Hata: " << e.what() << std::endl;
        return Vector(0, 0);
    }
}

Vector Vector::operator+(const Vector& v) {
    return Vector((x + v.x), (y + v.y));
}

Vector& Vector::operator+=(const Vector& v) {
    x += v.x;
    y += v.y;
    setMagnitude();
    return *this;
}


Vector Vector::operator-(const Vector& v) {
    return Vector((x - v.x), (y - v.y));
}

Vector Vector::operator!() {
    return Vector(-x, -y);
}

Vector Vector::operator*(double scalar) {
    return Vector((scalar * x), (scalar * y));
}

Vector Vector::operator/(double a) {
    return Vector(x / a, y / a);
}

bool Vector::operator==(const Vector& v) {
    return (x == v.x && y == v.y);
}

Vector::ComponentProxy Vector::operator[](int index) {
    if (index < 0 || index > 1)
        throw out_of_range("The index can be 0(x),1(y); rest is out of range");
    return ComponentProxy(*this, index);
}

const double& Vector::operator[](int index) const {
    if (index == 0) return x;
    else if (index == 1) return y;
    else throw out_of_range("The index can be 0(x),1(y); rest is out of range");
}

// Friend functions
Vector operator*(double scalar, const Vector& v) {
    return Vector((v.x * scalar), (v.y * scalar));
}

ostream& operator<<(ostream& COUT, const Vector v) {
    COUT << "<" << v.x << "," << v.y << ">" << " Magnitude: " << v.magnitude << '\n';
    return COUT;
}

istream& operator>>(istream& CIN, Vector& v) {
    CIN >> v.x;
    CIN >> v.y;
    v.setMagnitude();
    return CIN;
}

double FindAngle(Vector v1, Vector v2) {
    return acos(v1.DotProduct(v2) / (fabs(v1.magnitude) * fabs(v2.magnitude))) * (180.0 / M_PI);
}

