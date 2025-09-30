#include <iostream>
#include <stack>
#include <string>
#include <algorithm>
#include <filesystem>
#include "lib/Image_Class.h" 
using namespace std;

class ImageEditor {
private:
    Image originalImage;
    Image currentImage;
    std::stack<Image> history;

public:
    ImageEditor(const std::string& path)
        : originalImage(path), currentImage(path) {}

    void flipVertical() {
        history.push(currentImage);
        for (int i = 0; i < currentImage.height / 2; i++) {
            for (int j = 0; j < currentImage.width; j++) {
                for (int c = 0; c < 3; c++) {
                    std::swap(currentImage(j, i, c),
                              currentImage(j, currentImage.height - i - 1, c));
                }
            }
        }
    }

    void flipHorizontal() {
        history.push(currentImage);
        for (int i = 0; i < currentImage.height; i++) {
            for (int j = 0; j < currentImage.width / 2; j++) {
                for (int c = 0; c < 3; c++) {
                    std::swap(currentImage(j, i, c),
                              currentImage(currentImage.width - j - 1, i, c));
                }
            }
        }
    }

    void blackAndWhite() {
        history.push(currentImage);
        for (int i = 0; i < currentImage.height; i++) {
            for (int j = 0; j < currentImage.width; j++) {
                int r = currentImage(j, i, 0);
                int g = currentImage(j, i, 1);
                int b = currentImage(j, i, 2);
                int avg = (r + g + b) / 3;
                int bw = (avg < 128) ? 0 : 255;
                for (int c = 0; c < 3; c++) currentImage(j, i, c) = bw;
            }
        }
    }

    void grayscale() {
        history.push(currentImage);
        for (int i = 0; i < currentImage.height; i++) {
            for (int j = 0; j < currentImage.width; j++) {
                int r = currentImage(j, i, 0);
                int g = currentImage(j, i, 1);
                int b = currentImage(j, i, 2);
                int avg = (r + g + b) / 3;
                for (int c = 0; c < 3; c++) currentImage(j, i, c) = avg;
            }
        }
    }

    void invert() {
        history.push(currentImage);
        for (int i = 0; i < currentImage.height; i++) {
            for (int j = 0; j < currentImage.width; j++) {
                for (int c = 0; c < 3; c++) {
                    currentImage(j, i, c) = 255 - currentImage(j, i, c);
                }
            }
        }
    }

    void changeBrightness(double factor) {
        history.push(currentImage);
        for (int y = 0; y < currentImage.height; y++) {
            for (int x = 0; x < currentImage.width; x++) {
                unsigned char r = currentImage(x, y, 0);
                unsigned char g = currentImage(x, y, 1);
                unsigned char b = currentImage(x, y, 2);

                int newR = min(255, max(0, static_cast<int>(r * factor)));
                int newG = min(255, max(0, static_cast<int>(g * factor)));
                int newB = min(255, max(0, static_cast<int>(b * factor)));

                currentImage(x, y, 0) = newR;
                currentImage(x, y, 1) = newG;
                currentImage(x, y, 2) = newB;
            }
        }
    }

    Image resizeImage(const Image& src, int newW, int newH, bool forMerge = false) {
        if(!forMerge) history.push(currentImage);
        Image resized(newW, newH);

        double scaleX = static_cast<double>(src.width) / newW;
        double scaleY = static_cast<double>(src.height) / newH;

        for (int y = 0; y < newH; y++) {
            int srcY = static_cast<int>(y * scaleY);
            for (int x = 0; x < newW; x++) {
                int srcX = static_cast<int>(x * scaleX);

                unsigned char r = src(srcX, srcY, 0);
                unsigned char g = src(srcX, srcY, 1);
                unsigned char b = src(srcX, srcY, 2);

                resized(x, y, 0) = r;
                resized(x, y, 1) = g;
                resized(x, y, 2) = b;
            }
        }
        if(!forMerge) {
            currentImage = resized;
        }
        return resized;
    }

    void mergeWithImage(const std::string& imagePath) {
        history.push(currentImage);

        Image img2(imagePath);

        int newW = std::max(currentImage.width, img2.width);
        int newH = std::max(currentImage.height, img2.height);

        Image resized1 = resizeImage(currentImage, newW, newH, true);
        Image resized2 = resizeImage(img2, newW, newH, true);

        Image result(newW, newH);

        for (int y = 0; y < newH; y++) {
            for (int x = 0; x < newW; x++) {
                unsigned char r1 = resized1(x, y, 0);
                unsigned char g1 = resized1(x, y, 1);
                unsigned char b1 = resized1(x, y, 2);

                unsigned char r2 = resized2(x, y, 0);
                unsigned char g2 = resized2(x, y, 1);
                unsigned char b2 = resized2(x, y, 2);

                result(x, y, 0) = (r1 + r2) >> 1;
                result(x, y, 1) = (g1 + g2) >> 1;
                result(x, y, 2) = (b1 + b2) >> 1;
            }
        }

        currentImage = result;
    }


    void undo() {
        if (!history.empty()) {
            currentImage = history.top();
            history.pop();
        }
    }

    void save(const std::string& path) {
        currentImage.saveImage(path);
    }

    void preview(const std::string& path) {
        currentImage.saveImage(path);
    }

    Image getCurrentImage() const { return currentImage; }

};


int main() {
    string filename;
    string fullPath;

    while (true) {
        cout << "Enter image filename (or quit with 'exit'): ";
        cin >> filename;

        if (filename == "exit") {
            cout << "bye bey!\n";
            return 0; 
        }

        fullPath = "images/" + filename;

        if (std::filesystem::exists(fullPath)) {
            break; 
        } else {
            cout << "File not found: " << fullPath << endl;
            cout << "try again.\n";
        }
    }

    ImageEditor editor(fullPath);
    editor.preview("generated/preview.bmp");

    int choice;
    bool done = false;
    while (!done) {
        cout << "\n--- Welcome to your Image Editor ---\n";
        cout << "1. Flip Vertical\n";
        cout << "2. Flip Horizontal\n";
        cout << "3. Black & White\n";
        cout << "4. Grayscale\n";
        cout << "5. Invert Colors\n";
        cout << "6. Brighten/Darken\n";
        cout << "7. Merge with Image\n";
        cout << "8. Resize Image\n";
        cout << "9. Undo\n";
        cout << "10. Save & Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: editor.flipVertical(); break;
            case 2: editor.flipHorizontal(); break;
            case 3: editor.blackAndWhite(); break;
            case 4: editor.grayscale(); break;
            case 5: editor.invert(); break;
            case 6: {
                double factor;
                cout << "Enter brightness factor (>1 to brighten, <1 to darken): ";
                cin >> factor;
                editor.changeBrightness(factor);
                break;
            }
            case 7: {
                string mergeFilename;
                cout << "Enter filename of image to merge with (from images folder): ";
                cin >> mergeFilename;
                string mergePath = "images/" + mergeFilename;
                if (std::filesystem::exists(mergePath)) {
                    cout << "Merging images in progress...\n";
                    editor.mergeWithImage(mergePath);
                    cout << "Images merged successfully!\n";
                } else {
                    cout << "File not found: " << mergePath << endl;
                }
                break;
            }
            case 8: {
                int newWidth, newHeight;
                cout << "Enter new width: ";
                cin >> newWidth;
                cout << "Enter new height: ";
                cin >> newHeight;
                if (newWidth > 0 && newHeight > 0) {
                    cout << "Resizing image...\n";
                    editor.resizeImage(editor.getCurrentImage(), newWidth, newHeight);
                    cout << "Image resized to " << newWidth << "x" << newHeight << " pixels!\n";
                } else {
                    cout << "Invalid dimensions! Width and height must be positive.\n";
                }
                break;
            }
            case 9: editor.undo(); break;
            case 10: {
                string saveName;
                cout << "Enter name to save final image (leave empty to use original): ";
                cin.ignore();
                getline(cin, saveName);

                if (saveName.empty()) {
                    saveName = filename.substr(filename.find_last_of("/\\") + 1);
                }
                
                editor.save("generated/" + saveName);
                cout << "Image saved as: generated/" << saveName << endl;
                done = true;
                break;
            }
            default: cout << "Invalid choice!\n"; break;
        }
        if (!done) {
            editor.preview("generated/preview.bmp");
            cout << "Preview updated: generated/preview.bmp\n";
        }
    }

    cout << "Image editing finished. Goodbye!\n";
    return 0;
}
