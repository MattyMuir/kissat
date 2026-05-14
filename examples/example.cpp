#include <iostream>
#include <format>

#include <kissatlib.h>

int main()
{
	KissatSolver solver;
	std::vector<std::vector<int64_t>> clauses{
		{ 1, 2 },
		{ -1, 2 }
	};

	solver.LoadExpression(2, clauses);
	solver.Solve();
}