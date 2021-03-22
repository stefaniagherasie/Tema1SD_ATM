/* GHERASIE STEFANIA-ELENA 313 CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tip.h"


/*calculeaza suma cifrelor unui numar*/
int number_sum(long int x)
{
	int sum = 0;
	while(x != 0)
	{
		sum += x%10;
		x /= 10;
	}
	return sum;
}


/*testeaza daca pinul este valid*/
int pin_valid(char str[5])
{
	int nr = 0;

	while(str[nr] != '\0')
	{
		if(str[nr] >= '0' && str[nr] <= '9')
	        nr++;
	    else return 0;

	}
	if(nr == 4)
		return 1;
	else return 0;
}


/*verifica daca exista un card in lista*/
int find_card(TLSC LSC, long int card_number)
{
	for(; LSC != NULL; LSC = LSC->urm)
		if(LSC->card_number == card_number)
			return 1;
	return 0;
}


/* aloca celula de tip History in lista */
THistory alloc_history(char str[100])
{
	THistory aux;
	aux = (THistory)malloc(sizeof(History));
	if(aux)
	{
		strcpy (aux->info, str);
		aux->urm = NULL;		
	}
	return aux;
}


/*aloca si initializeaza celula de tip LSC */
TLSC alloc_lsc()
{
	TLSC aux;
	aux = (TLSC)malloc(sizeof(LSC));
	if(aux)
	{
		aux->card_number = 0; 
		strcpy(aux->pin, "");
		aux->pin_tries = 0;
		strcpy(aux->expiry_date, "");
		aux->cvv = 0;
		aux->balance = 0;
		strcpy(aux->status, "NEW");
		aux->history = alloc_history("");
		aux->urm = NULL;
	}
	return aux;
}


/*aloca celula de tip LC */
TLC alloc_lc(int poz)
{
	TLC aux;
	aux = (TLC)malloc(sizeof(LC));
	if(!aux)
		return NULL;
	aux -> info = NULL;
	aux->poz = poz;
	aux->urm = NULL;

	return aux;
}


/*insereaza la inceput in lista History*/
int insert_history(AHistory aH, char str[100])
{
	THistory aux;
	aux = alloc_history(str);
	if(!aux)
		return 0;
	aux->urm = (*aH);
	(*aH) = aux;

	return 1;	
}


/*insereaza la inceput in lista LSC */
int insert_lsc(ALSC aLSC)
{
	TLSC aux;
	aux = alloc_lsc();
	if(!aux)
		return 0;
	aux->urm = (*aLSC);
	strcpy(aux->status, "NEW");
	(*aLSC) = aux;

	return 1;
}


/*insereaza la sfarsit celule de tip LC */
int insert_lc(ALC aLC, int poz)
{
	TLC aux, u = NULL;

	if(*aLC != NULL)
	{
		u = *aLC;
		while(u->urm != NULL)
			u = u->urm;
	}
	aux = alloc_lc(poz);
	if(!aux)
		return 0;
	if(u == NULL)
		*aLC = aux;
	else
		u->urm = aux;
	aux->info = NULL;
	aux->urm = NULL;

	return 1;
}

/*eliberare history*/
void free_history(AHistory aH)
{
	THistory aux = NULL;
	while(*aH)
	{
		aux = *aH;
		*aH = aux->urm;
		free(aux);
	}
}


/*eliberare LSC*/
void free_lsc(ALSC aLSC)
{
	TLSC aux = NULL;
	while(*aLSC)
	{
		aux = *aLSC;
		*aLSC = aux->urm;
			free_history(&(aux->history)); 
		free(aux);
	}
}


/*eliberare LC*/
void free_lc(ALC aLC)
{
	TLC aux = NULL;
	while(*aLC)
	{
		aux = *aLC;
		*aLC = aux->urm;
			free_lsc(&(aux->info));
		free(aux);
	}
}


/*afiseaza in fisier o celula LSC */
void show_lsc(TLSC LSC, FILE* out)
{
	fprintf(out, "(card number: %ld, ", LSC->card_number);
	fprintf(out, "PIN: %s, ", LSC->pin);
	fprintf(out, "expiry date: %s, ", LSC->expiry_date);
	fprintf(out, "CVV: %d, ", LSC->cvv);
	fprintf(out, "balance: %d, ", LSC->balance);
	fprintf(out, "status: %s, ", LSC->status);
	fprintf(out,"history: [");
	/*parcurge si afiseaza lista History pentru fiecare celula LSC */
	for(; LSC->history->urm != NULL; LSC->history = LSC->history->urm)
	{
		fprintf(out,"%s", LSC->history->info);
		if(LSC->history->urm->urm != NULL)
			fprintf(out, ", ");
	}
	fprintf(out, "])");
}


/*afiseaza informatiile din card la apelarea functiei show*/
void show_card(TLC LC, int long card_number, FILE* out)
{
	for(; LC != NULL; LC = LC->urm)
		if(find_card(LC->info, card_number) == 1)
		{
			show_lsc(LC->info, out);
			break;
		}
}


/* afiseaza informatiile din lista la apelarea functiei show*/
void show(TLC LC, int max_poz, FILE* out)
{
	TLC p = LC;
	int  i = 0;

	for(; p!= NULL;  p = p->urm)	
	{	
		if(p->poz <= max_poz)
		{
			/*celula LC e vida*/
			if(p->info == NULL)			
				fprintf(out, "pos%d: []\n", i);
			/*celula LC contine carduri*/
			if(p->info != NULL)
			{
				fprintf(out, "pos%d: [\n", i);
				while(p->info != NULL)
				{
					show_lsc(p->info, out);
					fprintf(out, "\n" );
					p->info = p->info->urm;
				}
				fprintf(out, "]\n");	
			}
			i++;
		}	
		else  break;
	}		
}


/*testeaza daca cardul exista deja in lista*/
int card_exist(TLC LC, long int card_number)
{

	for(; LC != NULL; LC = LC->urm)
		if(find_card(LC->info, card_number) == 1)
				return 1;

	return 0;
}


/*adauga card*/
void add_card(ALC aLC, TLSC card, int poz, int max_poz, FILE* out)
{
	TLC p = *aLC, ant;
	int  var;
	for(ant = NULL; p != NULL; ant = p, p = p->urm)
	{
		/*introduce card pe pozitia corespunzatoare*/
		if(p->poz == poz)
		{
			var = insert_lsc(&(p->info));
			p->info->card_number = card->card_number;
			strcpy(p->info->pin, card->pin);
			strcpy(p->info->expiry_date, card->expiry_date);
			p->info->cvv = card->cvv;
			strcpy(p->info->status, "NEW");
		}	
		/*refacere lista*/
		if(ant == NULL)
			*aLC = p;
		else 
			ant->urm = p;	
	}
}


/*sterge card din lista*/
void delete_card(ALC aLC, long int card_number)
{
	TLC  p = *aLC, ant;
	TLSC ant_card;
	int ok = 0;

	for(ant = NULL; p != NULL; ant = p, p = p->urm)
	{
		for(ant_card = NULL; p->info != NULL; ant_card = p->info)
		{
			if(p->info->card_number == card_number)
			{
				ok = 1;
				break;
			}
			p->info = p->info->urm;
		}

		/*eliminare card prin refacerea legaturilor*/	
		if(ant_card == NULL)
			(*aLC)->info = p->info->urm;
		else ant_card->urm = p->info->urm;
		free(p->info);
		if(ok == 1)
			break;
	}
	/*refacere lista*/
	if(ant == NULL)
		*aLC = p;
	else 
		ant->urm = p;	
}


/*inserare cars*/
void insert_card(ALC aLC, long int card_number, char pin[5], FILE* out)
{
	TLC p = *aLC, ant = NULL;
	TLSC ant_card;
	int ok;
	char str[100];
	/*retine numarul de apelari ale functiei*/
	static int nr = 0; 

	for(ant = NULL; p != NULL; ant = p, p = p->urm)
	{
		for(ant_card = NULL; p->info != NULL; ant_card = p->info)
		{
			if(find_card(p->info, card_number) == 1)
			{	ok = 1;
				break;
			}
			p->info = p->info->urm;
		}
		/*card care are inca pinul initial*/
		if(p->info->pin_tries == 0)
			nr = 0;
		if(pin_valid(pin) == 0 || strcmp(p->info->pin, pin) != 0)
		{
			if(strcmp(p->info->status, "LOCKED") != 0)	
			{
				nr++;
				p->info->pin_tries = p->info->pin_tries + 1;

				sprintf(str, "(FAIL, insert_card %ld ", card_number);
				strcat(str, pin);
				strcat(str, ")");
				ok = insert_history(&(p->info->history), str);
				if(nr <= 3)
					fprintf(out, "Invalid PIN\n");
				if(nr == 3)
				{
					fprintf(out, "The card is blocked. Please contact the administrator.\n");
					strcpy(p->info->status, "LOCKED");
					nr = 0;
				}
			}
		}
		else if((strcmp(p->info->status, "LOCKED") != 0))
		{
			fprintf(out, "You must change your PIN.\n");
			sprintf(str, "(SUCCESS, insert_card %ld ", card_number);
			strcat(str, pin);
			strcat(str, ")");
			ok = insert_history(&(p->info->history), str);
			nr = 0;
		}
		else if(strcmp(p->info->status, "LOCKED") == 0)
		{
			sprintf(str, "(FAIL, insert_card %ld ", card_number);
			strcat(str, pin);
			strcat(str, ")");
			ok = insert_history(&(p->info->history), str);
			fprintf(out, "The card is blocked. Please contact the administrator.\n");
			strcpy(p->info->status, "LOCKED");
		}
		/*reconstructie lista LSC*/
		if(ant_card == NULL)
			(*aLC)->info = p->info;
		else
		{

			ant_card->urm = p->info;
		}
		if(ok == 1)
			break;
	}	
	/*reconstructie lista LC*/	
	if(ant == NULL)
		*aLC = p;
	else 
		ant->urm = p;	
}


/*deconecteaza cardul*/
void cancel(ALC aLC, long int card_number)
{
	TLC p = *aLC, ant = NULL;
	TLSC ant_card;
	int ok = 0, var;
	char str[100];
	/*parcurgere pentru a scrie in history*/
	for(ant = NULL; p != NULL; ant = p, p = p->urm)
	{
		for(ant_card = NULL; p->info != NULL; ant_card = p->info)
		{
			if(find_card(p->info, card_number) == 1)
			{	ok = 1;
				break;
			}
			p->info = p->info->urm;
		}
		if(ok == 0)			
		{	
			sprintf(str, "(FAIL, cancel %ld)", card_number);
			var = insert_history(&(p->info->history), str);
		}
		else
		{
			sprintf(str, "(SUCCESS, cancel %ld)", card_number);
			var = insert_history(&(p->info->history), str);
		}
		/*refacere lista LSC*/
		if(ant_card == NULL)
			(*aLC)->info = p->info;
		else
		{

			ant_card->urm = p->info;
		}
		if(ok == 1)
			break;
	}	
	/*refacere lista LC*/	
	if(ant == NULL)
		*aLC = p;
	else 
		ant->urm = p;	
}


/*deblocare care*/
void unblock_card(ALC aLC, long int card_number)
{
	TLC p = *aLC, ant = NULL;
	TLSC ant_card;
	int ok;

	/*parcurgere petru gasire card si activare*/
	for(ant = NULL; p != NULL; ant = p, p = p->urm)
	{
		for(ant_card = NULL; p->info != NULL; ant_card = p->info)
		{	if(p->info->card_number == card_number)
			{	
				ok = 1;
				p->info->pin_tries = 0;
				strcpy(p->info->status, "ACTIVE");
				break;
			}
			p->info = p->info->urm;
		}
		/*refacere lista LSC*/
		if(ant_card == NULL)
			(*aLC)->info = p->info;
		else
			ant_card->urm = p->info;

		if(ok == 1)
			break;
	}	
	/*refacere lista LC*/		
	if(ant == NULL)
		*aLC = p;
	else 
		ant->urm = p;	
}


/*schimbare pin*/
void pin_change(ALC aLC, long int card_number, char pin[5], FILE* out)
{
	TLC p = *aLC, ant = NULL;
	TLSC ant_card;
	int ok = 0, var;
	char str[100];
	/*caz pinul este invalid*/
	if(pin_valid(pin) == 0)
	{
		fprintf(out, "Invalid PIN\n");
		sprintf(str, "(FAIL, pin_change %ld ", card_number);
		strcat(str, pin);
		strcat(str, ")");
		var = insert_history(&(p->info->history), str);
	}
	/*schimbare pin si trecerea operatiei in history*/
	else
	{
		for(ant = NULL; p != NULL; ant = p, p = p->urm)
		{
			for(ant_card = NULL; p->info != NULL; ant_card = p->info)
			{
				if(p->info->card_number == card_number)
				{	
					ok = 1;		
					sprintf(str, "(SUCCESS, pin_change %ld ", card_number);
					strcat(str, pin);
					strcat(str, ")");					
					strcpy(p->info->pin, pin);
					strcpy(p->info->status, "ACTIVE");
					var = insert_history(&(p->info->history), str);
					break;
				}
				p->info = p->info->urm;
			}
			/*refacere lista LSC */
			if(ant_card == NULL)
				(*aLC)->info = p->info;
			else
				ant_card->urm = p->info;

			if(ok == 1)
				break;
		}	
		/*refacere lista LC*/		
		if(ant == NULL)
			*aLC = p;
		else 
			ant->urm = p;	
	}
}


/*afisare sold*/
void balance_inquiry(ALC aLC, long int card_number, FILE* out)
{
	TLC p = *aLC, ant = NULL;
	TLSC ant_card;
	int ok = 0, var;
	char str[100];

	/*gaseste cardul si afiseaza operatia in history*/
	for(ant = NULL; p != NULL; ant = p, p = p->urm)
	{
		for(ant_card = NULL; p->info != NULL; ant_card = p->info)
		{
			if(p->info->card_number == card_number)
			{	
				ok = 1;		
				sprintf(str, "(SUCCESS, balance_inquiry %ld)", card_number);				
				var = insert_history(&(p->info->history), str);
				fprintf(out, "%d\n", p->info->balance);
				break;
			}
			p->info = p->info->urm;
		}
		/*refacere lista LSC*/
		if(ant_card == NULL)
			(*aLC)->info = p->info;
		else
			ant_card->urm = p->info;

		if(ok == 1)
			break;
	}	
	/*refacere lista LC*/		
	if(ant == NULL)
		*aLC = p;
	else 
		ant->urm = p;			
}


/*depunere numerar*/
void recharge(ALC aLC, long int card_number, int sum, FILE* out)
{
	TLC p = *aLC, ant = NULL;
	TLSC ant_card;
	int ok = 0, var;
	char str[100];

	/*parcurgere pt gasirea cardului si efectuarea operatiei*/
	for(ant = NULL; p != NULL; ant = p, p = p->urm)
	{
		for(ant_card = NULL; p->info != NULL; p->info = p->info->urm)
		{
			if(p->info->card_number == card_number)
			{	
				ok = 1;	
				if(sum % 10 != 0)
				{
					fprintf(out,"The added amount must be multiple of 10\n");
					sprintf(str,"(FAIL, recharge %ld %d)",card_number, sum);
					var = insert_history(&(p->info->history), str);
				}
				else	
				{
					p->info->balance = p->info->balance + sum;
					sprintf(str, "(SUCCESS, recharge %ld %d)",card_number,sum);
					var = insert_history(&(p->info->history), str);
					fprintf(out, "%d\n", p->info->balance);
				}
				break;
			}
			ant_card = p->info;
		}
		/*refacere lista LSC*/
		if(ant_card == NULL)
			(*aLC)->info = p->info;
		else
			ant_card->urm = p->info;

		if(ok == 1)
			break;
	}		
	/*refacere lsita LC*/	
	if(ant == NULL)
		*aLC = p;
	else 
		ant->urm = p;	
}


/*retragere numerar*/
void cash_withdrawal(ALC aLC, long int card_number, int sum, FILE* out)
{
	TLC p = *aLC, ant = NULL;
	TLSC ant_card;
	int ok = 0, var;
	char str[100];

	/*parcurgere pt gasirea cardului si efectuarea operatiei*/	
	for(ant = NULL; p != NULL; ant = p, p = p->urm)
	{
		for(ant_card = NULL; p->info != NULL; ant_card = p->info)
		{
			if(p->info->card_number == card_number)
			{	
				ok = 1;
				if(sum % 10 != 0)
				{
					fprintf(out, "The requested amount must be multiple of 10\n");
					sprintf(str, "(FAIL, cash_withdrawal %ld %d)",card_number,sum);
					var = insert_history(&(p->info->history), str);
				}	
				else
				{
					if(p->info->balance < sum)
					{	fprintf(out, "Insufficient funds\n");
						sprintf(str,"(FAIL, cash_withdrawal %ld %d)",card_number,sum);
						var = insert_history(&(p->info->history), str);
					}
					else
					{
						p->info->balance = p->info->balance - sum;
						sprintf(str,"(SUCCESS, cash_withdrawal %ld %d)",card_number,sum);
						var = insert_history(&(p->info->history), str);
						fprintf(out, "%d\n", p->info->balance);
					}
				}
				break;
			}
			p->info = p->info->urm;
		}
		/*refacere lista LSC*/
		if(ant_card == NULL)
			(*aLC)->info = p->info;
		else
			ant_card->urm = p->info;

		if(ok == 1)
			break;
	}	
	/*refacere lista LC*/		
	if(ant == NULL)
		*aLC = p;
	else 
		ant->urm = p;
}


/*depunere numerar in cardul destinatie*/
void transfer_funds_d(ALC aLC, long int card_number_source,long int card_number_dest, int sum)
{

	TLC q = *aLC,  ant = NULL;
	TLSC ant_card = NULL;
	int ok = 0, var;
	char str[100];	
	/*depunere numerar*/
	for(ant = NULL; q != NULL; ant = q, q = q->urm)
	{	
		for(ant_card = NULL; q->info != NULL; ant_card = q->info)
		{
			if((q->info->card_number == card_number_dest))
			{	
				ok = 1;
				q->info->balance = q->info->balance + sum;
				sprintf(str, "(SUCCESS, transfer_funds %ld %ld %d)",
									card_number_source, card_number_dest, sum);
				var = insert_history(&(q->info->history), str);
				break;
			}
			q->info = q->info->urm;
		}
		if(ant_card == NULL)
			(*aLC)->info = q->info;
		else
			ant_card->urm = q->info;

		if(ok == 1)
			break;
	}		

	if(ant == NULL)
		*aLC = q;
	else 
		ant->urm = q;	
}


/*extragere numerar din cardul sursa*/
void transfer_funds_s(ALC aLC, long int card_number_source,
				long int card_number_dest, int sum, FILE* out)
{
	TLC p = *aLC,  ant = NULL;
	TLSC ant_card;
	int ok = 0, var;
	int transfer = 0;
	char str[100];	
	/*extragere numerar sau mesaje de eroare*/
	for(ant = NULL; p != NULL; ant = p, p = p->urm)
	{
		for(ant_card = NULL; p->info != NULL; ant_card = p->info,p->info = p->info->urm)
		{
			if(p->info->card_number == card_number_source)
			{	
				ok = 1;	
				if(sum % 10 != 0)
				{
					fprintf(out,"The transferred amount must be multiple of 10\n");
					sprintf(str, "(FAIL, transfer_funds %ld %ld %d)",
											card_number_source, card_number_dest, sum);
					var = insert_history(&(p->info->history), str);
				}
				else if(p->info->balance < sum)
				{	
					fprintf(out, "Insufficient funds\n");
					sprintf(str, "(FAIL, transfer_funds %ld %ld %d)", 
											card_number_source, card_number_dest, sum);
					var = insert_history(&(p->info->history), str);
				}
				else	
				{
					transfer = 1;
					p->info->balance = p->info->balance - sum;
					sprintf(str, "(SUCCESS, transfer_funds %ld %ld %d)", 
											card_number_source, card_number_dest, sum);		
					var = insert_history(&(p->info->history), str);
					fprintf(out, "%d\n", p->info->balance);
				}
				break;
			}
		}
		/*refacere lista LSC*/
		if(ant_card == NULL)
			(*aLC)->info = p->info;
		else
			ant_card->urm = p->info;

		if(ok == 1)
			break;
	}	
	/*refacere lista LC*/		
	if(ant == NULL)
		*aLC = p;
	else 
		ant->urm = p;
	if(transfer == 1)
		transfer_funds_d(aLC, card_number_source, card_number_dest, sum);
}



int main()
{
	int nr_max_cards, i, ok, poz = 0, sum = 0,
		 max_poz = -1;
	long int card_number, card_number_source, card_number_dest;
	char arguments[100], pin[5];

	TLC LC = (TLC)malloc(sizeof(LC));

	/*deschidere fisiere*/
	FILE* in = fopen("input.in", "rt");
	FILE* out = fopen("output.out", "wt");

	/*obtinere nr_max_cards*/
	fscanf(in, "%d", &nr_max_cards);
	fgetc(in);

	/*creare lista*/
	for(i = 1; i < nr_max_cards; i++)
		ok = insert_lc(&LC, i);

	/*obtinere operatii si efectuarea lor*/
	while(fgets(arguments, 100 * sizeof(char), in) !=0)
	{	
		char arguments2[100];
		strcpy(arguments2, arguments);

		arguments2[(strlen(arguments2)-1)] = '\0';
		char* buffer = strtok(arguments, " ");


		if(strcmp(buffer, "add_card") == 0)	
		{	
			TLSC card = (TLSC)malloc(sizeof(LSC));

			buffer = strtok(NULL, " ");
			card->card_number = atol(buffer);
			buffer = strtok(NULL, " ");
			strcpy(card->pin, buffer);
			buffer = strtok(NULL, " ");
			strcpy(card->expiry_date, buffer);
			buffer = strtok(NULL, " ");
			card->cvv = atoi(buffer);

			poz = number_sum(card->card_number) % nr_max_cards;

			if(poz >= max_poz)
				max_poz = poz;

			if(card_exist(LC, card->card_number) == 1)
				fprintf(out, "The card already exists\n" );
			else 
				add_card(&LC, card, poz, max_poz, out);

			free_lsc(&card);
			strcpy(buffer, "");
		}

		else if(strcmp(buffer, "show") == 0)
		{
			buffer = strtok(NULL, " ");
			card_number = atol(buffer);

			show_card(LC, card_number, out);
		}

		else if(strcmp(arguments2, "show") == 0)
				show(LC, max_poz, out);

		else if(strcmp(buffer, "delete_card") == 0)
		{
			buffer = strtok(NULL, " ");
			card_number = atol(buffer);

			delete_card(&LC, card_number);
		}

		else if(strcmp(buffer, "insert_card") == 0)
		{
			buffer = strtok(NULL, " ");
			card_number = atol(buffer);
			buffer = strtok(NULL, " ");
			strcpy(pin, buffer);
			pin[strlen(pin) - 1] = '\0';

			insert_card(&LC, card_number, pin, out);
		}

		else if(strcmp(buffer, "cancel") == 0)
		{
			buffer = strtok(NULL, " ");
			card_number = atol(buffer);

			cancel(&LC, card_number);
		}

		else if(strcmp(buffer, "unblock_card") == 0)
		{
			buffer = strtok(NULL, " ");
			card_number = atol(buffer);

			unblock_card(&LC, card_number);
		}

		else if(strcmp(buffer, "pin_change")==0)
		{
			buffer = strtok(NULL, " ");
			card_number = atol(buffer);
			buffer = strtok(NULL, " ");
			strcpy(pin, buffer);
			pin[strlen(pin) - 1] = '\0';

			pin_change(&LC, card_number, pin, out);
		}

		else if(strcmp(buffer, "balance_inquiry") == 0)
		{
			buffer = strtok(NULL, " ");
			card_number = atol(buffer);

			balance_inquiry(&LC, card_number, out);
		}

		else if(strcmp(buffer, "recharge") == 0)
		{
			buffer = strtok(NULL, " ");
			card_number = atol(buffer);
			buffer = strtok(NULL, " ");
			sum = atoi(buffer);

			recharge(&LC, card_number, sum, out);
		}

		else if(strcmp(buffer, "cash_withdrawal") == 0)
		{
			buffer = strtok(NULL, " ");
			card_number = atol(buffer);
			buffer = strtok(NULL, " ");
			sum = atoi(buffer);

			cash_withdrawal(&LC, card_number, sum, out);

		}

		else if(strcmp(buffer, "transfer_funds") == 0)
		{
			buffer = strtok(NULL, " ");
			card_number_source = atol(buffer);
			buffer = strtok(NULL, " ");
			card_number_dest = atol(buffer);
			buffer = strtok(NULL, " ");
			sum = atoi(buffer);

			transfer_funds_s(&LC,card_number_source,card_number_dest,sum,out);

		}
	}


	free_lc(&LC);		

	/*inchidere fisiere*/
	fclose(in);
	fclose(out);

	return 0;
}