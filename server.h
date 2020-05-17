
#include <stdio.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#define MAXQUEUE 5

typedef struct plane_details
{
	int id;         /*unique identification for a plane*/
	int time_arr;  /*time of arrival of plane inside the queue*/
	int fuel_rem;  /*fuel remaining in units when joining the queue*/
	
}plane_type;

typedef struct queue_details
{
	int front;
	int rear;
	int size;
	plane_type value[MAXQUEUE];
}Queue_type;

/* ARRIVE = 1*/
/*DEPART = 0*/

int RandomGenerator(int upper);
int FuelLvl(plane_type p);
void QueueInit(Queue_type *ptr);
void AddQueue(plane_type item,Queue_type* ptr);
void DeleteQueue(plane_type* p,Queue_type* ptr);
int Empty(Queue_type* ptr);
int Full(Queue_type* ptr);
void Begin(int *end_time,int* expect_arrive,int* expect_depart);
void NewPlane(plane_type *p,int* planes_proc,int curr_time,int end_time,int action);
void Refuse(plane_type p,int* refused,int action);
void Land(plane_type p,int curr_time,int* land_count,int* land_wait,Queue_type* pl);
void Fly(plane_type p,int curr_time,int* depart_count,int* depart_wait);
void Idle(int curr_time,int* idle);
void Finish(int crash_count,int planes_proc, int land_count, int depart_count,int refused, int land_wait, int depart_wait,int idle, int end_time,Queue_type *pt, Queue_type *pl);

