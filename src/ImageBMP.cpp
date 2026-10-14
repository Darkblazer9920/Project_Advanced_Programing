#include "ImageBMP.hpp"
using namespace std;

bool ImageBMP::load(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) return false;

    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    if (fileHeader.fileType != 0x4D42) { // 'BM'
        cerr << "Error: Not a valid BMP file.\n";
        return false;
    }

    if (infoHeader.bitCount != 24) {
        cerr << "Error: Only 24-bit uncompressed BMP files are supported.\n";
        return false;
    }

    // Calcular el stride real por fila y el padding (alineación a 4 bytes)
    int rowBytes = infoHeader.width * 3;
    padding = (4 - (rowBytes % 4)) % 4;
    rowStride = rowBytes + padding;

    file.seekg(fileHeader.dataOffset, ios::beg);
    
    int dataSize = rowStride * abs(infoHeader.height);
    pixelData.resize(dataSize);
    file.read(reinterpret_cast<char*>(pixelData.data()), dataSize);

    file.close();
    return true;
}

bool ImageBMP::save(const string& filename) {
    ofstream file(filename, ios::binary);
    if (!file) return false;

    file.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));
    
    file.seekp(fileHeader.dataOffset, ios::beg);
    file.write(reinterpret_cast<const char*>(pixelData.data()), pixelData.size());

    file.close();
    return true;
}
