#include <iostream>
#include "ImageBMP.hpp"
#include "StegoEngine.hpp"
using namespace std;

void showHelp() {
    cout << "=== ADVANCED BMP STEGANOGRAPHY CLI ===\n";
    cout << "Usage:\n";
    cout << "  Hide:    ./steg_tool hide <input.bmp> <output.bmp> \"secret message\" \"password\"\n";
    cout << "  Extract: ./steg_tool extract <input.bmp> \"password\"\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        showHelp();
        return 1;
    }

    string mode = argv[1];
    StegoEngine engine;

    if (mode == "hide" && argc == 6) {
        string inputFile = argv[2];
        string outputFile = argv[3];
        string message = argv[4];
        string password = argv[5];

        ImageBMP img;
        if (!img.load(inputFile)) {
            cerr << "Failed to load input image.\n";
            return 1;
        }

        if (engine.hideData(img, message, password)) {
            if (img.save(outputFile)) {
                cout << "[SUCCESS] Secret message hidden successfully in " << outputFile << "\n";
            } else {
                cerr << "Error saving output image.\n";
            }
        }
    } 
    else if (mode == "extract" && argc == 4) {
        string inputFile = argv[2];
        string password = argv[3];

        ImageBMP img;
        if (!img.load(inputFile)) {
            cerr << "Failed to load input image.\n";
            return 1;
        }

        string secret = engine.extractData(img, password);
        cout << "[SUCCESS] Extracted Secret Message: " << secret << "\n";
    } 
    else {
        showHelp();
    }

    return 0;
}