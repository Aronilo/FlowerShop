#pragma once

#include "FileEditor.h"

int get_answer(int min, int max);

class CommandManager {
public:
	FileEditor* p_fe;
	//инициализация полей класса
	CommandManager(FileEditor* p) : p_fe(p) {}

	void AddFlowerKeyBoard();

	void SellByName();

	void SellByColor();

	void SellByCost();

	void AddWorker();

	void DeleteWorker();

	void GetBalance();

	void IssueSalary();

	void ReplenishBalance();
};

