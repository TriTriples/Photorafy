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
        for (int i = 0; i < currentImage.height; i++) {
            for (int j = 0; j < currentImage.width; j++) {
                for (int c = 0; c < 3; c++) {
                    currentImage(j, i, c) = 255 - currentImage(j, i, c);
                }
            }
        }
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
        cout << "6. Undo\n";
        cout << "7. Save & Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: editor.flipVertical(); break;
            case 2: editor.flipHorizontal(); break;
            case 3: editor.blackAndWhite(); break;
            case 4: editor.grayscale(); break;
            case 5: editor.invert(); break;
            case 6: editor.undo(); break;
            case 7: {
                string saveName;
                cout << "Enter name to save final image: ";
                cin >> saveName;
                editor.save("generated/" + saveName);
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