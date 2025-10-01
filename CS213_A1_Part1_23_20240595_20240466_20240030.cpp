/*
* CS213 OOP - Assignment 1 - Image Filters
* Made by: Mostafa Essam, Mohamed Osama, Ahmed Sayed
* Section: 23/24 (We haven't yet registered because the website is down, so this might change later)
* Students and Who did what:
* - Mostafa Essam Atrees, ID: 20240595
*   - Implemented Filters: Invert, Rotate
* - Mohamed Osama Mohamed, ID: 20240466
*   - Implemented Filters: Flip (Vertical & Horizontal), Black & White
* - Ahmed Sayed Ibrahim, ID: 20240030
*   - Implemented Filters: Grayscale, Brighten/Darken, Resize, Merge
*  
*  Menu is mostly done by Mostafa Essam.
*  The application also has some more features, such as: Live Preview (preview.bmp in generated folder, lets you see the changes to the image), Undo functionality.
*  Github Repository: https://github.com/MrQuartz99/img-filterer (it is private tho)
*
* Our Project Structure:
*  - the main cpp file (this file)
*    - The filters logic are under the ImageEditor class.
*    - And the menu is in the main function.
*  - lib folder: Contains the libraries.
*  - generated folder: Contains the output images (preview.bmp and the saved images).
*  - images folder: Contains the input images (you can add your own images here to test the program).
*  
*
* Important NOTE: A specific project structure is not mentioned in the assignment instructures,
* but we thought it would be better to have a clean structure like this.
*
* Please kindly ensure to put the files in the correct folders and configure the PATHS below to match your testing environment, otherwise the program may not work as intended.
* To make it easy, the provided app will assume the default paths for everything (not our own structure but just the current directory).
*/

#include <iostream>
#include <stack>
#include <string>
#include <algorithm>
#include <filesystem>
#include <vector>

// Ensure to adjust this path if your lib file is located elsewhere
#include "Image_Class.h" 
using namespace std;

// ===== CONFIGURABLE PATHS =====
// The following can be modified these paths according to your testing environment
const string IMAGES_FOLDER = "";        // input images (leave empty for current dir, or '/images' for example if you have an images folder)
const string GENERATED_FOLDER = "";  // output/saved images (leave empty for current dir, or '/generated')
const string PREVIEW_FILE = "preview.bmp";     // Preview filename

class ImageEditor {
private:
    Image originalImage;
    Image currentImage;
    std::stack<Image> history;
    vector<int> supportedAngles = {0, 90, -90, 180, 270, -270};
    // .JPG, JPEG, .BMP, .PNG, .TGA
    vector<string> supportedFormats = {"jpg", "jpeg", "png", "bmp", "tga",};


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

    void rotate(int angleDegrees) {
        history.push(currentImage);

        int w = currentImage.width;
        int h = currentImage.height;
        int c = currentImage.channels;

        int turns = ((angleDegrees % 360) + 360) % 360 / 90;

        Image result((turns % 2 == 0) ? w : h, (turns % 2 == 0) ? h : w);

        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                for (int ch = 0; ch < c; ch++) {
                    int nx, ny;

                    switch (turns) {
                        case 0: nx = x;         ny = y;         break; // 0°
                        case 1: nx = h - 1 - y; ny = x;         break; // 90° CW
                        case 2: nx = w - 1 - x; ny = h - 1 - y; break; // 180°
                        case 3: nx = y;         ny = w - 1 - x; break; // 270° CW (-90°)
                    }

                    result(nx, ny, ch) = currentImage(x, y, ch);
                }
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

    bool isValidImageFormat(const string& filename) {
        size_t lastDot = filename.find_last_of('.');
        if (lastDot == string::npos) return false;
        
        string extension = filename.substr(lastDot + 1);
        transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
        
        return (std::find(supportedFormats.begin(), supportedFormats.end(), extension) != supportedFormats.end());
    }
    
    string promptForValidFilename(const string& defaultName) {
        string saveName;
        bool validFormat = false;
        
        while (!validFormat) {
            cout << "Enter name to save final image (leave empty to use original): ";
            cin.ignore();
            getline(cin, saveName);
            
            if (saveName.empty()) {
                saveName = defaultName;
            }
            
            if (isValidImageFormat(saveName)) {
                validFormat = true;
            } else {
                cout << "Invalid file format! Supported formats: .jpg, .jpeg, .png, .bmp, .tga\n";
                cout << "Please try again.\n";
            }
        }
        
        return saveName;
    }

    vector<int> getSupportedAngles() const { return supportedAngles; }

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

        fullPath = IMAGES_FOLDER + filename;

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
        cout << "1. Flip Image\n";
        cout << "2. Black & White\n";
        cout << "3. Grayscale\n";
        cout << "4. Invert Colors\n";
        cout << "5. Brighten/Darken\n";
        cout << "6. Rotate Image\n";
        cout << "7. Merge with Image\n";
        cout << "8. Resize Image\n";
        cout << "9. Undo\n";
        cout << "10. Exit\n";
        cout << "11. Save & Exit\n";
        cout << "Enter choice: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> choice;

        if(cin.fail()) {
            cout << "Invalid input! Please enter a valid choice number.\n";
            continue; 
        }

        switch (choice) {
            case 1: {
                int flipChoice;
                cout << "Select flip direction:\n";
                cout << "1. Vertically\n";
                cout << "2. Horizontally\n";
                cout << "Enter choice: ";
                cin >> flipChoice;
                
                if (flipChoice == 1) {
                    editor.flipVertical();
                    cout << "Image flipped vertically!\n";
                } else if (flipChoice == 2) {
                    editor.flipHorizontal();
                    cout << "Image flipped horizontally!\n";
                } else {
                    cout << "Invalid choice! No flip applied.\n";
                }
                break;
            }
            case 2: editor.blackAndWhite(); break;
            case 3: editor.grayscale(); break;
            case 4: editor.invert(); break;
            case 5: {
                double factor;
                cout << "Enter brightness factor (>1 to brighten, <1 to darken): ";
                cin >> factor;
                cout << factor;
                editor.changeBrightness(factor);
                break;
            }
            case 6: {
                int rotateChoice;
                cout << "Choose rotation:\n";
                cout << "1. 90° Clockwise\n";
                cout << "2. 90° Counter-clockwise\n";
                cout << "3. 180°\n";
                cout << "4. 270° Clockwise\n";
                cout << "5. 270° Counter-clockwise\n";
                cout << "Enter choice: ";
                cin >> rotateChoice;

                int angle = (rotateChoice >= 1 && rotateChoice <= 5) ? editor.getSupportedAngles()[rotateChoice] : 0;

                if (rotateChoice) {
                    editor.rotate(angle);
                    cout << "Image rotated " << angle << "°!\n";
                } else {
                    cout << "Invalid choice! No rotation applied.\n";
                }
                break;
            }
            case 7: {
                string mergeFilename;
                cout << "Enter filename of image to merge with (from images folder): ";
                cin >> mergeFilename;
                string mergePath = IMAGES_FOLDER + mergeFilename;
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
                char saveChoice;
                cout << "Do you want to save your changes before exiting? (y/n): ";
                cin >> saveChoice;
                
                if (saveChoice == 'y' || saveChoice == 'Y') {
                    string defaultName = filename.substr(filename.find_last_of("/\\") + 1);
                    string saveName = editor.promptForValidFilename(defaultName);
                    
                    editor.save(GENERATED_FOLDER + saveName);
                    cout << "Image saved as: " << GENERATED_FOLDER << saveName << endl;
                }
                
                cout << "Exiting without saving changes.\n";
                done = true;
                break;
            }
            case 11: {
                string defaultName = filename.substr(filename.find_last_of("/\\") + 1);
                string saveName = editor.promptForValidFilename(defaultName);
                
                editor.save(GENERATED_FOLDER + saveName);
                cout << "Image saved as: " << GENERATED_FOLDER << saveName << endl;
                done = true;
                break;
            }
            default: cout << "Invalid choice!\n"; break;
        }
        if (!done) {
            editor.preview(GENERATED_FOLDER + PREVIEW_FILE);
            cout << "Preview updated: " << GENERATED_FOLDER << PREVIEW_FILE << "\n";
        }
    }

    cout << "Image editing finished. Goodbye!\n";
    return 0;
}
