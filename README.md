# 🏦 A Simple Banking System Simulation

Welcome to the **Simple Banking System Simulation**! This project simulates a banking application where multiple customers can perform concurrent transactions such as account creation, deposits, withdrawals, and balance checks. It demonstrates key operating system functionalities such as process management, multithreading, CPU scheduling, memory management, and inter-process communication (IPC).

---

## 🎯 Features

### 1. **System Call Interface**
   - Provides API functions to manage accounts and transactions:
     - `create_account(customer_id, initial_balance)`
     - `deposit(account_id, amount)`
     - `withdraw(account_id, amount)`
     - `check_balance(account_id)`

### 2. **Process Creation**
   - Tracks and manages transactions as individual processes.
   - Each transaction is represented as a separate process, ensuring efficient management.

### 3. **Multithreading & Synchronization**
   - Enables concurrent transaction handling through threads.
   - Mutexes and semaphores are used to prevent race conditions and ensure data consistency.

### 4. **CPU Scheduling**
   - Implements a **Round Robin** scheduling algorithm to allocate CPU time.
   - Displays a Gantt chart visualizing CPU allocation for each transaction.

### 5. **Memory Management & Paging**
   - Implements an LRU-based page replacement algorithm for efficient memory management.

### 6. **Inter-Process Communication (IPC)**
   - Facilitates communication between transaction processes using message queues.

---

## 🖥 Example Workflow

1. A customer creates an account using the system call interface.
2. Multiple transactions (e.g., deposits, withdrawals) are handled concurrently by separate threads.
3. Synchronization mechanisms ensure only one thread can modify account data at a time.
4. The CPU allocates time to processes using Round Robin scheduling.
5. Memory is managed via paging to store account and transaction data efficiently.
6. Processes communicate through IPC to share transaction statuses.

---

## 🚀 How to Clone and Run the Project

### Step 1: Clone the Repository
To get started, clone the repository to your local machine using the following command:

```bash
git clone https://github.com/sulemaniftikhar/simple-banking-system-simulation.git
cd simple-banking-system-simulation
```

### Step 2: Build the Project
In the project folder, run the following command on terminal to compile the C++ files:

```bash
g++ -std=c++11 -pthread Account.cpp Transaction.cpp ProcessTable.cpp main.cpp -o run
```

### Step 3: Run the Program
After compilation, you can run the simulation with this command:

```bash
./run
```

---

## ⚠️ Limitations

This is a basic prototype and is not perfect. There are still many improvements that can be made, including:

- Enhanced error handling for invalid transactions and account management.
- Optimizing synchronization and performance.
- Adding more complex scheduling and memory management techniques.

---

## 🤝 Contributions

Feel free to contribute! Whether it's improving functionality, fixing bugs, or enhancing documentation, your contributions are welcome.

1. Fork the repository.
2. Create a new branch for your changes.
3. Submit a pull request.

---

## 📚 Acknowledgments

This project was developed as part of the **Operating Systems Lab (Fall 2024)**.

---

## 🖤 Special Thanks

Thanks to all the contributors and mentors who provided support and feedback!
