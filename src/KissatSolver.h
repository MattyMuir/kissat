#pragma once
#include <string>
#include <vector>

#ifdef __cplusplus
extern "C" {
#endif
#include "kissat.h"
#ifdef __cplusplus
}
#endif

class KissatSolver
{
public:
	enum Result
	{
		Unsatisfiable,
		Satisfiable,
		Unknown
	};

public:
	KissatSolver();
	~KissatSolver();

	void LoadExpression(int maxVar_, const std::vector<std::vector<int64_t>>& clauses);
	void LoadDimacs(const std::string& filepath);

	Result Solve();
	std::vector<int> GetWitness();

protected:
	kissat* solver;
	int maxVar;
};