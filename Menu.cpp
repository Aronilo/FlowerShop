#include "Menu.h"
#include "CommandManager.h"

//названия пунктов меню
vector<string> Menu::main_cmds = {
	"Добавить набор цветов",
	"Поиск и продажа цветов",
	"Работники",
	"Следующий день",
	"Пополнить баланс",
	"Выдать заработную плату",
	"Информация",
	"Завершение работы"
};
//контейнеры для каждого пункта меню
void Menu::MainMenu() {
	int cmd = Select(main_cmds);

	switch (cmd) {
	case 1:
		AddMenu();
		break;
	case 2:
		SellMenu();
		break;
	case 3:
		WorkerMenu();
		break;
	case 4:
		p_cm->p_fe->NextDay();
		break;
	case 5:
		p_cm->ReplenishBalance();
		break;
	case 6:
		p_cm->IssueSalary();
		break;
	case 7:
		InfoMenu();
		break;
	case 8:
		Terminate();
		break;
	}
}
//первый пункт меню
vector<string> Menu::add_cmds = {
	"Ввести данные о цветах с клавиатуры",
	"Ввести данные о цветах с файла",
	"Сохранить набор цветов в файл",
	"Назад",
};
//получение номера команды
void Menu::AddMenu() {
	int cmd = Select(add_cmds);
	//реакции на номер
	switch (cmd) {
	case 1:
		p_cm->AddFlowerKeyBoard();
		break;
	case 2:
		p_cm->p_fe->AddFlowerFile();
		break;
	case 3:
		p_cm->p_fe->SaveFlowersFile();
		break;
	case 4:
		break;
	}
}
//2 пункт меню
vector<string> Menu::sell_cmds = {
	"Поиск по имени",
	"Поиск по цвету",
	"Поиск по цене",
	"Назад",
};
//реализация пункта меню поиска пользователям цветов и предложения их продажи
void Menu::SellMenu() {
	int cmd = Select(sell_cmds);

	switch (cmd) {
	case 1:
		p_cm->SellByName();
		break;
	case 2:
		p_cm->SellByColor();
		break;
	case 3:
		p_cm->SellByCost();
		break;
	case 4:
		break;
	}
}
//3 пункт
vector<string> Menu::worker_cmds = {
	"Добавить работника",
	"Уволить работника",
	"Назад"
};
//реализация пункта меню взаимодествия с работниками
void Menu::WorkerMenu() {
	int cmd = Select(worker_cmds);

	switch (cmd) {
	case 1:
		p_cm->AddWorker();
		break;
	case 2:
		p_cm->DeleteWorker();
		break;
	case 3:
		break;
	}
}
//4 пункт
vector<string> Menu::info_cmds = {
	"Цветы на складе",
	"Список работников",
	"Баланс",
	"Назад"
};
//реализация пункта получения информации по текущему состоянию дел
void Menu::InfoMenu() {
	int cmd = Select(info_cmds);

	switch (cmd) {
	case 1:
		p_cm->p_fe->PrintFlowers();
		break;
	case 2:
		p_cm->p_fe->PrintWorkers();
		break;
	case 3:
		p_cm->GetBalance();
		break;
	case 4:
		break;
	}
}
//вывод меню и получение ответа
int Menu::Select(const vector<string>& cmds) {
	cout << endl;
	for (size_t i = 0; i < cmds.size(); i++) {
		cout << i + 1 << ". " << cmds[i] << endl;
	}
	cout << endl;
	return get_answer(1, cmds.size());
}
//завершение работы
void Menu::Terminate() {
	is_working = false;
}









