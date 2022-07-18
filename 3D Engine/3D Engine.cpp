#define _USE_MATH_DEFINES

#include "olcConsoleGameEngine.h"
#include <iostream>
#include <cmath>
using std::cout;

struct Vector3D {
	float x, y, z;
};
struct Triangle {
	Vector3D points[3];
};
struct Matrix4x4 {
	float matrix[4][4] = { 0 };
};
struct Mesh {
	std::vector<Triangle> tris;
};
class Engine3D : public olcConsoleGameEngine {
public:
	Engine3D() {
		m_sAppName = L"Ev's 3D Engine";
	}
	bool OnUserCreate() override {
		meshCube.tris = {
			// SOUTH
			{0.0f, 0.0f, 0.0f,		0.0f, 1.0f, 0.0f,		1.0f, 1.0f, 0.0f, },
			{0.0f, 0.0f, 0.0f,		1.0f, 1.0f, 0.0f,		1.0f, 0.0f, 0.0f, },

			// EAST
			{1.0f, 0.0f, 0.0f,		1.0f, 1.0f, 0.0f,		1.0f, 1.0f, 1.0f, },
			{1.0f, 0.0f, 0.0f,		1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f, },

			// NORTH
			{1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f, },
			{1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f, },

			// WEST
			{0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f, },
			{0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f, 0.0f, },

			// TOP
			{0.0f, 1.0f, 0.0f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f, },
			{0.0f, 1.0f, 0.0f,		1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 0.0f, },

			// BOTTOM
			{1.0f, 0.0f, 1.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f, 0.0f, },
			{1.0f, 0.0f, 1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f, }
		};

		float fNear = 0.1f;
		float fFar = 1000.0f;
		float fFov = 90.0f;
		float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
		float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * M_PI);

		matrixProjection.matrix[0][0] = fAspectRatio * fFovRad;
		matrixProjection.matrix[1][1] = fFovRad;
		matrixProjection.matrix[2][2] = fFar / fFar - fNear;
		matrixProjection.matrix[3][2] = (-fFar * fNear) / (fFar - fNear);
		matrixProjection.matrix[2][3] = 1.0f;
		matrixProjection.matrix[3][3] = 0.0f;

		return true;
	}
	bool OnUserUpdate(float elapsedTime) override {
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

		Matrix4x4 rotX, rotZ;
		fTheta += 1.0f * elapsedTime;

		rotZ.matrix[0][0] = cosf(fTheta);
		rotZ.matrix[0][1] = sinf(fTheta);
		rotZ.matrix[1][0] = -sinf(fTheta);
		rotZ.matrix[1][1] = cosf(fTheta);
		rotZ.matrix[2][2] = 1;
		rotZ.matrix[3][3] = 1;

		rotX.matrix[0][0] = 1;
		rotX.matrix[1][1] = cosf(fTheta * 0.5f);
		rotX.matrix[1][2] = sinf(fTheta * 0.5f);
		rotX.matrix[2][1] = -sinf(fTheta * 0.5f);
		rotX.matrix[2][2] = cosf(fTheta * 0.5f);
		rotX.matrix[3][3] = 1;

		for (auto tri : meshCube.tris) {
			Triangle triProjected;
			Triangle triRotZ, triRotZX;

			MatrixVectorMultiplication(tri.points[0], triRotZ.points[0], rotZ);
			MatrixVectorMultiplication(tri.points[1], triRotZ.points[1], rotZ);
			MatrixVectorMultiplication(tri.points[2], triRotZ.points[2], rotZ);

			MatrixVectorMultiplication(triRotZ.points[0], triRotZX.points[0], rotX);
			MatrixVectorMultiplication(triRotZ.points[1], triRotZX.points[1], rotX);
			MatrixVectorMultiplication(triRotZ.points[2], triRotZX.points[2], rotX);

			Triangle translated = triRotZX;

			translated.points[0].z = triRotZX.points[0].z + 3.0f;
			translated.points[1].z = triRotZX.points[1].z + 3.0f;
			translated.points[2].z = triRotZX.points[2].z + 3.0f;

			MatrixVectorMultiplication(translated.points[0], triProjected.points[0], matrixProjection);
			MatrixVectorMultiplication(translated.points[1], triProjected.points[1], matrixProjection);
			MatrixVectorMultiplication(translated.points[2], triProjected.points[2], matrixProjection);

			triProjected.points[0].x += 1.0f;
			triProjected.points[1].x += 1.0f;
			triProjected.points[2].x += 1.0f;
			triProjected.points[0].y += 1.0f;
			triProjected.points[1].y += 1.0f;
			triProjected.points[2].y += 1.0f;

			float xFac = 0.5f * (float)ScreenWidth();
			float yFac = 0.5f * (float)ScreenHeight();

			triProjected.points[0].x *= xFac;
			triProjected.points[1].x *= xFac;
			triProjected.points[2].x *= xFac;
			triProjected.points[0].y *= yFac;
			triProjected.points[1].y *= yFac;
			triProjected.points[2].y *= yFac;

			DrawTriangle(
				triProjected.points[0].x, triProjected.points[0].y,
				triProjected.points[1].x, triProjected.points[1].y,
				triProjected.points[2].x, triProjected.points[2].y,
				PIXEL_SOLID, FG_WHITE
			);
		}

		return true;
	}
private:
	Mesh meshCube;
	Matrix4x4 matrixProjection;

	float fTheta = 0;

	void MatrixVectorMultiplication(Vector3D &i, Vector3D &o, Matrix4x4 &m) {
		o.x = i.x * m.matrix[0][0] + i.y * m.matrix[1][0] + i.z * m.matrix[2][0] + m.matrix[3][0];
		o.y = i.x * m.matrix[0][1] + i.y * m.matrix[1][1] + i.z * m.matrix[2][1] + m.matrix[3][1];
		o.z = i.x * m.matrix[0][2] + i.y * m.matrix[1][2] + i.z * m.matrix[2][2] + m.matrix[3][2];
		float w = i.x * m.matrix[0][3] + i.y * m.matrix[1][3] + i.z * m.matrix[2][3] + m.matrix[3][3];

		if (w != 0.0f) {
			// Maybe..? o.x, o.y, o.z /= w;
			o.x /= w;
			o.y /= w;
			o.z /= w;
		}
	}
};

int main() {
	Engine3D engine;
	if (engine.ConstructConsole(200, 150, 4, 4)) engine.Start(); else cout << "Something went wrong.";

	return 0;
}  