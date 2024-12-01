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
        accounts[acc_id % 1000] = new_account;
        cout << "Account added successfully.\n";
    }
    else
    {
        throw runtime_error("Account limit reached.");
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
    process_table.addProcess(process_id, transaction_id);

    // process scheduling
    process_table.runRoundRobin();

    // multi-threading
    thread deposit(&Account::deposit_amount, this, amount);
    deposit.join();

    // memory management
    Transaction::log_transaction(transaction, "transactions.txt");
    Transaction::log_transactions_from_memory("transactions.txt");

    process_table.printProcesses();

    pthread_mutex_unlock(&mutex);

    // remove process at completing transaction
    process_table.waitAndRemoveProcess(process_id);

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
    process_table.addProcess(process_id, transaction_id);

    // process scheduling
    process_table.runRoundRobin();

    // multi-threading
    thread withdraw(&Account::withdraw_amount, this, amount);
    withdraw.join();

    // memory management
    Transaction::log_transaction(transaction, "transactions.txt");
    Transaction::log_transactions_from_memory("transactions.txt");

    process_table.printProcesses();

    pthread_mutex_unlock(&mutex);

    // remove process at completing transaction
    process_table.waitAndRemoveProcess(process_id);

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
        cout << "No account database found. Creating a new file..." << endl;
        return; // No file to load
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
    ofstream file(filename, ios::trunc); // Overwrite the file
    if (!file)
    {
        throw runtime_error("Error opening file.");
    }

    for (int i = 0; i < MAX_ACCOUNTS; i++)
    {
        if (accounts[i].account_id != 0)
        {
            file << accounts[i].get_account_id() << ","
                 << accounts[i].get_customer_id() << ","
                 << accounts[i].check_balance() << endl;
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
        outfile << 1001; // Start from 1000
        outfile.close();
        return 1001; // Return the initial ID
    }

    int id;
    infile >> id; // Read the existing ID
    infile.close();

    // Increment the ID
    id++;

    // Write the new ID back to the file
    ofstream outfile("accountID.txt");
    if (!outfile)
    {
        throw runtime_error("Error opening accountID.txt for writing.");
    }
    outfile << id; // Save the incremented ID
    outfile.close();
    return id - 1; // Return the original ID
}

// account deletion ... // needed to be checked
bool Account::delete_account(Account accounts[], int account_id)
{
    for (int i = 0; i < MAX_ACCOUNTS; i++)
    {
        if (accounts[i].get_account_id() == account_id)
        {
            for (int j = i; j < MAX_ACCOUNTS - 1; j++)
            {
                accounts[j] = accounts[j + 1];
            }
            return true;
        }
    }
    return false;
}

bool Account::search_by_customer_id(const Account accounts[], const string &cust_id)
{
    for (int i = 0; i < MAX_ACCOUNTS; i++)
    {
        if (accounts[i].get_customer_id() == cust_id)
        {
            cout << "Account ID: " << accounts[i].get_account_id()
                 << ", Balance: " << accounts[i].check_balance() << "\n";
            return true;
        }
    }
    return false;
}

// prints all account's data
void Account::view_all_accounts(const Account accounts[])
{
    cout << "Account ID\tCustomer ID\tBalance\n";
    cout << "----------------------------------------\n";
    for (int i = 0; i < MAX_ACCOUNTS; i++)
    {
        if (accounts[i].account_id != 0)
        {
            cout << accounts[i].get_account_id() << "\t\t"
                 << accounts[i].get_customer_id() << "\t\t"
                 << accounts[i].check_balance() << "\n";
        }
    }
    cout << "----------------------------------------\n";
}

bool Account::validate_account(const Account accounts[], int account_id)
{
    for (int i = 0; i < MAX_ACCOUNTS; i++)
    {
        if (accounts[i].get_account_id() == account_id)
        {
            return true;
        }
    }
    return false;
}

// checking database status
bool Account::file_exists(const string &filename)
{
    ifstream file(filename);
    return file.good();
}