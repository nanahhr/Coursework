#ifndef UI_H
#define UI_H

#include "grid.h"
#include <iostream>
#include <utility>
#include <vector> 
#include <string>

class UI: {
public:
	UI();
	void display_main_menu();
	void display_input_source_menu();
	void display_action_menu();
	void clear_input_buffer();
	
	int get_menu_choice(int min_choice, int max_choice);
	pair<int, int> enter_grid_dimensions();
	vector <Hint> enter_hints(int grid_rows, int grid_cols);
	void string enter_filename();
	void display_grid(const Grid& grid);
	
}

#endif