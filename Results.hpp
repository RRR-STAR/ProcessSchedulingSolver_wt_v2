
#ifndef __Results__
#define __Results__

#include "Classes.hpp"
#include <algorithm>


// it calculates the FIST-COME-FIRST-SERVE result
inline void ProcessSchedulingCalculator::displayFCFSResults(){
    
    ganttSegments_.clear(); // clears the previous gantt chart result
    
    double cpuCycle = 0; // cpu clock cycle 
    std::vector<Process*> availableProcesses(processes_.size(), nullptr);
    
    // make copy of the processes into pointers for operations
    // so that we can do operations without changing the arrangements of processes
    for (short int i = 0, n = processes_.size(); i < n; ++i){ availableProcesses[i] = &(processes_[i]); }
    
    // sort processes by arrival time
    std::sort(availableProcesses.begin(), availableProcesses.end(), 
                [](const Process *p1, const Process *p2){ 
                    return  p1->arrivalTime < p2->arrivalTime; 
                });
    
    for (auto &process : availableProcesses){
        
        // wait for process to arrive if no proceeses are in the queue
        if (cpuCycle < process->arrivalTime){ cpuCycle = process->arrivalTime; }
        
        // calculate times
        auto process_startTime = cpuCycle;
        cpuCycle += process->burstTime;
        process->completionTime = cpuCycle;
        process->turnaroundTime = process->completionTime - process->arrivalTime;
        process->waitingTime    = process->turnaroundTime - process->burstTime;
        process->responseTime   = process_startTime       - process->arrivalTime;
        
        // add this process to the Gantt chart
        ganttSegments_.emplace_back(process->id, process_startTime, 
                                    process->completionTime, getProcessColor(process->id));
    }
    availableProcesses.clear(); // after calculation remove the pointer references
    
    updateGanttChart(ganttSegments_);
    updateStatistics(processes_);
}


// it calculates the SHORTEST-JOB-FIRST result
inline void ProcessSchedulingCalculator::displaySJFResults(){
    
    ganttSegments_.clear(); // clears the previous gantt chart result
    
    double cpuCycle = 0; // cpu clock cycle
    short int arrivedProcessIndex = 0, totalProcesses = processes_.size(); 
    std::vector<Process*> availableProcesses(totalProcesses, nullptr), readyQueue;
    readyQueue.reserve(totalProcesses);
    
    for (short int i = 0; i < totalProcesses; ++i){ availableProcesses[i] = &(processes_[i]); }
    
    // sort processes by arrival time
    std::sort(availableProcesses.begin(), availableProcesses.end(), 
                [](const Process *p1, const Process *p2){ 
                    return  p1->arrivalTime < p2->arrivalTime; 
                });
    
    // when all processes are arrived and ready queue become empty,
    // it means all the processes are executed successfully
    while (arrivedProcessIndex < totalProcesses  ||  not readyQueue.empty()){
        
        // add the newly arrived processes in a ready queue
        // as the processes are already sorted thus no need to iterate from first to last
        while (arrivedProcessIndex < totalProcesses  and  
                availableProcesses[arrivedProcessIndex]->arrivalTime <= cpuCycle){
            readyQueue.push_back(availableProcesses[arrivedProcessIndex]);
            ++arrivedProcessIndex;
        }
        // if no new process arrived in this cpu clock cycle (means gap exists between this process and cpu cycle)
        // thus assign this process arrival time to the current cpu cycle
        if (readyQueue.empty()){
            if (arrivedProcessIndex < totalProcesses){
                cpuCycle = availableProcesses[arrivedProcessIndex]->arrivalTime;
            }
            continue;
        }
        // finding the process with smallest burst time from arrived processes
        auto shortestProcessIterator = std::min_element(readyQueue.begin(), readyQueue.end(),
                                                        [](const Process *p1, const Process *p2){ 
                                                            return  p1->burstTime < p2->burstTime; 
                                                        });
        Process *shortestProcess = *shortestProcessIterator;
        
        // execute the smallest burst time arrived process
        auto shortestProcess_startTime = cpuCycle;
        cpuCycle += shortestProcess->burstTime;
        shortestProcess->completionTime = cpuCycle;
        shortestProcess->turnaroundTime = shortestProcess->completionTime - shortestProcess->arrivalTime;
        shortestProcess->waitingTime    = shortestProcess->turnaroundTime - shortestProcess->burstTime;
        shortestProcess->responseTime   = shortestProcess_startTime       - shortestProcess->arrivalTime;
        
        // add this process to the Gantt chart
        ganttSegments_.emplace_back(shortestProcess->id, shortestProcess_startTime, 
                                    shortestProcess->completionTime, getProcessColor(shortestProcess->id));
        
        // remove the executed processes reference from the ready queue
        readyQueue.erase(shortestProcessIterator);
    }
    availableProcesses.clear(); // after calculation remove the references
    
    updateGanttChart(ganttSegments_);
    updateStatistics(processes_);
}


// it calculates the PRIORITY (non-premetive-version) result
inline void ProcessSchedulingCalculator::displayPriorityResults(){
    
    ganttSegments_.clear(); // clears the previous gantt chart result
    
    double cpuCycle = 0; // cpu clock cycle
    short int arrivedProcessIndex = 0, totalProcesses = processes_.size(); 
    std::vector<Process*> availableProcesses(totalProcesses, nullptr), readyQueue;
    readyQueue.reserve(totalProcesses);
    
    for (short int i = 0; i < totalProcesses; ++i){ availableProcesses[i] = &(processes_[i]); }
    
    // sort processes by arrival time
    std::sort(availableProcesses.begin(), availableProcesses.end(), 
                [](const Process *p1, const Process *p2){ 
                    return  p1->arrivalTime < p2->arrivalTime; 
                });
    
    // when all processes are arrived and ready queue become empty,
    // it means all the processes are executed successfully
    while (arrivedProcessIndex < totalProcesses  ||  not readyQueue.empty()){
        
        // add the newly arrived processes in a ready queue
        // as the processes are already sorted thus no need to iterate from first to last
        while (arrivedProcessIndex < totalProcesses  and  
                availableProcesses[arrivedProcessIndex]->arrivalTime <= cpuCycle){
            readyQueue.push_back(availableProcesses[arrivedProcessIndex]);
            ++arrivedProcessIndex;
        }
        // if no new process arrived in this cpu clock cycle (means gap exists between this process and cpu cycle)
        // thus assign this process arrival time to the current cpu cycle
        if (readyQueue.empty()){
            if (arrivedProcessIndex < totalProcesses){
                cpuCycle = availableProcesses[arrivedProcessIndex]->arrivalTime;
            }
            continue;
        }
        // finding the process with smallest burst time from arrived processes
        auto highestPriorityProcessIterator = std::max_element(readyQueue.begin(), readyQueue.end(),
                                                                [](const Process *p1, const Process *p2){ 
                                                                    return  p1->priority < p2->priority; 
                                                                });
        Process *highestPriorityProcess = *highestPriorityProcessIterator;
        
        // execute the smallest burst time arrived process
        auto highestPriorityProcess_startTime = cpuCycle;
        cpuCycle += highestPriorityProcess->burstTime;
        highestPriorityProcess->completionTime = cpuCycle;
        highestPriorityProcess->turnaroundTime = highestPriorityProcess->completionTime - highestPriorityProcess->arrivalTime;
        highestPriorityProcess->waitingTime    = highestPriorityProcess->turnaroundTime - highestPriorityProcess->burstTime;
        highestPriorityProcess->responseTime   = highestPriorityProcess_startTime       - highestPriorityProcess->arrivalTime;
        
        // add this process to the Gantt chart
        ganttSegments_.emplace_back(highestPriorityProcess->id, highestPriorityProcess_startTime, 
                                    highestPriorityProcess->completionTime, getProcessColor(highestPriorityProcess->id));
        
        // remove the executed processes reference from the ready queue
        readyQueue.erase(highestPriorityProcessIterator);
    }
    availableProcesses.clear(); // after calculation remove the references
    
    updateGanttChart(ganttSegments_);
    updateStatistics(processes_);
}


// it calculates the ROUND-ROBIN result
inline void ProcessSchedulingCalculator::displayRoundRobinResults(){
    
    ganttSegments_.clear(); // clears the previous gantt chart result
    
    double cpuCycle = 0; // cpu clock cycle 
    short int arrivedProcessIndex = 0, totalProcesses = processes_.size(); 
    std::vector<Process*> availableProcesses(totalProcesses, nullptr);
    std::list<Process*> readyQueue;
    
    for (short int i = 0; i < totalProcesses; ++i){ availableProcesses[i] = &(processes_[i]); }
    
    std::sort(availableProcesses.begin(), availableProcesses.end(), 
                [](const Process *p1, const Process *p2){ 
                    return  p1->arrivalTime < p2->arrivalTime; 
                });
    
    while (arrivedProcessIndex < totalProcesses  ||  not readyQueue.empty()){
        
        while (arrivedProcessIndex < totalProcesses  and  
                availableProcesses[arrivedProcessIndex]->arrivalTime <= cpuCycle){
            readyQueue.push_back(availableProcesses[arrivedProcessIndex]);
            ++arrivedProcessIndex;
        }
        if (readyQueue.empty()){
            if (arrivedProcessIndex < totalProcesses){
                cpuCycle = availableProcesses[arrivedProcessIndex]->arrivalTime;
            }
            continue;
        }
        // choose the first process and remove from the ready queue for execution
        Process *currentProcess = readyQueue.front();
        readyQueue.pop_front();
        
        if (not currentProcess->started){ // set response time if first time running
            currentProcess->responseTime = cpuCycle - currentProcess->arrivalTime;
            currentProcess->started = true;
        }
        
        auto currentProcess_startTime = cpuCycle; // mark the start time
        
        // if process remaining time is less that the time quantum,
        if (currentProcess->remainingTime < timeQuantum_)  cpuCycle += currentProcess->remainingTime;
        else  cpuCycle += timeQuantum_;
        
        // add this process to the Gantt chart
        ganttSegments_.emplace_back(currentProcess->id, currentProcess_startTime, 
                                    cpuCycle, getProcessColor(currentProcess->id));
        
        // add newly arrived processes during execution
        while (arrivedProcessIndex < totalProcesses  and  
                availableProcesses[arrivedProcessIndex]->arrivalTime <= cpuCycle){
            readyQueue.push_back(availableProcesses[arrivedProcessIndex]);
            ++arrivedProcessIndex;
        }
        
        // decrese the remainig time of the current process
        currentProcess->remainingTime -= timeQuantum_; 
        // if the process is not over, put again in the queue, otherwise calculate it
        if (currentProcess->remainingTime > 0){ readyQueue.push_back(currentProcess); }
        else { // process completed
            currentProcess->completionTime = cpuCycle;
            currentProcess->turnaroundTime = currentProcess->completionTime - currentProcess->arrivalTime;
            currentProcess->waitingTime    = currentProcess->turnaroundTime - currentProcess->burstTime;
            // response time is already set
        }
    }
    availableProcesses.clear(); // after calculation remove the references
    
    updateGanttChart(ganttSegments_);
    updateStatistics(processes_);
}


// it calculates the SHORTEST-REMAINING-TIME-FIRST result
inline void ProcessSchedulingCalculator::displaySRTFResults(){
    
    ganttSegments_.clear(); // clears the previous gantt chart result
    
    double executionTime = 0, cpuCycle = 0; // cpu clock cycle 
    short int arrivedProcessIndex = 0, totalProcesses = processes_.size(); 
    std::vector<Process*> availableProcesses(totalProcesses, nullptr), readyQueue;
    
    for (short int i = 0; i < totalProcesses; ++i){ availableProcesses[i] = &(processes_[i]); }
    
    std::sort(availableProcesses.begin(), availableProcesses.end(), 
                [](const Process *p1, const Process *p2){ 
                    return  p1->arrivalTime < p2->arrivalTime; 
                });
    
    while (arrivedProcessIndex < totalProcesses  ||  not readyQueue.empty()){
        
        while (arrivedProcessIndex < totalProcesses  and  
                availableProcesses[arrivedProcessIndex]->arrivalTime <= cpuCycle){
            readyQueue.push_back(availableProcesses[arrivedProcessIndex]);
            ++arrivedProcessIndex;
        }
        if (readyQueue.empty()){
            if (arrivedProcessIndex < totalProcesses){
                cpuCycle = availableProcesses[arrivedProcessIndex]->arrivalTime;
            }
            continue;
        }
        // finding the process with shortest remaining time from arrived processes
        auto shortestProcessIterator = std::min_element(readyQueue.begin(), readyQueue.end(),
                                                        [](const Process *p1, const Process *p2){ 
                                                            return  p1->remainingTime < p2->remainingTime; 
                                                        });
        Process *currentShortestProcess = *shortestProcessIterator;
        readyQueue.erase(shortestProcessIterator);
        
        if (not currentShortestProcess->started){ // set response time if first time running
            currentShortestProcess->responseTime = cpuCycle - currentShortestProcess->arrivalTime;
            currentShortestProcess->started = true;
        }
        // if next process remaining time is less than the current process remaining then,
        // execution time is set to the difference between next process arrival and current cpu clock
        // so that this(current process) executes upto next process arrival
        if (arrivedProcessIndex < totalProcesses  and  arrivedProcessIndex > 0){
            executionTime = availableProcesses[arrivedProcessIndex]->arrivalTime - cpuCycle;
            // if the next process arrival is more than this process remainig 
            if (executionTime > currentShortestProcess->remainingTime)
                executionTime = currentShortestProcess->remainingTime;
        }
        // when all pocesses arrived there is no chance of premption
        else  executionTime = currentShortestProcess->remainingTime;
        
        cpuCycle += executionTime;
        
        // add this process to the Gantt chart
        ganttSegments_.emplace_back(currentShortestProcess->id, cpuCycle - executionTime, 
                                    cpuCycle, getProcessColor(currentShortestProcess->id));
        
        // add newly arrived processes during execution
        while (arrivedProcessIndex < totalProcesses  and  
                availableProcesses[arrivedProcessIndex]->arrivalTime <= cpuCycle){
            readyQueue.push_back(availableProcesses[arrivedProcessIndex]);
            ++arrivedProcessIndex;
        }
        
        // decrese the remainig time of the current process
        currentShortestProcess->remainingTime -= executionTime; 
        // if the process is not over, put again in the queue, otherwise calculate it
        if (currentShortestProcess->remainingTime > 0){ readyQueue.push_back(currentShortestProcess); }
        else { // process completed
            currentShortestProcess->completionTime = cpuCycle;
            currentShortestProcess->turnaroundTime = currentShortestProcess->completionTime - currentShortestProcess->arrivalTime;
            currentShortestProcess->waitingTime    = currentShortestProcess->turnaroundTime - currentShortestProcess->burstTime;
            // response time is already set
        }
        
        /* ---------------- for debugging purpose -------------------
        std::cout <<std::endl<< currentShortestProcess->id <<" : "<< cpuCycle <<std::endl;
        for (Process *p : readyQueue)  std::cout <<" "<< p->id <<"|"<< p->remainingTime;
        */
    }
    availableProcesses.clear(); // after calculation remove the references
    
    updateGanttChart(ganttSegments_);
    updateStatistics(processes_);
}


// it calculates the PRIORITY (premetive-version) result
inline void ProcessSchedulingCalculator::displayPRPResults(){
    
    ganttSegments_.clear(); // clears the previous gantt chart result
    
    double executionTime = 0, cpuCycle = 0; // cpu clock cycle 
    short int arrivedProcessIndex = 0, totalProcesses = processes_.size(); 
    std::vector<Process*> availableProcesses(totalProcesses, nullptr), readyQueue;
    
    for (short int i = 0; i < totalProcesses; ++i){ availableProcesses[i] = &(processes_[i]); }
    
    std::sort(availableProcesses.begin(), availableProcesses.end(), 
                [](const Process *p1, const Process *p2){ 
                    return  p1->arrivalTime < p2->arrivalTime; 
                });
    
    while (arrivedProcessIndex < totalProcesses  ||  not readyQueue.empty()){
        
        while (arrivedProcessIndex < totalProcesses  and  
                availableProcesses[arrivedProcessIndex]->arrivalTime <= cpuCycle){
            readyQueue.push_back(availableProcesses[arrivedProcessIndex]);
            ++arrivedProcessIndex;
        }
        if (readyQueue.empty()){
            if (arrivedProcessIndex < totalProcesses){
                cpuCycle = availableProcesses[arrivedProcessIndex]->arrivalTime;
            }
            continue;
        }
        // finding the highest priority process from arrived processes
        auto highestPriorityProcessIterator = std::max_element(readyQueue.begin(), readyQueue.end(),
                                                                [](const Process *p1, const Process *p2){ 
                                                                    return  p1->priority < p2->priority; 
                                                                });
        Process *highestPriorityProcess = *highestPriorityProcessIterator;
        readyQueue.erase(highestPriorityProcessIterator);
        
        if (not highestPriorityProcess->started){ // set response time if first time running
            highestPriorityProcess->responseTime = cpuCycle - highestPriorityProcess->arrivalTime;
            highestPriorityProcess->started = true;
        }
        // if next process remaining time is less than the current process remaining then,
        // execution time is set to the difference between next process arrival and current cpu clock
        // so that this(current process) executes upto next process arrival
        if (arrivedProcessIndex < totalProcesses  and  arrivedProcessIndex > 0){
            executionTime = availableProcesses[arrivedProcessIndex]->arrivalTime - cpuCycle;
            // if the next process arrival is more than this process remainig 
            if (executionTime > highestPriorityProcess->remainingTime)
                executionTime = highestPriorityProcess->remainingTime;
        }
        // when all pocesses arrived there is no chance of premption
        else  executionTime = highestPriorityProcess->remainingTime;
        
        cpuCycle += executionTime;
        
        // add this process to the Gantt chart
        ganttSegments_.emplace_back(highestPriorityProcess->id, cpuCycle - executionTime, 
                                    cpuCycle, getProcessColor(highestPriorityProcess->id));
        
        while (arrivedProcessIndex < totalProcesses  and  
                availableProcesses[arrivedProcessIndex]->arrivalTime <= cpuCycle){
            readyQueue.push_back(availableProcesses[arrivedProcessIndex]);
            ++arrivedProcessIndex;
        }
        
        highestPriorityProcess->remainingTime -= executionTime; 
        if (highestPriorityProcess->remainingTime > 0){ readyQueue.push_back(highestPriorityProcess); }
        else { // process completed
            highestPriorityProcess->completionTime = cpuCycle;
            highestPriorityProcess->turnaroundTime = highestPriorityProcess->completionTime - highestPriorityProcess->arrivalTime;
            highestPriorityProcess->waitingTime    = highestPriorityProcess->turnaroundTime - highestPriorityProcess->burstTime;
        }
        
        /* ---------------- for debugging purpose -------------------
        std::cout <<std::endl<< highestPriorityProcess->id <<" : "<< cpuCycle <<std::endl;
        for (Process *p : readyQueue)  std::cout <<" "<< p->id <<"|"<< p->remainingTime;
        */
    }
    availableProcesses.clear();
    
    updateGanttChart(ganttSegments_);
    updateStatistics(processes_);
}


// it calculates the I/O SCHEDULING result
inline void ProcessSchedulingCalculator::displayIOResults(){
    
    ganttSegments_.clear(); // clears the previous gantt chart result
    
    short int arrivedProcesses = 0, totalProcesses = processes_.size(); 
    std::vector<double> processIoStartTime(totalProcesses, 0.0);
    double executionTime = 0.5, cpuCycle = 0.0; // cpu clock cycle 
    std::vector<bool> processArrived(totalProcesses, false), processIoComplete(totalProcesses, false);
    std::vector<Process*> availableProcesses(totalProcesses, nullptr);
    std::list<Process*> readyQueue, ioQueue;
    
    for (short int i = 0; i < totalProcesses; ++i){ availableProcesses[i] = &(processes_[i]); }
    
    auto processArrivalCheck = [&](){
        if (arrivedProcesses < totalProcesses){ // put arrived processes in queue
            for (Process *process : availableProcesses){
                if (not processArrived[process->id - 1]  and  process->arrivalTime <= cpuCycle){
                    readyQueue.push_back(process);
                    processArrived[process->id - 1] = true;
                    ++arrivedProcesses;
                }
            }
        }
    };
    auto processIoCheck = [&](){
        for (auto it = ioQueue.begin();  it != ioQueue.end();  ){
            Process *ioProcess = *it;
            if (cpuCycle  >=  processIoStartTime[ioProcess->id - 1] + ioProcess->ioTime){
                readyQueue.push_back(ioProcess);
                processIoComplete[ioProcess->id - 1] = true;
                it = ioQueue.erase(it); // point to the next valid iterator
            }
            else  ++it; // iterate to the next element
        }
    };
    
    while (arrivedProcesses < totalProcesses  ||  not readyQueue.empty()  ||  not ioQueue.empty()){
        
        processArrivalCheck();
        if (readyQueue.empty()){ // when no process are in queue 
            cpuCycle += executionTime;  processIoCheck();  continue; } 
        
        // finding the highest priority process from arrived processes
        auto highestPriorityProcessIterator = std::max_element(readyQueue.begin(), readyQueue.end(),
                                                                [](const Process *p1, const Process *p2){ 
                                                                    return  p1->priority < p2->priority; 
                                                                });
        Process *highestPriorityProcess = *highestPriorityProcessIterator;
        
        if (not highestPriorityProcess->started){ // set response time if first time running
            highestPriorityProcess->responseTime = cpuCycle - highestPriorityProcess->arrivalTime;
            highestPriorityProcess->started = true;
        }
        
        cpuCycle += executionTime; // increase the cpu cycle
        processArrivalCheck(); // to check newly arrived processes during execution
        processIoCheck(); // to check returned processes means, io complete processes during execution
        
        // add this process to the Gantt chart
        ganttSegments_.emplace_back(highestPriorityProcess->id, cpuCycle - executionTime, 
                                    cpuCycle, getProcessColor(highestPriorityProcess->id));
        
        // execute the process
        if (not processIoComplete[highestPriorityProcess->id - 1]) // when first burst time finish
            highestPriorityProcess->remainingTime -= executionTime; 
        else highestPriorityProcess->remainingTime2 -= executionTime;
        
        if (not processIoComplete[highestPriorityProcess->id - 1]  and  highestPriorityProcess->remainingTime <= 0.0){ // process completed it's first part
            if (highestPriorityProcess->ioTime > 0){ // if the process have io time to perform
                processIoStartTime[highestPriorityProcess->id - 1] = cpuCycle;
                readyQueue.erase(highestPriorityProcessIterator);
                ioQueue.push_back(highestPriorityProcess);
            }
            else processIoComplete[highestPriorityProcess->id - 1] = true; // if no io time, then consider io complete
        }
        if (processIoComplete[highestPriorityProcess->id - 1]  and  highestPriorityProcess->remainingTime2 <= 0.0){ // process completed
            highestPriorityProcess->completionTime = cpuCycle;
            highestPriorityProcess->turnaroundTime = highestPriorityProcess->completionTime - highestPriorityProcess->arrivalTime;
            highestPriorityProcess->waitingTime    = highestPriorityProcess->turnaroundTime - 
                                                    (highestPriorityProcess->burstTime + highestPriorityProcess->burstTime2);
            readyQueue.erase(highestPriorityProcessIterator);
        }
        
        /* ---------------- for debugging purpose (debug with cpuCycle increment +1) -------------------
        std::cout <<std::endl<< highestPriorityProcess->id <<" : "<< cpuCycle <<" : "<< highestPriorityProcess->remainingTime <<std::endl;
        for (Process *p : readyQueue)  std::cout <<" "<< p->id <<"|"<< p->remainingTime <<"|"<< p->remainingTime2;
        std::cout <<std::endl<<" ioQueue  ";
        for (Process *p : ioQueue)  std::cout <<" "<< p->id <<"|"<< p->remainingTime <<"|"<< p->remainingTime2;
        */
    }
    availableProcesses.clear();
    
    updateGanttChart(ganttSegments_);
    updateStatistics(processes_);
}


#endif // __Results__