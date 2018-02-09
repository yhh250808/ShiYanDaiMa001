#include"functions.h"

string file;
vector<string> username;
int *userscore;
int numofuser;
int difficulty;

void maininterface(vector<string> &username, int* &userscore, int &numofuser, int &difficulty, string file)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	string inputchoice;
	SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
	cout << "input the number of function you want to use: (1-4)" << endl;
	cout << "1. start" << endl;
	cout << "2. set difficulty" << endl;
	cout << "3. ranking list" << endl;
	cout << "4. exit" << endl;
	cin >> inputchoice;

	while (inputchoice[0]<49 || inputchoice[0]>52){
		system("cls");
		inputchoice.clear();
		cout << "input the ";

		SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "NUMBER ";
		SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

		cout<< "of function you want to use: " ;

		SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "(1-4)" << endl;
		SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);

		cout << "1. start" << endl;
		cout << "2. set difficulty" << endl;
		cout << "3. ranking list" << endl;
		cout << "4. exit" << endl;

		cin >> inputchoice;
	}

	while (inputchoice[0]<49 || inputchoice[0]>52){
		system("cls");
		inputchoice.clear();
		cout << "input the NUMBER of function you want to use:" << endl;
		cout << "1. start" << endl;
		cout << "2. set difficulty" << endl;
		cout << "3. ranking list" << endl;
		cout << "4. exit" << endl;
		
		cin >> inputchoice;
	}
	if (inputchoice[0] == 49)begingame(username, difficulty, userscore, numofuser);
	else if (inputchoice[0] == 50)changedifficulty(difficulty);
	else if (inputchoice[0] == 51)displayscore(username, userscore, numofuser);
	else if (inputchoice[0] == 52)exitfromprogram(username, userscore, numofuser, file);
	else {
		cout << "system error! wrong input value!" << endl;
		system("pause");
		exit(0);
	}
}

int main()
{	

	file = "userinfo.txt";
	userscore = new int[100]; for (int i = 0; i < 20; i++)userscore[i] = NULL;
	numofuser = 0;
	difficulty = 1;

	readfile(username, userscore, numofuser, file);

	while (true){
		maininterface(username, userscore, numofuser, difficulty, file);
	}
	return 0;
}