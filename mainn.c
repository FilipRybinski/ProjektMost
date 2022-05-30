#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#define true 0
#define false 1
int FLAG=false;
int A = 1; // CAR_A= THREAD
int B = 1; // CAR_B =THREAD
int Car_waitting_for_turn_A;
int Car_waitting_for_turn_B;
int From_A_get_to_B=0;
int From_B_get_to_A=0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
void Road_info(int i){
    if(i==1 /*A*/ ){
        printf("A-%d %d>>> [>> %d >>] <<<%d %d-B\n",From_B_get_to_A,Car_waitting_for_turn_A,A,Car_waitting_for_turn_B,From_A_get_to_B);
    }else{ /* B */
        printf("A-%d %d>>> [<< %d <<] <<<%d %d-B\n",From_B_get_to_A,Car_waitting_for_turn_A,B,Car_waitting_for_turn_B,From_A_get_to_B);
    }
}
void Bridge(char arr[], int value){
	srand(time(NULL));
   	//printf("Car #%d from %s is traveling on the bridge...\n",value,arr);//
   	int randomnumber = rand() % 4; // Gets Random Number From 0 to 3
   	sleep(randomnumber); // Sleep For up to 3 Seconds
}
void *Village_A(void* arg){ 
	pthread_mutex_lock(&mutex1); 
	char *name = "Village_A";
	//printf("Car #%d from %s can cross the bridge\n", A, name);//
	Bridge(name,A);
    Car_waitting_for_turn_A--;
    if(FLAG==true)Road_info(1);
	//printf("Car #%d from %s has left the bridge\n", A, name);//
	A++;
    From_A_get_to_B++;
	pthread_mutex_unlock(&mutex1); 
	pthread_exit(0);
}

void *Village_B(void* arg){
	pthread_mutex_lock(&mutex1);
	char *name = "Village_B";
	//printf("Car #%d from %s can cross the bridge\n", B, arr);//
	Bridge(name,B);
    Car_waitting_for_turn_B--;
    if(FLAG==true){Road_info(2);}
	//printf("Car #%d from %s has left the bridge\n", B, arr);//
	B++;
    From_B_get_to_A++;
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
    int CAR_COUNT =atoi(argv[1]);
    Car_waitting_for_turn_A=CAR_COUNT;
    Car_waitting_for_turn_B=CAR_COUNT;
	pthread_t A_Threads[CAR_COUNT]; // CAR_FROM_VILLAGE_A
	pthread_t B_Threads[CAR_COUNT]; // CAR_FROM_VILLAGE_B 
	pthread_mutex_init(&mutex1,NULL);
	for(int i = 0; i < CAR_COUNT; i++){
    	int t1 =  pthread_create(&A_Threads[i], NULL, Village_A, NULL); 
    	int t2 = pthread_create(&B_Threads[i], NULL, Village_B, NULL); 
   		if(t1 != 0 || t2 != 0){ 
      		fprintf(stderr, "Thread Create Failed");
      		return 1;
   		}
   	}

   	for(int i = 0; i < CAR_COUNT; i++){ 
      	int t1join = pthread_join(A_Threads[i],NULL); 
    	int t2join = pthread_join(B_Threads[i],NULL);
     	 if(t1join != 0 || t2join != 0){
        	fprintf(stderr, "Join Failed");
        	return 1;
      	}
   	}
   	pthread_mutex_destroy(&mutex1);
    }
   	return 0;
}