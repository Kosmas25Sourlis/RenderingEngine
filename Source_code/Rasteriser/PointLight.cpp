#include "PointLight.h"
PointLight::PointLight()
{
}

PointLight::PointLight(float red, float green, float blue, Vertex position, float attenuation1,float attenuation2,float attenuation3)
{
	_red = red;
	_green = green;
	_blue = blue;
	_position = position;
	_attenuation1 = attenuation1;
	_attenuation2 = attenuation2;
	_attenuation3 = attenuation3;
}
PointLight::PointLight(const PointLight& rhs)
{
	_red = rhs.GetRed();
	_blue = rhs.GetBlue();
	_green = rhs.GetGreen();
	_position = rhs.GetPosition();
	_attenuation1 = rhs.GetAttenuation1();
	_attenuation2 = rhs.GetAttenuation2();
	_attenuation3 = rhs.GetAttenuation3();
	
}
PointLight::~PointLight()
{

}


float PointLight::GetRed() const
{
	return _red;
}
float PointLight::GetGreen() const
{
	return _blue;
}

float PointLight::GetBlue() const
{
	return _blue;
}

Vertex PointLight::GetPosition() const
{
	return _position;
}

float PointLight::GetAttenuation1() const
{
	return _attenuation1;
}

float PointLight::GetAttenuation2() const
{
	return _attenuation2;
}

float PointLight::GetAttenuation3() const
{
	return _attenuation3;
}
