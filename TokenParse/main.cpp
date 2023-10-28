#include "Registers.h"
#include "Stack.h"
#include "Architecture.h"
#include "TokParse.h"
#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
#include <sstream>
#include <string>



int main() {

	/*
	architecture* testARM = new architecture();

	testARM->displayArch();

	testARM->changeReg("EBX", testxARM->popStack());
	testARM->pushStack(99);

	testARM->displayArch();
	*/

	std::ifstream infile("ARM_Assembly.txt"); //Open the file that contains the ASM code (can keep the file as .s or ASM as well)
	string line;	//Create a line object that will store the lines for the file

	if (infile.is_open())	{		//If the file opened correctly
		while (getline(infile, line)) {		//Write each line into line
			if (line.size() == 0) {		//If line is empty
				continue;	//Iterate the while loop
			}
			tokParse newTest(line);		//Otherwise create new tokParse object with the string 
			newTest.doToken();	//Tokenize it 
			newTest.jumpCheck();	//Check if its a jump or not and store the bool
			newTest.parseFirstElement(); //Parse the first element of the objs vector
		}
	}
}

//Need to change how the tokParse functions are called and stored. Was just getting the functionality down for now.