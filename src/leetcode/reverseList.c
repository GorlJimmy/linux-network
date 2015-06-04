#include <stdio.h>
#include <stdlib.h>

struct listNode
{
	int val;
	struct listNode *next;
};

struct listNode reverse_list(struct listNode *head);


int main(int argc, char const *argv[])
{
	struct listNode *head;
    head = (struct listNode *)malloc(sizeof(struct listNode));   
    head->next = NULL;
    int x;
    while(scanf("%d",&x) != EOF)
    {
        struct listNode *p;
        p = (struct listNode *)malloc(sizeof(struct listNode)); 
        p->val = x;                    
        p->next = head->next;                 
        head->next = p; 
    }

   	struct listNode reversed = reverse_list(head);
   

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
		
		head->next = elm;
		
		elm->next = new_list;
		
		new_list = elm;
	}
	return new_list;
}