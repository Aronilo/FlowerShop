#pragma once

#include <string>

using namespace std;
//�������
class Worker {
public:
	Worker(const string& n = "Incognito", int s = 0) : name(n), salary(s) {}

	string name;
	int salary;
	//������ � ����������
	bool GetKeyboard();
	//������� �� �����
	void Print();
};

