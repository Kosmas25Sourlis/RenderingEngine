#pragma once
#include "Vector3D.h"

class DirectionalLight
{
public:
	// constructors and destructor
	//empty constructor
	DirectionalLight();
	// constructor that initializes all four private variables
	DirectionalLight(float red, float green, float blue, Vector3D direction);
	DirectionalLight(const DirectionalLight& directionLightOther);
	~DirectionalLight();

	//Accessors // there is no need for mutatotrs since I am only going to need the constructor to initiliaze the variables
	float GetRed() const;
	float GetGreen() const;
	float GetBlue() const;
	Vector3D GetDirection() const;

	

private:
	float _redIntensity;
	float _greenIntensity;
	float _blueIntensity;
	Vector3D _direction;
};


