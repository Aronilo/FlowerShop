#include <iostream>
#include <iomanip>


#include "Worker.h"
#include "CommandManager.h"

using namespace std;
//��������� ���������� � ��������� � ����������
bool Worker::GetKeyboard() {
	cout << "���:\n>> ";
	cin >> name;

	cout << "������ ���������� �����:\n";
	salary = get_answer(1, 100000);

	return true;
}
//����� ����������
void Worker::Print() {
	cout << setw(20) << name << setw(6) << salary << endl;
}