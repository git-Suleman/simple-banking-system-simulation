#ifndef PROCESSTABLE_H
#define PROCESSTABLE_H

#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

using namespace std;

struct Process
{
    pid_t pid;
    int transaction_id;
    string status;
    int pipe_fd[2];
};

class ProcessTable
{
private:
    Process *processes;
    int size;
    int quantum;

public:
    ProcessTable(int size, int quantum);
    ~ProcessTable();

    // process related methods
    bool addProcess(pid_t pid, int transaction_id);
    void printProcesses();
    void waitAndRemoveProcess(pid_t pid);

    // scheduling algorithm
    void runRoundRobin();
    void printGanttChart();

    // inter-process communication (IPC)
    void setupPipe(Process &process);
    void cleanupPipe(Process &process);
};

#endif