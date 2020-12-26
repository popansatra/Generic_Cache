#include "cache.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int main(int argc, char *argv[]) {
	
	unsigned int bs;
	unsigned int l1_cs;
	unsigned int l1_a;
	unsigned int v_cs;
	unsigned int v_a;
	unsigned int l2_cs;
	unsigned int l2_a;
	double rp;
	char *tf;
	unsigned int wp = 0;
	float aat =0;
		
	bs 	= atoi (argv[1]);
	l1_cs 	= atoi (argv[2]);
	l1_a 	= atoi (argv[3]);
	v_cs	= atoi (argv[4]);
	l2_cs 	= atoi (argv[5]);
	l2_a 	= atoi (argv[6]);
	rp 	= atof (argv[7]);
	tf	=	argv[8];
	v_a 	= v_cs/bs;	


	cache l1(bs,l1_cs,l1_a,rp,wp,1);
	cache l2(bs,l2_cs,l2_a,rp,wp,2);
	cache v(bs,v_cs,v_a,2,wp,0);

        if(l2_cs!=0){
                l1.next = &l2;
                l2.next = NULL;

                if(v_cs!=0){
                        l1.vptr = &v;
                        v.next  = &l2;
                }
        }
        else{
                l1.next = NULL;
		if(v_cs!=0){
                      	l1.vptr = &v;
			v.next=NULL;
		}
		v.next=NULL;
	}
	char rw;
	unsigned int addr;
	FILE *f;
	f = fopen (tf,"r");


	while(!feof(f))
	{

		fscanf (f,"%c %x\n",&rw,&addr);

		if(rw == 'r'){
			l1.read(addr);
		}
		else if(rw == 'w'){
			l1.write(addr);
		}	
		
	}
	printf("  ===== Simulator configuration =====\n");
	printf("  L1_BLOCKSIZE:                  %d\n",bs);
	printf("  L1_SIZE:                      %d\n",l1_cs);
	printf("  L1_ASSOC:                     %d\n",l1_a);
	printf("  Victim_Cache_SIZE:            %d\n",v_cs);
	printf("  L2_SIZE:                      %d\n",l2_cs);
	printf("  L2_ASSOC:                     %d\n",l2_a);
	printf("  trace_file:                   %s\n",tf);
        if(rp== 2){ printf("  Replacement Policy:           LRU\n");}
        else if(rp == 3){ printf("  Replacement Policy:           LFU\n");}
        else { printf("  Replacement Policy:           LRFU\n");}
	printf("\n  ===================================\n");

	if(l1_cs!=0){
		printf("\n===== L1 contents ===== \n");
		l1.print_tag_store();
	}
	

	if (v_cs!=0){
		printf("===== Victim Cache contents ===== \n");
		v.print_tag_store();
		
	}
	if(l2_cs!=0){
		printf("===== L2 contents ===== \n");
		l2.print_tag_store();
	}
	


	printf("\n%s","====== Simulation results (raw) ======\n\n");

	l1.print_statistics();
	if(v_cs==0)
	{
		printf("g. number of victim cache writeback: 0\n");
	}
	v.print_statistics();
	if(l2_cs==0)
	{
                printf("h. number of L2 reads: 0\n");
                printf("i. number of L2 read misses: 0\n");
                printf("j. number of L2 writes: 0\n");
                printf("k. number of L2 write misses: 0\n");
                printf("l. L2 miss rate: 0\n");
                printf("m. number of L2 writeback: 0\n");
                printf("n. total memory traffic: %d\n",l1.tot_mem_traffic()+v.tot_mem_traffic());
	}
	
	else
	{
	l2.print_statistics();
	}

	l1.hit_time = 0.25 + 2.5 * ((float)l1.cs / (512*1024)) + 0.025 * ((float)l1.bs / 16.0) + 0.025 * (float)l1.a;		
	l2.hit_time = 2.5 + 2.5 * ((float)l2.cs / (512*1024)) + 0.025 * ((float)l2.bs / 16.0) + 0.025 * (float)l2.a;		
	l1.mp = 20 + 0.5 * ((float)l1.bs / 16);
	l2.mp = 20 + 0.5 * ((float)l2.bs / 16);
	
	
	if(l2_cs!=0)
		aat = (float)l1.hit_time + ((float)l1.mr *((float)l2.hit_time + (float)l2.mr * (float)l2.mp));
	else
		aat = (float)l1.hit_time + ((float)l1.mr * (float)l1.mp);


	
	printf("\n==== Simulation results (performance) ====\n");
	printf("%s %.4f %s","1. average access time:",aat,"ns\n");



}
