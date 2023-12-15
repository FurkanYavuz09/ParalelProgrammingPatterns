#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(int argc, char *argv[])
{
    if (argc!=2){
        printf("Please enter only 1 argument");
        return 1;
    }
    long const double REAL_PI = 4 * atan(1);

    long long int number_of_tosses = strtoll(argv[1], NULL, 10);
    
    long long int number_in_circle = 0;
   
    double itime = omp_get_wtime();
    // Required code for which execution time needs to be computed

#pragma omp parallel 
    {
    //Init Parallelazation with reduction techinue
#pragma omp  for reduction(+: number_in_circle)
        for (int toss = 0; toss < number_of_tosses; toss++) {
           
             
            long double x = (long double)rand()/RAND_MAX*2.0-1.0;//float in range -1 to 1
            long double y = (long double)rand()/RAND_MAX*2.0-1.0;//float in range -1 to 1
           
            if (x*x +y*y<= 1) number_in_circle++;
        }
    }
    
    double ftime = omp_get_wtime();
    
    long double pi_estimate = 4*number_in_circle/(long double)number_of_tosses;

    printf("Pi  : %.12Lf\n", pi_estimate);
    printf(  "Real pi : %.12Lf\n", REAL_PI);
    double exec_time = ftime - itime;

    printf("Time taken is %f seconds\n", exec_time);

    return 0;
}










