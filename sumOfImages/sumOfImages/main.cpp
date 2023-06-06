#include <iostream>
#include <cstdio>
#include <vector>
#include <new>


namespace std {
    enum class align_val_t : size_t;
};
void* operator new(std::size_t, std::align_val_t);	// new overload


// Image properties
const int width = 1024;
const int height = 768;
const int bit_depth = 10;

// Function to load a binary image
void * loadBinaryImage(const std::string& filename)
{
    FILE* file = fopen(filename.c_str(), "r");
    if (!file)
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return nullptr;
    }
    void* image = new (std::align_val_t(16)) short[width * height * 2];

    fread(image, sizeof(uint16_t), width * height * 2, file);
    fclose(file);

    return image;
}

// Function to save a binary image
void saveBinaryImage(const std::string& filename, void * image)
{
    FILE* file = fopen(filename.c_str(), "wb");
    if (!file)
    {
        std::cerr << "Failed to create file: " << filename << std::endl;
        return;
    }

    fwrite(image, sizeof(uint16_t), width * height, file);
    fclose(file);
}

// Function to add two images and return the result
//
void * addImages(void *image1, void *image2)
{
    void *result = new (std::align_val_t(32)) short[width * height * 2];

    for (int i = 0; i < width * height * 2; ++i)
    {
        //result[i] = image1[i] + image2[i] + 1;
    }

    return result;
}
/*assign array with maloc or new - new with alignment */
int main()
{
    std::string file1 = "G:/My Drive/myProject/pythonProjectss/pythonProjectss/createImages/image_1_with_noise.bin";
    std::string file2 = "G:/My Drive/myProject/pythonProjectss/pythonProjectss/createImages/image_2_with_noise.bin";
    std::string outputFile = "sum_of_images.bin";

    // Load the binary images
    //change to arrays with align malloc or new
    /*std::vector<uint16_t> image1 = loadBinaryImage(file1);
    std::vector<uint16_t> image2 = loadBinaryImage(file2);*/

    void * image1 = loadBinaryImage(file1);
    void * image2 = loadBinaryImage(file2);

    if (!image1 || !image2)
    {
        return 1;
    }

    // Add the images
    //array of sum
    void* sumImage = new (std::align_val_t(16)) short[width * height * 2];
    sumImage = addImages(image1, image2);

    // Save the resulting image
    saveBinaryImage(outputFile, sumImage);

    std::cout << "Images added and saved successfully." << std::endl;

    return 0;
}

//take from matlab 2 images - google
