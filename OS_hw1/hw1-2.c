#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<sys/time.h>

int min(a,b)
{
    return a>b?b:a;
}

struct SHM_t{
    int size;
    int matrix[1000][1000];
    unsigned int ans[4];
};

void create_process(int shm,int size,int index);

int main()
{
    int size = 0;
    printf("Dimension: ");
    scanf("%d",&size);
    struct SHM_t* mat;// = malloc(sizeof(struct SHM_t));
    struct shmid_ds shmbuffer;
    int now = 0;
    int shm_id = shmget(IPC_PRIVATE,sizeof(struct SHM_t),IPC_CREAT|0700);
    mat = (struct SHM_t*)shmat(shm_id,mat,0);
    mat->size = size;
    for (int i = 0 ; i < size ; i++ ){
        for (int j = 0 ; j < size ; j++){
            mat->matrix[i][j] = now;
            now++;
        }
    }
    for (int p_num = 1 ; p_num <= min(4,size) ; p_num++){
        for(int i = 0 ; i < 4 ;i++ )mat->ans[i] = 0;
        struct timeval start,end;
        gettimeofday(&start,0);
        unsigned int ans = 0;
        pid_t pid[4];
        for (int i = 0 ; i < p_num ; i++ ){
            pid[i] = fork();
            if (pid[i] < 0){
                fprintf(stderr,"Fork Error");
                abort();
            }else if(pid[i] == 0){
                create_process(shm_id,p_num,i);
                exit(0);
            }
        }

        for (int i = 0; i < p_num ; i++ ){
            int status;
            wait(&status);
        }

        for (int i = 0 ; i < p_num ; i++)ans += mat->ans[i];
        gettimeofday(&end,0);
        int sec = end.tv_sec - start.tv_sec;
        int usec = end.tv_usec - start.tv_usec;
        if(p_num==1 ||p_num==4)
        {
            printf("%d-process, ",p_num);
            printf("checksum = %u\n",ans);
            printf("elapsed %f s\n",(sec*1000+(usec/1000.0))/1000.0);
        }
    }

    shmdt(shm_id);
    shmctl(shm_id,IPC_RMID,0);
}

void create_process(int shm,int size,int index)
{
    struct SHM_t* tmp = (struct SHM_t*)shmat(shm,NULL,0);
    for (int i = index ; i < tmp->size ; i+=size ){
        for (int j = 0 ; j < tmp->size ; j++ ){
            for (int k = 0 ; k < tmp->size ; k++ ){
                tmp->ans[index] += tmp->matrix[i][j] * tmp->matrix[j][k];
            }
        }
    }
    shmdt(shm);
}
