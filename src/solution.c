#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

#define NUMBER_OF_BODIES 2    
#define XYZ 3*NUMBER_OF_BODIES

#define dT 0.0001 //time difference
#define eps 0.000001 //in case of dividing by 0

/* Array  of particles coordinates: 
*   Ex: coordinates[0] = x0
        coordinates[1] = y0
        coordinates[2] = z0
*/
double coordinates[XYZ];

/* Array  of particles velocities: 
*   Ex: velocities[0] = v(x0)
        velocities[1] = v(y0)
        velocities[2] = v(z0)
*/
double velocities[XYZ];
double grav_forces[XYZ];

double force_down;
double force_up;
int main(){
    clock_t start_t, end_t, total_t;
   start_t = clock();

    /*for(int i = 0; i < XYZ; i+=3){//start velocity of each particle
        velocities[i] = 1; //velocity in direction x
        velocities[i+1] = 0; //velocity in direction y
        velocities[i+2] = 0; //velocity in direction z
    }*/
    velocities[0] = 1;
    velocities[1] = 0;
    velocities[2] = 0;
    velocities[3] = 0;
    velocities[4] = 0;
    velocities[5] = 0;

    coordinates[0] = 0;
    coordinates[1] = 0;
    coordinates[2] = 0;
    coordinates[3] = 5;
    coordinates[4] = 0;
    coordinates[5] = 0;

    double time = dT;
    while(time < 1){
     for(int i = 0; i <XYZ; i+=3){
printf("%d. telo | koordinata X:%10f | hitrostX: %10f | silaX:%10f\n",i/3,coordinates[i], velocities[i], grav_forces[i]);
        for(int j = 0; j < XYZ; j+=3){
            if(i != j){
                
                //we calculate the sum of all particles on a singular particle in its own direction
                force_down = (abs(coordinates[i] - coordinates[j]+eps));
                force_down = force_down*force_down*force_down;
                force_up = coordinates[i] - coordinates[j];
                grav_forces[i]+= force_up/force_down;  //current force on particle i in direction x

                force_down = (abs(coordinates[i+1] - coordinates[j+1]+eps));
                force_down = force_down*force_down*force_down;
                force_up = coordinates[i+1] - coordinates[j+1];
                grav_forces[i+1]+= force_up/force_down; //current force on particle i in direction y

                force_down = (abs(coordinates[i+2] - coordinates[j+2]+eps));
                force_down = force_down*force_down*force_down;
                force_up = coordinates[i+2] - coordinates[j+2];
                grav_forces[i+2]+= force_up/force_down; //current force on particle i in direction z
            }
        }
         
        velocities[i]+=(dT*grav_forces[i]);
        coordinates[i]+=(dT*velocities[i]);
    }

        time+=dT;
    }


       end_t = clock();
          total_t = (double)(end_t - start_t);
                printf("Total time taken: %ld miliseconds\n", total_t);
             
    return 0;
}