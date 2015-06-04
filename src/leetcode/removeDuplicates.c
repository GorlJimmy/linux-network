#include <stdio.h>


int remove_duplicates(int array[],int n);


int main(void){
	int array[] = {1,2,3,3,3,4,4,5,5,6,7,8,9,10};
	int len = remove_duplicates(array,14);

	printf("The Array length is: %d \n",len);

	int i;
	printf("Target Array is: \n{");
	for(i=0;i<len;i++){
		printf("%d ,",array[i]);
	}
	printf("}");
}

int remove_duplicates(int array[],int n){

	if(0 == n){
		return 0;
	}
	int index = 0, i;
	for(i = 1; i<n; i++){
		if(array[index]!=array[i]){
			array[++index]=array[i];
		}
	}
	return index + 1;
}