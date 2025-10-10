/*
* CS213 OOP - Assignment 1 - Image Filters
*
*
*  Github Repository: https://github.com/MrQuartz99/img-filterer (it is private tho)
*  Shared Document: https://docs.google.com/document/d/1K5frqWIAup2jfgmTLkIUESWwHec8LexgF5wVzLGBv6Y/edit?usp=sharing
*  Video Demo: https://drive.google.com/file/d/1gaUNFoNvebYPPNmXoKVLQrlCS_5fUY_7/view?usp=sharing
*
* Made by: Mostafa Essam, Mohamed Osama, Ahmed Sayed
* Section: S22
* Students and Who did what:
* - Mostafa Essam Atrees, ID: 20240595 (S22)
*   - Implemented Filters: Invert, Rotate, Frames, Blur, Old TV, Image Skewing
* - Mohamed Osama Mohamed, ID: 20240466 (S22)
*   - Implemented Filters: Flip (Vertical & Horizontal), Black & White, Crop, Oil Painting
* - Ahmed Sayed Ibrahim, ID: 20240030 (All-B)
*   - Implemented Filters: Grayscale, Brighten/Darken, Resize, Merge, Edge Detection, Sunlight, Night Purple
*  
*  Menu is mostly done by Mostafa Essam.
*  The application also has some more features, such as: Live Preview (preview.bmp in generated folder, lets you see the changes to the image), Undo functionality.
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
* Important NOTE: A specific project structure is not mentioned in the assignment instructions,
* but we thought it would be better to have a clean structure like this.
*
* Please kindly ensure to put the files in the correct folders and configure the PATHS below to match your testing environment, otherwise the program may not work as intended.
* To make it easy, the provided app will assume the default paths for everything (not our own structure, but assuming everything is just in the current directory).
*/

#include <iostream>
#include <stack>
#include <string>
#include <algorithm>
#include <filesystem>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Ensure to adjust this path if your lib file is located elsewhere
// We'll assume it is in the same directory
#include "Image_Class.h" 
using namespace std;

// ===== CONFIGURABLE PATHS =====
// The following can be modified these paths according to your testing environment
const string IMAGES_FOLDER = "";        // input images (leave empty for current dir, or 'images/' for example if you have an images folder)
const string GENERATED_FOLDER = "";  // output/saved images (leave empty for current dir, or 'generated/')
const string PREVIEW_FILE = "preview.bmp";     // Preview filename

class ImageEditor {
private:
    Image originalImage;
    Image currentImage;
    std::stack<Image> history;
    static const int MAX_HISTORY_SIZE = 10;
    vector<int> supportedAngles = {0, 90, -90, 180, 270, -270};
    // .JPG, JPEG, .BMP, .PNG, .TGA
    vector<string> supportedFormats = {"jpg", "jpeg", "png", "bmp", "tga",};
    // keeping up to 10 undos
    void pushToHistory() {
        if (history.size() >= MAX_HISTORY_SIZE) {
            std::stack<Image> temp;
            
            for (int i = 0; i < MAX_HISTORY_SIZE - 1; i++) {
                temp.push(history.top());
                history.pop();
            }
            
            history.pop();
            
            while (!temp.empty()) {
                history.push(temp.top());
                temp.pop();
            }
        }
        history.push(currentImage);
    }
    public:
    ImageEditor(const std::string& path)
        : originalImage(path), currentImage(path) {}

    void flipVertical() {
        pushToHistory();
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
        pushToHistory();
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
        pushToHistory();
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
        pushToHistory();
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
        pushToHistory();
        for (int i = 0; i < currentImage.height; i++) {
            for (int j = 0; j < currentImage.width; j++) {
                for (int c = 0; c < 3; c++) {
                    currentImage(j, i, c) = 255 - currentImage(j, i, c);
                }
            }
        }
    }

    void changeBrightness(double factor) {
        pushToHistory();
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
        if(!forMerge) pushToHistory();
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
        pushToHistory();

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
        pushToHistory();

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

    int calculateSuitableThickness() {
        int minDim = min(currentImage.width, currentImage.height);
        int thickness = max(8, minDim / 34);
        return thickness;
    }

    void addFrame(int thickness, unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, bool decorated = false) {
        pushToHistory();

        int w = currentImage.width;
        int h = currentImage.height;

        if (decorated) {
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    int distLeft = x, distRight = w - 1 - x;
                    int distTop = y, distBottom = h - 1 - y;
                    int minDistFromEdge = min(min(distLeft, distRight), min(distTop, distBottom));
                    
                    if (minDistFromEdge < thickness) {
                        double gradientFactor = (double)minDistFromEdge / thickness;
                        double brightness = 0.6 + gradientFactor * 0.4;

                        double light = 1.0;
                        if (distTop < thickness / 2 && distLeft < thickness / 2) light = 1.3;
                        if (distBottom < thickness / 2 && distRight < thickness / 2) light = 0.8;
                        brightness *= light;

                        if ((x / 4 + y / 4) % 2 == 0) brightness *= 0.9;

                        int innerEdge = thickness * 2;
                        if (minDistFromEdge > thickness / 2 && minDistFromEdge < innerEdge)
                            brightness *= 1.0 + 0.3 * (1.0 - (double)(minDistFromEdge - thickness / 2) / (innerEdge - thickness / 2));

                        brightness *= 0.95 + (rand() % 10) / 100.0;

                        currentImage(x, y, 0) = min(255, (int)(r * brightness));
                        currentImage(x, y, 1) = min(255, (int)(g * brightness));
                        currentImage(x, y, 2) = min(255, (int)(b * brightness));
                    }
                }
            }
        } else {
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    bool isFrame = (x < thickness || x >= w - thickness || 
                                   y < thickness || y >= h - thickness);
                    
                    if (isFrame) {
                        currentImage(x, y, 0) = r;
                        currentImage(x, y, 1) = g;
                        currentImage(x, y, 2) = b;
                    }
                }
            }
        }
    }

    // This is using the Sobel operator
    void edgeDetection() {
        pushToHistory();
        Image temp = currentImage;
        int Gx[3][3] = {{-1, 0, 1},
                        {-2, 0, 2},
                        {-1, 0, 1}};
        int Gy[3][3] = {{-1, -2, -1},
                        { 0,  0,  0},
                        { 1,  2,  1}};

        for (int y = 1; y < currentImage.height - 1; y++) {
            for (int x = 1; x < currentImage.width - 1; x++) {
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

                currentImage(x, y, 0) = 255 - magnitude;
                currentImage(x, y, 1) = 255 - magnitude;
                currentImage(x, y, 2) = 255 - magnitude;
            }
        }
    }

    void purpleFilter() {
        pushToHistory();
        for (int y = 0; y < currentImage.height; y++) {
            for (int x = 0; x < currentImage.width; x++) {

                unsigned char r = currentImage(x, y, 0);
                unsigned char g = currentImage(x, y, 1);
                unsigned char b = currentImage(x, y, 2);

                int newR = min(255, int(r + 60));
                int newG = max(0, int(g - 30));
                int newB = min(255, int(b + 50));

                currentImage(x, y, 0) = newR;
                currentImage(x, y, 1) = newG;
                currentImage(x, y, 2) = newB;
            }
        }
    }

    void sunlightFilter(double intensity = 0.02) {
        pushToHistory();
        int lightX = currentImage.width / 2;
        int lightY = 0;

        double maxDist = sqrt(
            (currentImage.width - 1) * (currentImage.width - 1) +
            (currentImage.height - 1) * (currentImage.height - 1)
        );

        for (int y = 0; y < currentImage.height; y++) {
            for (int x = 0; x < currentImage.width; x++) {
                double dx = x - lightX;
                double dy = y - lightY;
                double dist = sqrt(dx * dx + dy * dy);

                double factor = 1 + intensity * (1 - (dist / maxDist));
                if (factor < 1) factor = 1;

                unsigned char r = currentImage(x, y, 0);
                unsigned char g = currentImage(x, y, 1);
                unsigned char b = currentImage(x, y, 2);

                int newR = min(255, int(r * factor + 45));
                int newG = min(255, int(g * factor + 30));
                int newB = min(255, int(b * factor - 10));

                currentImage(x, y, 0) = newR;
                currentImage(x, y, 1) = newG;
                currentImage(x, y, 2) = newB;
            }
        }
    }

    void oldTVFilter(double noiseLevel = 0.15, double scanlineIntensity = 0.7, double distortionLevel = 20.0) {
        pushToHistory();
        srand(static_cast<unsigned>(time(0)));
        
        for (int y = 0; y < currentImage.height; y++) {
            for (int x = 0; x < currentImage.width; x++) {
                unsigned char r = currentImage(x, y, 0);
                unsigned char g = currentImage(x, y, 1);
                unsigned char b = currentImage(x, y, 2);
                
                // 1. NOISE
                double noise = ((rand() % 1000) / 1000.0 - 0.5) * noiseLevel * 255.0;
                if (rand() % 3 == 0)
                    noise *= 1.5;
                else
                    noise *= 0.5;
                // 2. SCANLINES 
                double scanlineFactor = 1.0 - scanlineIntensity * (y % 2 == 0 ? 1.0 : 0.0);
                if (y % 2 == 0) {
                    scanlineFactor = scanlineIntensity;
                }
                // 3. DISTORTION
                double baseShift = sin(y * 0.1 ) * distortionLevel;
                double randomJitter = ((rand() % 1000) / 1000.0 - 0.5) * (distortionLevel * 0.2);
                double colorShift = (baseShift + randomJitter) * 0.3;   
                // Apply all effects
                int newR = min(255, max(0, static_cast<int>((r + noise + colorShift) * scanlineFactor)));
                int newG = min(255, max(0, static_cast<int>((g + noise - colorShift * 0.5) * scanlineFactor)));
                int newB = min(255, max(0, static_cast<int>((b + noise + colorShift * 0.3) * scanlineFactor)));
                
                currentImage(x, y, 0) = newR;
                currentImage(x, y, 1) = newG;
                currentImage(x, y, 2) = newB;
            }
        }
    }

    void skewImage(double skewDegree) {
        pushToHistory();
        
        double skewAngle = -(skewDegree * 3.14) / 180.0;
        double skewFactor = tan(skewAngle);
        
        int originalWidth = currentImage.width;
        int originalHeight = currentImage.height;
        
        int maxShift = static_cast<int>(abs(skewFactor * originalHeight));
        int newWidth = originalWidth + maxShift;
        
        Image skewed(newWidth, originalHeight);
        
        for (int y = 0; y < originalHeight; y++) {
            for (int x = 0; x < newWidth; x++) {
                skewed(x, y, 0) = 255;  // White bg
                skewed(x, y, 1) = 255;
                skewed(x, y, 2) = 255;
            }
        }
        
        for (int y = 0; y < originalHeight; y++) {
            int shift = static_cast<int>(skewFactor * y);
            
            int offset = (skewFactor < 0) ? maxShift : 0;
            
            for (int x = 0; x < originalWidth; x++) {
                int newX = x + shift + offset;
                
                // Check bounds
                if (newX >= 0 && newX < newWidth) {
                    skewed(newX, y, 0) = currentImage(x, y, 0);
                    skewed(newX, y, 1) = currentImage(x, y, 1);
                    skewed(newX, y, 2) = currentImage(x, y, 2);
                }
            }
        }
        
        for (int y = 0; y < originalHeight; y++) {
            for (int x = 1; x < newWidth - 1; x++) {
                // Check if this pixel is white (background) and neighbors have color
                if (skewed(x, y, 0) == 255 && skewed(x, y, 1) == 255 && skewed(x, y, 2) == 255) {
                    bool hasLeftNeighbor = (x > 0 && !(skewed(x-1, y, 0) == 255 && skewed(x-1, y, 1) == 255 && skewed(x-1, y, 2) == 255));
                    bool hasRightNeighbor = (x < newWidth-1 && !(skewed(x+1, y, 0) == 255 && skewed(x+1, y, 1) == 255 && skewed(x+1, y, 2) == 255));
                    
                    if (hasLeftNeighbor && hasRightNeighbor) {
                        skewed(x, y, 0) = (skewed(x-1, y, 0) + skewed(x+1, y, 0)) / 2;
                        skewed(x, y, 1) = (skewed(x-1, y, 1) + skewed(x+1, y, 1)) / 2;
                        skewed(x, y, 2) = (skewed(x-1, y, 2) + skewed(x+1, y, 2)) / 2;
                    }
                }
            }
        }
        
        currentImage = skewed;
    }

    // using 2d prefix sum 
    void boxBlur(int radius = 5) {
        pushToHistory();
        
        if (radius < 1) return;
        
        int w = currentImage.width;
        int h = currentImage.height;
        
        Image originalCopy = currentImage;
        
        vector<vector<vector<int>>> prefix(3, vector<vector<int>>(h + 1, vector<int>(w + 1, 0)));
        
        // Build prefix sum for each channel
        for (int y = 1; y <= h; y++) {
            int rSum = 0, gSum = 0, bSum = 0;
            for (int x = 1; x <= w; x++) {
                rSum += originalCopy(x - 1, y - 1, 0);
                gSum += originalCopy(x - 1, y - 1, 1);
                bSum += originalCopy(x - 1, y - 1, 2);
                
                prefix[0][y][x] = prefix[0][y - 1][x] + rSum;
                prefix[1][y][x] = prefix[1][y - 1][x] + gSum;
                prefix[2][y][x] = prefix[2][y - 1][x] + bSum;
            }
        }
        
        // Apply blur to each pixel using area sums
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                int x1 = max(0, x - radius);
                int y1 = max(0, y - radius);
                int x2 = min(w - 1, x + radius);
                int y2 = min(h - 1, y + radius);
                
                x1++; y1++; x2++; y2++;
                
                int area = (x2 - x1 + 1) * (y2 - y1 + 1);
                
                for (int c = 0; c < 3; c++) {
                    int sum = prefix[c][y2][x2]
                            - prefix[c][y1 - 1][x2]
                            - prefix[c][y2][x1 - 1]
                            + prefix[c][y1 - 1][x1 - 1];
                    
                    currentImage(x, y, c) = static_cast<unsigned char>(sum / area);
                }
            }
        }
    }

    void crop(int startX, int startY, int width, int height) {
        pushToHistory();
        
        if (startX < 0 || startY < 0 || width <= 0 || height <= 0 ||
            startX + width > currentImage.width || startY + height > currentImage.height) {
            cout << "Invalid crop dimensions!\n";
            return;
        }
        
        Image cropped(width, height);
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                cropped(j, i, 0) = currentImage(startX + j, startY + i, 0);
                cropped(j, i, 1) = currentImage(startX + j, startY + i, 1);
                cropped(j, i, 2) = currentImage(startX + j, startY + i, 2);
            }
        }
        currentImage = cropped;
    }

    void oilPainting(int radius = 3, int intensityLevels = 10) {
    pushToHistory();
    Image temp = currentImage;

    int width = currentImage.width;
    int height = currentImage.height;

    vector<vector<int>> intensityMap(height, vector<int>(width));
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            intensityMap[y][x] = (currentImage(x, y, 0)
                                + currentImage(x, y, 1)
                                + currentImage(x, y, 2)) / 3;

    vector<int> intensityCount(intensityLevels);
    vector<int> sumR(intensityLevels);
    vector<int> sumG(intensityLevels);
    vector<int> sumB(intensityLevels);
    for (int y = radius; y < height - radius; y++) {
        for (int x = radius; x < width - radius; x++) {

            std::fill(intensityCount.begin(), intensityCount.end(), 0);
            std::fill(sumR.begin(), sumR.end(), 0);
            std::fill(sumG.begin(), sumG.end(), 0);
            std::fill(sumB.begin(), sumB.end(), 0);
            for (int j = -radius; j <= radius; j++) {
                for (int i = -radius; i <= radius; i++) {
                    int R = currentImage(x + i, y + j, 0);
                    int G = currentImage(x + i, y + j, 1);
                    int B = currentImage(x + i, y + j, 2);
                    int intensity = (intensityMap[y+j][x+i] * intensityLevels) / 255;
                    if (intensity >= intensityLevels) intensity = intensityLevels - 1;

                    intensityCount[intensity]++;
                    sumR[intensity] += R;
                    sumG[intensity] += G;
                    sumB[intensity] += B;
                }
            }

            int maxCount = 0, maxIndex = 0;
            for (int k = 0; k < intensityLevels; k++)
                if (intensityCount[k] > maxCount)
                    maxCount = intensityCount[k], maxIndex = k;

            if (maxCount > 0) {
                temp(x, y, 0) = sumR[maxIndex] / maxCount;
                temp(x, y, 1) = sumG[maxIndex] / maxCount;
                temp(x, y, 2) = sumB[maxIndex] / maxCount;
            }
        }
    }

    currentImage = temp;
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

    bool canUndo() const { return !history.empty(); }

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
    editor.preview(GENERATED_FOLDER + PREVIEW_FILE);

    int choice;
    bool done = false;
    while (!done) {
        cout << "\n--- Welcome to your Image Editor ---\n";
        cout << "1. Grayscale\n";
        cout << "2. Black & White\n";
        cout << "3. Invert Colors\n";
        cout << "4. Merge with Image\n";
        cout << "5. Flip Image\n";
        cout << "6. Rotate Image\n";
        cout << "7. Brighten/Darken\n";
        cout << "8. Crop Image\n";
        cout << "9. Add Frame\n";
        cout << "10. Detect Edges\n";
        cout << "11. Resize Image\n";
        cout << "12. Blur\n";
        cout << "13. Sunlight Filter\n";
        cout << "14. Oil Painting\n";
        cout << "15. Old TV Filter\n";
        cout << "16. Purple Filter\n";
        cout << "17. Skew Image\n";
        cout << "18. Undo\n";
        cout << "19. Exit\n";
        cout << "20. Save & Exit\n";
        cout << "Enter choice: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> choice;

        if(cin.fail()) {
            cout << "Invalid input! Please enter a valid choice number.\n";
            continue; 
        }

        switch (choice) {
            case 1: editor.grayscale(); cout << "Grayscale filter applied!\n"; break;
            case 2: editor.blackAndWhite(); cout << "Black and white filter applied!\n"; break;
            case 3: editor.invert(); cout << "Colors inverted!\n"; break;
            case 4: {
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
            case 5: {
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
                double factor;
                cout << "Enter brightness factor (>1 to brighten, <1 to darken): ";
                cin >> factor;
                editor.changeBrightness(factor);
                cout << "Brightness adjusted with factor " << factor << "!\n";
                break;
            }
            case 8: {
                Image current = editor.getCurrentImage();
                int startX, startY, width, height;
                cout << "Current image size: " << current.width << "x" << current.height << " pixels\n";
                cout << "Enter starting position (x y): ";
                cin >> startX >> startY;
                cout << "Enter desired crop size (width height): ";
                cin >> width >> height;
                
                if (startX >= 0 && startY >= 0 && width > 0 && height > 0 &&
                    startX + width <= current.width && startY + height <= current.height) {
                    editor.crop(startX, startY, width, height);
                    cout << "Image cropped successfully to " << width << "x" << height << " pixels!\n";
                } else {
                    cout << "Invalid crop parameters! Make sure the crop region fits within the image.\n";
                }
                break;
            }
            case 9: {
                int thicknessChoice;
                int thickness;
                
                cout << "Frame thickness:\n";
                cout << "1. Auto (recommended based on image size)\n";
                cout << "2. Custom thickness\n";
                cout << "Enter choice: ";
                cin >> thicknessChoice;
                
                if (thicknessChoice == 1) {
                    thickness = editor.calculateSuitableThickness();
                    cout << "Auto-calculated thickness: " << thickness << " pixels\n";
                } else if (thicknessChoice == 2) {
                    cout << "Enter frame thickness (in pixels): ";
                    cin >> thickness;
                    
                    if (thickness <= 0) {
                        cout << "Invalid thickness! Must be positive.\n";
                        break;
                    }
                } else {
                    cout << "Invalid choice! Using auto thickness.\n";
                    thickness = editor.calculateSuitableThickness();
                }
                
                int frameStyle;
                cout << "\nFrame style:\n";
                cout << "1. Simple (solid border)\n";
                cout << "2. Decorated (ornamental)\n";
                cout << "Enter choice: ";
                cin >> frameStyle;
                
                bool decorated = (frameStyle == 2);
                
                cout << "\nChoose frame color:\n";
                cout << "1. Black\n";
                cout << "2. White\n";
                cout << "3. Gold\n";
                cout << "4. Silver\n";
                cout << "5. Wood Brown\n";
                cout << "6. Red\n";
                cout << "7. Blue\n";
                cout << "Enter choice: ";
                
                int colorChoice;
                cin >> colorChoice;
                
                unsigned char r = 0, g = 0, b = 0;
                
                switch (colorChoice) {
                    case 1: r = 0; g = 0; b = 0; break;           // Black
                    case 2: r = 255; g = 255; b = 255; break;     // White
                    case 3: r = 212; g = 175; b = 55; break;      // Gold
                    case 4: r = 192; g = 192; b = 192; break;     // Silver
                    case 5: r = 139; g = 90; b = 43; break;       // Wood Brown
                    case 6: r = 255; g = 0; b = 0; break;         // Red
                    case 7: r = 0; g = 0; b = 255; break;         // Blue
                    default:
                        cout << "Invalid choice! Using gold frame.\n";
                        r = 212; g = 175; b = 55;
                }
                
                editor.addFrame(thickness, r, g, b, decorated);
                cout << "Frame added successfully!\n";
                break;
            }
            case 10: editor.edgeDetection(); cout << "Edge detection applied!\n"; break;
            case 11: {
                Image current = editor.getCurrentImage();
                cout << "Current image size: " << current.width << "x" << current.height << " pixels\n";
                cout << "\nResize options:\n";
                cout << "1. Specify new dimensions (width x height)\n";
                cout << "2. Scale by ratio/percentage\n";
                cout << "Enter choice: ";
                
                int resizeChoice;
                cin >> resizeChoice;
                
                int newWidth, newHeight;
                
                if (resizeChoice == 1) {
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
                } else if (resizeChoice == 2) {
                    double ratio;
                    cout << "Enter scale ratio (e.g., 0.5 for 50%, 2.0 for 200%): ";
                    cin >> ratio;
                    
                    if (ratio > 0 && ratio <= 10) {
                        newWidth = static_cast<int>(current.width * ratio);
                        newHeight = static_cast<int>(current.height * ratio);
                        
                        if (newWidth > 0 && newHeight > 0) {
                            cout << "Resizing image by " << (ratio * 100) << "%...\n";
                            editor.resizeImage(editor.getCurrentImage(), newWidth, newHeight);
                            cout << "Image resized to " << newWidth << "x" << newHeight << " pixels!\n";
                        } else {
                            cout << "Resulting dimensions too small!\n";
                        }
                    } else {
                        cout << "Invalid ratio! Must be between 0 and 10.\n";
                    }
                } else {
                    cout << "Invalid choice!\n";
                }
                break;
            }
            case 12: {
                int blurRadius;
                cout << "Select blur strength:\n";
                cout << "1. Light (radius 3)\n";
                cout << "2. Medium (radius 5)\n";
                cout << "3. Strong (radius 15)\n";
                cout << "4. Custom radius\n";
                cout << "Enter choice: ";
                
                int blurChoice;
                cin >> blurChoice;
                
                switch(blurChoice) {
                    case 1: blurRadius = 3; break;
                    case 2: blurRadius = 5; break;
                    case 3: blurRadius = 15; break;
                    case 4:
                        cout << "Enter custom blur radius (1-100): ";
                        cin >> blurRadius;
                        if (blurRadius < 1 || blurRadius > 100) {
                            cout << "Invalid radius! Using medium blur (5).\n";
                            blurRadius = 5;
                        }
                        break;
                    default:
                        cout << "Invalid choice! Using medium blur.\n";
                        blurRadius = 5;
                }
                
                cout << "Applying blur with radius " << blurRadius << "...\n";
                editor.boxBlur(blurRadius);
                cout << "Blur filter applied successfully!\n";
                break;
            }
            case 13: editor.sunlightFilter(0.05); cout << "Sunlight filter applied!\n"; break;
            case 14: {
                cout << "Applying oil painting effect...\n";
                editor.oilPainting();
                cout << "Oil painting filter applied successfully!\n";
                break;
            }
            case 15: {
                double noiseLevel = 0.35;
                double scanlineIntensity = 0.7;
                double distortionLevel = 35.0;
                cout << "Applying Old TV filter...\n";
                editor.oldTVFilter(noiseLevel, scanlineIntensity, distortionLevel);
                cout << "Old TV filter applied!\n";
                break;
            }
            case 16: editor.purpleFilter(); cout << "Purple filter applied!\n"; break;
            case 17: {
                double skewDegree = 40.0;
                /*
                // Not needed for now, using default 40 degrees
                cout << "Enter skew angle in degrees (positive = right lean, negative = left lean):\n";
                cout << "range: -45 to 45 degrees\n";
                cout << "Skew degree: ";
                cin >> skewDegree;
                
                if (skewDegree < -45 || skewDegree > 45) {
                    cout << "skew degree Must be between -45 and 45 for good results.\n";
                    break;
                }*/
                
                if (skewDegree == 0) {
                    cout << "No skew applied (0 degrees).\n";
                } else {
                    cout << "Applying skew transformation with " << skewDegree << " degrees...\n";
                    editor.skewImage(skewDegree);
                    cout << "Skew filter applied successfully!\n";
                }
                break;
            }
            case 18: {
                if (editor.canUndo()) {
                    editor.undo();
                    cout << "Last action undone!\n";
                } else {
                    cout << "No actions to undo!\n";
                }
                break;
            }
            case 19: {
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
            case 20: {
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
