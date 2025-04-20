#include <iostream>
#include <vector>
using namespace std;

struct Hint {
    int row, col;
    int count;
    char direction;
	Hint(int r, int c, int cn, char dir) : row(r), col(c), count(cn), direction(dir) {}
};
enum class CellState{EMPTY, FILLED, HINT};

class Grid{
	vector<CellState> cells;
	vector<Hint> hints;
	int rows, cols;
	bool is_solved;
public:
	Grid() : rows(0), cols(0), is_solved(false) {}
	Grid(int r, int c) 
    : cells(r, vector<CellState>(c, CellState::EMPTY)),
      rows(r), cols(c), is_solved(false) {}
	Grid(const Grid& other)
    : cells(other.cells),
      hints(other.hints),
      rows(other.rows),
      cols(other.cols),
      is_solved(other.is_solved) {}
	  
	 
	int get_rows(){return rows;}
	int get_cols(){return cols;}
	CellState get_cell(int row, int col){return cells[row][col]}
	vector<Hint>& get_hints(){return hints;}
	bool get_is_solved(){return is_solved;}
	
	
	void set_cell(int row, int col, CellState state){cells[row][col] = state;}
	void add_hint(Hint& hint){
		hints.push_back(hint);
		cells[hint.row][hint.col] = CellState::HINT;
	}
	void set_is_solved(bool solved){is_solved = solved;}
	
	void is_cell_valid(int row, int col){
		return row >= 0 && row < rows && col >= 0 && col < cols;
	}
};	
