#include "Account.h"

#define MAX_PROESSES 10
#define TIME_QUANTUM 1

ProcessTable process_table(MAX_PROESSES, TIME_QUANTUM);

Account::Account(int acc_id, string cust_id, int init_balance)
    : account_id(acc_id), customer_id(cust_id), balance(init_balance)
{
    pthread_mutex_init(&mutex, nullptr);
}

Account::~Account()
{
    account_id = 0;
    balance = 0;
    customer_id = "\0";
    pthread_mutex_destroy(&mutex);
}

void Account::deposit_amount(int amount)
{
    balance += amount;
}

void Account::withdraw_amount(int amount)
{
    balance -= amount;
}

// API # 01 (create_account())
void Account::create_account(Account accounts[], int &count, const Account &new_account)
{
    if (count < MAX_ACCOUNTS)
    {
        int acc_id = new_account.account_id;

        pid_t pid = fork();

        if (pid == 0)
        {
            // child process
            cout << "Creating an account for Account ID: " << acc_id << endl;
            exit(0);
        }
        else if (pid > 0)
        {
            // parent process
            accounts[acc_id % 1000] = new_account;
            count++;
            cout << "Account created!!!" << endl;
        }
        else
        {
            // fork() failed
            cout << "Failed to create a child process." << endl;
            return;
        }
    }
    else
    {
        cout << "Account limit reached" << endl;
        return;
    }
}

// API # 02 (deposit())
void Account::deposit(int amount)
{
    // synchronization
    pthread_mutex_lock(&mutex);

    int transaction_id = Transaction::generate_transaction_id();
    Transaction transaction(transaction_id, account_id, "Deposit", amount);

    // memory management
    Transaction::add_transaction_to_memory(transaction);

    // process creation
    pid_t process_id = Transaction::create_process(transaction_id, account_id, "Deposit", amount);
    process_table.add_process(process_id, transaction_id);

    // process scheduling
    process_table.run_round_robin();

    // multi-threading
    thread deposit(&Account::deposit_amount, this, amount);
    deposit.join();

    // memory management
    Transaction::log_transaction(transaction, "transactions.txt");
    Transaction::log_transactions_from_memory("transactions.txt");

    process_table.print_processes();

    pthread_mutex_unlock(&mutex);

    // remove process at completing transaction
    process_table.wait_and_remove_process(process_id);

    cout << "Deposit successful. New Balance: " << balance << endl;
}

// API # 03 (withdraw())
void Account::withdraw(int amount)
{
    // synchronization
    pthread_mutex_lock(&mutex);

    // validitation
    if (balance < amount)
    {
        pthread_mutex_unlock(&mutex);
        throw runtime_error("Insufficient balance.");
    }

    int transaction_id = Transaction::generate_transaction_id();
    Transaction transaction(transaction_id, account_id, "Withdrawal", amount);

    // process creation
    pid_t process_id = Transaction::create_process(transaction_id, account_id, "Withdrawal", amount);
    process_table.add_process(process_id, transaction_id);

    // process scheduling
    process_table.run_round_robin();

    // multi-threading
    thread withdraw(&Account::withdraw_amount, this, amount);
    withdraw.join();

    // memory management
    Transaction::log_transaction(transaction, "transactions.txt");
    Transaction::log_transactions_from_memory("transactions.txt");

    process_table.print_processes();

    pthread_mutex_unlock(&mutex);

    // remove process at completing transaction
    process_table.wait_and_remove_process(process_id);

    cout << "Withdrawal successful. New Balance: " << balance << endl;
}

// API # 04 (check_balance())
int Account::check_balance() const
{
    return balance;
}

int Account::get_account_id() const
{
    return account_id;
}

string Account::get_customer_id() const
{
    return customer_id;
}

// load all data into Account array
void Account::load_all_accounts(Account accounts[], int &count, const string &filename)
{
    if (!file_exists(filename))
    {
        return;
    }

    ifstream file(filename);
    if (!file)
    {
        throw runtime_error("Error opening file.");
    }

    int acc_id;
    string cust_id;
    int bal;
    string line;
    count = 0;

    while (getline(file, line) && count < MAX_ACCOUNTS)
    {
        stringstream ss(line);
        string item;

        getline(ss, item, ',');
        acc_id = stoi(item);

        getline(ss, item, ',');
        cust_id = item;

        getline(ss, item, ',');
        bal = stoi(item);

        count = acc_id % 1000;

        accounts[count] = Account(acc_id, cust_id, bal);
    }

    file.close();
}

// save all data into database(account.txt)
void Account::save_all_accounts(const Account accounts[], const string &filename)
{
    // overwriting the database(accounts.txt)
    ofstream file(filename, ios::trunc);
    if (!file)
    {
        throw runtime_error("Error opening file.");
    }

    for (int i = 0; i < MAX_ACCOUNTS; i++)
    {
        if (accounts[i].account_id != 0)
        {
            file << accounts[i].get_account_id() << ",";
            file << accounts[i].get_customer_id() << ",";
            file << accounts[i].check_balance() << endl;
        }
    }

    file.close();
    cout << "Database updated successfully." << endl;
}

int Account::generate_account_id()
{
    ifstream infile("accountID.txt");
    if (!infile)
    {
        ofstream outfile("accountID.txt");
        if (!outfile)
        {
            throw runtime_error("Error creating accountID.txt");
        }
        outfile << 1001; // start account_id from 1001
        outfile.close();
        return 1001;
    }

    int id;
    infile >> id;
    infile.close();

    // auto increment account_id
    id++;

    // write account_id after incrementing it
    ofstream outfile("accountID.txt");
    if (!outfile)
    {
        throw runtime_error("Error opening accountID.txt for writing.");
    }
    outfile << id;
    outfile.close();
    return id - 1; // return account_id
}

// account deletion
bool Account::delete_account(Account accounts[], int account_id)
{
    int acc_id = account_id % 1000;
    if (accounts[acc_id].get_account_id() == account_id)
    {
        accounts[acc_id].account_id = 0;
        accounts[acc_id].balance = 0;
        accounts[acc_id].customer_id = "\0";
        cout << "Account Deleted Successfully!" << endl;
        return true;
    }
    return false;
}

void Account::search_by_account_id(const Account accounts[], const int &account_id)
{
    cout << "Account found" << endl;
    cout << "----------------------------------------------" << endl;
    cout << "Customer's ID: " << accounts[account_id - 1000].get_customer_id() << endl;
    cout << "Account's ID: " << accounts[account_id - 1000].get_account_id() << endl;
    cout << "Current balance: " << accounts[account_id - 1000].check_balance() << endl;
    cout << "----------------------------------------------" << endl;
}

bool Account::search_by_customer_id(const Account accounts[], const string &cust_id)
{
    for (int i = 0; i < MAX_ACCOUNTS; i++)
    {
        if (accounts[i].get_customer_id() == cust_id)
        {
            cout << "Account ID: " << accounts[i].get_account_id();
            cout << ", Balance: " << accounts[i].check_balance() << endl;
            return true;
        }
    }
    return false;
}

// prints all account's data
void Account::view_all_accounts(const Account accounts[])
{
    int accounts_count = 0;
    cout << "Account ID\tCustomer ID\tBalance\n";
    cout << "----------------------------------------\n";
    for (int i = 0; i < MAX_ACCOUNTS; i++)
    {
        if (accounts[i].account_id != 0)
        {
            accounts_count++;
            cout << accounts[i].get_account_id() << "\t\t";
            cout << accounts[i].get_customer_id() << "\t\t";
            cout << accounts[i].check_balance() << endl;
        }
    }
    if (accounts_count == 0)
    {
        cout << "No data found!" << endl;
    }
    cout << "----------------------------------------\n";
}

// checking database status
bool Account::file_exists(const string &filename)
{
    ifstream file(filename);
    return file.good();
}

bool Account::validate_account(const Account accounts[], int account_id)
{
    int acc_id = account_id % 1000;
    if (acc_id > 99 || acc_id <= 0)
    {
        return false;
    }

    if (accounts[acc_id].get_account_id() == account_id)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Account::validate_account_by_customerID(const Account accounts[], string customer_id)
{
    for (int i = 0; i < MAX_ACCOUNTS; i++)
    {
        if (accounts[i].get_customer_id() == customer_id)
        {
            return true;
        }
    }
    return false;
}