#include "Products.h"

using namespace std;

Products::Products(){

}

vector<Product> Products::get_item_list()const{
    return item_list;
}
vector<Product> Products::get_shop_list()const{
    return shop_list;
}

Products::Products(const Products & p){
    item_list = p.get_item_list();
    shop_list = p.get_shop_list();
}


void Products::read_files(ifstream & qr, ifstream & price) {
    string qr_name, price_name, str, s, sprev = "";
    short i = 0;
    
    do {
        cout << "Please enter a filename for QR database: "; cin >> qr_name;
        qr.open(qr_name.c_str());
        
        if (qr.fail()) {
            cout << "\nThe QR file does not exists" << endl;
        } else {
            
            Product p = {"", "", "", 0};
            
            vector<string> sv;
            do {
                qr >> s;
                if (s != sprev) {   sv.push_back(s);    }       sprev = s;
                item_list.push_back(p);
                ToUpper(s);     item_list[i].qr_code = s;   s.clear();
                StripWhite(str);    if (i>=1) {item_list[i-1].p_name = str;}    str.clear();
                i++;
            } while (getline(qr, str));
            item_list.pop_back();   sprev.clear();
            break;
        }
    } while (qr.fail());

    qr.close();
    
    
    do {
        cout << "\nPlease enter a filename for Price database: "; cin >> price_name;
        price.open(price_name.c_str());
        
        if (price.fail()) {
            cout << "\nThe Price file does not exists";
        } else {
            vector<string> sv, strv;
            do {
                price >> s;
                ToUpper(s);
                if (s != sprev) {
                    sv.push_back(s);         sprev = s;            s.clear();
                    StripWhite(str);    strv.push_back(str);     str.clear();
                }
            } while (getline(price, str));
            
            for (int i = 0; i < item_list.size(); i++) {
                for (int k = 0; k < sv.size(); k++) {
                    if (item_list[i].qr_code == sv[k]) {
                        item_list[i].p_price = strv[k+1];
                    }
                }
            }
            
            break;
        }
    } while (price.fail());
    price.close();
}


bool Products::find(string qr_code) {
    for (int i = 0; i < item_list.size(); i++) {
        if (UpperString(item_list[i].qr_code) == UpperString(qr_code)) {
            return 1;
        }
    }
    return 0;
}


bool Products::findInShopList(string qr_code) {
    for (int i = 0; i < shop_list.size(); i++) {
        if (UpperString(shop_list[i].qr_code) == UpperString(qr_code)) {
            return 1;
        }
    }
    return 0;
}


void Products::add_item(string qr, int amount) {
    Product p;      p.qr_code = qr;     ToUpper(qr);     p.buy_amount = amount;
    for (int i = 0; i < item_list.size(); i++) {
        if (item_list[i].qr_code == qr) {
            p.p_price = item_list[i].p_price;   p.p_name = item_list[i].p_name;
        }
    }
    shop_list.push_back(p);
}


void Products::edit_item(string qr, int amount) {
    for (int i = 0; i < shop_list.size(); i++) {
        if (UpperString(shop_list[i].qr_code) == UpperString(qr)) {
            shop_list[i].buy_amount = amount;
        }
    }
}


void Products::remove_item(string qr) {
    for (int i = 0; i < shop_list.size(); i++) {
        if (UpperString(shop_list[i].qr_code) == UpperString(qr)) {
            shop_list.erase(shop_list.begin() + i);
        }
    }
}


void Products::printCurrentReceipt() {
    float sum = 0, price;      setprecision(4);
    for (int i = 0; i < shop_list.size(); i++) {
        price = shop_list[i].buy_amount * atof(shop_list[i].p_price);
        cout << shop_list[i].p_name << " * " << setw(2) << shop_list[i].buy_amount << setw(31-shop_list[i].p_name.length()) << price << endl;
        sum += price;
    }
    cout << "\nVAT(18%):" << setw(27) << sum * 0.18 << endl << string(39, '_') << endl << "Total:" << setw(30) << sum * 1.18 << "\n\n";
}


void Products::printCurrentReceiptAscending() {
    float sum = 0; double tempd = 0.0; short tempi = 0; string temp = "";
    unsigned long sl = shop_list.size();          setprecision(4);
    vector<float> price(sl), q(sl); vector<string> name(sl);
    
    for (int i = 0; i < sl; i++) {
        name[i] += shop_list[i].p_name;
        q[i] = shop_list[i].buy_amount;
        price[i] = q[i] * atof(shop_list[i].p_price);
    }
    for (int i = 0; i < sl; i++) {
        for (int k = i+1; k < sl; k++) {
            if (price[k] < price[i]) {
                temp = name[i];      name[i] = name[k];     name[k] = temp;
                tempi = q[i];          q[i] = q[k];           q[k] = tempi;
                tempd = price[i];   price[i] = price[k];  price[k] = tempd;
            }
        }
    }
    for (int i = 0; i < sl; i++) {
        cout << name[i] << " * " << setw(2) << q[i] << setw(31-name[i].length()) << price[i] << endl;
        sum += price[i];
    }
    cout << "\nVAT(18%):" << setw(27) << sum * 0.18 << endl << string(39, '_') << endl << "Total:" << setw(30) << sum * 1.18 << "\n\n";
}


void Products::printCurrentReceiptDescending() {
    float sum = 0; double tempd = 0.0; short tempi = 0; string temp = "";
    unsigned long sl = shop_list.size();          setprecision(4);
    vector<float> price(sl), q(sl); vector<string> name(sl);
    
    for (int i = 0; i < sl; i++) {
        name[i] += shop_list[i].p_name;
        q[i] = shop_list[i].buy_amount;
        price[i] = q[i] * atof(shop_list[i].p_price);
    }
    for (int i = 0; i < sl; i++) {
        for (int k = i+1; k < sl; k++) {
            if (price[k] < price[i]) {
                temp = name[i];      name[i] = name[k];     name[k] = temp;
                tempi = q[i];          q[i] = q[k];           q[k] = tempi;
                tempd = price[i];   price[i] = price[k];  price[k] = tempd;
            }
        }
    }
    for (int i = sl-1; i > -1; i--) {
        cout << name[i] << " * " << setw(2) << q[i] << setw(31-name[i].length()) << price[i] << endl;
        sum += price[i];
    }
    cout << "\nVAT(18%):" << setw(27) << sum * 0.18 << endl << string(39, '_') << endl << "Total:" << setw(30) << sum * 1.18 << "\n\n";
}
