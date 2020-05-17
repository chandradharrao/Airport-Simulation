
#include <stdio.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include"server.h"

/*****return a random number generaotor**********/
int RandomGenerator(int upper)
{
	int num;
	int lower = 0;
	num = (rand() % (upper - lower + 1)) + lower; 
	return num;
}
/*return the fuel remaining in the plane*/
int FuelLvl(plane_type p)
{
	return p.fuel_rem;
}

/*initializing the queue*/
/*empty queue is indicated by:
rear == -1
front == -1
*/
void QueueInit(Queue_type *ptr)
{
	ptr->front = -1;
	ptr->rear = -1;
	ptr->size = -1;
}

/*add an item into the queue*/
void AddQueue(plane_type item,Queue_type* ptr)
{
	//printf("\nThe value of front before adding is %d\n",ptr->front);
	//printf("\nThe value of rear before adding is %d\n",ptr->rear);
	if (ptr->size < MAXQUEUE) /*if the queue is not yet full*/
	{
		if (ptr->size<0) /*first elemnt of queue*/
		{
			ptr->value[0] = item;
			ptr->size = 1;
			ptr->front = ptr->rear = 0;
		}
		else
		{
			ptr->value[(ptr->rear+1)%MAXQUEUE] = item;
			ptr->rear = (ptr->rear + 1)%MAXQUEUE;
			ptr->size++;
		}
	}
	else
	{
		printf("\nQueue full\n");
	}
}

/*delete the front item of the queue and return it*/
void DeleteQueue(plane_type* p,Queue_type* ptr)
{
	if (ptr->size < 0)
	{
		printf("\nQueue is Empty\n");
	}
	else
	{
		//printf("\nThis is the value of ptr front %d\n",ptr->front);
		//printf("\nThis is the value of ptr rear %d\n",ptr->rear);
		(*p) = ptr->value[ptr->front]; /*pop the front element of the queue and store it in p*/
		if (ptr->front != ptr->rear)
		{	
			(ptr->size)--;
			(ptr->front) = (ptr->front + 1)%MAXQUEUE;
		}
		else
		{
			ptr->front = -1;
			ptr->rear = -1;
			ptr->size = -1;
		}
		//printf("\nThis is the value of ptr front after deletion %d\n",ptr->front);
		//printf("\nThis is the value of ptr rear after delttion %d\n",ptr->rear);
		//printf("\nThe size of ptr after deletion is %d\n",ptr->size);
	}
}

/*check whether the Queue is empty*/
int Empty(Queue_type* ptr)
{
	if (ptr->size < 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*check if the que is full*/
int Full(Queue_type* ptr)
{
	if (ptr->size == MAXQUEUE)
	{
		return 1;
	}
	else if (ptr->size < MAXQUEUE)
	{
		return 0;
	}
}

/*Gneberate random numbers*/
void Begin(int *end_time,int* expect_arrive,int* expect_depart)
{
	int ok;
	do
	{
	printf("\nThis program simulates an airport with one runway\n");
	printf("\nA plane can land or depart in each unit of time\n");
	printf("\nUpto %d planes can be waiting to land or to take off\n",MAXQUEUE);
	printf("\nFor how many units of time should the simulation run?\n");
	scanf("%d",end_time);
	printf("\nExpected number of plane arrivals per unit time\n");
	scanf("%d",expect_arrive);
	printf("\nExpected number of plane departures per unit time\n");
	scanf("%d",expect_depart);
	if (*expect_arrive<0 || *expect_depart<0)
	{
		printf("\nEnter non-negetive numbers\n");
		ok = 0;
	}
	else if (*expect_arrive + *expect_depart > 10)
	{
		printf("\nThe airport will become saturated\nDo you want to read new numbers?0 for yes and 1 for no\n");
		/*******if user says yes,repeat the loop**********/
		scanf("%d",&ok);
	}
	else
	{
		ok = 1;
	}
	}while (ok == 0);
}

/****makes a record for a new plane********/
void NewPlane(plane_type *p,int* planes_proc,int curr_time,int end_time,int action)
{
	(*planes_proc)++;
	p->id = (*planes_proc);
	p->time_arr = curr_time;
	p->fuel_rem = RandomGenerator(end_time);
	if (action == 1)
	{
		printf("\nPlane %d ready to land\n",*planes_proc);
	}
	else if (action == 0)
	{
		printf("\nPlane %d ready to take off\n",*planes_proc);
		
	}
}

/*Processing a plane when th queue is full*/
void Refuse(plane_type p,int* refused,int action)
{
	if (action == 1)
	{
		printf("\nPlane %d directed to another airport\n",p.id);
	}
	else if (action == 0)
	{
		printf("\nPlane %d is instructed to try departing later\n",p.id);
	}
	(*refused)++;
	//printf("\nTHE VALUE OF REFUSE COUNT %d\n",*refused);
}

/*processing plane p that is actually landing*/
void Land(plane_type p,int curr_time,int* land_count,int* land_wait,Queue_type* pl)
{
	//printf("\nLAND() IS CALLED\n");
	int wait;
	int temp;
	wait = curr_time - p.time_arr;
	printf("\n%d : Plane %d landed\n",curr_time,p.id);
	printf("\nPlane %d Waited for %d units of time in Landing Queue\n",p.id,wait);
	(*land_count)++;
	*land_wait +=wait;
	if (Empty(pl) == 0)
	{
		temp = (pl->front);
		//printf("\nTHE VALE OF TEMP is %d\n",temp);
		//printf("\nTHE VALUE OF REAR is %d\n",(pl->rear));	
		do
		{
			//printf("\nINSIDE FUEL REDUCER LOOP\n");
			(pl->value[temp]).fuel_rem = (pl->value[temp]).fuel_rem -1;
			//printf("\nAmnt of fuel in plane %d in the landing queue is %d\n",(pl->value[temp]).id,(pl->value[temp]).fuel_rem);
			temp = (temp + 1)%MAXQUEUE;
			
		}while ((temp != ((pl->rear)+1)%MAXQUEUE));
			
	}
}

/*processing a plane that is actualy taking off*/
void Fly(plane_type p,int curr_time,int* depart_count,int* depart_wait)
{
	int wait;
	wait = curr_time - p.time_arr;
	printf("\n%d : Plane %d took off\n",curr_time,p.id);
	printf("\nPlane %d Waited for %d units of time in Takeoff Queue\n",p.id,wait);
	(*depart_count)++;
	(*depart_wait) +=wait;
}

/*updates idle runway variable*/
void Idle(int curr_time,int* idle)
{
	printf("%d : Runaway is idle.\n",curr_time);
	(*idle)++;
}

/*Finishing out the simulation*/
void Finish(int crash_count,int planes_proc, int land_count, int depart_count,int refused, int land_wait, int depart_wait,int idle, int end_time,Queue_type *pt, Queue_type *pl)

{
    printf("\nSimulation has concluded after %d units.\n", end_time);
    printf("\nTotal number of planes processed: %d\n", planes_proc);
    printf("\nNumber of planes landed: %d\n", land_count);
    printf("\nNumber of planes taken off: %d\n", depart_count);
    printf("\nNumber of planes refused: %d\n", refused);
    if (pl->size < 0 )
    {
    	printf("\nNumber left ready to land: %d\n", 0);
    }
    else
    {
    	printf("\nNumber left ready to land: %d\n", pl->size);
    }
    if (pt->size < 0)
    {
    	printf("\nNumber left ready to take off: %d\n", 0);
    }
    else
    {
    	printf("\nNumber left ready to take off: %d\n", pt->size);
    }
    if (pl->size < 0 )
    {
    	printf("\nNumber of Crashes: %d\n", 0);
    }
    else
    {
    	printf("\nNumber of Crashes: %d\n", crash_count);
    }

    if (end_time > 0)
    {
        printf("\nPercentage of time runway idle: %f\n",((float) idle / end_time) * 100.0);
    }
    if (land_count > 0)
    {
        printf("\nAverage wait time to land: %f\n",(float) land_wait / land_count);
    }
    if (depart_count > 0)
    {
        printf("\nAverage wait time to take off: %f\n",(float) depart_wait / depart_count);
    }
}
