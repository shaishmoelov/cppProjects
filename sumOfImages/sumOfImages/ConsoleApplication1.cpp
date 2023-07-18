// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <emmintrin.h>
#include <new>
#include <memory>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <vcruntime_new.h>
#include <cstdlib>
#include "ConsoleApplication1.h"
#include <malloc.h>

const int width = 1024;
const int height = 768;


unsigned short* loadBinaryImage(const std::string& filename, unsigned short* image) {
    FILE* file = fopen(filename.c_str(), "rb");
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return nullptr;
    }
    fread(image, sizeof(short), width * height, file);
    fclose(file);

    return image;
}

void saveBinaryImage(const std::string& filename, unsigned short* image) {
    FILE* file = fopen(filename.c_str(), "wb");
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    fwrite(image, sizeof(short), width * height, file);
    fclose(file);
}

void sum_of_images_sse(unsigned short* img1, unsigned short* img2, unsigned short* sum) {
    /*Here we need to use 128 bit register to sum up 16 bit pixels.
    Therefor we can take 128/16=8 pixels from each image and sum them respectivly*/
    __m128i i7_i6_i5_i4_i3_i2_i1_i0;
    __m128i j7_j6_j5_j4_j3_j2_j1_j0;
    __m128i k7_k6_k5_k4_k3_k2_k1_k0;
    for (int i = 0; i < width * height; i+=8) {
        i7_i6_i5_i4_i3_i2_i1_i0 = _mm_load_si128((__m128i*) & img1[i]);
        j7_j6_j5_j4_j3_j2_j1_j0 = _mm_load_si128((__m128i*) & img2[i]);
        k7_k6_k5_k4_k3_k2_k1_k0 = _mm_adds_epu16(i7_i6_i5_i4_i3_i2_i1_i0, j7_j6_j5_j4_j3_j2_j1_j0);
         _mm_store_si128((__m128i*) & sum[i], k7_k6_k5_k4_k3_k2_k1_k0);
    }
        
}


int main()
{   
    /*unsigned short* p1 = static_cast<unsigned short*>(
        operator new[](sizeof(unsigned short) * width * height, (std::align_val_t)(16)));
    unsigned short* p2 = static_cast<unsigned short*>(
        operator new[](sizeof(unsigned short)* width* height, (std::align_val_t)(16)));
    unsigned short* p3 = static_cast<unsigned short*>(
          operator new[](sizeof(unsigned short)* width* height, (std::align_val_t)(16)));*/

    // Align allocation with alignes malloc
    unsigned short* p1 = (unsigned short *)_aligned_malloc(width * height * sizeof(*p1), 16);
    unsigned short* p2= (unsigned short*)_aligned_malloc(width * height * sizeof(*p2), 16);
    unsigned short* p3 = (unsigned short*)_aligned_malloc(width * height * sizeof(*p3), 16);



    //unsigned short* image2 = new (std::align_val_t{16}) unsigned short[width * height];
    std::string file1 = "image_1_with_noise.bin";
    std::string file2 = "image_2_with_noise.bin";
    std::string sum_image_out_file = "sum_image.bin";

    /*Load the binary images*/
    loadBinaryImage(file1, p1);
    loadBinaryImage(file2, p2);
    if (!p1 || !p2) {
        std::cerr << "Load binary images error" << std::endl;
    }
    sum_of_images_sse(p1, p2, p3);
    saveBinaryImage(sum_image_out_file, p3);
    

    /*Free all three images*/
    _aligned_free(p1);
    _aligned_free(p2);
    _aligned_free(p3);

    return 0;
}
