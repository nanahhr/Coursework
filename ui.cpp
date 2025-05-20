/* ----------------------------------------------------------------<Header>-
 Name: ui.cpp
 Group: TV-42
 Student: Hryhorenko A.A.
 Written: 2025-05-18
 Revised: 
 Description: Manages user interaction with a grid-based puzzle.

 ------------------------------------------------------------------</Header>-*/

#include "ui.h"

/*Конструктор UI.*/
UI::UI(){grid_loaded = false;}

/*
	Запускає цикл роботи програми:
    - скидає прапорець завантаження сітки
    - поки користувач не вийде,
      якщо сітка не завантажена — показує головне меню,
      якщо сітка завантажена — показує меню дій
    - коли користувач виходить, виводить повідомлення про завершення
*/
void UI::run_program() {
	grid_loaded = false;
	while (true) {
		if (!grid_loaded) {
			if (!main_menu()) break;
		}
		if (grid_loaded && !action_menu()) {
			grid_loaded = false;
		}
	}
	cout << "End." << endl;
}

/*
    Відображає головне меню:
    - отримує вибір користувача
    - викликає відповідні функції:
      1 - меню вибору джерела введення сітки
      2 - меню тестових сіток
      3 - вихід з програми
*/
bool UI::main_menu() {
	display.display_main_menu();
	int choice = input.get_menu_choice(1, 3);
	switch (choice) {
		case 1:
			input_source_menu();
			break;
		case 2:
			test_grid_menu();
			break;
		case 3:
			return false;
	}
	return true;
}

/*
    Керує меню вибору джерела введення сітки:
    - показує меню,
    - отримує вибір користувача,
    - виконує створення сітки з консолі, файлу або повертається назад.
*/
void UI::input_source_menu() {
	display.display_input_source_menu();
	int choice = input.get_menu_choice(1, 3);
	switch (choice) {
		case 1:
			create_grid_from_console();
			break;
		case 2:
			create_grid_from_file();
			break;
		case 3:
			return;
	}
}

/*
	Керує меню вибору дій:
	- виводить меню дій,
	- отримує вибір користувача,
	- викликає відповідну функцію згідно з вибором.
	Якщо вибрано пункт виходу, повертається до основного меню.
*/
bool UI::action_menu() {
	display.display_action_menu();
	int choice = input.get_menu_choice(1, 3);
	switch (choice) {
		case 1:
			solve_manually();
			break;
		case 2:
			solve_grid_programmatically();
			break;
		case 3:
			cout << "Returning to main menu." << endl;
			return false;
	}
	return true;
}

/*
	Керує меню вибору тестових сіток:
	- виводить меню вибору,
	- отримує вибір користувача,
	- завантажує відповідну тестову сітку,
	- встановлює прапорець grid_loaded у true,
	- виводить завантажену сітку.
	Якщо вибрано пункт виходу, функція завершується без змін.
*/
void UI::test_grid_menu() {
	display.display_test_grid_menu();
	int choice = input.get_menu_choice(1, 4);
	bool grid_chosen = false;

	switch (choice) {
		case 1:
			load_test_grid_1();
			grid_chosen = true;
			break;
		case 2:
			load_test_grid_2();
			grid_chosen = true;
			break;
		case 3:
			load_test_grid_3();
			grid_chosen = true;
			break;
		case 4:
			return;
	}

	if (grid_chosen) {
		grid_loaded = true;
		cout << COLOR_GREEN << "\nTest grid is loaded." << COLOR_RESET << endl;
		display.display_grid(grid);
	}
}

/*
	Отримує дані від користувача: розміри сітки, підказки.
	Ініціалізує сітку, додає підказки, встановлює прапорець grid_loaded у true 
	та виводить сітку.
*/
void UI::create_grid_from_console() {
	pair<int, int> dimensions = input.enter_grid_dimensions();
	grid.resize_grid(dimensions.first, dimensions.second);

	vector<Hint> hints = input.enter_hints(dimensions.first, dimensions.second);
	for (const Hint& h : hints) {
		grid.add_hint(h);
	}
	grid_loaded = true;
	cout << "\nThe grid was " << COLOR_GREEN << "successfully" << COLOR_RESET << " created." << endl;
	display.display_grid(grid);
}

/*
	Отримує назву файлу, завантажує сітку з цього файлу.
	Якщо завантаження успішне, встановлює прапорець grid_loaded в true та виводить сітку.
*/
void UI::create_grid_from_file() {
	string filename = input.enter_filename();
	cout << "name (" << filename << ")" << endl;
	if (grid.load_from_file(filename)) {
		cout << "\nThe grid was " << COLOR_GREEN << "successfully" << COLOR_RESET << " created from file <" << filename << ">." << endl;
		grid_loaded = true;
		display.display_grid(grid);
	}
}

/*
	Самостійне розв'язання користувача.
	Передбачається, що користувач розв'яже сітку самостійно на папері
	або іншим способом, а потім звірить свій результат з програмою.
	Якщо користувач обрав показати результат, програма намагається знайти
	розв'язок і виводить користувачу для порівняння.
*/
void UI::solve_manually() {
	if (!grid_loaded) {
		cerr << "Grid is not loaded" << endl;
		return;
	}

	cout << "\n      ╔═════════════════════════════════════╗" << endl;
    cout << "      ║        _Solve grid yourself_        ║" << endl;
    cout << "      ╚═════════════════════════════════════╝" << endl;
	display.display_grid(grid);

	bool continue_solving = true;

	while (continue_solving) {
		cout << "\n_Options:_" << endl;
		cout << "1. Show the solution." << endl;
		cout << "2. Return to the action menu." << endl;
		int choice = input.get_menu_choice(1, 2);

		switch (choice) {
			case 1: {
				cout << "\nTrying to show the solution.." << endl;
				Grid solved_grid = this->grid;

				bool solved = false;
				this->solver.solve(solved_grid, solved);

				if (solved) {
					display.display_grid(solved_grid);
				} else {
					cout << COLOR_RED << "Can't find solution for the grid." << endl;
					cout << "Grid does not have a solution or the solver is not complete." << COLOR_RESET<< endl;
				}
				break;
			}
			case 2:
				continue_solving = false;
				break;
		}
	}

	cout << "Returning to the action menu." << endl;
}

/*
	Програмне розв'язання.
	Якщо сітка завантажена, програма створює її копію і намагається знайти розв'язок.
	Якщо розв'язок знайдено, запитується чи застосувати його до основної сітки.
	Після цього або виводиться результат або повідомлення про незнайдений розв'язок.
*/
void UI::solve_grid_programmatically() {
	if (!grid_loaded) {
		cerr << "Grid is not loaded" << endl;
		return;
	}

    cout << "\n      ╔═════════════════════════════════════╗" << endl;
    cout << "      ║    _Solve grid programmatically_    ║" << endl;
    cout << "      ╚═════════════════════════════════════╝" << endl;
	cout << "Starting solver... Please wait..." << endl;

	
	Grid solved_grid = this->grid;
	bool solved = false;
	this->solver.solve(solved_grid, solved);

	if (solved) {
		cout << COLOR_GREEN << "\nGrid is solved." << COLOR_RESET << endl;
		display.display_grid(solved_grid);

		cout << "\nApply this result to the current grid? (y/n): ";
		char response;
		while (true) {
			if (!(cin >> response)) {
				cerr << "Input error. Try again.\n";
				input.clear_input_buffer();
				continue;
			}
			cin.ignore(numeric_limits<streamsize>::max(), '\n');

			response = tolower(response);
			if (response == 'y') {
				this->grid = solved_grid;

				if (this->grid.get_is_solved()) {
					cout << "Result applied. grid marked as solved." << endl;
				} else {
					cout << "Result applied, but the solver did not mark the grid as fully solved." << endl;
				}
				break;
			}
			if (response == 'n') {
				cout << "Result not applied." << endl;
				break;
			}

			cerr << "Please enter 'y' or 'n': ";
		}
	} else {
		cout << COLOR_RED << "Could not find a solution." << COLOR_RESET << endl;
	}
}


/*Створює сітку для тестового завдання 1.*/
void UI::load_test_grid_1(){
	grid.resize_grid(10, 10); 

    grid.add_hint(Hint(0, 0, 2, 'R'));
    grid.add_hint(Hint(1, 2, 0, 'L'));
    grid.add_hint(Hint(1, 7, 0, 'R'));
    grid.add_hint(Hint(2, 5, 0, 'L'));
    grid.add_hint(Hint(3, 3, 1, 'L'));
    grid.add_hint(Hint(3, 6, 0, 'U'));
    grid.add_hint(Hint(4, 2, 0, 'L'));
    grid.add_hint(Hint(4, 6, 0, 'R'));
    grid.add_hint(Hint(4, 9, 0, 'U'));
    grid.add_hint(Hint(5, 4, 0, 'L'));
    grid.add_hint(Hint(5, 7, 0, 'R'));
    grid.add_hint(Hint(6, 2, 0, 'L'));
    grid.add_hint(Hint(7, 5, 1, 'L'));
    grid.add_hint(Hint(7, 7, 1, 'R'));
    grid.add_hint(Hint(8, 2, 0, 'L'));
    grid.add_hint(Hint(8, 7, 0, 'R'));
    grid.add_hint(Hint(9, 5, 0, 'L'));
}

/*Створює сітку для тестового завдання 2.*/
void UI::load_test_grid_2(){
	grid.resize_grid(8, 12); 
	
    grid.add_hint(Hint(0, 3, 3, 'D'));
    grid.add_hint(Hint(0, 11, 2, 'L'));
    grid.add_hint(Hint(1, 7, 1, 'R'));
    grid.add_hint(Hint(2, 5, 1, 'L'));
    grid.add_hint(Hint(2, 10, 0, 'R'));
    grid.add_hint(Hint(3, 5, 1, 'L'));
    grid.add_hint(Hint(5, 1, 1, 'D'));
    grid.add_hint(Hint(5, 6, 2, 'U'));
    grid.add_hint(Hint(5, 8, 2, 'L'));
    grid.add_hint(Hint(6, 8, 1, 'R'));
    grid.add_hint(Hint(6, 9, 1, 'U'));
    grid.add_hint(Hint(7, 11, 1, 'L'));
}

/*Створює сітку для тестового завдання 3.*/
void UI::load_test_grid_3(){
    grid.resize_grid(11, 16);

    grid.add_hint(Hint(0, 11, 1, 'D'));
    grid.add_hint(Hint(0, 15, 1, 'L'));
    grid.add_hint(Hint(1, 1, 1, 'R'));
    grid.add_hint(Hint(1, 5, 1, 'D'));
    grid.add_hint(Hint(1, 13, 1, 'D'));
    grid.add_hint(Hint(2, 1, 2, 'D'));
    grid.add_hint(Hint(2, 3, 3, 'R'));
    grid.add_hint(Hint(2, 5, 2, 'R'));
    grid.add_hint(Hint(3, 2, 1, 'D'));
    grid.add_hint(Hint(3, 4, 1, 'D'));
    grid.add_hint(Hint(3, 7, 1, 'U'));
    grid.add_hint(Hint(3, 8, 2, 'D'));
    grid.add_hint(Hint(3, 9, 2, 'R'));
    grid.add_hint(Hint(4, 7, 2, 'L'));
    grid.add_hint(Hint(4, 8, 0, 'R'));
    grid.add_hint(Hint(4, 9, 0, 'D'));
    grid.add_hint(Hint(5, 7, 0, 'D'));
    grid.add_hint(Hint(5, 12, 0, 'L'));
    grid.add_hint(Hint(5, 13, 1, 'U'));
    grid.add_hint(Hint(5, 14, 1, 'D'));
    grid.add_hint(Hint(6, 1, 4, 'R'));
    grid.add_hint(Hint(6, 7, 2, 'L'));
    grid.add_hint(Hint(6, 11, 3, 'L'));
    grid.add_hint(Hint(7, 7, 1, 'R'));
    grid.add_hint(Hint(7, 11, 1, 'L'));
    grid.add_hint(Hint(8, 3, 3, 'U'));
    grid.add_hint(Hint(8, 11, 0, 'R'));
    grid.add_hint(Hint(9, 5, 0, 'L'));
    grid.add_hint(Hint(9, 12, 0, 'U'));
    grid.add_hint(Hint(9, 13, 1, 'L'));
    grid.add_hint(Hint(9, 14, 1, 'U'));
    grid.add_hint(Hint(10, 5, 0, 'L'));
}