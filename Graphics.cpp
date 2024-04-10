#include "Graphics.h"
#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>
#pragma comment(lib, "opengl32.lib")

float MyClasses::Graphics::Camera::ugol = 0;
float MyClasses::Graphics::Camera::speed = 0;
float MyClasses::Graphics::Camera::xAlpha = 20;
float MyClasses::Graphics::Camera::zAlpha = 0;
POINTFLOAT MyClasses::Graphics::Camera::pos = { 0, 0 };

MyClasses::Array<void (*)()> MyClasses::Graphics::World::functions{};

void MyClasses::Graphics::World::ShowWorld(){
	for (int i = 0; i < functions.len(); i++)
		functions[i]();
}

void MyClasses::Graphics::World::addFunction(void(*f)()) { functions.append(f); }

void MyClasses::Graphics::Camera::MoveCamera(){
	if (GetKeyState(VK_UP) < 0) xAlpha = ++xAlpha > 180 ? 180 : xAlpha;
	if (GetKeyState(VK_DOWN) < 0) xAlpha = --xAlpha < 0 ? 0 : xAlpha;
	if (GetKeyState(VK_LEFT) < 0) zAlpha++;
	if (GetKeyState(VK_RIGHT) < 0) zAlpha--;

	float ugol = -zAlpha / 180 * M_PI;
	float speed = 0;
	if (GetKeyState('W') < 0) speed = 0.1;
	if (GetKeyState('S') < 0) speed = -0.1;
	if (GetKeyState('A') < 0) { speed = 0.1; ugol -= M_PI * 0.5; }
	if (GetKeyState('D') < 0) { speed = 0.1; ugol += M_PI * 0.5; }

	if (speed != 0) {
		pos.x += sin(ugol) * speed;
		pos.y += cos(ugol) * speed;
	}

	glRotatef(-xAlpha, 1, 0, 0);
	glRotatef(-zAlpha, 0, 0, 1);
	glTranslatef(-pos.x, -pos.y, -3);
}


void append(const MyClasses::Array<MyClasses::Array<double>>& u_approx, const MyClasses::Array<double>& X, const MyClasses::Array<double>& Y, double** arr, int line, int el, int& counter) {
	arr[counter] = new double[3];
	arr[counter][0] = X[el];
	arr[counter][1] = Y[line];
	arr[counter][2] = u_approx[line][el];
	counter++;
}

MyClasses::Graphics::Tape MyClasses::Graphics::triangularTape(const Array<Array<double>>& u_approx, const Array<double>& X, const Array<double>& Y){
	double** res = new double* [u_approx.len()*100];
	int counter = 0;
	double ma = u_approx[0][0], mi = u_approx[0][0];

	for (int i = 0; i < u_approx.len(); i++)
		for (int j = 0; j < u_approx[0].len(); j++) {
			mi = std::min<double>(mi, u_approx[i][j]);
			ma = std::max<double>(ma, u_approx[i][j]);
		}

	for (int line = 0; line < u_approx.len() - 1; line++) {
		if (!(line % 2)) {
			for (int el = 0; el < u_approx[0].len(); el++) {
				append(u_approx, X, Y, res, line, el, counter);
				append(u_approx, X, Y, res, line + 1, el, counter);
			}

			if (line == u_approx.len() - 2)
				continue;

			append(u_approx, X, Y, res, line + 1, X.len() - 1, counter);
			append(u_approx, X, Y, res, line + 2, X.len() - 1, counter);
		}
		else {
			for (int el = u_approx[0].len() - 1; el > -1; el--) {
				append(u_approx, X, Y, res, line + 1, el, counter);
				append(u_approx, X, Y, res, line, el, counter);
			}

			append(u_approx, X, Y, res, line + 1, 0, counter);
			append(u_approx, X, Y, res, line + 1, 0, counter);
		}
	}

	//for (int i = 0; i < counter; i++) {
	//	std::cout << res[i][0] << ' ' << res[i][1] << ' ' << res[i][2] << '\n';
	//}

	return Tape{ counter, res, mi, ma };
}

void MyClasses::Graphics::drowTriangularTape(const MyClasses::Graphics::Tape& t) {

	glBegin(GL_TRIANGLES);
	for (int i = 2; i < t.len; i += 1) {
		for (int j = 0; j < 3; j++) {
			if (std::abs(t.triangularTape[i - j][2] - t.mi) < std::abs((t.mi + t.ma) / 2 - t.triangularTape[i - j][2]))
				glColor3f(0, 0, 1);
			else if (std::abs(t.triangularTape[i - j][2] - t.ma) < std::abs((t.mi + t.ma) / 2 - t.triangularTape[i - j][2]))
				glColor3f(1, 0, 0);
			else
				glColor3f(1, 1, 1);

			glVertex3f(t.triangularTape[i - j][0], t.triangularTape[i - j][1], t.triangularTape[i - j][2]);
		}
	}
	glEnd();
}