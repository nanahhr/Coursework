/* ----------------------------------------------------------------<Header>-
 Name: input.cpp
 Group: TV-42
 Student: Hryhorenko A.A.
 Written: 2025-05-18
 Revised: 
 Description: Handles input from user.

 ------------------------------------------------------------------</Header>-*/

#include "input.h"

/*Конструктор Input.*/
Input::Input(){}

/*
	Запитує в користувача вибір в певних межах. 
	Виводить помилку при некоректному введені і повторює введення.
*/
int Input::get_menu_choice(int min_choice, int max_choice){
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
pair<int, int> Input::enter_grid_dimensions(){
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
vector <Hint> Input::enter_hints(int grid_rows, int grid_cols){
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
string Input::enter_filename(){
	string filename;
	cout << "Enter filename: ";
	cin >> filename;
	return filename;
}

void Input::clear_input_buffer() {
    cin.clear();
    cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
}