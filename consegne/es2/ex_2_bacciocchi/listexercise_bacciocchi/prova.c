#include <stdio.h>
#include "list.h"

int main(int argc, char* argv[]) {
    LIST mylist;
    ItemType i;

    mylist = NewList();
    printf("Fase di inizializzazione: Creata lista\n");

    printf("Fase di Controllo: ");
    if (isEmpty(mylist))
        printf(" La lista e' vuota\n");
    else
        printf(" La lista non e' vuota \n");

    printf("Fase di Inserimento dati\n");

    printf("La lista contiene %d elementi\n", getLength(mylist));

    printf("Contenuto della lista: [");
    PrintList(mylist);
    printf("]\n");

    i.value = 1;
    mylist = EnqueueLast(mylist, i);

    printf("La lista contiene %d elementi\n", getLength(mylist));

    i.value = 3.34;
    mylist = EnqueueLast(mylist, i);

    printf("La lista contiene %d elementi \n", getLength(mylist));

    i.value = 0.42;
    mylist = EnqueueLast(mylist, i);

    printf("La lista contiene %d elementi\n", getLength(mylist));

    i.value = 1.2345;
    mylist = EnqueueLast(mylist, i);

    printf("La lista contiene %d elementi\n", getLength(mylist));

    i.value = 2;
    mylist = EnqueueOrdered(mylist, i);

    printf("La lista contiene %d elementi\n", getLength(mylist));
    ItemType itemToFind = {4};
    ItemType* foundItem = Find(mylist, itemToFind);
    if (foundItem) {
        printf("Elemento trovato: %lf\n", foundItem->value);
    } else {
        printf("Elemento non trovato\n");
    }

    printf("Contenuto della lista: [");
    PrintList(mylist);
    printf("]\n");

    printf("Elemento di testa ");
    PrintItem(getHead(mylist));
    printf("\n");

    printf("Elemento di coda ");
    PrintItem(getTail(mylist));
    printf("\n");

    printf("Fase di Estrazione dati\n");

    printf("Estrazione ultimo elemento\n");
    mylist = DequeueLast(mylist);

    printf("Contenuto della lista: [");
    PrintList(mylist);
    printf("]\n");

    printf("Fase di Distruzione dati\n");
    mylist = DeleteList(mylist);

    printf("Fase di Controllo: ");
    if (isEmpty(mylist))
        printf(" La lista e' vuota\n");
    else
        printf(" La lista non e' vuota \n");

    return 0;

} /* Main */
