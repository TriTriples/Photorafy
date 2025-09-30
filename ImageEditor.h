#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include <string>
#include <stack>
#include "lib/Image_Class.h"
#include <vector>

class ImageEditor {
private:
    Image originalImage;       
    Image currentImage;
    std::stack<Image> history;
    std::vector<int> supportedAngles = {0, 90, -90, 180, 270, -270};

public:
    ImageEditor(const std::string& path);

    void flipVertical();
    void flipHorizontal();
    void blackAndWhite();
    void grayscale();
    void invert();
    void changeBrightness(double factor);
    void resizeImage(const Image& src, int newW, int newH);
    void mergeWithImage(const std::string& imagePath);
    void rotate(int angleDegrees);

    void undo();
    void save(const std::string& path);
    void preview(const std::string& path);
    const std::vector<int>& getSupportedAngles() const { return supportedAngles; }
    const Image& getImage() const { return currentImage; }
};

#endif
