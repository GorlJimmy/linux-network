#include <stdio.h>
#include <stdlib.h>
typedef int elemType
typedef struct listNode
{
	elemType data;
	struct listNode *next;
}listNode;

struct listNode* reverse_list(struct listNode *head);


int main(int argc, char const *argv[])
{
	struct listNode *head;
    head = (struct listNode *)malloc(sizeof(struct listNode));   
    head->next = NULL;
    int x;
    for(x=0; x<10; x++)
    {
        struct listNode *p;
        p = (struct listNode *)malloc(sizeof(struct listNode)); 
        p->data = x;                    
        p->next = head->next;                 
        head->next = p; 
    }

   	struct listNode* reversed = reverse_list(head);
	
	while(reversed != NULL){
		printf("%d\n",reversed->data);
		
		reversed = reversed->next;
	}   

	return 0;
}

struct listNode* reverse_list(struct listNode *head)
{
	if(head == NULL){
		return NULL;
	}
	struct listNode *new_list=NULL;

	while(head != NULL){
		struct listNode *elm = head;
		
		head = head->next;
		
		elm->next = new_list;
		
		new_list = elm;
	}
	return new_list;
}

