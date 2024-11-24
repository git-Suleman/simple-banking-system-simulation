#include "Account.h"
#include "ProcessTable.h"

ProcessTable process_table(100);

Account::Account(int acc_id, string cust_id, double init_balance)
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

void Account::deposit(double amount)
{
    pthread_mutex_lock(&mutex);

    int transaction_id = generate_transaction_id();
    balance += amount;

    Transaction transaction(transaction_id, account_id, "Deposit", amount);
    pid_t process_id = Transaction::create_process(transaction_id, account_id, "Deposit", amount);
    process_table.addProcess(process_id, transaction_id);
    Transaction::log_transaction(transaction, "transactions.txt");

    process_table.printProcesses();

    pthread_mutex_unlock(&mutex);

    process_table.waitAndRemoveProcess(process_id);

    cout << "Deposit successful. New Balance: " << balance << endl;
}

void Account::withdraw(double amount)
{
    pthread_mutex_lock(&mutex);
    if (balance < amount)
    {
        pthread_mutex_unlock(&mutex);
        throw runtime_error("Insufficient balance.");
    }
    int transaction_id = generate_transaction_id();
    balance -= amount;

    pid_t process_id = Transaction::create_process(transaction_id, account_id, "Withdrawal", amount);
    process_table.addProcess(process_id, transaction_id);
    Transaction transaction(transaction_id, account_id, "Withdrawal", amount);
    Transaction::log_transaction(transaction, "transactions.txt");

    process_table.printProcesses();

    pthread_mutex_unlock(&mutex);

    process_table.waitAndRemoveProcess(process_id);

    cout << "Withdrawal successful. New Balance: " << balance << endl;
}

double Account::check_balance() const
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

// Check if the file exists
bool Account::file_exists(const string &filename)
{
    ifstream file(filename);
    return file.good();
}

// Load all accounts from the file
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
    double bal;
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
        bal = stod(item);

        count = acc_id % 1000;

        accounts[count] = Account(acc_id, cust_id, bal);
    }

    file.close();
}

// Save all accounts to the file
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

// Add a new account to the array
void Account::add_account(Account accounts[], int &count, const Account &new_account)
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

// Update an existing account balance
bool Account::update_account(Account accounts[], int account_id, double new_balance)
{
    for (int i = 0; i < MAX_ACCOUNTS; i++)
    {
        if (accounts[i].get_account_id() == account_id)
        {
            accounts[i].deposit(new_balance);
            return true;
        }
    }
    return false;
}

// Delete an account from the array
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

bool Account::check_account(const Account accounts[], int account_id)
{
    for (int i = 0; i < MAX_ACCOUNTS; i++)
    {
        if (accounts[i].get_account_id() == account_id)
        {
            return true; // Account found
        }
    }
    return false; // Account not found
}

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
}

void Account::search_by_customer_id(const Account accounts[], int count, const string &cust_id)
{
    bool found = false;
    for (int i = 0; i < count; i++)
    {
        if (accounts[i].get_customer_id() == cust_id)
        {
            cout << "Account ID: " << accounts[i].get_account_id()
                 << ", Balance: " << accounts[i].check_balance() << "\n";
            found = true;
        }
    }
    if (!found)
    {
        cout << "No accounts found for Customer ID: " << cust_id << "\n";
    }
}

int Account::generate_transaction_id()
{
    ifstream infile("transactionID.txt");
    if (!infile)
    {
        ofstream outfile("transactionID.txt");
        if (!outfile)
        {
            throw runtime_error("Error creating transactionID.txt");
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
    ofstream outfile("transactionID.txt");
    if (!outfile)
    {
        throw runtime_error("Error opening transactionID.txt for writing.");
    }
    outfile << id; // Save the incremented ID
    outfile.close();
    return id - 1; // Return the original ID
}