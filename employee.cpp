#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
using namespace std;

class employee{
    private:
        int product_code;
        int product_quantity;
        float price;
        string product_name;
        string expiry_date;
        string making_date;
    
    public:
        void administrator();
        void add_product();
        void edit_product(); //edit product name , expire date, product quantity
        void product_list();
        
};

void employee :: administrator(){
    int choice;

    while(true){
        cout<<"1. Add product\n\n";
        cout<<"2. Modify product details\n\n";
        cout<<"3. Show product list\n\n";
        cout<<"4. exit\n\n";
        cout<<"Enter your choice: ";
        cin>>choice;

        switch(choice){
            case 1:
                add_product();
                break;
            case 2:
                edit_product();
                break;
            case 3:
                product_list();
                break;
            case 4:
                exit(0);
            
            default :
                cout<<"Invalid Choice!!!\n\n";
                

        }
        
    }
}
    

void employee :: add_product(){
    fstream product_data;
    int code, quantity;
    float pri;
    string n, making, expire;
    bool same=false;
    string choice;
    
    cout<<"\nAdd New Product\n\n";
    do{
        

        do{
            same=false;
            cout<<"Product code: ";
            cin>>product_code;

            product_data.clear();
            product_data.open("data_base.txt", ios::in);

            if(product_data){
                
                while(product_data>>code>>n>>pri>>quantity>>making>>expire){
                    if(code==product_code){
                        cout<<"This code is already exists.\n\n";
                        same=true;
                        break;
                    }
    
                }
                product_data.close();
            }
    
        }while(same);

        cin.ignore();
        cout<<"\nName of the product: ";
        getline(cin, product_name); 
        cout<<"\n";

        cout<<"Product Price: ";
        cin>>price;
        cout<<"\n";

        cout<<"Product Quantity: ";
        cin>>product_quantity;
        cout<<"\n";

        cout<<"Making Date: ";
        cin>>making_date;
        cout<<"\n";

        cout<<"Expiry Date: ";
        cin>>expiry_date;
        cout<<"\n";
        
        product_data.open("data_base.txt",ios::app | ios::out);
        product_data<<product_code<<"  "<<product_name<<"  "<<price<<"  "<<product_quantity<<"  "<<making_date<<"  "<<expiry_date<<"\n";
        product_data.close();
        cout<<"Product added successfully\n\n";

        cout<<"Do you want to add a another product?(Yes/No): ";
        cin>>choice;
    }while(choice=="Yes" || choice=="yes");
     
}

void employee:: edit_product(){
    cout<<"coming soon\n";
}

void employee:: product_list(){
    cout<<"coming soon\n";
}

int main(){
    employee emp;
    emp.administrator();
    return 0;
}


