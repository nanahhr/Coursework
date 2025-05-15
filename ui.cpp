#include "ui.h"

UI::UI(){}

void UI::display_main_menu(){
	cout << "\n_Menu_" << endl;
	cout << "1. Enter data by user." << endl;
	cout << "2. Enter data programmically." << endl;
	cout << "3. Quit." << endl;
}

void UI::display_input_source_menu(){
	cout << "\n_Choose input source_" << endl;
	cout << "1. Enter data from console." << endl;
	cout << "2. Enter data from file." << endl;
	cout << "3. Return." << endl;
}

void UI::display_action_menu(){
	cout << "\n_Choose an action_" << endl;
	cout << "1. Solve the puzzle yourself." << endl;
	cout << "2. Solve the puzzle programmically" << endl;
	cout << "3. Show the current state." << endl;
	cout << "Return to the main menu." << endl;
}

void UI::clear_input_buffer(){
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max, '\n');
}

int UI::get_menu_choice(int min_choice, int max_choice){
	int choice = 0;
	
	while(true){
		cout << "Enter your choice " << min_choice << "-" << max_choice << ": ";
		if(!(cin >> choice) || choice < min_choice || choice > max_choice){
			cerr << "Invalid choice. Please enter a number between " << min_choice 
				<< " and " << max_choice << "." << endl;
			clear_input_buffer();
		}else{
			clear_input_buffer();
			return choice;
		}
	}
}
pair<int, int> UI::enter_grid_dimensions(){
	int r, c;
	cout << "Enter grid size (rows colums): ";
	if(!(cin >> r >> c) || r < 1 || c < 1){
		cerr << "Invalid input. Enter two positive numbers for rows and columns." << endl;
		clear_input_buffer();
	}else {
		clear_input_buffer();
		return {r, c};
	}
}
vector <Hint> UI::enter_hints(int grid_rows, int grid_cols){
	vector<Hint> hints;
	int n;
	while(true){
		cout << "Enter number of hints(0 or more): ";
		if(!(cin >> n) || n < 0){
			cerr << "Invalid number of hints. Enter a positive number." << endl;
			clear_input_buffer();
			continue;
		}
		clear_input_buffer();
	}
	int r, c, num;
	char dir_input;
	
	for(int i = 0; i < n; i++){
		cout << "Hint №" << (i+1) << ":" << endl;
		
		cout << "Enter (row coloum (number of cells) direction(U/D/L/R):\n" << endl;
		if(!(cin >> r >> c >> num >> dir_input) || !is_valid(r , c) || num < 0){
			cerr << "Invalid input. Try again." << endl;
			clear_input_buffer();
			continue;
		}
		clear_input_buffer();
	}
	return hints;
}

void string UI::enter_filename(){
	string filename;
	cout << "Enter filename: ";
	getline(cin, filename);
	return filename;
}

void UI::display_grid(const Grid& grid){
	int rows = grid.get_rows();
	int cols = grid.get_cols();
	
	if(rows == 0 || cols == 0){
		cout << "Grid is empty or not initialized." << endl;
		return;
	}
	
	cout << "      ";
	for (int i = 0; i < col; ++i) cout << setw(2) <<  i << "  ";
	cout << "\n     ";
	for (int i = 0; i < col; ++i) cout << "----";
	cout << endl;

	for (int i = 0; i < rows; i++) {
		cout << setw(3) <<  i << " " << "| ";
		CellState cell = grid.get_cell(i, j);
		for (int j = 0; j < cols; j++) {
			switch (cell) {
                case CellState::EMPTY:  std::cout << " . "; break;
                case CellState::FILLED: std::cout << " # "; break;
                case CellState::LINE:   std::cout << " + "; break;
                case CellState::HINT: {
					Hint* h_ptr = grid.get_hint_at(i, j);
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
	for (int i = 0; i < col; i++) cout << "----";
	cout << endl;
	
	if(grid.get_is_solved()){
		cout << "Status: Solved." << endl;
	}else{
		cout << "Status: Not solved." << endl;
	}
}