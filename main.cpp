#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
string encrypt(string text, int key = 3)
{
    for (char &c : text)
    {
        c = c + key;
    }
    return text;
}

string decrypt(string text, int key = 3)
{
    for (char &c : text)
    {
        c = c - key;
    }
    return text;
}

int getNextId(const string &filename)
{
    ifstream file(filename);
    string line;
    int lastId = 0;
    while (getline(file, line))
    {
        stringstream ss(line);
        string idStr;
        getline(ss, idStr, ',');
        lastId = stoi(idStr);
    }
    return lastId + 1;
}

// this is a liner search
bool duplicateUsernameCheck(const string &username, const string &filename)
{
    ifstream file(filename);
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string id, user;
        getline(ss, id, ',');
        getline(ss, user, ',');
        if (user == username)
        {
            cout << "Username already exists. Please choose a different username.\n";
            return true;
        }
    }
    return false;
}

void registerUser(const string &role)
{
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    string filename = role == "customer" ? "customers.txt" : "employees.txt";
    bool isDuplicate = duplicateUsernameCheck(username, filename);
    if(!isDuplicate){
        ofstream file(filename, ios::app);
        int id = getNextId(filename);
        string encrypted = encrypt(password);
        file << id << "," << username << "," << encrypted << "\n";
        cout << "Registration successful. Your ID is " << id << "\n";
    }
}

void CustomerList()
{
    ifstream file("customers.txt");
    string line;
    cout << "Customer List:\n";
    while (getline(file, line))
    {
        cout << line << "\n";
    }
}

void employeeDashboard()
{
    cout << "Welcome to the Employee Dashboard\n";
    while (true)
    {
        int choice;
        cout << "\n1. View Customer List\n2. Logout\nEnter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            CustomerList();
        }
        else
        {
            cout << "Logging out...\n";
            break;
        }
    }
}

bool loginUser(const string &role)
{
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    string filename = role == "customer" ? "customers.txt" : "employees.txt";
    ifstream file(filename);
    string line;
    string encrypted = encrypt(password);
    while (getline(file, line))
    {
        stringstream ss(line);
        string id, user, pass;
        getline(ss, id, ',');
        getline(ss, user, ',');
        getline(ss, pass, ',');
        if (user == username && pass == encrypted)
        {
            cout << "Login successful. Welcome " << role << " " << username << "\n";
            employeeDashboard();
            return true;
        }
    }
    cout << "Invalid credentials\n";
    return false;
}

int main()
{
    while (true)
    {
        int choice;
        cout << "\n1. Register\n2. Login\n3. Exit\nEnter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            string role;
            cout << "Enter role (customer/employee): ";
            cin >> role;
            registerUser(role);
        }
        else if (choice == 2)
        {
            string role;
            cout << "Enter role (customer/employee): ";
            cin >> role;
            loginUser(role);
        }
        else
        {
            break;
        }
    }

    return 0;
}