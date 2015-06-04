#include <stdio.h>
#include <stdlib.h>
typedef int elemType
typedef struct listNode
{
	elemType data;
	struct listNode *next;
}listNode;

listNode* reverse_list(listNode *head);


int main(int argc, char const *argv[])
{
	listNode *head;
    head = (listNode *)malloc(sizeof(listNode));   
    head->next = NULL;
    int x;
    for(x=0; x<10; x++)
    {
        listNode *p;
        p = (listNode *)malloc(sizeof(listNode)); 
        p->data = x;                    
        p->next = head->next;                 
        head->next = p; 
    }

   	listNode* reversed = reverse_list(head);
	
	while(reversed != NULL){
		printf("%d\n",reversed->data);
		
		reversed = reversed->next;
	}   

	return 0;
}

listNode* reverse_list(listNode *head)
{
	if(head == NULL){
		return NULL;
	}
	listNode *new_list=NULL;

	while(head != NULL){
		listNode *elm = head;
		
		head = head->next;
		
		elm->next = new_list;
		
		new_list = elm;
	}
	return new_list;
}

