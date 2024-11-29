#ifndef PROCESSTABLE_H
#define PROCESSTABLE_H

#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>    // For O_* constants
#include <sys/stat.h> // For mkfifo()

using namespace std;
struct Process
{
    pid_t pid;
    int transaction_id;
    string status;
    int pipe_fd[2]; // Pipe file descriptors (read/write)
};

class ProcessTable
{
private:
    Process *processes;
    int size;
    int quantum; // Time slice for Round Robin

public:
    ProcessTable(int size, int quantum);
    ~ProcessTable();

    bool addProcess(pid_t pid, int transaction_id);
    void printProcesses();
    void waitAndRemoveProcess(pid_t pid);
    void runRoundRobin();
    void printGanttChart();
    void setupPipe(Process &process);
    void cleanupPipe(Process &process);
};

#endif