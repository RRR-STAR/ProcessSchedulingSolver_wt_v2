
#ifndef __EventHandlers__
#define __EventHandlers__

#include "Classes.hpp"

// clear the previous results and reloads the input fiels
// triggers whenever the algorithm changed (drop-down menu changed)
inline void ProcessSchedulingCalculator::onAlgorithmChanged(){
    
    short int algorithmIndex = algorithmComboBox_->currentIndex();
    auto timeQuantumInputBox = inputContainer_->findById("quantum-group");
    auto ioHeaderText        = inputContainer_->findById("inp-out-header");
    auto execute2HeaderText  = inputContainer_->findById("execute2-header");
    auto priorityHeaderText  = inputContainer_->findById("priority-header");
    
    switch (algorithmIndex){
        case 0: selectedAlgorithm_ = "FCFS";    break;
        case 1: selectedAlgorithm_ = "SJF";     break;
        case 2: selectedAlgorithm_ = "PR";      break;
        case 3: selectedAlgorithm_ = "RR";      break;
        case 4: selectedAlgorithm_ = "SRTF";    break;
        case 5: selectedAlgorithm_ = "PRP";     break;
        case 6: selectedAlgorithm_ = "IO";      break;
    }
    
    // show/hide quantum input for based on round-robin algotithm
    if (selectedAlgorithm_ == "RR"){ timeQuantumInputBox->show(); } 
    else { timeQuantumInputBox->hide(); }
    
    // show/hide execute2, io, priorty column based on algorithm change
    if (selectedAlgorithm_ == "IO"){ 
        ioHeaderText->show();  execute2HeaderText->show();  priorityHeaderText->show(); 
        for (short int i = 0; i < currentProcessCount_; ++i){
            processTable_->findById("inp-out-header" + std::to_string(i))->show();
            processTable_->findById("execute2-header" + std::to_string(i))->show();
            processTable_->findById("priority-row" + std::to_string(i))->show();
        }
    }
    else if (selectedAlgorithm_ == "PR"  or  selectedAlgorithm_ == "PRP"){ 
        ioHeaderText->hide();  execute2HeaderText->hide();  priorityHeaderText->show();
        for (short int i = 0; i < currentProcessCount_; ++i){
            processTable_->findById("inp-out-header" + std::to_string(i))->hide();
            processTable_->findById("execute2-header" + std::to_string(i))->hide();
            processTable_->findById("priority-row" + std::to_string(i))->show();
        }
    }
    else { 
        ioHeaderText->hide();  execute2HeaderText->hide();  priorityHeaderText->hide();
        for (short int i = 0; i < currentProcessCount_; ++i){
            processTable_->findById("inp-out-header" + std::to_string(i))->hide();
            processTable_->findById("execute2-header" + std::to_string(i))->hide();
            processTable_->findById("priority-row" + std::to_string(i))->hide();
        }
    }
    
    clearResults(); // clear results whenever the algorithm is changed
}


// resize the processes based on process count and initilize process values 
// triggers whenever the no. of processes are changed
inline void ProcessSchedulingCalculator::onProcessCountChanged(){
    
    short int newCount = processCountSpinBox_->value();
    
    // if the process count value is not in range then no changes happen to the table
    if (newCount < 1  or  newCount > 100)  return;
    else  processes_.resize(newCount); // resize processes vector
    
    // initialize newly added processes with default values
    if (currentProcessCount_ < newCount){
        for (short int i = currentProcessCount_; i < newCount; ++i){
            
            processes_[i].id = i + 1;
            // Default arrival times
            if (processes_[i].arrivalTime == 0){ processes_[i].arrivalTime = i; }
            // Default burst times
            if (processes_[i].burstTime == 0){ processes_[i].burstTime = (i + 1) * 2; }
            if (processes_[i].burstTime2 == 0){ processes_[i].burstTime2 = (i + 1) * 2; }
            // Default priorities
            if (processes_[i].priority == 0){ processes_[i].priority = 1; }
        }
    }
    
    updateProcessTable(newCount);
}


// adds a new process into the current process input table
// if reaches the maximum no.(100) then, shows error alert upon clicking 
inline void ProcessSchedulingCalculator::onAddProcessClicked(){
    
    if (currentProcessCount_ < 100){
        processCountSpinBox_->setValue(currentProcessCount_ + 1);
        onProcessCountChanged();
    }
    else { showAlertMessage("Maximum 100 processes are allowed !", false); }
}


// removes a process from the current process input table
// if reaches the minimum no.(1) then, shows error alert upon clicking
inline void ProcessSchedulingCalculator::onRemoveProcessClicked(){
    
    if (currentProcessCount_ > 1){
        processCountSpinBox_->setValue(currentProcessCount_ - 1);
        onProcessCountChanged();
    }
    else { showAlertMessage("At least 1 process is required !", false); }
}


// start the calculation process by reading the data and processed the data
inline void ProcessSchedulingCalculator::onCalculateClicked(){
    
    if (not validateInputs())  return; // if inputs are not validated then simply return
    timeQuantum_ = quantumSpinBox_->value();
    
    // read data from table
    for (short int i = 0; i < currentProcessCount_; ++i) {
        auto ith_row = processTable_->rowAt(i + 1);
        
        processes_[i].id = i + 1; // set the process id
        
        auto arrivalEdit = static_cast<WDoubleSpinBox*>(ith_row->elementAt(1)->widget(0));
        processes_[i].arrivalTime = arrivalEdit->value(); // read arrival time
        
        auto burstEdit = static_cast<WDoubleSpinBox*>(ith_row->elementAt(2)->widget(0));
        processes_[i].burstTime = burstEdit->value(); // read burst time
        
        auto ioEdit = static_cast<WDoubleSpinBox*>(ith_row->elementAt(3)->widget(0));
        processes_[i].ioTime = ioEdit->value(); // read io time
        
        auto burstEdit2 = static_cast<WDoubleSpinBox*>(ith_row->elementAt(4)->widget(0));
        processes_[i].burstTime2 = burstEdit2->value(); // read burst time
        
        auto priorityEdit = static_cast<WSpinBox*>(ith_row->elementAt(5)->widget(0));
        processes_[i].priority = priorityEdit->value(); // read priority
        
        // set other values
        processes_[i].remainingTime  = processes_[i].burstTime;
        processes_[i].remainingTime2 = processes_[i].burstTime2;
    }
    
    // calls the corosponding algorithms for specific caculations
    if (selectedAlgorithm_ == "FCFS")    displayFCFSResults();
    if (selectedAlgorithm_ == "SJF")     displaySJFResults();
    if (selectedAlgorithm_ == "PR")      displayPriorityResults(); 
    if (selectedAlgorithm_ == "RR")      displayRoundRobinResults();
    if (selectedAlgorithm_ == "SRTF")    displaySRTFResults();
    if (selectedAlgorithm_ == "PRP")     displayPRPResults();
    if (selectedAlgorithm_ == "IO")      displayIOResults();
    
    resultsContainer_->show();
    showAlertMessage("Calculations completed successfully !", true);
}


// clear the result and shows message
inline void ProcessSchedulingCalculator::onClearClicked(){
    
    clearResults();
    showAlertMessage("All fields are cleared !", true);
}



#endif // __EventHandlers__