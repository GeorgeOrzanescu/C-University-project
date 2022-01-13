/* Proiect de gestiune a unui MagazinCrypto
* Doresc sa realizez acest proiect deoarece consider ca tehnologia blockchain va fi urmatoarea revolutie
* dupa aparitia internetului. M-am ocupat de minarea si tranzactionarea criptomonedelor in ultimii 3 ani
* si consider ca pot crea o aplicatie tip management pentru acest domeniu.
*/
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <set>
#include <functional>
using namespace std;

namespace helper_functions
{
    string getDate()
    {
        time_t now = time(NULL);
        char str[26] = {};
        ctime_s(str, 26, &now);

        return str;
    };
    template <class T>
    int getFileSize(T a, string file)
    {
        int i = 0;
        T* obj = new T;
        ifstream fin(file, ios::binary);
        if (fin)
        {
            while (!fin.eof() && !(fin.peek() == EOF))
            {
                fin.read((char*)&*obj, sizeof(*obj));
                i++;
            }

            fin.close();
            return i;
        }
        else
        {
            return i;
        }
    }
};

// -----------------------------------------------------------------------------------------
class Wallet
{
private:
    const int walletId;
    static int totalWallets;
    string adress;
    vector<string> typeFunds{ "USD", "EUR", "RON", "BTC", "ETH" };
    int* funds;

public:
    Wallet() : walletId(++totalWallets), adress(""), funds(new int[5])
    {
        for (size_t i = 0; i < typeFunds.size(); i++)
        {
            funds[i] = 0;
        }
    }
    Wallet(string adress) : walletId(++totalWallets), adress(adress), funds(new int[5])
    {
        for (size_t i = 0; i < typeFunds.size(); i++)
        {
            funds[i] = 0;
        }
    }
    Wallet(string adress, int* funds) : walletId(++totalWallets), adress(adress), funds(funds)
    {
    }
    Wallet(const Wallet& other) : walletId(other.walletId), adress(other.adress), funds(new int[5])
    {
        for (int i = 0; i < 5; i++)
        {
            funds[i] = other.funds[i];
        }
    }

    ~Wallet()
    {
        delete[] funds;
    }
    Wallet& operator=(const Wallet& other)
    {
        if (this != &other)
        {
            delete[] funds;
            adress = other.adress;
            funds = new int[5];
            for (int i = 0; i < 5; i++)
            {
                funds[i] = other.funds[i];
            }
        }
        return *this;
    }

    void setAdress(string adress)
    {
        this->adress = adress;
    }
    string getAdress()
    {
        return adress;
    }
    void setFunds(string typeFund, int funds)
    {
        vector<string>::iterator itr;
        itr = find(typeFunds.begin(), typeFunds.end(), typeFund);
        int index = distance(typeFunds.begin(), itr);
        this->funds[index] = funds;
    }
    int* getFunds()
    {
        return funds;
    }
    int getWalletId()
    {
        return walletId;
    }
    static int getTotalWallets()
    {
        return totalWallets;
    }
    int getTotalFunds()
    {
        int totalFunds = 0;
        for (int i = 0; i < 5; i++)
        {
            totalFunds += funds[i];
        }
        return totalFunds;
    }
    void printWallet()
    {
        cout << "Wallet id: " << walletId << endl;
        cout << "Adress: " << adress << endl;
        cout << "Funds: ";
        for (int i = 0; i < 5; i++)
        {
            cout << funds[i] << " ";
        }
        cout << endl;
    }
    void editWallet()
    {
        cout << "Enter new adress: ";
        cin >> adress;
        cout << "Enter new funds: ";
        for (int i = 0; i < 5; i++)
        {
            cout << typeFunds[i] << ": ";
            cin >> funds[i];
        }
    }

    // transfer money from one wallet to another
    Wallet operator+(const Wallet& other)
    {
        for (int i = 0; i < 5; i++)
        {
            funds[i] += other.funds[i];
        }
        return *this;
    }
    // get funds at a certain position
    int operator[](int index)
    {
        return this->funds[index];
    }
    Wallet operator++(int)
    {
        Wallet temp = *this;
        for (int i = 0; i < 5; i++)
        {
            funds[i]++;
        }
        return temp;
    }
    Wallet operator++()
    {
        for (int i = 0; i < 5; i++)
        {
            funds[i]++;
        }
        return *this;
    }
    Wallet operator--(int)
    {
        Wallet temp = *this;
        for (int i = 0; i < 5; i++)
        {
            funds[i]--;
        }
        return temp;
    };
    Wallet operator--()
    {
        for (int i = 0; i < 5; i++)
        {
            funds[i]--;
        }
        return *this;
    };
    // checks if adress is empty
    bool operator!()
    {
        return adress == "";
    }
    //check which wallet has more funds
    bool operator>=(const Wallet& other)
    {
        int sumFirst=0, sumSecond=0;
        for (int i = 0; i < 5; i++)
        {
            sumFirst += funds[i];
            sumSecond += other.funds[i];
        }
        return sumFirst >= sumSecond;
    }
    // check if wallets have the same adress
    bool operator==(const Wallet& other)
    {
        return adress == other.adress;
    }
    explicit operator int()
    {
        return walletId;
    }

    void serializeWallet(ofstream& w, Wallet wallet)
    {
        w.write((char*)&wallet.walletId, sizeof(wallet.walletId));
        int l = wallet.adress.length() + 1;
        w.write((char*)&l, sizeof(l));
        w.write(wallet.adress.c_str(), l);
        for (int i = 0; i < 5; i++)
        {
            w.write((char*)&wallet.funds[i], sizeof(wallet.funds[i]));
        }
    }

    void deserializeWallet(ifstream& r, Wallet& wallet)
    {
        r.read((char*)&wallet.walletId, sizeof(wallet.walletId));
        int l;
        r.read((char*)&l, sizeof(l));
        char* buffer = new char[l];
        r.read(buffer, l);
        wallet.adress = buffer;
        for (int i = 0; i < 5; i++)
        {
            r.read((char*)&wallet.funds[i], sizeof(wallet.funds[i]));
        }
        delete[] buffer;
    }
    friend ostream& operator<<(ostream& os, Wallet& wallet);
    friend istream& operator>>(istream& is, Wallet& wallet);
};

int Wallet::totalWallets = 0;
ostream& operator<<(ostream& out, Wallet& wallet)
{
    out << "Wallet id: " << wallet.walletId << endl;
    out << "Adress: " << wallet.adress << endl;
    out << "Funds: \n";
    for (int i = 0; i < 5; i++)
    {
        out << wallet.typeFunds[i] << ": ";
        out << wallet.funds[i] << " \n";
    }
    out << endl;
    return out;
}
istream& operator>>(istream& in, Wallet& wallet)
{
    cout << "Enter adress: ";
    in >> wallet.adress;
    cout << "Enter funds: ";
    for (int i = 0; i < 5; i++)
    {
        cout << wallet.typeFunds[i] << ": ";
        in >> wallet.funds[i];
    }
    return in;
}

// WILL NOT BE USE BECAUSE WALLETS WILL BE MANAGED ALONG WITH CLIENTS --> HAS A RElANTIONSHIP
Wallet** manageWallets(Wallet** wallets)
{
    Wallet a;
    int size = helper_functions::getFileSize(a, "wallets.bin");

    wallets = new Wallet * [size];

    ifstream fin;
    fin.open("wallets.bin", ios::binary);
    // read all wallets from file
    for (int i = 0; i < size; i++)
    {
        wallets[i] = new Wallet;
        wallets[i]->deserializeWallet(fin, *wallets[i]);
    }

    int choice;
    do
    {
        cout << "\n\t\t\t MANAGE WALLETS";
        cout << "\n\t\t\t---------------";
        cout << "\n\t\t 1. Show all wallets stored";
        cout << "\n\t\t 2. Modify wallet";
        cout << "\n\t\t 3. Add new wallet";
        cout << "\n\t\t 4. Delete wallet";
        cout << "\n\t\t 0. EXIT";
        cout << "\n\t-----------------------------------------\n\n\t";
        cout << "\n\t\t Enter your choice: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            if (size != 0)
            {
                for (int i = 0; i < size; i++)
                {
                    cout << *wallets[i];
                }
            }
            else
            {
                cout << "There are no wallets stored!";
            }
            break;
        case 2:
            int idWallet;
            {
                bool found = false;
                if (size != 0)
                {
                    cout << "\n\t\t Enter wallet id: ";
                    do
                    {
                        cin >> idWallet;
                        for (int i = 0; i < size; i++)
                        {
                            if (wallets[i]->getWalletId() == idWallet)
                            {
                                found = true;
                                cout << "\n\t\t Wallet found:\n\n\t";
                                cin >> *wallets[i];
                                break;
                            }
                        }
                        if (!found)
                        {
                            cout << "\n\t\t Wallet not found. Try again: ";
                        }

                    } while (idWallet < 0 || idWallet > size);
                }
            }
            break;
        case 3:
            cout << "\n\t\t Enter new wallet:\n\n\t";
            wallets[size] = new Wallet();
            cin >> *wallets[size];
            cout << "\n\t\tWallet stored succesfuly!\n\n\t";
            size++;
            break;
        case 4:
        {

            bool found = false;
            if (size != 0)
            {
                cout << "\n\t\t Enter wallet id: ";
                do
                {
                    cin >> idWallet;
                    for (int i = 0; i < size; i++)
                    {
                        if (wallets[i]->getWalletId() == idWallet)
                        {

                            found = true;
                            cout << "\n\t\t Wallet found:\n\n\t";
                            cout << *wallets[i];
                            cout << "\n\t\t Are you sure you want to delete this wallet? (y/n): ";
                            char c;
                            cin >> c;
                            if (c == 'y')
                            {

                                for (int j = i; j < size - 1; j++)
                                {
                                    wallets[j] = wallets[j + 1];
                                }
                                cout << "\n\t\t Wallet deleted succesfuly!\n\n\t";
                                size--;
                                break;
                            }
                        }
                    }
                    if (!found)
                    {
                        cout << "\n\t\t Wallet not found. Try again: ";
                    }
                } while (idWallet < 0);
            }
        }
        break;
        case 0:
            break;
        }
    } while (choice != 0);

    fin.close();
    return wallets;
}

// ------------------------------------------------------------------------------------------

class Client
{
private:
    const int clientId;
    static int totalClients;
    string clientName;
    int age;
    string dateRegistered;
    Wallet wallet;

public:
    Client() : clientId(++totalClients), clientName(""), age(0), dateRegistered(helper_functions::getDate()), wallet() {};
    Client(string clientName, int age, Wallet wallet) : clientId(++totalClients), clientName(clientName), age(age), dateRegistered(helper_functions::getDate()), wallet(wallet) {};
    Client(const Client& client) : clientId(++totalClients), clientName(client.clientName), age(client.age), dateRegistered(helper_functions::getDate()), wallet(client.wallet) {};
    Client& operator=(const Client& client)
    {
        if (this != &client)
        {
            clientName = client.clientName;
            age = client.age;
            wallet = client.wallet;
        }
        return *this;
    };
    void printClient()
    {
        cout << "\n\t\t\t CLIENT INFO";
        cout << "\n\t\t\t---------------";
        cout << "\n\t\t Client id: " << clientId;
        cout << "\n\t\t Client name: " << clientName;
        cout << "\n\t\t Client age: " << age;
        cout << "\n\t\t Client date registered: " << dateRegistered;
        cout << "\n\t\t Client wallet: " << wallet;
    };
    void editClient()
    {
        cout << "\n\t\t\t EDIT CLIENT";
        cout << "\n\t\t\t---------------";
        cout << "\n\t\t Client name: ";
        getline(cin, clientName);
        cout << "\n\t\t Client age: ";
        cin >> age;
        cout << "\n\t\t Client wallet: ";
        cin >> wallet;
    };
    int getClientId() const { return clientId; };
    string getClientName() { return clientName; };
    int getAge() { return age; };
    string getDateRegistered() { return dateRegistered; };
    Wallet getWallet() { return wallet; };

    void setClientName(string clientName) { this->clientName = clientName; };
    void setAge(int age) { this->age = age; };
    void setWallet() { cin >> wallet; };
    // sum age of 2 clients
    Client& operator+(const Client& client)
    {
        age += client.age;
        return *this;
    };
    // get funds from client wallet at certain index
    int operator[](int index)
    {
        return wallet.getFunds()[index];
    };
    Client& operator++()
    {
        age++;
        return *this;
    };
    Client operator++(int)
    {
        Client temp(*this);
        age++;
        return temp;
    };
    Client& operator--()
    {
        age--;
        return *this;
    };
    Client operator--(int)
    {
        Client temp(*this);
        age--;
        return temp;
    };
    bool operator!()
    {
        return age >= 18;
    }
    // compare total
    bool operator>=(Client& client)
    {
        int tFunds = client.wallet.getTotalFunds();
        return this->wallet.getTotalFunds() >= tFunds;
    }
    // check if 2 clients are registered in the same year
    bool operator==(Client& client)
    {

        string y1 = this->dateRegistered.substr(20, 4);
        string y2 = client.dateRegistered.substr(20, 4);
        return y1 == y2;
    }
    operator string()
    {
        return to_string(age);
    }

    void serializeClient(ofstream& w, Client client)
    {
        w.write((char*)&client.clientId, sizeof(client.clientId));
        int l = client.clientName.length() + 1;
        w.write((char*)&l, sizeof(l));
        w.write(client.clientName.c_str(), l);
        w.write((char*)&client.age, sizeof(client.age));
        l = client.dateRegistered.length() + 1;
        w.write((char*)&l, sizeof(l));
        w.write(client.dateRegistered.c_str(), l);
        client.wallet.serializeWallet(w, client.wallet);
    }

    void deserializeClient(ifstream& r, Client& client)
    {
        r.read((char*)&client.clientId, sizeof(client.clientId));
        int l;
        r.read((char*)&l, sizeof(l));
        char* c = new char[l];
        r.read(c, l);
        client.clientName = c;
        delete[] c;
        r.read((char*)&client.age, sizeof(client.age));
        r.read((char*)&l, sizeof(l));
        c = new char[l];
        r.read(c, l);
        client.dateRegistered = c;
        delete[] c;
        client.wallet.deserializeWallet(r, client.wallet);
    }
    friend ostream& operator<<(ostream& out, const Client& client);
    friend istream& operator>>(istream& in, Client& client);
};
int Client::totalClients = 0;
ostream& operator<<(ostream& out, const Client& client)
{
    out << "Client id: " << client.clientId << endl;
    out << "Client name: " << client.clientName << endl;
    out << "Client age: " << client.age << endl;
    out << "Client date registered: " << client.dateRegistered << endl;
    out << "Client wallet: " << endl;
    Wallet w = client.wallet;
    out << w;
    return out;
}
istream& operator>>(istream& in, Client& client)
{
    cout << "Client name: ";
    cin.ignore();
    getline(in, client.clientName);
    cout << "\n\tClient age: ";
    in >> client.age;
    cout << "\n\tClient wallet: \n";
    in >> client.wallet;
    return in;
}

Client** manageClients(Client** clients)
{
    Client a;
    int size = helper_functions::getFileSize(a, "clients.bin");
    clients = new Client * [size];
    ifstream r("clients.bin", ios::binary);
    if (r)
    {
        for (int i = 0; i <= size; i++)
        {
            clients[i] = new Client;
            clients[i]->deserializeClient(r, *clients[i]);
        }
    }
    else
    {
        ofstream w("clients.bin", ios::binary);
        w.close();
    }

    int choice;
    do
    {
        cout << "\n\t\t\t MANAGE CLIENTS";
        cout << "\n\t\t\t---------------";
        cout << "\n\t\t 1. Show all clients";
        cout << "\n\t\t 2. Modify client";
        cout << "\n\t\t 3. Add new client";
        cout << "\n\t\t 4. Delete client";
        cout << "\n\t\t 0. EXIT";
        cout << "\n\t-----------------------------------------\n\n\t";
        cout << "\n\t\t Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            if (size > 0)
            {
                for (int i = 0; i < size; i++)
                {
                    cout << *clients[i];
                    cout << "\n\n";
                }
            }
            else
            {
                cout << "There are no clients registered!";
            }
            break;
        case 2:
            int idClient;
            {
                bool found = false;
                if (size != 0)
                {
                    cout << "\n\t\t Enter client id: ";
                    do
                    {
                        cin >> idClient;
                        for (int i = 0; i < size; i++)
                        {
                            if (clients[i]->getClientId() == idClient)
                            {
                                found = true;
                                cout << "\n\t\t Client found:\n\n\t";
                                cin >> *clients[i];
                                break;
                            }
                        }
                        if (!found)
                        {
                            cout << "\n\t\t Client not found. Try again: ";
                        }

                    } while (idClient < 0 || idClient > size);
                }
            }
            break;
        case 3:
            cout << "\n\t\t Enter new client:\n\n\t";
            clients[size] = new Client();
            cin >> *clients[size];
            cout << "\n\t\tClient stored succesfuly!\n\n\t";
            size++;
            break;
        case 4:
        {
            bool found = false;
            if (size != 0)
            {
                cout << "\n\t\t Enter client id: ";
                do
                {
                    cin >> idClient;
                    for (int i = 0; i < size; i++)
                    {
                        if (clients[i]->getClientId() == idClient)
                        {

                            found = true;
                            cout << "\n\t\t Client found:\n\n\t";
                            cout << *clients[i];
                            cout << "\n\t\t Are you sure you want to delete this client? (y/n): ";
                            char c;
                            cin >> c;
                            if (c == 'y')
                            {

                                for (int j = i; j < size - 1; j++)
                                {
                                    clients[j] = clients[j + 1];
                                }
                                cout << "\n\t\t Client deleted succesfuly!\n\n\t";
                                size--;
                                break;
                            }
                        }
                    }
                    if (!found)
                    {
                        cout << "\n\t\t Client not found. Try again: ";
                    }
                } while (idClient < 0);
            }
        }
        break;
        case 0:
            ofstream w;
            w.open("client.bin", ios::binary);
            for (int i = 0; i < size; i++)
            {
                clients[i]->serializeClient(w, *clients[i]);
            }
            w.close();
            break;
        }
    } while (choice != 0);

    r.close();
    return clients;
}

// ------------------------------------------------------------------------------------------

class Order
{

private:
    const int orderId;
    static int totalOrders;
    string date;
    string typeOfOrder;
    char* coinSymbol;
    double orderAmount;
    double price;
    bool completed;

public:
    Order() : orderId(++totalOrders), date(helper_functions::getDate()), typeOfOrder(""), coinSymbol(nullptr), orderAmount(0), price(0), completed(false) {};
    Order(string typeOrder, char* coinSymbol, double orderAmount, double price) : orderId(++totalOrders), date(helper_functions::getDate()), typeOfOrder(typeOrder), orderAmount(orderAmount), price(price), completed(false)
    {
        if (coinSymbol != nullptr)
        {
            this->coinSymbol = new char[strlen(coinSymbol) + 1];
            strcpy_s(this->coinSymbol, strlen(coinSymbol) + 1, coinSymbol);
        }
        else
        {
            this->coinSymbol = nullptr;
        }
    };

    Order(const Order& obj) : orderId(obj.orderId), date(obj.date), typeOfOrder(obj.typeOfOrder), coinSymbol(nullptr), orderAmount(obj.orderAmount), price(obj.price), completed(obj.completed)
    {
        if (obj.coinSymbol != nullptr)
        {
            this->coinSymbol = new char[strlen(obj.coinSymbol) + 1];
            strcpy_s(this->coinSymbol, strlen(obj.coinSymbol) + 1, obj.coinSymbol);
        }
        else
        {
            this->coinSymbol = nullptr;
        }
    };
    ~Order()
    {
        if (coinSymbol != nullptr)
        {
            delete[] coinSymbol;
        }
    };

    Order& operator=(const Order& obj)
    {
        if (this != &obj)
        {
            if (coinSymbol != nullptr)
            {
                delete[] coinSymbol;
            }
            date = obj.date;
            typeOfOrder = obj.typeOfOrder;
            coinSymbol = nullptr;
            orderAmount = obj.orderAmount;
            price = obj.price;
            if (obj.coinSymbol != nullptr)
            {
                this->coinSymbol = new char[strlen(obj.coinSymbol) + 1];
                strcpy_s(this->coinSymbol, strlen(obj.coinSymbol) + 1, obj.coinSymbol);
            }
            else
            {
                this->coinSymbol = nullptr;
            }
            completed = obj.completed;
        }
        return *this;
    };
    int getOrderId() const { return orderId; };
    string getDate() { return date; };
    string getTypeOfOrder() { return typeOfOrder; };
    char* getCoinSymbol() { return coinSymbol; };
    double getorderAmount() { return orderAmount; };
    double getPrice() { return price; };
    bool getCompleted() { return completed; };
    void setTypeOfOrder(string typeOfOrder) { this->typeOfOrder = typeOfOrder; };
    void setCoinSymbol(char* coinSymbol)
    {
        if (coinSymbol != nullptr)
        {
            this->coinSymbol = new char[strlen(coinSymbol) + 1];
            strcpy_s(this->coinSymbol, strlen(coinSymbol) + 1, coinSymbol);
        }
        else
        {
            this->coinSymbol = nullptr;
        }
    };
    void setorderAmount(double orderAmount) { this->orderAmount = orderAmount; };
    void setPrice(double price) { this->price = price; };
    void setCompleted(bool completed) { this->completed = completed; };
    void printOrder()
    {
        cout << "Order ID: " << orderId << endl;
        cout << "Date: " << date << endl;
        cout << "Type of Order: " << typeOfOrder << endl;
        cout << "Coin Symbol: " << coinSymbol << endl;
        cout << "orderAmount: " << orderAmount << endl;
        cout << "Price: " << price << endl;
        cout << "Completed: " << (completed ? "true" : "false") << endl;
    }
    void editOrder()
    {
        cout << "Enter the type of order: ";
        cin >> typeOfOrder;
        cout << "Enter the coin symbol: ";
        coinSymbol = new char[4];
        cin >> coinSymbol;
        cout << "Enter the order amount: ";
        cin >> orderAmount;
        cout << "Enter the price: ";
        cin >> price;
    }

    Order operator+(int amount)
    {
        orderAmount += amount;
        return *this;
    }
    Order operator++()
    {
        orderAmount++;
        return *this;
    }
    Order operator--()
    {
        orderAmount--;
        return *this;
    }
    Order operator++(int)
    {
        Order temp = *this;
        orderAmount++;
        return temp;
    }
    Order operator--(int)
    {
        Order temp = *this;
        orderAmount--;
        return temp;
    }
    // get total value of order
    operator double()
    {
        return orderAmount* price;
    }
    // check if order amount is greater than or equal to 10 units
    bool operator!()
    {
        return orderAmount >= 10;
    }
    // check if total value of order is greater then that of another order
    bool operator>=(Order& obj)
    {
        return float(*this) > float(obj);
    }
    // checks if 2 orders are the same (coin,amount,price)
    bool operator==(const Order& obj)
    {
        return (*coinSymbol == *obj.coinSymbol) && (orderAmount == obj.orderAmount) && (price == obj.price);
    }

    friend ostream& operator<<(ostream& out, Order& obj);
    friend istream& operator>>(istream& in, Order& obj);

    void serializeOrder(ofstream& w, Order order)
    {
        w.write((char*)&order.orderId, sizeof(order.orderId));
        int l = order.date.length() + 1;
        w.write((char*)&l, sizeof(l));
        w.write(order.date.c_str(), l);
        l = order.typeOfOrder.length() + 1;
        w.write((char*)&l, sizeof(l));
        w.write(order.typeOfOrder.c_str(), l);
        w.write(coinSymbol,(long long) strlen(coinSymbol) + 1);
        w.write((char*)&order.orderAmount, sizeof(order.orderAmount));
        w.write((char*)&order.price, sizeof(order.price));
        w.write((char*)&order.completed, sizeof(order.completed));
    }

    void deserializeOrder(ifstream& r, Order& order)
    {
        r.read((char*)&order.orderId, sizeof(order.orderId));
        int l;
        r.read((char*)&l, sizeof(l));
        char* temp = new char[l];
        r.read(temp, l);
        order.date = temp;
        delete[] temp;
        r.read((char*)&l, sizeof(l));
        temp = new char[l];
        r.read(temp, l);
        order.typeOfOrder = temp;
        delete[] temp;
        delete order.coinSymbol;
        order.coinSymbol = new char[4];
        r.read(order.coinSymbol, 4);
        r.read((char*)&order.orderAmount, sizeof(order.orderAmount));
        r.read((char*)&order.price, sizeof(order.price));
        r.read((char*)&order.completed, sizeof(order.completed));
    }
};
int Order::totalOrders = 0;
ostream& operator<<(ostream& out, Order& obj)
{
    out << "Order ID: " << obj.orderId << endl;
    out << "Date: " << obj.date << endl;
    out << "Type of Order: " << obj.typeOfOrder << endl;
    out << "Coin Symbol: " << obj.coinSymbol << endl;
    out << "orderAmount: " << obj.orderAmount << endl;
    out << "Price: " << obj.price << endl;
    out << "Completed: " << (obj.completed ? "true" : "false") << endl;
    return out;
}
istream& operator>>(istream& in, Order& obj)
{
    cout << "Enter the type of order:";
    in >> ws;
    getline(in, obj.typeOfOrder);
    delete[] obj.coinSymbol;
    string temp;
    cout << "Enter the coin symbol: ";
    in >> ws;
    getline(in, temp);
    obj.coinSymbol = new char[temp.length() + 1];
    strcpy_s(obj.coinSymbol, temp.length() + 1, temp.c_str());
    cout << "Enter the order amount: ";
    in >> obj.orderAmount;
    cout << "Enter the price: ";
    in >> obj.price;
    return in;
}

class Transaction : public Order
{
public:
    Transaction() : Order() {};
    Transaction(string typeOrder, char* coinSymbol, double orderAmount, double price) : Order(typeOrder, coinSymbol, orderAmount, price)
    {
        this->setCompleted(true);
    };
    // change order to transaction
    Transaction(const Order& obj) : Order(obj)
    {
        this->setCompleted(true);
    };
};
// ------------------------------------------------------------------------------------------
Order** manageOrders(Order** orders)
{
    Order a;
    int size = helper_functions::getFileSize(a, "orders.bin");
    orders = new Order * [size];
    ifstream r("orders.bin", ios::binary);

    if (r)
    {
        for (int i = 0; i < size; i++)
        {
            orders[i] = new Order();
            orders[i]->deserializeOrder(r, *orders[i]);
        }
    }
    else
    {
        ofstream w("orders.bin", ios::binary);
        w.close();
    }

    int choice;
    do
    {
        cout << "\n\t\t\t MANAGE ORDERS";
        cout << "\n\t\t\t---------------";
        cout << "\n\t\t 1. Show all orders";
        cout << "\n\t\t 2. Modify order";
        cout << "\n\t\t 3. Add new order";
        cout << "\n\t\t 4. Delete order";
        cout << "\n\t\t 5. Complete transaction";
        cout << "\n\t\t 0. EXIT";
        cout << "\n\t-----------------------------------------\n\n\t";
        cout << "\n\t\t Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            if (size > 0)
            {
                for (int i = 0; i < size; i++)
                {
                    cout << *orders[i];
                    cout << "\n\n";
                }
            }
            else
            {
                cout << "There are no orders registered!";
            }
            break;

        case 2:
            int idOrder;
            {
                bool found = false;
                if (size != 0)
                {
                    cout << "\n\t\t Enter Order id: ";
                    do
                    {
                        cin >> idOrder;
                        for (int i = 0; i < size; i++)
                        {
                            if (orders[i]->getOrderId() == idOrder)
                            {
                                found = true;
                                cout << "\n\t\t Order found:\n\n\t";
                                cin >> *orders[i];
                                cout << "\n\t\t Order modified successfully!";
                                break;
                            }
                        }
                        if (!found)
                        {
                            cout << "\n\t\t Order not found. Try again: ";
                        }

                    } while (idOrder < 0 || !found);
                }
            }
            break;
        case 3:
            cout << "\n\t\t Enter new order:\n\n\t";
            orders[size] = new Order();
            cin >> *orders[size];
            cout << "\n\t Order stored succesfuly!\n\n\t";
            size++;
            break;
        case 4:
        {
            bool found = false;
            if (size != 0)
            {
                cout << "\n\t\t Enter order id: ";
                do
                {
                    cin >> idOrder;
                    for (int i = 0; i < size; i++)
                    {
                        if (orders[i]->getOrderId() == idOrder)
                        {

                            found = true;
                            cout << "\n\t\t Order found:\n\n\t";
                            cout << *orders[i];
                            cout << "\n\t\t Are you sure you want to delete this order? (y/n): ";
                            char c;
                            cin >> c;
                            if (c == 'y')
                            {

                                for (int j = i; j < size - 1; j++)
                                {
                                    orders[j] = orders[j + 1];
                                }
                                cout << "\n\t\t order deleted succesfuly!\n\n\t";
                                size--;
                                break;
                            }
                        }
                    }
                    if (!found)
                    {
                        cout << "\n\t\t Order not found. Try again: ";
                    }
                } while (idOrder < 0);
            }
        }
        break;
        case 5:
        {
            bool found = false;
            if (size != 0)
            {
                cout << "\n\t\t Enter the id of order to complete transaction: ";
                do
                {
                    cin >> idOrder;
                    for (int i = 0; i < size; i++)
                    {
                        if (orders[i]->getOrderId() == idOrder && orders[i]->getCompleted() == false)
                        {
                            found = true;
                            cout << "\n\t\t Order found and marked as completed!\n\n\t";
                            *orders[i] = Transaction(*orders[i]);
                            break;
                        }
                    }
                    if (!found)
                    {
                        cout << "\n\t\t Order not found or already completed. Try again: ";
                    }

                } while (idOrder < 0 || !found);
            }
        }
        break;
        case 0:
            ofstream w;
            w.open("orders.bin", ios::binary);
            for (int i = 0; i < size; i++)
            {
                orders[i]->serializeOrder(w, *orders[i]);
            }
            w.close();
            break;
        }
    } while (choice != 0);

    r.close();
    return orders;
};
// ------------------------------------------------------------------------------------------
class Block
{
private:
public:
    int blockSize;
    unsigned int hashCode;
    string timeCreated;

    Block() { blockSize = 0; hashCode = 0; }
    virtual int useHashGenerator(string data) = 0;
    virtual int BlockSize() = 0;
    virtual void submitBlock(set<Block>& blockchain)
    {
    }
    virtual void extractBlock(set<Block>& blockchain)
    {
    }

    unsigned int getHash()
    {
        return hashCode;
    }
    int getBlockSize()
    {
        return blockSize;
    }
    string getTimeCreated()
    {
        return timeCreated;
    }
    virtual ostream& operator<<(ostream& out)
    {
        return out;
    }
    virtual istream& operator<<(istream& in)
    {
        return in;
    }

    friend int operator+(Block& a, Block& b)
    {
        return a.blockSize + b.blockSize;
    }
    friend bool operator!(Block& b)
    {
        return b.blockSize == 0;
    }
    friend bool operator==(Block& a, Block& b)
    {
        return a.hashCode == b.hashCode;
    }

protected:
    unsigned int generateHash(string& information)
    {
        unsigned int hash = 0;
        for (size_t i = 0; i < information.length(); i++)
        {
            hash = information[i] + (hash << 6) + (hash << 16) - (hash);
        }
        return hash;
    }
};

class ETHblock : public Block
{
private:
    int blockNumber;
    static int totalBlocks;

public:
    ETHblock() : blockNumber(++totalBlocks)
    {
        timeCreated = helper_functions::getDate();
        blockSize = 0;
        hashCode = 0;
    };

    ETHblock(string data) : blockNumber(++totalBlocks)
    {

        hashCode = useHashGenerator(data);
        blockSize = BlockSize();
        timeCreated = helper_functions::getDate();
    }
    int useHashGenerator(string data)
    {
        string label = "ETH";
        return generateHash(data) + generateHash(label);
    }

    int BlockSize()
    {
        return sizeof(hashCode);
    }

    int getBlockNumber()
    {
        return blockNumber;
    }

    void submitBlock(set<ETHblock, greater<ETHblock>>& blockchain)
    {
        blockchain.insert(*this);
    }
    void extractBlock(set<ETHblock, greater<ETHblock>>& blockchain, int blockNumberFind)
    {
        bool found = false;
        set<ETHblock>::iterator it;
        for (it = blockchain.begin(); it != blockchain.end(); it++)
        {
            if (it->blockNumber == blockNumberFind)
            {
                found = true;
                ETHblock block = *it;
                block << cout;
                break;
            }
        }
        if (!found)
        {
            cout << "Block with that number is not on the blockchain!" << endl;
        }
    }
    ostream& operator<<(ostream& out)
    {
        cout << "Block Number: " << blockNumber << endl;
        cout << "Hash Code: " << hashCode << endl;
        cout << "Block Size: " << blockSize << endl;
        cout << "Time Created: " << timeCreated << endl;
        cout << "Total number of blocks: " << totalBlocks << endl;

        return out;
    }
    istream& operator>>(istream& in)
    {
        cout << "Hash Code: " << endl;
        cin >> hashCode;
        cout << "Block Size: " << endl;
        cin >> blockSize;

        return in;
    }
    // in order for the set to work, the block number must be unique
    bool operator>(const ETHblock& other) const
    {
        return (this->blockNumber > other.blockNumber);
    }
    ETHblock operator++(int)
    {
        ETHblock temp = *this;
        hashCode++;
        return temp;
    }
    ETHblock operator--(int)
    {
        ETHblock temp = *this;
        hashCode--;
        return temp;
    }
    ETHblock operator++()
    {
        hashCode++;
        return *this;
    }
    ETHblock operator--()
    {
        hashCode--;
        return *this;
    }

    void serializeETHblock(ofstream& w, ETHblock& block)
    {
        w.write((char*)&block.blockNumber, sizeof(block.blockNumber));
        w.write((char*)&block.hashCode, sizeof(block.hashCode));
        w.write((char*)&block.blockSize, sizeof(block.blockSize));
        int l = block.timeCreated.length() + 1;
        w.write((char*)&l, sizeof(l));
        w.write(block.timeCreated.c_str(), l);
    }

    void deserializeETHblock(ifstream& r, ETHblock& block)
    {
        r.read((char*)&block.blockNumber, sizeof(block.blockNumber));
        r.read((char*)&block.hashCode, sizeof(block.hashCode));
        r.read((char*)&block.blockSize, sizeof(block.blockSize));
        int l;
        r.read((char*)&l, sizeof(l));
        char* temp = new char[l];
        r.read(temp, l);
        block.timeCreated = temp;
        delete[] temp;
    }
};
int ETHblock::totalBlocks = 0;

class BTCblock : public Block
{
private:
    int blockNumber;
    static int totalBlocks;

public:
    BTCblock() : blockNumber(++totalBlocks)
    {
        timeCreated = helper_functions::getDate();
        blockSize = 0;
        hashCode = 0;
    };

    BTCblock(string data) : blockNumber(++totalBlocks)
    {

        hashCode = useHashGenerator(data);
        blockSize = BlockSize();
        timeCreated = helper_functions::getDate();
    }
    int useHashGenerator(string data)
    {
        string label = "BTC";
        return generateHash(data) + generateHash(label);
    }

    int BlockSize()
    {
        return sizeof(hashCode);
    }
    int getBlockNumber()
    {
        return blockNumber;
    }

    void submitBlock(set<BTCblock, greater<BTCblock>>& blockchain)
    {
        blockchain.insert(*this);
    }
    void extractBlock(set<BTCblock, greater<BTCblock>>& blockchain, int blockNumberFind)
    {
        bool found = false;
        set<BTCblock>::iterator it;
        for (it = blockchain.begin(); it != blockchain.end(); it++)
        {
            if (it->blockNumber == blockNumberFind)
            {
                found = true;
                BTCblock block = *it;
                block << cout;
                break;
            }
        }
        if (!found)
        {
            cout << "Block with that number is not on the blockchain!" << endl;
        }
    }
    ostream& operator<<(ostream& out)
    {
        cout << "Block Number: " << blockNumber << endl;
        cout << "Hash Code: " << hashCode << endl;
        cout << "Block Size: " << blockSize << endl;
        cout << "Time Created: " << timeCreated << endl;
        cout << "Total number of blocks: " << totalBlocks << endl;

        return out;
    }
    istream& operator>>(istream& in)
    {
        cout << "Hash Code: " << endl;
        cin >> hashCode;
        cout << "Block Size: " << endl;
        cin >> blockSize;

        return in;
    }
    // in order for the set to work, the block number must be unique
    bool operator>(const BTCblock& other) const
    {
        return (this->blockNumber > other.blockNumber);
    }
    BTCblock operator++(int)
    {
        BTCblock temp = *this;
        hashCode++;
        return temp;
    }
    BTCblock operator--(int)
    {
        BTCblock temp = *this;
        hashCode--;
        return temp;
    }
    BTCblock operator++()
    {
        hashCode++;
        return *this;
    }
    BTCblock operator--()
    {
        hashCode--;
        return *this;
    }

    void serializeBTCblock(ofstream& w, BTCblock& block)
    {
        w.write((char*)&block.blockNumber, sizeof(block.blockNumber));
        w.write((char*)&block.hashCode, sizeof(block.hashCode));
        w.write((char*)&block.blockSize, sizeof(block.blockSize));
        int l = block.timeCreated.length() + 1;
        w.write((char*)&l, sizeof(l));
        w.write(block.timeCreated.c_str(), l);
    }

    void deserializeBTCblock(ifstream& r, BTCblock& block)
    {
        r.read((char*)&block.blockNumber, sizeof(block.blockNumber));
        r.read((char*)&block.hashCode, sizeof(block.hashCode));
        r.read((char*)&block.blockSize, sizeof(block.blockSize));
        int l;
        r.read((char*)&l, sizeof(l));
        char* temp = new char[l];
        r.read(temp, l);
        block.timeCreated = temp;
        delete[] temp;
    }
};
int BTCblock::totalBlocks = 0;
// ------------------------------------------------------------------------------------------
void manageBlocks(set<ETHblock, greater<ETHblock>>& ETHblockchain,
    set<BTCblock, greater<BTCblock>>& BTCblockchain)
{
    ETHblock a;
    int sizeETH = helper_functions::getFileSize(a, "ETHblockchain.bin");
    int sizeBTC = helper_functions::getFileSize(a, "BTCblockchain.bin");

    ifstream rETH("ETHblockchain.bin", ios::binary);
    ifstream rBTC("BTCblockchain.bin", ios::binary);

    if (rETH)
    {
        for (int i = 0; i < sizeETH; i++)
        {
            ETHblock block;
            block.deserializeETHblock(rETH, block);
            ETHblockchain.insert(block);
        }
    }
    else
    {
        ofstream wETH("ETHblockchain.bin", ios::binary);
        wETH.close();
    }

    if (rBTC)
    {
        for (int i = 0; i < sizeBTC; i++)
        {
            BTCblock block;
            block.deserializeBTCblock(rBTC, block);
            BTCblockchain.insert(block);
        }
    }
    else
    {
        ofstream wBTC("BTCblockchain.bin", ios::binary);
        wBTC.close();
    }
    rETH.close();
    rBTC.close();

    int choice;
    do
    {
        cout << "\n\t\t\t MANAGE BLOCKS ";
        cout << "\n\t\t\t---------------";
        cout << "\n\t\t 1. Show all ETH blocks";
        cout << "\n\t\t 2. Show all BTC blocks";
        cout << "\n\t\t 3. Add new ETH block";
        cout << "\n\t\t 4. Add new BTC block";
        cout << "\n\t\t 5. View a certain ETH block";
        cout << "\n\t\t 6. View a certain BTC block";
        cout << "\n\t\t 0. EXIT";
        cout << "\n\t-----------------------------------------\n\n\t";
        cout << "\n\t\t Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            if (sizeETH > 0)
            {
                set<ETHblock, greater<ETHblock>>::iterator it;
                for (it = ETHblockchain.begin(); it != ETHblockchain.end(); it++)
                {
                    ETHblock block = *it;
                    block << cout;
                }
            }
            else
            {
                cout << "There are no ETH blocks on the blockchain!" << endl;
            }
            break;
        case 2:
            if (sizeBTC > 0)
            {
                set<BTCblock, greater<BTCblock>>::iterator it;
                for (it = BTCblockchain.begin(); it != BTCblockchain.end(); it++)
                {
                    BTCblock block = *it;
                    block << cout;
                }
            }
            else
            {
                cout << "There are no BTC blocks on the blockchain!" << endl;
            }
            break;
        case 3:
        {
            cout << "\n\t\t Add new ETH block:\n\n\t";
            ETHblock* newETHblock = new ETHblock;
            *newETHblock >> cin;
            ETHblockchain.insert(*newETHblock);
            ++sizeETH;
            delete newETHblock;
            cout << "\n\t\t New ETH block added!\n\n\t";
        }
        break;
        case 4:
        {
            cout << "\n\t\t Add new BTC block:\n\n\t";
            BTCblock* newBTCblock = new BTCblock;
            *newBTCblock >> cin;
            BTCblockchain.insert(*newBTCblock);
            ++sizeBTC;
            delete newBTCblock;
            cout << "\n\t\t New BTC block added!\n\n\t";
        }
        break;
        case 5:
            int blockNumber;
            {
                bool found = false;
                if (sizeETH != 0)
                {
                    cout << "\n\t\t Enter the block number of the ETH block you want to view: ";
                    do
                    {
                        cin >> blockNumber;
                        set<ETHblock, greater<ETHblock>>::iterator it;
                        for (it = ETHblockchain.begin(); it != ETHblockchain.end(); it++)
                        {
                            ETHblock block = *it;
                            if (block.getBlockNumber() == blockNumber)
                            {
                                block << cout;
                                found = true;
                                break;
                            }
                        }
                        if (!found)
                        {
                            cout << "\n\t\t Block  not found. Try again: ";
                        }

                    } while (blockNumber < 0 || !found);
                }
                else
                {
                    cout << "There are no ETH blocks on the blockchain!" << endl;
                }
            }
            break;
        case 6:
        {
            bool found = false;
            if (sizeBTC != 0)
            {
                cout << "\n\t\t Enter the block number of the BTC block you want to view: ";
                do
                {
                    cin >> blockNumber;
                    set<BTCblock, greater<BTCblock>>::iterator it;
                    for (it = BTCblockchain.begin(); it != BTCblockchain.end(); it++)
                    {
                        BTCblock block = *it;
                        if (block.getBlockNumber() == blockNumber)
                        {
                            block << cout;
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                    {
                        cout << "\n\t\t Block  not found. Try again: ";
                    }

                } while (blockNumber < 0 || !found);
            }
            else
            {
                cout << "There are no BTC blocks on the blockchain!" << endl;
            }
        }
        break;
        case 0:
            ofstream wETH("ETHblockchain.bin", ios::binary);
            ofstream wBTC("BTCblockchain.bin", ios::binary);
            for (set<ETHblock, greater<ETHblock>>::iterator it = ETHblockchain.begin(); it != ETHblockchain.end(); it++)
            {
                ETHblock block = *it;
                block.serializeETHblock(wETH, block);
            }
            for (set<BTCblock, greater<BTCblock>>::iterator it = BTCblockchain.begin(); it != BTCblockchain.end(); it++)
            {
                BTCblock block = *it;
                block.serializeBTCblock(wBTC, block);
            }
            wETH.close();
            wBTC.close();
            break;
        }
    } while (choice != 0);
};

int main()
{
    Client** clients = nullptr;
    Order** orders = nullptr;
    set<ETHblock, greater<ETHblock>> ETHblockchain;
    set<BTCblock, greater<BTCblock>> BTCblockchain;

    int choiceMenu;
    do
    {
        cout << "\n\t\t\t MENU ";
        cout << "\n\t\t\t----------------------";
        cout << "\n\t\t 1. Manage clients and there wallets";
        cout << "\n\t\t 2. Manage orders and complete transactions";
        cout << "\n\t\t 3. Manage blocks and add them to the blockchain";
        cout << "\n\t\t 0. EXIT";
        cout << "\n\t-----------------------------------------\n\n\t";
        cout << "\n\t\t Enter your choice: ";
        cin >> choiceMenu;

        switch (choiceMenu)
        {
        case 1:
            clients = manageClients(clients);
            break;
        case 2:
            orders = manageOrders(orders);
            break;
        case 3:
            manageBlocks(ETHblockchain, BTCblockchain);
            break;
        case 0:
            break;
        }

    } while (choiceMenu != 0);
};
