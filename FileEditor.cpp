#pragma once

#include <fstream>
#include <iomanip>
#include <vector>

#include "Flower.h"
#include "FileEditor.h"
#include "CommandManager.h"
#include "Worker.h"

#define FLOWER_SIZE 57
#define WORKER_SIZE 25

using namespace std;
//конструктор в котором считываем баланс с файла
FileEditor::FileEditor(const string& ff, const string& wf, const string& inff) :
	flower_filename(ff), worker_filename(wf),
	info_filename(inff), balance(0) {

	ifstream in(inff);

	if (in.is_open()) {
		in.read((char*)&balance, sizeof(int));
		in.close();
	}
}
//проверка того что имя цветка fl совпадает с именем адрес которого arg
bool FileEditor::IsEqualName(const Flower& fl, void* arg) {
	string* ptr_str = (string*)arg;
	return fl.name == *ptr_str;
}

bool FileEditor::IsEqualColor(const Flower& fl, void* arg) {
	string* ptr_str = (string*)arg;
	return fl.color == *ptr_str;
}

bool FileEditor::HasLessCost(const Flower& fl, void* arg) {
	int* ptr_cost = (int*)arg;
	return fl.cost <= *ptr_cost;
}

bool FileEditor::HasGrossCost(const Flower& fl, void* arg) {
	int* ptr_cost = (int*)arg;
	return fl.cost >= *ptr_cost;
}
//добавление цветов в файл
bool FileEditor::AddFlowers(const Flower& fl) {
	//хватит денег или нет
	if (fl.count * fl.prime_cost > balance) {
		cout << "Для закупки цветов \"" << fl.name << "\" требуется " << fl.count * fl.prime_cost <<
			" денежных единиц." << endl << "Текущий баланс: " << balance << ". Закупка невозможна." << endl;
		return false;
	}

	fstream file(flower_filename, ios::binary | ios::in | ios::out);
	//если нет файла, то создается файл на запись
	if (!file.is_open()) {
		file.open(flower_filename, ios::binary | ios::out);
		file.close();
		file.open(flower_filename, ios::binary | ios::in | ios::out);
	}
	//перемещение в конец файла
	file.seekg(0, ios::end);
	
	int size = file.tellg();
	//перемещение в начао
	file.seekg(0, ios::beg);

	char is_deleted;
	int pos = 0;
	//ищем  место для записи информации о цветке
	while (pos < size) {
		
		file.read((char*)&is_deleted, 1);

		if (is_deleted) {
			file.seekg(-1, ios::cur);
			break;
		}

		file.seekg(FLOWER_SIZE - 1, ios::cur);

		pos += FLOWER_SIZE;
	}

	char x = 0;

	file.write(&x, 1);

	fl.SaveToFile(file);

	file.close();

	cout << "Цветы \"" << fl.name << "\" в количестве " << fl.count <<
		" штук были успешно закуплены." << endl;

	balance -= fl.count * fl.prime_cost;
	//обновляется банас в файле
	UpdateBalance();

	return true;
}
//метод сохранение данных о наборе цветов в файл
void FileEditor::SaveFlowersFile() {
	cout << "\nСохранение данных о наборе цветов в файл..." << endl;

	cout << "Имя файла с данными о цветах: ";

	string filename;
	cin >> filename;

	cout << "Количество наборов цветов для записи:" << endl;

	int n_flowers = get_answer(1, 20);

	Flower flower;

	fstream file(filename, ios::binary | ios::out);
	for (int i = 0; i < n_flowers; i++) {
		cout << "Получение данных о наборе #" << i + 1 << "." << endl;
		flower.GetKeyBoard();
		flower.SaveToFile(file);
	}

	cout << "\nИнформация о наборе цветов успешно сохранена!" << endl;
}
//метод получение данны о наборе цветов с файла
void FileEditor::AddFlowerFile() {

	cout << "\nПолучение данных о наборе цветов с файла..." << endl;

	cout << "Имя файла с данными о цветах: ";

	string filename;
	cin >> filename;

	fstream file(filename, ios::binary | ios::in);

	if (!file.is_open()) {
		cout << "Файл не открыт!" << endl;
		return;
	}

	file.seekg(0, ios::end);
	//получение позиции на которой мы находимся
	int size = file.tellg();

	file.seekg(0, ios::beg);
	//объъявление переменной flower
	Flower flower;

	bool flag = false;

	int pos = 0;

	while (pos < size) {

		pos += FLOWER_SIZE - 1;

		flower.GetFile(file);
		AddFlowers(flower);
	}

	file.close();

	cout << "\nИнформация о цветах успешно считана!" << endl;
}
//метод удаления цветов из файла
void FileEditor::DeleteFlowers(const vector<Flower>& flowers) {
	fstream file(flower_filename, ios::binary | ios::in | ios::out);

	if (!file.is_open()) {
		return;
	}

	file.seekg(0, ios::end);

	int size = file.tellg();

	file.seekg(0, ios::beg);

	char is_deleted;
	int pos = 0;

	char x = 1;

	Flower fl;

	while (pos < size) {

		pos += FLOWER_SIZE;
		//считывание первого байта очередной записи о цветке
		//0 запись корректна
		//1 удалена
		file.read((char*)&is_deleted, 1);

		if (is_deleted) {
			file.seekg(FLOWER_SIZE - 1, ios::cur);
			continue;
		}
		//считываем информацию о цветке
		fl.GetFile(file);

		if (file.fail()) {
			file.close();
			return;
		}
		//переборка цветов
		for (size_t i = 0; i < flowers.size(); i++) {

			if (flowers[i].count == 0)
				continue;

			if (fl.name == flowers[i].name) {
				if (fl.count < flowers[i].count) {
					cout << "Цветов \"" << flowers[i].name << "\" нет в наличии в заданном количестве." << endl;
					
					file.seekp(-FLOWER_SIZE, ios::cur);
					//запись удалена или нет
					//тут не удалена
					file.write((char*)&x, sizeof(char));
					file.seekg(-1, ios::cur);
				} else {
					cout << "Цветы \"" << flowers[i].name << "\" в количестве " << flowers[i].count <<
						" штук были успешно проданы." << endl;
					cout << "Выручка: " << flowers[i].count * flowers[i].cost << endl;
					balance += flowers[i].count * flowers[i].cost;
					UpdateBalance();
					file.seekp(sizeof(int) * (-1), ios::cur);
					//запись разницы между было и купили
					int difference = fl.count - flowers[i].count;

					file.write((char*)&difference, sizeof(int));
					file.seekg(0, ios::cur);
				}
			}
		}

		
	}
	file.close();
}
//метод поиска цветов для которых функция f возвращает true
vector<Flower> FileEditor::FindFlowers(bool(*f)(const Flower&, void*), void* arg) {
	vector<Flower> flowers;

	fstream in(flower_filename, ios::binary | ios::in);

	if (!in.is_open()) {
		return flowers;
	}

	in.seekg(0, ios::end);

	int size = in.tellg();

	in.seekg(0, ios::beg);

	int pos = 0;
	char is_deleted;

	Flower fl;

	while (pos < size) {

		pos += FLOWER_SIZE;
		//удалена или нет
		in.read((char*)&is_deleted, 1);

		if (is_deleted) {
			//если запись удалена, то мы ее пропускаем
			in.seekg(FLOWER_SIZE - 1, ios::cur);
			continue;
		}
		fl.GetFile(in);

		if (in.fail()) {
			in.close();
			return flowers;
		}
		//если цветок соответствует параметрам, тогда сохраняем
		if (f(fl, arg)) {
			flowers.push_back(fl);
		}
	}

	return flowers;
}
//метод добавления сотрудников
void FileEditor::AddWorker(const Worker& worker) {
	fstream file(worker_filename, ios::binary | ios::in | ios::out);

	if (!file.is_open()) {
		//создание файла на запись
		file.open(worker_filename, ios::binary | ios::out);
		file.close();
		//открытие на чтение и запись
		file.open(worker_filename, ios::binary | ios::in | ios::out);
	}

	file.seekg(0, ios::end);

	int size = file.tellg();

	file.seekg(0, ios::beg);

	char is_deleted;
	int pos = 0;

	while (pos < size) {

		file.read((char*)&is_deleted, 1);
		//поиск удаленных записей
		if (is_deleted) {
			//добавление информации, вместо удаленной
			file.seekg(-1, ios::cur);
			break;
		}

		file.seekg(WORKER_SIZE - 1, ios::cur);

		pos += WORKER_SIZE;
	}

	char x = 0;

	file.write(&x, 1);

	file.write(worker.name.c_str(), 20 * sizeof(char));
	file.write((char*)&worker.salary, sizeof(int));
	
	file.close();
}
//метод удаления раб из файла
bool FileEditor::DeleteWorker(const string& name) {	
	fstream file(worker_filename, ios::binary | ios::in | ios::out);

	if (!file.is_open()) {
		return false;
	}

	file.seekg(0, ios::end);

	int size = file.tellg();

	file.seekg(0, ios::beg);

	char is_deleted;
	int pos = 0;

	char buf[21];
	int s;
	char x = 1;

	while (pos < size) {

		file.read((char*)&is_deleted, 1);
		if (is_deleted) {
			file.seekg(WORKER_SIZE - 1, ios::cur);
		} else {
			file.read(buf, 20 * sizeof(char));
			file.read((char*)&s, sizeof(int));

			if (!strcmp(buf, name.c_str())) {
				file.seekp(-WORKER_SIZE, ios::cur);
				file.write((char*)&x, 1);
				file.close();
				return true;
			}
		}

		pos += WORKER_SIZE;
	}

	file.close();

	return false;
}
//метод выводящий список сотрудников
void FileEditor::PrintWorkers() {
	ifstream file(worker_filename, ios::binary);

	if (!file.is_open()) {
		cout << "Файл не открыт!" << endl;
		return;
	}

	file.seekg(0, ios::end);

	int size = file.tellg();

	file.seekg(0, ios::beg);

	Worker worker;

	cout << "Список работников:" << endl;

	bool flag = false;

	char is_deleted;
	int pos = 0;

	char buf[21];

	while (pos < size) {

		pos += WORKER_SIZE;

		file.read((char*)&is_deleted, 1);

		if (is_deleted) {
			file.seekg(WORKER_SIZE  - 1, ios::cur);
			continue;
		}

		file.read(buf, 20 * sizeof(char));

		worker.name = string(buf);
		file.read((char*)&worker.salary, sizeof(int));

		flag = true;
		worker.Print();
	}

	if (!flag) {
		cout << "Работники отсутствуют!" << endl;
	}

	file.close();
}
//метод расчета суммарной зарплаты
int FileEditor::GetTotalSalary() {
	ifstream file(worker_filename, ios::binary);

	if (!file.is_open()) {
		return 0;
	}

	file.seekg(0, ios::end);

	int size = file.tellg();

	file.seekg(0, ios::beg);

	Worker worker;

	char is_deleted;
	int pos = 0;

	char buf[21];

	int total_salary = 0;
	while (pos < size) {

		pos += WORKER_SIZE;

		file.read((char*)&is_deleted, 1);

		if (is_deleted) {
			file.seekg(WORKER_SIZE - 1, ios::cur);
			continue;
		}

		file.read(buf, 20 * sizeof(char));

		worker.name = string(buf);
		file.read((char*)&worker.salary, sizeof(int));

		total_salary += worker.salary;
	}

	file.close();

	return total_salary;
}
//метод след день
void FileEditor::NextDay() {
	fstream file(flower_filename, ios::binary | ios::in | ios::out);

	if (!file.is_open()) {
		return;
	}

	file.seekg(0, ios::end);

	int size = file.tellg();

	file.seekg(0, ios::beg);

	Flower flower;

	char is_deleted;
	int pos = 0;

	char x = 1;

	while (pos < size) {

		pos += FLOWER_SIZE;

		file.read((char*)&is_deleted, 1);

		if (is_deleted) {
			file.seekg(FLOWER_SIZE - 1, ios::cur);
			continue;
		}

		flower.GetFile(file);

		if (flower.shelf_life == 1) {
			cout << "Цветы " << flower.name << " в количестве " << flower.count << " штук испортились." << endl;
			file.seekp(-FLOWER_SIZE, ios::cur);
			file.write((char*)&x, 1);
			file.seekg(FLOWER_SIZE - 1, ios::cur);
		} else {
			file.seekp(sizeof(int) * (-2), ios::cur);
			flower.shelf_life--;
			file.write((char*)&flower.shelf_life, sizeof(int));
			file.seekg(sizeof(int), ios::cur);

			if (flower.shelf_life == 1) {
				cout << "Срок реализации " << flower.name << " в количестве " << flower.count << " штук составляет 1 день." << endl
					<< "Рекомендуется продажа данного товара." << endl;
			}
		}
	}

	file.close();
}
//метод список цветов в наличии
void FileEditor::PrintFlowers() {
	fstream file(flower_filename, ios::binary | ios::in);

	if (!file.is_open()) {
		cout << "Файл не открыт!" << endl;
		return;
	}

	file.seekg(0, ios::end);

	int size = file.tellg();

	file.seekg(0, ios::beg);

	Flower flower;

	cout << "Список цветов в наличии:" << endl;
	cout << setw(20) << "Название" << setw(20) << "Цвет" << setw(8) << "Цена" << 
		"  Закупочная цена  " << "Срок реализации   Количество" << endl;

	bool flag = false;

	char is_deleted;
	int pos = 0;

	while (pos < size) {

		pos += FLOWER_SIZE;

		file.read((char*)&is_deleted, 1);

		if (is_deleted) {
			file.seekg(FLOWER_SIZE - 1, ios::cur);
			continue;
		}

		flower.GetFile(file);

		flag = true;
		flower.Print();
	}

	if (!flag) {
		cout << "Цветов нет в наличии!" << endl;
	}

	file.close();
}
//метод обновления баланса в файле
void FileEditor::UpdateBalance() {
	ofstream out(info_filename);
	out.write((char*)&balance, sizeof(int));
	out.close();
}