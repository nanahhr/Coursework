/* ----------------------------------------------------------------<Header>-
 Name: ui.cpp
 Title: Implementation of User Interface components
 Group: TV-42
 Student: Hryhorenko A.A.
 Written: 2025-05-18
 Revised: 
 Description: Handles input, display updates, and user feedback.
              Manages user interaction with a grid-based puzzle.
              Provides menus for loading grids, solving options, and navigation.

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
	display_main_menu();
	int choice = get_menu_choice(1, 3);
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
	display_input_source_menu();
	int choice = get_menu_choice(1, 3);
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
	display_action_menu();
	int choice = get_menu_choice(1, 3);
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
	display_test_grid_menu();
	int choice = get_menu_choice(1, 4);
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
		cout << "\nTest grid is loaded." << endl;
		display_grid(grid);
	}
}

/*
	Отримує дані від користувача: розміри сітки, підказки.
	Ініціалізує сітку, додає підказки, встановлює прапорець grid_loaded у true 
	та виводить сітку.
*/
void UI::create_grid_from_console() {
	pair<int, int> dimensions = enter_grid_dimensions();
	grid.resize_grid(dimensions.first, dimensions.second);

	vector<Hint> hints = enter_hints(dimensions.first, dimensions.second);
	for (const Hint& h : hints) {
		grid.add_hint(h);
	}
	grid_loaded = true;
	cout << "\nThe grid was successfully created." << endl;
	display_grid(grid);
}

/*
	Отримує назву файлу, завантажує сітку з цього файлу.
	Якщо завантаження успішне, встановлює прапорець grid_loaded в true та виводить сітку.
*/
void UI::create_grid_from_file() {
	string filename = enter_filename();
	cout << "name (" << filename << ")" << endl;
	if (grid.load_from_file(filename)) {
		cout << "\nThe grid was successfully created from file <" << filename << ">." << endl;
		grid_loaded = true;
		display_grid(grid);
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

	cout << "\n_Solve grid yourself_" << endl;
	display_grid(grid);

	bool continue_solving = true;

	while (continue_solving) {
		cout << "\n_Options:_" << endl;
		cout << "1. Show the solution." << endl;
		cout << "2. Return to the action menu." << endl;
		int choice = get_menu_choice(1, 2);

		switch (choice) {
			case 1: {
				cout << "\nTrying to show the solution.." << endl;
				Grid solved_grid = this->grid;

				bool solved = false;
				this->solver.solve(solved_grid, solved);

				if (solved) {
					display_grid(solved_grid);
				} else {
					cout << "Can't find solution for the grid." << endl;
					cout << "Grid does not have a solution or the solver is not complete." << endl;
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

	cout << "\n_Solve grid programmatically_" << endl;
	cout << "Starting solver... Please wait..." << endl;

	Grid solved_grid = this->grid;
	bool solved = false;
	this->solver.solve(solved_grid, solved);

	if (solved) {
		cout << "\nGrid is solved." << endl;
		display_grid(solved_grid);

		cout << "\nApply this result to the current grid? (y/n): ";
		char response;
		while (true) {
			if (!(cin >> response)) {
				cerr << "Input error. Try again.\n";
				clear_input_buffer();
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
		cout << "Could not find a solution." << endl;
	}
}

/*Відображає меню для вибору способу внесення даних (користувачем/програмно).*/
void UI::display_main_menu(){
	cout << "\n_Menu_" << endl;
	cout << "1. Enter data by user." << endl;
	cout << "2. Enter data programmically." << endl;
	cout << "3. Quit." << endl;
}

/*Відображає меню для вибору способу внесення даних (з консолі/з файлу).*/
void UI::display_input_source_menu(){
	cout << "\n_Choose input source_" << endl;
	cout << "1. Enter data from console." << endl;
	cout << "2. Enter data from file." << endl;
	cout << "3. Return." << endl;
}

/*Відображає меню для вибору способу розв'язання.*/
void UI::display_action_menu(){
	cout << "\n_Choose an action_" << endl;
	cout << "1. Solve the grid yourself." << endl;
	cout << "2. Solve the grid programmically" << endl;
	cout << "3. Return to the main menu." << endl;
}

/*Відображає меню для вибору варіантів тестових завдань.*/
void UI::display_test_grid_menu(){
	cout << "\n_Choose test grid" << endl;
	cout << "1. grid 10x10." << endl;
	cout << "2. grid 12x8." << endl;
	cout << "3. grid 16x11." << endl;
	cout << "4. Return" << endl;
}

/*Очищає стан потоку вводу та видаляє непотрібні символи до кінця рядка.*/
void UI::clear_input_buffer(){
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
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

/*
	Запитує в користувача вибір в певних межах. 
	Виводить помилку при некоректному введені і повторює введення.
*/
int UI::get_menu_choice(int min_choice, int max_choice){
	int choice = 0;
	while(true){
		cout << "\nEnter your choice " << min_choice << "-" << max_choice << ": ";
		if(!(cin >> choice) || choice < min_choice || choice > max_choice){
			cerr << "Invalid choice. Please enter a number between " << min_choice 
				<< " and " << max_choice << "." << endl;
			clear_input_buffer();
		}else{
			return choice;
		}
	}
}

/*
	Запитує у користувача розмір сітки.
	Перевіряє, чи введені числа коректні, якщо ні - повторює ввід.
*/
pair<int, int> UI::enter_grid_dimensions(){
	int r, c;
	while(true){
		cout << "\nEnter grid size (rows colums): ";
		if(!(cin >> r >> c) || r < 1 || c < 1){
			cerr << "Invalid input. Enter two positive numbers for rows and columns." << endl;
			clear_input_buffer();
		}else {
			clear_input_buffer();
			return {r, c};
		}
	}
}

/*
	Запитує у користувача підказки для сітки.
	Для кожної підказки перевіряє коретність даних.
	Якщо дані не є коректними - виводить помилку та повторює введення.
	При успішному введені - додає підказку до hints.
	
*/
vector <Hint> UI::enter_hints(int grid_rows, int grid_cols){
	vector<Hint> hints;
	int hint_count;
	
	while(true){
		cout << "\nEnter number of hints(0 or more): ";
		if(!(cin >> hint_count) || hint_count < 0){
			cerr << "Invalid number of hints. Enter a positive number." << endl;
			clear_input_buffer();
		}else{
			clear_input_buffer();
			break;
		}
	}
	
	for(int i = 0; i < hint_count; i++){
		int r_hint, c_hint, hint_num;
		char dir;
		bool input_is_valid = false;
		
		cout << "Hint №" << (i+1) << ": ";
		
		while(!input_is_valid){		
			if(!(cin >> r_hint >> c_hint >> hint_num >> dir)){
				cerr << "Invalid input for hint №" << (i+1) << "." << endl;
				continue;
			}
			
			if(r_hint < 0 || r_hint >= grid_rows ||c_hint < 0 || c_hint >= grid_cols){
				cerr << "Hint №" << (i+1) << " coordinates (" << r_hint << "," << c_hint 
					<< ") are out of grid range" << endl;
				continue;
			}
			
			if(hint_num < 0){
				cerr << "Hint №" << (i+1) << " must be positive." << endl;

				continue;
			}
			dir = toupper(static_cast<unsigned char>(dir));
			int limit = 0;
			switch (dir) {
				case 'U': limit = r_hint; break;
				case 'D': limit = grid_rows - r_hint - 1; break;
				case 'L': limit = c_hint; break;
				case 'R': limit = grid_cols - c_hint - 1; break;
				default:
					cerr << "Invalid direction.\n";
					clear_input_buffer();
					continue;
			}

			if (hint_num > limit) {
				cerr << "Hint №" << (i+1) << " goes beyond the grid boundaries." << endl;
				continue;
			}
			
			input_is_valid = true;
			clear_input_buffer();
		}
		hints.emplace_back(r_hint, c_hint, hint_num, dir);
		cout << "\nAdded hint №" << (i + 1) << " (" << r_hint << "," << c_hint 
             << ", " << hint_num << ", " << dir << ")." << endl;
	}
	return hints;
}

/*Зчитує у користувача та повертає назву файлу.*/
string UI::enter_filename(){
	string filename;
	cout << "Enter filename: ";
	cin >> filename;
	return filename;
}

/*
	Залежно від стану клітинки у сітці, виводить її з відповідним символом
	і відображає межі сітки разом з номером клітинки.
*/
void UI::display_grid(const Grid& grid){
	int rows = grid.get_rows();
	int cols = grid.get_cols();
	
	if(rows == 0 || cols == 0){
		cout << "Grid is empty or not initialized." << endl;
		return;
	}

	cout << "      ";
	for (int i = 0; i < cols; ++i) cout << setw(2) <<  i << "  ";
	cout << "\n     ";
	for (int i = 0; i < cols; ++i) cout << "----";
	cout << endl;

	for (int i = 0; i < rows; i++) {
		cout << setw(3) <<  i << " " << "| ";
		for (int j = 0; j < cols; j++) {
			CellState cell = grid.get_cell(i, j);
			switch (cell) {
                case CellState::EMPTY: cout << " . "; break;
                case CellState::FILLED: cout << " # "; break;
                case CellState::LINE: cout << " + "; break;
                case CellState::HINT: {
					const Hint* h_ptr = grid.get_hint_at(i, j);
						if (h_ptr) {
							cout << setw(2) << h_ptr->num;
							switch (h_ptr -> direction) {
								case 'R': cout << "→"; break;
								case 'L': cout << "←"; break;
								case 'U': cout << "↑"; break;
								case 'D': cout << "↓"; break;
								default: cout << "?"; break;
							}
						}else{
							cout << " H?";
						}
					break;
				}
				default: cout << " ? "; break;
			}
			cout << " ";
		}
		cout << "|" << endl;
	}
	cout << "     ";
	for (int i = 0; i < cols; i++) cout << "----";
	cout << endl;
	
	if(grid.get_is_solved()){
		cout << "Status: Solved." << endl;
	}else{
		cout << "Status: Not solved." << endl;
	}
}