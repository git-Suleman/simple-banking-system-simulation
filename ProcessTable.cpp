#include "ProcessTable.h"

ProcessTable::ProcessTable(int size) : size(size)
{
    processes = new Process[size];
}

ProcessTable::~ProcessTable()
{
    delete[] processes;
}

bool ProcessTable::addProcess(pid_t pid, int transaction_id)
{
    for (int i = 0; i < size; ++i)
    {
        if (processes[i].pid == 0)
        {
            processes[i].pid = pid;
            processes[i].transaction_id = transaction_id;
            processes[i].status = "Running";
            return true;
        }
    }
    return false; // Process table is full
}

void ProcessTable::printProcesses()
{
    for (int i = 0; i < size; ++i)
    {
        if (processes[i].pid != 0)
        {
            cout << "Process (PID: " << processes[i].pid << ", Transaction ID: " << processes[i].transaction_id << ", Status: " << processes[i].status << ")\n";
        }
    }
}

void ProcessTable::waitAndRemoveProcess(pid_t pid)
{
    for (int i = 0; i < size; ++i)
    {
        if (processes[i].pid == pid)
        {
            int status;
            waitpid(pid, &status, 0);
            cout << "Child process (PID: " << pid << ") terminated.\n";
            processes[i].pid = 0;
            return;
        }
    }
}