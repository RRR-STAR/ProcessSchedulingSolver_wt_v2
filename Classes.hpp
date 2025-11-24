
#ifndef __Classes__
#define __Classes__


#include <Wt/WApplication.h>
#include <Wt/WWidget.h>
#include <Wt/WWebWidget.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WBootstrap5Theme.h>
#include <Wt/WText.h>
#include <Wt/WBreak.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WComboBox.h>
#include <Wt/WTable.h>
#include <Wt/WTreeTable.h>
#include <Wt/WTableRow.h>
#include <Wt/WTableCell.h>
#include <Wt/WGroupBox.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WGridLayout.h>
#include <Wt/WLabel.h>
#include <Wt/WSpinBox.h>
#include <Wt/WDoubleSpinBox.h>
#include <Wt/WButtonGroup.h>
#include <Wt/WRadioButton.h>
#include <Wt/WTextArea.h>
#include <Wt/WProgressBar.h>
#include <Wt/WPanel.h>
#include <Wt/WBorderLayout.h>
#include <Wt/WCssDecorationStyle.h>
#include <Wt/WColor.h>
#include <Wt/WFont.h>
#include <Wt/WLength.h>
#include <Wt/WTemplate.h>
#include <Wt/WMemoryResource.h>
#include <Wt/WImage.h>
#include <Wt/WLink.h>
#include <Wt/WEnvironment.h>
#include <Wt/WString.h>
#include <Wt/WMessageBox.h>
#include <Wt/WAnimation.h>
#include <Wt/WJavaScript.h>

#include <vector>
#include <string>

using namespace Wt;


struct Process { // structure for process information
    short int  id;
    int        priority;
    double     arrivalTime;
    double     burstTime;
    double     burstTime2;
    double     waitingTime;
    double     turnaroundTime;
    double     completionTime;
    double     responseTime;
    double     remainingTime;
    double     remainingTime2;
    double     ioTime;
    bool       started;
    
    Process() : id(0), arrivalTime(0), burstTime(0), burstTime2(0), ioTime(0), priority(0), 
                waitingTime(0), turnaroundTime(0), completionTime(0), responseTime(0), 
                remainingTime(0), remainingTime2(0), started(false) {}
};


struct GanttSegment { // segemnts for grant chart creation
    short int    processId;
    double       startTime;
    double       endTime;
    std::string  color;
    
    GanttSegment(int id, double start, double end, const std::string &c) 
        : processId(id), startTime(start), endTime(end), color(c) {}
};


// main class for creating the whole application
class ProcessSchedulingCalculator : public Wt::WApplication {
    
    Wt::WContainerWidget *mainContainer_;            // header.C + input.C + result.C + footer.C (root.container)
    Wt::WContainerWidget *headerContainer_;          // for the header message section
    Wt::WContainerWidget *inputContainer_;           // all input widgets + alertContainer_
    Wt::WContainerWidget *alertContainer_;           // alert messages section
    Wt::WContainerWidget *resultsContainer_;         // granttContainer_ + statisticsContainer_(result part)
    Wt::WContainerWidget *ganttContainer_;           // grantt chart section (vertically - scrollable)
    Wt::WContainerWidget *statisticsContainer_;      // statistics constainer (showing avg. calculations)
    Wt::WContainerWidget *footerContainer_;          // footer message section
    
    // All input widgets of inputContainer_
    Wt::WComboBox      *algorithmComboBox_;          // drop-down selection box for different algorithm selection
    Wt::WSpinBox       *processCountSpinBox_;        // process input box, to specify no. of processes
    Wt::WDoubleSpinBox *quantumSpinBox_;             // time quantum input box, for RR algorithm
    Wt::WTable         *processTable_;               // process input table (At,Bt,Pr)
    Wt::WPushButton    *addProcessButton_;           // add process button to add a new process to the process input table
    Wt::WPushButton    *removeProcessButton_;        // remove process button to remove a process from the process input table
    Wt::WPushButton    *calculateButton_;            // calculate button to calculate the results of inputs
    Wt::WPushButton    *clearButton_;                // clear button to clear the results
    
    // Result widgets for resultContainer_
    Wt::WText  *ganttChartText_;                     // grantt chart (process - id & completion time)
    Wt::WTable *statisticsTable_;                    // process output table 
    Wt::WText  *averageBurstTimeText_;               // average burst time (throughput)
    Wt::WText  *averageWaitingTimeText_;             // average waiting time
    Wt::WText  *averageTurnaroundTimeText_;          // average turn-around time
    Wt::WText  *averageResponseTimeText_;            // average response time
    
    // Data handling 
    std::vector<Process>      processes_;            // data-structure to hold each process details.
    std::vector<GanttSegment> ganttSegments_;        // data-structure to hold each process details.
    std::string               selectedAlgorithm_;    // current selected algorithm name
    short int                 currentProcessCount_;  // current no. of processes
    double                    timeQuantum_;          // time quantum value
    
    std::vector<std::string> processColors_;         // color palette for processes to show in grantt chart
    
    
    /* Here only the internal function calling of memeber functions is listed, 
    *  and for description (what the function does) goto the function definition. */
    
    // Designer methods
    
    void createInterface();                // setupHeader(), setupInputSection(), setupResultSection(), setupFooterSection()
    void setupHeader();                    //
    void setupInputSection();              // setupAlgorithmSelection(), setupProcessInputTable(), setupControlButtons()
    void setupAlgorithmSelection();        // onProcessCountChanged, onAlgorithmChanged(), 
    void setupProcessInputTable();         // onAddProcessClicked(), onRemoveProcessClicked()
    void setupControlButtons();            // onCalculateClicked(), onClearClicked()
    void setupResultsSection();            // setupGanttChart(), setupStatisticsTable()
    void setupGanttChart();                // 
    void setupStatisticsTable();           //
    void setupFooter();                    //
    
    // Event handlers
    
    void onProcessCountChanged();          // updateProcessTable()
    void onAlgorithmChanged();             // clearResults()
    void onCalculateClicked();             // validateInputs(), display'x'Results(), showAlertMessage()
    void onClearClicked();                 // clearResults(), showAlertMessage()
    void onAddProcessClicked();            // onProcessCountChanged() or showAlertMessage()
    void onRemoveProcessClicked();         // onProcessCountChanged() or showAlertMessage()
    bool validateInputs();                 // showAlertMessage()
    
    // Algorithm implementations
    
    void displayFCFSResults();             //  , updateGanttChart(), updateStatistics()
    void displaySJFResults();              //  , updateGanttChart(), updateStatistics()
    void displayPriorityResults();         //  , updateGanttChart(), updateStatistics()
    void displayRoundRobinResults();       //  , updateGanttChart(), updateStatistics()
    void displaySRTFResults();             //  , updateGanttChart(), updateStatistics()
    void displayPRPResults();              //  , updateGanttChart(), updateStatistics()
    void displayIOResults();               //  , updateGanttChart(), updateStatistics()
    
    // UI update methods
    
    void updateProcessTable(const short int &newProcessCount);         // 
    void updateGanttChart(const std::vector<GanttSegment> &segments);  // generateGanttHTML()
    void updateStatistics(const std::vector<Process> &processes);      // 
    void showAlertMessage(const std::string &message, const bool &messageTypeIsSuccess);
    void clearResults();                                               //
    
    // Helper methods
    
    std::string generateGanttHTML(const std::vector<GanttSegment> &segments);  //
    std::string getProcessColor(const short int processId);                    // 
    
    
    public:
    ProcessSchedulingCalculator(const Wt::WEnvironment &env);
};


#endif // __Classes__