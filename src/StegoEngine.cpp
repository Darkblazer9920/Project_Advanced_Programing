#include "StegoEngine.hpp"
#include <algorithm>
#include <numeric>
using namespace std;

string StegoEngine::encryptDecryptMessage(const string& data, const string& password) {
    string result = data;
    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] ^ password[i % password.size()];
    }
    return result;
}

bool StegoEngine::hideData(ImageBMP& img, const string& secretMessage, const string& password) {
    // Cifra el mensaje con la contraseña antes de ocultarlo // 
    //delimitador nulo '\0'//
    string processedMsg = encryptDecryptMessage(secretMessage, password);
    processedMsg.push_back('\0'); 

    // Convierte el mensaje a stream de bits//
    vector<bool> bitStream;
    for (char c : processedMsg) {
        for (int i = 7; i >= 0; --i) {
            bitStream.push_back((c >> i) & 1);
        }
    }

    auto& pixels = img.getPixelData();
    size_t totalPixelsAvailable = pixels.size();

    if (bitStream.size() > totalPixelsAvailable) {
        cerr << "Error: Message is too large for this image!\n";
        return false;
    }

    // Generar índices pseudoaleatorios basados en la contraseña para dispersar los bits
    unsigned int seed = 0;
    for (char c : password) seed += c;
    mt19937 rng(seed);
    
    vector<size_t> indices(totalPixelsAvailable);
    iota(indices.begin(), indices.end(), 0);
    shuffle(indices.begin(), indices.end(), rng);

    // Incrustar bits usando LSB (Least Significant Bit)
    for (size_t i = 0; i < bitStream.size(); ++i) {
        size_t idx = indices[i];
        pixels[idx] = (pixels[idx] & 0xFE) | bitStream[i];
    }

    return true;
}

string StegoEngine::extractData(ImageBMP& img, const string& password) {
    auto& pixels = img.getPixelData();
    size_t totalPixelsAvailable = pixels.size();

    unsigned int seed = 0;
    for (char c : password) seed += c;
    mt19937 rng(seed);

    vector<size_t> indices(totalPixelsAvailable);
    iota(indices.begin(), indices.end(), 0);
    shuffle(indices.begin(), indices.end(), rng);

    vector<bool> bitStream;
    // Extraer bits de forma secuencial según el orden PRNG
    for (size_t i = 0; i < totalPixelsAvailable; ++i) {
        size_t idx = indices[i];
        bitStream.push_back(pixels[idx] & 1);
    }

    // Reconstruir bytes y buscar el terminador nulo '\0'
    string extractedBytes = "";
    for (size_t i = 0; i + 8 <= bitStream.size(); i += 8) {
        char c = 0;
        for (int j = 0; j < 8; ++j) {
            c = (c << 1) | bitStream[i + j];
        }
        if (c == '\0') break;
        extractedBytes.push_back(c);
    }

    // Descifrar con la misma contraseña
    return encryptDecryptMessage(extractedBytes, password);
}