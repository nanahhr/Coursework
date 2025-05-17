#ifndef UI_H
#define UI_H

#include "grid.h"

#include <iostream>
#include <utility>
#include <vector> 
#include <string>
#include <limits>
#include <iomanip>

class UI{
public:
	UI();
	void run_program();
	
private:	
	Grid grid;
	//Solver solver;
	bool grid_loaded;
	
	void display_main_menu();
	void display_input_source_menu();
	void display_action_menu();
	void display_test_grid_menu();
	void clear_input_buffer();
	
	int get_menu_choice(int min_choice, int max_choice);
	pair<int, int> enter_grid_dimensions();
	vector <Hint> enter_hints(int grid_rows, int grid_cols);
	string enter_filename();
	
	bool main_menu();
	void input_source_menu();
	bool action_menu();
	void test_grid_menu();
	
	void create_grid_from_console();
	void create_grid_from_file();
	
	void solve_manually();
	void solve_grid_programmatically();
	
	void load_test_grid_1();
	void load_test_grid_2();
	void load_test_grid_3();
	
	void display_grid(const Grid& grid);
};

#endif