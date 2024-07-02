#include "raylib.h"
#include "game.h"
#include "defines.h"
#include "imageDraw.h"

Image duckies;
Image chair;
Image fish;

Texture2D gameGUI;
Texture2D upgradesGUI;

static int score = 0;
static int upgradeValues[UPGRADES_AMOUNT];
static float powerRunoutTime = 0;
static float gameTime = 0;

static int reward = 1;
static int imageMax = 0;

static bool quest1Done = false;
static bool quest2Done = false;

static bool displayUpgrades = false;

static void loadBar(float progress){
    // Draw bar. 
    Color barcolor = GREEN;
    if(quest1Done && quest2Done)barcolor = ORANGE;
    int loadStartingY = 50;
    DrawRectangle(0, loadStartingY, SCREEN_WIDTH, 100, DARKGRAY);
    DrawRectangle(10, loadStartingY + 10, (SCREEN_WIDTH - 20) * (progress), 80, barcolor);
}

int gameOver(){
    // Reset game on mouse click.
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        if(CheckCollisionPointRec(GetMousePosition(), 
        (Rectangle){100, 100, 100, 100})){
            setupImageLoading(0);
            powerRunoutTime = GetTime() + BATTERY_LIFE;
            for(int i = 0; i < UPGRADES_AMOUNT; i++){
                upgradeValues[i] = 1;
            }
            reward = 1;
            imageMax = 0;
            upgradeValues[1] = 10;
            upgradeValues[2] = 20;
            gameTime = GetTime();
            quest1Done = false;
            quest2Done = false;
            
            score = 0;
            return 1;
        }
    }
    BeginDrawing();
        ClearBackground(BLACK);

        DrawText("GAME OVER", GetScreenWidth()/2 - (MeasureText("GAMEOVER", 50)/2), GetScreenHeight()/3, 50,RED);
        DrawText(TextFormat("Survived %f seconds.", gameTime),
         GetScreenWidth()/2 - (MeasureText(TextFormat("Survived %f seconds.", gameTime), 30)/2), GetScreenHeight()/2, 30,RED);
        DrawRectangle(100, 100, 100, 100, GREEN);
    EndDrawing();
    return 2;
}

void upgradesPanel(bool drawing){
    if(drawing){
        DrawTexture(upgradesGUI, UPGRADES_OFFSET_X, UPGRADES_OFFSET_Y, RAYWHITE);
        // 1st upgrade
        if(upgradeValues[1]<=0)DrawRectangle(UPGRADES_OFFSET_X + 40, 
        UPGRADES_OFFSET_Y + 70, 240, 30, RED);
        DrawText("More points + images -> 10", UPGRADES_OFFSET_X + 160 - (MeasureText("More points + images -> 10", 20) / 2)
        , UPGRADES_OFFSET_Y + 75, 20, BLACK);
        // 2nd upgrade
        if(upgradeValues[2]<=0)DrawRectangle(UPGRADES_OFFSET_X + 40, 
        UPGRADES_OFFSET_Y + 130, 240, 30, RED);
        DrawText("More points + images -> 20", UPGRADES_OFFSET_X + 160 - (MeasureText("More points + images -> 20", 20) / 2)
        , UPGRADES_OFFSET_Y + 135, 20, BLACK);
    }else{
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if(CheckCollisionPointRec(GetMousePosition()
            , (Rectangle){UPGRADES_OFFSET_X + 10, UPGRADES_OFFSET_Y + 10, 30, 30})){
                displayUpgrades = false;
            }else if(CheckCollisionPointRec(GetMousePosition()
            , (Rectangle){UPGRADES_OFFSET_X + 40, UPGRADES_OFFSET_Y + 70, 240, 30})
            && upgradeValues[1] > 0 && score >= upgradeValues[1]){
                // Upgrade loading speed
                score -= upgradeValues[1];
                upgradeValues[1] = 0;
                reward *= 2;
                imageMax++;
            }else if(CheckCollisionPointRec(GetMousePosition()
            , (Rectangle){UPGRADES_OFFSET_X + 40, UPGRADES_OFFSET_Y + 130, 240, 30})
            && upgradeValues[2] > 0 && score >= upgradeValues[2]){
                score -= upgradeValues[2];
                upgradeValues[2] = 0;
                reward *= 2;
                imageMax++;
            }
        }
    }
}

int inGame(){
    // Progress image loading.
    if(quest1Done && quest2Done){
        updateImage(SPEEDUP_MULTIPLIER);
    }else{
        updateImage(1);
    }
    // Do score adding and reset.
    if(getLoadProgress() >= 1){
        score += reward;
        setupImageLoading(GetRandomValue(0,imageMax));
        quest1Done = false;
        quest2Done = false;
    }
    // Do power.
    if(powerRunoutTime - GetTime() < 0){
        gameTime = GetTime() - gameTime;
        return 2;
    }
    // Process mouse for upgrades or ingame.
    if(displayUpgrades)upgradesPanel(false);
    else{
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            // Refill power.
            if(CheckCollisionPointRec(GetMousePosition(), 
            (Rectangle){25, 5, 61, 40}) && score >= upgradeValues[0]){
                score -= upgradeValues[0];
                powerRunoutTime = GetTime() + BATTERY_LIFE;
                upgradeValues[0] *= 2;
            }
            // Upgrade speed.
            if(CheckCollisionPointRec(GetMousePosition(), 
            (Rectangle){50, 400, 200, 50})){
                displayUpgrades = true;
            }
            // Check for quest completion.
            
            int questType = imageInputDetector();
            if(questType == 1 && !quest1Done){
                quest1Done = true;
                score += reward;
            }else if(questType == 2 && !quest2Done){
                quest2Done = true;
                score += reward;
            }
        }
    }
    BeginDrawing();
        ClearBackground(GRAY);
        // Display the status.
        DrawTexture(gameGUI, 0, 0, RAYWHITE);
        DrawFPS(0,0);
        // Draw progress bar.
        loadBar(getLoadProgress());
        // Powermeter.
        DrawRectangle(25, 5, (61) * (powerRunoutTime-GetTime()) / BATTERY_LIFE, 40, GREEN);
        DrawText(TextFormat("%d", upgradeValues[0]), 100, 10, 40, GREEN);
        // Upgrade 1.
        DrawRectangle(50, 400, 200, 50, GREEN);
        DrawText("Access Upgrades", 150 - (MeasureText("Access Upgrades", 20) / 2), 415, 20, BLACK);
        // Quest 1.
        DrawRectangle(360, 410, 180 * (int)quest1Done, 30, GREEN);
        DrawText(getQuestPrompt(1), 450 - (MeasureText(getQuestPrompt(1), 20) / 2), 415, 20, BLACK);
        // Quest 2.
        DrawRectangle(360, 510, 180 * (int)quest2Done, 30, GREEN);
        DrawText(getQuestPrompt(2), 450 - (MeasureText(getQuestPrompt(2), 20) / 2), 515, 20, BLACK);
        // Calculate image loading.
        drawImage(211, 161);
        // Draw text.
        DrawText(TextFormat("%d",score), GetScreenWidth() - 40, 15, 30, RED);
        // Update upgrades panel if needed.
        if(displayUpgrades)upgradesPanel(true);
    EndDrawing();
    return 1;
}

void loadGame(){
    powerRunoutTime = GetTime() + BATTERY_LIFE;
    for(int i = 0; i < UPGRADES_AMOUNT; i++){
        upgradeValues[i] = 1;
    }
    upgradeValues[1] = 10;
    upgradeValues[2] = 20;

    duckies = LoadImage("images/duck.png");
    ImageResize(&duckies, 179, 179);

    chair = LoadImage("images/chair.png");
    ImageResize(&chair, 179, 179);

    fish = LoadImage("images/fish.png");
    ImageResize(&fish, 179, 179);

    gameGUI = LoadTexture("images/gameGUI.png");
    upgradesGUI = LoadTexture("images/upgrades.png");
    // CHANGE TO GETRANDOM.
    setupImageLoading(0);
    gameTime = GetTime();
}

void unloadGame(){
    UnloadImage(duckies);
    UnloadImage(chair);
    UnloadImage(fish);
    UnloadTexture(gameGUI);
    UnloadTexture(upgradesGUI);
}