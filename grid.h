#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>
using namespace std;


struct Hint {
    int row, col;
    int count;
    char direction;
	Hint(int r, int c, int cn, char dir);
};

enum class CellState{EMPTY, FILLED, HINT};

class Grid{
	vector<vector<CellState>> cells;
	vector<Hint> hints;
	int rows, cols;
	bool is_solved;
public:

	Grid();
	Grid(int r, int c);

	int get_rows();
	int get_cols();
	CellState get_cell(int row, int col);
	vector<Hint>& get_hints();
	bool get_is_solved();	
	
	void set_cell(int row, int col, CellState state);
	void add_hint(Hint& hint);
	void set_is_solved(bool solved);

	void is_cell_valid(int row, int col);
	void load_from_file(string& file_name);
	void input_from_console();
};	
#endif
