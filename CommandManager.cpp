#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

#include "CommandManager.h"
#include "FileEditor.h"
#include "Worker.h"

using namespace std;

//��������� ������ �� ������������ � ���� ������ ����� �� ��������� ��������� � ��������� �����
int get_answer(int min, int max) {
	int answer;

	cout << ">> ";
	cin >> answer;
	
	while (cin.fail() || answer < min || answer > max) {
		cout << "������������ ����. ������� ����� �� " << min << " �� " << max << endl << ">> ";
		cin.clear();
		cin.ignore(256, '\n');
		cin >> answer;
	}
	return answer;
}
//����� ������(���������� �� �����)
void CommandManager::AddFlowerKeyBoard() {

	cout << "\n��������� ������ � ������ ������ � ����������..." << endl;

	Flower flower;
	flower.GetKeyBoard();
	//���������� � ������ ������ ���������� �� ������
	p_fe->AddFlowers(flower);
}
//����� ���������� �� ������� ������ �� �����
void CommandManager::SellByName() {

	cout << "\n����� �� �������� ������ � �� �������..." << endl << endl;

	cout << "��������:" << endl << "> ";

	string name;
	cin >> name;
	//�������� ������� ������ �� ����� �� ������� ������� ������������ ������� IsEqualName � ���������� � ��� ���������� &name
	vector<Flower> result = p_fe->FindFlowers(p_fe->IsEqualName, &name);

	for (size_t i = 0; i < result.size(); i++) {
		cout << setw(20) << "��������" << setw(20) << "����" << setw(8) << "����" <<
			"  ���������� ����  " << "���� ����������   ����������" << endl;
		result[i].Print();

		cout << "������� �������� ���������� ������ ��� ������� (0 � ��������� ������)" << endl;
		//������� ����� �� 0 �� ���-�� ������ �� ������
		int answer = get_answer(0, result[i].count);

		result[i].count = answer;
	}

	if (!result.empty()) {
		p_fe->DeleteFlowers(result);
		cout << "������� ������� �����������!" << endl;
	}
}
//����� ������ ���������� ������ � ������� �� �����
void CommandManager::SellByColor() {

	cout << "\n����� ������ �� ����� � �� �������..." << endl << endl;

	cout << "����:" << endl << "> ";

	string color;
	cin >> color;

	vector<Flower> result = p_fe->FindFlowers(p_fe->IsEqualColor, &color);

	bool is_sell = false;
	
	for (size_t i = 0; i < result.size(); i++) {
		result[i].Print();

		cout << "������� �������� ���������� ������ ��� ������� (0 � ��������� ������)" << endl;

		int answer = get_answer(0, result[i].count);
		if (answer > 0) {
			is_sell = true;
		}
		result[i].count = answer;
	}

	if (!result.empty() && is_sell) {
		p_fe->DeleteFlowers(result);
		cout << "������� ������� �����������!" << endl;
	}
}
//����� ������ � ������� ������
void CommandManager::SellByCost() {

	cout << "\n����� ������ �� ���� � �� �������..." << endl << endl;

	cout << "����������� ����� � �����\n\t1. ���� ��������\n\t2.���� ��������" << endl;

	int answer = get_answer(1, 2);

	cout << "����:" << endl;

	int cost = get_answer(1, 10000);

	//����� � ����� ������ � ����� ���� ��� ���� �������� �������������
	vector<Flower> result;
	if (answer == 1) {
		result = p_fe->FindFlowers(p_fe->HasLessCost, &cost);
	} else {
		result = p_fe->FindFlowers(p_fe->HasGrossCost, &cost);
	}


	for (size_t i = 0; i < result.size(); i++) {
		result[i].Print();

		cout << "������� �������� ���������� ������ ��� ������� (0 � ��������� ������)" << endl;

		int answer = get_answer(0, result[i].count);

		result[i].count = answer;
	}

	if (!result.empty()) {
		//�������� ������ �� �����
		p_fe->DeleteFlowers(result);
		cout << "������� ������� �����������!" << endl;
	}
}
//����� ������ ���������� ���������� � ������� ��������� � ����
void CommandManager::AddWorker() {
	cout << "\n���������� ���������� � ���������..." << endl << endl;
	
	Worker worker;

	worker.GetKeyboard();
	p_fe->AddWorker(worker);
}
//����� ����� �������� ���������� �� ��������� ��������� �� �����
void CommandManager::DeleteWorker() {
	cout << "\n�������� ���������� � ���������..." << endl << endl;

	string name;
	cout << "���:" << endl;
	cin >> name;
	
	if (p_fe->DeleteWorker(name)) {
		cout << "�������� �������!" << endl;
	} else {
		cout << "���������� � ������ ��������� �� �������!" << endl;
	}
}
//����� ����������� ����� ���������� � ������� ������� ������� �����������
void CommandManager::GetBalance() {
	cout << "������� ������ �����������: " << p_fe->balance << endl;
}
//����� ������ ���������� �����
void CommandManager::IssueSalary() {
	//������ ����� ���������� �����
	int salary = p_fe->GetTotalSalary();

	if (salary == 0) {
		cout << "����������� �������� � ������������� ������� ���������� �����!" << endl;
	} else if (salary > p_fe->balance) {
		cout << "������������ ������� ��� ������ ���������� �����." << endl;
		cout << "���������: " << salary << endl << "������: " << p_fe->balance << endl;
	} else {
		p_fe->balance -= salary;
		//���������� ������� � �����
		p_fe->UpdateBalance();
		cout << "���������� ����� � ������� " << salary << " ���������. ������: " << p_fe->balance << endl;
	}
}
//����� ��������� ������
void CommandManager::ReplenishBalance() {
	cout << "���������� ������� �����������..." << endl;
	cout << "����� ����� ����� ������?" << endl;

	int total = get_answer(1, 1000000);

	p_fe->balance += total;
	p_fe->UpdateBalance();
	cout << "������ ��������. ������� ������: " << p_fe->balance << endl;
}