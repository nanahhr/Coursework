#ifndef INPUT_H
#define INPUT_H

#include "grid.h"
#include <iostream> 
#include <vector>
#include <limits>
#include <utility>
using namespace std;

class Input{
public:
	Input();
	
	int get_menu_choice(int min_choice, int max_choice);
	pair<int, int> enter_grid_dimensions();
	vector <Hint> enter_hints(int grid_rows, int grid_cols);
	string enter_filename();
	
	void clear_input_buffer();
};
#endif