#include <iostream>
#include <string>
#include <cctype>
#include "MyFunction.h"
#include <vector>
#include <iomanip>
#include <fstream>

using namespace std;
using namespace String;
using namespace myfunc;
using namespace OutPut;
const string ClientsFile = "Clients.txt";

struct sClient
{
	string AccountNumber = "";
	string PinCode = "";
	string Name = "";
	string Phone = "";
	int AccountBalance = 0;
};

enum enMainMenueOptions 
{ eClientList = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdateClientInfo = 4, eFindClient = 5, eExit = 6 };

void ShowMainMenueScreen();

sClient ReadClintData()
{
	sClient data;

	cout << "enter Account Number? ";
	getline(cin >> ws, data.AccountNumber);

	cout << "enter Pincode? ";
	getline(cin, data.PinCode);

	cout << "enter Name?";
	getline(cin, data.Name);

	cout << "enter phone?";
	getline(cin, data.Phone);

	cout << "enter Accountbalance?";
	cin >> data.AccountBalance;

	return data;
}

string ReadClientAccountNumber()
{
	string AccountNumber = "";
	cout << "Please enter AccountNumber?";
	cin >> AccountNumber;
	return AccountNumber;
}

void PrintClientRecord(sClient Client)
{
	cout << "The following is the extracted client record: " << endl << endl;

	cout << left << setw(15) << "Account Number" << ": " << Client.AccountNumber << endl;
	cout << left << setw(15) << "Pin Code" << ": " << Client.PinCode << endl;
	cout << left << setw(15) << "Name" << ": " << Client.Name << endl;
	cout << left << setw(15) << "Phone" << ": " << Client.Phone << endl;
	cout << left << setw(15) << "Account Balance" << ": " << Client.AccountBalance << endl;
}

void PrintClientRecordinOneLine(sClient Client)
{
	cout << "| ";
	SetWidthInCenter(15, Client.AccountNumber);
	cout << "| ";
	SetWidthInCenter(10, Client.PinCode);
	cout << "| ";
	SetWidthInCenter(40, Client.Name);
	cout << "| ";
	SetWidthInCenter(12, Client.Phone);
	cout << "| ";
	SetWidthInCenter(15, to_string(Client.AccountBalance));
}

void AddDataLineToFile(string FileName, string stDataLine)
{
	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << stDataLine << endl;

		MyFile.close();
	}
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
	string stClintRecord = "";

	stClintRecord += Client.AccountNumber + Seperator;
	stClintRecord += Client.PinCode + Seperator;
	stClintRecord += Client.Name + Seperator;
	stClintRecord += Client.Phone + Seperator;
	stClintRecord += to_string(Client.AccountBalance);

	return stClintRecord;
}

sClient ConvertLineToRecord(string stLine, string Seperator = "#//#")
{

	vector <string> vClientData = String::SplitString(stLine, Seperator);

	sClient Client;

	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stoi(vClientData[4]);

	return Client;

}

vector <sClient> LoadDataFromFiles(string FileName)
{
	vector <sClient> vClients;
	fstream Myfile;
	Myfile.open(FileName, ios::in);
	if (Myfile.is_open())
	{
		string line;
		sClient Client;

		while (getline(Myfile, line))
		{
			Client = ConvertLineToRecord(line, "#//#");

			vClients.push_back(Client);
		}

		Myfile.close();
	}

	return vClients;
}

void PrintAllClientsData(vector <sClient> vClients)
{
	cout << "\n" << Taps(4) << "Clients List (" << vClients.size() << ") Client(s).";

	cout << "\n_______________________________________________________";
	cout << "_________________________________________________\n" << endl;
	cout << "| ";
	SetWidthInCenter(15, "AccountNumber");
	cout << "| ";
	SetWidthInCenter(10, "PinCode");
	cout << "| ";
	SetWidthInCenter(40, "ClientName");
	cout << "| ";
	SetWidthInCenter(12, "Phone");
	cout << "| ";
	SetWidthInCenter(15, "Balance");
	cout << "\n_______________________________________________________";
	cout << "_________________________________________________\n" << endl;


	for (sClient Client : vClients)
	{
		PrintClientRecordinOneLine(Client);
		cout << endl;
	}


	cout << "\n_______________________________________________________";
	cout << "_________________________________________________\n" << endl;
}

bool FindClientByAccountNumber( vector <sClient> & vClients, sClient& Client, string AccountNumber);

sClient ChangeClientRecord(string AccountNumber);

void AddNewClinet( vector <sClient> & vClients)
{
	sClient Client;
	string AccountNumber = ReadClientAccountNumber();

	while (FindClientByAccountNumber(vClients, Client, AccountNumber))
	{
		cout << "Client With [" << AccountNumber << "] Already exists, Enter Another Account Number? ";
		cin >> AccountNumber;
	}

	Client = ChangeClientRecord(AccountNumber);
	vClients.push_back(Client);
	AddDataLineToFile(ClientsFile, ConvertRecordToLine(Client));
}

bool FindClientByAccountNumber(vector <sClient> & vClients, sClient& Client, string AccountNumber)
{
	for (int i = 0; i < vClients.size(); i++)
	{
		if (AccountNumber == vClients[i].AccountNumber)
		{
			Client = vClients[i];
			return true;
		}
	}

	return false;
}

sClient ChangeClientRecord(string AccountNumber)
{
	sClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "enter Pincode? ";
	getline(cin >> ws, Client.PinCode);

	cout << "enter Name?";
	getline(cin, Client.Name);

	cout << "enter phone?";
	getline(cin, Client.Phone);

	cout << "enter Accountbalance?";
	cin >> Client.AccountBalance;

	return Client;
}

void SaveClientsDataToFile(string FileName, vector <sClient> vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);

	if (MyFile.is_open())
	{
		for (sClient C : vClients)
		{
			string DataLine = ConvertRecordToLine(C, "#//#");
			MyFile << DataLine << endl;
		}
		MyFile.close();
	}
}

void DeleteClientFromFile(vector <sClient> & vClients)
{
	sClient Client ;
	string AccountNumber = ReadClientAccountNumber();
	char Sure = 'N';

	if (FindClientByAccountNumber(vClients,Client,AccountNumber))
	{
		PrintClientRecord(Client);

		cout << "Are you sure you want to delete this Client? Y/N?";
		cin >> Sure;

		if ('Y' == toupper(Sure))
		{
			for (int i = 0; i < vClients.size(); i++)
			{
				if (vClients[i].AccountNumber == AccountNumber)
				{
					vClients.erase(vClients.begin() + i);
					break;
				}
			}
			cout << "Client Deleted Successfully." << endl;

			SaveClientsDataToFile(ClientsFile, vClients);
		}
	}
	else
	{
		cout << "Client With Account Number (" << AccountNumber << ") is NOT Found!" << endl;
	}
}

void GoBackToMainMenue();

enMainMenueOptions ReadMainMenueScreen()
{
	short Number = 0;

	do
	{
		cout << "Chooce What do You Want To Do? [1 to 6]?";
		cin >> Number;
	} while (Number < 1 || Number > 6);

	return (enMainMenueOptions)Number;
}

void ShowAllClientsScreen()
{
	vector <sClient> vClients = LoadDataFromFiles(ClientsFile);

	PrintAllClientsData(vClients);
}

void ShowAddNewClientsScreen()
{
	vector <sClient> vClients = LoadDataFromFiles(ClientsFile);


	system("cls");


	cout << "----------------------------------------" << endl;
	cout << "         Add New Clients Screen         " << endl;
	cout << "----------------------------------------" << endl;

	char AddMore = 'Y';
	sClient Client;

	do
	{

		cout << "Adding New Client:\n\n";

		AddNewClinet(vClients);

		cout << "\nClient Added successfully, do you want to add more client? Y/N?" << endl;
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');

}

void ShowDeleteClientScreen() 
{
	vector <sClient> vClients = LoadDataFromFiles(ClientsFile);


	system("cls");


	cout << "----------------------------------------" << endl;
	cout << "         Delete Client Screen           " << endl;
	cout << "----------------------------------------" << endl;

	char AddMore = 'Y';
	sClient Client;

	DeleteClientFromFile(vClients);
}

void ShowUpdateClientInfoScreen()
{
	vector <sClient> vClients = LoadDataFromFiles(ClientsFile);


	system("cls");

	cout << "----------------------------------------" << endl;
	cout << "       Update Client Info Screen        " << endl;
	cout << "----------------------------------------" << endl;


	sClient Client;
	char Answer = 'n';
	string AccountNumber = ReadClientAccountNumber();

	if (FindClientByAccountNumber(vClients, Client, AccountNumber))
	{
		PrintClientRecord(Client);
		cout << "Are you sure do you want to Update? Y/N?";
		cin >> Answer;

		if ('Y' == toupper(Answer))
		{
			for (sClient& C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					cout << endl;
					C = ChangeClientRecord(AccountNumber);
					cout << "\nClient Update Successfully." << endl;
					break;
				}
			}

			SaveClientsDataToFile(ClientsFile, vClients);
		}
	}
	else
	{
		cout << "Client With Account Number (" << AccountNumber << ") Not Found!" << endl;
	}
}

void ShowFindClientScreen() 
{
	vector <sClient> vClients = LoadDataFromFiles(ClientsFile);


	system("cls");

	cout << "----------------------------------------" << endl;
	cout << "           Find Client Screen           " << endl;
	cout << "----------------------------------------" << endl;


	sClient Client;
	string AccountNumber = ReadClientAccountNumber();

	if (FindClientByAccountNumber(vClients, Client, AccountNumber))
	{
		cout << endl;
		PrintClientRecord(Client);
	}
	else
	{
		cout << "Client With Account Number (" << AccountNumber << ") is NOT Found!" << endl;
	}
}

void ShowEndScreen() 
{
	cout << "\n-----------------------------------\n";
	cout << "          Program Ends :-)           \n";
	cout << "-----------------------------------\n";
}

void PerforMainMenueOption(enMainMenueOptions MainMenueOption)
{
	switch (MainMenueOption)
	{
		case enMainMenueOptions::eClientList:
		system("cls");
		ShowAllClientsScreen();
		GoBackToMainMenue();
		break;

		case enMainMenueOptions::eAddNewClient:
		system("cls");
		ShowAddNewClientsScreen();
		GoBackToMainMenue();
		break;

		case enMainMenueOptions::eDeleteClient:
		system("cls");
		ShowDeleteClientScreen();
		GoBackToMainMenue();
		break;

		case enMainMenueOptions::eUpdateClientInfo:
		system("cls");
		ShowUpdateClientInfoScreen();
		GoBackToMainMenue();
		break;

		case enMainMenueOptions::eFindClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenue();
		break;

		case enMainMenueOptions::eExit:
		system("cls");
		ShowEndScreen();
		break;
	}
}

void ShowMainMenueScreen()
{
	cout << "===========================================================" << endl;
	cout << "                       Main Menue Screen                   " << endl;
	cout << "===========================================================" << endl;
	cout << "            [1] Show Client List.                          " << endl;
	cout << "            [2] Add New Client.                            " << endl;
	cout << "            [3] Delete Client.                             " << endl;
	cout << "            [4] Update Client Info.                        " << endl;
	cout << "            [5] Find Client.                               " << endl;
	cout << "            [6] Exit.                                      " << endl;
	cout << "===========================================================" << endl;

	 PerforMainMenueOption(ReadMainMenueScreen());

}

void GoBackToMainMenue()
{
	cout << "\n\n Prees any key to go back to main menue... ";
	system("pause>0");
	system("cls");
	ShowMainMenueScreen();
}

int main()
{

	ShowMainMenueScreen();

	return 0;
}