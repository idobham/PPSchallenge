#include <iostream>
#include <string>
#include <thread>
#include "sqlite3.h"
#include <chrono>
#include <ctime>
#include <time.h>
#include <fstream>

using namespace std;

static int DatabaseCreator(const char* c);
static int TableCreator(const char* c);
static int TableCreatorString(const char* c);
static int DataInserter(const char* c, string str);
static int DataSelector(const char* c);
static int DataPrinter(void* Unused, int argc, char** argv, char** azColName);
void DataCatcher();
void CatcherThread();
void PrinterThread();
string pfx1, pfx2;

const char* dir = "Database.db";

int main()
{
	ifstream ConfigFile("fconfig.ini");
	string str;
	for (int i = 0; i < 6; i++) {
		getline(ConfigFile, str);
		if (str[0] == 'p')
		{
			if (str[1] == '1')
			{
				str.erase(0, 5);
				pfx1 = str;
				cout << "Primary Prefix set successfully: " << pfx1 << endl;
			}
			else if (str[1] == '2')
			{
				str.erase(0, 5);
				pfx2 = str;
				cout << "Primary Prefix set successfully: " << pfx2 << endl;
			}
		}

	}
	DatabaseCreator(dir);
	TableCreator(dir);
	TableCreatorString(dir);
	thread ThreadI(CatcherThread);
	thread ThreadII(PrinterThread);

	ThreadI.join();
	ThreadII.join();

	return 0;
}

static int DatabaseCreator(const char* c)
{
	sqlite3* Database;
	int exit = 0;

	exit = sqlite3_open(c, &Database);
	sqlite3_close(Database);

	return 0;
}

static int TableCreator(const char* c)
{
	sqlite3* Database;
	char* messageError;

	string sql = "CREATE TABLE IF NOT EXISTS TABLEA ("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
		"NAME INT NOT NULL,"
		"TIME TEXT NOT NULL);";

	int exit = 0;
	exit = sqlite3_open(c, &Database);
	exit = sqlite3_exec(Database, sql.c_str(), NULL, 0, &messageError);

	if (exit != SQLITE_OK)
	{
		cout << "Error in table creation." << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "TableCreator created table successfully." << endl;

	sqlite3_close(Database);

	return 0;
}

static int TableCreatorString(const char* c)
{
	sqlite3* Database;
	char* messageError;

	string sql = "CREATE TABLE IF NOT EXISTS TABLESTRING ("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
		"NAME INTEGER NOT NULL,"
		"TIME TEXT NOT NULL);";

	int exit = 0;
	exit = sqlite3_open(c, &Database);
	exit = sqlite3_exec(Database, sql.c_str(), NULL, 0, &messageError);

	if (exit != SQLITE_OK)
	{
		cout << "Error in string table creation." << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "TableCreatorString created table successfully." << endl;

	sqlite3_close(Database);

	return 0;
}

static int DataInserter(const char* c, string str)
{
	sqlite3* Database;
	char* messageError;

	//cout << chrono::system_clock::now << endl;

	if (str.substr(0, pfx1.size()) == pfx1)
	{
		str.erase(0, pfx1.size());
		string sql = "INSERT INTO TABLEA('NAME', 'TIME') VALUES ('" + str + "', 'time'); ";
		int exit = sqlite3_open(c, &Database);
		exit = sqlite3_exec(Database, sql.c_str(), NULL, 0, &messageError);
		if (exit != SQLITE_OK) {
			cerr << "Error in insertData function." << endl;
			cout << messageError << endl;
			sqlite3_free(messageError);
		}
		else
			cout << "Records inserted Successfully!" << endl;
	}
	if (str.substr(0, pfx2.size()) == pfx2)
	{
		str.erase(0, pfx2.size());
		string sql = "INSERT INTO TABLESTRING('NAME', 'TIME') VALUES ('" + str + "', '" + "NowToString" + "'); ";
		int exit = sqlite3_open(c, &Database);
		exit = sqlite3_exec(Database, sql.c_str(), NULL, 0, &messageError);
		if (exit != SQLITE_OK) {
			cerr << "Error in insertData function." << endl;
			cout << messageError << endl;
			sqlite3_free(messageError);
		}
		else
			cout << "Records inserted Successfully!" << endl;
	}

	return 0;

}

void DataCatcher()
{
	for (;;)
	{
		string s;
		cin >> s;
		DataInserter(dir, s);
	}

}

static int DataSelector(const char* c)
{
	sqlite3* Database;
	char* messageError;

	string sql = "SELECT * FROM TABLEA ORDER BY NAME;";

	int exit = sqlite3_open(c, &Database);
	exit = sqlite3_exec(Database, sql.c_str(), DataPrinter, NULL, &messageError);

	if (exit != SQLITE_OK)
	{
		cout << "Error in data selection." << endl;
		cout << messageError << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "DataSelector selected data successfully." << endl;

	return 0;
}

static int DataPrinter(void* NotUsed, int argc, char** argv, char** azColName)
{

	for (int i = 0; i < argc; i++) {
		// column name and value
		cout << azColName[i] << ": " << argv[i] << endl;
	}

	cout << endl;


	return 0;
}

void PrinterThread()
{
	for (;;) {
		this_thread::sleep_for(chrono::milliseconds(10000));
		DataSelector("Database.db");
	}
}

void CatcherThread()
{
	DataCatcher();
}
