// 20170633 cho sung joon

#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/linkage.h>
#define MAXQUEUE 100

int queue[MAXQUEUE];
int f = 0;
int r = 0;
int i;
int res = 0;

asmlinkage void sys_my_enqueue(int n){

    // if queue is full
    if (r >= MAXQUEUE - 1){
		printk(KERN_CRIT "Queue is full");
        return ;
    }
    
    // check if there is a same value in queue
   
    for (i = f; i < r; i++){
        if (n == queue[i]){
            printk(KERN_CRIT "Same number in queue. Enqueue Failed : %d\n", n);    
            return ;
        }
    }
    

    // if n can be enqueued
    printk(KERN_CRIT "Enqueue : %d\n", n);

    queue[r] = n;
    r = r + 1;
    
    // print all of items in queue
    printk(KERN_CRIT "Queue : "); 
    for(i = f; i < r; i++){
        printk(KERN_CRIT " %d ", queue[i]);
    }
    printk(KERN_CRIT " \n"); 

	return ;
}

asmlinkage int sys_my_dequeue(void){
    // if queue is empty
    if (r == f){
        printk(KERN_CRIT "Queue is empty\n");
        return -2;
    }
   

    // pop first value in queue
    res = queue[f];

    // if dequeue can be possible
    printk(KERN_CRIT "Dequeue : %d\n", res);

    // pull items one by one
    for(i = f + 1; i < r; i++){
        queue[i-1] = queue[i];
    }
    r = r - 1;

    // print all of items in queue
    printk(KERN_CRIT "Queue : ");
    for(i = f; i < r; i++){
        printk(KERN_CRIT " %d ", queue[i]);
    }
    printk(KERN_CRIT " \n"); 

    return res;
}
