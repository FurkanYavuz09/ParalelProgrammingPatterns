#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>


void maximumConsecutiveSum(int arr[], int size) {
    int s[size], m[size], sm[size], indx[size];
    
    //  Calculate prefix sums (s)
    s[0] = arr[0];
    
    for (int i = 1; i < size; i++) {
        
            s[i] = s[i - 1] + arr[i];
    }
    
    // Calculate prefix mins (m)
    m[0] = s[0];
    
    for (int i = 1; i < size; i++) {
        
            m[i] = (s[i] < m[i - 1]) ? s[i] : m[i - 1];
    
    }
    
    // Calculate sm[j] = s[j] - m[j-1]
    // sm can be calculated parallel because there is no dependency between sm and s, m  when calculating sm[j]
    sm[0] = s[0] - m[0];
    #pragma omp parallel for
    for (int j = 1; j < size; j++) {
        #pragma omp critical
        {
            sm[j] = s[j] - m[j - 1];
        }
    }
    
    // Calculate indx[j] = max{i: 0 <= i <= j and s[i] = m[j]}
    indx[0] = 0;
    int maxIndex = 0;
   
    for (int j = 1; j < size; j++) {
        if (s[j] == m[j]) {
            
                maxIndex = j;
            
        }
        indx[j] = maxIndex;
    }
    
    // Find the maximum consecutive sum (MCS) and its indices
    int MCS = sm[0];
    int startIndex = maxIndex + 1;
    int endIndex = 0;
    
    
    for (int j = 1; j < size; j++) {
        if (sm[j] > MCS) {
            
                if (sm[j] > MCS) {
                    MCS = sm[j];
                    startIndex = indx[j] + 1;
                    endIndex = j;
                }
            
        }
    }
    
    printf("Maximum Consecutive Sum (MCS): %d\n", MCS);
    printf("Indices: [%d, %d]\n", startIndex, endIndex);
}


void gen_random_numbers(int *array, int len, int min, int max){
    for (int i = 0; i < len; i++){
        array[i] = rand() % (max - min + 1) + min;
        //printf("%d,", array[i]);
    }
}



int main(int argc, char *argv[]) {
    
    if (argc!=2){
        printf("Please enter only 1 argument");
        return 1;
    }
    

    int arr[atoi(argv[1])];
    gen_random_numbers(arr,atoi(argv[1]), -100, 100);
    

    long size = sizeof(arr) / sizeof(arr[0]);
    
    double itime = omp_get_wtime();
    // Required code for which execution time needs to be computed

    maximumConsecutiveSum(arr, size);

    double ftime = omp_get_wtime();
    
    double exec_time = ftime - itime;

    printf("Time taken is %f seconds\n", exec_time);
    
    
    return 0;
}
