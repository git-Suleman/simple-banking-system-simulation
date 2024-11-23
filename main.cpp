#include "Account.h"

int main()
{
    Account accounts[MAX_ACCOUNTS];
    int account_count = 0;
    string filename = "accounts.txt";

    try
    {
        // Load accounts from file
        Account::load_all_accounts(accounts, account_count, filename);

        Account::view_all_accounts(accounts);

        while (true)
        {
            cout << "\n-- Simple Banking System --\n";
            cout << "1. Create Account\n";
            cout << "2. Deposit\n";
            cout << "3. Withdraw\n";
            cout << "4. Check Balance\n";
            cout << "5. Delete Account\n";
            cout << "6. Exit\n";
            cout << "Enter your choice: ";
            int choice;
            cin >> choice;

            if (choice == 1)
            {
                int acc_id;
                string cust_id;
                double balance;
                acc_id = Account::generate_account_id();
                cout << "Enter customer ID: ";
                cin >> cust_id;
                cout << "Enter initial balance: ";
                cin >> balance;

                Account new_account(acc_id, cust_id, balance);
                Account::add_account(accounts, account_count, new_account);

                Account::save_all_accounts(accounts, filename);
            }
            else if (choice == 2)
            {
                int acc_id;
                double amount;
                cout << "Enter account ID: ";
                cin >> acc_id;
                cout << "Enter amount to deposit: ";
                cin >> amount;

                if (amount > 0)
                {
                    if (Account::update_account(accounts, acc_id, amount))
                    {
                        Account::save_all_accounts(accounts, filename);
                    }
                    else
                    {
                        cout << "Account not found.\n";
                    }
                }
                else
                {
                    cout << "Negative amount cannot be deposited." << endl;
                }
            }
            else if (choice == 3)
            {
                int acc_id;
                double amount;
                cout << "Enter account ID: ";
                cin >> acc_id;
                cout << "Enter amount to withdraw: ";
                cin >> amount;

                if (amount > 0)
                {
                    try
                    {
                        if (Account::check_account(accounts, acc_id))
                        {
                            Account &acc = accounts[acc_id % 1000];
                            acc.withdraw(amount);
                            Account::save_all_accounts(accounts, filename);
                        }
                        else
                        {
                            cout << "Account not found.\n";
                        }
                    }
                    catch (const runtime_error &e)
                    {
                        cout << e.what() << endl;
                    }
                }
                else
                {
                    cout << "Negative amount cannot be withdrawn." << endl;
                }
            }
            else if (choice == 4)
            {
                int acc_id;
                cout << "Enter account ID: ";
                cin >> acc_id;

                if (Account::check_account(accounts, acc_id))
                {
                    cout << "Current balance: " << accounts[acc_id - 1000].check_balance() << endl;
                }
                else
                {
                    cout << "Account not found.\n";
                }
            }
            else if (choice == 5)
            {
                int acc_id;
                cout << "Enter account ID to delete: ";
                cin >> acc_id;

                if (Account::delete_account(accounts, account_count, acc_id))
                {
                    Account::save_all_accounts(accounts, filename);
                }
                else
                {
                    cout << "Account not found.\n";
                }
            }
            else if (choice == 6)
            {
                cout << "Exiting program...\n";
                break;
            }
            else
            {
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