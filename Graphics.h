#pragma once
#include "Array.h"
#include <windows.h>


namespace MyClasses {
	namespace Graphics {
		class World {
		private:
			static Array<void (*)()> functions;
		public:
			static void ShowWorld();
			static void addFunction(void (*f)());

		};

		class Camera {
		private:
			static float ugol;
			static float speed;
			static float xAlpha, zAlpha;
			static POINTFLOAT pos;
		public:
			static void MoveCamera();
		};

		struct Tape {
			int len;
			double** triangularTape;
			double mi;
			double ma;

			//~Tape() {
			//	for (int i = 0; i < len; i++)
			//		delete triangularTape[i];
			//	delete triangularTape;
			//	this->len = 0;
			//}


			//Tape(int len, double** triangularTape) {
			//	len = t.len;
			//	mi = t.mi;
			//	ma = t.ma;

			//	triangularTape = new double* [len];
			//	for (int i = 0; i < len; i++) {
			//		triangularTape[i] = new double[3];
			//		triangularTape[i][0] = t.triangularTape[i][0];
			//		triangularTape[i][1] = t.triangularTape[i][1];
			//		triangularTape[i][2] = t.triangularTape[i][2];
			//	}
			//}
		};

		Tape triangularTape(const Array<Array<double>>& u_approx, const Array<double>& X, const Array<double>& Y);
		void drowTriangularTape(const Tape& t);
	}
}