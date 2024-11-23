#ifndef PROCESSTABLE_H
#define PROCESSTABLE_H

#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

using namespace std;

class ProcessTable
{
private:
    struct Process
    {
        pid_t pid;
        int transaction_id;
        string status;
    };

    Process *processes;
    int size;

public:
    ProcessTable(int size);
    ~ProcessTable();

    bool addProcess(pid_t pid, int transaction_id);
    void printProcesses();
    void waitAndRemoveProcess(pid_t pid);
};

#endif