#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
int logged_in_user_id = 0;
string logged_in_user_type;
string productFile = "products.txt";
string customerFile = "customers.txt";
string employeeFile = "employees.txt";
string orderFile = "orders.txt";
string orderDetailsFile = "order_details.txt";

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

product *product_head = NULL;
class productHelper
{
public:
    void loadFromFile()
    {
        ifstream file(productFile);
        product_head = NULL;
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

            if (product_head == NULL)
            {
                product_head = newProduct;
            }
            else
            {
                product *temp = product_head;
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

        product *temp = product_head;
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
        if (product_head == NULL)
        {
            cout << "No products available.\n";
            return;
        }

        product *temp = product_head;
        cout << "Product List:\n";
        while (temp != NULL)
        {
            if (logged_in_user_type == "customer" && temp->quantity > 0)
            {
                cout << "ID: " << temp->id << ", Name: " << temp->name
                     << ", Price: " << temp->price
                     << ", Quantity: " << temp->quantity
                     << ", Manufacturer Date: " << temp->manufacturer_date
                     << ", Expiry Date: " << temp->expiry_date << "\n";
            }
            else if (logged_in_user_type == "employee")
            {
                cout << "ID: " << temp->id << ", Name: " << temp->name
                     << ", Price: " << temp->price
                     << ", Quantity: " << temp->quantity
                     << ", Manufacturer Date: " << temp->manufacturer_date
                     << ", Expiry Date: " << temp->expiry_date << "\n";
            }
            temp = temp->next;
        }
    }

    // this function generates a new product id
    int productIdGenerator()
    {
        int id;
        if (product_head == NULL)
        {
            id = 1;
        }
        else
        {
            product *temp = product_head;
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

            if (product_head == NULL)
            {
                product_head = newProduct;
            }
            else
            {
                product *temp = product_head;
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

        product *temp = product_head;
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

        string temp_str;
        cout << "Enter new product name (current: " << temp->name << ", press: s for keep current): ";
        cin >> temp_str;
        if (temp_str == "s" || temp_str == "S")
        {
            name = temp->name;
        }
        else
        {
            name = temp_str;
        }
        cout << "Enter new product price (current: " << temp->price << ", press: s for keep current): ";
        cin >> temp_str;
        if (temp_str == "s" || temp_str == "S")
        {
            price = temp->price;
        }
        else
        {
            price = stod(temp_str);
        }
        cout << "Enter new product quantity (current: " << temp->quantity << ", press: s for keep current): ";
        cin >> temp_str;
        if (temp_str == "s" || temp_str == "S")
        {
            quantity = temp->quantity;
        }
        else
        {
            quantity = stoi(temp_str);
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

        product *temp = product_head;
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
            product_head = temp->next; // Deleting the product_head
        }
        else
        {
            prev->next = temp->next;
        }

        free(temp);

        saveToFile();

        cout << "Product deleted successfully.\n";
    }
    product *searchProductNoPrint(int id)
    {
        product *temp = product_head;
        while (temp != NULL)
        {
            if (temp->id == id)
            {
                return temp;
            }
            temp = temp->next;
        }
        cout << "Product not found.\n";
        return NULL;
    }

    product *searchProduct(int id)
    {
        product *temp = product_head;
        while (temp != NULL)
        {
            if (temp->id == id)
            {
                cout << "Product found: ID: " << temp->id << ", Name: " << temp->name
                     << ", Price: " << temp->price
                     << ", Quantity: " << temp->quantity
                     << ", Manufacturer Date: " << temp->manufacturer_date
                     << ", Expiry Date: " << temp->expiry_date << "\n";
                return temp;
            }
            temp = temp->next;
        }
        cout << "Product not found.\n";
        return NULL;
    }
};

class order
{
public:
    int id;
    int customer_id;
    double total_price;
    order *next = NULL;
};

class orderDetails
{
public:
    int id;
    int order_id;
    int customer_id;
    int product_id;
    int quantity;
    double price;
    orderDetails *next = NULL;
};

double total_price = 0.0;

class cart
{
public:
    int product_id;
    string product_name;
    double product_price;
    int quantity = 0;
    double price = 0.0;
    cart *next = NULL;
};

order *order_head = NULL;
orderDetails *orderDetails_head = NULL;

class orderHelper
{
public:
    void loadFromFile()
    {
        ifstream order_F(orderFile);
        ifstream orderD_F(orderDetailsFile);
        order_head = NULL;
        orderDetails_head = NULL;

        if (!order_F || !orderD_F)
        {
            cout << "No previous order file found.\n";
            return;
        }

        string line;

        while (getline(order_F, line))
        {
            if (line.empty())
                continue;

            stringstream ss(line);
            string idStr, customer_id, totalPriceStr;

            order *newOrder = new order();
            newOrder->next = NULL; // Prevent garbage link

            try
            {
                getline(ss, idStr, ',');
                newOrder->id = stoi(idStr);

                getline(ss, customer_id, ',');
                newOrder->customer_id = stoi(customer_id);

                getline(ss, totalPriceStr, ',');
                newOrder->total_price = stod(totalPriceStr);
            }
            catch (const exception &e)
            {
                cout << "Error loading order: " << e.what() << "\n";
                delete newOrder;
                continue;
            }

            if (order_head == NULL)
            {
                order_head = newOrder;
            }
            else
            {
                order *temp = order_head;
                while (temp->next != NULL)
                    temp = temp->next;
                temp->next = newOrder;
            }
        }

        while (getline(orderD_F, line))
        {
            if (line.empty())
                continue;

            stringstream ss(line);
            string idStr, order_idStr, customer_ids, product_idStr, quantityStr, priceStr;

            orderDetails *newOrderDetails = new orderDetails();
            newOrderDetails->next = NULL;

            try
            {
                getline(ss, idStr, ',');
                newOrderDetails->id = stoi(idStr);
                getline(ss, order_idStr, ',');
                newOrderDetails->order_id = stoi(order_idStr);
                getline(ss, customer_ids, ',');
                newOrderDetails->customer_id = stoi(customer_ids);
                getline(ss, product_idStr, ',');
                newOrderDetails->product_id = stoi(product_idStr);
                getline(ss, quantityStr, ',');
                newOrderDetails->quantity = stoi(quantityStr);
                getline(ss, priceStr, ',');
                newOrderDetails->price = stod(priceStr);
            }
            catch (const exception &e)
            {
                cout << "Error loading order detail: " << e.what() << "\n";
                delete newOrderDetails;
                continue;
            }

            if (orderDetails_head == NULL)
            {
                orderDetails_head = newOrderDetails;
            }
            else
            {
                orderDetails *temp = orderDetails_head;
                while (temp->next != NULL)
                    temp = temp->next;
                temp->next = newOrderDetails;
            }
        }
    }

    int orderIdGenerator()
    {
        int id;
        if (order_head == NULL)
        {
            id = 1;
        }
        else
        {
            order *temp = order_head;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            id = temp->id + 1;
        }
        return id;
    }

    int orderDetailsIdGenerator()
    {
        int id;
        if (orderDetails_head == NULL)
        {
            id = 1;
        }
        else
        {
            orderDetails *temp = orderDetails_head;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            id = temp->id + 1;
        }
        return id;
    }

    void showOrders()
    {
        order *order_temp = order_head;
        if (order_temp != NULL)
        {
            while (order_temp != NULL)
            {
                orderDetails *order_details_temp = orderDetails_head;
                cout << "Order No :" << order_temp->id << " \n";
                cout << "-----------------------------------------------------------------------\n";
                double this_order_total_price = 0;
                while (order_details_temp != NULL)
                {
                    if (order_details_temp->order_id == order_temp->id)
                    {
                        productHelper pH;
                        product *temp_product = pH.searchProductNoPrint(order_details_temp->product_id);
                        cout << "Product name : " << temp_product->name << "    Quantity : " << order_details_temp->quantity << "    Price : " << order_details_temp->price << "\n";
                        this_order_total_price += order_details_temp->price;
                    }
                    order_details_temp = order_details_temp->next;
                }

                cout << "\n-----------------------\n";
                cout << "Total Price   :" << this_order_total_price << "\n";
                cout << "------------------------------------------------------------------------\n";

                order_temp = order_temp->next;
            }
        }
        else
        {
            cout << "\nNo order created.\n";
        }
    }

    void showCustomerOrders()
    {
        order *order_temp = order_head;
        int customer_id = logged_in_user_id;
        if (logged_in_user_type == "employee")
        {
            cout << "Enter customer user id:\n";
            cin >> customer_id;
        }

        if (order_temp != NULL)
        {
            while (order_temp != NULL)
            {
                if (order_temp->customer_id = customer_id)
                {
                    orderDetails *order_details_temp = orderDetails_head;
                    cout << "Order No :" << order_temp->id << " \n";
                    cout << "-----------------------------------------------------------------------\n";
                    double this_order_total_price = 0;
                    while (order_details_temp != NULL)
                    {
                        if (order_details_temp->order_id == order_temp->id)
                        {
                            productHelper pH;
                            product *temp_product = pH.searchProductNoPrint(order_details_temp->product_id);
                            cout << "Product name : " << temp_product->name << "    Quantity : " << order_details_temp->quantity << "    Price : " << order_details_temp->price << "\n";
                            this_order_total_price += order_details_temp->price;
                        }
                        order_details_temp = order_details_temp->next;
                    }

                    cout << "\n-----------------------\n";
                    cout << "Total Price   :" << this_order_total_price << "\n";
                    cout << "------------------------------------------------------------------------\n";
                }
                order_temp = order_temp->next;
            }
        }
        else
        {
            cout << "\nNo order created.\n";
        }
    }
    void saveToFile()
    {
        ofstream order_F(orderFile);
        ofstream orderD_F(orderDetailsFile);
        if (!order_F || !orderD_F)
        {
            cout << "Error opening order file for writing.\n";
            return;
        }

        order *temp = order_head;
        while (temp != NULL)
        {
            order_F << temp->id << "," << temp->customer_id << "," << temp->total_price << "\n";
            temp = temp->next;
        }

        orderDetails *tempD = orderDetails_head;
        while (tempD != NULL)
        {
            orderD_F << tempD->id << "," << tempD->order_id << "," << tempD->customer_id << "," << tempD->product_id
                     << "," << tempD->quantity << "," << tempD->price << "\n";
            tempD = tempD->next;
        }
    }
};

cart *cart_head = NULL;

class cartHelper
{
public:
    void addToCart()
    {
        productHelper ph;
        while (true)
        {
            ph.displayProducts();
            int id, quantity;
            cout << "Enter product ID to add to cart (0 to finish): ";
            cin >> id;
            if (id == 0)
                break;

            product *temp = ph.searchProduct(id);
            if (temp == NULL)
            {
                cout << "Invalid product ID.\n";
                continue;
            }
            if (temp->quantity == 0)
            {
                cout << "Product is out of stock.\n";
                continue;
            }

            cart *validate = searchCartProduct(id);

            cout << "Enter quantity: ";
            cin >> quantity;
            while (temp->quantity < quantity)
            {
                cout << "Insufficient stock. Available quantity: " << temp->quantity << "\n";
                cout << "Enter quantity: ";
                cin >> quantity;
            }

            if (validate != NULL)
            {
                total_price -= validate->price;
                validate->quantity = quantity;
                validate->price = temp->price * quantity;
                total_price += validate->price;
            }
            else
            {
                if (temp->quantity > 0)
                {
                    cart *newCart = new cart();
                    newCart->product_id = temp->id;
                    newCart->product_name = temp->name;
                    newCart->product_price = temp->price;
                    newCart->quantity = quantity;
                    newCart->price = temp->price * quantity;
                    newCart->next = NULL;

                    if (cart_head == NULL)
                        cart_head = newCart;
                    else
                    {
                        cart *tempCart = cart_head;
                        while (tempCart->next != NULL)
                            tempCart = tempCart->next;
                        tempCart->next = newCart;
                    }

                    total_price += newCart->price;
                }
            }

            temp->quantity -= quantity;

            displayCart();
            cout << "Do you want to add another product? (y/n): ";
            string choice;
            cin >> choice;
            if (choice == "n" || choice == "N")
                break;
        }
    }

    void displayCart()
    {
        if (cart_head == NULL)
        {
            cout << "Cart is empty.\n";
            return;
        }
        cout << "Cart Items:\n";
        cart *temp = cart_head;
        while (temp != NULL)
        {
            cout << "Product ID: " << temp->product_id
                 << ", Name: " << temp->product_name
                 << ", Product price :" << temp->product_price
                 << ", Quantity: " << temp->quantity
                 << ", Price: " << temp->price << "\n";
            temp = temp->next;
        }
        cout << "--------------------------\n";
        cout << "Total Price: " << total_price << "\n";
    }

    bool checkout()
    {
        if (cart_head != NULL)
        {
            cart *cart_temp = cart_head;

            // order create start
            orderHelper order_helper;
            order *new_order = new order();
            order *temp_order = order_head;
            new_order->id = order_helper.orderIdGenerator();
            new_order->customer_id = logged_in_user_id;
            new_order->total_price = total_price;
            if (temp_order == NULL)
            {
                order_head = new_order;
            }
            else
            {
                while (temp_order->next != NULL)
                {
                    temp_order = temp_order->next;
                }
                temp_order->next = new_order;
            }
            // order create end

            // order details create start
            orderDetails *order_details_temp = orderDetails_head;
            if (order_details_temp != NULL)
            {
                while (order_details_temp->next != NULL)
                {
                    order_details_temp = order_details_temp->next;
                }
            }
            while (cart_temp != NULL)
            {
                orderDetails *new_order_details = new orderDetails();
                new_order_details->id = order_helper.orderDetailsIdGenerator();
                new_order_details->order_id = new_order->id;
                new_order_details->customer_id = new_order->customer_id;
                new_order_details->product_id = cart_temp->product_id;
                new_order_details->quantity = cart_temp->quantity;
                new_order_details->price = cart_temp->price;
                new_order_details->next = NULL;

                if (orderDetails_head == NULL)
                {
                    orderDetails_head = new_order_details;
                    order_details_temp = orderDetails_head;
                }
                else
                {
                    order_details_temp->next = new_order_details;
                    order_details_temp = order_details_temp->next;
                }

                // product stock qty update

                product *product_temp = product_head;
                while (product_temp != NULL)
                {
                    if (product_temp->id == cart_temp->product_id)
                    {
                        product_temp->quantity -= cart_temp->quantity;
                    }

                    product_temp = product_temp->next;
                }

                cart_temp = cart_temp->next;
            }

            // order details create end
            total_price = 0;
            order_helper.saveToFile();
            cart_head = NULL;
            productHelper ph;
            ph.saveToFile();
            cout << " Order successfully created. ";
            return true;
        }
        else
        {
            cout << "Cart is empty, please buy something.";
            return false;
        }
        return false;
    }

    cart *searchCartProduct(int id)
    {
        cart *temp = cart_head;
        if (cart_head != NULL)
        {
            while (temp != NULL)
            {
                if (temp->product_id == id)
                {
                    return temp;
                }
                temp = temp->next;
            }
        }
        return NULL;
    }
};

void employeeDashboard()
{
    cout << "Welcome to the Employee Dashboard\n";
    productHelper ph;
    orderHelper order_helper;
    while (true)
    {
        orderHelper oH;
        oH.loadFromFile();
        int choice;
        cout << "\n1. View Customer List\n2. View Product\n3. Add Product\n4. Edit Product\n5. Delete Product\n6. Show orders\n7. Search Customer Order\n8. Logout\nEnter choice: ";
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
        case 6:
            order_helper.showOrders();
            break;
        case 7:
            order_helper.showCustomerOrders();
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
        cout << "\n1. View Products\n2. Add to Cart\n3. Display Cart Product\n4. Checkout\n5. My orders\n6.Logout\n Enter choice: ";
        cin >> choice;

        productHelper ph;
        cartHelper c;
        orderHelper oH;
        oH.loadFromFile();
        switch (choice)
        {
        case 1:
            ph.displayProducts();
            break;
        case 2:
            c.addToCart();
        case 3:
            c.displayCart();
            break;
        case 4:
            c.checkout();
            break;
        case 5:
            oH.showCustomerOrders();
            break;
        default:
            cout << "Logging out...\n";
            return;
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
            logged_in_user_id = stoi(id);
            logged_in_user_type = role;
            cout << "Login successful. Welcome " << role << " " << username << "\n";
            if (role == "customer")
            {
                customerDashboard();
            }
            else if (role == "employee")
            {
                employeeDashboard();
            }
            else
            {
                cout << "Invalid credentials\n";
                return false;
            }
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
            productHelper ph;
            ph.loadFromFile();
            loginUser(role);
        }
        else
        {
            break;
        }
    }

    return 0;
}