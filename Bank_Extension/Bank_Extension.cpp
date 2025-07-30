#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;
const string FileName = "Clients.txt";
const string FileUserName = "User.txt";

void LoginScreen();
void ShowMainMenue();
void TransactionsMenueScreen();
void ShowManageUsers();

enum enMainMenueOptions
{
	eListClients = 1, eAddNewClient = 2,
	eDeleteClient = 3, eUpdateClient = 4,
	eFindClient = 5, eTransactions = 6,
	eManageUsers = 7, eLogout = 8
};

enum enTransactionsMenueOptions
{
	eDeposit = 1, eWithDraw = 2,
	eTotalBalances = 3, eMainMenue_Transactions = 4
};

enum enManageUsers
{
	eListUsers = 1, eAddNewUser = 2,
	eDeleteUser = 3, eUpdateUser = 4,
	eFindUser = 5, eMainMenue_Users = 6
};

enum enMainMenuePermission
{
	eAll = -1, eShowClient = 1, eAddNewClientPermission = 2, eDeleteClientPermission = 4, eUpdateClientPermission = 8,
	eFindClientPermission = 16, eTransactionsPermission = 32, eManageUsersPermission = 64
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

struct stManageUsers
{
	string Name;
	string Password;
	int Permissions = 0;
	bool MarkToDelete = false;
	bool MarkToUpdate = false;
};

void GoBackToMainMenue()
{
	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
	ShowMainMenue();
}

stManageUsers CurrentUser;

void GoBackToTransactionsMenue()
{
	cout << "\n\nPress any key to go back to Transactions Menue...";
	system("pause>0");
	TransactionsMenueScreen();
}

void GoBackToManageUserMenue()
{
	cout << "\n\nPress any key to go back to Manage User Menue...";
	system("pause>0");
	ShowManageUsers();
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

// For Client 

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

// For User 

bool isExistUserNameWithoutUser(string& String, vector <stManageUsers>& vString)
{
	for (stManageUsers& ManageUser : vString)
	{
		if (ManageUser.Name == String)
		{
			cout << "User with [" << ManageUser.Name << "] already exists, Enter another Username? ";
			return true;
		}
	}
	return false;
}

// For Client

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

// For User

bool isExistUsername(string String, vector <stManageUsers>& vString, stManageUsers& User)
{
	for (stManageUsers& ManageUser : vString)
	{
		if (ManageUser.Name == String)
		{
			User = ManageUser;
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

//For Client
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

//For User
stManageUsers ConvertLineToRecordUser(string Line)
{
	stManageUsers ManageUser;
	vector <string> vWords;
	SplitLine(vWords, Line, "#//#");
	ManageUser.Name = vWords[0];
	ManageUser.Password = vWords[1];
	ManageUser.Permissions = stoi(vWords[2]);
	return ManageUser;
}

// For Client

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

// For User

string JoinUserLine(stManageUsers ManageUser, string delim)
{
	string Line = "";
	Line += ManageUser.Name + delim;
	Line += ManageUser.Password + delim;
	Line += to_string(ManageUser.Permissions);
	return Line;
}

//For Client
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


// For User
vector <stManageUsers> LoadDataFileUserToVector(string FileName)
{
	fstream MyFile;
	vector <stManageUsers> vString;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		stManageUsers ManageUser;
		while (getline(MyFile, Line))
		{
			ManageUser = ConvertLineToRecordUser(Line);
			vString.push_back(ManageUser);
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

void DeleateUser(stManageUsers ManageUser, vector <stManageUsers>& vString)
{
	fstream MyFile;

	for (stManageUsers& User : vString)
	{
		if (User.Name == ManageUser.Name)
			User.MarkToDelete = true;
	}

	MyFile.open(FileUserName, ios::out);
	if (MyFile.is_open())
	{
		for (stManageUsers& ManageUser : vString)
		{
			if (ManageUser.MarkToDelete != true)
				MyFile << JoinUserLine(ManageUser, "#//#") << endl;
		}
		cout << "\n\n\nUser Deleted Successfully." << endl << endl;
		MyFile.close();
		vString = LoadDataFileUserToVector(FileUserName);
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

void UserDetails(stManageUsers User)
{
	cout << endl << "The following are the User details:" << endl;
	cout << "___________________________________" << endl;
	cout << "\nUsername     : " << User.Name << endl;
	cout << "Password     : " << User.Password << endl;
	cout << "Permissions  : " << User.Permissions << endl;
	cout << "___________________________________" << endl << endl;
}

// For Client

void ShowClientData(stDataClient Client, vector <stDataClient>& vString)
{
	ClientDetails(Client);

	if (Again("\nAre you Sure you want delete this client? y/n ? ", "Are you Sure you want delete this client? y/n ? "))
		DeleateClient(Client, vString);

}

// For User

void ShowClientData(stManageUsers User, vector <stManageUsers>& vString)
{
	UserDetails(User);

	if (Again("\nAre you Sure you want delete this client? y/n ? ", "Are you Sure you want delete this client? y/n ? "))
		DeleateUser(User, vString);

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
	cout << "                                                    Total Balances = " << count << endl << endl;
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

// Show Users List

void PrintManageUsers(stManageUsers User)
{
	cout << "| " << setw(20) << left << User.Name;
	cout << "| " << setw(40) << left << User.Password;
	cout << "| " << setw(40) << left << to_string(User.Permissions) << endl;
}

void ShowUsersList()
{
	vector <stManageUsers> vString = LoadDataFileUserToVector(FileUserName);
	cout << endl << "                                Users List (" << vString.size() << ") User(s).                         " << endl;
	cout << "_______________________________________________________________________________________________" << endl << endl;
	cout << "| " << setw(20) << left << "User Name";
	cout << "| " << setw(40) << left << "Password";
	cout << "| " << setw(40) << left << "Permissions" << endl;
	cout << "_______________________________________________________________________________________________" << endl << endl;
	for (stManageUsers& User : vString)
		PrintManageUsers(User);
	cout << endl;
	cout << "_______________________________________________________________________________________________" << endl << endl << endl;
}

// Add New User

string ConvertDecimalToBinary(int Number)
{
	if (Number == -1)
		return "-1";
	string Binary = "";
	while (Number > 0)
	{
		Binary += to_string(Number % 2);
		Number /= 2;
	}
	return Binary;
}

int NumberOfPermissions()
{
	int Permission = 0;
	cout << endl << "Do you want to give access to :" << endl << endl;
	if (Again("\nShow Client List? y/n? ", "Show Client List? y/n? "))
		Permission += enMainMenuePermission::eShowClient;
	if (Again("\nAdd New Client? y/n? ", "Add New Client? y/n? "))
		Permission += enMainMenuePermission::eAddNewClientPermission;
	if (Again("\nDelete Client? y/n? ", "Delete Client? y/n? "))
		Permission += enMainMenuePermission::eDeleteClientPermission;
	if (Again("\nUpdate Client? y/n? ", "Update Client? y/n? "))
		Permission += enMainMenuePermission::eUpdateClientPermission;
	if (Again("\nFind Client? y/n? ", "Find Client? y/n? "))
		Permission += enMainMenuePermission::eFindClientPermission;
	if (Again("\nTransactions? y/n? ", "Transactions? y/n? "))
		Permission += enMainMenuePermission::eTransactionsPermission;
	if (Again("\nManage User? y/n? ", "Manage User? y/n? "))
		Permission += enMainMenuePermission::eManageUsersPermission;
	return Permission;
}

stManageUsers FillDataUser(stManageUsers& ManageUser)
{
	cout << "Enter Password? ";
	getline(cin >> ws, ManageUser.Password);

	if (Again("Do you want to give full access? y/n? ", "Do you want to give full access? y/n? "))
		ManageUser.Permissions = -1;
	else
		ManageUser.Permissions = NumberOfPermissions();
	return ManageUser;
}

void AddUserInFile(stManageUsers ManageUser, vector <stManageUsers>& vString)
{
	fstream MyFile;
	MyFile.open(FileUserName, ios::out | ios::app);
	if (MyFile.is_open())
	{
		MyFile << JoinUserLine(ManageUser, "#//#") << endl;
		MyFile.close();
		vString = LoadDataFileUserToVector(FileUserName);
	}
}

void AddNewUsers()
{
	vector <stManageUsers> vString = LoadDataFileUserToVector(FileUserName);
	stManageUsers ManageUser, User;
	string Answer = "y";
	BarreShow("Add  New User Screen");
	cout << "Adding New User:";
	do
	{
		cout << endl << endl;
		cout << "Enter Username? ";
		do
		{
			getline(cin >> ws, ManageUser.Name);
		} while (isExistUserNameWithoutUser(ManageUser.Name, vString));
		AddUserInFile(FillDataUser(ManageUser), vString);
	} while (Again("\nUser Added Successfully, do you want to add more Users ? Y/N ? ", "\nDo you want to add more Users? Y/N? "));
}

// Delete User

void DeleteUser()
{
	vector <stManageUsers> vString = LoadDataFileUserToVector(FileUserName);
	stManageUsers ManageUser, User;
	string Sure = "y";
	BarreShow("Delete User Screen");
	cout << "Please enter Username? ";
	getline(cin >> ws, ManageUser.Name);
	if (!isExistUsername(ManageUser.Name, vString, User))
		cout << "\nThis Acoount (" << ManageUser.Name << ") is NOT Found ! " << endl << endl << endl;
	else
		ShowClientData(User, vString);
}

// Update User

void UpdateUser(stManageUsers ManageUser, vector <stManageUsers>& vString, stManageUsers User)
{
	fstream MyFile;
	for (stManageUsers& UpdUser : vString)
	{
		if (UpdUser.Name == User.Name)
			UpdUser.MarkToUpdate = true;
	}

	MyFile.open(FileUserName, ios::out);
	if (MyFile.is_open())
	{
		for (stManageUsers& ManageUsers : vString)
		{
			if (ManageUsers.MarkToUpdate != true)
				MyFile << JoinUserLine(ManageUsers, "#//#") << endl;
			else
				MyFile << JoinUserLine(ManageUser, "#//#") << endl;
		}
		MyFile.close();
		vString = LoadDataFileUserToVector(FileUserName);
		cout << "\n\n\nClient Updated Successfully." << endl << endl;
	}
}

void UpdateUserInfo()
{
	vector <stManageUsers> vString = LoadDataFileUserToVector(FileUserName);
	stManageUsers ManageUser, User;
	string Answer = "y";
	BarreShow("Update New Users Screen");
	cout << "Please enter Username? ";
	getline(cin >> ws, ManageUser.Name);
	if (isExistUsername(ManageUser.Name, vString, User))
	{
		UserDetails(User);
		if (Again("\nAre you Sure you want update this client? y/n ? ", "Are you Sure you want update this client? y/n ? "))
		{
			cout << endl << endl << endl;
			UpdateUser(FillDataUser(ManageUser), vString, User);
		}
		cout << endl << endl;
	}
	else
		cout << "\nThis Acoount (" << ManageUser.Name << ") is NOT Found ! " << endl << endl << endl;
}

// Find User

void FindUser()
{
	vector <stManageUsers> vString = LoadDataFileUserToVector(FileUserName);
	stManageUsers ManageUser, User;
	BarreShow("Find Client Screen");
	cout << "Please enter Username? ";
	getline(cin >> ws, ManageUser.Name);
	if (isExistUsername(ManageUser.Name, vString, User))
	{
		UserDetails(User);
		cout << endl << endl;
	}
	else
		cout << endl << endl << "This Acoount (" << ManageUser.Name << ") is NOT Found ! " << endl << endl << endl;
}

void SelectManageUsersMenuOption(enManageUsers ManageUsers)
{
	cout << endl << endl << endl;
	switch (ManageUsers)
	{
	case enManageUsers::eListUsers:
		system("cls");
		ShowUsersList();
		GoBackToManageUserMenue();
		break;
	case enManageUsers::eAddNewUser:
		system("cls");
		AddNewUsers();
		GoBackToManageUserMenue();
		break;
	case enManageUsers::eDeleteUser:
		system("cls");
		DeleteUser();
		GoBackToManageUserMenue();
		break;
	case enManageUsers::eUpdateUser:
		system("cls");
		UpdateUserInfo();
		GoBackToManageUserMenue();
		break;
	case enManageUsers::eFindUser:
		system("cls");
		FindUser();
		GoBackToManageUserMenue();
		break;
	default:
		GoBackToMainMenue();
	}
}

void ShowManageUsers()
{
	system("cls");
	cout << "=================================================" << endl;
	cout << "            Manage Users Menue Screen            " << endl;
	cout << "=================================================" << endl;
	cout << "           [1] List Users.                 " << endl;
	cout << "           [2] Add New User.                   " << endl;
	cout << "           [3] Delete User.                    " << endl;
	cout << "           [4] Update User.               " << endl;
	cout << "           [5] Find User.                    " << endl;
	cout << "           [6] Main Menue.               " << endl;
	cout << "=================================================" << endl;
	SelectManageUsersMenuOption((enManageUsers)ReadNumber("Choose what do you want to do? [1 to 6]? ", 1, 6));
}

void AccessDenied()
{
	system("cls");
	cout << "_________________________________________________" << endl;
	cout << "Access Denied," << endl;
	cout << "You don't Have Permission To Do this," << endl;
	cout << "Please Contact Your Admin." << endl;
	cout << "-------------------------------------------------" << endl << endl << endl;
	GoBackToMainMenue();
}

bool CheckPermissions(enMainMenuePermission Permission)
{
	if (Permission == enMainMenuePermission::eAll)
		return true;
	if ((Permission & CurrentUser.Permissions) == Permission)
		return true;
	return false;
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
	if (!CheckPermissions(enMainMenuePermission::eShowClient))
	{
		AccessDenied();
		return;
	}


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
	if (!CheckPermissions(enMainMenuePermission::eAddNewClientPermission))
	{
		AccessDenied();
		return;
	}

	vector <stDataClient> vString = LoadDataFileToVector(FileName);
	stDataClient DataClient;
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
	if (!CheckPermissions(enMainMenuePermission::eDeleteClientPermission))
	{
		AccessDenied();
		return;
	}

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
	if (!CheckPermissions(enMainMenuePermission::eUpdateClientPermission))
	{
		AccessDenied();
		return;
	}
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
	if (!CheckPermissions(enMainMenuePermission::eFindClientPermission))
	{
		AccessDenied();
		return;
	}
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
		cout << endl << endl << "This Acoount (" << DataClient.AccountNum << ") is NOT Found ! " << endl << endl << endl;
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
		GoBackToMainMenue();
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
	case enMainMenueOptions::eManageUsers:
		ShowManageUsers();
		break;
	default:
		system("cls");
		LoginScreen();
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
	cout << "           [7] Manage Users.                      " << endl;
	cout << "           [8] Logout.                             " << endl;
	cout << "=================================================" << endl;
	SelectMainMenuOption((enMainMenueOptions)ReadNumber("Choose what do you want to do? [1 to 8]? ", 1, 8));
}

void LoginScreen()
{
	vector <stManageUsers> vString = LoadDataFileUserToVector(FileUserName);
	stManageUsers User;
	BarreShow("Login Screen");
	string Username, Password;
	cout << "Enter Username? ";
	getline(cin >> ws, Username);
	Password = ReadString("Enter Password? ");
	while (!isExistUsername(Username, vString, User) || !(User.Password == Password))
	{
		system("cls");
		BarreShow("Login Screen");
		cout << "Invalid Username/Password!" << endl;
		cout << "Enter Username? ";
		getline(cin >> ws, Username);
		Password = ReadString("Enter Password? ");
	}

	CurrentUser = User;
	ShowMainMenue();
}

int main()
{
	LoginScreen();
}