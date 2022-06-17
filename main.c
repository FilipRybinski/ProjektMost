#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#define CITY_A 0 //// W MIESCIE A LUB JEDZIE W KEIRNKU MIASTA A ////////////////
#define CITY_B 1 //// W MIESCIE B LUB JEDZIE W KIERUNKU B /////
#define NONE 2 /// BRAK KIERUNKU /// 
#define ON_BRIDGE 3 //// NA MOSCIE //////////////
#define IN_QUE_A 4 //// W KOLEJCE DO MIASTA B ///
#define IN_QUE_B 5 /// W KOLEJCE DO MIASTA A ////
#define FLAG 10
#define CLEAR 11
int In_A=0,Que_A=0,In_B=0,Que_B=0; ///// POMOCNICZE ///////////////
int N=10; ////////// ILOSC SAMOCHODOW ///////////////
int debug=0;
int clear=0;
pthread_mutex_t bridge = PTHREAD_MUTEX_INITIALIZER; /////////////INIT /////////
int *carStatus; /// STATUS SAMOCHODU W JAKIM STANIE SIE ZNAJDUJE ///////////////////////
int *carDirection; ////////////////// KIERUNEK W KTORYM JEDZIE SAMOCHOD //////////////////////
/////////////////////////KOLEJKA///////////////////////////////////////////////////
typedef struct List
{
    int nr_samochod;
    struct List *next;
} list;
list* Que_AA=NULL;
list* Que_BB=NULL;
void wypisz_lista(list *l )
{
    list *temp = l;
    while(temp!=NULL)
    {
        printf("%d ",temp->nr_samochod);
        temp = temp->next;
    }
}
void umiesc_w_liscie(list **l,int x )
{
    list * nowy= malloc(sizeof(list));
    nowy->nr_samochod = x;
    nowy->next = NULL;
    if(*l==NULL)
    {
        *l = nowy;
    }
    else
    {
        list * temp= *l;
        while(temp->next!=NULL)
        {
            temp=temp->next;
        }

        temp->next=nowy;
        return;
    }

}

void usun_z_listy(list **l,int x)
{
    if(*l==NULL)
    {
        return;
    }
    if((*l)->nr_samochod==x)
    {
        list * temp = (*l);
        list * temp2 = (*l)->next;
        free(temp);
        *l=temp2;
        return;


    }

    list * przed = *l;
    list * temp = *l;
    list * po = *l;
    while (temp->next != NULL)
    {
        if(temp->nr_samochod==x)
            break;
        przed=temp;
        temp = temp->next;
    }
    po=temp->next;
    free(temp);
    przed->next=po;
}
void wypisz_kolejki()
{
    printf("Kolejka A:");
    wypisz_lista(Que_AA);
    printf("Kolejka B:");
    wypisz_lista(Que_BB);

}
///////////////OBLICZA ILOSC SAMOCHODOW  W MIESCIE A////////////////////////////
int Count_In_A(){
    int count=0;
    for(int i=0;i<N;i++){
        if(carStatus[i]==CITY_A){ count++;}
    }
    return count;
}
///////////////OBLICZA ILOSC SAMOCHODOW  W MIESCIE B////////////////////////////
int Count_In_B(){
    int count=0;
    for(int i=0;i<N;i++){
        if(carStatus[i]==CITY_B){ count++;}
    }
    return count;
}
///////////////OBLICZA ILOSC SAMOCHODOW W KOLEJCE W MIESCIE A////////////////////////////
int Count_In_Que_A(){
    int count=0;
    for(int i=0;i<N;i++){
        if(carStatus[i]==IN_QUE_A){ count++;}
    }
    return count;
}
///////////////OBLICZA ILOSC SAMOCHODOW W KOLEJCE W MIESCIE B////////////////////////////
int Count_In_Que_B(){
    int count=0;
    for(int i=0;i<N;i++){
        if(carStatus[i]==IN_QUE_B){ count++;}
    }
    return count;
}
////////////////////////////////////////
void printer(int id ){
    if(carDirection[id-1]!=NONE){
        int Old_status=carStatus[id-1];
        ////////////SAMOCHOD WJEZDZA NA MOST////////////////////////
        carStatus[id-1]=ON_BRIDGE;
        if(Old_status==IN_QUE_A){
            usun_z_listy(&Que_AA,id);
        }else{
            usun_z_listy(&Que_BB,id);
        }
        ///ZZMIENIA STATUS SAMOCHODU///// 
        if(carDirection[id-1]==CITY_A && carStatus[id-1]==ON_BRIDGE){

            if(debug==FLAG){
                printf("A-%d ",Count_In_A());printf("[ ");wypisz_lista(Que_AA);printf("]>>> ");printf("[<< %d <<]",id);printf(" <<<[ ");wypisz_lista(Que_BB);printf("]");printf(" %d-B--JEDZIE\n",Count_In_B());
            }else{
                printf("A-%d %d>>> [<< %d <<] <<<%d %d-B--JEDZIE\n",Count_In_A(),Count_In_Que_A(),id,Count_In_Que_B(),Count_In_B());
            }

        }
        if(carDirection[id-1]==CITY_B && carStatus[id-1]==ON_BRIDGE){

            if(debug==FLAG){
            printf("A-%d ",Count_In_A());printf("[ ");wypisz_lista(Que_AA);printf("]>>> ");printf("[>> %d >>]",id);printf(" <<<[ ");wypisz_lista(Que_BB);printf("]");printf(" %d-B--JEDZIE\n",Count_In_B());
            }else{
            printf("A-%d %d>>> [>> %d >>] <<<%d %d-B--JEDZIE\n",Count_In_A(),Count_In_Que_A(),id,Count_In_Que_B(),Count_In_B());
            }

        }
        if(clear==CLEAR){printf("\e[1;1H\e[2J");}
        ////CZAS JAZDY///////
        usleep(1000000);
        ////////////PRZEJEZDZA PRZEZ MOST/////////////////////
        if(Old_status==IN_QUE_A){
            carStatus[id-1]=CITY_B;
        }else{
            carStatus[id-1]=CITY_A;
        }
        carDirection[id-1]=NONE;
        Old_status=-1;
        ////////////PRZEJEZDZA PRZEZ MOST I DOJEZDZA DO MIASTA/////////////////////
            if(debug==FLAG){
            printf("A-%d ",Count_In_A());printf("[ ");wypisz_lista(Que_AA);printf("]>>> ");printf("[       ]");printf(" <<<[ ");wypisz_lista(Que_BB);printf("]");printf(" %d-B----DOJECHAL DO MIASTA\n",Count_In_B());
            }else{
            printf("A-%d %d>>> [       ] <<<%d %d-B----DOJECHAL DO MIASTA\n",Count_In_A(),Count_In_Que_A(),Count_In_Que_B(),Count_In_B());
            }
            usleep(1000000);////1s
            if(clear==CLEAR){printf("\e[1;1H\e[2J");}

        }else{

            if(debug==FLAG){
            printf("A-%d ",Count_In_A());printf("[ ");wypisz_lista(Que_AA);printf("]>>> ");printf("[       ]");printf(" <<<[ ");wypisz_lista(Que_BB);printf("]");printf(" %d-B----W MIESCIE ALE NIE UCZESTNICZY W KOLEJCE // CZYLI NIE JEDZIE\n",Count_In_B());
            }else{
            printf("A-%d %d>>> [       ] <<<%d %d-B----W MIESCIE ALE NIE UCZESTNICZY W KOLEJCE // CZYLI NIE JEDZIE\n",Count_In_A(),Count_In_Que_A(),Count_In_Que_B(),Count_In_B());    
            }
            usleep(1000000); ///1s
            if(clear==CLEAR){printf("\e[1;1H\e[2J");}

        }
}
void *Drive(void* arg){ 
    int val= *((int*)arg);//////////////////////ID SAMOCHODU//////////////////////
    while(1){
    //////LOCKUJE /////
	pthread_mutex_lock(&bridge); 
    printer(val);
	pthread_mutex_unlock(&bridge);
    //////UNLOCKUJE///////////////////////
    ////////////////CZEKA////////////////////////
    int tmp;
    tmp=rand()%500 +1500;
    usleep(tmp* 1000);
    //////////////////JESLI KTORYS Z WATKOW ZNAJDOWAL SIE W MIESCIE ALE NIE UCZESTNICZYL W KOLEJCE TO DODAJE GO DO KOLEJKI I WYBIERA KIERUNEK///////////
        ////////DODAJ DO KOLEJKI SAMOCHOD KTORY NIE BYL W KOLEJCE//////////////////////
        if(carDirection[val-1]==NONE){
            if(carStatus[val-1]==CITY_A){
                carDirection[val-1]=CITY_B;
                carStatus[val-1]=IN_QUE_A;
                umiesc_w_liscie(&Que_AA,val);
            }else{
                carDirection[val-1]=CITY_A;
                carStatus[val-1]=IN_QUE_B;
                umiesc_w_liscie(&Que_BB,val);
            }
        }
    }
	pthread_exit(0);
}
int main(int argc, char *argv[]){
    srand((time)NULL);
    int x = 0;
    while((x = getopt (argc, argv, ":n:dc"))!=-1)
    {
        switch(x)
        {
        //liczba samochodow
        case 'n':
            N=atoi(optarg);
            break;
        //debug
        case 'd':
            debug=FLAG;
            break;
        case 'c':
            clear=CLEAR;
            break;
       
        }
    }
    if(argc<2){
        printf("Blad przy czytaniu argumentow z linii polecen\n");
        return 1;
    }
	pthread_t A_Threads[N];
	pthread_mutex_init(&bridge,NULL);
    carStatus=(int*)malloc(N * sizeof(int));
    carDirection=(int*)malloc(N * sizeof(int));
    In_A=rand()%N;
    Que_A=N-In_A;
    for(int i=0;i<In_A;i++){
        carStatus[i]=CITY_A;
        carDirection[i]=NONE;
    }
    for(int i=In_A;i<N;i++){
        carStatus[i]=IN_QUE_A;
        carDirection[i]=CITY_B;
        umiesc_w_liscie(&Que_AA,i+1);
    }
    printf("KOLEJKI I POCZATEK PROGRAMU\n");
    printf("A-%d [ %d ] >> [     ] << [ %d ] %d-B\n",Count_In_A(),Que_A,In_B,Que_B);
    printf("A-%d ",Count_In_A());printf("[ ");wypisz_lista(Que_AA);printf("] ");printf(">> [     ] << ");printf("[ ");wypisz_lista(Que_BB);printf("] ");printf("%d-B\n",Count_In_B());
    
    usleep(1000000); ///1s
    if(clear==CLEAR){printf("\e[1;1H\e[2J");}
	for(int i = 0; i < N; i++){
        int* id_car = (int*)malloc(sizeof(int*));
        *id_car = i + 1;
    	if(pthread_create(&A_Threads[i], NULL, Drive, id_car)!=0)
        {perror("Create error:  ");}
   	}

   	for(int i = 0; i < N; i++){ 
      	int t1join = pthread_join(A_Threads[i],NULL); 
     	 if(t1join != 0){
        	fprintf(stderr, "Join Failed");
        	return 1;
      	}
   	}
   	return 0;
}