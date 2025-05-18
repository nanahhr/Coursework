/* ----------------------------------------------------------------<Header>-
 Name: task13.cpp
 Title: Main program entry point.
 Group: TV-42
 Student: Hryhorenko A.A.
 Written: 2025-05-18
 Revised: 
 Description: This file contains the main function that creates
              the UI program manager object and runs the program.
 ------------------------------------------------------------------</Header>-*/
 

#include <iostream>
#include "ui.h"
using namespace std;

int main(){
	UI program_manager;
	program_manager.run_program();
	return 0;
}