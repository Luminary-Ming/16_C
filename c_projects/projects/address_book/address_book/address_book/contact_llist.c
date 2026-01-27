#include "contact_llist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LLIST *llist_create(int size)
{}

int llist_insert(LLIST *handler, Contact *contact, int mode)
{}

void llist_display(LLIST *handler, llist_contact_print contact_print)
{}

Contact *llist_find(LLIST *handler, int id)
{}

int llist_get_count(LLIST *handler)
{}

Contact *llist_get_at(LLIST *handler, int index)
{}


void llist_destroy(LLIST *handler)
{}

int llist_delete(LLIST *handler, int id)
{}