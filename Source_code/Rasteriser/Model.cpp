#include <math.h>
#include "Model.h"
using namespace std;
Model::Model()
{
	_polygons.clear();
	_vertices.clear();
	_transform.clear();

}
//Copy Constructor
Model::Model(const Model& m)
{
	_polygons = m.GetPolygons();
	_vertices = m.GetVertices();
	_transform = m.GetTransform();

}
Model::~Model()
{

}
//Accessors and mutators
const std::vector<Polygon3D>& Model::GetPolygons() const
{
	return _polygons;
}

const std::vector<Vertex>& Model::GetVertices() const
{
	return _vertices;
}

const std::vector<Vertex>& Model::GetTransform() const
{
	return _transform;
}


size_t Model::GetPolygonCount() const
{
	return _polygons.size();
}

size_t Model::GetVertexCount() const
{
	return _vertices.size();
}


void Model::AddVertex(float x, float y, float z)
{
	Vertex temp(x, y, z, 1);
	_vertices.push_back(temp);
}

void Model::AddPolygon(int i0, int i1, int i2)
{
	Polygon3D temp(i0, i1, i2);
	_polygons.push_back(temp);
	
}

void Model::AmbientLightCalculation(AmbientLight light)
{
	//Create variables for total r, g, band temp r, g, b
	float totalRed{};
	float totalGreen{};
	float totalBlue{};
	float tempRed{};
	float tempGreen{};
	float tempBlue{};
	for (size_t i{0}; i < GetPolygons().size(); i++)
	{
		//For each polygon in the model

		//Set to black
		totalRed = 0;
		totalGreen = 0;
		totalBlue = 0;
		
		//Set temp colour to intensity of the light
		tempRed = light.GetRed();
		tempGreen = light.GetGreen();
		tempBlue = light.GetBlue();

		//Multiply the temp colour by the diffuse coefficients
		tempRed = tempRed * _kaRed;
		tempGreen = tempGreen * _kaGreen;
		tempBlue = tempBlue * _kaBlue;

		//Add temp colour to total colour
		totalRed += tempRed;
		totalGreen += tempGreen;
		totalBlue += tempBlue;

		//Clamp total r, g, b values to be in the range 0 – 255

		if (totalRed < 0 || totalRed > 255)
		{
			totalRed > 255 ? totalRed = 255 : totalRed = 0;
		}
		if (totalGreen < 0 || totalGreen > 255)
		{
			totalGreen > 255 ? totalGreen = 255 : totalGreen = 0;
		}
		if (totalBlue < 0 || totalBlue > 255)
		{
			totalBlue > 255 ? totalBlue = 255 : totalBlue = 0;
		}
		COLORREF tempColour = RGB(totalRed, totalGreen, totalBlue);

		//Save the colour to the polygon
		_polygons.at(i).SetColour(RGB(totalRed, totalGreen, totalBlue));
		//Save the colour to the vertices
		_transform.at(_polygons.at(i).GetIndex(0)).SetColour(tempColour);
		_transform.at(_polygons.at(i).GetIndex(1)).SetColour(tempColour);
		_transform.at(_polygons.at(i).GetIndex(2)).SetColour(tempColour);
	}
}

void Model::CalculateLightingDirectional(std::vector<DirectionalLight> directionalLights)
{
	//Create variables for total r, g, band temp r, g, b
	float totalRed{};
	float totalGreen{};
	float totalBlue{};
	float tempRed{};
	float tempGreen{};
	float tempBlue{};
	for (size_t i{ 0 }; i < GetPolygons().size(); i++)
	{
		// set tyhe colour of eack polygon
		totalRed = GetRValue(_polygons.at(i).GetColour());
		totalGreen = GetGValue(_polygons.at(i).GetColour());
		totalBlue = GetBValue(_polygons.at(i).GetColour());
		for (size_t a {0}; a < directionalLights.size(); a++)
		{
			tempRed =  directionalLights.at(a).GetRed();
			tempGreen = directionalLights.at(a).GetGreen();
			tempBlue = directionalLights.at(a).GetBlue();

			//Multiply(modulate) temp r.g.b by the corresponding
			//diffuse reflectance coefficients for the model
			tempRed = tempRed * _kdRed;
			tempGreen = tempGreen * _kdGreen;
			tempBlue = tempBlue * _kdBlue;
			//Calculate the dot - product of the normal vector to
			//the light source and the polygons’ normal vector
			double dotProduct{ directionalLights.at(a).GetDirection().DotProduct(_polygons.at(i).GetNormal())};
			//Multiply temp r, g, b by dot - product
			tempRed = tempRed * dotProduct;
			tempGreen = tempGreen * dotProduct;
			tempBlue = tempBlue * dotProduct;
			//Add temp r, g, b to total r, g, b
			totalRed += tempRed;
			totalGreen += tempGreen;
			totalBlue += tempBlue;
			//Clamp total r, g, b values to be in the range 0 – 255

			if (totalRed < 0 || totalRed > 255)
			{
				totalRed > 255 ? totalRed = 255 : totalRed = 0;
			}
			if (totalGreen < 0 || totalGreen > 255)
			{
				totalGreen > 255 ? totalGreen = 255 : totalGreen = 0;
			}
			if (totalBlue < 0 || totalBlue > 255)
			{
				totalBlue > 255 ? totalBlue = 255 : totalBlue = 0;
			}
			//*Build RGB colour using total r, g, b values
			//Store colour in polygon* 
			COLORREF colour{ RGB(totalRed,totalGreen,totalBlue) };
			_polygons.at(i).SetColour(colour);
		}
	}
}
void Model::CalculatePointLighting(std::vector<PointLight> pointLights)
{
	//Create variables for total r, g, band temp r, g, b
	float totalRed{};
	float totalGreen{};
	float totalBlue{};
	float tempRed{};
	float tempGreen{};
	float tempBlue{};

	for (size_t i{ 0 }; i < GetPolygons().size(); i++)
	{
		totalRed = GetRValue(_polygons.at(i).GetColour());
		totalGreen = GetGValue(_polygons.at(i).GetColour());
		totalBlue = GetBValue(_polygons.at(i).GetColour());
		for (size_t a{ 0 }; a < pointLights.size(); a++)
		{
			tempRed = pointLights.at(a).GetRed();
			tempGreen = pointLights.at(a).GetGreen();
			tempBlue = pointLights.at(a).GetBlue();
			tempRed = tempRed * _ksRed;
			tempGreen = tempGreen * _ksGreen;
			tempBlue = tempBlue * _ksBlue;
			//To calculate the vector to the light source, you can subtract the position
			//of the light source from the position of the first vertex in the polygon
			int index0{ _polygons.at(i).GetIndex(0) };
			Vertex ver0{ _transform.at(index0) };

			Vector3D lightSource{ pointLights.at(a).GetPosition().SubtractionVector(ver0)};
			// This is calculated to calculate the attenuation
			float length = sqrt(lightSource.GetX() * lightSource.GetX()+ lightSource.GetY() * lightSource.GetY()+ lightSource.GetZ() * lightSource.GetZ());
			float dotProduct = lightSource.DotProduct(_polygons.at(i).GetNormal());

			//Normalize after dot product calculation
			lightSource.Normalize();

			// Calculate attenuation
			float a1 = pointLights.at(a).GetAttenuation1();
			float b2 = pointLights.at(a).GetAttenuation2();
			float c3 = pointLights.at(a).GetAttenuation3();
			float distanceMultipliedByTwo = length * length;
			float attenuation = 1 / (a1 + b2 * length + c3 * distanceMultipliedByTwo);

			tempRed = tempRed * dotProduct;
			tempGreen = tempGreen * dotProduct;
			tempBlue = tempBlue * dotProduct;

			//Multiply by the attenuation
			tempRed = tempRed * attenuation;
			tempGreen = tempGreen * attenuation;
			tempBlue = tempBlue * attenuation;
			// add tempRed, tempGreen and tempBlue to total
			totalRed += tempRed;
			totalGreen += tempGreen;
			totalBlue += tempBlue;

			//Clamp total r, g, b values to be in the range 0 – 255

			if (totalRed < 0 || totalRed > 255)
			{
				totalRed > 255 ? totalRed = 255 : totalRed = 0;
			}
			if (totalGreen < 0 || totalGreen > 255)
			{
				totalGreen > 255 ? totalGreen = 255 : totalGreen = 0;
			}
			if (totalBlue < 0 || totalBlue > 255)
			{
				totalBlue > 255 ? totalBlue = 255 : totalBlue = 0;
			}


			//*Build RGB colour using total r, g, b values
			//Store colour in polygon* 
			COLORREF colour{ RGB(totalRed,totalGreen,totalBlue) };
			_polygons.at(i).SetColour(colour);
		}
	}
}

void Model::SetKValues(const float kaRed, const float kaGreen, const float kaBlue, const float kdRed, const float kdGreen, const float kdBlue, const float ksRed, const float ksGreen, const float ksBlue)
{
	_kaRed = kaRed;
	_kaGreen = kaGreen;
	_kaBlue = kaBlue;

	_kdRed = kdRed;
	_kdGreen = kdGreen;
	_kdBlue = kdBlue;

	_ksRed = ksRed;
	_ksGreen = ksGreen;
	_ksBlue = ksBlue;
}

void Model::InitialTransformation(const Matrix &transform)
{
	// Clear the vector of vertexes of local private variable tranform
	_transform.clear();

	// Fill the vector of transform with different values according to the parameter of the function (tranform).

	for (size_t i {0}; i < _vertices.size(); i++)
	{
		_transform.push_back(transform.operator*(_vertices.at(i)));
	}
}

void Model::ApplyTransformToTransformedVertices(const Matrix &Camera)
{
	// This method will be called to bring the objct in front of the camera.
	// It basicaly change the vertexes tranformed vertexes to place them in front of the camera.
	for (size_t i{ 0 }; i < _transform.size(); i++) 
	{
		_transform.at(i) = Camera.operator*(_transform.at(i));
	}
}

void Model::Dehomogenize()
{
	for (size_t i{ 0 }; i < _vertices.size(); i++)
	{
		_transform.at(i).Dehomogenize();
	}
}


Model& Model::operator=(const Model& rhs)
{
	//Only do the assignment if we are not assigning
	//to ourselves
	if (this != &rhs)
	{
		_polygons = rhs.GetPolygons();
		_vertices = rhs.GetVertices();
		_transform = rhs.GetTransform();
	}
	return *this;
}

void Model::BackFaceCulling( Camera _camera)
{
	//For each polygon in the model
	for (size_t i{0}; i < GetPolygons().size(); i++)
	{
		// Get the indices 
		int index0 {_polygons.at(i).GetIndex(0)};
		int index1 {_polygons.at(i).GetIndex(1)};
		int index2 {_polygons.at(i).GetIndex(2)};
		// Get the vertice from those indices
		Vertex ver0 {_transform.at(index0)};
		Vertex ver1 {_transform.at(index1)};
		Vertex ver2 {_transform.at(index2)};
		// Create vectors vectorA and vector B buy subtructing apropriate vertexes
		Vector3D vectorA{ver0.SubtractionVector(ver1)};
		Vector3D vectorB{ver0.SubtractionVector(ver2)};
		// For each polygon set the normal vector.
		Vector3D vec{ vectorB.CrossProduct(vectorA) };
		// normalize before storing on private on polygons because we need to use this value on lighting
		vec.Normalize();
		
		_polygons.at(i).SetNormal(vec);
		// Calculate the dot product by creating the eye vector and subtructing eye vector from ver0.
		Vector3D eyeVector{ver0.SubtractionVector(_camera.GetPos())};
		float dotProduct{ _polygons.at(i).GetNormal().DotProduct(eyeVector) };
		// Mark the polygon for culling if the dot product is less than zero.
		if (dotProduct < 0)
		{
			_polygons.at(i).SetCull(true);
		}
		else
		{
			_polygons.at(i).SetCull(false);
		}
	}
}



void Model::Sort()
{
	// It is a sorting algorithm that I thought it is not very efficient 
	// but it does the work
	std::vector <float> unsortedList{};
	std::vector <float> sortedList{};
	for (size_t i{ 0 }; i < GetPolygons().size(); i++)
	{
		int index0{ _polygons.at(i).GetIndex(0) };
		int index1{ _polygons.at(i).GetIndex(1) };
		int index2{ _polygons.at(i).GetIndex(2) };
		
		Vertex ver0{ _transform.at(index0) };
		Vertex ver1{ _transform.at(index1) };
		Vertex ver2{ _transform.at(index2) };
		_polygons.at(i).SetAverageZ(ver0.GetZ() + ver1.GetZ() + ver2.GetZ() / 3);
	}
	
	for (size_t i{ 0 }; i < GetPolygons().size(); i++)
	{
		unsortedList.push_back(_polygons.at(i).GetAverageZ());
	}
	float largest = {};
	int elementToremove{};
	largest = -FLT_MAX;
	while (unsortedList.size() != 0 )
	{
		for (int i{ 0 }; i < unsortedList.size(); i++)
		{

			if (largest < unsortedList.at(i))
			{
				largest = unsortedList.at(i) ;
				elementToremove = i;
			}
		}
			
		sortedList.push_back(largest);
		unsortedList.erase(unsortedList.begin() + elementToremove);
		largest = -FLT_MAX;
	}
		for (size_t i{ 0 }; i < GetPolygons().size(); i++)
		{
			_polygons.at(i).SetAverageZ(sortedList.at(GetPolygons().size()-i-1));
		}
	}


