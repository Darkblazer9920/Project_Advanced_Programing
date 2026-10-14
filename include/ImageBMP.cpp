#ifndef IMAGE_BMP_HPP
#define IMAGE_BMP_HPP

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t fileType;
    uint32_t fileSize;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t dataOffset;
};

struct BMPInfoHeader {
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPelsPerMeter;
    int32_t yPelsPerMeter;
    uint32_t clrUsed;
    uint32_t clrImportant;
};
#pragma pack(pop)

class ImageBMP {
private:
    BMPHeader fileHeader;
    BMPInfoHeader infoHeader;
    std::vector<uint8_t> pixelData;
    int rowStride;
    int padding;

public:
    bool load(const std::string& filename);
    bool save(const std::string& filename);
    
    int getWidth() const { return infoHeader.width; }
    int getHeight() const { return infoHeader.height; }
    uint16_t getBitCount() const { return infoHeader.bitCount; }
    
    std::vector<uint8_t>& getPixelData() { return pixelData; }
    int getRowStride() const { return rowStride; }
    int getPadding() const { return padding; }
};

#endif
