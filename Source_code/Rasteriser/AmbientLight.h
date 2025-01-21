#pragma once
class AmbientLight
{
public:
	// constructors and destructor
	//empty constructor
	AmbientLight();
	// constructor that initializes all three private variables
	AmbientLight(float red, float green, float blue);
	//copy constructor
	AmbientLight(const AmbientLight& a);
	~AmbientLight();

	//Accessors and mutators
	float GetRed() const;
	void SetRed(const float red);
	float GetGreen() const;
	void SetGreen(const float green);
	float GetBlue() const;
	void SetBlue(const float blue);


private:
	float _red;
	float _green;
	float _blue;

};

