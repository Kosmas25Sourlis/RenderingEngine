#include "DirectionalLight.h"
DirectionalLight::DirectionalLight()
{

}

DirectionalLight::DirectionalLight(float red, float green, float blue, Vector3D direction)
{
	_redIntensity = red;
	_greenIntensity = green;
	_blueIntensity = blue;
	_direction = direction;
}

DirectionalLight::DirectionalLight(const DirectionalLight& direction)
{
	_direction = direction.GetDirection();
	_redIntensity = direction.GetRed();
	_blueIntensity = direction.GetGreen();
	_greenIntensity = direction.GetBlue();
}

DirectionalLight::~DirectionalLight()
{

}
float DirectionalLight::GetRed() const
{
	return _redIntensity;
}
float DirectionalLight::GetGreen() const
{
	return _blueIntensity;
}

float DirectionalLight::GetBlue() const
{
	return _blueIntensity;
}

Vector3D DirectionalLight::GetDirection() const
{
	return _direction;
}

