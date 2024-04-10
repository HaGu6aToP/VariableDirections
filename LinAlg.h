#pragma once
#include "Array.h"

namespace MyClasses {
	namespace LinAlg {
		Array<double> runThrought(const Array<Array<double>>& A, const Array<double>& B);
		bool runThroughtCheckCondition(const Array<Array<double>>& A);
	}
}