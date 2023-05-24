#include <iostream>
#include <cstdio>
#include <vector>

// Image properties
const int width = 1024;
const int height = 768;
const int bit_depth = 10;

// Function to load a binary image
std::vector<uint16_t> loadBinaryImage(const std::string& filename)
{
    FILE* file = fopen(filename.c_str(), "r");
    if (!file)
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return std::vector<uint16_t>();
    }

    std::vector<uint16_t> image(width * height);

    fread(image.data(), sizeof(uint16_t), width * height, file);
    fclose(file);

    return image;
}

// Function to save a binary image
void saveBinaryImage(const std::string& filename, const std::vector<uint16_t>& image)
{
    FILE* file = fopen(filename.c_str(), "wb");
    if (!file)
    {
        std::cerr << "Failed to create file: " << filename << std::endl;
        return;
    }

    fwrite(image.data(), sizeof(uint16_t), width * height, file);
    fclose(file);
}

// Function to add two images and return the result
std::vector<uint16_t> addImages(const std::vector<uint16_t>& image1, const std::vector<uint16_t>& image2)
{
    std::vector<uint16_t> result(width * height);

    for (int i = 0; i < width * height; ++i)
    {
        result[i] = std::min(image1[i] + image2[i], static_cast<int>((1 << bit_depth) - 1));
    }

    return result;
}

int main()
{
    std::string file1 = "G:\\My Drive\\myProject\\pythonProjectss\\pythonProjectss\\createImages\\image1.bin";
    std::string file2 = "G:\\My Drive\\myProject\\pythonProjectss\\pythonProjectss\\createImages\\image2.bin";
    std::string outputFile = "sum_of_images.bin";

    // Load the binary images
    std::vector<uint16_t> image1 = loadBinaryImage(file1);
    std::vector<uint16_t> image2 = loadBinaryImage(file2);

    if (image1.empty() || image2.empty())
    {
        return 1;
    }

    // Add the images
    std::vector<uint16_t> sumImage = addImages(image1, image2);

    // Save the resulting image
    saveBinaryImage(outputFile, sumImage);

    std::cout << "Images added and saved successfully." << std::endl;

    return 0;
}