/*
TaCoya S. Harris
CSCE 420
Prof. Wu
October 15th, 2021
Implementation of RR and SJF scheduling algorithms
*/
#include <iostream>
#include <queue> 
#include <fstream>
#include <vector>
#include <cstdlib>
#include <iomanip>
using namespace std;

class Process {
    public:
    int pid = 0;
    int arrivalTime = 0;
    int burstTime = 0;
    int currentbt = 0;
    int startTime = 0;
    int finishTime = 0;
    int tatTime = 0;
    int waitTime = 0;
    int ctr = 0;

    //load Process data from file
    void loadFile(vector<int> &data, string f){
        ifstream file(f, ios::in); //open file
        int value;
        while (file >> value){ 
            data.push_back(value);   
        }
        file.close();
    }
    //generate random Process data
    void generateData(vector<int> &data, int n){
        int value = 0;
        for (int i = 0; i < n/2; i++){
                //generate random burst times
                value = 1 + rand() % 10; //1-10
                data.push_back(value);

        }
        for (int i = n/2; i < n; i++){
            //generate random arrival times
            value = rand() % 11;//0-10
            data.push_back(value);
        }
    }
    //sort Processes by arrival time
    void sortArrival(Process *p, int n){
        int i, j;   
        for (i = 0; i < n-1; i++)    
            for (j = 0; j < n-i-1; j++)
                if (p[j].arrivalTime > p[j+1].arrivalTime)
                    swap(p[j], p[j+1]); 
    }
    //sort process ids for display
    void sortPID(Process *p, int n){
        int i, j;   
        for (i = 0; i < n-1; i++)    
            for (j = 0; j < n-i-1; j++)
                if (p[j].pid > p[j+1].pid)
                    swap(p[j], p[j+1]); 
    }
    //set pid, burst t, arrival t
    void setMemVars(vector<int> &pData, Process *p, int n){
        for(int i=0; i < n; i++){
        p[i].pid = i + 1;
        }   
        for(int i=0; i < n; i++){
            p[i].burstTime = pData[i];
        }
        int j = pData.size()/2;
        for(int i = 0; i < pData.size() ; i++){
            p[i].arrivalTime = pData[j];
            j++;
        }
        for(int i = 0; i < pData.size()/2; i++) {
            p[i].currentbt = p[i].burstTime;
        }
    }
    //Round Robin scheduling
    void RR(Process *p, int n, int tq){
        queue<int> rq;//ready queue
        rq.push(0);
        int currentTime = 0;
        int Processed = 0;//finished processes
        int j = 0;

        sortArrival (p, n);//sort by arrival

        cout << "Gantt Chart: ";
        while(Processed != n){ 
            cout << "(" << currentTime << ") ";
            j = rq.front(); //get next Process in ready queue
            cout<<"P"<<p[j].pid<< " ";
            rq.pop();
            //if current burst time is the same as starting burst time
            if(p[j].currentbt == p[j].burstTime){
                p[j].startTime = max(currentTime,p[j].arrivalTime);
                currentTime = p[j].startTime;
            }
            //if current burst time > 0
            if(p[j].currentbt-tq > 0) {
                p[j].currentbt -= tq;
                currentTime += tq;
            }
            else {//curent bt = 0 //process done
                currentTime += p[j].currentbt;
                p[j].currentbt = 0;
                Processed++;
                p[j].finishTime = currentTime;
                //turnaround time = end time - arrival time
                p[j].tatTime = p[j].finishTime - p[j].arrivalTime;
                //wait time = turnaround time - burst time
                p[j].waitTime = p[j].tatTime - p[j].burstTime;
            }
            for(int i = 1; i < n; i++) {
                if(p[i].currentbt > 0 && p[i].arrivalTime <= currentTime && p[i].ctr == 0) {
                    rq.push(i);//put process back into ready queue
                    p[i].ctr = 1;
                }
            }
            if(p[j].currentbt > 0) {
                rq.push(j);//put back in ready queue
            }
            if(rq.empty()) {//if there are still processes in ready queue
                for(int i = 1; i < n; i++) {
                    if(p[i].currentbt > 0) {
                        rq.push(i);//put back in ready queue
                        p[i].ctr = 1;
                        break;
                    }
                }
            }
        //all processes finished
        }cout << "(" << currentTime << ")" <<endl;  
    }
    void SJF(Process *p, int n){
        sortArrival (p, n);//sort by arrival

        cout << "Gantt Chart: ";

        int currentTime = 0;
        int processed = 0;
    

        while(processed != n) {
            int j = -1;
            //cpu does not know how long processes 
            //will take to finish so set min time to high value
            int min = 10000000;
            cout << "(" << currentTime << ") ";
           
            for(int i = 0; i < n; i++) {
                //if arrival time is <= current and process has not been processed yet
                if(p[i].arrivalTime <= currentTime && p[i].ctr == 0) {//may need to reset ctr
                    if(p[i].burstTime < min) {
                        min = p[i].burstTime;
                        j = i;
                    }
                    if(p[i].burstTime == min) {
                        if(p[i].arrivalTime < p[j].arrivalTime) {
                            //set min total amount of time to burst time with earliest arrival time
                            min = p[i].burstTime;
                            j = i;
                        }
                    }
                }
            }
            if(j != -1) {
                cout<<"P"<<p[j].pid<< " ";
                p[j].startTime = currentTime;
                p[j].finishTime = p[j].startTime + p[j].burstTime;
                p[j].tatTime = p[j].finishTime - p[j].arrivalTime;
                p[j].waitTime = p[j].tatTime - p[j].burstTime;
          
                p[j].ctr = 1;
                processed++;
                currentTime = p[j].finishTime;

            }
            else {
                currentTime++;
            }
            
        }cout << "(" << currentTime << ")\n";
            

    }
    void display (Process *p, int n){
        cout<<"\nProcess"<<"--------->\t";
        for(int i = 0; i < n; i++)
            cout<<p[i].pid<<"\t";
        cout<<endl;
        cout<<"BurstTime"<<"------->\t";
        for(int i = 0; i < n; i++)
            cout<<p[i].burstTime<<"\t";
        cout<<endl;
        cout<<"ArrivalTime"<<"----->\t";
        for(int i = 0; i < n; i++)
            cout<<p[i].arrivalTime<<"\t";
        cout<<endl;
        cout<<"TurnaroundTime"<<"-->\t";;
        for(int i = 0; i < n; i++)
            cout<<p[i].tatTime<<"\t";
        cout<<endl;
        cout<<"WaitTime"<<"-------->\t";
        for(int i = 0; i < n; i++)
            cout<<p[i].waitTime<<"\t";
        cout<<endl;
    }
};

int main() {

    int in;
    int n = 0;
    Process p;
    vector<int> pData;

    cout<<"0. Load a file." << endl;
    cout<<"1. Enter # of Processes." << endl; 
    cout<<"Select an option: ";
    cin >> in;
   
    while (in < 0 || in > 1){
        cout<<"Select an option: ";
        cin >> in;
    }
    if (in == 0){
        cout<<"Enter file name: ";
        string file;
        cin >> file;
        p.loadFile(pData, file);
    }
    else{
        cout<<"Enter max Process #: ";
        cin >> in;
        while (in > 20 || in < 1){
            cout<<"Enter max Process #: ";
            cin >> in;
        }
        p.generateData(pData, in*2);
    }
  
    n = pData.size()/2;
    int tq;
    Process ps[n];//array of Process objects

    cout << "Enter a time Quantum: ";
    cin >> tq;
    cout << endl;

    p.setMemVars(pData, ps, n);
    
    cout<<"P\t"<<"BT\t"<<"AT\n";
    for(int i = 0; i < n ; i++) {
        cout<<ps[i].pid<<"\t"<<ps[i].burstTime<<"\t"<<ps[i].arrivalTime<<"\t"<<"\n"<<endl;
    }

    cout << "ROUND ROBIN...\n" << endl;

    cout << "Time Quantum: " << tq << endl;
    
    p.RR(ps, n, tq);
    
    p.sortPID(ps, n);
    p.display(ps, n);

    //reset member vars for sjf
    for(int i = 0; i < n ; i++) {
        ps[i].pid = 0;
        ps[i].arrivalTime = 0;
        ps[i].burstTime = 0;
        ps[i].currentbt = 0;
        ps[i].startTime = 0;
        ps[i].finishTime = 0;
        ps[i].tatTime = 0;
        ps[i].waitTime = 0;
        ps[i].ctr = 0;
    }

    cout << "\nSHORTEST JOB FIRST...\n" << endl;

    p.setMemVars(pData, ps, n);
    
    p.SJF(ps,n);
    cout << setprecision(2) << fixed;
    p.sortPID(ps, n);
    p.display(ps, n);
    
}