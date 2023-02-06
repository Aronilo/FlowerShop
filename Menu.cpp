#include "Menu.h"
#include "CommandManager.h"

//�������� ������� ����
vector<string> Menu::main_cmds = {
	"�������� ����� ������",
	"����� � ������� ������",
	"���������",
	"��������� ����",
	"��������� ������",
	"������ ���������� �����",
	"����������",
	"���������� ������"
};
//���������� ��� ������� ������ ����
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
//������ ����� ����
vector<string> Menu::add_cmds = {
	"������ ������ � ������ � ����������",
	"������ ������ � ������ � �����",
	"��������� ����� ������ � ����",
	"�����",
};
//��������� ������ �������
void Menu::AddMenu() {
	int cmd = Select(add_cmds);
	//������� �� �����
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
//2 ����� ����
vector<string> Menu::sell_cmds = {
	"����� �� �����",
	"����� �� �����",
	"����� �� ����",
	"�����",
};
//���������� ������ ���� ������ ������������� ������ � ����������� �� �������
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
//3 �����
vector<string> Menu::worker_cmds = {
	"�������� ���������",
	"������� ���������",
	"�����"
};
//���������� ������ ���� ������������� � �����������
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
//4 �����
vector<string> Menu::info_cmds = {
	"����� �� ������",
	"������ ����������",
	"������",
	"�����"
};
//���������� ������ ��������� ���������� �� �������� ��������� ���
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
//����� ���� � ��������� ������
int Menu::Select(const vector<string>& cmds) {
	cout << endl;
	for (size_t i = 0; i < cmds.size(); i++) {
		cout << i + 1 << ". " << cmds[i] << endl;
	}
	cout << endl;
	return get_answer(1, cmds.size());
}
//���������� ������
void Menu::Terminate() {
	is_working = false;
}









