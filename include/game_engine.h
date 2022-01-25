#pragma once
#include <iostream>
#include <string>
#include <list>
#include "cinder/gl/gl.h"
#include "cinder/app/App.h"

using std::string;
using std::vector;
using std::map;
using glm::vec2;
using ci::app::KeyEvent;
using ci::app::MouseEvent;
using ci::Rectf;

namespace flappybird{
class GameEngine {
  public:
    GameEngine();
    /**
     * Draws the bird and obstacles
     */
    void Display();
    
    /**
     * Changes the position of bird and obstacles
     * Also changes the velocity and acceleration of the bird
     */
    void AdvanceOneFrame();

    /**
     * When the user presses space, this method sets the bird's velocity to a positive value and acceleration to zero
     * so that the bird propels up
     * It also allows the user to press space to go back to the start screen once the game is over
     * @param event 
     */
    void keyDown(const KeyEvent &event);

    /**
     * This method checks if a user has clicked on a game button and then performs the actions of that button
     * @param event 
     */
    void mouseDown(const MouseEvent &event);

    //only the y velocity is considered for the bird so I didn't need to use a vec2 for velocity
    //the bird stays in the same x position while the obstacles move closer
    struct Bird {
        Bird(float set_x, float set_y, const char* color_, float set_radius);
        vec2 position_;
        float y_velocity_ = 0.0;
        float acceleration_ = 0.0;
        float radius_;
        char* color_;
        const char* kOutlineColor = "black";
        const float kOutlineWidth = 1.5;
        float gravity_ = 0.2;
        bool started_ = false;
        bool has_collided_ = false;
        void Display() const;
        void UpdateBird();
        void SetStarted(bool set_started);
        bool GetStarted() const;
        void SetPosition(float x_position, float y_position);
    };

    struct Obstacle {
        Rectf upper_main_;
        Rectf lower_main_;
        Rectf upper_secondary_;
        Rectf lower_secondary_;
        char* color_;
        float pipe_width_ = 10;
        Obstacle(Rectf set_upper_main, Rectf set_lower_main, Rectf set_upper_secondary, Rectf set_lower_secondary, const char * set_color);
        void Display() const;
    };

    struct Ground {
        Rectf top_;
        Rectf bottom_;
        char* top_color_;
        char* bottom_color_;
        Ground(Rectf set_top, Rectf set_bottom, const char * set_top_color, const char * set_bottom_color);
        void Display() const;
    };

    struct Button {
        Rectf area_;
        char* color_;
        string title_;
        bool highlighted_ = false;
        const string kGameFont = "Times New Roman";
        const char* kGameTextColor = "white";
        const char* kHighlightColor = "darkgray";
        const float kPositionAverage = 2;
        const float kTitlePositionDivider = 3;
        const float kHighlightWidthDivider = 10;
        float font_size_;
        Button(Rectf set_area, const char * set_color, string set_title, float set_font_size);
        void Display() const;
    };

    struct Leaderboard {
        Leaderboard();
        void Display();
        vector<size_t> scores_ = {0, 0, 0, 0, 0};
        void ManageScores();
        const string kGameFont = "Times New Roman";
        const char* kGameTextColor = "white";
        const float kLineGap = 60;
        const string kLeaderboardTitle = "Leaderboard";
        const string kDot = ".";
        const float kLeaderboardTitleX_Position = 300;
        const float kLeaderboardTitleY_Position = 100;
        const float kLeaderboardTitleFontSize = 40;
        const size_t kLeaderboardPositions = 5;
        const float kFirstLineX1_Position = 100;
        const float kFirstLineX2_Position = 500;
        const float kFirstLineY1_Position = 150;
    };

    // GameState enum that helps decide what to display
    enum GameState {
        StartScreen,
        GameScreen,
        GameOverScreen,
        LeaderBoard,
        CustomizeScreen
    };

    /**
     * Getters and Setters for Testing Purposes 
     */
    vector<Obstacle> GetObstacles();
    void SetGameState(GameState game_state);
    Bird GetBird();
    size_t GetScore() const;
    bool GetHasCollided() const;

  private:
    /**
     * Moves obstacles to the left by updating their rectangle vectors
     */
    void UpdateObstacles();

    /**
     * Removes obstacles from the obstacle once they move out of the window and then adds a new one
     */
    void UpdateObstacleVector();

    /**
     * If the bird passes an object, this method increments the score
     */
    void UpdateScore();

    /**
     * Checks if the bird has touched an obstacle and sets has_collided_ to true
     * Also sets the bird acceleration so that it falls down in a line
     */
    void HandleCollision();

    /**
     * This method brings about the Game Over screen once the falling bird touches the ground
     */
    void HandleDeath();
    
    /**
     * This method resets the game variables to their original values;
     */
    void ResetGame();
    
    /**
     * Screen Display methods that draw what is necessary for the respective screen when the current games screen 
     * is that screen
     */
    void DisplayStartScreen();
    void DisplayCustomizeScreen();
    void DisplayLeaderboard();
    void DisplayGameScreen();
    void DisplayGameOverScreen();

    // size of the game window
    const float kWindowSize = 600;
    
    // score variable that keeps track of score
    size_t score_ = 0;

    // Bird class fields and constants
    bool has_collided_ = false;  
    const float kX_Position = 150.0;
    const float kInitialY_Position = kWindowSize / 2;
    const char* kBirdColor = "yellow";
    const float kRadius = 10.0;
    Bird bird_ = Bird(kX_Position, kInitialY_Position, kBirdColor, kRadius);
    const float kBirdDeathAcceleration = 0.25;

    // Ground class fields and constants
    const float kTopHeight = 8;
    const float kBottomHeight = 40;
    const float kGroundHeight = kTopHeight + kBottomHeight;
    const char* kGroundTopColor = "green";
    const char* kGroundBottomColor = "brown";
    Ground ground_ = Ground(Rectf(vec2(0, kWindowSize - kBottomHeight - kTopHeight),
                            vec2(kWindowSize, kWindowSize - kTopHeight)),
                            Rectf(vec2(0, kWindowSize - kBottomHeight),
                            vec2(kWindowSize, kWindowSize)),
                            kGroundTopColor, kGroundBottomColor);

    // Obstacle class fields and constants
    vector<Obstacle> obstacles_;
    const char* kObstacleColor = "green";
    const float kNumObstaclesOnScreen = 2;
    const float kStartingIncrement = 700;
    const float kGapSize = 95;
    const float kObstacleWidth = 50;
    const float kLowerBoundDivider = 4; 
    float ObstacleSpeed = 2;
    const float kSecondaryPipeWidth = 10;
    const float kSecondaryPipeHeight = 50;
    const size_t kObstacleRange = 401 - kGroundHeight;
    
    // The current game screen
    GameState current_game_state_ = StartScreen;
    
    // Game Buttons and their constants
    Button start_leaderboard_ = Button(Rectf(vec2(450, 500), vec2(550, 525)), "red", "Leaderboard", 15);
    Button start_customize_ = Button(Rectf(vec2(450, 465), vec2(550, 490)), "purple", "Customize", 15);
    Button start_challenge_ = Button(Rectf(vec2(450, 430), vec2(550, 455)), "black", "Challenge", 15);
    Button start_normal_ = Button(Rectf(vec2(450, 395), vec2(550, 420)), "yellowgreen", "Normal", 15);
    Button gameover_restart_ = Button(Rectf(vec2(100, 400), vec2(275 , 500)), "orange", "Restart", 30);
    Button gameover_leaderboard_ = Button(Rectf(vec2(325, 400), vec2(500 , 500)), "red", "Leaderboard", 30);
    Button back_ = Button(Rectf(vec2(50, 50), vec2(150, 75)), "red", "Back", 15);
    Button customize_red_ = Button(Rectf(vec2(100, 150), vec2(200 , 250)), "red", "", 30);
    Button customize_yellow_ = Button(Rectf(vec2(250, 150), vec2(350 , 250)), "yellow", "", 30);
    Button customize_blue_ = Button(Rectf(vec2(400, 150), vec2(500, 250)), "blue", "", 30);
    Button customize_purple_ = Button(Rectf(vec2(100, 350), vec2(200 , 450)), "purple", "", 30);
    Button customize_orange_ = Button(Rectf(vec2(250, 350), vec2(350 , 450)), "orange", "", 30);
    Button customize_green_ = Button(Rectf(vec2(400, 350), vec2(500, 450)), "green", "", 30);
    
    Leaderboard leaderboard_ = Leaderboard();
    
    // Game String Constants
    const string kGameFont = "Times New Roman";
    const char* kGameTextColor = "white";
    const string kGameTitle = "Flappy Bird";
    const float kTitleX_Position = kWindowSize / 2;
    const float kTitleY_Position = kWindowSize / 12;
    const float kTitleFontSize = kWindowSize / 15;
    const string kInstruction = "Tap Space to Start";
    float kInstructionX_Position = kWindowSize / 2;
    float kInstructionY_Position = kWindowSize / 6;
    float kInstructionFontSize = kWindowSize / 30;

    const float kOptionFontSize = kWindowSize / 20;
    const string kOption_1 = "Select Bird Color:";
    const float kOption_1_X_Position = kWindowSize / 2;
    const float kOption_1_Y_Position = kWindowSize * 0.1875;
    const string kOption_2 = "Select Pipe Color:";
    const float kOption_2_X_Position = kWindowSize / 2;
    const float kOption_2_Y_Position = kWindowSize / 2;

    const float kScoreFontSize = kWindowSize / 24;
    const float kScore_X_Position = kWindowSize / 2;
    const float kScore_Y_Position = kWindowSize / 12;
    
    const string kGameOverTitle = "Game Over";
    const float kGameOverTitle_X_Position = kWindowSize / 2;
    const float kGameOverTitle_Y_Position = kWindowSize / 12;
    const float kGameOverTitleFontSize = 2 * kWindowSize / 15;
    
    const string kFinalScoreMessage = "Final Score: ";
    const float kFinalScoreMessage_X_Position = kWindowSize / 2;
    const float kFinalScoreMessage_Y_Position = kWindowSize / 2;
    const float kFinalScoreMessageFontSize = kWindowSize / 15;
    
    // Game Constants
    const char* kLeaderboardBackground = "gray";
    const char* kGameOverBackground = "blue";
    const float kObstacleDelay = 20;
    const float kFlapVelocity = -5;
    const float kFlapBoundary = kFlapVelocity / 2;
    const float kNormalObstacleSpeed = 2;
    const float kNormalGravity = 0.2;
    const float kChallengeObstacleSpeed = 5;
    const float kChallengeGravity = 0.5;
};
} // namespace flappybird
