#ifndef UI_H
#define UI_H

#include "grid.h"
#include "solver.h"
#include "display.h"
#include "input.h"

#include <iostream>
#include <vector> 
#include <string>

#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_RED     "\033[1;31m"

class UI{
public:
	UI();
	void run_program();
	
private:	
	Grid grid;
	Solver solver;
	bool grid_loaded;
	
    Display display;
    Input input;
	
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

};

#endif