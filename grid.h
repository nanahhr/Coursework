#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>
#include <cstring>

using namespace std;


struct Hint {
    int row, col;
    int num;
    char direction;
	Hint(int r, int c, int n, char dir);
};

enum class CellState{EMPTY, FILLED, HINT, LINE};

class Grid{
	int rows, cols;
	bool is_solved;
	vector<vector<CellState>> cells;
	vector<Hint> hints;
public:

	Grid();
	Grid(int r, int c);

	int get_rows() const;
	int get_cols() const;
	CellState get_cell(int row, int col) const;
	vector<Hint> get_hints() const;
	const Hint* get_hint_at(int row, int col) const;
	bool get_is_solved() const;	
	
	void set_cell(int row, int col, CellState state);
	void resize_grid(int nr, int nc);
	void add_hint(const Hint& hint);
	void set_is_solved(bool solved);

	bool is_valid(int row, int col);
	bool load_from_file(const string& file_name);
};	
#endif
