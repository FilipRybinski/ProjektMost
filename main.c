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
#define FLAG 10 //// flaga debuga ///
#define CLEAR 11 //// flaga cleara ///////
int In_A=0,Que_A=0,In_B=0,Que_B=0; ///// POMOCNICZE ///////////////
int N=10; ////////// ILOSC SAMOCHODOW ///////////////
int debug=0; //// pokazywanie kolejki ////////
int clear=0; /// ladniejsze wypisywanie na konsole.//////
int mode=0; //// 0- bez zmiennych warunkowych /// mode-1 zmienen warunkowe
int ticket=0; /// zmienna ticket dla samochodow////
pthread_mutex_t bridge = PTHREAD_MUTEX_INITIALIZER; /////////////INIT /////////
pthread_cond_t ticket_var=PTHREAD_COND_INITIALIZER; /// INIT ///////
int *carStatus; /// STATUS SAMOCHODU W JAKIM STANIE SIE ZNAJDUJE ///////////////////////
int *carDirection; ////////////////// KIERUNEK W KTORYM JEDZIE SAMOCHOD //////////////////////
/////////////////////////KOLEJKA///////////////////////////////////////////////////
typedef struct List
{
    int nr_samochod;
    struct List *next;
} list;
///////////////////////////////////////////
list* Que_AA=NULL; /// kolejka w miescie A///
list* Que_BB=NULL; /// Kolejka w miescie B///
/////// wypisuje cala kolejke //////
void print_all_que(list *l )
{
    list *temp = l;
    while(temp!=NULL)
    {
        printf("%d ",temp->nr_samochod);
        temp = temp->next;
    }
}
/////dodaje wybrany samochod do kolejki /////
void add_to_list(list **l,int x )
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
/////// usuwa dany samochod z kolejki /////
void delete_from_que(list **l,int x)
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
///// funckja watka ktory nadaje bilet dla samochodow////////
void *Ticket_station(){
    while(1){
        pthread_mutex_lock(&bridge);
        ticket=1;
        pthread_mutex_unlock(&bridge);
        pthread_cond_signal(&ticket_var);
    }
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
    if(carDirection[id-1]!=NONE){ /// JESLI MA WYBRANY KIERUNEK ///
        int Old_status=carStatus[id-1]; /// ZAPISUJE STARY STATUS SAMOCHODU ///
        ////////////SAMOCHOD WJEZDZA NA MOST////////////////////////
        carStatus[id-1]=ON_BRIDGE; /// AKUTALNY STATUS NA MOSCIE
        if(Old_status==IN_QUE_A){
            delete_from_que(&Que_AA,id);  /// USUWA ZE STAREJ KOLEJKI JESLI JEST NA MOSCIE JUZ 
        }else{
            delete_from_que(&Que_BB,id);    /// USUWA ZE STAREJ KOLEJKI JESLI JEST NA MOSCIE JUZ
        }
        ///ZZMIENIA STATUS SAMOCHODU///// 
        if(carDirection[id-1]==CITY_A && carStatus[id-1]==ON_BRIDGE){ // WYPISUJE WSZYSTKIE DANE 

            if(debug==FLAG){
                printf("A-%d ",Count_In_A());printf("[ ");print_all_que(Que_AA);printf("]>>> ");printf("[<< %d <<]",id);printf(" <<<[ ");print_all_que(Que_BB);printf("]");printf(" %d-B--JEDZIE\n",Count_In_B());
            }else{
                printf("A-%d %d>>> [<< %d <<] <<<%d %d-B--JEDZIE\n",Count_In_A(),Count_In_Que_A(),id,Count_In_Que_B(),Count_In_B());
            }

        }
        if(carDirection[id-1]==CITY_B && carStatus[id-1]==ON_BRIDGE){ // WYPISUJE WSZYSTKIE DANE 

            if(debug==FLAG){
            printf("A-%d ",Count_In_A());printf("[ ");print_all_que(Que_AA);printf("]>>> ");printf("[>> %d >>]",id);printf(" <<<[ ");print_all_que(Que_BB);printf("]");printf(" %d-B--JEDZIE\n",Count_In_B());
            }else{
            printf("A-%d %d>>> [>> %d >>] <<<%d %d-B--JEDZIE\n",Count_In_A(),Count_In_Que_A(),id,Count_In_Que_B(),Count_In_B());
            }

        }
        if(clear==CLEAR){printf("\e[1;1H\e[2J");} // CZYSCIE KONSOLE ///
        ////CZAS JAZDY///////
        usleep(1000000);
        ////////////PRZEJEZDZA PRZEZ MOST/////////////////////
        if(Old_status==IN_QUE_A){ // USTAWIA NOWY SATUS SAMOCHODU PO PRZJECHANIU PRZEZ MOST CZYLI TAM GDZIE DOJECHAL
            carStatus[id-1]=CITY_B;
        }else{
            carStatus[id-1]=CITY_A;
        }
        carDirection[id-1]=NONE;
        Old_status=-1; /// STARY STATUS NA -1
        ////////////PRZEJEZDZA PRZEZ MOST I DOJEZDZA DO MIASTA/////////////////////
            if(debug==FLAG){ /// INFORMUJE ZE DOJECHAŁ DO MIASTA /////
            printf("A-%d ",Count_In_A());printf("[ ");print_all_que(Que_AA);printf("]>>> ");printf("[       ]");printf(" <<<[ ");print_all_que(Que_BB);printf("]");printf(" %d-B----DOJECHAL DO MIASTA\n",Count_In_B());
            }else{
            printf("A-%d %d>>> [       ] <<<%d %d-B----DOJECHAL DO MIASTA\n",Count_In_A(),Count_In_Que_A(),Count_In_Que_B(),Count_In_B());
            }
            usleep(1000000);////1s
            if(clear==CLEAR){printf("\e[1;1H\e[2J");}

        }else{

            if(debug==FLAG){ //// SAMOCHOD NIE JEST W KOLEJCE WIEC CZEKA NA PRZEJSCIE DO KOLEJKI W MIESCIA A LUB B ZALEZNIE GDZIE SIE ZNAJDUJE
            printf("A-%d ",Count_In_A());printf("[ ");print_all_que(Que_AA);printf("]>>> ");printf("[       ]");printf(" <<<[ ");print_all_que(Que_BB);printf("]");printf(" %d-B----W MIESCIE ALE NIE UCZESTNICZY W KOLEJCE // CZYLI NIE JEDZIE\n",Count_In_B());
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
    if (pthread_mutex_lock(&bridge) != 0){// lockuje watek ///
        printf("Error");
    }
    if(mode==1){ ///// jesli tryb na zmiennych to czeka na otrzymanie biletu ! ze stacji ///////
        while(ticket==0){
            printf("No ticket to drive\n"); ////// nie ma biletu czeka na bilet
            pthread_cond_wait(&ticket_var, &bridge);
        }
        ticket=0; ///// otrzymal bilet i go używa na pzrejechanie jeśli jest w kolejce ///// jesli nie jest to go traci po prostu ///////
        printf("Now can drive \n"); 
    }
    printer(val); /// wypisuje calosc /////
    if (pthread_mutex_unlock(&bridge) != 0){ ///unlockuje wate /////
        printf("Error");
    }
    //////UNLOCKUJE///////////////////////
    ////////////////CZEKA////////////////////////
    int tmp; //// zmienna pomocnicza ///
    tmp=rand()%500 +1500;
    usleep(tmp* 1000); //// losowy czas czekania /////
    //////////////////JESLI KTORYS Z WATKOW ZNAJDOWAL SIE W MIESCIE ALE NIE UCZESTNICZYL W KOLEJCE TO DODAJE GO DO KOLEJKI I WYBIERA KIERUNEK///////////
        ////////DODAJ DO KOLEJKI SAMOCHOD KTORY NIE BYL W KOLEJCE//////////////////////
        if(carDirection[val-1]==NONE){ ////jesli samochod dojechal do miasta wybranego wczesniej i nie ma nowego kierunku to ustawia B LUB A
            if(carStatus[val-1]==CITY_A){ /// JESLI W MIESCIE A
                carDirection[val-1]=CITY_B; //// TO AKTUALNY KIERUNEK TO B 
                carStatus[val-1]=IN_QUE_A; /// DODAJE ZNOWU DO KOLEJKI
                add_to_list(&Que_AA,val); // DODAJE DO KOLJEKI W A
            }else{
                carDirection[val-1]=CITY_A; /// JESLI W MIESCIE B TO AKTUALNY KIERUNEK A
                carStatus[val-1]=IN_QUE_B; /// DODAJE DO KOLEJKI B
                add_to_list(&Que_BB,val); /// DODAJE DO KOLEJKI B
            }
        }
    }
	pthread_exit(0);
}
int main(int argc, char *argv[]){
    srand((time)NULL); // losowe //
    int x = 0; // zmienna do geopta////
    int extra_thread; /// dodatkowy watek do zmiennej warunkowej ktroy daje bilety dla watkow aby mogly przejechac /////
    while((x = getopt (argc, argv, ":n:dcm"))!=-1) /// wczytuje arguemnty
    {
        switch(x)
        {
        //liczba samochodow
        case 'n':
            N=atoi(optarg); /// ilosc samochodow /////
            break;
        //debug
        case 'd':
            debug=FLAG; // ustawia flage debug aby widziec zawartosc kolejki
            break;
        case 'c':
            clear=CLEAR; /// ladniejsze wyswietlanie////
            break;
        case 'm':
            mode=1; // ustawia mode na zmienne warunkowe
            break;
        }
    }
    if(argc<2){
        printf("Blad przy czytaniu argumentow z linii polecen\n");
        return 1;
    }
	pthread_t A_Threads[N]; /// ILOSC SAMOCHODOW /////
    pthread_t Ticket_Station_Thread; /// watek do zmiennej warunkowej ktory daje bilety samochodom //////
    carStatus=(int*)malloc(N * sizeof(int));  /// malloc dla statusow wszystkich samochodow ///
    if (carStatus == NULL) { 
        printf("Nie udalo sie zaallokowac carsState\n"); 
        return EXIT_FAILURE; 
    }
    carDirection=(int*)malloc(N * sizeof(int)); /// malloc dla  kierunku samochodow ////
     if (carDirection == NULL) { 
        printf("Nie udalo sie zaallokowac carsState\n"); 
        return EXIT_FAILURE; 
    }
    In_A=rand()%N; /// losowa ilosc samochodow w miescie A ///
    Que_A=N-In_A; //// losowa ilosc samochodow w kolejce do miasta B--- czyli kolejka w miescia A ////
    for(int i=0;i<In_A;i++){ ////// nadaje statusy do tablicy i kierunki samochodow ////
        carStatus[i]=CITY_A;
        carDirection[i]=NONE;
    }
    for(int i=In_A;i<N;i++){ ////// nadaje statusy do tablicy i kierunki samochodow ////
        carStatus[i]=IN_QUE_A;
        carDirection[i]=CITY_B;
        add_to_list(&Que_AA,i+1); /// dodaje do kolejki w miescie A //////
    }
    printf("KOLEJKI I POCZATEK PROGRAMU\n"); //// Wypisuje stan poczatkowy wylosowanych samochodow. Ktore sa w kolejce a ktore w miescie po prostu///////
    printf("A-%d [ %d ] >> [     ] << [ %d ] %d-B\n",Count_In_A(),Que_A,In_B,Que_B);
    printf("A-%d ",Count_In_A());printf("[ ");print_all_que(Que_AA);printf("] ");printf(">> [     ] << ");printf("[ ");print_all_que(Que_BB);printf("] ");printf("%d-B\n",Count_In_B());
    usleep(1000000); ///1s
    if(clear==CLEAR){printf("\e[1;1H\e[2J");}//// czysci konsole jesli tryb clear ////
    if(mode==1){
        extra_thread=pthread_create(&Ticket_Station_Thread,NULL,Ticket_station,NULL); /// tworzy watek do nadawania biletow /////
        if(extra_thread!=0){
            printf("Blad tworzenia pthread (create)");
        }
    }
	for(int i = 0; i < N; i++){ /// tworzy watki dla samochodow //////
        int* id_car = (int*)malloc(sizeof(int*));
        *id_car = i + 1;
    	if(pthread_create(&A_Threads[i], NULL, Drive, id_car)!=0)
        {perror("Create error:  ");}
   	}

   	for(int i = 0; i < N; i++){ /// join dla samochodow /////
      	int t1join = pthread_join(A_Threads[i],NULL); 
     	 if(t1join != 0){
        	fprintf(stderr, "Join Failed");
        	return EXIT_FAILURE;
      	}
   	}
    if(pthread_join( Ticket_Station_Thread, NULL)!=0){
        printf("Blad przy dolaczaniu do watkow (join)\n");
            return EXIT_FAILURE;
    } /// join dla stacji z biletami ////
    ///zwolnienie pamieci ////
    free(carStatus); 
    free(carDirection);
   	return EXIT_SUCCESS;
}