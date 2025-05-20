/* ----------------------------------------------------------------<Header>-
 Name: solver.cpp
 Title: Implementation of Solver class for grid puzzle
 Group: TV-42
 Student: Hryhorenko A.A.
 Written: 2025-05-18
 Revised: 
 Description: Contains methods to check adjacency, place hints,
              fill lines, and solve the grid puzzle based on hints.
              Ensures rules compliance and continuous path creation.
 ------------------------------------------------------------------</Header>-*/


#include "solver.h"

/*Конструктор Solver.*/
Solver::Solver() {}

/*
    Перевіряє, чи є у вказаній клітинці сітки (r, c) хоча б одна суміжна
    по ортогоналі (вгорі, внизу, зліва або справа) клітинка зі станом FILLED.
    Повертає true, якщо така клітинка знайдена, інакше false.
*/
bool Solver::has_orthogonally_adjacent_filled(const Grid& grid, int r, int c) const{
    static const int dr_adj[] = {0, 0, 1, -1};
    static const int dc_adj[] = {1, -1, 0, 0};

    for (int i = 0; i < 4; i++) {
        int nr = r + dr_adj[i];
        int nc = c + dc_adj[i];
        if (grid.is_valid(nr, nc) && grid.get_cell(nr, nc) == CellState::FILLED) {
			return false;
        }
    }
    return false;
}

/*
    Перевіряє, чи можна прокласти шлях довжиною hint.num з підказки на сітці:
    - Перевіряє напрямок руху і кожну клітинку шляху.
    - Клітинки мають бути в межах сітки, порожні, без інших підказок і без сусідів FILLED.
    Повертає true, якщо шлях можливий, і false з повідомленням про помилку — інакше.
*/
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

/*
    Розміщує FILLED клітинки на сітці відповідно до підказки:
    - Спочатку перевіряє можливість розміщення через can_place_from_hints.
    - За напрямком заповнює hint.num клітинок станом FILLED.
    - Якщо при розміщенні виявлено сусідство FILLED клітинок або непорожня клітинка — виводить помилку і повертає false.
    - Інакше повертає true.
*/
bool Solver::place_from_hints(Grid& grid, const Hint& hint){
    if(!can_place_from_hints(grid, hint)) return false;

    int dr = 0, dc = 0;
    if (hint.direction == 'R') dc = 1;
    else if (hint.direction == 'L') dc = -1;
    else if (hint.direction == 'U') dr = -1;
    else if (hint.direction == 'D') dr = 1;

    int r = hint.row;
    int c = hint.col;

    for (int i = 0; i < hint.num; ++i) {
        r += dr;
        c += dc;

		if (!grid.is_valid(r, c)) {
			cerr << "Out of bounds at (" << r << "," << c << ") for hint (" 
				 << hint.row << "," << hint.col << ")." << endl;
			return false;
		}
        if (has_orthogonally_adjacent_filled(grid, r, c)) {
            cerr << "Adjacency conflict attempting to place FILLED at (" 
                      << r << "," << c << ") for hint (" 
                      << hint.row << "," << hint.col << ")." << endl;
            return false; 
        }
		
		if(grid.get_cell(r, c) != CellState::EMPTY){
			cerr << "Attempting to place filled at cell (" 
					<< r << "," << c << ") that is not empty for hint (" 
                      << hint.row << "," << hint.col << ")." << endl;

		}
        grid.set_cell(r, c, CellState::FILLED);
    }
    return true;
}

/*Видаляє розміщені заповнені клітинки, створені за підказкою.*/
void Solver::remove_from_hint(Grid& grid, const Hint& hint) {
    int dr = 0, dc = 0;
    if (hint.direction == 'R') dc = 1;
    else if (hint.direction == 'L') dc = -1;
    else if (hint.direction == 'U') dr = -1;
    else if (hint.direction == 'D') dr = 1;

    int r = hint.row, c = hint.col;
    for (int i = 0; i < hint.num; i++) {
        r += dr;
        c += dc;
        grid.set_cell(r, c, CellState::EMPTY);
    }
}

/*
	Рекурсивно намагається розташувати підказки по черзі на сітці.
	Якщо всі підказки розмістили без конфліктів, повертає true,
	інакше повертає false і відкатує зміни.
*/
bool Solver::backtrack(Grid& grid, int hint_index) {
    if (hint_index >= (int)grid.get_hints().size()) {
        return true;
    }

    const Hint& hint = grid.get_hints()[hint_index];

    if (!can_place_from_hints(grid, hint)) return false;

    place_from_hints(grid, hint);

    if (backtrack(grid, hint_index + 1)) {
        return true;
    }

    remove_from_hint(grid, hint);
    return false;
}
/*
	Заповнює суміжні порожні клітинки лінією,
	починаючи з заданої позиції.
*/
void Solver::fill_line(Grid& grid, int r, int c, vector<vector<bool>>& visited) {
    if (!grid.is_valid(r, c)) return;
    if (visited[r][c]) return;
    if (grid.get_cell(r, c) != CellState::EMPTY) return;

    stack<pair<int, int>> stack;
    stack.push({r, c});

    int dr[] = {1, -1, 0, 0};
    int dc[] = {0, 0, 1, -1};

    while (!stack.empty()) {
        auto [row, col] = stack.top();
        stack.pop();

        if (!grid.is_valid(row, col)) continue;
        if (visited[row][col]) continue;
        if (grid.get_cell(row, col) != CellState::EMPTY) continue;

        visited[row][col] = true;
        grid.set_cell(row, col, CellState::LINE);

        for (int i = 0; i < 4; ++i) {
            int nr = row + dr[i];
            int nc = col + dc[i];
            stack.push({nr, nc});
        }
    }
}

/*
	Головна функція розв'язання головоломки:
	очищає всі клітинки, крім підказок, виконує backtracking,
	перевіряє чи утворена одна суцільна лінія без пропусків, 
	позначає розв'язок як успішний або ні.
*/
void Solver::solve(Grid& grid, bool& solved) {
    int rows = grid.get_rows();
    int cols = grid.get_cols();

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid.get_cell(r, c) != CellState::HINT) {
                grid.set_cell(r, c, CellState::EMPTY);
            }
        }
    }

    grid.set_is_solved(false);

    if (!backtrack(grid, 0)) {
        solved = false;
        return;
    }

    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    bool started = false;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid.get_cell(r, c) == CellState::EMPTY && !visited[r][c]) {
                if (started) {
                    solved = false;
                    return;
                }
                fill_line(grid, r, c, visited);
                started = true;
            }
        }
    }
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid.get_cell(r, c) == CellState::EMPTY) {
                solved = false;
                return;
            }
        }
    }

    grid.set_is_solved(true);
    solved = true;
}
