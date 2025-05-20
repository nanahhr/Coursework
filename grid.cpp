/* ----------------------------------------------------------------<Header>-
 Name: grid.cpp
 Group: TV-42
 Student: Hryhorenko A.A.
 Written: 2025-05-18
 Revised: 
 Description: Manages a grid structure with cells and hints, including loading
              from file, resizing, and cell state management.
              The Hint structure represents hints associated with cells
              in the grid, including position, number, and direction.
 ------------------------------------------------------------------</Header>-*/
 
#include "grid.h"

/*Конструктор з параметрами для підказок.*/
Hint::Hint(int r, int c, int num, char dir) : 
	row(r), col(c), num(num), direction(dir) {}

/*Контруктор без параметрів для сітки.*/
Grid::Grid() : rows(0), cols(0), is_solved(false) {}

/*Конструктор з параметрами(рядки і стовпці) для сітки.*/
Grid::Grid(int r, int c) 
	: rows(r),
	cols(c), is_solved(false),
	cells(r, vector<CellState>(c, CellState::EMPTY)) {}
	   
/*Повертає кількість рядків сітки.*/
int Grid::get_rows() const {return rows;}
	
/*Повертає кількість стовпців сітки.*/
int Grid::get_cols() const {return cols;}
	
/*Повертає клітинки.*/
CellState Grid::get_cell(int row, int col) const {return cells[row][col];}
	
/*Повертає вектор з підказками.*/
vector<Hint> Grid::get_hints() const {return hints;}

/*Якщо є, знаходить підказку за координатами.*/
const Hint* Grid::get_hint_at(int row, int col) const {
	if(cells[row][col] == CellState::HINT){
		for(auto& h : hints){
			if(h.row == row && h.col == col){
				return &h;
			}
		}
	}
	return nullptr;
}

/*Повертає стан вирішення сітки.*/
bool Grid::get_is_solved() const {return is_solved;}
	
/*Встановлює значення клітинки сітки.*/	
void Grid::set_cell(int row, int col, CellState state){cells[row][col] = state;}

/*Змінює розміри сітки та очищує її вміст.*/
void Grid::resize_grid(int nr, int nc){
	if(nr <= 0 || nc <= 0){
		cerr << "Invalid input. Rows and columns must be positive. ";
		cerr << "Grid not resized." << endl;
		return;
	}
	this->rows = nr;
	this->cols = nc;
	this->hints.clear();
	
	this->cells.assign(this->rows, vector<CellState>(this->cols, CellState::EMPTY));
	
	
	this->is_solved = false;
	
	cout << "Grid resized to (" << this->rows << "," << this->cols << ")." << endl; 
}	

/*Додає підказку для сітки.*/
void Grid::add_hint(const Hint& hint){
	hints.push_back(hint);
	cells[hint.row][hint.col] = CellState::HINT;
}
/*Встановлює стан вирішення сітки.*/
void Grid::set_is_solved(bool solved){is_solved = solved;}
	
/*Перевіряє, чи координати клітинки є в межах сітки.*/
bool Grid::is_valid(int row, int col) const{
	return row >= 0 && row < rows && col >= 0 && col < cols;
}
	
/*
	Завантажує сітку з файлу з перевіркою коректності даних:
	розмірів, кількості та параметрів підказок.
	Повертає true при успіху, false — при помилках.
*/
bool Grid::load_from_file(const string& filename){
	ifstream file(filename);
		
	if(!file.is_open()){
		cerr << "Cannot open file <" << filename << ">." << endl;
		return false;
	}
	int file_nr, file_nc;
	
	if(!(file >> file_nr >> file_nc) || file_nr <= 0 || file_nc <= 0){
		cerr << "Failed to read grid size from <" << filename << ">." << endl;
		file.close();
		return false;
	}
	
	this->resize_grid(file_nr, file_nc);
	
	int file_hint_count;
	
	if(!(file >> file_hint_count) || file_hint_count < 0){
		cerr << "Failed to read number of hints from <" << filename << ">." << endl;
		file.close();
		return false;
	}
	
	for(int i = 0; i < file_hint_count; i++){
		int r_hint, c_hint, hint_num;
		char dir_char_input;
		
		if(!(file >> r_hint >> c_hint >> hint_num >> dir_char_input)){
			cerr << "Failed to read data for hint №" << (i+1) << "." << endl;
			file.close();
			return false;
		}
		
		if(r_hint < 0 || r_hint >= this->rows ||c_hint < 0 || c_hint >= this-> cols){
			cerr << "Hint №" << (i+1) << " coordinates (" << r_hint << "," << c_hint 
				<< ") are out of grid range in file <" << filename << ">." << endl;
		    file.close();
			return false;
		}
		
		if(hint_num < 0){
			cerr << "Hint №" << (i+1) << " has negative value in file <" << filename << ">." << endl;
			file.close();
			return false;
		}
		
		char dir_final = toupper(static_cast<unsigned char>(dir_char_input));
		if(dir_final != 'U' && dir_final != 'D' && dir_final != 'L' && dir_final != 'R'){
			cerr << "Hint №" << (i+1) << " has invalid direction(not U/D/L/R) in file <" << filename << ">." << endl;
			file.close();
			return false;
		}
		
		if(this->cells[r_hint][c_hint] == CellState::HINT){
			cerr << "Hint №" << (i+1) << " is a dublicate. Skipping this hint from file <" << filename << ">." << endl;
			continue;
		}
		
		add_hint(Hint(r_hint, c_hint, hint_num, dir_final));
	}
	
	file.close();
	return true;
}