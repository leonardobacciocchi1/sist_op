#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //includo la stringa per far funzionare meglio Itemcompare

#include "list.h"

/************* Funzioni locali *********************************/

/* Confronta due elementi della lista:
   - ritorna un valore >0 se item1 > item2;
   - ritorna un valore <0 se item1 < item2;
   - ritorna un valore ==0 se item1 == item2.
  NB: puo' essere utilizzata nelle funzioni di ricerca e/o ordinamento
*/
int itemCompare(ItemType item1, ItemType item2) {
    return strcmp(item1.name,item2.name);
}

/* alloca un nuovo nodo per la lista, copiandone all'interno l'elemento item */
NODE* createNode(ItemType item) {
    NODE* p = (NODE*)malloc(sizeof(NODE));
    assert(p != NULL);

    p->item = item;
    p->next = NULL;
    return p;
}

/* dealloca il nodo p */
void deleteNode(NODE* p) {
    free(p);
}

/********** Funzioni standard *******************************/

/* Inizializza una nuova lista */
LIST NewList() {
    return NULL;
}

/* Azzera la lista */
LIST DeleteList(LIST l) {
    LIST tmp = l;

    /* dealloca tutti i nodi della lista */
    while (!isEmpty(tmp)) {
        NODE* todel = tmp;
        tmp = tmp->next;
        deleteNode(todel);
    }

    /* la lista e' ora vuota, e puo' essere utilizzata di nuovo */
    return NewList();
}

/* ritorna TRUE se la lista e' vuota */
BOOL isEmpty(LIST l) {
    return l == NULL;
}

/* ritorna il numero di elementi presenti nella lista */
int getLength(LIST l) {
    int size = 0;
    LIST tmp = l;

    while (!isEmpty(tmp)) {
        ++size;
        tmp = tmp->next;
    }

    return size;
}

/* Restituisce l'elemento in testa alla lista, senza modificarla */
ItemType getHead(LIST l) {
    assert(!isEmpty(l));
    return l->item;
}

/* Restituisce l'elemento in coda alla lista, senza modificarla */
ItemType getTail(LIST l) {
    NODE* tmp = l;
    assert(!isEmpty(l));

    while (!isEmpty(tmp->next))
        tmp = tmp->next;

    return tmp->item;
}

/* Cerca l'elemento item nella lista e ne restituisce il puntatore
 ritorna NULL se non lo trova */
ItemType* Find(LIST l, ItemType item) {
    LIST tmp = l;
    /*if (tmp == NULL) {
      return NULL;
    }*/
    while (tmp != NULL && itemCompare(tmp->item, item) != 0) {
        tmp = tmp->next;
    }
    if (tmp == NULL) {
        return NULL;
    }
    return &(tmp->item);
}

ItemType* FindFirstAvailable (LIST l , int quantita){
    LIST tmp = l;
    while (!isEmpty(tmp) && tmp->item.num_vaccini < quantita)
    {
        tmp = tmp->next;
    }
    if (!isEmpty(tmp))
    {
        return &tmp->item;
    }
    return NULL;
    
}
ItemType* FindFirstAvailablefornitore(LIST l , int quantita){
    LIST tmp = l;
    while (!isEmpty(tmp)&& tmp->item.min_richieste > quantita)
    {
        tmp = tmp->next;
    }
    if (!isEmpty(tmp))
    {
        return &tmp->item;
    }
    return NULL;
    
}

/* Inserisce un elemento nella prima posizione della lista */
LIST EnqueueFirst(LIST l, ItemType item) {
    assert(FALSE);

    /* TODO */

    return l;
}

/* Inserisce un elemento nell'ultima posizione della lista */
LIST EnqueueLast(LIST l, ItemType item) {
    NODE* new_node = createNode(item);

    if (isEmpty(l)) {
        /* Lista vuota: inserimento in testa */
        l = new_node;
    } else {
        LIST tmp = l;
        while (!isEmpty(tmp->next))
            tmp = tmp->next;

        tmp->next = new_node;
    }

    return l;
}

/* Inserisce un elemento mantenendo la lista ordinata */
LIST EnqueueOrdered(LIST l, ItemType item) {
    NODE* new_node = createNode(item);
    if (l == NULL || l->item.num_vaccini >= item.num_vaccini) {
        new_node->next = l;
        return new_node;  // Inserisce in testa se la lista è vuota o l'elemento va in testa
    }

    NODE* temp = l;
    while (temp->next != NULL && temp->next->item.num_vaccini < item.num_vaccini) {
        temp = temp->next;
    }

    new_node->next = temp->next;
    temp->next = new_node;  // Inserisce l'elemento nella posizione corretta

    return l;
}

/* Toglie il primo elemento della lista (se non e' vuota) */
LIST DequeueFirst(LIST l) {
    if (!isEmpty(l)) {
        NODE* todel = l;
        l = l->next;
        deleteNode(todel);
    }
    return l;
}

/* Toglie l'ultimo elemento della lista (se non e' vuota) */
LIST DequeueLast(LIST l) {
    if (!isEmpty(l)) {
        assert(FALSE);

        /* TODO */
    }

    return l;
}

/* Toglie l'elemento item dalla lista (se e' presente) */
LIST Dequeue(LIST l, ItemType item) {
    if (!isEmpty(l)) {
        if (itemCompare(l->item, item) == 0) {
            /* eliminazione dell'elemento in testa */
            NODE* todel = l;
            l = l->next;
            deleteNode(todel);
        } else {
            LIST tmp = l;

            while (!isEmpty(tmp->next) &&
                   itemCompare(tmp->next->item, item) != 0)
                tmp = tmp->next;

            if (!isEmpty(tmp->next)) {
                /* l'elemento e' stato trovato nella lista (viene eliminato) */
                NODE* todel = tmp->next;
                tmp->next = tmp->next->next;
                deleteNode(todel);
            }
        }
    }
    return l;
}

/* Stampa a video un elemento della lista */
void PrintItem(ItemType item) {
    printf("Nome: %s, num vaccini: %d, min_richiesta: %d, Socket: %d\n", item.name, item.num_vaccini, item.min_richieste,item.socket);
}

/* Stampa a video la lista (esegue PrintItem() su tutti gli elementi) */
void PrintList(LIST l) {
    LIST tmp = l;
    while (!isEmpty(tmp)) {
        PrintItem(tmp->item);
        tmp = tmp->next;

        if (!isEmpty(tmp))
            printf(" ");
    }
}