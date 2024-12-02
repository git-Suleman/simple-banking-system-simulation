#include "Account.h"

// validate if the entered string is interger
bool validate_integer(string input)
{
    for (int i = 0; i < input.length(); i++)
    {
        if (!isdigit(input[i]))
        {
            return false;
        }
    }
    return true;
}

// get valid input if user give wrong input(max 3 times)
bool get_valid_input(const string prompt, string &input)
{
    for (int i = 0; i < 3; i++)
    {
        cout << prompt;
        getline(cin, input);
        if (validate_integer(input))
        {
            return true;
        }
        cout << "Invalid input!, try Again!" << endl;
    }
    cout << "Too much invalid attempts!!!" << endl;
    cout << "Better Luck next time!" << endl;
    return false;
}

// globle function for account_number validation
bool get_account_id(string acc_id, int &account_id)
{
    string prompt = "Enter account ID: ";
    if (!get_valid_input(prompt, acc_id))
    {
        return false;
    }
    account_id = stoi(acc_id);
    return true;
}

int main()
{
    Account accounts[MAX_ACCOUNTS];
    int valid_count = 0;
    int account_count = 0;
    string filename = "accounts.txt";

    try
    {
        // load data from accounts.txt
        Account::load_all_accounts(accounts, account_count, filename);

        Account::view_all_accounts(accounts);

        while (true)
        {
            cout << "\n----- Simple Banking System -----" << endl;
            cout << "1. Create Account" << endl;
            cout << "2. Deposit" << endl;
            cout << "3. Withdraw" << endl;
            cout << "4. Check Balance" << endl;
            cout << "5. Delete Account" << endl;
            cout << "6. Search account by AccountID" << endl;
            cout << "7. Search account by CustomerID" << endl;
            cout << "8. View All Accounts" << endl;
            cout << "9. Exit" << endl;
            cout << "Enter your choice: ";
            string choice;
            getline(cin, choice);

            if (choice == "1")
            {
                int acc_id;
                string cust_id;
                string balance;

                cout << "Enter customer ID: ";
                getline(cin, cust_id);

                string prompt = "Enter initial balance: ";
                if (!get_valid_input(prompt, balance))
                {
                    return 0;
                }
                int bal = stoi(balance);

                acc_id = Account::generate_account_id();

                Account new_account(acc_id, cust_id, bal);
                Account::create_account(accounts, account_count, new_account);

                Account::save_all_accounts(accounts, filename);
            }
            else if (choice == "2")
            {
                string acc_id;
                string amt;
                int account_id;

                if (!get_account_id(acc_id, account_id))
                {
                    return 0;
                }

                if (!Account::validate_account(accounts, account_id))
                {
                    cout << "Account doesn't exist!" << endl;
                    return 0;
                }

                string prompt = "Enter amount to deposit: ";
                if (!get_valid_input(prompt, amt))
                {
                    return 0;
                }
                int amount = stoi(amt);

                if (amount > 0)
                {
                    Account &acc = accounts[account_id % 1000];
                    acc.withdraw(amount);
                    Account::save_all_accounts(accounts, filename);
                }
                else
                {
                    cout << "Negative amount cannot be deposited" << endl;
                }
            }
            else if (choice == "3")
            {
                string acc_id;
                string amt;
                int account_id;

                if (!get_account_id(acc_id, account_id))
                {
                    return 0;
                }

                if (!Account::validate_account(accounts, account_id))
                {
                    cout << "Account doesn't exist!" << endl;
                    return 0;
                }

                string prompt = "Enter amount to withdraw: ";
                if (!get_valid_input(prompt, amt))
                {
                    return 0;
                }
                int amount = stoi(amt);

                if (amount > 0)
                {
                    Account &acc = accounts[account_id % 1000];
                    acc.withdraw(amount);
                    Account::save_all_accounts(accounts, filename);
                }
                else
                {
                    cout << "Negative amount cannot be withdrawn" << endl;
                }
            }
            else if (choice == "4")
            {
                string acc_id;
                int account_id;

                if (!get_account_id(acc_id, account_id))
                {
                    return 0;
                }

                if (!Account::validate_account(accounts, account_id))
                {
                    cout << "Account doesn't exist!" << endl;
                    return 0;
                }
                else
                {
                    cout << "Current balance: " << accounts[account_id - 1000].check_balance() << endl;
                }
            }
            else if (choice == "5")
            {
                string acc_id;
                int account_id;

                if (!get_account_id(acc_id, account_id))
                {
                    return 0;
                }

                if (Account::delete_account(accounts, account_id))
                {
                    Account::save_all_accounts(accounts, filename);
                }
                else
                {
                    cout << "Account not found.\n";
                }
            }
            else if (choice == "6")
            {
                string acc_id;
                int account_id;

                if (!get_account_id(acc_id, account_id))
                {
                    return 0;
                }

                if (!Account::validate_account(accounts, account_id))
                {
                    cout << "Account doesn't exist!" << endl;
                    return 0;
                }
                else
                {
                    cout << "Account found" << endl;
                    cout << "----------------------------------------------" << endl;
                    cout << "Customer's ID: " << accounts[account_id - 1000].get_customer_id() << endl;
                    cout << "Account's ID: " << accounts[account_id - 1000].get_account_id() << endl;
                    cout << "Current balance: " << accounts[account_id - 1000].check_balance() << endl;
                    cout << "----------------------------------------------" << endl;
                }
            }
            else if (choice == "7")
            {
                string customer_id;
                cout << "Enter CustomerID: ";
                getline(cin, customer_id);
                if (!Account::validate_account_by_customerID(accounts, customer_id))
                {
                    cout << "Invalid CustomerID!" << endl;
                    return 0;
                }
                Account::search_by_customer_id(accounts, customer_id);
            }
            else if (choice == "8")
            {
                Account::view_all_accounts(accounts);
            }
            else if (choice == "9")
            {
                cout << "Terminating program\n";
                break;
            }
            else
            {
                if (valid_count == 3)
                {
                    cout << "Too much invalid attempts!!!" << endl;
                    cout << "Better Luck next time!" << endl;
                    break;
                }
                valid_count++;
                cout << "Invalid choice. Please try again.\n";
            }
        }
    }
    catch (const exception &e)
    {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}