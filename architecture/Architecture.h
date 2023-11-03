#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

#include "Registers.h"
#include "Stack.h"

#include <string>
using std::string;
#include <vector>
using std::vector;


class architecture {

public:

	architecture();

	void createReg();
	vector<registerCPU> getRegisterArch();
	void displayRegArch();
	void changeReg(int regName, int newValue);

	void createStack();
	stack<int> getStackArch();
	void displayStackArch();
  string getStackArchStr();
	
	int getStackPointer();
	void pushStack(int pushedVal);
	int popStack();
	void swapStack();

	int getRegisterValue(int regName);
	void setRegisterValue(int regName, int regValue);

	void displayArch();




private:
	string _architectureName;
	vector<registerCPU> _registerArch;
	stackArch _stackArch;
	stack<int> x86Stack;
};

#endif 
