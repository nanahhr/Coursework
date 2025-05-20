/* ----------------------------------------------------------------<Header>-
 Name: display.cpp
 Group: TV-42
 Student: Hryhorenko A.A.
 Written: 2025-05-18
 Revised: 
 Description: Shows different menu variations.

 ------------------------------------------------------------------</Header>-*/

#include "display.h"

/*Конструктор Display.*/
Display::Display(){}

/*Відображає меню для вибору способу внесення даних (користувачем/програмно).*/
void Display::display_main_menu(){
    cout << "\n╔═════════════════════════════════════╗" << endl;
    cout << "║             _Menu_                  ║" << endl;
    cout << "╠═════════════════════════════════════╣" << endl;
    cout << "║ 1. Enter data by user.              ║" << endl;
    cout << "║ 2. Enter data programmatically.     ║" << endl;
    cout << "║ 3. Quit.                            ║" << endl;
    cout << "╚═════════════════════════════════════╝" << endl;
}

/*Відображає меню для вибору способу внесення даних (з консолі/з файлу).*/
void Display::display_input_source_menu(){
    cout << "\n╔══════════════════════════════════════╗" << endl;
    cout << "║         _Choose input source_        ║" << endl;
    cout << "╠══════════════════════════════════════╣" << endl;
    cout << "║ 1. Enter data from console.          ║" << endl;
    cout << "║ 2. Enter data from file.             ║" << endl;
    cout << "║ 3. Return.                           ║" << endl;
    cout << "╚══════════════════════════════════════╝" << endl;
}

/*Відображає меню для вибору способу розв'язання.*/
void Display::display_action_menu(){
    cout << "\n╔══════════════════════════════════════╗" << endl;
    cout << "║          _Choose an action_          ║" << endl;
    cout << "╠══════════════════════════════════════╣" << endl;
    cout << "║ 1. Solve the grid yourself.          ║" << endl;
    cout << "║ 2. Solve the grid programmatically.  ║" << endl;
    cout << "║ 3. Return to the main menu.          ║" << endl;
    cout << "╚══════════════════════════════════════╝" << endl;
}

/*Відображає меню для вибору варіантів тестових завдань.*/
void Display::display_test_grid_menu(){
    cout << "\n╔═════════════════════════════════════╗" << endl;
    cout << "║           _Choose test grid_        ║" << endl;
    cout << "╠═════════════════════════════════════╣" << endl;
    cout << "║ 1. grid 10x10.                      ║" << endl;
    cout << "║ 2. grid 12x8.                       ║" << endl;
    cout << "║ 3. grid 16x11.                      ║" << endl;
    cout << "║ 4. Return                           ║" << endl;
    cout << "╚═════════════════════════════════════╝" << endl;
}

/*
	Залежно від стану клітинки у сітці, виводить її з відповідним символом
	і відображає межі сітки разом з номером клітинки.
*/
void Display::display_grid(const Grid& grid){
	int rows = grid.get_rows();
	int cols = grid.get_cols();
	
	if(rows == 0 || cols == 0){
		cout << "Grid is empty or not initialized." << endl;
		return;
	}
    cout << "      ";
    for (int i = 0; i < cols; ++i) cout << setw(2) << i << "  ";
    cout << endl;

    cout << "     ╔";
    for (int i = 0; i < cols; ++i) cout << "═══╦";
    cout << "\b╗" << endl;

    for (int i = 0; i < rows; i++) {
        cout << setw(3) << i << "  ║";
        for (int j = 0; j < cols; j++) {
            CellState cell = grid.get_cell(i, j);
            switch (cell) {
                case CellState::EMPTY: cout << "   "; break;
                case CellState::FILLED: cout << " # "; break;
                case CellState::LINE: cout << " + "; break;
                case CellState::HINT: {
                    const Hint* h_ptr = grid.get_hint_at(i, j);
                    if (h_ptr) {
                        cout << setw(2) << h_ptr->num;
                        switch (h_ptr->direction) {
                            case 'R': cout << "→"; break;
                            case 'L': cout << "←"; break;
                            case 'U': cout << "↑"; break;
                            case 'D': cout << "↓"; break;
                            default: cout << "?"; break;
                        }
                    } else {
                        cout << " H?";
                    }
                    break;
                }
                default: cout << " ? "; break;
            }
            cout << "║";
        }
        cout << endl;

        if (i < rows - 1) {
            cout << "     ╠";
            for (int j = 0; j < cols; ++j) cout << "═══╬";
            cout << "\b╣" << endl;
        }
    }

    cout << "     ╚";
    for (int i = 0; i < cols; ++i) cout << "═══╩";
    cout << "\b╝" << endl;

	if (grid.get_is_solved()) {
		cout << COLOR_GREEN << "Status: Solved." << COLOR_RESET << endl;
	} else {
		cout << COLOR_RED << "Status: Not solved." << COLOR_RESET << endl;
	}
}
