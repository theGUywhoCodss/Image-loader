#ifndef _IMAGE_DRAW_H
#define _IMAGE_DRAW_H

#include "raylib.h"

void drawImage(int startX, int startY);
void updateImage(int speedMultiplier);
float getLoadProgress();
void setupImageLoading(int type);

void updatePixelSpeed(int value);

int imageInputDetector();
char* getQuestPrompt(int type);

#endif