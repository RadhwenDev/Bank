#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;
const string FileName = "MyFile.txt";

void ShowMainMenue();
void TransactionsMenueScreen();

enum enMainMenueOptions 
{
	eListClients = 1, eAddNewClient = 2,
	eDeleteClient = 3, eUpdateClient = 4,
	eFindClient = 5, eTransactions = 6,
	eExit = 7
};

enum enTransactionsMenueOptions
{
	eDeposit = 1, eWithDraw = 2,
	eTotalBalances = 3, eMainMenue = 4
};

struct stDataClient
{
	string AccountNum;
	string PinCode;
	string Name;
	string Phone;
	float AccountBalance;
	bool MarkToDelete = false;
	bool MarkToUpdate = false;
	bool DepositWithDarwAmount = false;
};

void GoBackToMainMenue()
{
	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
	ShowMainMenue();
}

void GoBackToTransactionsMenue()
{
	cout << "\n\nPress any key to go back to Transactions Menue...";
	system("pause>0");
	TransactionsMenueScreen();
}

void BarreShow(string String)
{
	cout << "____________________________________________" << endl;
	cout << endl << "            " << String << endl;
	cout << "____________________________________________" << endl << endl;
}

int ReadNumber(string Message, int From, int To)
{
	int Num = 0;
	do
	{
		cout << Message;
		cin >> Num;
	} while (Num < From || Num > To);
	return Num;
}

string ReadString(string Message)
{
	string String;
	cout << Message;
	getline(cin, String);
	return String;
}

stDataClient FillDataClient(stDataClient& DataClient)
{
	cout << "Enter Pin Code? ";
	getline(cin >> ws, DataClient.PinCode);
	DataClient.Name = ReadString("Enter Name? ");
	DataClient.Phone = ReadString("Enter Phone? ");
	cout << "Enter Account Balance? ";
	cin >> DataClient.AccountBalance;
	return DataClient;
}

bool Again(string Message1, string Message2)
{
	string Answer = "Y";
	cout << Message1;
	getline(cin >> ws, Answer);
	while (Answer != "Y" && Answer != "y" && Answer != "n" && Answer != "N")
	{
		Answer = ReadString(Message2);
	}
	if (Answer == "Y" || Answer == "y")
		return true;
	return false;
}

bool isExistAccountNumberWithoutClient(string& String, vector <stDataClient>& vString)
{
	for (stDataClient& DataClient : vString)
	{
		if (DataClient.AccountNum == String)
		{
			cout << "Client with [" << DataClient.AccountNum << "] already exists, Enter another Account Number? ";
			return true;
		}
	}
	return false;
}

bool isExistAccountNumber(string String, vector <stDataClient>& vString, stDataClient& Client)
{
	for (stDataClient& DataClient : vString)
	{
		if (DataClient.AccountNum == String)
		{
			Client = DataClient;
			return true;
		}
	}
	return false;
}

vector <string> SplitLine(vector <string>& vWords, string Line, string delim)
{
	int pos = 0;
	string word = "";
	while ((pos = Line.find(delim)) != Line.npos)
	{
		word = Line.substr(0, pos);
		if (word != "")
			vWords.push_back(word);
		Line.erase(0, pos + delim.length());
	}
	if (Line != "")
		vWords.push_back(Line);
	return vWords;
}

stDataClient ConvertLineToRecord(string Line)
{
	stDataClient DataClient;
	vector <string> vWords;
	SplitLine(vWords, Line, "#//#");

	DataClient.AccountNum = vWords[0];
	DataClient.PinCode = vWords[1];
	DataClient.Name = vWords[2];
	DataClient.Phone = vWords[3];
	DataClient.AccountBalance = stof(vWords[4]);
	return DataClient;
}

string JoinLine(stDataClient DataClient, string delim)
{
	string Line = "";
	Line += DataClient.AccountNum + delim;
	Line += DataClient.PinCode + delim;
	Line += DataClient.Name + delim;
	Line += DataClient.Phone + delim;
	Line += to_string(DataClient.AccountBalance);
	return Line;
}

vector <stDataClient> LoadDataFileToVector(string FileName)
{
	fstream MyFile;
	vector <stDataClient> vString;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		stDataClient DataClient;
		while (getline(MyFile, Line))
		{
			DataClient = ConvertLineToRecord(Line);
			vString.push_back(DataClient);
		}
		MyFile.close();
	}
	return vString;
}

void DeleateClient(stDataClient DataClient, vector <stDataClient>& vString)
{
	fstream MyFile;

	for (stDataClient& Client : vString)
	{
		if (Client.AccountNum == DataClient.AccountNum)
			Client.MarkToDelete = true;
	}

	MyFile.open(FileName, ios::out);
	if (MyFile.is_open())
	{
		for (stDataClient& DataClient : vString)
		{
			if (DataClient.MarkToDelete != true)
				MyFile << JoinLine(DataClient, "#//#") << endl;
		}
		cout << "\n\n\nClient Deleted Successfully." << endl << endl;
		MyFile.close();
		vString = LoadDataFileToVector(FileName);
	}
}

void ClientDetails(stDataClient Client)
{
	cout << endl << "The following are the client details:" << endl;
	cout << "___________________________________" << endl;
	cout << "\nAccount Number : " << Client.AccountNum << endl;
	cout << "Pin Code       : " << Client.PinCode << endl;
	cout << "Name           : " << Client.Name << endl;
	cout << "Phone          : " << Client.Phone << endl;
	cout << "Account Balanc : " << Client.AccountBalance << endl;
	cout << "___________________________________" << endl << endl;
}

void ShowClientData(stDataClient Client, vector <stDataClient>& vString)
{
	ClientDetails(Client);

	if (Again("\nAre you Sure you want delete this client? y/n ? ", "Are you Sure you want delete this client? y/n ? "))
		DeleateClient(Client, vString);

}

void AddClientInFile(stDataClient DataClient, vector <stDataClient>& vString)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open())
	{
		MyFile << JoinLine(DataClient, "#//#") << endl;
		MyFile.close();
		vString = LoadDataFileToVector(FileName);
	}
}

void UpdateClient(stDataClient DataClient, vector <stDataClient>& vString, stDataClient Client)
{
	fstream MyFile;
	for (stDataClient& UpdCli : vString)
	{
		if (UpdCli.AccountNum == Client.AccountNum)
			UpdCli.MarkToUpdate = true;
	}

	MyFile.open(FileName, ios::out);
	if (MyFile.is_open())
	{
		for (stDataClient& DataClients : vString)
		{
			if (DataClients.MarkToUpdate != true)
				MyFile << JoinLine(DataClients, "#//#") << endl;
			else
				MyFile << JoinLine(DataClient, "#//#") << endl;
		}
		cout << "\n\n\nClient Updated Successfully." << endl << endl;
		MyFile.close();
		vString = LoadDataFileToVector(FileName);
	}
}

// [1] Show Client List.

void PrintDataClients(stDataClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNum;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << to_string((int)Client.AccountBalance) << endl;
}

void ShowClientList()
{
	vector <stDataClient> vString = LoadDataFileToVector(FileName);
	cout << endl << "                                Client List (" << vString.size() << ") Client(s).                         " << endl;
	cout << "_______________________________________________________________________________________________" << endl << endl;
	cout << "| " << setw(15) << left << "Account Number";
	cout << "| " << setw(10) << left << "Pin Code";
	cout << "| " << setw(40) << left << "Client Name";
	cout << "| " << setw(12) << left << "Phone";
	cout << "| " << setw(12) << left << "Balance" << endl;
	cout << "_______________________________________________________________________________________________" << endl << endl;
	for (stDataClient& Client : vString)
		PrintDataClients(Client);
	cout << endl;
	cout << "_______________________________________________________________________________________________" << endl << endl << endl;
}

// [2] Add New Client.

void AddNewClients()
{
	vector <stDataClient> vString = LoadDataFileToVector(FileName);
	stDataClient DataClient, Client;
	string Answer = "y";
	BarreShow("Add  New Clients Screen");
	cout << "Adding New Client:";
	do
	{
		cout << endl << endl;
		cout << "Enter Account Number? ";
		do
		{
			getline(cin >> ws, DataClient.AccountNum);
		} while (isExistAccountNumberWithoutClient(DataClient.AccountNum, vString));
		AddClientInFile(FillDataClient(DataClient), vString);
	} while (Again("\nClient Added Successfully, do you want to add more clients? Y/N? ", "\nDo you want to add more clients? Y/N? "));
}

// [3] Delete Client.

void DeleteClient()
{
	vector <stDataClient> vString = LoadDataFileToVector(FileName);
	stDataClient DataClient, Client;
	string Sure = "y";
	BarreShow("Delete Client Screen");
	cout << "Please enter Account Number? ";
	getline(cin >> ws, DataClient.AccountNum);
	if (!isExistAccountNumber(DataClient.AccountNum, vString, Client))
	{
		cout << "\nThis Acoount (" << DataClient.AccountNum << ") is NOT Found ! " << endl << endl << endl;
	}
	else
	{
		ShowClientData(Client, vString);
	}

}

// [4] Update Client Info.

void UpdateClientInfo()
{
	vector <stDataClient> vString = LoadDataFileToVector(FileName);
	stDataClient DataClient, Client;
	string Answer = "y";
	BarreShow("Update New Clients Screen");
	cout << "Please enter Account Number? ";
	getline(cin >> ws, DataClient.AccountNum);
	if (isExistAccountNumber(DataClient.AccountNum, vString, Client))
	{
		ClientDetails(Client);
		if (Again("\nAre you Sure you want update this client? y/n ? ", "Are you Sure you want update this client? y/n ? "))
		{
			cout << endl << endl << endl;
			UpdateClient(FillDataClient(DataClient), vString, Client);
		}
		cout << endl << endl;
	}
	else
	{
		cout << "\nThis Acoount (" << DataClient.AccountNum << ") is NOT Found ! " << endl << endl << endl;
	}
}


// [5] Find Client.

void FindClient()
{
	vector <stDataClient> vString = LoadDataFileToVector(FileName);
	stDataClient DataClient, Client;
	BarreShow("Find Client Screen");
	cout << "Please enter Account Number? ";
	getline(cin >> ws, DataClient.AccountNum);
	if (isExistAccountNumber(DataClient.AccountNum, vString, Client))
	{
		ClientDetails(Client);
		cout << endl << endl;
	}
	else
	{
		cout << endl << endl << "This Acoount (" << DataClient.AccountNum << ") is NOT Found ! " << endl << endl << endl;
		//BackToChoices(vString);
	}
}

// [6] Exit.

void Exit()
{
	BarreShow("Program Ends :-)");
	system(0);
}

void DepositWithDrawAmount(float AmountDeposit, stDataClient Client)
{
	vector <stDataClient> vString = LoadDataFileToVector(FileName);
	fstream MyFile;
	float NewAccountBalance = 0;
	for (stDataClient& DataClients : vString)
	{
		if (DataClients.AccountNum == Client.AccountNum)
		{
			DataClients.DepositWithDarwAmount = true;
			break;
		}
	}
	MyFile.open(FileName, ios::out);
	if (MyFile.is_open())
	{
		string Line;
		for (stDataClient& DataClient : vString)
		{
			if (DataClient.DepositWithDarwAmount != true)
				MyFile << JoinLine(DataClient, "#//#") << endl;
			else
			{
				DataClient.AccountBalance += AmountDeposit;
				NewAccountBalance = DataClient.AccountBalance;
				MyFile << JoinLine(DataClient, "#//#") << endl;
			}
		}
		MyFile.close();
		cout << endl << endl << endl << "Done Successfully New Balance = " << NewAccountBalance << endl << endl;
	}
}

void DepositAmount()
{
	vector <stDataClient> vString = LoadDataFileToVector(FileName);
	stDataClient DataClient, Client;
	float AmountDeposit = 0;
	BarreShow("Deposit Screen");
	cout << "Please enter Account Number? ";
	getline(cin >> ws, DataClient.AccountNum);
	while (!isExistAccountNumber(DataClient.AccountNum, vString, Client))
	{
		cout << "\nThis Acoount (" << DataClient.AccountNum << ") is NOT Found ! " << endl << endl << endl;
		cout << "Please enter Account Number? ";
		getline(cin >> ws, DataClient.AccountNum);
	}
	ClientDetails(Client);
	cout << "Please enter deposit amount? ";
	cin >> AmountDeposit;
	cout << endl << endl << endl;
	if (Again("Are you sure you want perform this transaction? y/n ? ", "Are you sure you want perform this transaction? y/n ? "))
	{
		DepositWithDrawAmount(AmountDeposit, Client);
		GoBackToTransactionsMenue();
	}
	else
		GoBackToTransactionsMenue();
}

void WithDrawAmount()
{
	vector <stDataClient> vString = LoadDataFileToVector(FileName);
	stDataClient DataClient, Client;
	float AmountWithDraw = 0;
	BarreShow("Withdraw Screen");
	cout << "Please enter Account Number? ";
	getline(cin >> ws, DataClient.AccountNum);
	while (!isExistAccountNumber(DataClient.AccountNum, vString, Client))
	{
		cout << "\nThis Acoount (" << DataClient.AccountNum << ") is NOT Found ! " << endl << endl << endl;
		cout << "Please enter Account Number? ";
		getline(cin >> ws, DataClient.AccountNum);
	}
	ClientDetails(Client);
	cout << "Please enter withdraw amount? ";
	cin >> AmountWithDraw;
	while (AmountWithDraw > Client.AccountBalance)
	{
		cout << endl << endl << "Amount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
		cout << "Please enter another amount? ";
		cin >> AmountWithDraw;
	}
	cout << endl << endl << endl;
	if (Again("Are you sure you want perform this transaction? y/n ? ", "Are you sure you want perform this transaction? y/n ? "))
	{
		DepositWithDrawAmount(AmountWithDraw * -1, Client);
		GoBackToTransactionsMenue();
	}
	else
		GoBackToTransactionsMenue();
}

void PrintDataClientsTransactions(stDataClient Client)
{
	cout << "| " << setw(20) << left << Client.AccountNum;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(40) << left << to_string((int)Client.AccountBalance) << endl;
}

void ShowClientListTransactions()
{
	float count = 0;
	vector <stDataClient> vString = LoadDataFileToVector(FileName);
	cout << endl << "                                Client List (" << vString.size() << ") Client(s).                         " << endl;
	cout << "_______________________________________________________________________________________________" << endl << endl;
	cout << "| " << setw(20) << left << "Account Number";
	cout << "| " << setw(40) << left << "Client Name";
	cout << "| " << setw(40) << left << "Balance" << endl;
	cout << "_______________________________________________________________________________________________" << endl << endl;
	for (stDataClient& Client : vString)
	{
		PrintDataClientsTransactions(Client);
		count += Client.AccountBalance;
	}

	cout << endl;
	cout << "_______________________________________________________________________________________________" << endl << endl << endl;
	cout << "                                                    Total Balances = "<< count << endl << endl;
	GoBackToTransactionsMenue();
}

void SelectTransactionsMenuOption(enTransactionsMenueOptions TransactionsMenueOptions)
{
	cout << endl << endl << endl;
	switch (TransactionsMenueOptions)
	{
	case enTransactionsMenueOptions::eDeposit:
		system("cls");
		DepositAmount();
		break;
	case enTransactionsMenueOptions::eWithDraw:
		system("cls");
		WithDrawAmount();
		break;
	case enTransactionsMenueOptions::eTotalBalances:
		system("cls");
		ShowClientListTransactions();
		break;
	default:
		GoBackToMainMenue();
	}
}

void TransactionsMenueScreen()
{
	system("cls");
	cout << "=================================================" << endl;
	cout << "            Transactions Menue Screen            " << endl;
	cout << "=================================================" << endl;
	cout << "           [1] Deposit.                 " << endl;
	cout << "           [2] Withdraw.                   " << endl;
	cout << "           [3] Total Balances.                    " << endl;
	cout << "           [4] Main Menue.               " << endl;
	cout << "=================================================" << endl;
	SelectTransactionsMenuOption((enTransactionsMenueOptions)ReadNumber("Choose what do you want to do? [1 to 4]? ", 1, 4));
}

void SelectMainMenuOption(enMainMenueOptions MainMenueOptions)
{
	cout << endl << endl << endl;
	switch (MainMenueOptions)
	{
	case enMainMenueOptions::eListClients:
		system("cls");
		ShowClientList();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eAddNewClient:
		system("cls");
		AddNewClients();
		GoBackToMainMenue();;
		break;
	case enMainMenueOptions::eDeleteClient:
		system("cls");
		DeleteClient();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eUpdateClient:
		system("cls");
		UpdateClientInfo();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eFindClient:
		system("cls");
		FindClient();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eTransactions:
		TransactionsMenueScreen();
		break;
	default:
		system("cls");
		Exit();
	}
}

void ShowMainMenue()
{
	system("cls");
	cout << "=================================================" << endl;
	cout << "                  Main Menue Screen              " << endl;
	cout << "=================================================" << endl;
	cout << "           [1] Show Client List.                 " << endl;
	cout << "           [2] Add New Client.                   " << endl;
	cout << "           [3] Delete Client.                    " << endl;
	cout << "           [4] Update Client Info.               " << endl;
	cout << "           [5] Find Client.                      " << endl;
	cout << "           [6] Transactions.                      " << endl;
	cout << "           [7] Exit.                             " << endl;
	cout << "=================================================" << endl;
	SelectMainMenuOption((enMainMenueOptions)ReadNumber("Choose what do you want to do? [1 to 7]? ", 1, 7));
}

int main()
{
	ShowMainMenue();
}