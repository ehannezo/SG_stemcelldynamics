//     C++ code; compiling on standard linux, using the command line below. Needs a compiler such as g++.

//    ./clean.sh; g++ -o sim simulation.cpp; ./sim

#define DEBUG
using namespace std;

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <sys/time.h>
#include "MersenneTwister.h"
#include <fstream>
#include <stdio.h>
#include <string>

#include "variables.cpp"

#include "writedata_mean.cpp"
#include "writedata_count.cpp"
#include "writedata_cumul.cpp"

MTRand mtrand1; //random number generator

int main()
{

 // initializing clone size distributions
 for(int p=0;p<T;p++)
 {
    for(int n=0;n<N;n++)
    {
        distrib[n][p]=0;
    }
 }

 for(int m=0;m<runs;m++) //running the simulations many times
 {
   cout << m << endl;
   for(int i=0;i<N;i++)
   {
        for(int j=0;j<N;j++)
        {
            basal_grid[i][j]=0; //grid of basal cells
            supra_grid[i][j]=0; //grid of suprabasal cells
        }
   }
     

    // the first step is to simulate the growth of a SG (from P2 to P7, which will then be evolved)
   int countp=initial_p; // we start with a given number of progenitors at P2
   t=initial_t;// simulation starting
   while(t<homeo_t)
   {
         double temppp=countp;
         for(int k=0;k<countp;k++)
         {
             if(mtrand1()<divrate_P2*dt)
             {
                 double temp=mtrand1();
                 if(temp<r_P+d_P)//symmetric division
                 {
                     temppp+=1;
                 }
                 if(temp>(1-r_P+d_P))// differentiation
                 {
                     temppp-=1;
                 }
             }
         }
       countp=temppp;
       t=t+dt;
     }
     // we store the total size (in basal cell number) of this SG (tot_cells)
     tot_cells=int(countp);

    
     
    // the second step is to stimulate the dynamics of a single clone from P2 to P7 (unconstrained growth, so independent from the global SG dynamics above). We count the basal and suprabasal clone size (countp and countd resp.)

    countp=0;
    countd=0;
    while(countp==0)// while there are basal cells in the clone
    {
        countp=1;
        t=initial_t;
        while(t<homeo_t)	// one Monte-Carlo step
        {
        
        
            double temppp=countp; //temporary variable to store the basal clone size during the loop
            for(int k=0;k<countp;k++)//looping over all basal cells to see if they divide or differentiate
            {
                if(mtrand1()<divrate_P2*dt)
                {
                    double temp=mtrand1();
                    if(temp<r_P+d_P)//symmetric division
                    {
                        temppp+=1;
                    }
                    if(temp>(1-r_P+d_P))// differentiation
                    {
                        temppp-=1;
                        countd+=1;
                    }
            }
        }
        countp=temppp;
        t=t+dt;
     }
    }

     
    // the third step is to populate the SG derived from step 1, with the clone size calculated in step 2
     
    size_x=int(4+floor(1.5*mtrand1()));
    size_y=int(tot_cells/size_x)+int(floor(2*mtrand1()));
    // this makes a grid for the SG, with dimensions size_x and size_z, approximating a total number of cells of tot_cells
     
     
    // we now populate the grid with n_start basal cells in a clone
    int n_start=int(countp);
    if(n_start>size_x*size_y){n_start=size_x*size_y;}
    //randomly initiating a first  cell and labelling it
     
    // we place our "start" number of basal cells randomly on the grid
    int temp_count=0;
    int y_temp=0;
    while(temp_count<n_start)
    {
         ch1=floor(size_y*mtrand1());
         ch2=floor(size_x*mtrand1());
         if(basal_grid[ch1][ch2]==0)
         {//then this location has not been labelled yet, and we label it.
             basal_grid[ch1][ch2]=1;
             temp_count+=1;
         }
    }
    int n_start_d=int(countd);
         // we place our "start_d" number of differentiated cells randomly on the grid
    temp_count=0;
    y_temp=0;
    while(temp_count<n_start_d)
    {
         ch1=floor(size_y*mtrand1());
         ch2=floor(size_x*mtrand1());
         if(supra_grid[int(ch1/ratio_SB)][ch2]==0)
         {//then this location has not been labelled yet, and we label it.
             supra_grid[int(ch1/ratio_SB)][ch2]=1;
             temp_count+=1;
         }
    }

    // the fourth step is now to perform, from P7 onwards, the stochastic clone dynamics on the SG lattice
     
     
    t=homeo_t;
    while(t<T)
    {
	 for(int i=0;i<size_x*size_y;i++)
	 {
      // going over all positions randomly
      ch1=floor(size_y*mtrand1());
      ch2=floor(size_x*mtrand1());
         
      double temp=mtrand1();
      double temp_h=mtrand1();
        
      divrate_cell=divrate_homeo+(divrate_P2-divrate_homeo)*exp(-(t-homeo_t)/30.0);// we smoothly interpolate between the developmental and homeostasis division rate (see section 2.3 of the SI Note)
         
      if(temp<divrate_cell*dt)//symmetric division
        {
            int ch_new1;
            int ch_new2;
            
            ch_new1=int(size_y*mtrand1());
            ch_new2=int(size_x*mtrand1());
            //we pick the cell which will be replaced. Here random, but random nearest-neighbors picking gives very close results, see Extended Data Fig. XX (uncomment the bloacl below to have nearest-neighbor replacement.
            
//            if(mtrand1()>0.5)
//            {
//                // we replace the neighbor along y
//                ch_new2=ch2;
//                if(mtrand1()>0.5)
//                {
//                    // we replace it on the top
//                    ch_new1=int((ch1+1)%size_y);
//                }
//                else
//                {
//                    // we replace it on the bottom
//                    ch_new1=int((ch1-1)%size_y);
//                    if(ch_new1<0)
//                    {
//                        ch_new1=int(ch_new1+size_y);
//                    }
//                }
//            }
//            else
//            {
//                // we replace the neighbor along x
//                ch_new1=ch1;
//                if(mtrand1()>0.5)
//                {
//                    // we replace it on the right
//                    ch_new2=int((ch2+1)%size_x);
//                }
//                else
//                {
//                    // we replace it on the left
//                    ch_new2=int((ch2-1)%size_x);
//                    if(ch_new2<0)
//                    {
//                        ch_new2=int(ch_new2+size_x);
//                    }
//                }
//            }
            
            // we  replace the cell (the dividing one divides in a new position, the removed one goes suprbasally, where it replaces the old suprabasal one). See Fig. XX for a sketch.
            supra_grid[int(ch_new1/ratio_SB)][ch_new2]=basal_grid[ch_new1][ch_new2];
            basal_grid[ch_new1][ch_new2]=basal_grid[ch1][ch2];
        }
	 }//end of the dynamics at time t

    
    // we count the number of differentiated (suprasabal) and progenitor (basal) cells
    countd=0;
    for(int i=0;i<size_y;i++)
    {
        for(int j=0;j<size_x;j++)
        {
            if(supra_grid[i][j]==1){countd+=1;}
        }
    }
	countp=0;
	for(int i=0;i<size_y;i++)
	{
        for(int j=0;j<size_x;j++)
        {
            if(basal_grid[i][j]==1){countp+=1;}
        }
	}

    if(countp!=0)
    {// if there are still basal cells in a clone, we add it to the persistence, mean clone size, distributions etc
        pcount[int(t)]+=1/(runs/dt+0.0);
        distrib[countp][int(t)]+=1;
        mean_p[int(t)]+=countp/(runs/dt);
        mean_d[int(t)]+=countd/(runs/dt);
    }
        
    if(countp==0)
    {
        // no more basal cells= we stop the simulation
        t=T;
    }
        
	t=t+dt;
  }
 }// end of the simulation for that clone
    
    
    
    
    // we now calculate the average clone sizes and their distributions
 for(int i=0;i<T;i++)
 {
        if(pcount[i]>0)
        {
            mean_p[i]/=pcount[i];
            mean_d[i]/=pcount[i];
        }
 }
 for(int i=0;i<T;i++)
 {
        double tempp=0;
        for(int j=0;j<100;j++)
        {
            tempp+=distrib[j][i];
        }
        for(int j=0;j<100;j++)
        {
            distrib[j][i]/=tempp;
        }
        tempp=0;
        for(int j=0;j<100;j++)
        {
            tempp+=distrib[j][i];
            cumul[j][i]=1-tempp;
        }
 }
    


//to measure the mean number of labelled cells
    writedata_mean();
// to measure clonal persistence
 	writedata_count();
// to write distributions
    writedata_cumul();

	cout << endl << "I'm finished!!!!" << endl;
	

	return 0;
		
}
