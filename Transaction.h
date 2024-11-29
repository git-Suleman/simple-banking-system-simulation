#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

using namespace std;

const int MAX_PAGES = 5; // Maximum number of transactions that can be stored in memory (simulated pages)

class Transaction
{
private:
    int transaction_id;
    int account_id;
    string operation_type;
    double amount;
    time_t timestamp;

public:
    // Constructor
    Transaction();
    Transaction(int trans_id, int acc_id, const string &op_type, double amt);

    // Method to save the transaction to a file
    static void log_transaction(const Transaction &transaction, const string &filename);

    // Helper method to generate timestamp for the transaction
    static time_t get_current_time();

    // Getter for transaction details
    int get_transaction_id() const;
    int get_account_id() const;
    string get_operation_type() const;
    double get_amount() const;
    string get_timestamp() const;

    static int generate_transaction_id();
    static pid_t create_process(int transaction_id, int account_id, const string &operation_type, double amount);

    // Memory management
    static void add_transaction_to_memory(const Transaction &transaction);
    static void log_transactions_from_memory(const string &filename);
};

#endif // TRANSACTION_H