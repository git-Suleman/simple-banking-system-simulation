#include "ProcessTable.h"

ProcessTable::ProcessTable(int size, int quantum)
    : size(size), quantum(quantum)
{
    processes = new Process[size];
}

ProcessTable::~ProcessTable()
{
    delete[] processes;
}

// add process to processTable + IPC via pipes
bool ProcessTable::add_process(pid_t pid, int transaction_id)
{
    for (int i = 0; i < size; ++i)
    {
        if (processes[i].pid == 0)
        {
            processes[i].pid = pid;
            processes[i].transaction_id = transaction_id;
            processes[i].status = "Waiting";
            setup_pipe(processes[i]);
            return true;
        }
    }
    return false;
}

// active processes in processTable
void ProcessTable::print_processes()
{
    for (int i = 0; i < size; ++i)
    {
        if (processes[i].pid != 0)
        {
            cout << "Process (PID: " << processes[i].pid << endl;
            cout << "Transaction ID: " << processes[i].transaction_id << endl;
            cout << "Status: " << processes[i].status << endl;
        }
    }
}

// removes process after it's execution
void ProcessTable::wait_and_remove_process(pid_t pid)
{
    for (int i = 0; i < size; ++i)
    {
        if (processes[i].pid == pid)
        {
            int status;
            waitpid(pid, &status, 0);
            cout << "Child process (PID: " << pid << ") terminated." << endl;
            cleanup_pipe(processes[i]);
            processes[i].pid = 0;
            processes[i].status = "Finished";
            return;
        }
    }
}

// scheduling algorithms for fair execution
void ProcessTable::run_round_robin()
{
    print_gantt_chart();
    bool hasActiveProcesses = true;

    while (hasActiveProcesses)
    {
        hasActiveProcesses = false;

        for (int i = 0; i < size; ++i)
        {
            if (processes[i].pid != 0 && processes[i].status != "Finished")
            {
                hasActiveProcesses = true;

                processes[i].status = "Running";
                cout << "Executing process PID: " << processes[i].pid << endl;
                cout << "Transaction ID: " << processes[i].transaction_id << " for " << quantum << " seconds" << endl;
                print_processes();
                sleep(quantum);

                // status update to waiting after time quantum
                processes[i].status = "Waiting";

                string message = "Process in waiting queue";
                write(processes[i].pipe_fd[1], message.c_str(), message.length() + 1);
                cout << "Status update sent to pipe for PID " << processes[i].pid << endl;

                // read from the pipe after writing
                read_pipe_message(processes[i]);
                wait_and_remove_process(processes[i].pid);
            }
        }
    }
    cout << "All processes have completed.\n";
}

// visualization of processes execution
void ProcessTable::print_gantt_chart()
{
    cout << "\nGantt Chart:" << endl;
    cout << "-----------------------------------------" << endl;

    for (int i = 0; i < size; ++i)
    {
        if (processes[i].pid != 0)
        {
            cout << "| PID: " << processes[i].pid << " ";
        }
    }
    cout << "|" << endl;
    cout << "-----------------------------------------" << endl;
}

// pipe for IPC
void ProcessTable::setup_pipe(Process &process)
{
    if (pipe(process.pipe_fd) == -1)
    {
        cout << "Pipe creation failed" << endl;
        return;
    }
}

// destroy pipe after process finishes
void ProcessTable::cleanup_pipe(Process &process)
{
    close(process.pipe_fd[0]); // close reading
    close(process.pipe_fd[1]); // close writing
}

// read message from pipe
void ProcessTable::read_pipe_message(Process &process)
{
    char buffer[256];
    int bytesRead = read(process.pipe_fd[0], buffer, sizeof(buffer));

    if (bytesRead > 0)
    {
        buffer[bytesRead] = '\0';
        cout << "Message from PID " << process.pid << ": " << buffer << endl;
    }
    else
    {
        cout << "No message read from pipe for PID " << process.pid << endl;
    }
}