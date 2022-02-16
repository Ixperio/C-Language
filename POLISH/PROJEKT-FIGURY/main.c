//Program zaprojektowal Ixperio (Artur) 

#include <stdio.h>
#include <stdlib.h>
#define N 32 //maksymalna liczba znakow kategorii
#define M 5 //maksymalna liczba znakow kodu produktu
#define L 128 //maksymalna dlugosc znakow pliku do wczytania

typedef struct element_podlisty{        /*Elementy dane w tresci zadania , tresc znajduje sie w plikach na git*/
char kod[M];
long stan;
struct element_podlisty *pod_next;
} Element;

typedef struct element_listy_glownej{
char kategoria[N];
struct element_listy_glownej *next;
struct element_podlisty *pod_next;
} GElement;
                                       //---------------------------------
typedef GElement *ListaG; //definicja wskaznika na nowa liste ListaG - lista glowna (zawierajaca kategorie i wskaznik na pierwszy element liksty produktowej)
typedef Element *PodLista; //definicja wskaznika na Podliste (zawierajaca produkty)

void obramuj(char *informacja){ //funkcja zwiekszajaca czytelnosc w programie

    int liczba_znakow = strlen(informacja), i;  //definiuje dlugosc
    printf("\n\t"); //wypisuje tabluacje, aby przeniesc informacje blizej srodka ekranu
    for(i=0; i<=liczba_znakow+3; i++){ //stawia kreski o liczbie dlugosci znakow dodaje 3 aby ramka byla dluzsza od tekstu
        printf("-");
    }
    printf("\n\t");//wypisuje tabluacje
    printf("| %s |", informacja);  //stawia pionowe kreski na przodzie i koncu informacji, pomiedzy wypisujac ja
    printf("\n\t");//wypisuje tabluacje
    for(i=0; i<=liczba_znakow+3; i++){ //ponawia procedure z poczatku funkcji
        printf("-");
    }
    printf("\n\t");//wypisuje tabluacje

}

PodLista lista_Produkty(char *lista, char *kategoria){ //zadaniem tej listy jest dodawanie produktow do list kategorii

    PodLista pomoc, wierzcholek = NULL; //definiuje podliste pomoc i ustala jej wierzcholek na null

        FILE *plik = fopen(lista, "r"); //otwiera plik podany do funkcji
                        //deklaracje zmiennych lokalnych
        char *kat[N];
        char *kod[M];
        long int ilosc;  //ilosc produktu

        if(plik == NULL){
            obramuj("Nie odnaleziono pliku z danymi, upewnij sie czy plik nadal istnieje.");
            return NULL;
        }else{
            int p=0;//zmienna pomocnicza powoduje, ze pierwszy element zostanie zaimportowany
            while((fscanf(plik,"%s %s %ld",kat ,kod, &ilosc)) != EOF){ //petla wybiera kolejne wartosci z pliku

                    if(strcmp(kategoria, kat)==0){ // jezeli kategoria wprowadzona do funkcji jest zgodna z kategoria wczytana z pliku wartosc logiczna jest prawda
                           // sprawdz_czy_zawProd()
                         if((p==0) || ((sprawdz_czy_zawProd(kod, wierzcholek, ilosc))==0)){ /*jezeli jest to pierwszy element listy if jest spelniony, w innych przypadkach
                                                                                    wykonywana jest funkcja sprawdz_czy_zawiera, ktora sprawdza czy dany produkt jest juz na liscie
                                                                                    jezeli jest to nie jest dodawany nowy produkt*/
                            pomoc = malloc(sizeof(Element)); //towrzy miejsce dla listy o wielkosci rownej strukturze Element
                            strcpy(pomoc->kod, kod); //kopiuje kod produktu z pliku tekstowego i wpisuje do listy
                            pomoc->stan = ilosc; //ustawia stan elementu w liscie na ilosc w pliku tekstowym
                            pomoc->pod_next = wierzcholek; //przypisuje wartosc next na wierzcholek
                            wierzcholek = pomoc; //ustawia wartosc wierzcholka na pomoc
                            p++; //zwieksza licznik p

                        }

                    }
            }

        }


    return wierzcholek; //zwraca nam wierzcholek na liste produktow
    fclose(plik); //i pamieta o zamknieciu pliku :)
}

sprawdz_czy_zawProd(char *kod, PodLista wierzcholek, long int stan){ 
        //funkcja pobiera kod, wierzcholek na podliste i stan, jest wykonywana aby sprawdzic czy produkt nie pojawil sie juz na liscie
    int x=0; //parametr kontrolujacy
    while(wierzcholek != NULL){  //wczytuje liste do momentu kiedy napotka null na wierzcholku, dlatego w funkcji lista produkty zawarty jest parametr "p"
            if(strcmp(wierzcholek->kod, kod)==0){ //jezeli znajdzie wierzcholek tej samej nazwy war. spelniony
                    wierzcholek->stan += stan; //wtedy stan magazynowy zostaje zwiekszony
                    x=1; //parametr kontorlujacy ustawiony na 1 aby wykluczyc ponowne dodanie tego prduktu do listy.
                break; //przerywa i zwraca x, poniewaz kazdy produkt moze byc jedynie raz w bazie dlatego nie ma sensu sprawdzac dalej
            }
        wierzcholek = wierzcholek->pod_next; //ustawia wierzcholek na kolejny element w liscie produktow
    }

return x; //zwraca parametr kontrolujacy

}

ListaG Czytaj_z_pliku(char *nazwa_plik){ //funkcja czyta z pliku, zadaniem tej funkcji jest pogrupowac listy na kategorie

    FILE *plik = fopen(nazwa_plik, "r");

    ListaG pomoc, wierzcholekG = NULL; //deklaruje liste glowna pomoc i jej wierzcholek
    PodLista wierzcholekPodLista; //deklaruje wierzcholek na podliste;
                            //zmienne lokalne
    long int stan, p=0; //p jako licznik
    char *kategoria[N];
    char *kod[M];

    if(plik == NULL){ //jezeli nie udalo sie pobrac pliku

        printf("\n\tNie odnaleziono pliku: ( %s ) , upewnij sie ze plik o takiej nazwie istnieje.", nazwa_plik); //zwraca blad braku mozliwosci wczytania pliku
        return NULL;
    }else{
            while((fscanf(plik, "%s %s %ld", kategoria, kod, &stan)) != EOF){ //petla pobiera kolejne wartosci z pliku i przypisuje je do zmiennych
                    if((p==0) || (sprawdz_czy_zaw(kategoria, wierzcholekG))==0){ //0 nie zawiera | 1 zawiera , p aby pierwszy element mogl zostac dodany

                        pomoc = malloc(sizeof(GElement)); //alokuje pamiec wielkosci struktury GElement
                        strcpy(pomoc->kategoria, kategoria); //kopiuje wartosc zaminnej kategoria do pomoc->kategoria, jako ze jest to zmienna znakowa
                        pomoc->next = wierzcholekG; //ustawia wskaznik next na wierzcholek


                        if((wierzcholekPodLista = lista_Produkty(nazwa_plik, kategoria)) == NULL){ //w tym miejscu wykonywana jest funkcja lista_produkty,
                                                                                                    //ktora zwraca wierzcholek na liste produktow jezeli lista jet pusta to zwraca null
                            wierzcholekPodLista = NULL; //ustawia wierzcholek podlisty na null jezeli prawda
                        }
                        pomoc->pod_next = wierzcholekPodLista; //ustawia wskaznik next na wskaznik podlistyproduktow

                        wierzcholekG = pomoc; //ustawia nowa wartosc wierzcholka listy kategorii na pomoc

                        p++;//zwieksza licznik
                    }
            }
    }
    obramuj("Dane zostaly wczytane poprawnie"); //wyswietla komunikat
    return wierzcholekG; //zwraca najwazniejszy w programie wierzcholek na liste produktow
    fclose(plik); //zamyka plik

}

sprawdz_czy_zaw(char *kategoria, ListaG wierzcholek){  //wyklucza dodanie do listy glownej wierzcholka o tej samej nazwie
    int x=0; //zmienna kontrolna
    while(wierzcholek != NULL){
            if(strcmp(wierzcholek->kategoria, kategoria)==0){ //jezeli znajdzie wierzcholek tej samej nazwy war. spelniony
                    x=1;//zmienna kontrolna ustawiona na 1 aby zaznaczyc ze takowa kategoira znajduje sie juz na liscie kategorii
                break; //przerywa i zwraca x, gdyz kategoria moze pojawic sie jedynie raz na liscie wiec jak juz zostala odnaleziona to bez sensu jest dalej przeszukiwac
            }
        wierzcholek = wierzcholek->next; //zmienia wierzcholek na kolejny
    }

    return x; //zwraca zmienna kontorlna
}

void wyswietl_liste(ListaG wierzcholek){  //funkcja wypisujaca liste na standardowe wyjscie
    PodLista wierzcholekP; //definiuje wierzcholek na podliste
     printf("\n-----------------------\n\n");  //tworzy linnie ulatwiajaca wglad do danych
    while(wierzcholek != NULL){ //konczy petle jezeli wierzcholek listy kategorii jest ostatnim czyli tym o wartosci null
        printf(" %s :\n", wierzcholek->kategoria); //wypisuje kolejne elementy listy
        wierzcholekP = wierzcholek->pod_next; //zmienia wierzcholek na kolejny
        if(wierzcholekP == NULL){ //jezeli wierzcholek listy produktowej jest null'em to wyswietla komunikat informacyjny brak produktow
            printf("  |- Brak produktow!\n\n");
        }else{ //w przeciwnym wypadku
             while(wierzcholekP != NULL){  //przeszukuje liste produktow dopuki nie napotka wierzcholka o wartosci null
             printf("  |- %s %ld \n", wierzcholekP->kod, wierzcholekP->stan); //wypisuje kolejne elementy listy
             wierzcholekP = wierzcholekP->pod_next; //zmienia wartosc wierzcholka listy produktow na kolejny
            }
            printf("\n");//po zakonczeniu listy produktow robi odstemp w celu lepszej przejrzystosci
        }

        wierzcholek = wierzcholek->next; //zmienia wartosc 'wskaznika' na kolejny w liscie kategorii
    }
 printf("-----------------------\n\n"); //konczac wypisywac obie listy tworzy jak na poczatku linnie oddzielajaca menu od wyswietlonych danych

}

void Pomnijesz_wartosc(ListaG magazyn, char *kategoria, char *kod, long int ilosc_zmiany){ //funkcja pomniejszajaca wartosc wybranych produktow w programie
        if(ilosc_zmiany < 0){ //sprawdza czy przypadkiem nie chcemy odjac wartosci ujemnej
            obramuj("Ilosc zmiany nie moze byc ujemna!");
            obramuj("Operacja przerwana!");

        }else{

     PodLista produkty; //definiuje podliste, wierzcholek na liste glowna zostal przekazany do funkcji
     int poprawnosc = 0;//parametr kontrolujaca

     while(magazyn != NULL){ //jezeli wskaznik listy kategorii jest null
        if(strcmp(magazyn->kategoria, kategoria)==0){ //sprawdza czy kategoria wprowadzona do funkcji znajduje sie na liscie kategorii 0- tak | 1 - nie

                produkty = magazyn->pod_next; //pobiera wartosc wskaznika na liste produktow w liscie kategorii

                while(produkty != NULL){ //tworzy petle dopoki wartosc wskaznika produktow rozna od null

                    if(strcmp(produkty->kod,kod)==0){ //jezeli wartosc kodu jest zgodna z kodem przekazanym funkcji
                            poprawnosc = 1; //wtedy parametr kontrolujacy ustawiany na 1
                            produkty->stan -= ilosc_zmiany; //stan tego prduktu jest obnizany o wartosc zadana do funkcji
                             obramuj("Wartosc produktu zostala zaktualizowana!"); //wypisany zostaje komunikat
                            if(produkty->stan < 0){ // jezeli stan przekroczy 0 i przejdzie na wartosc ujemna
                                produkty->stan = 0; //zostaje ponownie ustawiony na 0
                                obramuj("Stan magazynowy produktu przekroczylby minimalna wartosc, zatem jego ilosc ustawiono na 0"); /*zostaje wypisany
                                stosowny komunikat, zgodnie z warunkami polecenia */
                            }

                        break; //jezeli produkt zostal odnaleziony, petla zostaje przerwana, gdyz w liscie nie moze byc dwoch takich samych produktow
                    }
                    produkty = produkty->pod_next; //wskaznik produktow zostaje zmieniony na następny
                }

            break; //rowniez jezeli kategoria zostala znaleziona i petla produktow zostal skonczona to petla magazyn rownize musi zostac przerwana
        }
        magazyn = magazyn->next; //wskaznik na kolejna kategorie proguktow
     }
     if(poprawnosc == 0){ //komunikat o bledzie
        obramuj("Nie odnaleziono podanego produktu w bazie");
     }
        }
}

void wypisz_do_pliku(ListaG wierzcholek, char *nazwa_pliku, char *nazwa_pliku_w){ //wypisuje dane z programu po edycji do zdefiniowanego pliku zachowujac wejsciowy format
    int i=0;//zmienna lokalna do petli definiuje dlugosc pierwszego czlonu tekstu

    for(i;nazwa_pliku_w[i] != '.'; i++);//sprawdza dlugosc pierwszego czlonu wprowadzonej nazwy pierwszego pliku z ktorego pobrano dane

    int j = i, k =0; //dodaje kolejne parametry
    for(j;nazwa_pliku_w[j] != 0; j++){ //ta petla sprawdza ile wynosi dlugosc formatu jezeli nazwa_pliku_w = 0 oznacza to jej koniec
        k++;
    }

    char format[k]; //nastepnie jest definiowany format jako zmienna znakowa

    k=0; //ponowne uzycie k
    for(j=i;nazwa_pliku_w[j] != 0; j++){//ta petla sprawdza ile wynosi dlugosc formatu jezeli nazwa_pliku_w = 0 oznacza to jej koniec
        format[k] = nazwa_pliku_w[j]; //do zmiennej format zapisywany jest czlon formatu razem z kropka
        k++; //k staje sie licznikiem dla formatu
    }

    int y=0, licznik=0;

    for(y; nazwa_pliku[y] != 0; y++){ //do zmiennej licznik wprowadzona zostaje wartosc dlugosci pliku nazwa_pliku
        licznik++;
    }

     char *nowa_nazwa = malloc(sizeof(nazwa_pliku)+sizeof(format)); //zostaje zdefiniowana przestrzen dla nowej nazwy dlatego zostaly policzone dlugosci formatu i nazwy pliku i zostaly zsumowane

    strcpy(nowa_nazwa, nazwa_pliku); //do nowej nazwy zostaje skopiowany tekst nazwa_pliku
    strcat(nowa_nazwa, format); //oraz do nowej_nazwy zostaje 'dopiety' fortmat pliku jaki zostal wczytany

    FILE *plik = fopen(nowa_nazwa, "w"); //nastempnie zostaje otwarty strumien do zapisu pliku na dysk

    PodLista wierzcholekP; //definiuje wierzcholek na podliste, wierzcholek na liste glowna zostal zadany w funkcji


     while(wierzcholek != NULL){ //konczy petle jezeli wierzcholek jest ostatnim czyli tym o wartosci null
        fprintf(plik," %s :\n", wierzcholek->kategoria); //wypisuje kolejne elementy listy
        wierzcholekP = wierzcholek->pod_next; // ustawia wierzcholek podlisty na wierzcholek podlisty 'wyciagniety' z listy kategorii
        if(wierzcholekP == NULL){ //jezeli wierzcholek produtkow na starcie rowny jest null
            fprintf(plik,"  |- Brak produktow!\n"); //wypisuje do pliku informacje
        }else{ //w przeciwnym wypadku
             while(wierzcholekP != NULL){ //jezeli wierzcholek podlisty jest rozny od null'a
             fprintf(plik,"  |- %s %ld \n", wierzcholekP->kod, wierzcholekP->stan); //wypisuje kolejne elementy listy do pliku
             wierzcholekP = wierzcholekP->pod_next; //wierzcholek podlisty produktow zostaje zastapiony przez nastepny
            }
        }

        wierzcholek = wierzcholek->next; //zmienia wartosc 'wskaznika' na kolejny
    }

    fclose(plik); //zamyka plik

    printf("\n\n\tGotowe utworzono plik (%s)! \n", nowa_nazwa); //wypiisuje komunikat

}

void usunElement(ListaG wierzcholekG, char *kat, char *kod){ //funkcja usuwajaca element z listy produktow

     PodLista produkt, produktWczes=NULL, produktNast=NULL; //definiuje wskaznik na produkt wczesniejszy, kolejny oraz obecny produkt
     int poprawnosc = 0; //parametr kontrolny

     while(wierzcholekG != NULL){ //przeszukuje liste kategorii zadana w funkcji

        if(strcmp(wierzcholekG->kategoria, kat)==0){ //jezeli kategoria znaleziona w liscie pokrywa sie z nazwa kategorii przekazana do funkcji

                produkt = wierzcholekG->pod_next; //wierzcholek obecnego produktu zostaje zapisany jako wskaznik z listy kategorii

                while(produkt != NULL){ //petla dopuki wskaznik rozny od null, jezeli bedzie null to kolejny tez jest null

                    if(strcmp(produkt->kod,kod)==0){ //jezeli natrafiono na kod produktu zadany do funkcji

                            if(produktWczes == NULL){ //jezeli produkt wczesniejszy nadal ma wartosc null tyczy sie to produktu pierwszego na liscie produktow
                                wierzcholekG->pod_next = produkt->pod_next; //wtedy wykonuje usuniecie tego elementu przypisujac do listy glownej wierzcholek na 2-gi element listy produktow
                            }else{
                                produktWczes->pod_next = produkt->pod_next; /*w przeciwnym wypadku istnieje produkt wczesniejszy zatem to jego wskaznik nalezy zamienic na wskaznik kolejnego
                                                                            produktu z listy produktow */
                            }
                                obramuj("Produkt usuniety z bazy!"); //wyswietla komunikat
                            poprawnosc = 1;//ustawia parametr kontrolny
                            break; //przerywa gdyz funkcja wykonuje jedynie jedno usuniecie na jedno wywolanie
                    }else{ //jezeli nie natrafiono na produkt
                         produktWczes = produkt; //wskaznik produktu ktory byl wczesniejszy, czyli ten co wlasnie byl sprawdzany
                         produkt = produkt->pod_next; // wskaznik produktu zostaje zmieniony na kolejny produkt
                    }
                }

            break; //jezeli nie znaleziono w kategorii zadnego produktu to przerywa, bo juz nie znajdzie kolejnej tej samej aktegorii na liscie
        }

        wierzcholekG = wierzcholekG->next; //ustawia wierzcholek na kolejny element w liscie kategroii
     }

     if(poprawnosc == 0){ //jezeli nie znaleziono wypisuje komunikat
        obramuj("Nie odnaleziono podanego produktu w bazie");
     }

}

int main()
{

        ListaG wierzcholek; //deklaruje glowny wierzcholek na liste kategroii

        char *kat[N],*kod[M],*pewnosc[1]; //definiuje wskazniki na kategroie, kod produktu i wskaznik na pewnosc zawieracjaca znak T/N
        char *lista[L], *nazwa_pliku[L]; //definiuje wskaznik na liste i nazwe pliku
        long int ilosc; //deklaracja zmiennej lokalnej
        int czy_wczytana = 0, x=0; //parametry pomocnicze sprawdzajace czy plik zostal wczytany , oraz definiujaca wybor


        //menu

    do{ //petla typu do while aby wykonywala menu bez przerwy do momentu zamkniecia numerem 6, i nie blokujaca dzialania programu
            //komunikaty
            printf("\n------------------------\n");

            printf("(1) Wczytaj dane z pliku\n");
            printf("(2) Wyswietl liste\n");
            printf("(3) Zmnijesz wartosc porduktu w magazynie\n");
            printf("(4) Przepisz dane do pliku\n");
            printf("(5) Usun konkretny produkt z listy\n");
            printf("(6) Wyjdz z programu\n");

            printf("------------------------\n");

            printf("Podaj numer operacji: ");

            scanf("%d", &x); //WYBOR

        switch(x){ //przelacznik zmienia wybor w zaleznosci od podanej liczby

            case 1: //jezeli wczytano 1
                printf("\nPodaj nazwe pliku w ktorym znajduje sie lista (wraz z jego formatem po kropce) : ");
                scanf("%s", &lista); //wczutuje nazwe pliku z ktorego program ma pobrac dane razem z rozszerzeniem (formatem pliku)

                wierzcholek = Czytaj_z_pliku(lista); //wywoluje funkcje czytaj, tworzaca podlisty
                if(wierzcholek==NULL){ //jezeli wartosc wierzcholka pozostaje Null , oznacza to ,ze dane nie zostaly poprawnie wczytane
                     czy_wczytana=0; //ustawia 0 jako falsz
                }else{
                     czy_wczytana=1;   //ustawia 1 jako prawda
                }


                break;//przerwya i nie wykonuje kolejnuch przypadkow
            case 2:
                if(czy_wczytana==0){ //jezeli jest nie wczytana lista
                    obramuj("Najpierw wczytaj liste");
                }else{ //jezeli zostala wczytana
                    obramuj("Oto aktualna lista:"); //tworzy komunikat
                    wyswietl_liste(wierzcholek); //nastepnie wyswietla na standardowe wyjscie zawartosc listy list
                }
                break;//przerwya i nie wykonuje kolejnuch przypadkow
            case 3:
                 if(czy_wczytana==0){//jezeli jest nie wczytana lista
                    obramuj("Najpierw wczytaj liste");
                }else{//jezeli zostala wczytana
                    printf("\nPodaj kategorie: ");
                    scanf("%s", &kat); //kategroia produktow z ktorej ma pochodzic produkt do pomniejszenia

                    printf("Podaj kod produktu: ");
                    scanf("%s", &kod); //kod produktu, ktory ma zostac pomniejszony

                    printf("Podaj jaka ilosc usunac z magazynu: ");
                    scanf("%ld", &ilosc); //ilosc o jako ma zostac pomniejszony stan produktu
                    printf("\n");

                    Pomnijesz_wartosc(wierzcholek, kat, kod, ilosc); //funkcja zmniejszajaca wartosc konkretnego produktu z listy
                }
                break;//przerwya i nie wykonuje kolejnuch przypadkow
            case 4:
                if(czy_wczytana==0){//jezeli jest nie wczytana lista
                   obramuj("Najpierw wczytaj liste");
                }else{//jezeli zostala wczytana
                    printf("\nPodaj nazwe pliku jaki ma zostac utworzony (bez rozszerzenia pliku): ");
                    scanf("%s", &nazwa_pliku); /*pobiera nazwe pliku wprowadzona przez uzytkownika, w poleceniu plik ma miec to samo rozszerzenie co wprowadzony plik
                                                 wiec program prosi o podanie nazwy docelowej bez rozszerzenia   */
                    wypisz_do_pliku(wierzcholek, nazwa_pliku, lista); //wykonuje funkcje ktora ma wypisac dane z programu do pliku
                }
                break;//przerwya i nie wykonuje kolejnuch przypadkow
            case 5:
                if(czy_wczytana==0){//jezeli jest nie wczytana lista
                   obramuj("Najpierw wczytaj liste");
                }else{//jezeli zostala wczytana
                obramuj("Uwaga probujesz usunac produkt z bazy!");

                printf("\nPodaj kategorie: ");
                scanf("%s", &kat); //pobiera kategorie produktu ktory chce sie usunac

                printf("Podaj kod produktu: ");
                scanf("%s", &kod); //pobiera kod produktu ktory chce sie usunac
                printf("\n");

                obramuj("Czy jestes pewny ze chcesz usunac podany produkt? Wpisz T/N"); //wyswietla komunikat
                scanf("%s", &pewnosc); //pobiera wartosc jednego znaku

                    if((strcmp(pewnosc,"T")==0) || (strcmp(pewnosc,"t")==0)){ //sprawdza czy uzytkownik na pewno chce usunac produkt, jest to operacja nieodwracalna
                        usunElement(wierzcholek, kat, kod); //wykonuje funkcje usuwajaca
                    }else{
                        obramuj("Operacja przerwana!"); //wyswietla komunikat i przerywa
                        break; //przerywa
                    }
                }
                break;//przerwya i nie wykonuje kolejnuch przypadkow
            case 6:
                exit(0); //konczy dzialanie programu
                break;//przerwya i nie wykonuje kolejnuch przypadkow
        }

    }while(x); //WYKONUJE SIE BEZ PRZERWY

    return 0;
}
