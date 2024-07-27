#include <iostream>
#include <vector>
#include <fstream>
#include <string>


using namespace std;

void Login();
void ShowATMMainMenueScreen();

string ClientsFileName = "Clients.txt";

enum enATMMainMenueOptions
{
	QuickWIthdraw = 1, NormalWithdraw = 2, Deposit = 3,
	CheckBalance = 4, Logout = 5
};

enum enQuickWithdraw
{
	Twenty = 1, Fifty = 2, Hundred = 3,
	TwoHundred = 4, FourHundred = 5, SixHundred = 6,
	EightHundred = 7, OneThousand = 8
};

int arr[9] = { 0,20,50,100,200,400,600,800,1000 };

struct sClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};

sClient CurrentClient;

vector<string> SplitString(string S1, string Delim)
{
	vector<string> vString;
	short pos = 0;
	string sWord; // define a string variable  

	// use find() function to get the position of the delimiters  
	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, pos); // store the word   
		if (sWord != "")
		{
			vString.push_back(sWord);
		}

		S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
	}

	if (S1 != "")
	{
		vString.push_back(S1); // it adds last word of the string.
	}

	return vString;

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
	sClient Client;
	vector<string> vClientData;
	vClientData = SplitString(Line, Seperator);

	Client.AccountNumber = vClientData[0];
	Client.PinCode = vClientData[1];
	Client.Name = vClientData[2];
	Client.Phone = vClientData[3];
	Client.AccountBalance = stod(vClientData[4]);//cast string to double
	return Client;
}

vector <sClient> LoadClientsDataFromFile(string FileName)
{
	vector <sClient> vClients;
	fstream Clients;
	Clients.open(FileName, ios::in);//read Mode

	if (Clients.is_open())
	{
		string Line;
		sClient Client;

		while (getline(Clients, Line))
		{
			Client = ConvertLinetoRecord(Line);
			vClients.push_back(Client);
		}
		Clients.close();
	}
	return vClients;
}

bool FindClientByAccountNumberAndPincode(string AccountNumber, string PinCode, sClient& CurrentClient)
{

	vector <sClient> vUsers = LoadClientsDataFromFile(ClientsFileName);

	for (sClient C : vUsers)
	{

		if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
		{
			CurrentClient = C;

			return true;
		}

	}
	return false;

}

bool  LoadClientInfo(string AccountNumber, string PinCode)
{

	if (FindClientByAccountNumberAndPincode(AccountNumber, PinCode, CurrentClient))
		return true;
	else
		return false;
}

string ConvertRecordToLine(sClient &Client, string Seperator = "#//#")
{

	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.AccountBalance);
	return stClientRecord;
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> &vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);//overwrite

	string DataLine;

	if (MyFile.is_open())
	{
		for (sClient &C : vClients)
		{
	
			if (C.MarkForDelete == false )
			{
				if (C.Name == CurrentClient.Name)
				{
					C.AccountBalance = CurrentClient.AccountBalance;
				}

				//we only write records that are not marked for delete.  
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}

		}

		MyFile.close();
	}

	return vClients;
}

void GoBackToATMMainMenue()
{
	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
	ShowATMMainMenueScreen();
}

void QuickWithdraw(enQuickWithdraw Withdraw)
{
	vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	switch (Withdraw)
	{
	case Twenty:
		CurrentClient.AccountBalance -= 20;
		break;
	case Fifty:
		CurrentClient.AccountBalance -= 50;
		break;
	case Hundred:
		CurrentClient.AccountBalance -= 100;
		break;
	case TwoHundred:
		CurrentClient.AccountBalance -= 200;
		break;
	case FourHundred:
		CurrentClient.AccountBalance -= 400;
		break;
	case SixHundred:
		CurrentClient.AccountBalance -= 600;
		break;
	case EightHundred:
		CurrentClient.AccountBalance -= 800;
		break;
	case OneThousand:
		CurrentClient.AccountBalance -= 1000;
		break;
	}

	SaveCleintsDataToFile(ClientsFileName, vClients);

	cout << "Done Sucessfully, New Balance is: " << CurrentClient.AccountBalance << endl;
}

void NormalWirhdraw() 
{
	int Withdraw;
	char Answer;
	vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);	
	do
	{
		cout << "\nEnter an amount multiple of 5's ? ";
		cin >> Withdraw;

	}while (Withdraw % 5 != 0 && Withdraw < 5);

	while (Withdraw > CurrentClient.AccountBalance)
	{
		cout << "The amount exceeds your balance, make another choice.\n";
		cout << "Press AnyKey to continue...\n";
		system("pause > 0");
		system("cls");
		cout << "==============================================\n";
		cout << "\t\tNormal Withdraw Screen\n";
		cout << "==============================================\n";
		cout << "\nEnter an amount multiple of 5's ? ";
		cin >> Withdraw;
	}

	cout << "Are you sure you want perform this transation ? y/n ? ";
	cin >> Answer;

	if (tolower(Answer) == 'y')
	{
		CurrentClient.AccountBalance -= Withdraw;
		SaveCleintsDataToFile(ClientsFileName, vClients);
		cout << "Done Successfully, New Balance is: " << CurrentClient.AccountBalance << endl;
	}
}

void DepositCheck()
{
	int Amount;
	char Answer;
	vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	cout << "\nEnter a positive Deposit Amount? ";
	cin >> Amount;

	if (Amount >= 0)
	{
		cout << "Are you sure you want perform this transaction? y/n ? ";
		cin >> Answer;
		if (tolower(Answer) == 'y')
		{
			CurrentClient.AccountBalance += Amount;
			SaveCleintsDataToFile(ClientsFileName, vClients);
			cout << "Done Successfully, New Balance is: " << CurrentClient.AccountBalance << endl;
		}
	}
	
}

void ShowQuickWihdrawScreen()
{
	cout << "==============================================\n";
	cout << "\t\tQuick Withdraw\n";
	cout << "==============================================\n";
	cout << "\t [1] 20\t\t [2] 50\n";
	cout << "\t [3] 100\t [4] 200\n";
	cout << "\t [5] 400\t [6] 600\n";
	cout << "\t [7] 800\t [8] 1000\n";
	cout << "\t [9] Exit\n";
	cout << "==============================================\n";
	cout << "Your Balance is " << CurrentClient.AccountBalance << endl;

	short Choice;
	cout << "Choose what to withdraw from[1] to [8] ? ";
	cin >> Choice;

	while (arr[Choice] > CurrentClient.AccountBalance)
	{
		cout << "The amount exceeds your balance, make another choice.\n";
		cout << "Press AnyKey to continue...\n";
		system("pause > 0");

		system("cls");
		cout << "==============================================\n";
		cout << "\t\Quick Withdraw\n";
		cout << "==============================================\n";
		cout << "\t [1] 20\t\t [2] 50\n";
		cout << "\t [3] 100\t [4] 200\n";
		cout << "\t [5] 400\t [6] 600\n";
		cout << "\t [7] 800\t [8] 1000\n";
		cout << "\t [9] Exit\n";
		cout << "==============================================\n";
		cout << "Your Balance is " << CurrentClient.AccountBalance << endl;

		cout << "Choose what to withdraw from[1] to [8] ? ";
		cin >> Choice;
	}

	if (Choice != 9)
	{
		char Answer;
		cout << "Are you sure you want perform this transaction? y/n ? ";
		cin >> Answer;
		if (tolower(Answer) == 'y')
			QuickWithdraw(enQuickWithdraw(Choice));
	}
}

void ShowNormalWithdrawScreen()
{
   cout << "==============================================\n";
   cout << "\t\tNormal Withdraw Screen\n";
   cout << "==============================================\n";
   NormalWirhdraw();
}

void ShowDepositScreen() 
{
	cout << "==============================================\n";
	cout << "\t\tDeposit Screen\n";
	cout << "==============================================\n";
	DepositCheck();
}

void ShowCheckBalabnceScreen()
{
	cout << "==============================================\n";
	cout << "\t\tCheck Balance Screen\n";
	cout << "==============================================\n";
	cout << "Your Balance is " << CurrentClient.AccountBalance << endl;
}

void PerformATMMainMenueOption(enATMMainMenueOptions ATMMainMenueOption)
{
	switch (ATMMainMenueOption)
	{
	case QuickWIthdraw:
		system("cls");
		ShowQuickWihdrawScreen();
		GoBackToATMMainMenue();
		break;
	case NormalWithdraw:
		system("cls");
		ShowNormalWithdrawScreen();
		GoBackToATMMainMenue();
		break;
	case Deposit:
		system("cls");
		ShowDepositScreen();
		GoBackToATMMainMenue();
		break;
	case CheckBalance:
		system("cls");
		ShowCheckBalabnceScreen();
		GoBackToATMMainMenue();
		break;
	case Logout:
		system("cls");
		Login();
		break;
	}
}

void ShowATMMainMenueScreen()
{
	system("cls");

	cout << "==============================================\n";
	cout << "\t\tATM Main Menue Screen\n";
	cout << "==============================================\n";
	cout << "\t[1] Quick Withdraw.\n";
	cout << "\t[2] Normal Withdraw.\n";
	cout << "\t[3] Deposit.\n";
	cout << "\t[4] Check Balance.\n";
	cout << "\t[5] Logout.\n";
	cout << "==============================================\n";

	short Choice;
	cout << "Choose what do you want to do? [1 to 5]? ";
	cin >> Choice;

	PerformATMMainMenueOption(enATMMainMenueOptions(Choice));
};

void Login()
{
	bool LoginFaild = false;

	string AccountNumber, PinCode;

	do
	{
		system("cls");

		cout << "\n---------------------------------\n";
		cout << "\tLogin Screen";
		cout << "\n---------------------------------\n";

		if (LoginFaild)
		{
			cout << "Invalid Account Number/Pincode!" << endl;
		} 

		cout << "Enter Account Number? ";
		cin >> AccountNumber;

		cout << "Enter PinCode? ";
		cin >> PinCode;

		LoginFaild = !LoadClientInfo(AccountNumber, PinCode);

	} while (LoginFaild);

	ShowATMMainMenueScreen();
}

int main()
{
	
	Login();

	system("pause > 0");
	return 0;

}

