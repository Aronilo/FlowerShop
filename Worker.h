#pragma once

#include <string>

using namespace std;
//рабочий
class Worker {
public:
	Worker(const string& n = "Incognito", int s = 0) : name(n), salary(s) {}

	string name;
	int salary;
	//ввести с клавиатуры
	bool GetKeyboard();
	//вывести на экран
	void Print();
};

