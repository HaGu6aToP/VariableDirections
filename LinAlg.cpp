#include "LinAlg.h"


MyClasses::Array<double> MyClasses::LinAlg::runThrought(const Array<Array<double>>& A, const Array<double>& B)
{
	Array<double> res{};
	if (!runThroughtCheckCondition(A))
		return res;

	res.setLength(A.len());

	Array<double> alpha{};
	alpha.append(-A[0][1] / A[0][0]);

	Array<double> beta{};
	beta.append(B[0] / A[0][0]);


	double a, b, c, d, e;
	// Прямой ход
	for (int i = 1; i < A.len() - 1; i++) {
		a = A[i][i - 1];
		b = A[i][i];
		c = A[i][i + 1];
		d = B[i];
		e = a * alpha[i - 1] + b;

		alpha.append(-c / e);
		beta.append((d - a * beta[i - 1]) / e);
	}

	res[A.len() - 1] = ((B[B.len() - 1] - A[A.len() - 1][A.len() - 2] * beta[beta.len() - 1]) / (A[A.len() - 1][A.len() - 1] + A[A.len() - 1][A.len() - 2] * alpha[alpha.len() - 1]));

	// Обратный ход
	for (int i = A.len() - 2; i > -1; i--) {
		a = alpha[i];
		b = beta[i];
		res[i] = (a * res[i + 1] + b);
	}

	return res;
}

bool MyClasses::LinAlg::runThroughtCheckCondition(const Array<Array<double>>& A) {
	int a, b, c;
	bool flag = false;
	int n = A.len();

	b = A[0][0];
	c = A[0][1];
	if (!(std::abs(b) >= std::abs(c)))
		return false;

	if (std::abs(b) > std::abs(c))
		flag = true;

	b = A[n - 1][n - 1];
	a = A[n - 1][n - 2];

	if (!(std::abs(b) >= std::abs(a)))
		return false;

	if (!flag)
		if (std::abs(b) > std::abs(a))
			flag = true;

	for (int i = 1; i < n - 1; i++) {
		a = A[i][i - 1];
		b = A[i][i];
		c = A[i][i + 1];

		if (!(std::abs(b) >= (std::abs(a) + std::abs(c))))
			return false;

		if (!flag)
			if (std::abs(b) > std::abs(a) + std::abs(c))
				flag = true;
	}

	return flag;
}
