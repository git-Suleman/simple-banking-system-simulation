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
    bool add_process(pid_t pid, int transaction_id);
    void print_processes();
    void wait_and_remove_process(pid_t pid);

    // scheduling algorithm
    void run_round_robin();
    void print_gantt_chart();

    // inter-process communication (IPC)
    void setup_pipe(Process &process);
    void cleanup_pipe(Process &process);
    void read_pipe_message(Process &process);
};

#endif