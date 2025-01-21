#include "Rasteriser.h"

Rasteriser app;

// This public variables are used to make the transformations in the model.

float rotation = 1;
float scaling = 1;
float translationToX = 1;
float translationToY = 1;
float translationToZ = 1;
const int timeFactor = 1800;


string Rasteriser::ModelPath()
{
	char buf[256];
	GetCurrentDirectoryA(256, buf);
	return std::string(buf) + '\\';
}

bool Rasteriser::Initialise()
{
	_modelpath="";
	_modelpath = ModelPath() + "\\cube.md2";

	if (!MD2Loader::LoadModel(_modelpath.c_str(), _model,
		&Model::AddPolygon,
		&Model::AddVertex))
	{
		return false;
	}

	// Private variable of rasterizer is used to set the light 
	_ambientLight.SetRed(86);
	_ambientLight.SetGreen(131);
	_ambientLight.SetBlue(210);
	_model.SetKValues(0.5, 0.5, 0.5,
		0.8, 0.9, 0.85,
		0.5, 0.5, 0.5);
	// Light objects to apply when lighting effects are included on presentation
	Vector3D lightDirection(1, 0, 1);
	// normalize the vector to make correct calculations
	lightDirection.Normalize();
	DirectionalLight light1(86, 131, 210, lightDirection);
	_directionalLights.push_back(light1);
	Vertex pointLight1Position(0, -50, 0, 1);
	PointLight pointLight(255, 255, 255, pointLight1Position, 0, 1, 0);
	_pointLights.push_back(pointLight);
	Camera temp(0, 0, 0, Vertex(0, 0, -50, 1)); 
	_camera = temp;
	return true;
}

void Rasteriser::DrawWireFrame(Bitmap &bitmap)
{
	// This method is used to simply draw the wireframe inside the panel  without colour on the polygons. 
	std::vector<Polygon3D> tempPolygon(_model.GetPolygons());
	int polygonSize = int(_model.GetPolygonCount());
	std::vector<Vertex> tempVertex(_model.GetTransform());
	for (int i = 0; i < polygonSize; i++)
	{
		
		if (!tempPolygon.at(i).GetCull())
		{
			HPEN hPen = CreatePen(PS_SOLID, 1, tempPolygon.at(i).GetColour());
			HGDIOBJ oldPen = SelectObject(bitmap.GetDC(), hPen);
			//Get the indices
			int tempIndex0 = tempPolygon[i].GetIndex(0);
			Vertex tempVertex0(tempVertex[tempIndex0]);
			int tempIndex1 = tempPolygon[i].GetIndex(1);
			Vertex tempVertex1(tempVertex[tempIndex1]);
			int tempIndex2 = tempPolygon[i].GetIndex(2);
			Vertex tempVertex2(tempVertex[tempIndex2]);
			
			//Draw the triangle
			MoveToEx(bitmap.GetDC(), tempVertex0.GetIntX(), tempVertex0.GetIntY(), NULL);
			LineTo(bitmap.GetDC(), tempVertex1.GetIntX(), tempVertex1.GetIntY());
			LineTo(bitmap.GetDC(), tempVertex2.GetIntX(), tempVertex2.GetIntY());
			LineTo(bitmap.GetDC(), tempVertex0.GetIntX(), tempVertex0.GetIntY());
			DeleteObject(hPen);
		}
	}
}
void Rasteriser::DrawSolidFlat(Bitmap& bitmap) 
{
	// To  fill the polygons with color we need to create a vector of temp polygon 
	// and apply color on each polygon
	std::vector<Polygon3D> tempPolygon(_model.GetPolygons());
	int polygonSize = int(_model.GetPolygonCount());
	std::vector<Vertex> tempVertex(_model.GetTransform());
	for (int i = 0; i < polygonSize; i++)
	{
		if (!tempPolygon.at(i).GetCull())
		{ 
			// set the colour 

			HBRUSH hBrush = CreateSolidBrush(tempPolygon[i].GetColour());
			HGDIOBJ oldBrush = SelectObject(bitmap.GetDC(), hBrush);

			HPEN hPen = CreatePen(PS_SOLID, 1, tempPolygon[i].GetColour());
			HGDIOBJ oldPen = SelectObject(bitmap.GetDC(), hPen);
			//Get the indices
			int tempIndex0 = tempPolygon[i].GetIndex(0);
			Vertex tempVertex0(tempVertex[tempIndex0]);
			int tempIndex1 = tempPolygon[i].GetIndex(1);
			Vertex tempVertex1(tempVertex[tempIndex1]);
			int tempIndex2 = tempPolygon[i].GetIndex(2);
			Vertex tempVertex2(tempVertex[tempIndex2]);
			
			// We need to get the the three points
			POINT point0{ long(tempVertex0.GetX()), long(tempVertex0.GetY()) };
			POINT point1{ long(tempVertex1.GetX()), long(tempVertex1.GetY()) };
			POINT point2{ long(tempVertex2.GetX()), long(tempVertex2.GetY()) };

			POINT tempPoints[]  { point0, point1, point2 };
			// Polygon method fills the model with colour.
			Polygon(bitmap.GetDC(), tempPoints, 3);

			DeleteObject(hPen);
			DeleteObject(hBrush);
		}
	}

}
void Rasteriser::GeneratePerspectiveMatrix(float d, float aspectRatio)
{
	Matrix matrixPerspectiveTransform{ (d / aspectRatio), 0, 0, 0, 0, d, 0, 0, 0, 0, d, 0, 0, 0, 1, 0 };
	_perspectiveTransform = matrixPerspectiveTransform;
}

void Rasteriser::GenerateViewMatrix(float d, int width, int height)
{
	Matrix matrixscreenTransformation{ (float)width / 2, 0, 0, (float)width / 2, 0, (float)-height / 2, 0, (float)height / 2, 0, 0, d / 2, d / 2, 0, 0, 0, 1 };
	_screenTransform = matrixscreenTransformation ;
}

// This method is used for displaying strings in the panel that characterize the feature that is been 
void Rasteriser::DrawString(Bitmap& bitmap, LPCTSTR text, int xPos, int yPos, int fontSize)
{
	HDC hdc = bitmap.GetDC();
	HFONT hFont, hOldFont;

	hFont = hFont = CreateFont(fontSize, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
	CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Myfont"));

	if (hOldFont = (HFONT)SelectObject(hdc, hFont))
	{
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkMode(hdc, TRANSPARENT);

		TextOut(hdc, xPos, yPos, text, lstrlen(text));

		SelectObject(hdc, hOldFont);
	}
	DeleteObject(hFont);
}

void Rasteriser::Update(Bitmap &bitmap)
{
	// translation to Y 
	if (clock() <= 2 * timeFactor) {

		translationToY += 0.1f;
		_modelTransform = Matrix::TranslationMatrix(0, translationToY,0);

	}
	if (clock() <= 4 * timeFactor && clock() > 2 * timeFactor) {

		translationToY -= 0.1f;
		_modelTransform = Matrix::TranslationMatrix(0, translationToY, 0);

	}
	// Translation to X
	if (clock() <= 6 * timeFactor && clock() > 4 * timeFactor) {

		translationToX += 0.2f;
		_modelTransform = Matrix::TranslationMatrix(translationToX, 0, 0);

	}
	if (clock() <= 8 * timeFactor && clock() > 6 * timeFactor) {

		translationToX -= 0.2f;
		_modelTransform = Matrix::TranslationMatrix(translationToX, 0, 0);

	}
	//Translation to Z
	if (clock() <= 10 * timeFactor && clock() > 8 * timeFactor) {

		translationToZ += 0.2f;
		_modelTransform = Matrix::TranslationMatrix(0, 0, translationToZ);

	}
	if (clock() <= 12 * timeFactor && clock() > 10 * timeFactor) {

		translationToZ -= 0.2f;
		_modelTransform = Matrix::TranslationMatrix(0, 0, translationToZ);
	}
	// Scale X
	if (clock() <= 14 * timeFactor && clock() > 12 * timeFactor) {

		scaling += 0.01f;
		_modelTransform = Matrix::ScalingMatrix(scaling,1,1);
	}
	if (clock() <= 16 * timeFactor && clock() > 14 * timeFactor) {

		scaling -= 0.01f;
		_modelTransform = Matrix::ScalingMatrix(scaling, 1, 1);
	}
	// Scale Y
	if (clock() <= 18 * timeFactor && clock() > 16 * timeFactor) {

		scaling += 0.003f;
		_modelTransform = Matrix::ScalingMatrix(1, scaling, 1);
	}
	if (clock() <= 20 * timeFactor && clock() > 18 * timeFactor) {

		scaling -= 0.003f;
		_modelTransform = Matrix::ScalingMatrix(1, scaling, 1);
	}
	// Scale Z
	if (clock() <= 22 * timeFactor && clock() > 20 * timeFactor) {
		// rotation to Y in order to see clearly the scaling on Z axis.
		scaling += 0.003f;
		_modelTransform = Matrix::YRotationMatrix(0.6) * Matrix::ScalingMatrix(1, 1, scaling);
	}
	if (clock() <= 24 * timeFactor && clock() > 22 * timeFactor) {

		scaling -= 0.003f;
		_modelTransform = Matrix::YRotationMatrix(0.6)* Matrix::ScalingMatrix(1, 1, scaling);
	}
    // Rotate to X
	if (clock() <= 28 * timeFactor && clock() > 24 * timeFactor) {

		rotation += 0.02;
		_modelTransform = Matrix::XRotationMatrix(rotation);
	}
	// Rotate to Z
	if (clock() <= 32 * timeFactor && clock() > 28 * timeFactor) {

		rotation += 0.02;
		_modelTransform = Matrix::ZRotationMatrix(rotation);
	}
	// Rotate to Y
	if (clock() <= 40 * timeFactor && clock() > 32 * timeFactor) {

		rotation += 0.02;
		_modelTransform = Matrix::YRotationMatrix(rotation);

	}
	// Rotate X,Y,Z
	if (clock() <= 50 * timeFactor && clock() > 40 * timeFactor) {

		rotation += 0.015;
		_modelTransform = Matrix::XRotationMatrix(rotation) * Matrix::YRotationMatrix(rotation) * Matrix::ZRotationMatrix(rotation);
	}
	
	float aspectRatio = float(float(bitmap.GetWidth()) / float(bitmap.GetHeight()));
	GeneratePerspectiveMatrix(1, aspectRatio);
	GenerateViewMatrix(1, bitmap.GetWidth(), bitmap.GetHeight());
}
void Rasteriser::Render(Bitmap &bitmap)
{
		// Execute different pipeline based on the feature that is been applied and demostrated.
		if (clock() <= 4 * timeFactor)
		{
			bitmap.Clear(RGB(0, 0, 0));
		_model.InitialTransformation(_modelTransform);
		_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
		_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
		_model.Dehomogenize();
		_model.ApplyTransformToTransformedVertices(_screenTransform);
		DrawWireFrame(bitmap);
		DrawString(bitmap, L"Draw wireframe + translation to Y", 10, 55, 35);

		}
		
		if (clock() <= 8 * timeFactor && clock() > 4 * timeFactor)
		{
			bitmap.Clear(RGB(0, 0, 0));
			_model.InitialTransformation(_modelTransform);
			_model.BackFaceCulling(_camera);
			_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
			_model.Sort();
			_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
			_model.Dehomogenize();
			_model.ApplyTransformToTransformedVertices(_screenTransform);
			DrawWireFrame(bitmap); 
			DrawString(bitmap, L"Draw wireframe + translation to X + back face culling", 10, 55, 35);

		}
		if (clock() <= 12 * timeFactor && clock() > 8 * timeFactor)
		{
			bitmap.Clear(RGB(0, 0, 0));
			_model.InitialTransformation(_modelTransform);
			_model.BackFaceCulling(_camera);
			_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
			_model.Sort();
			_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
			_model.Dehomogenize();
			_model.ApplyTransformToTransformedVertices(_screenTransform);
			DrawSolidFlat(bitmap);
			DrawString(bitmap, L"Draw solid flat + translation to Z + back face culling", 10, 55, 35);
		}
		if (clock() <= 16 * timeFactor && clock() > 12 * timeFactor)
		{
			bitmap.Clear(RGB(0, 0, 0));
			_model.InitialTransformation(_modelTransform);
			_model.BackFaceCulling(_camera);
			_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
			_model.Sort();
			_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
			_model.Dehomogenize();
			_model.ApplyTransformToTransformedVertices(_screenTransform);
			DrawSolidFlat(bitmap);
			DrawString(bitmap, L"Draw solid flat + scale to X + back face culling", 10, 55, 35);
		}
		if (clock() <= 20 * timeFactor && clock() > 16 * timeFactor)
		{
			bitmap.Clear(RGB(0, 0, 0));
			_model.InitialTransformation(_modelTransform);
			_model.BackFaceCulling(_camera);
			_model.AmbientLightCalculation(_ambientLight);
			_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
			_model.Sort();
			_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
			_model.Dehomogenize();
			_model.ApplyTransformToTransformedVertices(_screenTransform);
			DrawSolidFlat(bitmap);
			DrawString(bitmap, L"Ambient light + scale to Y + back face culling", 10, 55, 35);
		}
		if (clock() <= 24 * timeFactor && clock() > 20 * timeFactor)
		{
			bitmap.Clear(RGB(0, 0, 0));
			_model.InitialTransformation(_modelTransform);
			_model.BackFaceCulling(_camera);
			_model.AmbientLightCalculation(_ambientLight);
			_model.CalculateLightingDirectional(_directionalLights);
			_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
			_model.Sort();
			_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
			_model.Dehomogenize();
			_model.ApplyTransformToTransformedVertices(_screenTransform);
			DrawSolidFlat(bitmap);
			DrawString(bitmap, L"Directional light + scale to Z + back face culling", 10, 55, 35);
		}
		if (clock() <= 28 * timeFactor && clock() > 24 * timeFactor)
		{
			bitmap.Clear(RGB(0, 0, 0));
			_model.InitialTransformation(_modelTransform);
			_model.BackFaceCulling(_camera);
			_model.AmbientLightCalculation(_ambientLight);
			_model.CalculateLightingDirectional(_directionalLights);
			_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
			_model.Sort();
			_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
			_model.Dehomogenize();
			_model.ApplyTransformToTransformedVertices(_screenTransform);
			DrawSolidFlat(bitmap);
			DrawString(bitmap, L"Directional light + rotate to X + back face culling", 10, 55, 35);
		}
		if (clock() <= 32 * timeFactor && clock() > 28 * timeFactor)
		{
			bitmap.Clear(RGB(0, 0, 0));
			_model.InitialTransformation(_modelTransform);
			_model.BackFaceCulling(_camera);
			_model.AmbientLightCalculation(_ambientLight);
			_model.CalculateLightingDirectional(_directionalLights);
			_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
			_model.Sort();
			_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
			_model.Dehomogenize();
			_model.ApplyTransformToTransformedVertices(_screenTransform);
			DrawSolidFlat(bitmap);
			DrawString(bitmap, L"Directional light + rotate to Z + back face culling", 10, 55, 35);
		}
		if (clock() <= 36 * timeFactor && clock() > 32 * timeFactor)
		{
			bitmap.Clear(RGB(0, 0, 0));
			_model.InitialTransformation(_modelTransform);
			_model.BackFaceCulling(_camera);
			_model.AmbientLightCalculation(_ambientLight);
			_model.CalculateLightingDirectional(_directionalLights);
			_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
			_model.Sort();
			_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
			_model.Dehomogenize();
			_model.ApplyTransformToTransformedVertices(_screenTransform);
			DrawSolidFlat(bitmap);
			DrawString(bitmap, L"Directional light + rotate to Y + back face culling", 10, 55, 35);
		}
		if (clock() <= 40 * timeFactor && clock() > 36 * timeFactor)
		{
			bitmap.Clear(RGB(0, 0, 0));
			_model.InitialTransformation(_modelTransform);
			_model.BackFaceCulling(_camera);
			_model.AmbientLightCalculation(_ambientLight);
			_model.CalculateLightingDirectional(_directionalLights);
			_model.CalculatePointLighting(_pointLights);
			_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
			_model.Sort();
			_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
			_model.Dehomogenize();
			_model.ApplyTransformToTransformedVertices(_screenTransform);
			DrawSolidFlat(bitmap);
			DrawString(bitmap, L"Point light + rotate to Y + back face culling", 10, 55, 35);
		}
		if (clock() <= 50 * timeFactor && clock() > 40 * timeFactor)
		{
			bitmap.Clear(RGB(0, 0, 0));
			_model.InitialTransformation(_modelTransform);
			_model.BackFaceCulling(_camera);
			_model.AmbientLightCalculation(_ambientLight);
			_model.CalculateLightingDirectional(_directionalLights);
			_model.CalculatePointLighting(_pointLights);
			_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
			_model.Sort();
			_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
			_model.Dehomogenize();
			_model.ApplyTransformToTransformedVertices(_screenTransform);
			DrawSolidFlat(bitmap);
			DrawString(bitmap, L"Point light + rotate to X,Y,Z + back face culling", 10, 55, 35);
		}
		// inform the viewer that the presentation is completed
		if (clock() <= 53 * timeFactor && clock() > 50 * timeFactor)
		{
			bitmap.Clear(RGB(0, 0, 0));
			DrawString(bitmap, L"End of presentation", 600, 500, 85);
		}
}