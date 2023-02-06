#pragma once

#include <vector>
#include <iostream>

#include "CommandManager.h"
#include "FileEditor.h"

using namespace std;

class Menu {
public:
	bool is_working;
	//реализация выполнения команд из меню
	CommandManager* p_cm;
	
	static vector<string> main_cmds;

	static vector<string> add_cmds;

	static vector<string> sell_cmds;

	static vector<string> worker_cmds;

	static vector<string> info_cmds;
	//конструктор
	Menu(CommandManager* cm) : 
		is_working(true), p_cm(cm) {}
	//вывод меню и запрос команды
	int Select(const vector<string>& cmds);
	
	void MainMenu();
	
	void AddMenu();

	void SellMenu();
	
	void WorkerMenu();

	void InfoMenu();

	void Terminate();

};

