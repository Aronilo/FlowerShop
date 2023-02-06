#pragma once

#include <iostream>

using namespace std;
//набор цветков из одной партии
class Flower {
public:
	string name;    // 20 * char
	string color;   // 20 * char
	int cost;       // 1 * int
	int prime_cost; // 1 * int
	int shelf_life; // 1 * int
	int count;      // 1 * int
	//конструктор
	Flower() : name("Undefined"), color("Undefined"),
	cost(0), prime_cost(0), shelf_life(0), count(0) {};
	//запись в файл
	void SaveToFile(fstream& file) const;
	//чтение информации с файла
	bool GetFile(fstream& file);
	//чтение информации с клавиатуры
	bool GetKeyBoard();
	//вывод на экран
	void Print() const;
};

