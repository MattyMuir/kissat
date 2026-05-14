#include "KissatSolver.h"

extern "C"
{
#define export export_
#include "internal.h"
#include "file.h"
#include "parse.h"
#include "collect.h"
#undef export
}

KissatSolver::KissatSolver()
{
	solver = kissat_init();
}

KissatSolver::~KissatSolver()
{
	kissat_release(solver);
}

void KissatSolver::LoadExpression(int maxVar_, const std::vector<std::vector<int64_t>>& clauses)
{
	maxVar = maxVar_;
	kissat_reserve(solver, maxVar);

	for (const auto& clause : clauses)
	{
		for (int64_t lit : clause)
			kissat_add(solver, (int)lit);
		kissat_add(solver, 0);
	}

	if (!solver->inconsistent)
		kissat_defrag_watches(solver);
}

void KissatSolver::LoadDimacs(const std::string& filepath)
{
	// Open file
	file file;
	kissat_open_to_read_file(&file, filepath.c_str());

	// Parse DIMACS
	uint64_t lineno;
	const char* error = kissat_parse_dimacs(solver, NORMAL_PARSING, &file, &lineno, &maxVar);

	// Close file
	kissat_close_file(&file);
}

KissatSolver::Result KissatSolver::Solve()
{
	int res = kissat_solve(solver);
	switch (res)
	{
	case 10: return Result::Satisfiable;
	case 20: return Result::Unsatisfiable;
	default: return Result::Unknown;
	}
}

std::vector<int> KissatSolver::GetWitness()
{
	std::vector<int> witness;

	for (int eidx = 1; eidx <= maxVar; eidx++)
	{
		int tmp = kissat_value(solver, eidx);
		if (!tmp) tmp = eidx;
		witness.push_back(tmp);
	}
	
	return witness;
}