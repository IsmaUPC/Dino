#include "Space4HD.h"
#define PI 3.14159265
#define RADTODEG 57.29577958f
void Body::AddForces(fPoint force)
{
	forces.x += force.x;
	forces.y += force.y;
}
void Body::AddTorque(fPoint force, fPoint distance)
{
	torque += (force.x*distance.y)-(forces.y*distance.x);
}
void Body::CalculateAcceleration()
{
	acceleration.x = forces.x / mass;
	acceleration.y = forces.y / mass;
}
void Body::CalculateAngularAcceleration(float radio)
{
	float inercia = mass * radio * radio;
	angularAcceleration = torque / inercia;
}
void Body::ResetForces()
{
	forces = { 0,0 };
}
void Body::ResetTorque()
{
	torque = 0;
}

void Body::RotateBody(fPoint pointsCollision[], int numPoints, fPoint axis)
{
	for (int i = 0; i < numPoints; i++)
	{
		fPoint vector = pointsCollision[i] - axis;
		float posX = (vector.x * cos(angularPosition)) - (vector.y * sin(angularPosition));//Matrix rotation
		float posY = (vector.x * sin(angularPosition)) + (vector.y * cos(angularPosition));
		pointsCollision[i].x = posX + axis.x;//+axis change base
		pointsCollision[i].y = posY + axis.y;
	}
}
float Body::GetRotation() const
{
	return  RADTODEG*angularPosition;
}

fPoint PhysicsEngine::ForceGrav(float mass)
{
	fPoint fg;
	fg.x = 0;
	fg.y = mass * GRAVITY;
	return fg;
}

fPoint PhysicsEngine::ForceAeroDrag(fPoint dirVelocity, float density, float velRelative, float surface, float Cd)
{
	float fdModule;
	fdModule = 0.5 * density * velRelative * surface * Cd;
	fPoint fd;
	fd.x = -fdModule * dirVelocity.x;
	fd.y = -fdModule * dirVelocity.y;
	return fd;
}

void PhysicsEngine::VelocityVerletLinear(fPoint& position, fPoint& velocity, fPoint acceleration, float dt)
{
	position.x += velocity.x * dt + 0.5 * acceleration.x * dt * dt;
	position.y += velocity.y * dt + 0.5 * acceleration.y * dt * dt;

	velocity.x += acceleration.x * dt;
	velocity.y += acceleration.y * dt;
}
void PhysicsEngine::VelocityVerletAngular(float& angularPosition, float& angularVelocity, float angularAcceleration, float dt)
{
	angularPosition += angularVelocity * dt + 0.5 * angularAcceleration * dt * dt;
	angularVelocity += angularAcceleration * dt;
}
