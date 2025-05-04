#include "grid.h"

//Конструктор з параметрами для підказок.
Hint::Hint(int r, int c, int num, char dir) : 
	row(r), col(c), num(n), direction(dir) {}

//Контруктор без параметрів для сітки.
Grid::Grid() : rows(0), cols(0), is_solved(false) {}

//Конструктор з параметрами(рядки і стовпці).
Grid::Grid(int r, int c) 
	: rows(r),
	cols(c),
	is_solved(false),
	cells(r, vector<CellState>(c, CellState::EMPTY)) {}
	   
//Повертає кількість рядків сітки.  
int Grid::get_rows(){return rows;}
	
//Повертає кількість стовпців сітки.
int Grid::get_cols(){return cols;}
	
//Повертає клітинки.
CellState Grid::get_cell(int row, int col){return cells[row][col]}
	
//Повертає вектор з підказками.
vector<Hint>& Grid::get_hints(){return hints;}

//Якщо є, знаходить підказку за координатами.
Hint* get_hint_at(int row, int col){
	if(cells[row][col] == CellState::HINT){
		for(auto& h : hints){
			if(h.row == row && h.col == col){
				return &h;
			}
		}
	}
	return nullptr;
}

//Повертає стан вирішення сітки.
bool Grid::get_is_solved(){return is_solved;}
	
//Встановлює значення клітинки сітки.	
void Grid::set_cell(int row, int col, CellState state){cells[row][col] = state;}
	
//Додає підказку для сітки.
void Grid::add_hint(Hint& hint){
	hints.push_back(hint);
	cells[hint.row][hint.col] = CellState::HINT;
}
//Встановлює стан вирішення сітки.
void Grid::set_is_solved(bool solved){is_solved = solved;}
	
//Перевіряє, чи координати клітинки є в межах сітки.
void Grid::is_valid(int row, int col){
	return row >= 0 && row < rows && col >= 0 && col < cols;
}
	
//Функція для отримання данних з файлу.
void Grid::load_from_file(string& file_name){
	ifstream file(file_name);
		
	if(!file){
		cerr << "Cannot open file.\n";
		return 1;
	}
	file >> rows >> cols;
	cells.resize(rows, vector<CellState>(cols, CellState::EMPTY));
			
	int hints_count;
	file >> hints_count;
			
	for (int i = 0; i < hints_count; ++i) {
		Hint hint;
		char dir;
		file >> hint.row >> hint.col >> hint.num >> dir;

		if (hint.row < 0 || hint.row >= rows || 
			hint.col < 0 || hint.col >= cols) {
			cerr << "Hint is out of range.";
		}

		hint.direction = toupper(dir);
		hints.push_back(hint);
		cells[hint.row][hint.col] = CellState::HINT;
	}
}

//Функція для отримання данних з консолі.
void Grid::input_from_console() {
		
    cout << "Enter grid size (rows columns): ";
    while (!(cin >> rows >> cols) || rows < 1 || cols < 1) {
        cerr << "Enter two positive numbers.\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    cells.assign(rows, vector<CellState>(cols, CellState::EMPTY));
	
    cout << "Number of hints: ";
    int n;
    while (!(cin >> n) || n < 0) {
        cerr << "Invalid number of hints. Enter a number >= 0\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    for (int i = 0; i < n; ++i) {
        Hint h;
        char dir;
        
        cout << "Hint №" << i+1 << " (row column number direction): ";
        while (!(cin >> h.row >> h.col >> h.num >> dir) || 
               h.row < 0 || h.row >= rows || 
               h.col < 0 || h.col >= cols ||
               (dir = tolower(dir), !strchr("udlr", dir))) {
            cerr << "Input error. Try again.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        hint.direction = direction;

        hints.push_back(hint);
        cells[hint.row][hint.col] = CellState::HINT;
    }
}

//Функція для виведення сітки.
void Grid::print_grid() {
	cout << "      ";
	for (int i = 0; i < col; ++i) cout << setw(2) <<  i << "  ";
	cout << "\n     ";
	for (int i = 0; i < col; ++i) cout << "----";
	cout << endl;

	for (int i = 0; i < row; i++) {
		cout << setw(3) <<  i << " " << "| ";
		for (int j = 0; j < col; j++) {
			switch (cells[i][j]) {
				case EMPTY: cout << " · "; break;
				case FILLED: cout << " # "; break;
				case HINT: {
					for (const Hint& h : hints) {
						if (h.r == i && h.c == j) {
							cout << setw(2) << h.num;
							switch (h.dir) {
								case 'r': cout << "→"; break;
								case 'l': cout << "←"; break;
								case 'u': cout << "↑"; break;
								case 'd': cout << "↓"; break;
								default: cout << "?"; break;
							}
							break;
						}
					}
					break;
				}
				case LINE: cout << " + "; break;
			}
			cout << " ";
		}
		cout << "|" << endl;
	}
	cout << "     ";
	for (int i = 0; i < col; i++) cout << "----";
	cout << endl;
}
