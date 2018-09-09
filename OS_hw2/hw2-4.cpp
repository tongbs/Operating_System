#include <bits/stdc++.h>
using namespace std;
const int high = 1;
const int mid = 2;
const int low = 3;

struct input2
{
    int *ptr_id;
    int *ptr_ar;
    int *ptr_bu;
    int *ptr_wa;
    int *ptr_tu;
    int *ptr_re;
};

struct input
{
    int p_id;
    int p_ar;
    int p_bu;
    int p_wa;
    int p_tu;
    int p_re;
    int p_q2_ar;
};

bool cmp(input2 a , input2 b)
{
    if((*a.ptr_bu) == (*b.ptr_bu))
    {
        return *(a.ptr_id) < *(b.ptr_id);
    }
    else
        return *(a.ptr_bu) < *(b.ptr_bu);
}

bool cmpq0 (input min1, input max1)
{
    return min1.p_ar < max1.p_ar;
}

bool cmpq1 (input min2, input max2)
{
    return min2.p_q2_ar < max2.p_q2_ar;
}

struct multilevel
{
    int now_q;
    int now_p;
};

int scheduling (vector<input2> p , int now_t , int num)
{
    int now_pc = -1;
    int check1 = 0;
    int burst = 0;
    for(int i = 0; i<num; i++)
    {
        if(*(p[i].ptr_ar)<=now_t && *(p[i].ptr_re)!=0)
        {
            if(check1 == 0)
            {
                check1 = 1;
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

multilevel work (const vector<input>* q_level , int now_time , int process_num , vector<input2> c)
{
    multilevel fb_q;
    if(process_num!=0)
    {
        for(int i = 0; i<process_num; i++)
        {
            *(c[i].ptr_re) = *(c[i].ptr_re) - 1;
        }
    if (q_level[0].front().p_ar <= now_time && q_level[0].size() > 0 && process_num > 0)
    {
        fb_q.now_p = q_level[0].front().p_id;
        fb_q.now_q = high;
        int f2 = 9;
        for(int i = 0; i<process_num; i++)
        {
            *(c[i].ptr_tu) = f2 - *(c[i].ptr_ar);
            *(c[i].ptr_wa) = *(c[i].ptr_tu) - *(c[i].ptr_bu);
        }
        return fb_q;
    }
    else if (q_level[1].front().p_q2_ar <= now_time && q_level[1].size() > 0 && process_num > 0)
    {
        fb_q.now_p = q_level[1].front().p_id;
        fb_q.now_q = mid;
        return fb_q;
    }
    else
    {
        int f = process_num;

        *(c[f-1].ptr_tu) = f - *(c[f-1].ptr_ar);
        *(c[f-1].ptr_wa) = *(c[f-1].ptr_tu) - *(c[f-1].ptr_bu);

        int now_p = -1;
        int short_burst = 0;
        bool check = false;
        for (int i = 0; i < q_level[2].size(); i++)
        {
            if (q_level[2].at(i).p_ar <= now_time && q_level[2].at(i).p_re != 0 && process_num > 0)
            {
                if (!check)
                {
                    now_p = q_level[2].at(i).p_id;
                    short_burst = q_level[2].at(i).p_re;
                    check = true;
                }
                else
                {
                    if (q_level[2].at(i).p_re < short_burst && process_num > 0)
                    {
                        now_p = q_level[2].at(i).p_id;
                        short_burst = q_level[2].at(i).p_re;
                    }
                }
            }
        }
        fb_q.now_p = now_p;
        fb_q.now_q = low;
        return fb_q;
    }
    }
}


bool cmpq2 (input min3, input max3)
{
    return min3.p_id < max3.p_id;
}

int main()
{
    //fstream infile;
    //infile.open("Q3.txt",ios::in);
    //int process_num;
    int point[25] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};
    int arrival_time[25] , burst_time[25] , waiting_time[25] , turnaround_time[25] , remain[25];
    int process_num;
    float avg_waiting_time = 0;
    float avg_turn_around_time = 0;
    int high_qt;
    int mid_qt;
    vector<input> q_level[3];
    for(int i = 0; i<25; i++)
    {
        arrival_time[i] = 0;
        burst_time[i] = 0;
        waiting_time[i] = 0;
        turnaround_time[i] = 0;
        remain[i] = 0;
    }
    fstream infile;
    infile.open("Q4.txt", ios::in);
    if(infile.is_open())
    {
        infile >> process_num;
        q_level[0].resize(process_num);
        for (int i = 0; i < process_num; i++)
        {
            infile >> q_level[0].at(i).p_ar;
        }
        for (int i = 0; i < process_num; i++)
        {
            infile >> q_level[0].at(i).p_bu;
            q_level[0].at(i).p_re = q_level[0].at(i).p_bu;
        }
        infile >> high_qt >> mid_qt;
    }
    vector<input2> c;
    for(int i = 0; i<process_num; i++)
    {
        arrival_time[i] = q_level[0].at(i).p_ar;
        burst_time[i] = q_level[0].at(i).p_bu;
    }
    input2 tmp;
    for(int i = 0; i<process_num; i++)
    {
        tmp.ptr_id = &point[i];
        tmp.ptr_ar = &arrival_time[i];
        tmp.ptr_bu = &burst_time[i];
        tmp.ptr_wa = &waiting_time[i];
        tmp.ptr_tu = &turnaround_time[i];
        tmp.ptr_re = &remain[i];
        c.push_back(tmp);
    }
    sort(c.begin(),c.end(),cmp);

    for(int i = 0; i < process_num; i++)
    {
        q_level[0].at(i).p_id = i;
    }
    int complete;
    complete = process_num;
    int now_time= 0;
    multilevel fb_q;
    int now_t = 0;
    int now;
    while (complete != 0)
    {
        now = scheduling(c,now_t,process_num);
        int l = q_level[0].size();
        int m = q_level[1].size();

        sort(q_level[0].begin(), q_level[0].begin()+l, cmpq0);
        sort(q_level[1].begin(), q_level[1].begin()+m, cmpq1);
        for(int i = 0;i<process_num; i++)
        {
            *(c[i].ptr_re) = *(c[i].ptr_re) - 1;
        }
        fb_q = work(q_level,now_time,process_num,c);
        if (fb_q.now_p == -1)
        {
            now_time++;
            for(int j = 0;j<process_num; j++)
            {
                int nt = 0;
                *(c[j].ptr_tu) = nt - *(c[j].ptr_ar);
                *(c[j].ptr_wa) = *(c[j].ptr_tu) - *(c[j].ptr_bu);
            }
        }

        else
        {
            vector<input2> h;
            if (fb_q.now_q == mid)
            {
                if (q_level[1].front().p_re > mid_qt)
                {
                        int s = 2;
                        tmp.ptr_id = &point[s];
                        tmp.ptr_ar = &arrival_time[s];
                        tmp.ptr_bu = &burst_time[s];
                        tmp.ptr_wa = &waiting_time[s];
                        tmp.ptr_tu = &turnaround_time[s];
                        tmp.ptr_re = &remain[s];
                        h.push_back(tmp);

                    now_time += mid_qt;
                    q_level[1].front().p_re -= mid_qt;
                    q_level[2].push_back(q_level[1].front());
                    q_level[1].erase(q_level[1].begin());
                }
                else
                {

                    complete--;
                    q_level[1].front().p_tu = now_time + q_level[1].front().p_re - q_level[1].front().p_ar;
                    q_level[1].front().p_wa = q_level[1].front().p_tu - q_level[1].front().p_bu;
                    now_time += q_level[1].front().p_re;
                    q_level[1].front().p_re = 0;
                    q_level[2].push_back(q_level[1].front());
                    q_level[1].erase(q_level[1].begin());
                    for(int j = 0;j<process_num; j++)
                    {
                        int g = 1;
                        *(c[g].ptr_tu) = g - *(c[g].ptr_ar);
                        *(c[g].ptr_wa) = *(c[g].ptr_tu) - *(c[g].ptr_bu);
                    }
                }
            }
            if(fb_q.now_q==high)
            {
                for(int j = 0;j<process_num; j++)
                {
                    int t = 0;
                    *(c[j].ptr_tu) = t - *(c[j].ptr_ar);
                    *(c[j].ptr_wa) = *(c[j].ptr_tu) - *(c[j].ptr_bu);
                }
                if (q_level[0].front().p_re > high_qt)
                {
                    now_time += high_qt;
                    q_level[0].front().p_re -= high_qt;
                    q_level[0].front().p_q2_ar = now_time;
                    q_level[1].push_back(q_level[0].front());
                    q_level[0].erase(q_level[0].begin());
                    for(int i = 0;i<process_num; i++)
                    {
                        *(c[i].ptr_re) = *(c[i].ptr_re) - 1;
                    }
                }
                else
                {
                    complete--;
                    q_level[0].front().p_tu = now_time + q_level[0].front().p_re - q_level[0].front().p_ar;
                    q_level[0].front().p_wa = q_level[0].front().p_tu - q_level[0].front().p_bu;
                    now_time += q_level[0].front().p_re;
                    q_level[0].front().p_re = 0;
                    q_level[2].push_back(q_level[0].front());
                    q_level[0].erase(q_level[0].begin());
                    *(c[0].ptr_re) = *(c[0].ptr_re) - 1;
                }
            }
            if (fb_q.now_q==low)
            {
                for (int i = 0; i < q_level[2].size(); i++)
                {
                    for(int k = 0;k<process_num; k++)
                        {
                            int u = 0;
                            *(c[k].ptr_tu) = u - *(c[k].ptr_ar);
                            *(c[k].ptr_wa) = *(c[k].ptr_tu) - *(c[k].ptr_bu);
                        }
                    if (fb_q.now_p == q_level[2].at(i).p_id)
                    {
                        complete--;
                        q_level[2].at(i).p_tu = now_time + q_level[2].at(i).p_re - q_level[2].at(i).p_ar;
                        q_level[2].at(i).p_wa = q_level[2].at(i).p_tu - q_level[2].at(i).p_bu;
                        now_time += q_level[2].at(i).p_re;
                        q_level[2].at(i).p_re = 0;
                        for(int j = 0;j<process_num; j++)
                        {
                            int t = 0;
                            *(c[j].ptr_tu) = t - *(c[j].ptr_ar);
                            *(c[j].ptr_wa) = *(c[j].ptr_tu) - *(c[j].ptr_bu);
                        }
                        break;
                        }
                    }
                }
            }
    }
    int n = q_level[2].size();
    sort(q_level[2].begin(), q_level[2].begin()+n, cmpq2);
    for(int i = 0; i< process_num; i++)
    {
        avg_waiting_time += q_level[2].at(i).p_wa;
        avg_turn_around_time += q_level[2].at(i).p_tu;
    }
    cout << "Process" << "\t" << "Waiting Time" << "\t" << "Turnaround Time"<< endl;

    for (int i = 0;i < process_num; i++)
    {
        cout << "P[" << q_level[2].at(i).p_id + 1 << "]" << "\t" << q_level[2].at(i).p_wa << "\t\t" << q_level[2].at(i).p_tu << endl;
    }
    avg_waiting_time/=process_num;
    avg_turn_around_time/=process_num;
    cout << endl;
    cout << endl;
    cout << "Average waiting time : " << avg_waiting_time << endl;
    cout << "Average turnaround time : " << avg_turn_around_time << endl;
}
