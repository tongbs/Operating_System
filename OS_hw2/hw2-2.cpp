#include <bits/stdc++.h>

using namespace std;

struct input
{
    int *ptr11;//process.no
    int *ptr22;//arrival
    int *ptr33;//burst
    int *ptr44;//waiting
    int *ptr55;//turn
    int *ptr66;//do it or not
};

bool cmp1(input min11, input max11)
{
    if((*min11.ptr11) < (*max11.ptr11))
    {
        return true;
    }
    else
        return false;
}

bool cmp(input min1, input max1)
{
    if((*min1.ptr33) == (*max1.ptr33))
    {
        return *(min1.ptr11) < *(max1.ptr11);
    }
    else
        return *(min1.ptr33) < *(max1.ptr33);

}

int main()
{
    fstream infile;
    infile.open("Q2.txt",ios::in);
    int process_num;
    int point[30] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};
    int arrival_time[30] , waiting_time[30] , burst_time[30] , turnaround_time[30];
    float avg_waiting_time = 0;
    float avg_turnaround_time = 0;
    vector<input> c;
    for(int i = 0; i<30; i++)
    {
        arrival_time[i] = 0;
        waiting_time[i] = 0;
        burst_time[i] = 0;
        turnaround_time[i] = 0;
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
    int zero[20] = {0};
    input tmp1;
    for(int i = 0; i<process_num; i++)
    {

        tmp1.ptr11 = &point[i];
        tmp1.ptr22 = &arrival_time[i];
        tmp1.ptr33 = &burst_time[i];
        tmp1.ptr44 = &waiting_time[i];
        tmp1.ptr55 = &turnaround_time[i];
        tmp1.ptr66 = &zero[i];
        c.push_back(tmp1);
    }

    /*for(int i = 0; i<process_num; i++)
    {
        cout << *(c[i].ptr11) << " " << *(c[i].ptr22) << " " << *(c[i].ptr33) << " " << *(c[i].ptr44) << " " << *(c[i].ptr55) << " " << *(c[i].ptr66) << endl;
    }
    cout << "-----------------------------" << endl;*/

    sort(c.begin(),c.end(),cmp);

    /*for(int i = 0; i<process_num; i++)
    {
        cout << *(c[i].ptr11) << " " << *(c[i].ptr22) << " " << *(c[i].ptr33) << " " << *(c[i].ptr44) << " " << *(c[i].ptr55) << " " << *(c[i].ptr66) << endl;
    }
    cout << "-----------------------------" << endl;*/
    int time = 0;
    for(int i = 0; i<process_num; i++)
    {
        int f = 0, nowID = -1;
        for(int j = 0; j<process_num; j++)
        {
            if((!*(c[j].ptr66)) && *(c[j].ptr22) <= time)
            {
                nowID = j;
                break;
            }
        }

        if(nowID == -1)
        {
            time++;
            i--;
            //cout << "112233" << endl;
        }
        else
        {
            //if(i==0) *(c[nowID].ptr44) = time;
            *(c[nowID].ptr44) = time - *(c[nowID].ptr22);
            time += *(c[nowID].ptr33);
            *(c[nowID].ptr55) = time - *(c[nowID].ptr22);
            //cout << "--------------" <<endl;

            //cout << "i = " << i << "f = " << f << endl;
            //system("pause");

            //for(int j = i+1; j<process_num; j++)
            //{
                //cout << *(c[nowID].ptr66) << endl;
               /* if((!*(c[nowID].ptr66)) && *(c[nowID].ptr22) <= time)
                {
                    if(j > f)f = j;
                    //cout << "fuck ";
                }*/
            //}
            //cout << endl;
            //cout << "f = " << f << endl;
            //system("pause");

            //if(i!= (process_num-1)) sort(c.begin()+i+1, c.begin()+f+1, cmp);
            //cout << "WWW" << *(c[1].ptr33) << endl;
            *(c[nowID].ptr66) = 1;
        }
    }
    for(int h = 0; h<process_num; h++)
    {
        avg_waiting_time+=waiting_time[h];
        avg_turnaround_time+=turnaround_time[h];
    }
    avg_waiting_time/=process_num;
    avg_turnaround_time/=process_num;
    sort(c.begin(),c.end(),cmp1);
    cout << "Process" << "\t" << "Waiting Time" << "\t" << "Turnaround Time"<< endl;
    for(int h = 0; h<process_num; h++)
    {
        cout << "P[" << *(c[h].ptr11) << "]" << "\t" << *(c[h].ptr44) << "\t\t" << *(c[h].ptr55) << endl;
    }
    cout << endl;
    cout << endl;
    cout << "Average waiting time : " << avg_waiting_time << endl;
    cout << "Average turnaround time : " << avg_turnaround_time << endl;
}


/*
if(i==0) *(c[nowID].ptr44) = time;
            else *(c[nowID].ptr44) = time - *(c[nowID].ptr22);
            time += *(c[nowID].ptr33);
            *(c[nowID].ptr55) = time - *(c[nowID].ptr22);
            cout << "--------------" <<endl;

            cout << "i = " << i << "f = " << f << endl;
            system("pause");

            for(int j = i+1; j<process_num; j++)
            {
                cout << *(c[nowID].ptr66) << endl;
                if((!*(c[nowID].ptr66)) && *(c[nowID].ptr22) <= time)
                {
                    if(j > f)f = j;
                    cout << "fuck ";
                }
            }
            cout << endl;
            cout << "f = " << f << endl;
            system("pause");

            if(i!= (process_num-1)) sort(c.begin()+i+1, c.begin()+f+1, cmp);
            cout << "WWW" << *(c[1].ptr33) << endl;
            *(c[i].ptr66) = 1;
*/
