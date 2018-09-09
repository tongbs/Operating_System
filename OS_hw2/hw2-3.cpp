#include <iostream>
#include <bits/stdc++.h>

using namespace std;

struct input
{
    int *ptr_id;
    int *ptr_ar;
    int *ptr_bu;
    int *ptr_wa;
    int *ptr_tu;
    int *ptr_re;
};

/*bool cmp(input min1 , input max1)
{
    if((*min1.ptr_bu) == (*max1.ptr_bu))
    {
        return *(min1.ptr_id) < *(max1.ptr_id);
    }
    else
        return *(min1.ptr_bu) < *(max1.ptr_bu);
}*/

bool cmp1(input min11 , input max11)
{
    if((*min11.ptr_id) < (*max11.ptr_id))
    {
        return true;
    }
    else
        return false;
}
int scheduling (vector<input> p , int now_time , int num)
{
    int now_pc = -1;
    int check = 0;
    int burst = 0;
    for(int i = 0; i<num; i++)
    {
        if(*(p[i].ptr_ar)<=now_time && *(p[i].ptr_re)!=0)
        {
            if(check == 0)
            {
                check = 1;
                now_pc = *(p[i].ptr_id);
                burst = *(p[i].ptr_re);
            }
            else
            {
                if(*(p[i].ptr_re)<burst)
                {
                    now_pc = *(p[i].ptr_id);
                    burst = *(p[i].ptr_re);
                }
            }
        }
    }
    return now_pc;
}

int main()
{
    fstream infile;
    infile.open("Q3.txt",ios::in);
    int process_num;
    int point[25] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};
    int arrival_time[25] , burst_time[25] , waiting_time[25] , turnaround_time[25] , remain[25];
    float avg_waiting_time = 0;
    float avg_turnaround_time = 0;
    for(int i = 0; i<25; i++)
    {
        arrival_time[i] = 0;
        burst_time[i] = 0;
        waiting_time[i] = 0;
        turnaround_time[i] = 0;
        remain[i] = 0;
    }
    //read file
    if(infile.is_open())
    {
        infile >> process_num;
        for(int i = 0; i<process_num; i++)
        {
            infile >> arrival_time[i];
        }
        for(int i = 0; i<process_num; i++)
        {
            infile >> burst_time[i];
        }
    }
    for(int i = 0; i<process_num; i++)
    {
        remain[i] = burst_time[i];
    }

    vector<input> p;
    input tmp;
    for(int i = 0; i<process_num; i++)
    {
        tmp.ptr_id = &point[i];
        tmp.ptr_ar = &arrival_time[i];
        tmp.ptr_bu = &burst_time[i];
        tmp.ptr_wa = &waiting_time[i];
        tmp.ptr_tu = &turnaround_time[i];
        tmp.ptr_re = &remain[i];
        p.push_back(tmp);
    }

    //sort(p.begin(),p.end(),cmp); // sort by burst_time
    /*for(int i = 0; i<process_num; i++)
    {
        cout << *(p[i].ptr_id) << " " << *(p[i].ptr_ar) << " " << *(p[i].ptr_bu) << " " << *(p[i].ptr_wa) << " " << *(p[i].ptr_tu) << " " << *(p[i].ptr_re) << endl;
    }*/
    int complete;
    complete = process_num;
    int now_time = 0;
    int now_p;
    /*for(int i = 0; i<process_num; i++)
    {
        cout << *(p[i].ptr_re) << " ";
    }*/
    while(complete!=0)
    {
        now_p = scheduling(p,now_time++,process_num);
        if(now_p == -1) now_time++;
        else
        {
            for(int j = 0; j<process_num; j++)
            {
                if(now_p == *(p[j].ptr_id))
                {
                    //cout << now_p << "LL" << endl;
                    //system("pause");
                    //cout << "q" << *(p[j].ptr_re);
                    *(p[j].ptr_re) = *(p[j].ptr_re) - 1; //do not use --!!
                    //cout << *(p[j].ptr_re) << "MM";
                    //system("pause");
                    if(*(p[j].ptr_re) == 0)
                    {
                        complete--;
                        *(p[j].ptr_tu) = now_time - *(p[j].ptr_ar);
                        *(p[j].ptr_wa) = *(p[j].ptr_tu) - *(p[j].ptr_bu);//turnaround - burst = waiting
                        avg_waiting_time = avg_waiting_time + *(p[j].ptr_wa);
                        avg_turnaround_time = avg_turnaround_time + *(p[j].ptr_tu);
                    }
                    break;
                }
            }
        }
    }
    avg_waiting_time/=process_num;
    avg_turnaround_time/=process_num;
    //sort(p.begin(),p.end(),cmp1);
    cout << "Process" << "\t" << "Waiting Time" << "\t" << "Turnaround Time"<< endl;
    for(int h = 0; h<process_num; h++)
    {
        cout << "P[" << *(p[h].ptr_id) << "]" << "\t" << *(p[h].ptr_wa) << "\t\t" << *(p[h].ptr_tu) << endl;
    }
    cout << endl;
    cout << endl;
    cout << "Average waiting time : " << avg_waiting_time << endl;
    cout << "Average turnaround time : " << avg_turnaround_time << endl;
}
