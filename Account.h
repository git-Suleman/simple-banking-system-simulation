#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <sstream>
#include <pthread.h>
#include <thread>
#include <cctype>
#include "Transaction.h"
#include "ProcessTable.h"

using namespace std;

const int MAX_ACCOUNTS = 100;

class Account
{
private:
    int account_id;
    string customer_id;
    int balance;
    pthread_mutex_t mutex; // Mutex for thread-safe operations

    void deposit_amount(int amount);
    void withdraw_amount(int amount);

public:
    Account(int acc_id = 0, string cust_id = "\0", int init_balance = 0);
    ~Account();

    // API methods
    static void create_account(Account accounts[], int &count, const Account &new_account);
    void deposit(int amount);
    void withdraw(int amount);
    int check_balance() const;

    // getters
    int get_account_id() const;
    string get_customer_id() const;

    // database + loadings from database
    static void load_all_accounts(Account accounts[], int &count, const string &filename);
    static void save_all_accounts(const Account accounts[], const string &filename);

    // CRUD on Accounts
    static int generate_account_id();
    static bool delete_account(Account accounts[], int account_id);
    static bool search_by_customer_id(const Account accounts[], const string &cust_id);
    static void view_all_accounts(const Account accounts[]);

    // validations
    static bool file_exists(const string &filename);
    static bool validate_account(const Account accounts[], int account_id);
    static bool validate_account_by_customerID(const Account accounts[], string customer_id);
};

#endif