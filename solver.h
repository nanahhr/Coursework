#ifndef SOLVER_H
#define SOLVER_H

#include "grid.h"
#include <iostream>
#include <vector>
#include <stack>
#include <utility>
using namespace std;

class Solver{
	bool has_orthogonally_adjacent_filled(const Grid& grid, int r, int c) const;
	bool can_place_from_hints(const Grid& grid, const Hint& hint) const;
	bool place_from_hints(Grid& grid, const Hint& hint);
	void fill_line(Grid& grid, int r, int c, vector<vector<bool>>& visited);
public:
	Solver();
	void solve(Grid& grid_to_solve, bool &solved);
};
#endif