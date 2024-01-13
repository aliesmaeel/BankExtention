#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<iomanip>

using namespace std;

enum enMainMenuOptions { Show = 1, Add = 2, Delete = 3, Update = 4, Find = 5, Transactions = 6 , Exit=7 };
enum enTransactionsMenuOptions { Diposit = 1, Withdraw = 2, TotalBalance = 3, MainTransactionsMenu = 4 };

const string ClientsFileName = "Clients.txt";

void ShowMainScreen();
void ShowTransactionsMenu();
void ShowHeaderTransactionScreen(string type);
bool AreYouSure();
bool AreYouSure(string type);


void ClientNotFoundMessage(string AccountNumber) {
    cout << "Client With Account Number [" << AccountNumber << "] Is Not Found  !" << endl;
}

struct sClient {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

vector < string > SplitString(string S1, string Delim) {
    vector < string > vString;
    short pos = 0;
    string sWord;
    while ((pos = S1.find(Delim)) != std::string::npos) {
        sWord = S1.substr(0, pos);
        if (sWord != "") {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }
    if (S1 != "") {
        vString.push_back(S1);
    }
    return vString;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#") {

    sClient Client;
    vector < string > vClientData;
    vClientData = SplitString(Line, Seperator);
    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#") {
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

vector < sClient > LoadCleintsDataFromFile(string FileName) {
    vector < sClient > vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open()) {
        string Line;
        sClient Client;
        while (getline(MyFile, Line)) {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

bool FindClientByAccountNumber(string AccountNumber, vector < sClient > vClients, sClient& Client) {
    for (sClient C : vClients) {
        if (C.AccountNumber == AccountNumber) {
            Client = C;
            return true;
        }
    }
    return false;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector < sClient >& vClients) {
    for (sClient& C : vClients) {
        if (C.AccountNumber == AccountNumber) {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

vector < sClient > SaveCleintsDataToFile(string FileName, vector < sClient > vClients) {

    fstream MyFile;
    MyFile.open(FileName, ios::out);
    string DataLine;
    if (MyFile.is_open()) {
        for (sClient C : vClients) {
            if (C.MarkForDelete == false) {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClients;
}

void PrintClientRecord(sClient Client) {
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(30) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}
void PrintClientRecordBalance(sClient Client) {
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}
void backToHome()
{
    cout << "\nPress Any Key To Go Back To Home Screen  !" << endl;
    system("pause>0");
    ShowMainScreen();
}

void ShowClientsList(string ClientsFileName) {
 
    vector <sClient > vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n________________________________________________________________________________________\n";
    cout << "\n________________________________________________________________________________________\n";
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(30) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n________________________________________________________________________________________\n";
    cout << endl;

    for (sClient Client : vClients) {
        PrintClientRecord(Client);
        cout << endl;
    }
    cout << "\n________________________________________________________________________________________\n";

}

bool CheckAccountNumberExists(string NewAccountNumber)
{
    vector <sClient> Vclients = LoadCleintsDataFromFile(ClientsFileName);
    for (sClient& c : Vclients) {
        if (c.AccountNumber == NewAccountNumber) {
            return true;
        }
    }
    return false;
}

sClient ReadNewClient() {
    sClient Client;
    do {
        cout << "Enter Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    } while (CheckAccountNumberExists(Client.AccountNumber));

    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);
    cout << "Enter Name? ";
    getline(cin, Client.Name);
    cout << "Enter Phone? ";
    getline(cin, Client.Phone);
    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

void AddDataLineToFile(string FileName, string stDataLine) {
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open()) {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}

void ShowClientsHeader(string message) {
    cout << "--------------------------------------------------------------" << endl;
    cout << "\t \t "<< message <<"  \t \t " << endl;
    cout << "--------------------------------------------------------------" << endl;
}
void AddClients() {

    string lineData;
    ShowClientsHeader("Add New Clients To Screen ");
    cout << "Adding New Client :" << endl;
    lineData = ConvertRecordToLine(ReadNewClient(), "#//#");
    AddDataLineToFile(ClientsFileName, lineData);
}

void PrintClientCard(sClient Client) {
    cout << "\nThe following are the client details:\n";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector < sClient >& vClients) {

    sClient Client;
    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {

        PrintClientCard(Client);
       
        if (AreYouSure("Delete")) {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);
            vClients = LoadCleintsDataFromFile(ClientsFileName);
            cout << "\n\nClient Deleted Successfully." << endl;
            return true;
        }
    }
    else {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!" << endl;
        return false;
    }
}

void DeleteClient()
{    
    string accountnumber;
    ShowClientsHeader(" Delete Client Screen  ");
    cout << "Please Enter Account Number " << endl;
    vector <sClient> Vclients = LoadCleintsDataFromFile(ClientsFileName);
    cin >> accountnumber;
    if (CheckAccountNumberExists(accountnumber)) {
        DeleteClientByAccountNumber(accountnumber, Vclients);
    }
    else {
        ClientNotFoundMessage(accountnumber);
    }
}

string ReadClientAccountNumber() {
    string AccountNumber = "";
    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;
}

sClient ReadNewClientWithoutAccount(string accountnumber) {
    sClient Client;

    Client.AccountNumber = accountnumber;
    cout << "Enter PinCode? " << endl;
    getline(cin >> ws, Client.PinCode);
    cout << "Enter Name? " << endl;
    getline(cin, Client.Name);
    cout << "Enter Phone? " << endl;
    getline(cin, Client.Phone);
    cout << "Enter AccountBalance? " << endl;
    cin >> Client.AccountBalance;

    return Client;
}

void UpdateClient() {

    ShowClientsHeader(" Update Client  Screen  ");
    cout << "Please Enter Account Number " << endl;
    char option;
    vector <sClient> VClients = LoadCleintsDataFromFile(ClientsFileName);
    string enteredAccountnumber = ReadClientAccountNumber();
    sClient Client;

    if (FindClientByAccountNumber(enteredAccountnumber, VClients, Client)) {

        PrintClientCard(Client);
        if (AreYouSure("Update"))
        {
            for (sClient& C : VClients)
            {
                if (C.AccountNumber == enteredAccountnumber)
                {
                    C = ReadNewClientWithoutAccount(enteredAccountnumber);
                    break;
                }
            }
            SaveCleintsDataToFile(ClientsFileName, VClients);
        }
    }
    else {
        ClientNotFoundMessage(enteredAccountnumber);
    }
}

void FindClient() {
    string accountnumber;
    sClient Client;
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    ShowClientsHeader(" Find Client Screen  ");
    cout << "Please Enter Account Number " << endl;
    cin >> accountnumber;
    if (FindClientByAccountNumber(accountnumber, vClients, Client)) {
        PrintClientCard(Client);
    }
    else {
        ClientNotFoundMessage(accountnumber);
    }
    cout << endl;
}

void ExitProgram()
{
    exit(0);
}

void MainMenuRouter(enMainMenuOptions  option)
{
    switch (option)
    {
    case enMainMenuOptions::Show:
        system("cls");
         ShowClientsList(ClientsFileName);
         backToHome();
         break;
    case enMainMenuOptions::Add:
        system("cls");
        AddClients();
        backToHome();
        break;
    case enMainMenuOptions::Delete:
        system("cls");
        DeleteClient();
        backToHome();
        break;
    case enMainMenuOptions::Update:
        system("cls");
        UpdateClient();
        backToHome();
        break;
    case enMainMenuOptions::Find:
        system("cls");
        FindClient();
        backToHome();
        break;
    case enMainMenuOptions::Transactions:
        return ShowTransactionsMenu();
    case enMainMenuOptions::Exit:
         system("cls");
         ExitProgram();
         break;
    }
}

int ReadDiposetAmount() {
    int diposit;
    cin >> diposit;
    return diposit;
}
void TransactionsDeposit() {
 
   
   
    ShowHeaderTransactionScreen("Diposit");

    vector <sClient> VClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client ;
   
    if (FindClientByAccountNumber(ReadClientAccountNumber(), VClients, Client)) {
        PrintClientCard(Client);
        cout << "\n\nplease Enter Diposit Amount : \n";
        int diposit = ReadDiposetAmount();
   
        if (AreYouSure()) {

            for (sClient& C : VClients)
            {
                if (C.AccountNumber == Client.AccountNumber)
                {
                    C.AccountBalance += diposit;
                    break;
                }
            }
            SaveCleintsDataToFile(ClientsFileName, VClients);

            cout << "Done Successfully ! \n";
        }
    }

       
}
bool AreYouSure() {
   
    char option;
    cout << "\n Are You Sure You Want To Perform This Action ? Y / N \n";
    cin >> option;
    return (option == 'y' || option == 'Y') ? true : false;
}
bool AreYouSure(string type) {

    char option;
    cout << "\n\nAre You Sure You Want To " << type <<" This Client ? Y / N \n";
    cin >> option;
    return (option == 'y' || option == 'Y') ? true : false;
}
void ShowHeaderTransactionScreen(string type) {
    cout << "--------------------------------------" << endl;
    cout << "\t \t "<< type <<" Screen  \t \t " << endl;
    cout << "---------------------------------------" << endl;
}
void TransactionsWithdraw() {

    ShowHeaderTransactionScreen("Withdraw");
    vector <sClient> VClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;

    if (FindClientByAccountNumber(ReadClientAccountNumber(), VClients, Client)) {
        PrintClientCard(Client);
        cout << "\n\nplease Enter Withdraw Amount : \n";
        int diposit = ReadDiposetAmount();
    
        if (AreYouSure()) {

            for (sClient& C : VClients)
            {
                if (C.AccountNumber == Client.AccountNumber)
                {
                    while (diposit > C.AccountBalance)
                    {
                        cout << " Amount Exceeds the balance , you can withdraw up to " << C.AccountBalance << endl;
                        cout << "please Enter anoter amount " << endl;
                        diposit = ReadDiposetAmount();
                    }
                    C.AccountBalance -= diposit;
                    break;
                }
            }
            SaveCleintsDataToFile(ClientsFileName, VClients);
            cout << "Done Successfully ! \n";
        }
    }
}
int CalculateTotalBalances(vector <sClient> Clients) {
    int sum = 0;
    for (sClient& s : Clients)
    {
        sum += s.AccountBalance;
    }

    return sum;
}
void TotalTransactionsBalance() {
    vector <sClient > vClients = LoadCleintsDataFromFile(ClientsFileName);
    int totalBalances = CalculateTotalBalances(vClients);
    cout << "\n\t\t\ Client List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________________________\n";
    
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________________________\n";
    cout  << endl;
    for (sClient Client : vClients) {
        PrintClientRecordBalance(Client);
        cout << endl;
    }
    cout << "\n_______________________________________________________________________\n";
    cout << "\t \t Total Balances :  "<< totalBalances <<" \t\t \n";
    cout << "_______________________________________________________________________\n" << endl;
    cout << "Press any Key To Go Back !" << endl;
    system("pause>0");
}

void TransactionsRouter(enTransactionsMenuOptions  option)
{
    switch (option)
    {
    case enTransactionsMenuOptions::Diposit:
         system("cls");
         TransactionsDeposit();
         ShowTransactionsMenu();
         break;
    case enTransactionsMenuOptions::Withdraw:
        system("cls");
        TransactionsWithdraw();
        ShowTransactionsMenu();
        break;

    case enTransactionsMenuOptions::TotalBalance:
        system("cls");
        TotalTransactionsBalance();
        ShowTransactionsMenu();
        break;
    case enTransactionsMenuOptions::MainTransactionsMenu:
        ShowMainScreen();
        break;
    }
}

short ReadMainMenuOption()
{
    short option;
    cout << "Choose What do you Want to Do ?  [ 1 To 6 ]" << endl;
    cin >> option;
    return option;
}
short ReadTransactionsMenuOption()
{
    short option;
    cout << "Choose What do you Want to Do ?  [ 1 To 6 ]" << endl;
    cin >> option;
    return option;
}

void ShowMainScreen() {
   
    system("cls");
    cout << "===========================================================================================" << endl;
    cout << setw(15) << "\t\t\t\t\t   Main Menu Screen  \t \t \t \t  " << endl;
    cout << "===========================================================================================" << endl;
    cout << "\t [1] Show Client List " << endl;
    cout << "\t [2] Add New Client " << endl;
    cout << "\t [3] Delete Client " << endl;
    cout << "\t [4] Update Client Info " << endl;
    cout << "\t [5] Find Client " << endl;
    cout << "\t [6] Transactions " << endl;
    cout << "\t [7] Exit " << endl;
    cout << "===========================================================================================" << endl;

    MainMenuRouter((enMainMenuOptions)ReadMainMenuOption());

}
void ShowTransactionsMenu() {
    
    system("cls");
    cout << "===========================================================================================" << endl;
    cout << setw(15) << "\t\t\t\t\t   Transactions Menu Screen  \t \t \t \t  " << endl;
    cout << "===========================================================================================" << endl;
    cout << "\t [1] Deposit " << endl;
    cout << "\t [2] Withdraw " << endl;
    cout << "\t [3] Total Balances " << endl;
    cout << "\t [4] Main Menu. " << endl;
    cout << "==========================================================================================" << endl;

    TransactionsRouter((enTransactionsMenuOptions)ReadTransactionsMenuOption());

}

int main() {

    ShowMainScreen();
    system("pause>0");

    return 0;
}