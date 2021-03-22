/* GHERASIE STEFANIA-ELENA 313 CB */

typedef struct history
{
	char info[100];
	struct history *urm;

} History, *THistory, **AHistory;


typedef struct lsc
{	
	long int card_number;
	char pin[5];
	int pin_tries;  
	char expiry_date[10];
	int cvv;
	int balance;
	char status[10];
	THistory history;
	struct lsc *urm;
} LSC, *TLSC, **ALSC;


typedef struct lc
{
	TLSC info;
	int poz;
	struct lc *urm;
} LC, *TLC, **ALC;