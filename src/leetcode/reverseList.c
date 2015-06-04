#include <stdio.h>
#include <stdlib.h>

struct listNode
{
	int val;
	struct listNode *next;
};

struct listNode reverse_list(struct list *head);


int main(int argc, char const *argv[])
{
	listNode *head;
    head = (listNode *)malloc(sizeof(listNode));   
    head->next = NULL;                      
    int x;
    while(scanf("%d",&x) != EOF)
    {
        listNode *p;
        p = (listNode *)malloc(sizeof(listNode)); 
        p->val = x;                    
        p->next = head->next;                 
        head->next = p; 
    }

   	listNode *reversed=reverse_list(head);
   	while(reversed!=NULL){
   		struct listNode *elm = reversed->next;
   		reversed=reversed->next;
   		printf("%d\n", elm->val);
   	}

	return 0;
}

struct listNode reverse_list(struct list *head)
{
	if(head == NULL){
		return NULL;
	}
	struct listNode *new_list=NULL;

	while(head! = NULL){
		struct listNode *elm = head;
		
		head->next = elm;
		
		elm->next = new_list;
		
		new_list = elm;
	}
	return new_list;
}