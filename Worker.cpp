#include <iostream>
#include <iomanip>


#include "Worker.h"
#include "CommandManager.h"

using namespace std;
//получение информации о работнике с клавиатуры
bool Worker::GetKeyboard() {
	cout << "Имя:\n>> ";
	cin >> name;

	cout << "Размер заработной платы:\n";
	salary = get_answer(1, 100000);

	return true;
}
//вывод работников
void Worker::Print() {
	cout << setw(20) << name << setw(6) << salary << endl;
}