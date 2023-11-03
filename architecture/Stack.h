#ifndef STACK_H
#define STACK_H

#include <vector>
using std::vector;
#include <stack>
using std::stack;
#include <string>
using std::string;


class stackArch {

public:
	stackArch();
	stackArch(int sizeBytes);

	stack<int> getStack();
	int getStackSize();

	void displayStack();

	int getDataAtAddress(string stackAddress);
	
private:
	int stackPointer;
	int stackSizeBytes;
	stack<int> ARMstack;
};

#endif