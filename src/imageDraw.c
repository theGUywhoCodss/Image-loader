#include "imageDraw.h"
#include "defines.h"

extern Image duckies;
extern Image chair;
extern Image fish;

Image currentImage;

static float currentPixel = 0;
static double lastPixelTime = 0;
static int pixelCount = 2500;

static int imageSelected = 0;

void updatePixelSpeed(int value){
    pixelCount += value;
}

float getLoadProgress(){
    return (float)currentPixel / (duckies.height * duckies.width);
}

void ImageSelectionType(int imgKey){
    UnloadImage(currentImage);
    imageSelected = imgKey;
    // Set the current image with image key.
    if(imgKey == 0){
        currentImage = ImageCopy(duckies);
    }else if(imgKey == 1){
        currentImage = ImageCopy(chair);
    }else if(imgKey == 2){
        currentImage = ImageCopy(fish);

    }
}

void drawImage(int startX, int startY){
    bool endDisplay = false;

    for(int y = 0; y < currentImage.height; y++){
        for(int x = 0; x < currentImage.width; x++){
            DrawPixel(x + startX, y + startY, GetImageColor(currentImage, x, y));
            // CHANGE CURRENTPIXEL FOR FORMATTING.
            if(x + y * currentImage.width >= currentPixel){
                endDisplay = true;
                break;
            }
        }
        if(endDisplay){
            break;
        }
    }
}

void updateImage(int speedMultiplier){
    if(GetTime() - lastPixelTime >= 0.01){
        currentPixel += (pixelCount * speedMultiplier) * GetFrameTime();
        lastPixelTime = GetTime();
    }
}

void setupImageLoading(int type){
    lastPixelTime = GetTime();
    currentPixel = 0;
    ImageSelectionType(type);
}

char* getQuestPrompt(int type){
    if(imageSelected == 0){
        if(type == 1){
            return "Click the beak.";
        }else{
            return "Click the eye.";
        }
    }else if(imageSelected == 1){
        if(type == 1){
            return "Click the seat.";
        }else if(type == 2){
            return "Click a chair leg.";
        }
    }else if(imageSelected == 2){
        if(type == 1){
            return "Click the eye.";
        }else if(type == 2){
            return "Click the fin.";
        }
    }
    return "";
}

int imageInputDetector(){
    // Check image.
    if(imageSelected == 0){
        // Return side quest number.
        if(CheckCollisionPointRec(GetMousePosition(),
        (Rectangle){231,217,50,40})){
            return 1;
        }else if(CheckCollisionPointRec(GetMousePosition(),
        (Rectangle){285,191,30,30})){
            return 2;
        }
    }else if(imageSelected == 1){
        if(CheckCollisionPointRec(GetMousePosition(),
        (Rectangle){240,230,100,30})){
            return 1;
        }else if(CheckCollisionPointRec(GetMousePosition(),
        (Rectangle){241, 250, 20,60})||CheckCollisionPointRec(GetMousePosition(),
        (Rectangle){291, 260, 20,70})||CheckCollisionPointRec(GetMousePosition(),
        (Rectangle){331, 250, 20,60})){
            return 2;
        }
    }else if(imageSelected == 2){
        if(CheckCollisionPointRec(GetMousePosition(),
        (Rectangle){231, 236, 10, 10})){
            return 1;
        }else if(CheckCollisionPointRec(GetMousePosition(),
        (Rectangle){211 + 130, 161 + 70, 30,40})){
            return 2;
        }
    }
    return 0;
}
