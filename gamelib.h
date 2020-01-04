#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/* Questo file contiene le dichiarazioni delle funzioni crea_mappa(), gioca(), termina_gioco().
 Continene anche le definizioni dei tipi utilizzati in gamelib.c:
 struct Giocatore, struct Zona, enum Stato_giocatore, enum Tipo_zona, enum Tipo_oggetto. */

enum Tipo_zona {cucina, soggiorno, rimessa, strada, lungo_lago, uscita_campeggio};
enum Tipo_oggetto {cianfrusaglia, bende, coltello, pistola, benzina, adrenalina};
enum Stato_giocatore {morto, ferito, vivo};

void crea_mappa();
void gioca();
void termina_gioco();

struct Zona {
    enum Tipo_zona zona;
    enum Tipo_oggetto oggetto;
    struct Zona* zona_successiva;
 };

struct Giocatore {
    enum Stato_giocatore stato;
    unsigned short zaino[6];
    struct Zona* posizione;
    unsigned short int mostra;  //lo utilizzo per capire se è stato mostrato l'oggetto prima di prenderlo, se è 0 l'oggetto non è stato mostrato
 };
