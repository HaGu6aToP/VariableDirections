#pragma once
#include "Array.h"

namespace MyClasses {

	namespace Derivatives {
		double center(double u_right, double u_center, double u_left, double step);
	}

	namespace Methods {

		struct VariableDirectionsParams {
			int A, B, T;
			double h1, h2, tau;
			double (*f)(double, double, double);
			double (*u0)(double, double);

			double (*g1)(double, double);
			double (*g2)(double, double);
			double (*g3)(double, double);
			double (*g4)(double, double);
		};

		struct VariableDirectionsResult {
			Array<double> X;
			Array<double> Y;
			Array<double> T;
			Array<Array<Array<double>>> u;
		};

		class VariableDirections {
		public:
			static int NX, NY, NT_2;
			static Array<double> X;
			static Array<double> Y;
			static Array<double> T;
			static bool prepared;
			static VariableDirectionsParams params;

		public:
			static Array<Array<double>> firstStep(int k, Array<Array<double>> u);
			static Array<Array<double>> secondStep(int k, Array<Array<double>> u);
		public:
			static void init(const VariableDirectionsParams& params);
			static VariableDirectionsResult start();
		};


	}
}