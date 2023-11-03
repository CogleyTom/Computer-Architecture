
#include "instructExec.h"
#include "Registers.h"
#include "Stack.h"
#include "Architecture.h"
#include "TokParse.h"

#include <string>
using std::string;
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;

architecture::architecture() {
	_architectureName = "ARMv7 32bit";
	createReg();
	createStack();
	}

	//store these in a vector
void architecture::createReg() {

	for (int i = 0; i < 17; i++) {
		_registerArch.emplace_back(i);
	}
}


vector<registerCPU> architecture::getRegisterArch() {
	return _registerArch;
}

void architecture::displayRegArch() {
	cout << endl << endl;
	for (int i = 0; i < _registerArch.size(); i++) {
		cout << "Register Name: " << _registerArch[i].getRegisterName() << "      Register Contents: " << _registerArch[i].getRegValue() << endl;
	}
	cout << endl << endl;
}

void architecture::changeReg(int regName, int newValue) {
	int i = 0;
	while (regName != _registerArch[i].getRegisterName()) {
		i++;
	}
	_registerArch[i].setRegValue(newValue);
}
	

void architecture::createStack() {
	_stackArch = stackArch();
	x86Stack = _stackArch.getStack();
}

void architecture::displayStackArch() {

	stack<int> temp = x86Stack;
	int tempStackSize = _stackArch.getStackSize();

	while (temp.size() < tempStackSize) {
		cout << "0  ";
		tempStackSize--;
	}

	while (temp.empty() == false) {
		int t = temp.top();
		cout << t << "  ";
		temp.pop();
	}
}

string architecture::getStackArchStr() {
  string str;
	stack<int> temp = x86Stack;
	int tempStackSize = _stackArch.getStackSize();

	while (temp.size() < tempStackSize) {
    str += "0 ";
		tempStackSize--;
	}

	while (temp.empty() == false) {
		int t = temp.top();
    str += t + " ";
		temp.pop();
	}
  return str;
}

int architecture::getStackPointer() {
	return x86Stack.size() - 1;
}

void architecture::pushStack(int pushedVal) {
	x86Stack.push(pushedVal);
}

int architecture::popStack() {
	int temp = x86Stack.top();
	x86Stack.pop();
	return temp;
}

int architecture::getRegisterValue(int regName) {
	return _registerArch[regName].getRegValue();
}

void architecture::setRegisterValue(int regName, int regValue) {
	_registerArch[regName].setRegValue(regValue);
}

void architecture::displayArch() {
	displayRegArch();
	cout << endl << "DISPLAYING STACK BELOW" << endl;
	displayStackArch();

	cout << endl << endl;
}