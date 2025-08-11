#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

string productFile = "products.txt";
string customerFile = "customers.txt";
string employeeFile = "employees.txt";

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

    string filename = role == "customer" ? customerFile : employeeFile;
    bool isDuplicate = duplicateUsernameCheck(username, filename);
    if (!isDuplicate)
    {
        ofstream file(filename, ios::app);
        int id = getNextId(filename);
        string encrypted = encrypt(password);
        file << id << "," << username << "," << encrypted << "\n";
        cout << "Registration successful. Your ID is " << id << "\n";
    }
}

void CustomerList()
{
    ifstream file(customerFile);
    string line;
    cout << "Customer List:\n";
    while (getline(file, line))
    {
        stringstream ss(line);
        string id, user;
        getline(ss, id, ',');
        getline(ss, user, ',');
        cout << "ID: " << id << " " << " username: " << user << endl;
    }
}

class product
{
public:
    int id;
    string name;
    double price;
    int quantity;
    string manufacturer_date;
    string expiry_date;
    product *next = NULL;
};

product *head = NULL;

class productHelper
{
public:
    void loadFromFile()
    {
        ifstream file(productFile);
        if (!file)
        {
            cout << "No previous product file found.\n";
            return;
        }

        string line;
        while (getline(file, line))
        {
            if (line.empty())
                continue;

            stringstream ss(line);
            product *newProduct = new product();
            string idStr, priceStr, quantityStr, manufacturer_date, expiry_date;
            getline(ss, line, ',');
            newProduct->id = stoi(line);
            getline(ss, newProduct->name, ',');
            getline(ss, priceStr, ',');
            newProduct->price = stod(priceStr);
            getline(ss, quantityStr, ',');
            newProduct->quantity = stoi(quantityStr);
            getline(ss, newProduct->manufacturer_date, ',');
            getline(ss, newProduct->expiry_date, ',');

            if (head == NULL)
            {
                head = newProduct;
            }
            else
            {
                product *temp = head;
                while (temp->next != NULL)
                {
                    temp = temp->next;
                }
                temp->next = newProduct;
            }
        }
    };

    void saveToFile()
    {
        ofstream file(productFile);
        if (!file)
        {
            cout << "Error opening product file for writing.\n";
            return;
        }

        product *temp = head;
        while (temp != NULL)
        {
            file << temp->id << "," << temp->name << "," << temp->price
                 << "," << temp->quantity << "," << temp->manufacturer_date
                 << "," << temp->expiry_date << "\n";
            temp = temp->next;
        }
    }
    void displayProducts()
    {
        if (head == NULL)
        {
            cout << "No products available.\n";
            return;
        }

        product *temp = head;
        cout << "Product List:\n";
        while (temp != NULL)
        {
            cout << "ID: " << temp->id << ", Name: " << temp->name
                 << ", Price: " << temp->price
                 << ", Quantity: " << temp->quantity
                 << ", Manufacturer Date: " << temp->manufacturer_date
                 << ", Expiry Date: " << temp->expiry_date << "\n";
            temp = temp->next;
        }
    }

    // this function generates a new product id
    int productIdGenerator()
    {
        int id;
        if (head == NULL)
        {
            id = 1;
        }
        else
        {
            product *temp = head;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            id = temp->id + 1;
        }
        return id;
    }

    // this function adds a new product to the linked list and saves it to the file
    void addProduct()
    {
        string choice = "y";
        while (choice == "y" || choice == "Y")
        {

            string name, manufacturer_date, expiry_date;
            double price;
            int quantity;
            cout << "Enter product name: ";
            cin >> name;
            cout << "Enter product price: ";
            cin >> price;
            cout << "Enter product quantity: ";
            cin >> quantity;
            cout << "Enter product manufacturer date (YYYY-MM-DD): ";
            cin >> manufacturer_date;
            cout << "Enter product expiry date (YYYY-MM-DD): ";
            cin >> expiry_date;

            product *newProduct = new product();
            newProduct->id = productIdGenerator();
            newProduct->name = name;
            newProduct->price = price;
            newProduct->quantity = quantity;
            newProduct->manufacturer_date = manufacturer_date;
            newProduct->expiry_date = expiry_date;

            if (head == NULL)
            {
                head = newProduct;
            }
            else
            {
                product *temp = head;
                while (temp->next != NULL)
                {
                    temp = temp->next;
                }
                temp->next = newProduct;
            }

            ofstream file(productFile, ios::app);
            file << newProduct->id << "," << name << "," << price << "," << quantity << "," << manufacturer_date << "," << expiry_date << "\n";
            cout << "Product added successfully. Product ID: " << newProduct->id << "\n";
            cout << "Do you want to add another product? (y/n): ";
            cin >> choice;
        }
    }

    void editProduct()
    {
        int id;
        displayProducts();
        cout << "Enter product ID to edit: ";
        cin >> id;

        product *temp = head;
        while (temp != NULL && temp->id != id)
        {
            temp = temp->next;
        }

        if (temp == NULL)
        {
            cout << "Product not found.\n";
            return;
        }

        string name, manufacturer_date, expiry_date;
        double price;
        int quantity;

        cout << "Enter new product name (current: " << temp->name << ", press: s for keep current): ";

        cin >> name;
        if (name == "s" || name == "S")
        {
            name = temp->name; 
        }
        cout << "Enter new product price (current: " << temp->price << ", press: s for keep current): ";
        cin >> price;
        if (price == 's' || price == 'S')
        {
            price = temp->price; 
        }
        cout << "Enter new product quantity (current: " << temp->quantity << ", press: s for keep current): ";
        cin >> quantity;
        if (quantity == 's' || quantity == 'S')
        {
            quantity = temp->quantity; 
        }
        cout << "Enter new product manufacturer date (current: " << temp->manufacturer_date << ", press: s for keep current): ";
        cin >> manufacturer_date;
        if (manufacturer_date == "s" || manufacturer_date == "S")
        {
            manufacturer_date = temp->manufacturer_date; 
        }
        cout << "Enter new product expiry date (current: " << temp->expiry_date << ", press: s for keep current): ";
        cin >> expiry_date;
        if (expiry_date == "s" || expiry_date == "S")
        {
            expiry_date = temp->expiry_date; 
        }

        temp->name = name;
        temp->price = price;
        temp->quantity = quantity;
        temp->manufacturer_date = manufacturer_date;
        temp->expiry_date = expiry_date;

        saveToFile();

        cout << "Product updated successfully.\n";
    }

    void deleteProduct()
    {
        int id;
        displayProducts();
        cout << "Enter product ID to delete: ";
        cin >> id;

        product *temp = head;
        product *prev = NULL;

        while (temp != NULL && temp->id != id)
        {
            prev = temp;
            temp = temp->next;
        }

        if (temp == NULL)
        {
            cout << "Product not found.\n";
            return;
        }

        if (prev == NULL)
        {
            head = temp->next; // Deleting the head
        }
        else
        {
            prev->next = temp->next;
        }

        free(temp);

        saveToFile();

        cout << "Product deleted successfully.\n";
    }


};

void employeeDashboard()
{
    cout << "Welcome to the Employee Dashboard\n";
    productHelper ph;
    while (true)
    {
        int choice;
        cout << "\n1. View Customer List\n2. View Product\n3. Add Product\n4. Edit Product\n5. Delete Product\n6. Logout\n Enter choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            CustomerList();
            break;
        case 2:

            ph.displayProducts();
            break;
        case 3:
            ph.addProduct();
            break;
        case 4:
            ph.editProduct();
            break;
        case 5:
            ph.deleteProduct();
            break;
        default:
            cout << "Logging out...\n";
            return;
            break;
        }
    }
}

void customerDashboard()
{
    cout << "Welcome to the Customer Dashboard\n";
    while (true)
    {
        int choice;
        cout << "\n1. View Products\n2. Logout\nEnter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            cout << "Displaying products...\n"; // Placeholder for product display
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

    string filename = role == "customer" ? customerFile : employeeFile;
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
            if (role == "customer")
            {
                customerDashboard();
            }
            else if (role == "employee")
            {
                employeeDashboard();
            }
            return true;
        }
    }
    cout << "Invalid credentials\n";
    return false;
}

int main()
{
    productHelper ph;
    ph.loadFromFile();
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