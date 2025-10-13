#define STB_IMAGE_WRITE_STATIC

#include "ImageEditor.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;

// keeping up to 10 undos
void ImageEditor::pushToHistory() {
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


ImageEditor::ImageEditor(const std::string& path)
    : originalImage(path), currentImage(path) {}

void ImageEditor::flipVertical() {
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

void ImageEditor::flipHorizontal() {
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

void ImageEditor::blackAndWhite() {
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

void ImageEditor::grayscale() {
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

void ImageEditor::invert() {
    pushToHistory();
    for (int i = 0; i < currentImage.height; i++) {
        for (int j = 0; j < currentImage.width; j++) {
            for (int c = 0; c < 3; c++) {
                currentImage(j, i, c) = 255 - currentImage(j, i, c);
            }
        }
    }
}

void ImageEditor::changeBrightness(double factor) {
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

Image ImageEditor::resizeImage(const Image& src, int newW, int newH, bool forMerge) {
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

void ImageEditor::mergeWithImage(const std::string& imagePath) {
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

    void ImageEditor::rotate(int angleDegrees) {
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

    int ImageEditor::calculateSuitableThickness() {
        int minDim = min(currentImage.width, currentImage.height);
        int thickness = max(8, minDim / 34);
        return thickness;
    }

    void ImageEditor::addFrame(int thickness, unsigned char r, unsigned char g, unsigned char b, bool decorated) {
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
    void ImageEditor::edgeDetection() {
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

    void ImageEditor::purpleFilter() {
        pushToHistory();
        for (int y = 0; y < currentImage.height; y++) {
            for (int x = 0; x < currentImage.width; x++) {

                unsigned char r = currentImage(x, y, 0);
                unsigned char g = currentImage(x, y, 1);
                unsigned char b = currentImage(x, y, 2);

                int newR = min(255, int(r + 20));
                int newG = max(0, int(g - 70));
                int newB = min(255, int(b + 10));

                currentImage(x, y, 0) = newR;
                currentImage(x, y, 1) = newG;
                currentImage(x, y, 2) = newB;
            }
        }
    }

    void ImageEditor::sunlightFilter(double intensity) {
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

    void ImageEditor::oldTVFilter(double noiseLevel, double scanlineIntensity, double distortionLevel) {
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

    void ImageEditor::skewImage(double skewDegree) {
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
    void ImageEditor::boxBlur(int radius) {
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

    void ImageEditor::crop(int startX, int startY, int width, int height) {
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

    void ImageEditor::oilPainting(int radius, int intensityLevels) {
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

    void ImageEditor::waveDistortion() {
        pushToHistory();
        Image result = currentImage;
        int width = currentImage.width;
        int height = currentImage.height;

        double intensity = 1.0;
        double frequency = 8.0;
        double amplitude = intensity * 5.0; // wave height in pixels

        for (int y = 0; y < height; y++) {
            double offset = sin(y / frequency) * amplitude;

            for (int x = 0; x < width; x++) {
                int srcX = std::clamp(int(x + offset), 0, width - 1);
                for (int c = 0; c < 3; c++) {
                    result(x, y, c) = currentImage(srcX, y, c);
                }
            }
        }

        currentImage = result;
    }

    void ImageEditor::reflect(double fadeStrength, bool addRipple, bool horizontal) {
        pushToHistory();
        int width = currentImage.width;
        int height = currentImage.height;
        
        int newWidth = horizontal ? width : width * 2;
        int newHeight = horizontal ? height * 2 : height;

        Image result(newWidth, newHeight);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                for (int c = 0; c < 3; c++) {
                    result(x, y, c) = currentImage(x, y, c);
                }
            }
        }

        if (horizontal) {
            for (int y = 0; y < height; y++) {
                double fade = 1.0 - (y / (double)height) * fadeStrength; 
                for (int x = 0; x < width; x++) {
                    int srcY = height - 1 - y; 

                    double rippleOffset = 0.0;
                    if (addRipple) {
                        rippleOffset = sin(x * 0.06 + y * 0.15) * 3.0; 
                        rippleOffset += sin(x * 0.02 + y * 0.05) * 1.5;
                    }

                    int distortedX = std::clamp((int)(x + rippleOffset), 0, width - 1);

                    for (int c = 0; c < 3; c++) {
                        double px = currentImage(distortedX, srcY, c) * fade;

                        if (c == 2) px *= 1.03;
                        if (c == 0) px *= 0.97;  

                        result(x, height + y, c) = std::clamp((int)px, 0, 255);
                    }
                }
            }
        } else {
            for (int x = 0; x < width; x++) {
                double fade = 1.0 - (x / (double)width) * fadeStrength; 
                for (int y = 0; y < height; y++) {
                    int srcX = width - 1 - x; 

                    double rippleOffset = 0.0;
                    if (addRipple) {
                        rippleOffset = sin(y * 0.06 + x * 0.15) * 3.0; 
                        rippleOffset += sin(y * 0.02 + x * 0.05) * 1.5; 
                    }

                    int distortedY = std::clamp((int)(y + rippleOffset), 0, height - 1);

                    for (int c = 0; c < 3; c++) {
                        double px = currentImage(srcX, distortedY, c) * fade;

                        if (c == 2) px *= 1.03;  
                        if (c == 0) px *= 0.97;  

                        result(width + x, y, c) = std::clamp((int)px, 0, 255);
                    }
                }
            }
        }

        currentImage = result;
    }

    void ImageEditor::glitch(double intensity) {
        pushToHistory();
        int width = currentImage.width;
        int height = currentImage.height;
        Image result = currentImage;

        int numBands = static_cast<int>(5 * intensity);
        int maxShift = static_cast<int>(width * 0.03 * intensity);
        double colorShiftAmount = 0.02 * intensity;

        srand(time(0));

        int rShift = rand() % static_cast<int>(maxShift * 2) - maxShift;
        int gShift = rand() % static_cast<int>(maxShift * 2) - maxShift;
        int bShift = rand() % static_cast<int>(maxShift * 2) - maxShift;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int xr = std::clamp(x + rShift, 0, width - 1);
                int xg = std::clamp(x + gShift, 0, width - 1);
                int xb = std::clamp(x + bShift, 0, width - 1);

                result(x, y, 0) = currentImage(xr, y, 0); 
                result(x, y, 1) = currentImage(xg, y, 1); 
                result(x, y, 2) = currentImage(xb, y, 2); 
            }
        }

        for (int i = 0; i < numBands; i++) {
            int yStart = rand() % height;
            int bandHeight = 3 + rand() % 20;
            int shift = (rand() % (2 * maxShift)) - maxShift;

            for (int y = yStart; y < std::min(yStart + bandHeight, height); y++) {
                for (int x = 0; x < width; x++) {
                    int newX = std::clamp(x + shift, 0, width - 1);
                    for (int c = 0; c < 3; c++) {
                        result(x, y, c) = currentImage(newX, y, c);
                    }
                }
            }
        }

        for (int i = 0; i < width * height * 0.002 * intensity; i++) {
            int x = rand() % width;
            int y = rand() % height;
            for (int c = 0; c < 3; c++) {
                result(x, y, c) = rand() % 256;
            }
        }

        for (int y = 0; y < height; y++) {
            if (rand() % 200 < 1) { 
                double tint = 0.6 + (rand() % 40) / 100.0;
                for (int x = 0; x < width; x++) {
                    result(x, y, 0) *= tint;      
                    result(x, y, 2) *= 1.0 - tint / 4.0; 
                }
            }
        }

        for (int y = 0; y < height; y++) {
            double factor = (y % 2 == 0) ? 0.94 : 1.0;
            for (int x = 0; x < width; x++) {
                for (int c = 0; c < 3; c++) {
                    result(x, y, c) = std::clamp(int(result(x, y, c) * factor), 0, 255);
                }
            }
        }

        currentImage = result;
    }

    void ImageEditor::undo() {
        if (!history.empty()) {
            currentImage = history.top();
            history.pop();
        }
    }

    void ImageEditor::save(const std::string& path) {
        currentImage.saveImage(path);
    }

    void ImageEditor::preview(const std::string& path) {
        currentImage.saveImage(path);
    }

    Image ImageEditor::getCurrentImage() const { return currentImage; }

    bool ImageEditor::isValidImageFormat(const string& filename) {
        size_t lastDot = filename.find_last_of('.');
        if (lastDot == string::npos) return false;
        
        string extension = filename.substr(lastDot + 1);
        transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
        
        return (std::find(supportedFormats.begin(), supportedFormats.end(), extension) != supportedFormats.end());
    }

    string ImageEditor::promptForValidFilename(const string& defaultName) {
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

    bool ImageEditor::canUndo() const { return !history.empty(); }

    vector<int> ImageEditor::getSupportedAngles() const { return supportedAngles; }

