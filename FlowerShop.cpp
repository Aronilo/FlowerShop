#include <iostream>
#include <locale>
#include <Windows.h>

#include "Menu.h"
#include "FileEditor.h"
#include "CommandManager.h"

using namespace std;
//запуск магазина
int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "RUS");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	//создаем объект методами которого будем пользоваться для записи и чтения из файла
	FileEditor file_editor;
	if (argc == 4) {
		file_editor = FileEditor(argv[1], argv[2], argv[3]);
	}
	//для обработки команд из меню
	CommandManager cm(&file_editor);
	//объект класса для реализации меню
	Menu menu(&cm);
	
	//основной цикл программы
	while (menu.is_working) {
		menu.MainMenu();
	}
}