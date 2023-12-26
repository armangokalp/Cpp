#include "Products.h"

using namespace std;


void menu() {
    cout << "MENU\n" << "1. Add item to the shopping list\n" << "2. Delete item from the shopping list\n" << "3. Edit the amount of existing item\n" << "4. Print current receipt\n" << "5. Print current receipt in order\n" << "6. Print current receipt in reverse order\n" << "7. Exit\n" << "---\n" << "Enter your choice: ";
}


int main() {
    ifstream qr, price;
    string qr_code;
    short option = 0, amount = 1;
    Products p;
    
    p.read_files(qr, price);
    
    
    menu();
    
    
    do {
        cin >> option;
        switch (option) {
            case 1:
                cout << "Please enter the QR code to add: "; cin >> qr_code;
                if(p.find(qr_code) && !p.findInShopList(qr_code)) {
                    cout << "Please enter the amount to add: "; cin >> amount;
                    if (amount >= 1 && amount <= 25) {
                            
                        p.add_item(qr_code, amount);    menu();
                        
                    } else {    cout << "Invalid amount, try again" << endl; menu();    }
                    
                } else if (p.findInShopList(qr_code)) {
                        cout << "Item is already in the shoplist, if you want to edit the amount please choose option 3" << endl;   menu();
                } else {  cout << "Invalid QR code, try again" << endl; menu();   }
                break;
            case 2:
                cout << "Please enter the QR code to delete: "; cin >> qr_code;
                if (p.findInShopList(qr_code)) {
                    
                    p.remove_item(qr_code);                 menu();
                    
                } else {  cout << "Shoplist does not contain given QR code" << endl; menu();   }
                break;
            case 3:
                cout << "Please enter the QR code to edit: "; cin >> qr_code;
                if (p.findInShopList(qr_code)) {
                    
                    cout << "Please enter the amount to edit: "; cin >> amount;
                    if (amount >= 1 && amount <= 25) {
                        
                        p.edit_item(qr_code, amount);       menu();
                        
                    } else {    cout << "Invalid amount, try again" << endl; menu();    }
                    
                } else {  cout << "Shoplist does not contain given QR code" << endl; menu();   }
                break;
            case 4:
                p.printCurrentReceipt();                    menu();
                break;
            case 5:
                p.printCurrentReceiptAscending();           menu();
                break;
            case 6:
                p.printCurrentReceiptDescending();          menu();
                break;
            case 7:
                cout << "Goodbye!" << endl;
                break;
        }
    } while (option != 7);
    
    return 0;
}
