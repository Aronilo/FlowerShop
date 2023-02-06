#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

#include "CommandManager.h"
#include "FileEditor.h"
#include "Worker.h"

using namespace std;

//Получение ответа от пользователя в виде целого числа из заданного диапазона с проверкой ввода
int get_answer(int min, int max) {
	int answer;

	cout << ">> ";
	cin >> answer;
	
	while (cin.fail() || answer < min || answer > max) {
		cout << "Некорректный ввод. Введите число от " << min << " до " << max << endl << ">> ";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> answer;
	}
	return answer;
}
//метод класса(добавление на склад)
void CommandManager::AddFlowerKeyBoard() {

	cout << "\nПолучение данных о наборе цветов с клавиатуры..." << endl;

	Flower flower;
	flower.GetKeyBoard();
	//добавление к другим цветам хранящимся на складе
	p_fe->AddFlowers(flower);
}
//метод отвечающий за покупку цветка по имени
void CommandManager::SellByName() {

	cout << "\nПоиск по названию цветов и их продажа..." << endl << endl;

	cout << "Название:" << endl << "> ";

	string name;
	cin >> name;
	//получене вектора цветов из файла со складом которые соотвествуют функции IsEqualName с переданным в нее аргументом &name
	vector<Flower> result = p_fe->FindFlowers(p_fe->IsEqualName, &name);

	for (size_t i = 0; i < result.size(); i++) {
		cout << setw(20) << "Название" << setw(20) << "Цвет" << setw(8) << "Цена" <<
			"  Закупочная цена  " << "Срок реализации   Количество" << endl;
		result[i].Print();

		cout << "Введите желаемое количество цветов для покупки (0 в противном случае)" << endl;
		//введите ответ от 0 до кол-во цветов на складе
		int answer = get_answer(0, result[i].count);

		result[i].count = answer;
	}

	if (!result.empty()) {
		p_fe->DeleteFlowers(result);
		cout << "Продажа успешно реализована!" << endl;
	}
}
//метод класса реализации поиска и продажи по цвету
void CommandManager::SellByColor() {

	cout << "\nПоиск цветов по цвету и их продажа..." << endl << endl;

	cout << "Цвет:" << endl << "> ";

	string color;
	cin >> color;

	vector<Flower> result = p_fe->FindFlowers(p_fe->IsEqualColor, &color);

	bool is_sell = false;
	
	for (size_t i = 0; i < result.size(); i++) {
		result[i].Print();

		cout << "Введите желаемое количество цветов для покупки (0 в противном случае)" << endl;

		int answer = get_answer(0, result[i].count);
		if (answer > 0) {
			is_sell = true;
		}
		result[i].count = answer;
	}

	if (!result.empty() && is_sell) {
		p_fe->DeleteFlowers(result);
		cout << "Продажа успешно реализована!" << endl;
	}
}
//метод поиска и продажи цветов
void CommandManager::SellByCost() {

	cout << "\nПоиск цветов по цене и их продажа..." << endl << endl;

	cout << "Рассмотреть цветы с ценой\n\t1. Ниже заданной\n\t2.Выше заданной" << endl;

	int answer = get_answer(1, 2);

	cout << "Цена:" << endl;

	int cost = get_answer(1, 10000);

	//поиск в файле цветов с ценой выше или ниже заданной пользователем
	vector<Flower> result;
	if (answer == 1) {
		result = p_fe->FindFlowers(p_fe->HasLessCost, &cost);
	} else {
		result = p_fe->FindFlowers(p_fe->HasGrossCost, &cost);
	}


	for (size_t i = 0; i < result.size(); i++) {
		result[i].Print();

		cout << "Введите желаемое количество цветов для покупки (0 в противном случае)" << endl;

		int answer = get_answer(0, result[i].count);

		result[i].count = answer;
	}

	if (!result.empty()) {
		//удаление цветов из файла
		p_fe->DeleteFlowers(result);
		cout << "Продажа успешно реализована!" << endl;
	}
}
//метод класса добавления инфрмациии о нанятом работнике в файл
void CommandManager::AddWorker() {
	cout << "\nДобавление информации о работнике..." << endl << endl;
	
	Worker worker;

	worker.GetKeyboard();
	p_fe->AddWorker(worker);
}
//метод класс удаления информации об уволенном работнике из файла
void CommandManager::DeleteWorker() {
	cout << "\nУдаление информации о работнике..." << endl << endl;

	string name;
	cout << "Имя:" << endl;
	cin >> name;
	
	if (p_fe->DeleteWorker(name)) {
		cout << "Удаление успешно!" << endl;
	} else {
		cout << "Информация о данном работнике не найдена!" << endl;
	}
}
//метод реализующий вывод информации о текущем балансе средств предприятия
void CommandManager::GetBalance() {
	cout << "Текущий баланс предприятия: " << p_fe->balance << endl;
}
//метод выдачи заработной платы
void CommandManager::IssueSalary() {
	//расчет общей заработной платы
	int salary = p_fe->GetTotalSalary();

	if (salary == 0) {
		cout << "Отсутствуют сведения о необходимости платить заработную плату!" << endl;
	} else if (salary > p_fe->balance) {
		cout << "Недостаточно средств для выдачи заработной платы." << endl;
		cout << "Требуется: " << salary << endl << "Баланс: " << p_fe->balance << endl;
	} else {
		p_fe->balance -= salary;
		//обновление баланса в файле
		p_fe->UpdateBalance();
		cout << "Заработная плата в размере " << salary << " выплачена. Баланс: " << p_fe->balance << endl;
	}
}
//метод Пополнить баланс
void CommandManager::ReplenishBalance() {
	cout << "Пополнение баланса предприятия..." << endl;
	cout << "Какую сумму нужно внести?" << endl;

	int total = get_answer(1, 1000000);

	p_fe->balance += total;
	p_fe->UpdateBalance();
	cout << "Баланс пополнен. Текущий баланс: " << p_fe->balance << endl;
}