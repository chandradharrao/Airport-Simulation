
#include <stdio.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include"server.h"
void main(void)
{
	Queue_type landing;
	Queue_type takeoff;
	Queue_type* pl = &landing;
	Queue_type* pt = &takeoff;
	plane_type plane;
	int curr_time = 1; /*current timeie one unit for landing or taking off*/
	int end_time; /*total units for running the simulation*/
	int expect_arrive; /*no of planes arriving in one unit*/
	int expect_depart; /*number off planes ready to take off*/
	int count; /*loop cotrol variable*/
	int idle; /*units of time the run-way is idle*/
	int land_count; /*number of planes landed so far*/
	int land_wait; /*total time waited by planes landed*/
	int planes_proc; /*number of planes processed so far*/
	int refused; /*number of planes refused to join the queue*/
	int depart_count; /*number of planes taken off*/
	int randi; /*pseudo random number we are going to generate*/
	int depart_wait; /*total waiting time before takeoff*/
	int crash_count; /*count the number of crashes in simulation*/
	
	/*******************initialization of variables*********************/
	QueueInit(pl);
	QueueInit(pt);
	planes_proc = land_count = depart_count = refused = 0;
	land_wait = depart_wait = idle = 0;
	crash_count = 0;
	srand(time(0)); /*initializing the seed for random number generation*/
	
	Begin(&end_time,&expect_arrive,&expect_depart);
	while (curr_time<=end_time)
	{
		/********new planes that are ready to land***********/
		
		randi = RandomGenerator(expect_arrive);
		//printf("\nTHIS IS RANDOM ARRIVE %d\n",randi);
		for (count = 1;count<=randi;count++)
		{
			/*******adding to landing queue***********/
			NewPlane(&plane,&planes_proc,curr_time,end_time,1);
			//printf("\nTHIS IS PLANE ID %d \n",plane.id);
			//printf("\nTHIS IS FUEL LEVEL OF NEW PLANE %d\n",plane.fuel_rem);
			
			if (Full(pl) == 1)
			{
				Refuse(plane,&refused,1);
			}
			else if (Full(pl) == 0 && FuelLvl(plane) > 1)
			{
				AddQueue(plane,pl);
			}
			else if (Full(pl) == 0 && FuelLvl(plane) == 1)
			{
				//printf("\nGOING TO call LAND()\n");
				Land(plane,curr_time,&land_count,&land_wait,pl);
				curr_time = curr_time + 1;
			}
			else if (Full(pl) == 0 && FuelLvl(plane) <=0)
			{
				crash_count++;
				//printf("\nThis is the crash count %d\n",crash_count);
			}
			//printf("THIS IS QUEUE SIZE OF LANDING %d\n",pl->size);
		}
		
		/*******new planes ready to take off*************/
		randi = RandomGenerator(expect_depart);
		//printf("\nTHIS IS RANDOM DEPART %d\n",randi);
		for (count = 1;count<=randi;count++)
		{
			/****adding to take off queue*****/
			NewPlane(&plane,&planes_proc,curr_time,end_time,0);
			
			if (Full(pt) == 1)
			{
				Refuse(plane,&refused,0);
			}
			else
			{
				AddQueue(plane,pt);
			}
			//printf("THIS IS QUEUE SIZE OF takeoff %d\n",pt->size);
		}
			/****plane landing******/
		//printf("\nTHIS IS SIZE OF PL %d\n",pl->size);
		if (pl->size>0) 
		{
			/*****bring plane to landing******/
			int perform = 1;	
				while (perform == 1 && (pl->size) > 0 ) //Empty(pl) == 0
				{
					//printf("\nInside the while loop\n");
					DeleteQueue(&plane,pl);
					
					//printf("\nThe plane deleted from the queue is planeid: %d\n",plane.id);
					if (plane.fuel_rem <= 0)
					{
						crash_count++;
						//printf("\nThis is the crash count %d\n",crash_count);
					}
					else
					{
						perform = 0;
						Land(plane,curr_time,&land_count,&land_wait,pl);
						//printf("\nThe size of queue after deletion is %d\n",pl->size);
						//printf("\nTHe VALE of PErFORM is %d\n",perform);
					}
				}
		}
			/******************plane takeoff**************/
		if ((pl->size<0) && (pt->size>0))
		{
			/******allow plane to take off*************/
			//printf("\nPlane taking off\n");
			DeleteQueue(&plane,pt);
			Fly(plane,curr_time,&depart_count,&depart_wait);
		}
		/**********run way idle*************/
		if ((pl->size)<0 && (pt->size)<0)
		{
			Idle(curr_time,&idle);
		}
		curr_time = curr_time + 1;
	}
	
	/*****************Finish the simulation************************/
	Finish(crash_count,planes_proc,land_count,depart_count,refused,land_wait,depart_wait,idle,end_time,pt,pl);
}
