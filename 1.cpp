#include <iostream>
#include <string>
#include <algorithm>
#include <thread>
#include <sqlite3.h>
#include <chrono>
#include <ctime>
#include <vector>
#include <time.h>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

static int DatabaseCreator(const char* c);
static int TableCreator(const char* c);
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
	TableCreator(dir);
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
		"NAME TEXT NOT NULL,"
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
		"NAME TEXT NOT NULL,"
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

	cout << chrono::system_clock::now << endl;

	if (str.substr(0, pfx1.size()) == pfx1)
	{
		str.erase(0,pfx1.size());
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
		str.erase(0,pfx2.size());
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
    ifstream ConfigFile("fconfig.ini");
	string str;
    int port;
	for (int i = 0; i < 6; i++) {
		getline(ConfigFile, str);
		if (str[0] == 'p')
		{
			if (str[1] == '1')
			{
				str.erase(0, 5);
				pfx1 = str;
				cout << pfx1 << endl;
			}
			else if (str[1] == '2')
			{
				str.erase(0, 5);
				pfx2 = str;
				cout << pfx2 << endl;
			}
            else if (str[2] == 'o')
            {
                str.erase(0, 7);
                port = atoi(str.c_str());
            }
		}

	}
    ConfigFile.close();

    int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = { 0 };
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return;
    }
    if ((client_fd
         = connect(sock, (struct sockaddr*)&serv_addr,
                   sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return;
    }
	for (;;)
	{
        valread = read(sock, buffer, 1024);
		DataInserter(dir, string(buffer));
	}

}

static int DataSelector(const char* c)
{
	sqlite3* Database;
	char* messageError;

	string sql = "SELECT * FROM TABLEA;";
	
	int exit = sqlite3_open(c, &Database);
	exit = sqlite3_exec(Database, sql.c_str(), DataPrinter, NULL, &messageError);

	if (exit != SQLITE_OK)
	{
		cout << "Error in data selection." << endl;
		cout << messageError << endl;
		sqlite3_free(messageError);
	}
	else
		cout << "DataSelector selected data successfully.";

	return 0;
}

static int DataPrinter(void* NotUsed, int argc, char** argv, char** azColName)
{
	vector <string> sorter;
	//cout << azColName[0] << endl;
	for (int i = 0; i < argc; i++) {
		//cout << azColName[i] << ": " << argv[i] << endl;
		sorter.push_back(argv[i]);
	}
	sort(sorter.begin(), sorter.end());
	for (int i = 0; i < sorter.size(); i++)
	{
		cout << sorter[i] << ": " << argv[i] << endl;
	}

	cout << endl;

	return 0;
}

void PrinterThread()
{
	for(;;) {
		this_thread::sleep_for(chrono::milliseconds(10000));
		DataSelector("Database.db");
	}
}

void CatcherThread()
{
	DataCatcher();
}
