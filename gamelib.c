#include "gamelib.h"

/* Questo file contiene le definizioni di tutte le funzioni da implementare.
   Quelle visibili in main crea_mappa(), gioca(), termina_gioco() hanno linkage esterno.
   Tutte le funzioni non visibili in main.c e definite qui, devono avere linkage interno.
   Tutte le variabili globali definite in questo file devono avere linkage interno.
   Per esempio, la matrice con zone/probabilita' */

   int check; //variabile visibile in tutto il main usata per effettuare il controllo dell'input da tastiera diverso dal numero;
//Variabili static
   static struct Zona* prima_zona=NULL;     //puntatore alla prima zona.
   static struct Zona* ultima_zona=NULL;    //puntatore all'ultima zona.
   static short count = 0;
   static short mappa = 0;   //Se è 0 la mappa non è chiusa, se è 1 la mappa è chiusa
   static short benzina_usata = 0; //Lo utilizzo per non far apparire gieson in caso assuma valore diverso da 0;
   static short fine_gioco = 0; //Lo utilizzo per capire se entrambi i giocatori hanno vinto o perso;
   static unsigned short roll() { //funzione per la creazione di un numero random
       return rand() % 100;
   }
   static int func_oggetto (int zona) { //funzione per inserire un oggetto ad una data zona;
       const unsigned int oggettoCasuale[6][6]={ //il primo numero è il tipo di zona, il secondo è l'oggetto
           {30, 50, 90, 90, 90, 100},
           {20, 30, 40, 70, 70, 100},
           {20, 30, 60, 60, 90, 100},
           {80, 80, 90, 90, 100, 100},
           {70, 70, 80, 80, 100, 100},
           {90, 90, 100, 100, 100, 100}
       };
       int x = roll();
       int oggetto = 0;
       int pippo = 0;
       for (int i = 0; i < 5; i++) { //scorre la riga della tabella
           if (x>=pippo && x<oggettoCasuale[zona][i] && pippo!=oggettoCasuale[zona][i]) {
               oggetto=i;
               // DEBUG printf("pippo = %d, x = %d, oggettoCasuale = %d\n\n", pippo, x, oggettoCasuale[zona][i]);
           }
           pippo = oggettoCasuale[zona][i];
       }
       return oggetto;
   }
   static char* swap_zona(enum Tipo_zona temp) { //funzione per swappare il valore numerico in testo per la zona
       switch (temp) {
           case cucina:
           return "Cucina\n";
           break;
           case soggiorno:
           return "Soggiorno\n";
           break;
           case rimessa:
           return "Rimessa\n";
           break;
           case strada:
           return "Strada\n";
           break;
           case lungo_lago:
           return "Lungo lago\n";
           break;
           case uscita_campeggio:
           return "Uscita Campeggio\n";
           break;
       }
       return "errore zona";
   }
   static char* swap_oggetto(enum Tipo_oggetto temp) { //funzione per swappare il valore numerico in testo per l'oggetto
       switch (temp) {
           case cianfrusaglia:
           return "cianfrusaglia\n";
           break;
           case bende:
           return "bende\n";
           break;
           case coltello:
           return "coltello\n";
           break;
           case pistola:
           return "pistola\n";
           break;
           case benzina:
           return "benzina\n";
           break;
           case adrenalina:
           return "adrenalina\n";
           break;
       }
       return "errore oggetto";
   }
   static char* swap_stato(enum Stato_giocatore temp) { //funzione per swappare il valore numerico in testo per lo stato
       switch(temp) {
           case morto:
           return "Morto";
           break;
           case ferito:
           return "Ferito";
           break;
           case vivo:
           return "Vivo";
           break;
       }
       return "errore stato";
   }
   static struct Giocatore Marzia;
   static struct Giocatore Giacomo;
   //Dichiarazioni delle funzioni usate in gioca();
   static void avanza();
   static void mostra_oggetto();
   static void prendi_oggetto();
   static void cura();
   static void usa_adrenalina();
   static void gieson();
   static void giesonON();
   static void turno();

    static void ins_zona() {
        unsigned int choice=0;
        system("clear");
        do {
            printf("Inserire il rispettivo numero della scelta per aggiungere quella zona:\n"
            "[1] Cucina\n"
            "[2] Soggiorno\n"
            "[3] Rimessa\n"
            "[4] Strada\n"
            "[5] Lungo lago\n"
            "[6] Torna al menù precedente\n");
            check = scanf(" %d", &choice);
            while (check!=1) {
                system("clear");
                printf("Selezione non valida, inserisci un numero compreso tra 1 e 6 per proseguire:\n"
                "Inserire il rispettivo numero della scelta per aggiungere quella zona:\n"
                "[1] Cucina\n"
                "[2] Soggiorno\n"
                "[3] Rimessa\n"
                "[4] Strada\n"
                "[5] Lungo lago\n"
                "[6] Torna al menù precedente\n");
                getchar();
                check = scanf(" %d", &choice);
            }
            choice--;

            if (choice>=0 && choice<=4) {
                struct Zona* new_zona=(struct Zona*)malloc(sizeof(struct Zona));
                new_zona->zona=choice;
                new_zona->zona_successiva=NULL; //così è valido anche per la prima zona

                if (prima_zona==NULL) {
                    prima_zona=new_zona;
                    ultima_zona=new_zona;
                } else {
                    ultima_zona->zona_successiva=new_zona;
                    ultima_zona = ultima_zona->zona_successiva;
                }
                count++;
                new_zona->oggetto=func_oggetto(choice);

                system("clear");
                if (count==1) {
                    printf("Hai inserito 1 zona!\n");
                } else {
                    printf("Hai inserito %d zone!\n", count);
                }
            } else {
                if (choice==5) {
                    return;
                }
                system("clear");
                printf("Selezione non valida, inserisci un numero compreso tra 1 e 6 per proseguire\n");
            }
        } while(choice!=5);
    }
    static void canc_zona() {
        struct Zona* zona_temp=prima_zona;
        system("clear");
            if (count==0) {
                printf("Non ci sono zone da eliminare!\n");
           }
            else {
                if (count==1) {
                    printf("Hai eliminato l'unica zona!\n");
                    printf("La mappa non ha zone!\n");
                    free(prima_zona);
                    prima_zona=NULL;
                    ultima_zona=NULL;
                    count--;
                    return;
                }
                else {
                    while (zona_temp->zona_successiva!=ultima_zona) {
                    zona_temp=zona_temp->zona_successiva;
                }
                printf("Hai eliminato l'ultima zona!\n");
                free(zona_temp->zona_successiva);
                ultima_zona = zona_temp;
                ultima_zona->zona_successiva=NULL;
                count--;
                if (count==1) {
                    printf("Ora la mappa è composta da 1 zona!\n");
                } else {
                    printf("Ora la mappa è composta da %d zone!\n", count);
                    }
                }
            }
    }
    static void stampa_mappa() {
        system("clear");
        if (count==0) {
            printf("La mappa non ha zone!\n");
       } else {
            struct Zona* temp_zona=prima_zona;
            printf("La mappa è composta dalle seguenti %d zone:\n", count);
            printf("Quando verrà chiusa la mappa verrà automaticamente aggiunta l'ultima zona che corrisponderà all'uscita campeggio\n\n");
            do {
                printf("Nella zona: %s", swap_zona(temp_zona->zona));
                printf("Oggetto: %s", swap_oggetto(temp_zona->oggetto));
                printf("Valore del puntatore: %p\n", temp_zona);
                printf("Valore del puntatore alla zona successiva: %p\n\n", temp_zona->zona_successiva);
                temp_zona=temp_zona->zona_successiva;
            } while(temp_zona!=NULL);
            printf("Fine mappa\n\n");
        }
    }
    static void chiudi_mappa() {
        char temp='0';
        system("clear");
        printf("Attenzione!\nUna volta chiusa la mappa non sarà possibile modificarla ulteriormente\n");
        do {
            printf("Inserire 's' per continuare oppure 'n' per tornare indietro:\n");
            scanf(" %c", &temp);

            if (temp=='n') {
                crea_mappa();
            } else if (temp=='s') {
                struct Zona* new_zona=(struct Zona*)malloc(sizeof(struct Zona));
                new_zona->zona=uscita_campeggio;
                ultima_zona->zona_successiva=new_zona;;
                ultima_zona = new_zona;
                new_zona->oggetto=func_oggetto(uscita_campeggio);
                count++;
                system("clear");
                printf("Congratulazioni! Hai creato una mappa!\n");
            } else {
                system("clear");
                printf("ATTENZIONE\nValore non riconosciuto, inserire nuovamente la risposta\n");
            }
        } while((temp!='s')&&(temp!='n'));
        mappa=1;
    }

    void crea_mappa() {
        struct Zona;
       if (mappa==1)  {
           system("clear");
           printf("La mappa è stata già chiusa, non è possibile modificarla ulteriormente;\n");
       } else {
           int choice=0;
    	   do {
    		   printf("Inserire il rispettivo numero della scelta per proseguire:\n"
    		   "[1] Inserisci una zona\n"
    		   "[2] Cancella ultima zona inserita\n"
    		   "[3] Stampa mappa\n"
    		   "[4] Chiudi mappa\n");
               check = scanf("%d", &choice);
               while (check!=1) {
                   system("clear");
                   printf("Selezione non valida, inserisci un numero compreso tra 1 e 4 per proseguire\n"
                   "Inserire il rispettivo numero della scelta per proseguire:\n"
        		   "[1] Inserisci una zona\n"
        		   "[2] Cancella ultima zona inserita\n"
        		   "[3] Stampa mappa\n"
        		   "[4] Chiudi mappa\n");
                   getchar();
                   check = scanf(" %d", &choice);
               }

               switch (choice) {
                    case 1:
                        ins_zona();
                    break;

                    case 2:
                        canc_zona();
                    break;
                    case 3:
                        stampa_mappa();
                    break;
                    case 4:
                        if (count<7) {
                            system("clear");
                            printf("Non è possibile chiudere la mappa senza almeno 7 zone;\n");
                            printf("La tua mappa attualmente è composta da %d zone;\n", count);
                            crea_mappa();
                        }
                        chiudi_mappa();
                    break;
                    default:
                        system("clear");
                        printf("Selezione non valida, inserisci un numero compreso tra 1 e 4 per proseguire\n");
             }
	        } while(choice!=4);
        }
    }

    // DEFINIZIONE DELLE FUNZIONI USATE IN GIOCA
    static void avanza(struct Giocatore* avanzamento) {
        system("clear");
        if (avanzamento->posizione->zona==uscita_campeggio) {
            giesonON(avanzamento);
            avanzamento->posizione=NULL;
            printf("Complimenti giocatore! Sei riuscito a scappare dal campeggio infestato e a salvarti!\n");
            return;
        }
            giesonON(avanzamento);
            avanzamento->posizione = avanzamento->posizione->zona_successiva;
            avanzamento->mostra=0;
    }
    static void mostra_oggetto(struct Zona* oggettoRandom, struct Giocatore* giocatoreON) {
        system("clear");
        giocatoreON->mostra = 1; //assume valore 1 in modo da controllare se un giocatore ha fatto l'azione 'Cerca oggetto nella zona'
        if (giocatoreON == &Marzia) {
            printf("Marzia");
        } else if (giocatoreON == &Giacomo) {
            printf("Giacomo");
        }
        switch(oggettoRandom->oggetto) {
            case 0:
                printf(", cercando in %sHai trovato della cianfrusaglia!\n\n", swap_zona(oggettoRandom->zona));
                break;
            case 1:
                printf(", cercando in %sHai trovato delle bende!\n\n", swap_zona(oggettoRandom->zona));
                break;
            case 2:
                printf(", cercando in %sHai trovato un coltello!\n\n", swap_zona(oggettoRandom->zona));
                break;
            case 3:
                printf(", cercando in %sHai trovato una pistola!\n\n", swap_zona(oggettoRandom->zona));
                break;
            case 4:
                printf(", cercando in %sHai trovato della benzina!\n\n", swap_zona(oggettoRandom->zona));
                break;
            case 5:
                printf(", cercando in %sHai trovato adrenalina!\n\n", swap_zona(oggettoRandom->zona));
                break;
        }
    }
    static void prendi_oggetto(struct Zona* oggettoRandom, struct Giocatore* giocatoreON) {
        system("clear");
        if (giocatoreON->mostra==0) {
            printf("Non puoi prendere l'oggetto se prima non hai fatto 'Cerca un oggetto nella zona!' Hai perso il turno!\n\n");
        } else {
            if (giocatoreON == &Marzia) {
                printf("Marzia ");
            } else if (giocatoreON == &Giacomo) {
                printf("Giacomo ");
            }
            switch(oggettoRandom->oggetto) {
                case 0:
                    if (giocatoreON->mostra == 1) {
                        printf("hai raccolto della cianfrusaglia!\n\n");
                        oggettoRandom->oggetto=0;
                        giocatoreON->zaino[cianfrusaglia]++;
                    }
                    if (giocatoreON->mostra == 2) {
                        printf("l'oggetto è stato raccolto dall'altro giocatore, hai raccolto della cianfrusaglia!\n\n");
                        giocatoreON->zaino[cianfrusaglia]++;
                    }
                break;
                case 1:
                    printf("hai raccolto delle bende! Puoi utilizzarle quando sei ferito per curarti!\n\n");
                    giocatoreON->zaino[bende]++;
                    oggettoRandom->oggetto=0;
                break;
                case 2:
                    printf("hai raccolto un coltello!\n\n");
                    giocatoreON->zaino[coltello]++;
                    oggettoRandom->oggetto=0;
                break;
                case 3:
                    printf("hai raccolto una pistola!\n\n");
                    giocatoreON->zaino[pistola]++;
                    oggettoRandom->oggetto=0;
                break;
                case 4:
                    printf("hai raccolto della benzina! Se la utilizzi, Gieson non apparirà nei prossimi 4 turni!\n\n");
                    giocatoreON->zaino[benzina]++;
                    oggettoRandom->oggetto=0;
                break;
                case 5:
                    printf("hai raccolto adrenalina!\n\n");
                    giocatoreON->zaino[adrenalina]++;
                    oggettoRandom->oggetto=0;
                break;
                }
                Marzia.mostra=2; //Assume valore 2 quando l'oggetto è stato raccolto;
                Giacomo.mostra=2;
        }
    }
    static void cura(struct Giocatore* heal) {
        system("clear");
        if (heal == &Marzia) {
            printf("Marzia ");
        } else if (heal == &Giacomo) {
            printf("Giacomo ");
        }
        if (heal->zaino[bende]==0) {
            printf("non puoi curarti se non hai bende! Hai perso il turno!\n\n");
        } else {
            if (heal->stato==vivo) {
                printf("non puoi curarti se non sei ferito! Hai perso il turno!\n\n");
            } else {
                printf("hai usato delle bende per curarti! Le ferite riportate dallo scontro con Gieson sono scomparse!\n\n");
                heal->stato=vivo;
                heal->zaino[bende]--;
            }
        }
    }
    static void usa_adrenalina(struct Giocatore* giocatoreON) {
        system("clear");
        if (giocatoreON == &Marzia) {
            printf("Marzia ");
        } else if (giocatoreON == &Giacomo) {
            printf("Giacomo ");
        }
        if (giocatoreON->zaino[adrenalina]==0) {
            printf("non hai adrenalina, non puoi utilizzarla! Hai perso il turno!\n\n");
        } else {
            printf("hai usato adrenalina, puoi effettuare due mosse consecutive!\n\n");
            giocatoreON->zaino[adrenalina]--;
        }
    }
    static void gieson(struct Giocatore* giocatoreON) {
        int choice;
        //system("clear");
        printf("E' apparso Gieson\n\n");
        if (giocatoreON == &Marzia) {
            printf("Marzia, ");
        } else if (giocatoreON == &Giacomo) {
            printf("Giacomo, ");
        }
        if (giocatoreON->zaino[coltello]==0 && giocatoreON->zaino[pistola]==0 && giocatoreON->zaino[benzina]==0) {
            if (giocatoreON->stato!=morto) {
                printf("non hai nulla con cui difenderti, Gieson ti ha ucciso!\n\n");
                giocatoreON->stato=morto;
                return;
            }
        } else if (giocatoreON->zaino[coltello]>=1 && giocatoreON->zaino[pistola]>=1 && giocatoreON->zaino[benzina]>=1) {
            do {
                printf("hai più di un oggetto per difenderti, seleziona il numero corrispondente per effettuare la mossa:\n"
                "[1] Coltello (Gieson va via, ma vi colpisce e si rimane feriti)\n"
                "[2] Pistola (Gieson scappa e non si rimane feriti)\n"
                "[3] Benzina (Gieson scappa, non si rimane feriti e non apparirà per i prossimi 4 turni, ovvero 2 turni per te e 2 per l'altro giocatore)\n");
                check = scanf("%d", &choice);
                while (check!=1) {
                    system("clear");
                    printf("Selezione non valida, inserisci un numero compreso tra 1 e 3 per scegliere come difendersi:\n"
                    "[1] Coltello (Gieson va via, ma vi colpisce e si rimane feriti)\n"
                    "[2] Pistola (Gieson scappa e non si rimane feriti)\n"
                    "[3] Benzina (Gieson scappa, non si rimane feriti e non apparirà per i prossimi 4 turni, ovvero 2 turni per te e 2 per l'altro giocatore)\n");
                    getchar();
                    check = scanf(" %d", &choice);
                }

                switch (choice) {
                    case 1:
                        if (giocatoreON->stato==ferito) {
                            printf("hai usato il coltello per difenderti ma, essendo giò ferito, Gieson ti uccide!\n\n");
                                giocatoreON->stato=morto;
                                giocatoreON->zaino[coltello]--;
                            } else {
                                printf("hai usato il coltello per difenderti! Gieson si è dileguato nel buio, ma sei rimasto ferito!\n\n");
                                giocatoreON->stato--;
                            }
                    break;
                    case 2:
                        printf("hai usato la pistola per difenderti, Gieson è scappato!\n\n");
                            giocatoreON->zaino[pistola]--;
                    break;
                    case 3:
                        printf("hai usato la benzina per difenderti! Gieson è scappato e non apparirà per i prossimi 4 turni!\n\n");
                        benzina_usata = 4;
                        giocatoreON->zaino[benzina]--;
                    break;
                    default:
                        system("clear");
                        printf("Selezione non valida, inserisci un numero compreso tra 1 e 3 per scegliere come difendersi:\n");
                }
        } while (choice<1 || choice>3);
        } else if (giocatoreON->zaino[coltello]>=1 && giocatoreON->zaino[pistola]>=1) {
            do {
                printf("hai più di un oggetto per difenderti, seleziona il numero corrispondente per effettuare la mossa:\n"
                "[1] Coltello (Gieson va via, ma vi colpisce e si rimane feriti)\n"
                "[2] Pistola (Gieson scappa e non si rimane feriti)\n");
                check = scanf("%d", &choice);
                while (check!=1) {
                    system("clear");
                    printf("Selezione non valida, inserisci un numero compreso tra 1 e 2 per scegliere come difendersi:\n"
                    "[1] Coltello (Gieson va via, ma vi colpisce e si rimane feriti)\n"
                    "[2] Pistola (Gieson scappa e non si rimane feriti)\n");
                    getchar();
                    check = scanf(" %d", &choice);
                }
                switch (choice) {
                    case 1:
                        if (giocatoreON->stato==ferito) {
                            printf("hai usato il coltello per difenderti ma, essendo giò ferito, Gieson ti uccide!\n\n");
                                giocatoreON->stato=morto;
                                giocatoreON->zaino[coltello]--;
                            } else {
                                printf("hai usato il coltello per difenderti! Gieson si è dileguato nel buio, ma sei rimasto ferito!\n\n");
                                giocatoreON->stato--;
                            }
                    break;
                    case 2:
                        printf("hai usato la pistola per difenderti! Gieson è scappato!\n\n");
                        giocatoreON->zaino[pistola]--;
                    break;
                    default:
                        system("clear");
                        printf("Selezione non valida, inserisci un numero compreso tra 1 e 2 per scegliere come difendersi:\n");
                }
            } while (choice<1 || choice>2);
        } else if (giocatoreON->zaino[pistola]>=1 && giocatoreON->zaino[benzina]>=1) {
            do {
                printf("hai più di un oggetto per difenderti, seleziona il numero corrispondente per effettuare la mossa:\n"
                "[1] Pistola (Gieson scappa e non si rimane feriti)\n"
                "[2] Benzina (Gieson scappa, non si rimane feriti e non apparirà per i prossimi 4 turni, ovvero 2 turni per te e 2 per l'altro giocatore)\n");
                check = scanf("%d", &choice);
                while (check!=1) {
                    system("clear");
                    printf("Selezione non valida, inserisci un numero compreso tra 1 e 2 per scegliere come difendersi:\n"
                    "[1] Pistola (Gieson scappa e non si rimane feriti)\n"
                    "[2] Benzina (Gieson scappa, non si rimane feriti e non apparirà per i prossimi 4 turni, ovvero 2 turni per te e 2 per l'altro giocatore)\n");
                    getchar();
                    check = scanf(" %d", &choice);
                }
                switch (choice) {
                    case 1:
                        printf("hai usato la pistola per difenderti! Gieson è scappato!\n\n");
                        giocatoreON->zaino[pistola]--;
                        giocatoreON->stato--;
                    break;
                    case 2:
                        printf("hai usato la benzina per difenderti! Gieson è scappato e non apparirà per i prossimi 4 turni!\n\n");
                        benzina_usata = 4;
                        giocatoreON->zaino[benzina]--;
                    break;
                    default:
                        system("clear");
                        printf("Selezione non valida, inserisci un numero compreso tra 1 e 2 per scegliere come difendersi:\n");
                }
            } while (choice<1 || choice>2);
        } else if (giocatoreON->zaino[coltello]>=1 && giocatoreON->zaino[benzina]>=1) {
            do {
                printf("hai più di un oggetto per difenderti, seleziona il numero corrispondente per effettuare la mossa:\n"
                "[1] Coltello (Gieson va via, ma vi colpisce e si rimane feriti)\n"
                "[2] Benzina (Gieson scappa, non si rimane feriti e non apparirà per i prossimi 4 turni, ovvero 2 turni per te e 2 per l'altro giocatore)\n");
                check = scanf("%d", &choice);
                while (check!=1) {
                    system("clear");
                    printf("Selezione non valida, inserisci un numero compreso tra 1 e 2 per scegliere come difendersi:\n"
                    "[1] Coltello (Gieson va via, ma vi colpisce e si rimane feriti)\n"
                    "[2] Benzina (Gieson scappa, non si rimane feriti e non apparirà per i prossimi 4 turni, ovvero 2 turni per te e 2 per l'altro giocatore)\n");
                    getchar();
                    check = scanf(" %d", &choice);
                }
                switch (choice) {
                    case 1:
                        if (giocatoreON->stato==ferito) {
                            printf("hai usato il coltello per difenderti ma, essendo giò ferito, Gieson ti uccide!\n\n");
                                giocatoreON->stato=morto;
                                giocatoreON->zaino[coltello]--;
                            } else {
                                printf("hai usato il coltello per difenderti! Gieson si è dileguato nel buio, ma sei rimasto ferito!\n\n");
                                giocatoreON->stato--;
                            }
                    break;
                    case 2:
                        printf("hai usato la benzina per difenderti! Gieson è scappato e non apparirà per i prossimi 4 turni!\n\n");
                        benzina_usata = 4;
                        giocatoreON->zaino[benzina]--;
                    break;
                    default:
                        system("clear");
                        printf("Selezione non valida, inserisci un numero compreso tra 1 e 2 per scegliere come difendersi:\n");
                }
            } while (choice<1 || choice>2);
        } else if (giocatoreON->zaino[coltello]>=1) {
            if (giocatoreON->stato==ferito) {
                printf("hai usato il coltello per difenderti ma, essendo giò ferito, Gieson ti uccide!\n\n");
                    giocatoreON->stato=morto;
                } else {
                    printf("hai usato il coltello per difenderti! Gieson si è dileguato nel buio, ma sei rimasto ferito!\n\n");
                    giocatoreON->stato--;
                    giocatoreON->zaino[coltello]--;
                }
            } else if (giocatoreON->zaino[pistola]>=1) {
                printf("hai usato la pistola per difenderti! Gieson è scappato!\n\n");
                giocatoreON->zaino[pistola]--;
            } else if (giocatoreON->zaino[benzina]>=1) {
                printf("hai usato la benzina per difenderti! Gieson è scappato e non apparirà per i prossimi 4 turni!\n\n");
                benzina_usata = 4;
                giocatoreON->zaino[benzina]--;
            }
    }
    static void giesonON(struct Giocatore* giocatoreON) {
        int y = roll();
        if (benzina_usata>0) {
            return;
        } else {
            if (giocatoreON->posizione->zona==uscita_campeggio) {
                if (y<=75) {
                    gieson(giocatoreON);
                }
            } else if (Marzia.posizione==NULL || Giacomo.posizione==NULL || Marzia.stato==morto || Giacomo.stato==morto) {
                if (y<=50) {
                    gieson(giocatoreON);
                }
            } else {
                if (y<=30) {
                    gieson(giocatoreON);
                }
            }
        }
    }
    static void turno(struct Giocatore* giocatoreON) {
        unsigned int choice = 0;
            do {
                if (giocatoreON == &Marzia) {
                    printf("TURNO DI MARZIA\n\n");
                } else {
                    printf("TURNO DI GIACOMO\n\n");
                }
                if (benzina_usata!=0) {
                    printf("Numero di turni senza Gieson grazie all'utilizzo della benzina: %d\n\n", benzina_usata);
                }
                printf("Stato %s\nSi trova in %s\nOggetti nello zaino:\n", swap_stato(giocatoreON->stato), swap_zona(giocatoreON->posizione->zona));
                if (giocatoreON->zaino[0]==1) {
                    printf("1 cianfrusaglia\n");
                } else {
                    printf("%d cianfrusaglie\n", giocatoreON->zaino[cianfrusaglia]);
                }
                if (giocatoreON->zaino[1]==1) {
                    printf("1 benda\n");
                } else {
                    printf("%d bende\n", giocatoreON->zaino[bende]);
                }
                if (giocatoreON->zaino[2]==1) {
                    printf("1 coltello\n");
                } else {
                    printf("%d coltelli\n", giocatoreON->zaino[coltello]);
                }
                if (giocatoreON->zaino[3]==1) {
                    printf("1 pistola\n");
                } else {
                    printf("%d pistole\n", giocatoreON->zaino[pistola]);
                }
                if (giocatoreON->zaino[4]==1) {
                    printf("1 benzina\n");
                } else {
                    printf("%d benzine\n", giocatoreON->zaino[benzina]);
                }
                if (giocatoreON->zaino[5]==1) {
                    printf("1 adrenalina\n\n");
                } else {
                    printf("%d adrenaline\n\n", giocatoreON->zaino[adrenalina]);
                }
                printf("Inserire il rispettivo numero della scelta per eseguire la mossa:\n"
                "[1] Avanza alla zona successiva\n"
                "[2] Cerca un oggetto nella zona\n"
                "[3] Raccogli l'oggetto trovato nella zona\n"
                "[4] Curati\n"
                "[5] Usa adrenalina\n");
                check = scanf("%d", &choice);
                while (check!=1) {
                    system("clear");
                    printf("Selezione non valida, inserisci un numero compreso tra 1 e 5 per eseguire la mossa:\n"
                    "[1] Avanza alla zona successiva\n"
                    "[2] Cerca un oggetto nella zona\n"
                    "[3] Raccogli l'oggetto trovato nella zona\n"
                    "[4] Curati\n"
                    "[5] Usa adrenalina\n");
                    getchar();
                    check = scanf(" %d", &choice);
                }

                switch (choice) {
                    case 1:
                        avanza(giocatoreON);
                    break;
                    case 2:
                        mostra_oggetto(giocatoreON->posizione, giocatoreON);
                        giesonON(giocatoreON);
                    break;
                    case 3:
                        prendi_oggetto(giocatoreON->posizione, giocatoreON);
                        giesonON(giocatoreON);
                    break;
                    case 4:
                        cura(giocatoreON);
                        giesonON(giocatoreON);
                    break;
                    case 5:
                        usa_adrenalina(giocatoreON);
                        if (giocatoreON == &Marzia) {
                            giocatoreON = &Marzia;
                            turno(giocatoreON);
                            if (giocatoreON->stato!=morto && giocatoreON->posizione!=NULL) {
                                giesonON(giocatoreON);
                            }
                            if (giocatoreON->stato==morto) {
                                return;
                            } else {
                                turno(giocatoreON);
                                if (giocatoreON->stato!=morto && giocatoreON->posizione!=NULL) {
                                    giesonON(giocatoreON);
                                }
                            }
                        } else {
                            giocatoreON = &Giacomo;
                            turno(giocatoreON);
                            if (giocatoreON->stato!=morto && giocatoreON->posizione!=NULL) {
                                giesonON(giocatoreON);
                            }
                            if (giocatoreON->stato==morto) {
                                return;
                            } else {
                                turno(giocatoreON);
                                if (giocatoreON->stato!=morto && giocatoreON->posizione!=NULL) {
                                    giesonON(giocatoreON);
                                }
                            }
                        };
                    break;
                    default:
                        system("clear");
                        printf("Selezione non valida, inserisci un numero compreso tra 1 e 5 per eseguire la mossa:\n");
                }
            } while(choice<1 || choice>5);
            if (benzina_usata>0) {
                benzina_usata--;
            }
    }

    void gioca() {
        system("clear");
        char a;
        struct Giocatore* giocatoreON;
        if (mappa==0) {
            printf("Non hai ancora creato la mappa, non puoi giocare!\n");
            return;
        }
        if (fine_gioco==1) {
            printf("Hai già giocato, non puoi rigiocare, termina e riavvia il gioco!\n");
            return;
        }
        while(a!='s') {
            printf("È venerdì 13 giugno 1980 ed un gruppo di studenti universitari del Corso di Laurea in Informatica si trasferiscono in vacanza al “Campeggio Lake Trasymeno”, che sta per riaprire. Ventitre anni prima, infatti, nello stesso campeggio, un ragazzino di nome Gieson era annegato per colpa della negligenza di un programmatore: a causa di un segmentation fault nel suo programma di noleggio delle barche, alla famiglia di Gieson era stata affidata una barca con un motore difettoso. Gieson però infesta ancora il campeggio, e nutre vendetta nei confronti degli informatici poco attenti che hanno seguito il corso di Programmazione I giocando a League of Legends... Giacomo e Marzia, entrambi studenti del primo anno, rimangono isolati dagli altri, dunque preoccupati decidono di scappare dal campeggio, ritrovandosi in..\nPremi s per continuare!\n");
            scanf(" %c", &a);
            if (a!='s') {
                system("clear");
                printf("ATTENZIONE\nValore non riconosciuto, inserire nuovamente la risposta\n");
            }
        }

        //Info Marzia
        Marzia.stato = vivo;
        //Marzia.zaino[benzina]=4; //DEBUG
        Marzia.zaino[adrenalina]=2;
        Marzia.posizione = prima_zona;
        Marzia.mostra = 0;

        //Info Giacomo
        Giacomo.stato = vivo;
        //Giacomo.zaino[benzina]=4; //DEBUG
        Giacomo.zaino[coltello]=1;
        Giacomo.posizione = prima_zona;
        Giacomo.mostra = 0;

        do {
            if (Marzia.stato==morto && Giacomo.stato==morto) {
                fine_gioco = 1;
                printf("Entrambi siete morti!\n\n");
                termina_gioco();
                return;
            }
            if (Giacomo.posizione==NULL && Marzia.posizione==NULL) {
                fine_gioco = 1;
                printf("Complimenti entrambi siete sopravvissuti!\n\n");
                return;
            }
            if ((Giacomo.stato==morto || Giacomo.posizione==NULL) && (Marzia.stato!=morto && Marzia.posizione!=NULL)) {
                giocatoreON = &Marzia;
                turno(giocatoreON);
            }
            if  ((Marzia.stato==morto || Marzia.posizione==NULL) && (Giacomo.stato!=morto && Giacomo.posizione!=NULL)) {
                giocatoreON = &Giacomo;
                turno(giocatoreON);
            }
            if (Giacomo.stato!=morto && Giacomo.posizione==NULL && Marzia.stato==morto && Marzia.posizione!=NULL) {
                printf("Ha vinto solo Giacomo!\n\n");
                return;
            }
            if (Marzia.stato!=morto && Marzia.posizione==NULL && Giacomo.stato==morto && Giacomo.posizione!=NULL) {
                printf("Ha vinto solo Marzia!\n\n");
                return;
            }
            if (Giacomo.stato!=morto && Marzia.stato!=morto && Giacomo.posizione!=NULL && Marzia.posizione!=NULL) {
                int x = roll();
                if (x<50) {
                    giocatoreON = &Marzia;
                    turno(giocatoreON);
                    giocatoreON = &Giacomo;
                    turno(giocatoreON);
                } else {
                    giocatoreON = &Giacomo;
                    turno(giocatoreON);
                    giocatoreON = &Marzia;
                    turno(giocatoreON);
                }
            }
        } while(fine_gioco!=1);
    }

    void termina_gioco() {
        printf("\n"
        "ÛÛÛÛÛÛÛÛÛ    ÛÛÛÛÛÛÛÛÛ   ÛÛÛÛÛÛ   ÛÛÛÛÛÛ ÛÛÛÛÛÛÛÛÛÛ       ÛÛÛÛÛÛÛ    ÛÛÛÛÛ   ÛÛÛÛÛ ÛÛÛÛÛÛÛÛÛÛ ÛÛÛÛÛÛÛÛÛÛÛ  \n"
       "ÛÛÛ°°°°°ÛÛÛ  ÛÛÛ°°°°°ÛÛÛ °°ÛÛÛÛÛÛ ÛÛÛÛÛÛ °°ÛÛÛ°°°°°Û     ÛÛÛ°°°°°ÛÛÛ °°ÛÛÛ   °°ÛÛÛ °°ÛÛÛ°°°°°Û°°ÛÛÛ°°°°°ÛÛÛ \n"
     " ÛÛÛ     °°°  °ÛÛÛ    °ÛÛÛ  °ÛÛÛ°ÛÛÛÛÛ°ÛÛÛ  °ÛÛÛ  Û °     ÛÛÛ     °°ÛÛÛ °ÛÛÛ    °ÛÛÛ  °ÛÛÛ  Û °  °ÛÛÛ    °ÛÛÛ \n"
     "°ÛÛÛ          °ÛÛÛÛÛÛÛÛÛÛÛ  °ÛÛÛ°°ÛÛÛ °ÛÛÛ  °ÛÛÛÛÛÛ      °ÛÛÛ      °ÛÛÛ °ÛÛÛ    °ÛÛÛ  °ÛÛÛÛÛÛ    °ÛÛÛÛÛÛÛÛÛÛ  \n"
    " °ÛÛÛ    ÛÛÛÛÛ °ÛÛÛ°°°°°ÛÛÛ  °ÛÛÛ °°°  °ÛÛÛ  °ÛÛÛ°°Û      °ÛÛÛ      °ÛÛÛ °°ÛÛÛ   ÛÛÛ   °ÛÛÛ°°Û    °ÛÛÛ°°°°°ÛÛÛ \n"
    " °°ÛÛÛ  °°ÛÛÛ  °ÛÛÛ    °ÛÛÛ  °ÛÛÛ      °ÛÛÛ  °ÛÛÛ °   Û   °°ÛÛÛ     ÛÛÛ   °°°ÛÛÛÛÛ°    °ÛÛÛ °   Û °ÛÛÛ    °ÛÛÛ \n"
    "  °°ÛÛÛÛÛÛÛÛÛ  ÛÛÛÛÛ   ÛÛÛÛÛ ÛÛÛÛÛ     ÛÛÛÛÛ ÛÛÛÛÛÛÛÛÛÛ    °°°ÛÛÛÛÛÛÛ°      °°ÛÛÛ      ÛÛÛÛÛÛÛÛÛÛ ÛÛÛÛÛ   ÛÛÛÛÛ\n"
     "   °°°°°°°°°  °°°°°   °°°°° °°°°°     °°°°° °°°°°°°°°°       °°°°°°°         °°°      °°°°°°°°°° °°°°°   °°°°° \n"
        "\n\n");
        if(prima_zona!=NULL){
            struct Zona* pnt=prima_zona, *temp=pnt;
            while(temp->zona_successiva!=NULL){
                pnt=temp;
                temp=temp->zona_successiva;
                free(pnt);
            }
            free(temp);
        }
    }
