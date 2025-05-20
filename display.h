#ifndef DISPLAY_H
#define DISPLAY_H

#include "grid.h"
#include <iostream>
#include <string>
#include <iomanip>

#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_RED     "\033[1;31m"
using namespace std;

class Display{
public:
	Display();
	
	void display_main_menu();
	void display_input_source_menu();
	void display_action_menu();
	void display_test_grid_menu();
	void display_grid(const Grid& grid);
};
#endif