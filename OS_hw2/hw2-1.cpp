#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int main()
{
    fstream infile;
    infile.open("Q1.txt",ios::in);
    int process_num;
    int burst_time[30] , waiting_time[30] , turnaround_time[30];
    for(int i = 0; i<30; i++)
    {
        burst_time[i] = 0;
        waiting_time[i] = 0;
        turnaround_time[i] = 0;
    }
    float avg_waiting_time = 0;
    float avg_turnaround_time = 0;
    //read file
    int time;
    if(infile.is_open())
    {
        infile >> process_num;
        for(int i = 0; i<process_num; i++)
        {
            infile >> burst_time[i];
        }
    }
    //cout << process_num << endl;
    //for(int i = 0; i<process_num; i++)
    //{
    //    cout << burst_time[i] << " ";
    //}
    cout << "Process" << "\t" << "Waiting Time" << "\t" << "Turnaround Time"<< endl;
    waiting_time[0] = 0;
    for(int i = 1; i<process_num; i++)
    {
        waiting_time[i] = 0;
        for(int j = 0; j<i; j++)
        {
            waiting_time[i]+=burst_time[j];
        }
    }

    for(int i = 0; i<process_num; i++)
    {
        turnaround_time[i] = burst_time[i] + waiting_time[i];
        avg_waiting_time+=waiting_time[i];
        avg_turnaround_time+=turnaround_time[i];
        cout << "P[" << i+1 << "]" << "\t" << waiting_time[i] << "\t\t" << turnaround_time[i] << endl;
    }
    avg_waiting_time/=process_num;
    avg_turnaround_time/=process_num;
    cout << endl;
    cout << endl;
    cout << "Average waiting time : " << avg_waiting_time << endl;
    cout << "Average turnaround time : " << avg_turnaround_time << endl;
}
