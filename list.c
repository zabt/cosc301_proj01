#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* your list function definitions */
void list_deconstruct(struct node **head) { //free up memory from linked list
	struct node *temp;
	while(head != NULL){
		temp = *head;
		free(*head);
		if((temp->next)==NULL){
			return;
		}
		head = &(temp->next);
	}
}

void list_print(const struct node *list) { //prints values in linked list, formed from code by Professor Sommers
	while (list != NULL) {
		printf("%d\n", list->value);
		list = list->next;
	}
}

void list_append(int value, struct node **head) { //appends node to end of linked list
	struct node *new = malloc(sizeof(struct node));
	new->value = value;
	new->next = NULL;
	if (*head == NULL) {
		*head = new;
		return;
	}
	struct node *thisnode = *head;
	while (thisnode->next != NULL) {
		thisnode = thisnode->next;
	}
	thisnode->next = new;
}

void list_sort(struct node **head) { //sorts linked list, least to greatest
	if ((*head == NULL)||((*head)->next == NULL)) {
		return;
	}
	int sorted = 0;

	while (!sorted){
		sorted = 1;
		struct node *current = *head;
		struct node *next = *head;
		next = next->next;
		struct node *previous = *head;
		struct node *temp;
		while (next != NULL){
			if ((current->value)>(next->value)) {
				if (current == *head){
					*head = next;
				}
				temp = next->next;
				previous->next = next;
				previous = next;
				next->next = current;
				next = temp;
				current->next = next;
				sorted = 0;
			}else{
				previous = current;
				current = next;
				next = current->next;
			}
		}
	}
}
