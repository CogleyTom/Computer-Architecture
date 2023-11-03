#ifndef REGISTERS_H
#define REGISTERS_H

#include <string>
using std::string;
#include <vector>
using std::vector;

class registerCPU {

public:

	registerCPU(int regName, int regValue = 0, int byteSize = 32, int index = 0);

	int getRegisterName();
	int getRegValue();
	int getRegByteSize();
	int getRegIndex();

	void setRegValue(int value);
	void setRegByteSize(int byteSize);

private:
	int _regName;
	int _regValue;
	int _regByteSize;
	int _regIndex;
};

#endif
