#ifndef STEGO_ENGINE_HPP
#define STEGO_ENGINE_HPP

#include "ImageBMP.hpp"
#include <string>
#include <random>
using namespace std;

class StegoEngine {
private:
    string encryptDecryptMessage(const string& data, const string& password);

public:
    bool hideData(ImageBMP& img, const string& secretMessage, const string& password);
    string extractData(ImageBMP& img, const string& password);
};

#endif