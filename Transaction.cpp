#include "Transaction.h"

// paging for transactions
struct Page
{
    Transaction transaction;
    bool in_use;
};

Page pages[MAX_PAGES];

Transaction::Transaction(int trans_id, int acc_id, const string &op_type, int amt)
    : transaction_id(trans_id), account_id(acc_id), operation_type(op_type), amount(amt)
{
    timestamp = get_current_time();
}

time_t Transaction::get_current_time()
{
    time_t timestamp;
    return time(&timestamp);
}

// Getter methods for transaction details
int Transaction::get_transaction_id() const
{
    return transaction_id;
}

int Transaction::get_account_id() const
{
    return account_id;
}

string Transaction::get_operation_type() const
{
    return operation_type;
}

int Transaction::get_amount() const
{
    return amount;
}

string Transaction::get_timestamp() const
{
    return ctime(&timestamp);
}

int Transaction::generate_transaction_id()
{
    ifstream infile("transactionID.txt");
    if (!infile)
    {
        ofstream outfile("transactionID.txt");
        if (!outfile)
        {
            throw runtime_error("Error creating transactionID.txt");
        }
        outfile << 10001; // start transaction_id from 10001
        outfile.close();
        return 10001;
    }

    int id;
    infile >> id;
    infile.close();

    // auto increment transaction_id
    id++;

    // write transaction_id after incrementing it
    ofstream outfile("transactionID.txt");
    if (!outfile)
    {
        throw runtime_error("Error opening transactionID.txt for writing.");
    }
    outfile << id;
    outfile.close();
    return id - 1; // return transaction_id
}

// create process while doing transaction
pid_t Transaction::create_process(int transaction_id, int account_id, const string &operation_type, int amount)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        cout << "fork() failed!" << endl;
        return -1;
    }
    else if (pid == 0)
    {
        // child process
        cout << "Child process started (PID: " << getpid() << ", Transaction ID: " << transaction_id << ")\n";

        // transaction processing
        cout << "Simulating " << operation_type << " of " << amount << " for account " << account_id << "\n";
        sleep(2);

        cout << "Child process finished.\n";
        exit(0);
    }
    else
    {
        // Parent process – pid holds the child's PID
        cout << "Parent process created child process (PID: " << pid << ")\n";
        return pid;
    }
}

void Transaction::add_transaction_to_memory(const Transaction &transaction)
{
    static int next_page = 0; // keep track of the next available page

    if (next_page >= MAX_PAGES)
    {
        cout << "Memory is full, replacing the oldest transaction (Page " << next_page % MAX_PAGES << ").\n";
    }

    pages[next_page % MAX_PAGES] = {transaction, true};
    next_page++;
}

// logs all transactions to database(transactions.txt)
void Transaction::log_transactions_from_memory(const string &filename)
{
    for (int i = 0; i < MAX_PAGES; i++)
    {
        if (pages[i].in_use)
        {
            log_transaction(pages[i].transaction, filename);
        }
    }
}

// save all data into database(transactions.txt)
void Transaction::log_transaction(const Transaction &transaction, const string &filename)
{
    ofstream file(filename, ios::app); // overwriting the file
    if (!file)
    {
        throw runtime_error("Error opening transaction log file.");
    }

    file << transaction.get_transaction_id() << ",";
    file << transaction.get_account_id() << ",";
    file << transaction.get_operation_type() << ",";
    file << transaction.get_amount() << ",";
    file << transaction.get_timestamp() << endl;

    file.close();
    cout << "Transaction logged successfully." << endl;
}