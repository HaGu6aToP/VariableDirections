#include "Methods.h"
#include "LinAlg.h"
int MyClasses::Methods::VariableDirections::NX = 0;
int MyClasses::Methods::VariableDirections::NY = 0;
int MyClasses::Methods::VariableDirections::NT_2 = 0;
bool MyClasses::Methods::VariableDirections::prepared = false;

MyClasses::Array<double> MyClasses::Methods::VariableDirections::X{};
MyClasses::Array<double> MyClasses::Methods::VariableDirections::Y{};
MyClasses::Array<double> MyClasses::Methods::VariableDirections::T{};
//MyClasses::Array<MyClasses::Array<MyClasses::Array<double>>> MyClasses::Methods::VariableDirections::u{};
MyClasses::Methods::VariableDirectionsParams MyClasses::Methods::VariableDirections::params{};

MyClasses::Array<MyClasses::Array<double>> MyClasses::Methods::VariableDirections::firstStep(int k, Array<Array<double>> u) {
	Array<Array<double>> res{};

	res.setLength(NY);
	for (int j = 0; j < NY; j++)
		res[j].setLength(NX);


	double l;
	// Краевые условия
	for (int i = 0; i < NX; i++) {
		res[0][i] = params.g1(X[i], T[k]);
		l = params.g1(X[i], T[k]);
		std::cout << l;
	}

	for (int i = 0; i < NX; i++)
		res[NY - 1][i] = params.g3(X[i], T[k]);

	for (int j = 0; j < NY; j++)
		res[j][0] = params.g2(Y[j], T[k]);

	for (int j = 0; j < NY; j++)
		res[j][NX - 1] = params.g4(Y[j], T[k]);


	Array<Array<double>> Am{};
	Array<double> Bm{};

	Am.setLength(NX - 2);
	for (int j = 0; j < NX - 2; j++)
		Am[j].setLength(NX - 2);

	for (int i = 0; i < NX - 2; i++)
		for (int j = 0; j < NX - 2; j++)
			Am[i][j] = 0;

	Bm.setLength(NX - 2);

	Am[0][0] = 1 / (0.5 * params.tau) + 2 / (params.h1 * params.h1);
	Am[0][1] = -1 / (params.h1 * params.h1);

	for (int i = 1; i < NX - 3; i++) {
		Am[i][i - 1] = -1 / (params.h1 * params.h1);
		Am[i][i] = 1 / (0.5 * params.tau) + 2 / (params.h1 * params.h1);
		Am[i][i + 1] = -1 / (params.h1 * params.h1);
	}

	Am[Am.len() - 1][Am.len() - 1] = 1 / (0.5 * params.tau) + 2 / (params.h1 * params.h1);
	Am[Am.len() - 1][Am.len() - 2] = -1 / (params.h1 * params.h1);

	Array<double> x{};
	for (int j = 1; j < NY - 1; j++) {
		Bm[0] = 1 / (params.h1 * params.h1) * params.g2(Y[j], T[k]) + 1 / (0.5 * params.tau) * u[j][1] + Derivatives::center(u[j - 1][1], u[j][1], u[j + 1][1], params.h2) + params.f(X[1], Y[j], T[k]);


		for (int i = 1; i < NX - 3; i++)
			Bm[i] = 1 / (0.5 * params.tau) * u[j][i + 1] + Derivatives::center(u[j - 1][i + 1], u[j][i + 1], u[j + 1][i + 1], params.h2) + params.f(X[i + 1], Y[j], T[k]);

		Bm[Bm.len() - 1] = 1 / (params.h1 * params.h1) * params.g4(Y[j], T[k]) + 1 / (0.5 * params.tau) * u[j][u[0].len() - 2] + Derivatives::center(u[j - 1][u[0].len() - 2], u[j][u[0].len() - 2], u[j + 1][u[0].len() - 2], params.h2) + params.f(X[X.len() - 2], Y[j], T[k]);

		x = LinAlg::runThrought(Am, Bm);


		for (int i = 1; i < NX - 1; i++)
			res[j][i] = x[i - 1];
	}

	return res;
}

MyClasses::Array<MyClasses::Array<double>> MyClasses::Methods::VariableDirections::secondStep(int k, Array<Array<double>> u) {
	Array<Array<double>> res{};

	res.setLength(NY);
	for (int j = 0; j < NY; j++)
		res[j].setLength(NX);

	// Краевые условия
	for (int i = 0; i < NX; i++)
		res[0][i] = params.g1(X[i], T[k]);

	for (int i = 0; i < NX; i++)
		res[NY - 1][i] = params.g3(X[i], T[k]);

	for (int j = 0; j < NY; j++)
		res[j][0] = params.g2(Y[j], T[k]);

	for (int j = 0; j < NY; j++)
		res[j][NX - 1] = params.g4(Y[j], T[k]);

	Array<Array<double>> Am{};
	Array<double> Bm{};

	Am.setLength(NY - 2);
	for (int j = 0; j < NY - 2; j++)
		Am[j].setLength(NY - 2);

	for (int i = 0; i < NY - 2; i++)
		for (int j = 0; j < NY - 2; j++)
			Am[i][j] = 0;

	Bm.setLength(NY - 2);

	Am[0][0] = 1 / (0.5 * params.tau) + 2 / (params.h2 * params.h2);
	Am[0][1] = -1 / (params.h2 * params.h2);

	for (int j = 1; j < NY - 3; j++) {
		Am[j][j - 1] = -1 / (params.h2 * params.h2);
		Am[j][j] = 1 / (0.5 * params.tau) + 2 / (params.h2 * params.h2);
		Am[j][j + 1] = -1 / (params.h2 * params.h2); 
	}

	Am[Am.len() - 1][Am.len() - 1] = 1 / (0.5 * params.tau) + 2 / (params.h2 * params.h2);
	Am[Am.len() - 1][Am.len() - 2] = -1 / (params.h2 * params.h2);

	Array<double> x{};
	for (int i = 1; i < NX - 1; i++) {
		Bm[0] = 1 / (params.h1 * params.h1) * params.g1(X[i], T[k]) + 1 / (0.5 * params.tau) * u[1][i] + Derivatives::center(u[1][i - 1], u[1][i], u[1][i + 1], params.h1) + params.f(X[i], Y[1], T[k]);

		for (int j = 1; j < NY - 3; j++)
			Bm[j] = 1 / (0.5 * params.tau) * u[j + 1][i] + Derivatives::center(u[j + 1][i - 1], u[j + 1][i], u[j + 1][i + 1], params.h1) + params.f(X[i], Y[j + 1], T[k]);

		Bm[Bm.len() - 1] = 1 / (params.h2 * params.h2) * params.g3(X[i], T[k]) + 1 / (0.5 * params.tau) * u[u.len() - 2][i] + Derivatives::center(u[u.len() - 2][i - 1], u[u.len() - 2][i], u[u.len() - 2][i + 1], params.h1) + params.f(X[i], Y[Y.len() - 2], T[k - 1]);

		x = LinAlg::runThrought(Am, Bm);

		for (int j = 1; j < NY - 1; j++)
			res[j][i] = x[j - 1];
	}

	return res;
}

void MyClasses::Methods::VariableDirections::init(const VariableDirectionsParams& params) {
	VariableDirections::params = params;

	NX = (int)(params.A / params.h1) + 1;
	NY = (int)(params.B / params.h2) + 1;
	int NT = int(params.T / params.tau) + 1;
	NT_2 = (int)(params.T / (params.tau / 2)) + 1;

	X.setLength(NX);
	for (int i = 0; i < NX; i++)
		X[i] = i * params.h1;

	Y.setLength(NY);
	for (int i = 0; i < NY; i++)
		Y[i] = i * params.h2;

	T.setLength(NT_2);
	for (int i = 0; i < NT_2; i++)
		T[i] = i * params.tau / 2;

	//u.setLength(NT); 

	//for (int k = 0; k < NT; k++) {
	//	u[k].setLength(NY);
	//	for (int j = 0; j < NY; j++)
	//		u[k][j].setLength(NX);
	//}

	prepared = true;
}

MyClasses::Methods::VariableDirectionsResult MyClasses::Methods::VariableDirections::start() {
	Array<Array<double>> u{};

	u.setLength(NY);
	for (int j = 0; j < NY; j++)
		u[j].setLength(NX);

	for (int j = 0; j < NY; j++)
		for (int i = 0; i < NX; i++)
			u[j][i] = params.u0(X[i], Y[j]);

	Array<Array<Array<double>>> U{};
	U.append(u);

	for (int k = 1; k < NT_2; k += 2) {
		u = firstStep(k, u);
		u = secondStep(k + 1, u);
		U.append(u);
	}

	VariableDirectionsResult res = {
		X,
		Y,
		T,
		U
	};

	return res;
}

double MyClasses::Derivatives::center(double u_right, double u_center, double u_left, double step)
{
	return (u_right - 2 * u_center + u_left) / (step * step);
}
