#include "AmbientLight.h"



AmbientLight::AmbientLight()
{

}

AmbientLight::AmbientLight(float red, float green, float blue)
{
	_red = red;
	_green = green;
	_blue = blue;
}

//Copy Constructor
AmbientLight::AmbientLight(const AmbientLight& a)
{
	_red = a.GetRed();
	_green = a.GetGreen();
	_blue = a.GetBlue();
}

AmbientLight::~AmbientLight()
{

}

//Accessors and mutators
float AmbientLight::GetRed() const
{
	return _red;
}

void AmbientLight::SetRed(const float red)
{
	_red = red;
}

float AmbientLight::GetGreen() const
{
	return _green;
}

void AmbientLight::SetGreen(const float green)
{
	_green = green;
}

float AmbientLight::GetBlue() const
{
	return _blue;
}
void AmbientLight::SetBlue(const float blue)
{
	_blue = blue;
}
