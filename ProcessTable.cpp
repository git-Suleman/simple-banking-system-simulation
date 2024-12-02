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

// add process to processTable + IPC via Pipes
bool ProcessTable::addProcess(pid_t pid, int transaction_id)
{
    for (int i = 0; i < size; ++i)
    {
        if (processes[i].pid == 0)
        {
            processes[i].pid = pid;
            processes[i].transaction_id = transaction_id;
            processes[i].status = "Waiting";
            setupPipe(processes[i]);
            return true;
        }
    }
    return false;
}

// currently active processes in processTable
void ProcessTable::printProcesses()
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
void ProcessTable::waitAndRemoveProcess(pid_t pid)
{
    for (int i = 0; i < size; ++i)
    {
        if (processes[i].pid == pid)
        {
            int status;
            waitpid(pid, &status, 0);
            cout << "Child process (PID: " << pid << ") terminated." << endl;
            cleanupPipe(processes[i]);
            processes[i].pid = 0;
            processes[i].status = "Finished";
            return;
        }
    }
}

// scheduling algorithms for fair execution
void ProcessTable::runRoundRobin()
{
    printGanttChart();
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
                printProcesses();
                sleep(quantum);

                // update status to waiting after time quantum
                processes[i].status = "Waiting";

                waitAndRemoveProcess(processes[i].pid);

                string message = "Process in waiting queue";
                write(processes[i].pipe_fd[1], message.c_str(), message.length() + 1);
                cout << "Status update sent to pipe for PID " << processes[i].pid << endl;
            }
        }
    }
    cout << "All processes have completed.\n";
}

// visualization of processes execution
void ProcessTable::printGanttChart()
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

// create Pipe for IPC
void ProcessTable::setupPipe(Process &process)
{
    if (pipe(process.pipe_fd) == -1)
    {
        cout << "Pipe creation failed" << endl;
        return;
    }
}

// destroy Pipe after process finishes
void ProcessTable::cleanupPipe(Process &process)
{
    close(process.pipe_fd[0]); // close reading
    close(process.pipe_fd[1]); // close writing
}