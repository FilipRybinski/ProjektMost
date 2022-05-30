#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#define true 0
#define false 1
#define CITY_A 0
#define CITY_B 1
int FLAG=false;
int Car_waitting_dst_B=0;
int Car_waitting_dst_A=0;
int Complete_to_B=0;
int Complete_to_A=0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

void Bridge(int car_direction){
	for(int i=0;i<100000;i++){
        for(int j=0;j<1000;j++){}
    }
}
void Road_info(int id,int dst){
    if(dst==CITY_A){
        printf("A-%d %d>>> [<< %d <<] <<<%d %d-B\n",Complete_to_A,Car_waitting_dst_B,id,Car_waitting_dst_A, Complete_to_B);
    }else{
        printf("A-%d %d>>> [>> %d >>] <<<%d %d-B\n",Complete_to_A,Car_waitting_dst_B,id,Car_waitting_dst_A, Complete_to_B);
    }
}
void *Village(void* arg){ 
    int val= *((int*)arg);
    int car_direction =rand()%2;
    //printf("CAR ID %d and Destination %s\n",val,car_direction==CITY_A ? "CITY_A" : "CITY_B");//
    if(car_direction==CITY_A){Car_waitting_dst_A++;}else{Car_waitting_dst_B++;} 
	pthread_mutex_lock(&mutex1); 
    Bridge(car_direction);
    if(FLAG==true)Road_info(val,car_direction);
    if(car_direction==CITY_A){Car_waitting_dst_A--;}else{Car_waitting_dst_B--;}
    if(car_direction==CITY_B){Complete_to_B++;}else{Complete_to_A++;}
    if(FLAG==true)Road_info(val,car_direction);
	pthread_mutex_unlock(&mutex1); 
	pthread_exit(0);
}
int main(int argc, char *argv[]){
    char *tmp_debug="-debug";
    if(argc<=1 || argc >3){
                printf("Error\n");
    }else{
    if(argc==3){if(strcmp(argv[2],tmp_debug)==0){FLAG=true;}}
    printf("Processing....\n");
    srand((time)NULL);
    int CAR_COUNT =atoi(argv[1]);
	pthread_t A_Threads[CAR_COUNT]; // CARs
	pthread_mutex_init(&mutex1,NULL);
    while (1)
    {
	for(int i = 0; i < CAR_COUNT; i++){
        int* id_car = (int*)malloc(sizeof(int*));
        *id_car = i + 1;
    	int t1 =  pthread_create(&A_Threads[i], NULL, Village, id_car); 
   		if(t1 != 0){ 
      		fprintf(stderr, "Thread Create Failed");
      		return 1;
   		}
   	}
   	for(int i = 0; i < CAR_COUNT; i++){ 
      	int t1join = pthread_join(A_Threads[i],NULL); 
     	 if(t1join != 0){
        	fprintf(stderr, "Join Failed");
        	return 1;
      	}
   	}
    sleep(2);
    Car_waitting_dst_B=0;
    Car_waitting_dst_A=0;
    Complete_to_B=0;
    Complete_to_A=0;
    printf("\n\n");}
   	pthread_mutex_destroy(&mutex1);
    }
   	return 0;
}