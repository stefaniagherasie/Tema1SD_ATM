# Tema1SD_ATM
[Tema2 Structuri de Date (2018-2019, seria CB)]



Tema presupune realizarea unui program care să simuleze funcționalitatea unui ATM. Astfel se efectueaza gestionarea conturilor si se primesc cereri care oferă clientului posibilitatea de a efectua tranzacții. Enunt: https://acs.curs.pub.ro/2018/pluginfile.php/51630/mod_assign/intro/%5BSD%5D%20Tema%201%20-%202019.pdf?time=1552498583262


#### IMPLEMENTARE
Operatiile care trebuie efectuate sunt citite din fisierul de input, impreuna cu nr_max_cards. Dupa citire acestea sunt apelate si efectuate prin intermediul functiilor corespunzatoare.

Pentru a retine ierarhia de carduri se construiesc 3 structuri de date de tip lista. Tipul LC contine submultimi de carduri create in functie de pozitia lor, avand in structura un camp numit "poz". Tipul LSC contine toate campurile corespunzatoare informatiilor din card, inclusiv lista History care retine istoricul tranzactiilor, adaugandu-se "pin_tries" care tine evidenta numarului incercarilor de introducere a pinului.

S-au creat functii pentru suma cifrelor unui card, testarea pinului(daca e  valid), gasire card intr-o lista LSC, verificare daca cardul exista deja.  Urmeaza functiile de alocare a celulelor de lista si de adaugare a acestora la lista, tinandu-se cont ca pentru LSC si History se face inserare la inceput, iar in LC inserare la sfarsit. La apelarea acestora se initializeaza cu valori standard.

Functia SHOW_LSC afiseaza in fisier informatiile corespunzatoare unui card, inclusiv istoricul, tinandu-se cont de modul de afisare cerut in cerinta. Functia SHOW_CARD afiseaza informatiile despre card la introducerea comenzii show <card_number>. Functia SHOW afiseaza toate cardurile introduse. Avand in  vedere ca lista LC este creata avand toate celulele pana la nr_max_cards alocate deja, este transmis ca paramentru pozitia maxima pana unde au fost  introduse carduri(max_poz), astfel functia stiind cand sa opreasca afisarea.

Functia ADD_CARD parcurge lista LC, oprindu-se cand ajunge in pozitia corecta, dupa care insereaza o celula LSC in care copiaza informatiile despre cardul respectiv. Dupa modificare se reface lista.

Functia DELETE_CARD parcurge lista LC si fiecare submultime asociata, cauta cardul, in cazul in care l-a gasit se refac legaturile din lista, sarind peste celula cardului care trebuie eliminat si se elibereaza memoria.

Functia INSERT_CARD parcurge lista LC si fiecare submultime asociata pana gaseste cardul cautat. Se afiseaza mesajele de eroare daca a fost adaugat pinul gresit, tinand evidenta numarului de incercari(prin static int nr si campul pin_tries din structura). Daca numarul de incercari este mai mare ca 3 se blocheaza cardul si orice operatie care urmeaza este invalida. Daca pinul este introdus intr-un mod corect, se activeaza cardul. Se introduce in memorie operatia.

Functia CANCEL parcurge lista si deconecteaza cardul cautat, adaugand in istoric daca operatie a reusit sau nu.

Functia UNBLOCK_CARD parcurge lista si la gasirea cardului ii schimba acestuia statusul din "BLOCKED" in "ACTIVE", initializand si numarul de incercari de introducere a pinului.

Functia PIN_CHANGE testeaza daca pinul este valid si afiseaza mesajul de eroare in caz contrar, adaugand in istoric operatia. Daca pinul a fost introdus corect se schimba pinul si se introduce in istoric operatia reusita.

Functia BALANCE_INQUIRY parcurge lista si afiseaza soldul curent al cardului introdus. Functia RECHARGE cauta cardul introdus si verifica daca suma introdusa este multiplu de 10, in caz afirmativ adaugand la soldul curent al cardului suma respectiva, afisand mesaje de eroare daca conditia nu e indeplinita. Functia CASH_WITHDRAWAL efectueaza acelasi lucru, dar scazand suma din soldul cardului, testand in plus daca fondurile sunt suficiente. Aceste operatii sunt adaugate de fiecare data in istoric.

Functia TRANSFER_FUNDS_S parcurge lista si cand gaseste cardul destinatie, 
testeaza conditiile pentru efectuarea tranzactiei(suma este multiplu de 10 si 
sunt fonduri suficiente pentru transfer), afisand mesajele de eroare in cazul 
in care nu sunt indeplinite. Scade din soldul cardului suma corespunzatoare si 
adauga in istoric operatie(reusita sau nu).In cazul in care tranzactia s-a 
realizat apeleaza functia TRANSFER_FUNDS_D care scade suma sin soldul cardului 
sursa si afiseaza in istoric operatia reusita.




