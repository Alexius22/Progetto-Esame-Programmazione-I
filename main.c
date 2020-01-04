#include "gamelib.h"

/* Questo file contiene solo la stampa del menu principale e poi richiama una delle
 tre funzioni possibili: crea_mappa(), gioca(), termina_gioco(). */
extern int check;

int main (void) {
    static time_t t;
    srand((unsigned) time(&t));

    char a;
    int choice=0;

    printf("\n");
    printf("ATTENZIONE SI RACCOMANDA DI APRIRE LA FINESTRA A TUTTO SCHERMO PER EVITARE BUG VISIVI\n\n");
    printf("Benvenuto nel Campeggio Lake Trasymeno, infestato dal vendicativo Gieson! Premi qualsiasi tasto con invio per continuare!\n");
    scanf("%c", &a);

    system("clear");
    do {
        printf("Inserire il rispettivo numero della scelta per proseguire:\n"
        "[1] Crea mappa\n"
        "[2] Gioca\n"
        "[3] Termina gioco\n");
        check = scanf(" %d", &choice);
        while (check!=1) {
            system("clear");
            printf("Selezione non valida, inserisci un numero compreso tra 1 e 3 per proseguire\n"
            "Inserire il rispettivo numero della scelta per proseguire:\n"
            "[1] Crea mappa\n"
            "[2] Gioca\n"
            "[3] Termina gioco\n");
            getchar();
            check = scanf(" %d", &choice);
    }


        switch(choice) {
            case 1:
                crea_mappa();
                break;
            case 2:
                gioca();
                break;
            case 3:
                termina_gioco();
                break;
            default:
                system("clear");
                printf("Selezione non valida, inserisci un numero compreso tra 1 e 3 per proseguire\n");
        }
    } while (choice!=3);

return 0;
}
