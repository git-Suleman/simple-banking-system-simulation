#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <pthread.h>
#include <thread>
#include "Transaction.h"

using namespace std;

const int MAX_ACCOUNTS = 100;

class Account
{
private:
    int account_id;
    string customer_id;
    double balance;
    pthread_mutex_t mutex; // Mutex for thread-safe operations

    void deposit_amount(int amount);
    void withdraw_amount(int amount);

public:
    // Constructor
    Account(int acc_id = 0, string cust_id = "\0", double init_balance = 0.0);

    // Destructor
    ~Account();

    // API Methods
    void deposit(double amount);
    void withdraw(double amount);
    double check_balance() const;
    static void create_account(Account accounts[], int &count, const Account &new_account);

    // Getters
    int get_account_id() const;
    string get_customer_id() const;

    // database + loadings from database
    static void load_all_accounts(Account accounts[], int &count, const string &filename);
    static void save_all_accounts(const Account accounts[], const string &filename);

    // CRUD on Accounts
    static int generate_account_id();
    static bool update_account(Account accounts[], int account_id, double new_balance);
    static bool delete_account(Account accounts[], int account_id);
    static void search_by_customer_id(const Account accounts[], int count, const string &cust_id);
    static void view_all_accounts(const Account accounts[]);

    // validations
    static bool file_exists(const string &filename);
    static bool check_account(const Account accounts[], int account_id);
};

#endif // ACCOUNT_H