#pragma once

#include <iostream>

using namespace std;
//����� ������� �� ����� ������
class Flower {
public:
	string name;    // 20 * char
	string color;   // 20 * char
	int cost;       // 1 * int
	int prime_cost; // 1 * int
	int shelf_life; // 1 * int
	int count;      // 1 * int
	//�����������
	Flower() : name("Undefined"), color("Undefined"),
	cost(0), prime_cost(0), shelf_life(0), count(0) {};
	//������ � ����
	void SaveToFile(fstream& file) const;
	//������ ���������� � �����
	bool GetFile(fstream& file);
	//������ ���������� � ����������
	bool GetKeyBoard();
	//����� �� �����
	void Print() const;
};

