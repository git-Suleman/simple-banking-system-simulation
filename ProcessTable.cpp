#include "ProcessTable.h"

ProcessTable::ProcessTable(int size, int quantum)
    : size(size), quantum(quantum)
{
    processes = new Process[size]();
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
            processes[i].status = "Waiting";
            setupPipe(processes[i]);
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
            cout << "Process (PID: " << processes[i].pid
                 << ", Transaction ID: " << processes[i].transaction_id
                 << ", Status: " << processes[i].status << ")\n";
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
            cleanupPipe(processes[i]);
            processes[i].pid = 0;
            processes[i].status = "Finished";
            return;
        }
    }
}

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

                // Simulate running the process for the quantum
                processes[i].status = "Running";
                cout << "Executing process (PID: " << processes[i].pid
                     << ", Transaction ID: " << processes[i].transaction_id << ") for "
                     << quantum << " seconds.\n";
                printProcesses();
                sleep(quantum); // Simulate the time slice

                // Update the status to waiting (simulating that the process needs more time)
                processes[i].status = "Waiting";

                // Optionally, simulate the process completing randomly
                if (rand() % 2 == 0) // Simulate 50% chance of finishing
                {
                    waitAndRemoveProcess(processes[i].pid);
                }
                else
                {
                    // Simulate IPC by writing status to the pipe
                    const char *message = "Process still running";
                    write(processes[i].pipe_fd[1], message, strlen(message) + 1);
                    cout << "Status update sent to pipe for PID " << processes[i].pid << ".\n";
                }
            }
        }
    }

    cout << "All processes have completed.\n";
}

void ProcessTable::printGanttChart()
{
    cout << "\nGantt Chart:\n";
    cout << "-----------------------------------------\n";

    for (int i = 0; i < size; ++i)
    {
        if (processes[i].pid != 0)
        {
            cout << "| PID: " << processes[i].pid << " ";
        }
    }
    cout << "|\n";

    cout << "-----------------------------------------\n";
}

void ProcessTable::setupPipe(Process &process)
{
    if (pipe(process.pipe_fd) == -1)
    {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }
}

void ProcessTable::cleanupPipe(Process &process)
{
    close(process.pipe_fd[0]); // Close read end
    close(process.pipe_fd[1]); // Close write end
}