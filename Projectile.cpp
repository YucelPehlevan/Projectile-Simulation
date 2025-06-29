#include "Projectile.h"

//constructors for different scenarios
Projectile::Projectile() :position(0, 0), velocity(0, 0), acceleration(0, -9.807) {}

Projectile::Projectile(Vector velocity) :position(0, 0), velocity(velocity), acceleration(0, -9.807) {}

Projectile::Projectile(Vector position, Vector velocity) : position(position), velocity(velocity), acceleration(0, -9.807) {}

Projectile::Projectile(Vector position, Vector velocity, Vector acceleration) : position(position), velocity(velocity), acceleration(acceleration) {}

// getters and setters
Vector Projectile::getPosition() { return position; }

void Projectile::setPosition(const Vector& v) { position = v; }

Vector Projectile::getVelocity() { return velocity; }

void Projectile::setVelocity(const Vector& v) { velocity = v; }

double Projectile::getVelocityMagnitude() { return velocity.getMagnitude(); }

Vector Projectile::getAcceleration() { return acceleration; }

void Projectile::setAcceleration(double gravity) { acceleration.setY(gravity); } // it sets the Y component of acceleration 

//updating projectile with different scenarios
void Projectile::updateProjectile(double Time) {
	velocity += acceleration * Time;
	position += velocity * Time;
}

void Projectile::updateProjectile(double Time, Vector& wind) {
	velocity += wind + acceleration * Time;
	position += velocity * Time;
}

void Projectile::updateProjectileWithAirResistance(double Time, double coefficient) {
	Vector velocity = getVelocity();
	double speed = velocity.getMagnitude();
	Vector airResistance = velocity.Normalize() * (-coefficient * speed);
	Vector totalAcceleration = acceleration + airResistance;

	velocity += totalAcceleration * Time;
	position += velocity * Time;
	setVelocity(velocity);
	setPosition(position);

}

void Projectile::updateProjectileWithAirResistance(double Time, double coefficient, Vector& wind) {
	Vector velocity = getVelocity();
	double speed = velocity.getMagnitude();
	Vector airResistance = velocity.Normalize() * (-coefficient * speed);
	Vector totalAcceleration = acceleration + airResistance;

	velocity += wind + totalAcceleration * Time;
	position += velocity * Time;
	setVelocity(velocity);
	setPosition(position);
}

