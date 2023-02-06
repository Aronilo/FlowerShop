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
//����������� � ������� ��������� ������ � �����
FileEditor::FileEditor(const string& ff, const string& wf, const string& inff) :
	flower_filename(ff), worker_filename(wf),
	info_filename(inff), balance(0) {

	ifstream in(inff);

	if (in.is_open()) {
		in.read((char*)&balance, sizeof(int));
		in.close();
	}
}
//�������� ���� ��� ��� ������ fl ��������� � ������ ����� �������� arg
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
//���������� ������ � ����
bool FileEditor::AddFlowers(const Flower& fl) {
	//������ ����� ��� ���
	if (fl.count * fl.prime_cost > balance) {
		cout << "��� ������� ������ \"" << fl.name << "\" ��������� " << fl.count * fl.prime_cost <<
			" �������� ������." << endl << "������� ������: " << balance << ". ������� ����������." << endl;
		return false;
	}

	fstream file(flower_filename, ios::binary | ios::in | ios::out);
	//���� ��� �����, �� ��������� ���� �� ������
	if (!file.is_open()) {
		file.open(flower_filename, ios::binary | ios::out);
		file.close();
		file.open(flower_filename, ios::binary | ios::in | ios::out);
	}
	//����������� � ����� �����
	file.seekg(0, ios::end);
	
	int size = file.tellg();
	//����������� � �����
	file.seekg(0, ios::beg);

	char is_deleted;
	int pos = 0;
	//����  ����� ��� ������ ���������� � ������
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

	cout << "����� \"" << fl.name << "\" � ���������� " << fl.count <<
		" ���� ���� ������� ���������." << endl;

	balance -= fl.count * fl.prime_cost;
	//����������� ����� � �����
	UpdateBalance();

	return true;
}
//����� ���������� ������ � ������ ������ � ����
void FileEditor::SaveFlowersFile() {
	cout << "\n���������� ������ � ������ ������ � ����..." << endl;

	cout << "��� ����� � ������� � ������: ";

	string filename;
	cin >> filename;

	cout << "���������� ������� ������ ��� ������:" << endl;

	int n_flowers = get_answer(1, 20);

	Flower flower;

	fstream file(filename, ios::binary | ios::out);
	for (int i = 0; i < n_flowers; i++) {
		cout << "��������� ������ � ������ #" << i + 1 << "." << endl;
		flower.GetKeyBoard();
		flower.SaveToFile(file);
	}

	cout << "\n���������� � ������ ������ ������� ���������!" << endl;
}
//����� ��������� ����� � ������ ������ � �����
void FileEditor::AddFlowerFile() {

	cout << "\n��������� ������ � ������ ������ � �����..." << endl;

	cout << "��� ����� � ������� � ������: ";

	string filename;
	cin >> filename;

	fstream file(filename, ios::binary | ios::in);

	if (!file.is_open()) {
		cout << "���� �� ������!" << endl;
		return;
	}

	file.seekg(0, ios::end);
	//��������� ������� �� ������� �� ���������
	int size = file.tellg();

	file.seekg(0, ios::beg);
	//����������� ���������� flower
	Flower flower;

	bool flag = false;

	int pos = 0;

	while (pos < size) {

		pos += FLOWER_SIZE - 1;

		flower.GetFile(file);
		AddFlowers(flower);
	}

	file.close();

	cout << "\n���������� � ������ ������� �������!" << endl;
}
//����� �������� ������ �� �����
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
		//���������� ������� ����� ��������� ������ � ������
		//0 ������ ���������
		//1 �������
		file.read((char*)&is_deleted, 1);

		if (is_deleted) {
			file.seekg(FLOWER_SIZE - 1, ios::cur);
			continue;
		}
		//��������� ���������� � ������
		fl.GetFile(file);

		if (file.fail()) {
			file.close();
			return;
		}
		//��������� ������
		for (size_t i = 0; i < flowers.size(); i++) {

			if (flowers[i].count == 0)
				continue;

			if (fl.name == flowers[i].name) {
				if (fl.count < flowers[i].count) {
					cout << "������ \"" << flowers[i].name << "\" ��� � ������� � �������� ����������." << endl;
					
					file.seekp(-FLOWER_SIZE, ios::cur);
					//������ ������� ��� ���
					//��� �� �������
					file.write((char*)&x, sizeof(char));
					file.seekg(-1, ios::cur);
				} else {
					cout << "����� \"" << flowers[i].name << "\" � ���������� " << flowers[i].count <<
						" ���� ���� ������� �������." << endl;
					cout << "�������: " << flowers[i].count * flowers[i].cost << endl;
					balance += flowers[i].count * flowers[i].cost;
					UpdateBalance();
					file.seekp(sizeof(int) * (-1), ios::cur);
					//������ ������� ����� ���� � ������
					int difference = fl.count - flowers[i].count;

					file.write((char*)&difference, sizeof(int));
					file.seekg(0, ios::cur);
				}
			}
		}

		
	}
	file.close();
}
//����� ������ ������ ��� ������� ������� f ���������� true
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
		//������� ��� ���
		in.read((char*)&is_deleted, 1);

		if (is_deleted) {
			//���� ������ �������, �� �� �� ����������
			in.seekg(FLOWER_SIZE - 1, ios::cur);
			continue;
		}
		fl.GetFile(in);

		if (in.fail()) {
			in.close();
			return flowers;
		}
		//���� ������ ������������� ����������, ����� ���������
		if (f(fl, arg)) {
			flowers.push_back(fl);
		}
	}

	return flowers;
}
//����� ���������� �����������
void FileEditor::AddWorker(const Worker& worker) {
	fstream file(worker_filename, ios::binary | ios::in | ios::out);

	if (!file.is_open()) {
		//�������� ����� �� ������
		file.open(worker_filename, ios::binary | ios::out);
		file.close();
		//�������� �� ������ � ������
		file.open(worker_filename, ios::binary | ios::in | ios::out);
	}

	file.seekg(0, ios::end);

	int size = file.tellg();

	file.seekg(0, ios::beg);

	char is_deleted;
	int pos = 0;

	while (pos < size) {

		file.read((char*)&is_deleted, 1);
		//����� ��������� �������
		if (is_deleted) {
			//���������� ����������, ������ ���������
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
//����� �������� ��� �� �����
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
//����� ��������� ������ �����������
void FileEditor::PrintWorkers() {
	ifstream file(worker_filename, ios::binary);

	if (!file.is_open()) {
		cout << "���� �� ������!" << endl;
		return;
	}

	file.seekg(0, ios::end);

	int size = file.tellg();

	file.seekg(0, ios::beg);

	Worker worker;

	cout << "������ ����������:" << endl;

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
		cout << "��������� �����������!" << endl;
	}

	file.close();
}
//����� ������� ��������� ��������
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
//����� ���� ����
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
			cout << "����� " << flower.name << " � ���������� " << flower.count << " ���� �����������." << endl;
			file.seekp(-FLOWER_SIZE, ios::cur);
			file.write((char*)&x, 1);
			file.seekg(FLOWER_SIZE - 1, ios::cur);
		} else {
			file.seekp(sizeof(int) * (-2), ios::cur);
			flower.shelf_life--;
			file.write((char*)&flower.shelf_life, sizeof(int));
			file.seekg(sizeof(int), ios::cur);

			if (flower.shelf_life == 1) {
				cout << "���� ���������� " << flower.name << " � ���������� " << flower.count << " ���� ���������� 1 ����." << endl
					<< "������������� ������� ������� ������." << endl;
			}
		}
	}

	file.close();
}
//����� ������ ������ � �������
void FileEditor::PrintFlowers() {
	fstream file(flower_filename, ios::binary | ios::in);

	if (!file.is_open()) {
		cout << "���� �� ������!" << endl;
		return;
	}

	file.seekg(0, ios::end);

	int size = file.tellg();

	file.seekg(0, ios::beg);

	Flower flower;

	cout << "������ ������ � �������:" << endl;
	cout << setw(20) << "��������" << setw(20) << "����" << setw(8) << "����" << 
		"  ���������� ����  " << "���� ����������   ����������" << endl;

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
		cout << "������ ��� � �������!" << endl;
	}

	file.close();
}
//����� ���������� ������� � �����
void FileEditor::UpdateBalance() {
	ofstream out(info_filename);
	out.write((char*)&balance, sizeof(int));
	out.close();
}