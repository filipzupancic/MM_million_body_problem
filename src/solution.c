 
#include <math.h> 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "helper.h"

#define dT 1e-4 //time step

/*
    variable counter shall be used as counter of processing time, every 100th process will
    have  processing time written out
*/
int counter=0;

double F[XYZ];

double generate_rand(int min, int max)
{
    return min + (double)rand() / (double) (RAND_MAX) * (max - min + 1);
}

void generate_parameters()
{
    masses[0] = 10000000;
    for (int i = 1; i < NUMBER_OF_BODIES; i++)
    {
        masses[i] = 1;
    }

    //our blackhole will have its center in (0,0,0)
    coordinates[0] = 0;
    coordinates[1] = 0;
    coordinates[2] = 0;
    
    velocities[0] = 0;
    velocities[1] = 0;
    velocities[2] = 0;

    int i = 3; 
    while (i < XYZ)
    {
        //random coordinates
        double x = generate_rand(-R, R);
        double y = generate_rand(-R, R);
        double z = generate_rand(-H, H);

        if (pow(x, 2) + pow(y, 2) < pow(R, 2) && pow(x, 2) + pow(y, 2) > pow(R-R_min, 2))
        {
            
            coordinates[i] = x;
            coordinates[i+1] = y;
            coordinates[i+2] = z;
            
            double r = sqrt(x*x + y*y + z*z);
            double v = sqrt(masses[0]/r);

            //vx = y/r
            velocities[i] = -v * coordinates[i+1]/r;
            //vy = x/r
            velocities[i+1] = v * coordinates[i]/r;
            velocities[i+2] = 0;

            i+=3;
        }  
    }
}

void compute_coordinates()
{   
    clock_t begin = clock();
    for(int i = 0; i <  XYZ; i++)
    {
        //forces at the beggining are all set to 0
        F[i] = 0;
    }

        //parallel processing syntax
        #pragma omp parallel
        #pragma omp for
    for(int i = 0; i < XYZ; i+=3){
        double dx,dy,dz,D,F1,F2,F3;
        for(int j = i+3; j < XYZ; j+=3){
            //calculating distances in all 3 dimensions
            dx = coordinates[i] - coordinates[j];
            dy = coordinates[i+1] - coordinates[j+1];
            dz = coordinates[i+2] - coordinates[j+2];
            D = sqrt(dx*dx + dy*dy + dz*dz);
            D = pow(D,3);

            //calculating current force in every dimension
            F1 = (dx/(D+eps))*masses[i/3]*masses[j/3];
            F2 = (dy/(D+eps))*masses[i/3]*masses[j/3];
            F3 =  (dz/(D+eps))*masses[i/3]*masses[j/3];


            //Newton's second law: F21 = -F12;
            F[i]-=F1;
            F[i+1]-=F2;
            F[i+2]-=F3;
        
            F[j]+=F1;
            F[j+1]+=F2;
            F[j+2]+=F3;

        }
    }
    for(int i = 0; i < XYZ; i+=3){
        //we user Euler's method- numerical method for solving ordinary differential equations
        // first, we update the newly made coordinates
        coordinates[i]+=(velocities[i]*dT);
        coordinates[i+1]+=(velocities[i+1]*dT);
        coordinates[i+2]+=(velocities[i+2]*dT);
        //second, we update velocities
        velocities[i]+=((F[i]/masses[i/3])*dT); 
        velocities[i+1]+=((F[i+1]/masses[i/3])*dT);
        velocities[i+2]+=((F[i+2]/masses[i/3])*dT);
    }
    counter++;
clock_t end = clock();
        double time_spent = (double)(end - begin);

        if(counter%100 == 0){
        printf("Time spent: %f\n",time_spent);
        }
        
}

int main(int argc, char *argv[])
{

    generate_parameters();

    visualize(argc, argv);
    return 0;
}
