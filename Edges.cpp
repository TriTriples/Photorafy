#include <iostream>
#include <cmath>
#include "lib/Image_Class.h"
using namespace std;

void edgeDetection(Image& image) {
   Image temp = image;
    int Gx[3][3] = {{-1, 0, 1},
                    {-2, 0, 2},
                    {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1},
                    { 0,  0,  0},
                    { 1,  2,  1}};

    for (int y = 1; y < image.height - 1; y++) {
        for (int x = 1; x < image.width - 1; x++) {
            double sumX = 0, sumY = 0;

            for (int j = -1; j <= 1; j++) {
                for (int i = -1; i <= 1; i++) {
                    int gray = (temp(x+i, y+j, 0) + temp(x+i, y+j, 1) + temp(x+i, y+j, 2)) / 3;
                    sumX += Gx[j+1][i+1] * gray;
                    sumY += Gy[j+1][i+1] * gray;
                }
            }

            int magnitude = sqrt(sumX * sumX + sumY * sumY);
            magnitude = min(255, max(0, magnitude));

            image(x, y, 0) = 255 - magnitude;
            image(x, y, 1) = 255 - magnitude;
            image(x, y, 2) = 255 - magnitude;
        }
    }
}

int main() {
    string inputName, outputName;

    cout << "Enter input image name (e.g. photo.jpg): ";
    cin >> inputName;

    cout << "Enter output image name (e.g. edges.jpg): ";
    cin >> outputName;
    
    Image img("images/" + inputName);
    
    edgeDetection(img);
    
    img.saveImage(outputName);
    return 0;
}
