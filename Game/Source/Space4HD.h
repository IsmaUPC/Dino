#include "Point.h"
#include "List.h"
#include "Collisions.h"
#define GRAVITY 9.81 //LA GRAVEDAD SER� VARIABLE, ESTE DEFINE SE ELIMINAR�

class Body
{
public:

	void AddForces(fPoint force);
	void AddTorque(fPoint force, fPoint distance);
	void RotateBody(fPoint pointsCollision[], int numPoints, fPoint axis);
	float GetRotation() const;

	void CalculateAcceleration();
	void CalculateAngularAcceleration(float radio); //hacer un GetDistance() para saber a que distance se est� aplicando  
													//la fuerza y as� no tener que pasarle ning�n valor a la funci�n

	void ResetForces();
	void ResetTorque();

	fPoint position;
	fPoint velocity = { 0,0 };
	fPoint acceleration = { 0,0 };
	float angularPosition = 0;
	float angularVelocity = 0;
	float angularAcceleration = 0;
	fPoint forces = { 0,0 };
	float torque = 0;
	float mass;
};

class Ship : public Body
{
public:
	float health;
	float fuel;
	int ammo; //munici�n

	void LaunchMissiles();
};

class PhysicsEngine
{
public:
	PhysicsEngine() {};
	~PhysicsEngine() {};

	Ship ship;
	List<Body*> asteroids;
	List<Body*> missiles;

	fPoint ForceGrav(float mass);
	fPoint ForceAeroDrag(fPoint dirVelocity, float density, float velRelative, float surface, float Cd);
	//iPoint ForceAeroLift();
	//iPoint ForceHydroBuoy();
	//iPoint ForceHydroDrag();
	
	void Step(); //Step physics: apply current physics & integrate & solve collisions advance one frame
	//Remember to reset current forces/momentum of each body.

	void VelocityVerletLinear(fPoint& position, fPoint& velocity, fPoint acceleration, float dt);
	void VelocityVerletAngular(float& angularPosition, float& angularVelocity, float angularAcceleration, float dt);

	// Collisions:

};// Space4HD; // Instantiate PhysicsEngine as a global variable


