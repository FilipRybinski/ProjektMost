## Wąski most
## Poprawne użycie programu:

 (1) ```Składnia programu (bez debuga, bez zmiennych warunkowych,bez clear)= ./program -n [Ilosc samochodow]```
 
 (2) ```Składnia programu (bez zmiennych warunkowych,bez clear)= ./program -n [Ilosc samochodow] -d```
 
 (3) ``` Składnia programu (bez zmiennych warunkowych)= ./program -n [Ilosc samochodow] -d -c```
 
 (4) ``` Składnia programu = ./program -n [Ilosc samochodow] -d -c -m```
 
### Opis opcji:
 ```-n``` : Ilość przejeżdżających samochodów // ilosc wątków

 ```-d``` : wyświetlanie zawartości kolejek
 
 ``` -m``` : działanie na zmiennych warunkowych / bez biletu samochod nie przejedzie ani nie zmieni swojego stanu

 ```-c``` : clear czyści konsole po kazdym komunikacie / trochę ładniej wygląda :)

### Proste uzycie:
* ```./program -n 10 - podstawowe działanie```

## INFO
Program działający bez zmiennych warunkowych tworzy n watków które przejeżdżają z miasta do miasta, po dojechaniu ich do nowego miasta watek odczekuje chwile po czym przechodzi znowu do kolejki i zmienia kierunek w ktorym chce jechać na nowy.
Np. Jechał z miasta A do B dojeżdża chwile jest w miescie B po czym przchodzi do kolejki w mieście B i ustawia swoj kierunek na Miasto A
## AUTORZY
Projekt wykonali Filip Rybiński i Dominik Gudalewski
