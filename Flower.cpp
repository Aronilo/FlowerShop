#include <fstream>
#include <iomanip>

#include "Flower.h"
#include "CommandManager.h"

using namespace std;
//запись в файл о наборе цветов
void Flower::SaveToFile(fstream& file) const {
	
	if (!file.is_open()) {
		return;
	}
	//записываем информацию о цветке в бинарном виде в поток файл
	file.write(name.c_str(), 20 * sizeof(char));
	file.write(color.c_str(), 20 * sizeof(char));
	file.write((char*)&cost, sizeof(int));
	file.write((char*)&prime_cost, sizeof(int));
	file.write((char*)&shelf_life, sizeof(int));
	file.write((char*)&count, sizeof(int));
}
//чтение из файла
bool Flower::GetFile(fstream& in) {

	char buffer[21];
	//считываем имя
	in.read(buffer, 20 * sizeof(char));
	name = string(buffer);
	//считывае цвет
	in.read(buffer, 20 * sizeof(char));
	color = string(buffer);

	in.read((char*)&cost, sizeof(int));
	in.read((char*)&prime_cost, sizeof(int));
	in.read((char*)&shelf_life, sizeof(int));
	in.read((char*)&count, sizeof(int));
	
	if (in.fail()) {
		in.close();
		return false;
	}
	return true;
}
//ввод параметров с клавиатуры
bool Flower::GetKeyBoard() {

	cout << "Название:\n>> ";
	cin >> name;

	cout << "Цвет:\n>> ";
	cin >> color;

	cout << "Цена:\n";
	cost = get_answer(1, 10000);

	cout << "Себестоимость:\n";
	prime_cost = get_answer(1, 10000);

	cout << "Срок реализации:\n";
	shelf_life = get_answer(1, 100);

	cout << "Количество:\n";
	count = get_answer(1, 1000);

	return true;
}
//вывод информации о цветке
void Flower::Print() const {
	cout << setw(20) << name << setw(20) << color << setw(8) << cost <<
		setw(17) << prime_cost << setw(17) << shelf_life << setw(13) << count << endl;
}