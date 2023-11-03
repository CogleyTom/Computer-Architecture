#include "Stack.h"

#include <vector>
using std::vector;
#include <stack>
using std::stack;
#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
using std::stoi;

stackArch::stackArch() {
	stackSizeBytes = 100;
	stackPointer = 0;

	for (int i = 0; i < 20; i++) {
		ARMstack.push(0);
	}
}

stackArch::stackArch(int sizeBytes) {
	stackSizeBytes = sizeBytes;
	stackPointer = 0;

	for (int i = 0; i < (stackSizeBytes / 100) || i > 1000; i++) {
		ARMstack.push(i);
	}
}

void stackArch::displayStack() {
	stack<int> temp;
	size_t stackAllocated = ARMstack.size();
	int tempSize = stackSizeBytes;

	while (tempSize > stackAllocated) {
		cout << "0  ";
		tempSize--;
	}

	while (temp.empty() == false) {
		int t = temp.top();
		cout << t << "  ";
		temp.pop();
	}
}

stack<int> stackArch::getStack() {
	return ARMstack;
}

int stackArch::getStackSize() {
	return stackSizeBytes;
}

int stackArch::getDataAtAddress(string stackAddress) { //Incomplete - cant index into a stack so will have see how the controls break out.
	int address = stoi(stackAddress);

	return ARMstack.top();
}