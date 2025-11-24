
#ifndef __DesignUI__
#define __DesignUI__

#include "Classes.hpp"


// creates the main user-interface of the whole pg.
// divided into 4 section (header + input + result + footer)
inline void ProcessSchedulingCalculator::createInterface() {
    
    addMetaHeader("viewport", "width=device-width, initial-scale=1.0");

    // Set application title
    setTitle("Process Scheduling Solver");
    
    // Apply Bootstrap 5 theme
    auto theme = std::make_shared<Wt::WBootstrap5Theme>();
    setTheme(theme);
    
    // Initialize process colors
    processColors_ = {
        "#FF6B6B", "#4ECDC4", "#45B7D1", "#96CEB4", "#FFEAA7",
        "#DDA0DD", "#98D8C8", "#F7DC6F", "#BB8FCE", "#85C1E9",
        "#F8C471", "#82E0AA", "#F1948A", "#85929E", "#D7BDE2"
    };
    
    // Main container with Bootstrap classes
    mainContainer_ = root()->addNew<WContainerWidget>();
    mainContainer_->setStyleClass("container-fluid");
    
    setupHeader();
    setupInputSection();
    setupResultsSection();
    setupFooter();
    
    // initialize with default values for create process table on load
    onProcessCountChanged(); 
}


// header part, contains the heading and sub-heading of the webpage
inline void ProcessSchedulingCalculator::setupHeader(){
    
    headerContainer_ = mainContainer_->addNew<WContainerWidget>();
    headerContainer_->setStyleClass("row mb-4");
    
    auto headerSection = headerContainer_->addNew<WContainerWidget>();
    headerSection->setStyleClass("col-12 text-center");
    
    headerSection->addNew<WBreak>();
    headerSection->addNew<WBreak>();
    
    auto title = headerSection->addNew<WText>("Process Scheduling Solver");
    title->setStyleClass("h1 text-primary mb-3"); // header text & style
    
    headerSection->addNew<WBreak>();
    headerSection->addNew<WBreak>();
    
    auto subtitle = headerSection->addNew<WText>(
        "Calculate waiting time, turnaround time, and visualize Gantt charts for various process scheduling algorithms");
    subtitle->setStyleClass("lead text-muted"); // subtitle text & style
    
    headerSection->addNew<WBreak>();
    
    // creates a horizontal line and space after the subtitle
    auto navContainer = headerSection->addNew<WContainerWidget>();
    navContainer->setStyleClass("nav nav-tabs justify-content-center mt-4");
    
    auto algorithmInfo = headerSection->addNew<WText>("Supports: FCFS, SJF, SRTF, Round Robin, Priority and IO scheduling");
    algorithmInfo->setStyleClass("small text-info mt-3");   // sub-sub-heading text & style
    
    headerSection->addNew<WBreak>();
    headerSection->addNew<WBreak>();
}


// footer part, contains the end summury part to make the webpage looks good 
inline void ProcessSchedulingCalculator::setupFooter(){
    
    footerContainer_ = mainContainer_->addNew<WContainerWidget>();
    footerContainer_->setStyleClass("row mt-5");
    
    auto footerSection = footerContainer_->addNew<WContainerWidget>();
    footerSection->setStyleClass("col-12 text-center");
    
    auto footerText = footerSection->addNew<WText>("Process Scheduling Solver - Built with Wt Framework. ");
    footerText->setStyleClass("text-muted small");       // footer text & text style
    
    auto footerText2 = footerSection->addNew<WText>(     // next footer text & style.
        "This tool helps visualize and analyze various process scheduling algorithms used in operating systems. ");
    footerText2->setStyleClass("text-muted small mt-2"); // mentioned separately for responsiveness
    
    footerSection->addNew<WBreak>();
    footerSection->addNew<WBreak>();
}


// set the input section for user inputs
// combination of 3 section (algo selection + process table inputs + control buttons)
inline void ProcessSchedulingCalculator::setupInputSection(){
    
    inputContainer_ = mainContainer_->addNew<WContainerWidget>();
    inputContainer_->setStyleClass("row mb-4");
    
    setupAlgorithmSelection();
    setupProcessInputTable();
    setupControlButtons();
}


// design the ui for algo selection, no. of process input, time quantum input
inline void ProcessSchedulingCalculator::setupAlgorithmSelection(){
    
    auto algorithmSection = inputContainer_->addNew<WContainerWidget>();
    algorithmSection->setStyleClass("col-lg-6 mb-3");
    
    // creates a wrap-up type section
    auto algorithmCard = algorithmSection->addNew<WContainerWidget>();
    algorithmCard->setStyleClass("card"); // a type wrap-up section
    
    auto cardHeader = algorithmCard->addNew<WContainerWidget>();
    cardHeader->setStyleClass("card-header bg-primary text-white");
    
    auto cardcardHeaderText = cardHeader->addNew<WText>(" Algorithm Configuration ");
    cardcardHeaderText->setStyleClass("h5 mb-0"); // wrap-up section header text
    
    auto cardBody = algorithmCard->addNew<WContainerWidget>();
    cardBody->setStyleClass("card-body");
    
    auto algorithmGroup = cardBody->addNew<WContainerWidget>();
    algorithmGroup->setStyleClass("mb-4");
    
    auto algorithmLabel = algorithmGroup->addNew<WLabel>(" Scheduling Algorithm: ");
    algorithmLabel->setStyleClass("form-label"); // drop-down menu label
    
    // drop-down menu input box & it's items for algorithm selection
    algorithmComboBox_ = algorithmGroup->addNew<WComboBox>();
    algorithmComboBox_->setStyleClass("form-select");
    algorithmComboBox_->addItem("First Come First Serve (FCFS)");
    algorithmComboBox_->addItem("Shortest Job First (SJF)");
    algorithmComboBox_->addItem("Priority Non-premetive (PR)");
    algorithmComboBox_->addItem("Round Robin (RR)");
    algorithmComboBox_->addItem("Shortest Remaining Time First (SRTF)");
    algorithmComboBox_->addItem("Priority Premetive (PRP)");
    algorithmComboBox_->addItem("Input-Output Scheduling (IO)");
    algorithmComboBox_->setCurrentIndex(0);
    algorithmComboBox_->changed().connect(this, &ProcessSchedulingCalculator::onAlgorithmChanged);
    
    auto processCountGroup = cardBody->addNew<WContainerWidget>();
    processCountGroup->setStyleClass("mb-3");
    
    auto processCountLabel = processCountGroup->addNew<WLabel>(" Number of Processes: ");
    processCountLabel->setStyleClass("form-label"); // process count label
    
    // process count input box for specify the total no. of processes  
    processCountSpinBox_ = processCountGroup->addNew<WSpinBox>();
    processCountSpinBox_->setStyleClass("form-control");
    processCountSpinBox_->setRange(1, 100);
    processCountSpinBox_->setValue(5);
    processCountSpinBox_->valueChanged().connect(this, &ProcessSchedulingCalculator::onProcessCountChanged);
    
    auto quantumGroup = cardBody->addNew<WContainerWidget>();
    quantumGroup->setStyleClass("mb-3");
    quantumGroup->setId("quantum-group"); // time quantum container id
    
    auto quantumLabel = quantumGroup->addNew<WLabel>(" Time Quantum: ");
    quantumLabel->setStyleClass("form-label"); // time quantum label
    
    // time quantum input box for specify the time quantum for premetive algos
    quantumSpinBox_ = quantumGroup->addNew<WDoubleSpinBox>();
    quantumSpinBox_->setStyleClass("form-control");
    quantumSpinBox_->setRange(0.1, 99.9);
    quantumSpinBox_->setValue(2.0);
    quantumSpinBox_->setDecimals(1);
    quantumSpinBox_->setSingleStep(0.5);
    
    quantumGroup->hide(); // Initially hide quantum input (only needs for premetive types)
}


// design table-ui to create a input table for process input 
inline void ProcessSchedulingCalculator::setupProcessInputTable(){
    
    auto tableSection = inputContainer_->addNew<WContainerWidget>();
    tableSection->setStyleClass("col-lg-6 mb-3");
    
    auto tableCard = tableSection->addNew<WContainerWidget>();
    tableCard->setStyleClass("card");
    
    auto cardHeader = tableCard->addNew<WContainerWidget>();
    cardHeader->setStyleClass("card-header d-flex justify-content-between align-items-center bg-primary text-white");
    
    auto cardHeaderText = cardHeader->addNew<WText>(" Process Information ");
    cardHeaderText->setStyleClass("h5 mb-0"); // wrap-up section header text
    
    auto buttonGroup = cardHeader->addNew<WContainerWidget>();
    buttonGroup->setStyleClass("btn-group btn-group-sm"); // button group
    
    addProcessButton_ = buttonGroup->addNew<WPushButton>("Add"); // button group : 
    addProcessButton_->setStyleClass("btn btn-success text-white"); // button 1 (add button)
    addProcessButton_->clicked().connect(this, &ProcessSchedulingCalculator::onAddProcessClicked);
    
    removeProcessButton_ = buttonGroup->addNew<WPushButton>("Remove"); // button group : 
    removeProcessButton_->setStyleClass("btn btn-danger text-white");     // button 2 (remove button)
    removeProcessButton_->clicked().connect(this, &ProcessSchedulingCalculator::onRemoveProcessClicked);
    
    auto cardBody = tableCard->addNew<WContainerWidget>();
    cardBody->setStyleClass("card-body");
    
    auto tableContainer = cardBody->addNew<WContainerWidget>();
    tableContainer->setStyleClass("table-responsive");
    
    processTable_ = tableContainer->addNew<WTable>(); // process input table
    processTable_->setStyleClass("table align-middle table-striped table-hover");
    
    // Create table headers (first row as the header row)
    auto headerRow = processTable_->rowAt(0);
    headerRow->elementAt(0)->addNew<WText>("Process ID");
    headerRow->elementAt(1)->addNew<WText>("Arrival Time");
    headerRow->elementAt(2)->addNew<WText>("Execute Time");
    headerRow->elementAt(3)->addNew<WText>("Inp_Out Time");
    headerRow->elementAt(4)->addNew<WText>("Execute Time");
    headerRow->elementAt(5)->addNew<WText>("Priority");
    
    headerRow->elementAt(3)->setId("inp-out-header");
    headerRow->elementAt(4)->setId("execute2-header");
    headerRow->elementAt(5)->setId("priority-header");
    headerRow->elementAt(3)->hide(); // initially hide because default is FCFS
    headerRow->elementAt(4)->hide(); // initially hide because default is FCFS
    headerRow->elementAt(5)->hide(); // initially hide because default is FCFS
    
    for (short int i = 0; i <= 5; ++i) { // Style header
        headerRow->elementAt(i)->setStyleClass("text-center rounded-pill bg-secondary text-white");
        // headerRow->elementAt(i)->setStyleClass("text-center");
    }
}


// design the-ui calculate & clear button just below the process input table 
inline void ProcessSchedulingCalculator::setupControlButtons(){
    
    auto buttonSection = inputContainer_->addNew<WContainerWidget>();
    buttonSection->setStyleClass("col-12 text-center mb-3");
    
    buttonSection->addNew<WBreak>();
    
    auto buttonGroup = buttonSection->addNew<WContainerWidget>();
    buttonGroup->setStyleClass("btn-group");
    
    calculateButton_ = buttonGroup->addNew<WPushButton>("Calculate");
    calculateButton_->setStyleClass("btn btn-primary btn-lg"); // calculate button (primary button)
    calculateButton_->clicked().connect(this, &ProcessSchedulingCalculator::onCalculateClicked);
    
    clearButton_ = buttonGroup->addNew<WPushButton>("Clear");
    clearButton_->setStyleClass("btn btn-secondary btn-lg"); // clear button (secondary button)
    clearButton_->clicked().connect(this, &ProcessSchedulingCalculator::onClearClicked);
    
    // alert container instantiation for alert messages
    alertContainer_ = buttonSection->addNew<WContainerWidget>();
    alertContainer_->setStyleClass("mt-3");
}


// set the output/result section for user outputs
// combination of 2 section (grantt chart + statistics table and summury)
inline void ProcessSchedulingCalculator::setupResultsSection(){
    
    resultsContainer_ = mainContainer_->addNew<WContainerWidget>();
    resultsContainer_->setStyleClass("row mb-4");
    resultsContainer_->setId("results-section");
    resultsContainer_->hide(); // initially hide the whole resultant container
    
    setupGanttChart();
    setupStatisticsTable();
}


// design the ui structure of grantt chart 
inline void ProcessSchedulingCalculator::setupGanttChart(){
    
    auto ganttSection = resultsContainer_->addNew<WContainerWidget>();
    ganttSection->setStyleClass("col-12 mb-4");
    
    auto ganttCard = ganttSection->addNew<WContainerWidget>();
    ganttCard->setStyleClass("card");
    
    auto cardHeader = ganttCard->addNew<WContainerWidget>();
    cardHeader->setStyleClass("card-header text-center bg-primary text-white");
    
    auto headerText = cardHeader->addNew<WText>("--- Grantt Chart ---");
    headerText->setStyleClass("h5 mb-0"); // grantt chart header text
    
    auto cardBody = ganttCard->addNew<WContainerWidget>();
    cardBody->setStyleClass("card-body");
    
    ganttContainer_ = cardBody->addNew<WContainerWidget>();
    ganttContainer_->setStyleClass("gantt-chart-container");
    
    ganttChartText_ = ganttContainer_->addNew<WText>("demo grant text");
    ganttChartText_->setTextFormat(TextFormat::XHTML);
}


// design the table-ui for process resultant/output table and summary section
inline void ProcessSchedulingCalculator::setupStatisticsTable(){
    
    auto statisticsSection = resultsContainer_->addNew<WContainerWidget>();
    statisticsSection->setStyleClass("col-lg-8 mb-4");
    
    auto statsCard = statisticsSection->addNew<WContainerWidget>();
    statsCard->setStyleClass("card");
    
    auto cardHeader = statsCard->addNew<WContainerWidget>();
    cardHeader->setStyleClass("card-header text-center bg-primary text-white");
    
    auto headerText = cardHeader->addNew<WText>("Process Statistics");
    headerText->setStyleClass("h5 mb-0"); // process output table header text
    
    auto cardBody = statsCard->addNew<WContainerWidget>();
    cardBody->setStyleClass("card-body");
    
    auto tableContainer = cardBody->addNew<WContainerWidget>();
    tableContainer->setStyleClass("table-responsive");
    
    statisticsTable_ = tableContainer->addNew<WTable>();
    statisticsTable_->setStyleClass("table table-striped table-hover");
    
    // statistics summary (calculated average times)
    
    auto summarySection = resultsContainer_->addNew<WContainerWidget>();
    summarySection->setStyleClass("col-lg-4 mb-4");
    
    auto summaryCard = summarySection->addNew<WContainerWidget>();
    summaryCard->setStyleClass("card");
    
    auto summaryHeader = summaryCard->addNew<WContainerWidget>();
    summaryHeader->setStyleClass("card-header text-center bg-primary text-white");
    
    auto summaryHeaderText = summaryHeader->addNew<WText>("Summary Statistics");
    summaryHeaderText->setStyleClass("h5 mb-0"); // summary section header text
    
    statisticsContainer_ = summaryCard->addNew<WContainerWidget>();
    statisticsContainer_->setStyleClass("card-body text-center");
    
    averageBurstTimeText_ = statisticsContainer_->addNew<WText>("demo avg bt");
    averageBurstTimeText_->setStyleClass("mb-2"); // average throughput/burst time text
    
    averageWaitingTimeText_ = statisticsContainer_->addNew<WText>("demo avg wt");
    averageWaitingTimeText_->setStyleClass("mb-2"); // average waititng time text
    
    averageTurnaroundTimeText_ = statisticsContainer_->addNew<WText>("demo avg tat");
    averageTurnaroundTimeText_->setStyleClass("mb-2"); // average turn-around time text
    
    averageResponseTimeText_ = statisticsContainer_->addNew<WText>("demo avg rt");
    averageResponseTimeText_->setStyleClass("mb-2"); // average response time text
}


#endif // __DesignUI__