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