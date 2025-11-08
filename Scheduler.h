#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "Process.h"
#include <vector>
#include <algorithm>
#include <iomanip>
#include <climits>
using namespace std;

struct SchedulingMetrics{
    double avg_waiting_time;
    double avg_turnaround_time;
    string algorithm_name;
};

class Scheduler{
    private:
        vector<Process> processes;

        void calculateMetrics(vector<Process>& procs){
            for (auto& p : procs){
                p.setTurnaroundTime(p.getCompletionTime() - p.getArrivalTime());
                p.setWaitingTime(p.getTurnaroundTime() - p.getBurstTime());
            }
        }

        void printGanttChart(const vector<pair<int, int>>& gantt){
            cout << "\nGantt Chart\n";
            for (const auto& entry : gantt){
                cout << " P" << entry.first << " |";
            }
            cout << "\n";

            cout << "0";
            int time = 0;
            for (const auto& entry : gantt){
                time += entry.second;
                cout << "       " << time;
            }
            cout << "\n";
        }

        void printResults(const vector<Process>& procs, const string& algorithm){
            cout << "Algorithm: " << algorithm << "\n";
            cout << left << setw(6) << "PID"
                << setw(10) << "Arrival"
                << setw(10) << "Burst"
                << setw(12) << "Completion"
                << setw(12) << "Turnaround"
                << setw(10) << "Waiting" << "\n";
            cout << "----------------------------------------------------\n";

            double total_waiting = 0;
            double total_turnaround = 0;
            for (const auto& p : procs){
                cout << left << setw(6) << p.getPID()
                    << setw(10) << p.getArrivalTime()
                    << setw(10) << p.getBurstTime()
                    << setw(12) << p.getCompletionTime()
                    << setw(12) << p.getTurnaroundTime()
                    << setw(10) << p.getWaitingTime() << "\n";
                
                total_waiting += p.getWaitingTime();
                total_turnaround += p.getTurnaroundTime();
            }

            cout << "\nAverage Waiting Time: " << (total_waiting / procs.size()) << "\n";
            cout << "Average Turnaround Time: " << (total_turnaround / procs.size()) << "\n";
        }
    
    public:
        Scheduler(const vector<Process>& procs) : processes(procs) {}

        SchedulingMetrics scheduleFCFS(){
            vector<Process> procs = processes;
            vector<pair<int, int>> gantt;

            sort(procs.begin(), procs.end(),
                [](const Process& a, const Process& b){
                    return a.getArrivalTime() < b.getArrivalTime();
                });
            
            int current_time = 0;
            for (auto& p : procs){
                if (current_time < p.getArrivalTime()){
                    current_time = p.getArrivalTime();
                }

                current_time += p.getBurstTime();
                p.setCompletionTime(current_time);
                gantt.push_back({p.getPID(), p.getBurstTime()});
            }

            calculateMetrics(procs);
            printResults(procs, "First Come First Serve (FCFS)");
            printGanttChart(gantt);

            double avg_wt = 0;
            double avg_tat = 0;
            for (const auto& p : procs){
                avg_wt += p.getWaitingTime();
                avg_tat += p.getTurnaroundTime();
            }
            return {avg_wt / procs.size(), avg_tat / procs.size(), "FCFS"};
        }

        SchedulingMetrics scheduleSJF(){
            vector<Process> procs = processes;
            vector<pair<int, int>> gantt;
            vector<bool> completed(procs.size(), false);
            
            int current_time = 0;
            int completed_count = 0;
            while (completed_count < procs.size()){
                int shortest = -1;
                int min_burst = INT_MAX;

                for (size_t i = 0; i < procs.size(); i++){
                    if (!completed[i] && procs[i].getArrivalTime() <= current_time){
                        if (procs[i].getBurstTime() < min_burst){
                            min_burst = procs[i].getBurstTime();
                            shortest = i;
                        }
                    }
                }

                if (shortest == -1){
                    current_time++;
                } else {
                    current_time += procs[shortest].getBurstTime();
                    procs[shortest].setCompletionTime(current_time);
                    completed[shortest] = true;
                    completed_count++;
                    gantt.push_back({procs[shortest].getPID(), procs[shortest].getBurstTime()});
                }
            }

            calculateMetrics(procs);
            printResults(procs, "Shortest Job First (SJF)");
            printGanttChart(gantt);

            double avg_wt = 0;
            double avg_tat = 0;
            for (const auto& p : procs){
                avg_wt += p.getWaitingTime();
                avg_tat += p.getTurnaroundTime();
            }
            return {avg_wt / procs.size(), avg_tat / procs.size(), "SJF"};
        }

        SchedulingMetrics scheduleRoundRobin(int quantum){
            vector<Process> procs = processes;
            vector<pair<int, int>> gantt;

            for (auto&& p : procs){
                p.setRemainingTime(p.getBurstTime());
            }

            int current_time = 0;
            vector<int> ready_queue;
            vector<bool> in_queue(procs.size(), false);
            size_t next_arrival = 0;

            sort(procs.begin(), procs.end(),
                [](const Process& a, const Process& b){
                    return a.getArrivalTime() < b.getArrivalTime();
                });

            if (procs[0].getArrivalTime() <= current_time){
                ready_queue.push_back(0);
                in_queue[0] = true;
                next_arrival = 1;
            }

            while (!ready_queue.empty() || next_arrival < procs.size()){
                if (ready_queue.empty()){
                    current_time = procs[next_arrival].getArrivalTime();
                    ready_queue.push_back(next_arrival);
                    in_queue[next_arrival] = true;
                    next_arrival++;
                }

                int idx = ready_queue.front();
                ready_queue.erase(ready_queue.begin());

                int exec_time = min(quantum, procs[idx].getRemainingTime());
                procs[idx].setRemainingTime(procs[idx].getRemainingTime() - exec_time);
                current_time += exec_time;
                gantt.push_back({procs[idx].getPID(), exec_time});

                while (next_arrival < procs.size() && procs[next_arrival].getArrivalTime() <= current_time){
                    ready_queue.push_back(next_arrival);
                    in_queue[next_arrival] = true;
                    next_arrival++;
                }

                if (procs[idx].getRemainingTime() > 0){
                    ready_queue.push_back(idx);
                } else {
                    procs[idx].setCompletionTime(current_time);
                }
            }

            calculateMetrics(procs);
            printResults(procs, "Round Robin (Quantum = " + to_string(quantum) + ")");
            printGanttChart(gantt);

            double avg_wt = 0;
            double avg_tat = 0;
            for (const auto& p : procs){
                avg_wt += p.getWaitingTime();
                avg_tat += p.getTurnaroundTime();
            }
            return {avg_wt / procs.size(), avg_tat / procs.size(), "Round Robin"};
        }

        void compareAlgorithms(int quantum = 2){
            cout << "PROCESS SCHEDULING COMPARISON\n";
            
            auto fcfs_metrics = scheduleFCFS();
            auto sjf_metrics = scheduleSJF();
            auto rr_metrics = scheduleRoundRobin(quantum);

            cout << "\n\n";
            cout << "PERFORMANCE COMPARISON\n";
            cout << left << setw(20) << "Algorithm"
                 << setw(20) << "Average Waiting Time"
                 << setw(25) << "Average Turnaround Time" << "\n";
            
            cout << left << setw(20) << fcfs_metrics.algorithm_name
                 << setw(20) << fixed << setprecision(2) << fcfs_metrics.avg_waiting_time
                 << setw(25) << fcfs_metrics.avg_turnaround_time << "\n";

            cout << left << setw(20) << sjf_metrics.algorithm_name
                 << setw(20) << sjf_metrics.avg_waiting_time
                 << setw(25) << sjf_metrics.avg_turnaround_time << "\n";

            cout << left << setw(20) << rr_metrics.algorithm_name
                 << setw(20) << rr_metrics.avg_waiting_time
                 << setw(25) << rr_metrics.avg_turnaround_time << "\n";
            cout << "\n";
        }
};

#endif