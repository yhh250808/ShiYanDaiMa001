#include<iostream>
#include<string>
#include<time.h>
#include<vector>
#include <Windows.h>
#include <process.h>
#include <cstdlib>
#include<fstream>
#include<cassert>
#include<stack>
using namespace std;

HANDLE hTimer;
string userAnwserVec;



//函数声明
int begingame(vector<string> &username, int &difficulity, int* &userscore, int &numofuser);
int searchuser(vector<string> username, int* &userscore, string gamername, int &numofuser);
int createaccount(vector<string> &username, int* &userscore, string gamername, int &numofuser);
void deleteaccount(vector<string> &username, int* &userscore, int &numofuser, int numofdeleteaccount);
void sortscore(vector<string> &username, int* &userscore, int &numofuser, int* &order);
int creategame(int a, int b, int c, int d);
bool chk(double a[], int n);
unsigned WINAPI ThreadProc(void* param);
int countunswer(int isexist,int a,int b,int c,int d);
bool IsOperator(char ch);
int Precedence(char op1, char op2);
void inFix2PostFix(string inFix, char* postFix);
double postFixEval(char* postFix);
void addscore(int uid, int *userscore, int score);
int changedifficulty(int &difficulty);
int displayscore(vector<string> &username, int* &userscore, int &numofuser);
int exitfromprogram(vector<string> &username, int* &userscore, int &numofuser, string filename);
int readfile(vector<string> &username, int* &userscore, int &numofuser, string file);
int writefile(vector<string> &username, int* &userscore, int &numofuser, string file);
string userunswer;
//开始游戏功能
int begingame(vector<string> &username, int &difficulity, int* &userscore, int &numofuser)//dirriculty 1-6;
{
	system("cls");

	string gamername;
	int uid, timelimit, score, isexist;
	int a, b, c, d;


	uid = 0; timelimit = 0; score = 0;
	a = 0; b = 0; c = 0; d = 0;

	cout << "Be care: your game account should be less than 20 characters (include space)" << endl;
	system("pause");
	cout << "please input your game account:" << endl;
	cin >> gamername;
	gamername.push_back('\0');
	uid = searchuser(username, userscore, gamername, numofuser);
	if (uid == -1)uid = createaccount(username, userscore, gamername, numofuser);
	system("cls");

	cout << "game start!" << endl;
	cout << "the game will repeat three times" << endl;
	system("pause");

	int i = 1;
	//第一遍
	for (int i = 0; i < 3; i++){
		system("cls");

		userAnwserVec.clear();
		cout << "the" << i << "time" << endl;
		cout << "you will have" << (7 - difficulity) * 10 << "secounds to write down your unswer after the game is created. " << endl;
		cout << "creating the game..." << endl;

		a = (rand() % 10) + 1;
		b = (rand() % 10) + 1;
		c = (rand() % 10) + 1;
		d = (rand() % 10) + 1;
		//a = 6; b = 4; c = 3; d = 3;
		isexist = creategame(a, b, c, d);
		cout << "create geme succeed" << endl;
		system("pause");
		system("cls");

		cout << "the four numbers are: " << endl;
		cout << a << '\t' << b << '\t' << c << '\t' << d << endl;
		cout << "please input your unswer: " << endl;
		hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
		LARGE_INTEGER dueTime;
		const int unitsPerSecond = 10000000;
		int timelimit = 0;
		switch (difficulity)
		{
		case 1:
			timelimit = 10 * 60;
		case 2:
			timelimit = 5 * 60;
		case 3:
			timelimit = 3 * 60;
		case 4:
			timelimit = 2 * 60;
		case 5:
			timelimit = 1 * 60;
		case 6:
			timelimit = 20;
		}
		dueTime.QuadPart = -(timelimit *unitsPerSecond);

		SetWaitableTimer(hTimer, &dueTime, 0, NULL, NULL,FALSE);			
		HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, NULL, 0, NULL);
		WaitForSingleObject(hTimer, 600000);

		TerminateProcess(hThread, 0);

		int y_or_n1 = countunswer(isexist,a,b,c,d);
		if (y_or_n1 == 1)score++;

	}
		

	system("cls");

	score = score*difficulity;
	cout << "game over! your score is:" << score << endl;
	addscore(uid, userscore,score);
	system("pause");
	system("cls");
	return 0;
}

int searchuser(vector<string> username, int* &userscore, string gamername, int &numofuser)
{
	int dif;
	if (numofuser == 0)
		return createaccount(username, userscore, gamername, numofuser);
	for (int i = 0; i < numofuser; i++)
	{	
		dif = 0;
		if (username[i].length() != gamername.length()){
			continue;
		}
		int len = min(username[i].length(), gamername.length());
		cout << username[i].length() << gamername.length();
		for (int j = 0; j < len - 1; j++){
			if ((username[i][j] != gamername[j]))
				dif++;
		}
		if (dif == 0)
			return i;
	}
	cout << "creating account" << endl;
	system("pause");
	return -1;
}

int createaccount(vector<string> &username, int* &userscore, string gamername, int &numofuser)
{
	if (numofuser >= 99){
		char input = 0;
		cout << "to much accounts! do you want to delete some accounts that gets low score? Y/N" << endl;
		while (input != 89 && input != 78 && input != 110 && input != 121){
			cout << "wrong input!" << endl;
			system("pause");
			cout << "to much accounts! do you want to delete some accounts that gets low score? Y/N" << endl;
			cin >> input;
		}
		if (input == 78 || input == 110){
			cout << "system will exit" << endl;
			system("pause");
			exit(0);
		}
		else {
			int numofdeleteaccount=0;
			cout << "input the number of accounts you want to delete" << endl;
			cin >> numofdeleteaccount;
			while (numofdeleteaccount<0 || numofdeleteaccount>99){
				cout << "wrong input!" << endl;
				system("pause");
				cout << "input the number of accounts you want to delete" << endl;
				cin >> numofdeleteaccount;
			}
			deleteaccount(username, userscore, numofuser, numofdeleteaccount);
		}
	}

	numofuser++;
	username.push_back(gamername);
	return numofuser-1;
}

void deleteaccount(vector<string> &username, int* &userscore, int &numofuser, int numofdeleteaccount){
	for (int i = numofuser - 1; i > numofuser - 1 - numofdeleteaccount; i--){
		username.pop_back();
		userscore[i] = 0;
	}
	numofuser -= numofdeleteaccount;

}
void sortscore(vector<string> &username, int* &userscore, int &numofuser, int* &order)//从大到小排序
{
	int max = 0;
	for (int i = 0; i < numofuser-1; i++)
	{
		max = i;
		for (int j = i; j < numofuser; j++)
		{
			if (userscore[j]>userscore[max]){
				int temp = 0;
				temp = userscore[max];
				userscore[max] = userscore[j];
				userscore[j] = temp;
				temp = order[max];
				order[max] = order[j];
				order[j] = temp;
			}
		}
		
	}
}
int creategame(int a,int b,int c,int d)//返回是否存在24点
{
	double p[4] = { a, b, c, d };
	return chk(p, 4);
}

bool chk(double a[], int n)
{
	if (n == 1)
	{
		if (fabs(a[0] - 24) < 10e-12) {  return 1; }
		else return 0;
	}
	for (int i = 0; i< n; ++i)
	{
		for (int j = i + 1; j< n; ++j)
		{
			double t1 = a[i];
			double t2 = a[j];
			double b[4];
			int r = 0;
			for (int k = 0; k < n; ++k)
			{
				if ((k != i) && (k != j)) b[r++] = a[k];
			}
			b[n - 2] = t1 + t2;
			if (chk(b, n - 1)) return 1;
			b[n - 2] = t1*t2;
			if (chk(b, n - 1)) return 1;
			b[n - 2] = t1 - t2;
			if (chk(b, n - 1)) return 1;
			b[n - 2] = t2 - t1;
			if (chk(b, n - 1)) return 1;
			b[n - 2] = t1 / t2;
			if (chk(b, n - 1)) return 1;
			b[n - 2] = t2 / t1;
			if (chk(b, n - 1)) return 1;
		}
	}
	return 0;
}
unsigned WINAPI ThreadProc(void* param)
{
	string userunswer;
	cin >> userunswer;
	userAnwserVec = userunswer;
	
	DWORD ret = WaitForSingleObject(hTimer, 0);
	if (WAIT_TIMEOUT == ret)	// 如果测试时间还没有走完，提前触发
	{
		LARGE_INTEGER dueTime;
		dueTime.QuadPart = -1;
		SetWaitableTimer(hTimer, &dueTime, 0, NULL, NULL, FALSE);			
	}
	
	ExitThread(0);
	return 0;

}

void sortnum(int* &a){
	int min = 0;
	for (int i = 0; i < 4; i++){
		min = i;
		for (int j = i; j < 4; j++){
			if (a[min]>a[j]){
				int temp = a[j];
				a[j] = a[min];
				a[min] = temp;
			}
		}
	}
}

int countunswer(int isexist, int a, int b, int c, int d)//isexist=1存在，=0不存在
{
	int lenofunswer = userAnwserVec.length();
	if (lenofunswer == 0){
		cout << "time out!" << endl;
		Sleep(1000);
		return 0;
	}
	if (lenofunswer > 20){
		cout << "wrong expression!" << endl;
		system("pause");
		return 0;
	}
	char userunswer[20] = { 0 };
	for (int i = 0; i < lenofunswer; i++)
	{
		userunswer[i] = userAnwserVec[i];
	}
	//控制输入格式
	for (int i = 0; i < lenofunswer; i++){
		if ((int)userunswer[i] == 110 || (int)userunswer[i] == 78){
			if ((int)userunswer[i + 1] == 111 || (int)userunswer[i + 1] == 79){
				if (!isexist){
					cout << "right unswer!" << endl;
					system("pause");
					return 1;
				}
				else{
					cout << "wrong unswwer!" << endl;
					system("pause");
					return 0;
				}
			}
			cout << "wrong expression!" << endl;
			system("pause");
			return 0;
		}
		if (((int)userunswer[i]<48 || (int)userunswer[i]>57) && (int)userunswer[i] != 40 && (int)userunswer[i] != 41 && (int)userunswer[i] != 43 && (int)userunswer[i] != 45 && (int)userunswer[i] != 42 && (int)userunswer[i] != 47 && (int)userunswer[i] != 0)
		{
			cout << "wrong expression!" << endl;
			system("pause");
			return 0;
		}
	}
	if ((int)userunswer[0] == 43 || (int)userunswer[0] == 42 || (int)userunswer[0] == 45 || (int)userunswer[0] == 47){
		cout << "wrong expression!" << endl;
		system("pause");
		return 0;
	}
	if ((int)userunswer[0] != 40 && ((int)userunswer[0]<49 && (int)userunswer[0]>57)){
		cout << "wrong expression!" << endl;
		system("pause");
		return 0;
	}
	if ((int)userunswer[19] == 40){
		cout << "wrong expression!" << endl;
		system("pause");
		return 0;
	}
	if ((int)userunswer[0] == 41){
		cout << "wrong expression!" << endl;
		system("pause");
		return 0;
	}
	for (int i = 0; i < lenofunswer; i++){
		if ((int)userunswer[i] == 40){
			if ((int)userunswer[i + 1]<49 && (int)userunswer[i + 1]>57){
				cout << "wrong expression!" << endl;
				system("pause");
				return 0;
			}
			else if (i != 0 && (int)userunswer[i - 1] != 43 && (int)userunswer[i - 1] != 45 && (int)userunswer[i - 1] != 42 && (int)userunswer[i - 1] != 47){
				cout << "wrong expression!" << endl;
				system("pause");
				return 0;
			}
		}
		if ((int)userunswer[i] == 41){
			if ((int)userunswer[i - 1]<49 && (int)userunswer[i - 1]>57){
				cout << "wrong expression!" << endl;
				system("pause");
				return 0;
			}
			else if (i != 19 && (int)userunswer[i + 1] != 43 && (int)userunswer[i + 1] != 45 && (int)userunswer[i + 1] != 42 && (int)userunswer[i + 1] != 47 && (int)userunswer[i + 1] != '\0'){
				cout << "wrong expression!" << endl;
				system("pause");
				return 0;
			}
		}
		if ((int)userunswer[i] == 43 || (int)userunswer[i] == 45 || (int)userunswer[i] == 42 || (int)userunswer[i] == 47){
			if ((int)userunswer[i + 1] == 43 && (int)userunswer[i + 1] == 45 && (int)userunswer[i + 1] == 42 && (int)userunswer[i + 1] == 47){
				cout << "wrong expression!" << endl;
				system("pause");
				return 0;
			}
		}
	}
	int *num; num = new int[4]; for (int i = 0; i < 4; i++)num[i] = 0;
int order = 0;
	for (int i = 0; i < 20; i++){
		if (userunswer[i] >= 50 && userunswer[i] <= 57){
			num[order] = userunswer[i]-48;
			order++;
		}
		else if (userunswer[i] == 49 && userunswer[i + 1] == 48){
			num[order] = 10;
			order++;
		}
		else if (userunswer[i] == 49){
			num[order] = 1;
			order++;
		}
	}
	if (order != 4){
		cout << "wrong expression!" << endl;
		system("pause");
		return 0;
	}
	int *and; and = new int[4]; and[0] = a; and[1] = b; and[2] = c; and[3] = d;
	sortnum(and);
	sortnum(num);
	for (int i = 0; i < 4; i++){
		if (and[i] != num[i]){
			cout << "wrong expression!" << endl;
			system("pause");
			return 0;
		}
	}

	//控制输入格式结束

	double expressionvalue = 0;
	char postfixunswer[20] = { 0 };
	inFix2PostFix(userunswer, postfixunswer);
	expressionvalue = postFixEval(postfixunswer);
	if (fabs(expressionvalue - 24) < 10e-12){
		cout << "right answer!" << endl;
		system("pause");
		return 1;
	}
	else{
		cout << "wrong unswwer!" << endl;
		system("pause");
		return 0;
	}
}

//判断是否是操作符
bool IsOperator(char ch)
{
	char ops[] = "+-*/";
	for (int i = 0; i < sizeof(ops) / sizeof(char); i++)
	{
		if (ch == ops[i])
			return true;
	}
	return false;
}

// 比较两个操作符的优先级
int Precedence(char op1, char op2)
{
	if (op1 == '(')
	{
		return -1;
	}

	if (op1 == '+' || op1 == '-')
	{
		if (op2 == '*' || op2 == '/')
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	if (op1 == '*' || op1 == '/')
	{
		if (op2 == '+' || op2 == '-')
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

// 中缀表达式转换成后缀表达式
void inFix2PostFix(string inFix, char* postFix)
{
	int j = 0, len;
	char c;
	stack<char> st;

	len = inFix.length();

	for (int i = 0; i < len; i++)
	{
		c = inFix[i];

		if (c == '(')
			st.push(c);
		else if (c == ')')
		{
			while (st.top() != '(')
			{
				postFix[j++] = st.top();
				st.pop();
			}
			st.pop();
		}
		else
		{
			if (!IsOperator(c))
				st.push(c);
			else
			{
				while (st.empty() == false
					&& Precedence(st.top(), c) >= 0)
				{
					postFix[j++] = st.top();
					st.pop();
				}
				st.push(c);
			}
		}
	}

	while (st.empty() == false)
	{
		postFix[j++] = st.top();
		st.pop();
	}
	postFix[j] = 0;
}

// 后缀表达式求值程序
double postFixEval(char* postFix)
{
	stack<char> st;
	int len = strlen(postFix);
	char c;

	for (int i = 0; i < len; i++)
	{
		c = postFix[i];
		if (IsOperator(c) == false)
		{
			st.push(c - '0');
		}
		else
		{
			char op1, op2;
			int val;

			op1 = st.top();
			st.pop();
			op2 = st.top();
			st.pop();

			switch (c)
			{
			case '+':
				val = op1 + op2;
				break;
			case '-':
				val = op2 - op1;
				break;
			case '*':
				val = op1 * op2;
				break;
			case '/':
				val = op2 / op1;
				break;
			}
			st.push(val);
		}
	}

	return st.top();
}

void addscore(int uid, int *userscore,int score)
{
	if (score > userscore[uid])userscore[uid] = score;
}
//开始游戏功能结束

//修改难度功能
int changedifficulty(int &difficulty)
{
	
	string userdif;

	while (userdif.length()!=1){
		system("cls");
		cout << "the difficulty should between 1 to 6" << endl;
		cout << "input the difficulty you want to change to:" << endl;
		cin >> userdif;
	}

	while ((int)userdif[0]<49 || (int)userdif[0]>54){
		system("cls");
		cout << "the difficulty should between 1 to 6" << endl;
		cout << "input the difficulty you want to change to:" << endl;
		cin >> userdif;
	}
	difficulty = (int)userdif[0] - 48;
	cout << "change difficulty succeed " << endl;
	system("pause");
	system("cls");
	return 0;
}
//修改难度功能结束

//显示排行榜功能
int displayscore(vector<string> &username, int* &userscore, int &numofuser)
{
	system("cls");
	cout << numofuser;
	int *order; order = new int[numofuser]; for (int i = 0; i < numofuser; i++)order[i] = i;
	int *copyuserscore; copyuserscore = new int[numofuser]; for (int i = 0; i < numofuser; i++)copyuserscore[i] = userscore[i];
	sortscore(username, copyuserscore, numofuser, order);

	int displaynum = 10;
	if (numofuser < 10)displaynum = numofuser;
	if (numofuser == 0){
		cout << "no record!" << endl;
		system("pause");
		system("cls");
		return 0;
	}
	cout << "rank" << '\t';
	cout << "gamername" << "\t\t" << "gamerscore" << endl;
	for (int i = 0; i < displaynum; i++)
	{
		cout << i + 1 << '\t';
			cout << username[order[i]];
		cout << "\t\t\t";
		cout << userscore[order[i]] << endl;
	}
	system("pause");
	system("cls");
	return 0;
}
//显示排行榜功能结束

//退出
int exitfromprogram(vector<string> &username, int* &userscore, int &numofuser,string filename)
{
	char input;
	input = 0;
	cout << "are you sure you want to quit? Y/N" << endl;
	cin >> input;
	while (input != 89 && input != 78 && input != 110 && input != 121){
		cout << "wrong input!" << endl;
		system("pause");
		cout << "are you sure you want to quit? Y/N" << endl;
		cin >> input;
	}
	if (input == 78 || input == 110)return 0;
	else {
		writefile(username, userscore, numofuser, filename);
	}
}
//退出

//读文件
int readfile(vector<string> &username, int* &userscore, int &numofuser, string file)
{
	fstream infile;
	infile.open(file.data());   //将文件流对象与文件连接起来 
	assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 

	string s;
	char *scoreinfo;  
	scoreinfo = new char[2]; for (int i = 0; i < numofuser; i++)scoreinfo[i] = NULL;
	int uid = 0;
	getline(infile, s);
	numofuser = (int)s[0] - 48;
	for (int i = 0; i < numofuser;i++)
	{
		getline(infile, s);
		int len = s.length();
		scoreinfo[0] = s[0];
		scoreinfo[1] = s[1];
		userscore[uid] = (scoreinfo[0] - 48) * 10 + scoreinfo[1] - 48;

		char *input; input = new char[len - 1]; for (int i = 0; i < len - 2; i++)input[i] = s[i + 2]; input[len - 1] = '\0';
		username.push_back(input);
		uid++;
	}
	infile.close();             //关闭文件输入流 
	return 0;
}

//写文件
int writefile(vector<string> &username, int* &userscore, int &numofuser, string file)
{
	fstream outfile;
	outfile.open(file.data());   //将文件流对象与文件连接起来 
	assert(outfile.is_open());   //若失败,则输出错误消息,并终止程序运行 

	char inputdata[100] = { 0 };
	outfile << numofuser;
	outfile << '\n';
	for (int i = 0; i < 100; i++)inputdata[i] = NULL;
	for (int i = 0; i < numofuser; i++)
	{
		inputdata[0] = (char)userscore[i] / 10 + 48;
		inputdata[1] = (char)userscore[i] % 10 + 48;
		inputdata[2] = '\0';


		outfile << inputdata;

		int len = username[i].length();
		for (int j = 0; j < len - 1; j++)outfile << username[i][j];
		outfile << '\n';
	}
	outfile.close();
	exit(0);
}