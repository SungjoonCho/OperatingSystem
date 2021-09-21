// 20170633 cho sung joon

#include<unistd.h>
#include<stdio.h>

int main(){
	int a = 0;
	int i = 0;
	

	for(i = 1; i < 4; i++){
		// call syscall(sys_my_enequeue) - 1~3 enqueue
		syscall(326, i); 
		printf("Enqueue : ");			
		printf("%d\n", i);
	}

	// call syscall(sys_my_enequeue) - try 2 enqueue
	syscall(326, 2); 
	printf("Enqueue : ");			
	printf("2\n");
	
	// call syscall(sys_my_deequeue) - try 3 times
	for(i=0; i<3; i++){
		a = syscall(327); 

		// print dequeued item if it's not empty queue
		if (a != -2 ){
			printf("Dequeue : ");		
			printf("%d\n", a);
		}
		else{
			printf("Queue is empty");
		}
	}
	
	
	return 0;
}