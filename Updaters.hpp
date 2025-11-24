
#ifndef __Updaters__
#define __Updaters__

#include "Classes.hpp"
#include <iomanip>
#include <sstream>


// update the process table UI 
// add/remove process rows based on the input(new value) and current value
inline void ProcessSchedulingCalculator::updateProcessTable(const short int &newProcessCount){
    
    // if current count is < the new count, then just add new process rows
    if (currentProcessCount_ < newProcessCount){
        for (short int i = currentProcessCount_; i < newProcessCount; ++i){
            
            auto ith_row = processTable_->insertRow(i + 1); // add rows for each process
            
            // Process ID
            auto idText = ith_row->elementAt(0)->addNew<WText>("P" + std::to_string(i + 1));
            ith_row->elementAt(0)->setStyleClass("text-center");
            idText->setStyleClass("fw-semibold"); 
            
            // Arrival time
            auto arrivalEdit = ith_row->elementAt(1)->addNew<WDoubleSpinBox>();
            arrivalEdit->setStyleClass("form-control form-control-sm");
            arrivalEdit->setRange(0, 200);
            arrivalEdit->setValue(processes_[i].arrivalTime);
            arrivalEdit->setDecimals(1);
            
            // Burst time
            auto burstEdit = ith_row->elementAt(2)->addNew<WDoubleSpinBox>();
            burstEdit->setStyleClass("form-control form-control-sm");
            burstEdit->setRange(1, 200);
            burstEdit->setValue(processes_[i].burstTime);
            burstEdit->setDecimals(1);
            
            // Input-Output time
            auto ioEdit = ith_row->elementAt(3)->addNew<WDoubleSpinBox>();
            ioEdit->setStyleClass("form-control form-control-sm");
            ioEdit->setRange(0, 100);
            ioEdit->setValue(processes_[i].ioTime);
            ioEdit->setDecimals(1);
            
            // Burst time 2
            auto burstEdit2 = ith_row->elementAt(4)->addNew<WDoubleSpinBox>();
            burstEdit2->setStyleClass("form-control form-control-sm");
            burstEdit2->setRange(0, 200);
            burstEdit2->setValue(processes_[i].burstTime2);
            burstEdit2->setDecimals(1);
            
            // Priority
            auto priorityEdit = ith_row->elementAt(5)->addNew<WSpinBox>();
            priorityEdit->setStyleClass("form-control form-control-sm");
            priorityEdit->setRange(1, 100);
            priorityEdit->setValue(processes_[i].priority);
            
            ith_row->elementAt(3)->setId("inp-out-header" + std::to_string(i));
            ith_row->elementAt(4)->setId("execute2-header" + std::to_string(i));
            ith_row->elementAt(5)->setId("priority-row" + std::to_string(i));
            
            // hide/show the execute2, io, priority column based on the selected algorithm
            if (selectedAlgorithm_ == "IO"){ 
                ith_row->elementAt(3)->show();  
                ith_row->elementAt(4)->show(); 
                ith_row->elementAt(5)->show();
            }
            else if (selectedAlgorithm_ == "PR"  or  selectedAlgorithm_ == "PRP"){
                ith_row->elementAt(3)->hide(); 
                ith_row->elementAt(4)->hide();
                ith_row->elementAt(5)->show();  
            }
            else { 
                ith_row->elementAt(3)->hide();  
                ith_row->elementAt(4)->hide(); 
                ith_row->elementAt(5)->hide();
            }
            
        }
    }
    // else current count is > the new count, then just remove the existing process rows from end
    else { // currentProcessCount_ > newProcessCount 
        while (currentProcessCount_ != newProcessCount){ 
            // as newProcessCount can't be 0, so no worries for header row removal
            
            processTable_->removeRow(currentProcessCount_);
            currentProcessCount_ -= 1;
        }
    }
    // at last the new process count must be assign to the current count to maintain the other logics
    currentProcessCount_ = newProcessCount; // though it not necessary for else part, but you know............
}


// show the given message as an alert message
// message type is based on the 2nd-param of the function 
inline void ProcessSchedulingCalculator::showAlertMessage(const std::string &message, const bool &messageTypeIsSuccess){
    
    alertContainer_->clear(); // clear any existing msz
    auto space = alertContainer_->addNew<WBreak>();
    auto alert = alertContainer_->addNew<WContainerWidget>();
    
    (messageTypeIsSuccess) ? 
        alert->setStyleClass("alert alert-success alert-dismissible fade show") :
        alert->setStyleClass("alert alert-danger alert-dismissible fade show");
    
    alert->addNew<WText>(message);
    auto closeButton = alert->addNew<WPushButton>();
    
    closeButton->setStyleClass("btn-close");
    closeButton->clicked().connect([=](){ // clears the alert message upon clicking on close button
        alertContainer_->removeWidget(alert);  alertContainer_->removeWidget(space);
    }); // and pass-by-value needed, bcz local objects destroyed after control exits from the function
}


// validate the input values for process input table & time quantum input
inline bool ProcessSchedulingCalculator::validateInputs(){
    
    if (selectedAlgorithm_ == "RR"  and  quantumSpinBox_->value() <= 0){
        showAlertMessage("Time-Quantum value must be greater than 0.0 !", false);
        return false;
    }
    for (short int i = 0; i < currentProcessCount_; ++i){
        auto ith_row = processTable_->rowAt(i + 1);
        
        auto arrivalEdit = static_cast<WDoubleSpinBox*>(ith_row->elementAt(1)->widget(0));
        if (arrivalEdit->value() < 0.0){
            showAlertMessage("Arrival time cannot be negative !", false);
            return false;
        }
        auto burstEdit = static_cast<WDoubleSpinBox*>(ith_row->elementAt(2)->widget(0));
        if (burstEdit->value() <= 0.0){
            showAlertMessage("Burst time must be greater than 0 !", false);
            return false;
        }
        auto ioEdit = static_cast<WDoubleSpinBox*>(ith_row->elementAt(3)->widget(0));
        if (selectedAlgorithm_ == "IO"  and  ioEdit->value() < 0.0){
            showAlertMessage("I/O time cannot be negative !", false);
            return false;
        }
        auto burstEdit2 = static_cast<WDoubleSpinBox*>(ith_row->elementAt(4)->widget(0));
        if (selectedAlgorithm_ == "IO"  and  burstEdit2->value() < 0.0){
            showAlertMessage("Second Burst time cannot be negative !", false);
            return false;
        }
        auto priorityEdit = static_cast<WSpinBox*>(ith_row->elementAt(5)->widget(0));
        if ((selectedAlgorithm_ == "PR"  or  selectedAlgorithm_ == "PRP"  or  selectedAlgorithm_ == "IO")  
            and  (priorityEdit->value() <= 0  or  priorityEdit->value() > 100)){
                showAlertMessage("Priority must be between 1 to 100 !", false);
                return false;
        }
    }
    return true;
}


// return the actual process color to differentiate between the processes in the grantt chart
inline std::string ProcessSchedulingCalculator::getProcessColor(const short int processId){
    return  processColors_[(processId - 1) % processColors_.size()];
}


// create & update the statistics values with the result
inline void ProcessSchedulingCalculator::updateStatistics(const std::vector<Process> &processes){
    
    // clear existing table
    while (statisticsTable_->rowCount() > 0){ statisticsTable_->removeRow(0); }
    
    // create the header row
    auto headerRow = statisticsTable_->insertRow(0);
    headerRow->elementAt(0)->addNew<WText>("PID");  // process id
    headerRow->elementAt(1)->addNew<WText>("AT");   // arrival time
    headerRow->elementAt(2)->addNew<WText>("CT");   // completion time
    headerRow->elementAt(3)->addNew<WText>("TT");   // turnaround time 
    headerRow->elementAt(4)->addNew<WText>("WT");   // waiting time
    headerRow->elementAt(5)->addNew<WText>("RT");   // response time
    
    // style header
    for (short int i = 0; i <= 5; ++i){ 
        headerRow->elementAt(i)->setStyleClass("text-center rounded-pill bg-secondary text-white"); 
    }
    // add process data
    double totalBurstTime = 0, totalWaitingTime = 0, totalTurnaroundTime = 0, totalResponseTime = 0;
    
    for (const auto &process : processes){
        // insert the row at the ith + 1 position every time it returns the row count
        auto ith_row = statisticsTable_->insertRow(statisticsTable_->rowCount());
        std::ostringstream  at, ct, tat, wt, rt;
        
        ith_row->elementAt(0)->addNew<WText>("P" + std::to_string(process.id));
        at << std::fixed << std::setprecision(1) << process.arrivalTime;
        ith_row->elementAt(1)->addNew<WText>(at.str());
        ct << std::fixed << std::setprecision(1) << process.completionTime;
        ith_row->elementAt(2)->addNew<WText>(ct.str());
        tat << std::fixed << std::setprecision(1) << process.turnaroundTime;
        ith_row->elementAt(3)->addNew<WText>(tat.str());
        wt << std::fixed << std::setprecision(1) << process.waitingTime;
        ith_row->elementAt(4)->addNew<WText>(wt.str());
        rt << std::fixed << std::setprecision(1) << process.responseTime;
        ith_row->elementAt(5)->addNew<WText>(rt.str());
        
        for (short int i = 0; i <= 5; ++i){ ith_row->elementAt(i)->setStyleClass("text-center"); }
        
        (selectedAlgorithm_ == "IO") ? 
        totalBurstTime      += process.burstTime + process.burstTime2 : 
        totalBurstTime      += process.burstTime;
        totalWaitingTime    += process.waitingTime;
        totalTurnaroundTime += process.turnaroundTime;
        totalResponseTime   += process.responseTime;
    }
    
    // calculate averages
    double avgBurstTime      = totalBurstTime / processes.size();
    double avgWaitingTime    = totalWaitingTime / processes.size();
    double avgTurnaroundTime = totalTurnaroundTime / processes.size();
    double avgResponseTime   = totalResponseTime / processes.size();
    
    // update summary statistics
    averageBurstTimeText_->setText("<h6> Throughput = "                   + 
                                        std::to_string(avgBurstTime)      + " processes/unit </h6>");
    averageWaitingTimeText_->setText("<h6> Average Waiting Time = "       + 
                                        std::to_string(avgWaitingTime)    + " units </h6>");
    averageTurnaroundTimeText_->setText("<h6> Average Turnaround Time = " + 
                                        std::to_string(avgTurnaroundTime) + " units </h6>");
    averageResponseTimeText_->setText("<h6> Average Response Time = "     + 
                                        std::to_string(avgResponseTime)   + " units </h6>");
}


// html design for gantt chart creation
inline std::string ProcessSchedulingCalculator::generateGanttHTML(const std::vector<GanttSegment> &segments){
    if (segments.empty())  return "";
    
    std::ostringstream html;
    html <<"<div style='overflow-x: auto; margin: 30px 0; font-family: Arial, sans-serif;'>";
    // Start table
    html <<"<table style='border-collapse: separate; border-spacing: 0; width: auto;'>";
    
    // --- First row: processes ---
    html << "<tr>";
    for (const auto &segment : segments){
        int width = (segment.endTime - segment.startTime) * 40; // adjust as space 
        html <<"\
        <td style='width: "<< width <<"px; height: 40px; background:"<< segment.color <<"; \
        border: none; border-radius: 25px; text-align: center; vertical-align: middle; font-size: 15px; font-weight: bold;'> \
            P"<< segment.processId <<" \
        </td>";
    }
    html << "</tr>";
    
    // --- Second row: edge times ---
    html << "<tr>";
    for (const auto &segment : segments){
        html <<"\
        <td style='height: 20px; border: none; background: transparent; position: relative;'> \
            <div class='d-flex justify-content-between text-dark fs-6'> \
                <span style='padding-left : 5px; '>"<< segment.startTime <<"</span> \
                <span> - </span> \
                <span style='padding-right: 5px; '>"<< segment.endTime <<"</span> \
            </div> \
        </td>";
        /* same creates the gantt chart but the timings are in middle point with this line 
        <div class='d-flex justify-content-center text-dark fs-6'> \
        */
    }
    html << "</tr>";
    html << "</table>";
    html << "</div>";
    
    return html.str();
}


// update the gantt chart with the given segments
inline void ProcessSchedulingCalculator::updateGanttChart(const std::vector<GanttSegment> &segments){
    if (segments.empty()){
        ganttChartText_->setText("<h6> No processes to display. </h6>");
        return;
    }
    
    std::string html = generateGanttHTML(segments);
    ganttChartText_->setText(html);
}


// clear the results (gantt chart and statistics table)
inline void ProcessSchedulingCalculator::clearResults(){
    
    ganttSegments_.clear();
    ganttChartText_->setText("");
    
    // clear statistics table
    while (statisticsTable_->rowCount() > 0){ statisticsTable_->removeRow(0); }
    
    // clear summary statistics
    averageBurstTimeText_->setText("");
    averageWaitingTimeText_->setText("");
    averageTurnaroundTimeText_->setText("");
    averageResponseTimeText_->setText("");
    
    alertContainer_->clear();  // clear alert container
    resultsContainer_->hide(); // hide the result container
}


#endif // __Updaters__