#pragma once
#include "Vector.h"

class Projectile {
private:
	Vector position;
	Vector velocity;
	Vector acceleration;
public:
	Projectile();
	Projectile(Vector velocity);
	Projectile(Vector position, Vector velocity);
	Projectile(Vector position, Vector velocity,Vector acceleration);

	Vector getPosition();
	void setPosition(const Vector& v);
	Vector getVelocity();
	void setVelocity(const Vector& v);
	double getVelocityMagnitude();
	Vector getAcceleration();
	void setAcceleration(double gravity);

	void updateProjectile(double Time);
	void updateProjectile(double Time, Vector& wind);
	void updateProjectileWithAirResistance(double Time, double resistanceCoefficient );
	void updateProjectileWithAirResistance(double Time, double resistanceCoefficient, Vector& wind);

};
