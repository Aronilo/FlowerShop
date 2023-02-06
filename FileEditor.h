#pragma once

#include <iostream>
#include <vector>

#include "Flower.h"
#include "Worker.h"

using namespace std;
//�������������� � �������
class FileEditor {
public:
	string flower_filename;
	string worker_filename;
	string info_filename;
	int balance;
	//������� ���� ������ �������� ������ � �������� ����� �������� ���� �����
	FileEditor(const string& ff = "flowers.bin",
		const string& wf = "workers.bin",
		const string& inff = "info.bin");
	//�����������
	static bool IsEqualName(const Flower& fl, void* arg);

	static bool IsEqualColor(const Flower& fl, void* arg);

	static bool HasLessCost(const Flower& fl, void* arg);

	static bool HasGrossCost(const Flower& fl, void* arg);
	
	bool AddFlowers(const Flower& fl);

	void AddFlowerFile();

	void SaveFlowersFile();

	void DeleteFlowers(const vector<Flower>& flowers);

	vector<Flower> FindFlowers(bool(*f)(const Flower&, void*), void* arg);

	void AddWorker(const Worker& worker);

	bool DeleteWorker(const string& name);

	void PrintWorkers();

	void NextDay();

	int GetTotalSalary();

	void PrintFlowers();

	void UpdateBalance();
};

