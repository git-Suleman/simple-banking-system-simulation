#include "Transaction.h"

// Constructor to initialize a transaction
Transaction::Transaction(int trans_id, int acc_id, const string &op_type, double amt)
    : transaction_id(trans_id), account_id(acc_id), operation_type(op_type), amount(amt)
{
    timestamp = get_current_time();
}

time_t Transaction::get_current_time()
{
    time_t timestamp;
    return time(&timestamp);
}

// Method to log the transaction to the file
void Transaction::log_transaction(const Transaction &transaction, const string &filename)
{
    ofstream file(filename, ios::app); // Open in append mode
    if (!file)
    {
        throw runtime_error("Error opening transaction log file.");
    }

    // Log the transaction details
    file << transaction.get_transaction_id() << ","
         << transaction.get_account_id() << ","
         << transaction.get_operation_type() << ","
         << transaction.get_amount() << ","
         << transaction.get_timestamp() << endl;

    file.close();
    cout << "Transaction logged successfully." << endl;
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

double Transaction::get_amount() const
{
    return amount;
}

string Transaction::get_timestamp() const
{
    return ctime(&timestamp);
}

pid_t Transaction::create_process(int transaction_id, int account_id, const string &operation_type, double amount)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        // Fork failed
        perror("fork");
        return -1;
    }
    else if (pid == 0)
    {
        // Child process
        cout << "Child process started (PID: " << getpid() << ", Transaction ID: " << transaction_id << ")\n";

        // Simulate transaction processing (replace with your actual logic)
        cout << "Simulating " << operation_type << " of " << amount << " for account " << account_id << "\n";
        sleep(2); // Simulate some work

        cout << "Child process finished.\n";
        exit(0); // Child process exits
    }
    else
    {
        // Parent process – pid holds the child's PID
        cout << "Parent process created child process (PID: " << pid << ")\n";
        return pid;
    }
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