#include "solver.h"

Solver::Solver() {}

bool Solver::has_orthogonally_adjacent_filled(const Grid& grid, int r, int c) const{
    static const int dr_adj[] = {0, 0, 1, -1};
    static const int dc_adj[] = {1, -1, 0, 0};

    for (int i = 0; i < 4; i++) {
        int nr = r + dr_adj[i];
        int nc = c + dc_adj[i];
        if (grid.is_valid(nr, nc) && grid.get_cell(nr, nc) == CellState::FILLED) {
            return true;
        }
    }
    return false;
}

bool Solver::can_place_from_hints(const Grid& grid, const Hint& hint) const{
	int dr = 0, dc = 0;
    if (hint.direction == 'R') dc = 1;
    else if (hint.direction == 'L') dc = -1;
    else if (hint.direction == 'U') dr = -1;
    else if (hint.direction == 'D') dr = 1;
	else {
		cerr << "Invalid direction <" << hint.direction << "> for a hint." << endl;
		return false;
	}
	
    int r = hint.row, c = hint.col;
	
    for (int i = 0; i < hint.num; i++) {
		r += dr, c += dc;
		
        if (!grid.is_valid(r, c)){
			cerr << "Path for a hint at (" << r << "," << c << ") is out of bounds." << endl;
			return false;
		}
		CellState cell_at_rc = grid.get_cell(r, c);
		
        if (cell_at_rc == CellState::HINT){
			cerr << "Another hint at (" << r << "," << c << ")." << endl;
			return false;
		}
        if (cell_at_rc != CellState::EMPTY) {
            cerr << "Path for a hint at (" << r << "," << c << ") is not empty." << endl;
            return false;
        }
        if (has_orthogonally_adjacent_filled(grid, r, c)) {
            cerr << "Placing FILLED at (" << r << "," << c << ") results in orthogonal neighbords." << endl;
            return false;
        }
    }
    return true;
}