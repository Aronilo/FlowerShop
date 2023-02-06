#include <fstream>
#include <iomanip>

#include "Flower.h"
#include "CommandManager.h"

using namespace std;
//������ � ���� � ������ ������
void Flower::SaveToFile(fstream& file) const {
	
	if (!file.is_open()) {
		return;
	}
	//���������� ���������� � ������ � �������� ���� � ����� ����
	file.write(name.c_str(), 20 * sizeof(char));
	file.write(color.c_str(), 20 * sizeof(char));
	file.write((char*)&cost, sizeof(int));
	file.write((char*)&prime_cost, sizeof(int));
	file.write((char*)&shelf_life, sizeof(int));
	file.write((char*)&count, sizeof(int));
}
//������ �� �����
bool Flower::GetFile(fstream& in) {

	char buffer[21];
	//��������� ���
	in.read(buffer, 20 * sizeof(char));
	name = string(buffer);
	//�������� ����
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
//���� ���������� � ����������
bool Flower::GetKeyBoard() {

	cout << "��������:\n>> ";
	cin >> name;

	cout << "����:\n>> ";
	cin >> color;

	cout << "����:\n";
	cost = get_answer(1, 10000);

	cout << "�������������:\n";
	prime_cost = get_answer(1, 10000);

	cout << "���� ����������:\n";
	shelf_life = get_answer(1, 100);

	cout << "����������:\n";
	count = get_answer(1, 1000);

	return true;
}
//����� ���������� � ������
void Flower::Print() const {
	cout << setw(20) << name << setw(20) << color << setw(8) << cost <<
		setw(17) << prime_cost << setw(17) << shelf_life << setw(13) << count << endl;
}