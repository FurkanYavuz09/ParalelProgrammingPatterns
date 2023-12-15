#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"
#include <time.h>
#include <omp.h>

void blurImage(const unsigned char* inputImage, unsigned char* outputImage, int width, int height)
{
    // 5x5 Gaussian blur kernel
    int kernel[5][5] = {
        {1,  4,  7,  4, 1},
        {4, 16, 26, 16, 4},
        {7, 26, 41, 26, 7},
        {4, 16, 26, 16, 4},
        {1,  4,  7,  4, 1}
    };

    #pragma omp parallel for collapse(2)
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            // Calculate the weighted sum of the pixel and its neighbors
            int sum = 0;
            for (int i = -2; i <= 2; i++)
            {
                for (int j = -2; j <= 2; j++)
                {
                    int neighborRow = row + i;
                    int neighborCol = col + j;

                    // Apply boundary policy: clamp to image bounds
                    neighborRow = (neighborRow < 0) ? 0 : neighborRow;
                    neighborRow = (neighborRow >= height) ? height - 1 : neighborRow;
                    neighborCol = (neighborCol < 0) ? 0 : neighborCol;
                    neighborCol = (neighborCol >= width) ? width - 1 : neighborCol;

                    // Apply the kernel weight to the pixel value
                    sum += inputImage[neighborRow * width + neighborCol] * kernel[i + 2][j + 2];
                }
            }

            // Normalize the sum and update the output pixel value
            outputImage[row * width + col] = sum / 273;
        }
    }
}

int main(void)
{
    int width, height, channels;

    
    unsigned char *img = stbi_load("input.png", &width, &height, &channels, 0);
    //channels = 1;

    //printf("%d",channels);
    if (img == NULL)
    {
        printf("Error in loading the image\n");
        exit(1);
    }

    unsigned char *img_out = (unsigned char *)malloc(width * height * channels);

    if (img_out == NULL)
    {
        printf("Error in allocating memory for the output image\n");
        exit(1);
    }

    double itime = omp_get_wtime();
    // Required code for which execution time needs to be computed


   #pragma omp parallel
    {
        #pragma omp single
        {
            blurImage(img, img_out, width, height);
        }
    }

    double ftime = omp_get_wtime();
    
    double exec_time = ftime - itime;

    printf("Time taken is %f seconds\n", exec_time);


    stbi_write_jpg("output.png", width, height, channels, img_out, 100);

    printf("Image blurred successfully!\n");

    stbi_image_free(img);
    free(img_out);

    return 0;
}
