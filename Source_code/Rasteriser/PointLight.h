#pragma once
#include "Vertex.h"
class PointLight
{
public:
	// constructors and destructor
	PointLight();

	PointLight(float red, float green, float blue, Vertex position, float attenuation1, float attenuation2, float attenuation3);
	PointLight(const PointLight& rhs);
	~PointLight();
	// accessors
	float GetRed() const;
	float GetGreen() const;
	float GetBlue() const;
	Vertex GetPosition() const;
	float GetAttenuation1() const;
	float GetAttenuation2() const;
	float GetAttenuation3() const;

private:
	float _red;
	float _green;
	float _blue;
	Vertex _position;
	float _attenuation1;
	float _attenuation2;
	float _attenuation3;



};