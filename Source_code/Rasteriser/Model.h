#pragma once

#include <vector>
#include "Polygon3D.h"
#include "Vertex.h"
#include "Matrix.h"
#include "Camera.h"
#include <algorithm>
#include "stdafx.h"
#include <math.h>
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"

class Model
{
public:
	Model();
	Model(const Model& m);
	~Model();

	//Accessors and mutators
	const std::vector<Polygon3D>& GetPolygons() const;
	const std::vector<Vertex>& GetVertices()const;
	const std::vector<Vertex>& GetTransform()const;

	
	size_t GetPolygonCount() const;
	size_t GetVertexCount() const;

	void AddVertex(float x, float y, float z);
	void AddPolygon(int i0, int i1, int i2);
	void AmbientLightCalculation(AmbientLight light);
	void CalculateLightingDirectional(std::vector<DirectionalLight> directionalLights);
	void SetKValues(const float kaRed, const float kaGreen, const float kaBlue, const float kdRed, const float kdGreen, const float kdBlue, const float ksRed, const float ksGreen, const float ksBlue);
	void InitialTransformation(const Matrix &transform);
	void ApplyTransformToTransformedVertices(const Matrix &transform);
	void Dehomogenize();
	void BackFaceCulling(Camera camera);
	void Sort(void);
	void CalculatePointLighting(std::vector<PointLight> pointLights);

	Model& operator= (const Model& rhs);

private:
	std::vector<Polygon3D> _polygons;
	std::vector<Vertex> _vertices;
	std::vector<Vertex> _transform;
	float _kaRed;
	float _kaGreen;
	float _kaBlue;

	float _kdRed;
	float _kdGreen;
	float _kdBlue;

	float _ksRed;
	float _ksGreen;
	float _ksBlue;

	COLORREF _colour;
};