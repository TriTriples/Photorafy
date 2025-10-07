#include <iostream>
#include "lib/Image_Class.h"
using namespace std;

void purpleFilter(Image& image) {
    for (int y = 0; y < image.height; y++) {
        for (int x = 0; x < image.width; x++) {

            unsigned char r = image(x, y, 0);
            unsigned char g = image(x, y, 1);
            unsigned char b = image(x, y, 2);

            int newR = min(255, int(r + 60));
            int newG = max(0, int(g - 30));
            int newB = min(255, int(b + 50));

            image(x, y, 0) = newR;
            image(x, y, 1) = newG;
            image(x, y, 2) = newB;
        }
    }
}

int main() {
    string inputName, outputName;

    cout << "Enter input image name: ";
    cin >> inputName;
    cout << "Enter output image name: ";
    cin >> outputName;

    Image img("images/" + inputName);

    purpleFilter(img);

    img.saveImage(outputName);
    return 0;
}
