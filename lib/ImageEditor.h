#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include <string>
#include <stack>
#include "Image_Class.h"
#include <vector>

class ImageEditor {
private:
    Image originalImage;       
    Image currentImage;
    std::stack<Image> history;
    static const int MAX_HISTORY_SIZE = 10;
    std::vector<int> supportedAngles = {0, 90, -90, 180, 270, -270};
    std::vector<std::string> supportedFormats = {"jpg", "jpeg", "png", "bmp", "tga"};
    
    void pushToHistory();

public:
    ImageEditor(const std::string& path);

    void flipVertical();
    void flipHorizontal();
    void blackAndWhite();
    void grayscale();
    void invert();
    void changeBrightness(double factor);
    Image resizeImage(const Image& src, int newW, int newH, bool forMerge = false);
    void mergeWithImage(const std::string& imagePath);
    void rotate(int angleDegrees);
    int calculateSuitableThickness();
    void addFrame(int thickness, unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, bool decorated = false);
    void edgeDetection();
    void purpleFilter();
    void sunlightFilter(double intensity = 0.02);
    void oldTVFilter(double noiseLevel = 0.15, double scanlineIntensity = 0.7, double distortionLevel = 20.0);
    void skewImage(double skewDegree);
    void boxBlur(int radius = 5);
    void crop(int startX, int startY, int width, int height);
    void oilPainting(int radius = 3, int intensityLevels = 10);
    void waveDistortion();
    void reflect(double fadeStrength = 0.6, bool addRipple = true, bool horizontal = true);
    void glitch(double intensity = 1.0);
    void infrared();

    void undo();
    void save(const std::string& path);
    void preview(const std::string& path);
    Image getCurrentImage() const;
    bool isValidImageFormat(const std::string& filename);
    std::string promptForValidFilename(const std::string& defaultName);
    bool canUndo() const;
    std::vector<int> getSupportedAngles() const;
};

#endif
