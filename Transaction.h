#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

// frames
const int MAX_PAGES = 5;

class Transaction
{
private:
    int transaction_id;
    int account_id;
    string operation_type;
    int amount;
    time_t timestamp;

public:
    Transaction() {}
    Transaction(int trans_id, int acc_id, const string &op_type, int amt);

    // record transaction time
    static time_t get_current_time();

    // getters
    int get_transaction_id() const;
    int get_account_id() const;
    string get_operation_type() const;
    int get_amount() const;
    string get_timestamp() const;

    // transaction_id generation + process creation
    static int generate_transaction_id();
    static pid_t create_process(int transaction_id, int account_id, const string &operation_type, int amount);

    // memory management + paging
    static void add_transaction_to_memory(const Transaction &transaction);
    static void log_transactions_from_memory(const string &filename);

    // save transaction data to database(transactions.txt)
    static void log_transaction(const Transaction &transaction, const string &filename);
};

#endif