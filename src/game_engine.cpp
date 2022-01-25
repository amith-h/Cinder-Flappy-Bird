#include <string>
#include <utility>
#include <game_engine.h>

namespace flappybird {
    
using std::string;
using std::greater;
using std::vector;
using std::to_string;
using std::sort;
using std::move;
using ci::Color;
using ci::Font;
using ci::Rectf;
using ci::gl::color;
using ci::gl::drawStringCentered;
using ci::gl::drawSolidRect;
using ci::gl::drawSolidCircle;
using ci::gl::drawStrokedCircle;
using ci::gl::drawStrokedRect;
using ci::gl::drawLine;
using ci::app::KeyEvent;
using ci::app::MouseEvent;

// GameEngine Constructor and Functions
GameEngine::GameEngine() {
    // These values must be true as soon as the game starts
    customize_yellow_.highlighted_ = true;
    customize_green_.highlighted_ = true;
    start_normal_.highlighted_ = true;
}

void GameEngine::Display() {
    DisplayStartScreen();
    DisplayCustomizeScreen();
    DisplayLeaderboard();
    DisplayGameScreen();
    DisplayGameOverScreen();
}

void GameEngine::DisplayStartScreen() {
    if (current_game_state_ == StartScreen) {
        Font title_font = Font(kGameFont, kTitleFontSize);
        drawStringCentered(kGameTitle, vec2(kTitleX_Position, kTitleY_Position), kGameTextColor, title_font);
        Font instruction_font = Font(kGameFont, kInstructionFontSize);
        drawStringCentered(kInstruction, vec2(kInstructionX_Position, kInstructionY_Position), kGameTextColor
                           , instruction_font);
        bird_.Display();
        ground_.Display();
        start_customize_.Display();
        start_leaderboard_.Display();
        start_challenge_.Display();
        start_normal_.Display();
    }
}

void GameEngine::DisplayCustomizeScreen() {
    if (current_game_state_ == CustomizeScreen) {
        back_.Display();
        Font option_font = Font(kGameFont, kOptionFontSize);
        drawStringCentered(kOption_1, vec2(kOption_1_X_Position, kOption_1_Y_Position), kGameTextColor, 
                           option_font);
        customize_red_.Display();
        customize_yellow_.Display();
        customize_blue_.Display();
        drawStringCentered(kOption_2, vec2(kOption_2_X_Position, kOption_2_Y_Position), kGameTextColor, 
                           option_font);
        customize_purple_.Display();
        customize_orange_.Display();
        customize_green_.Display();
    }
}

void GameEngine::DisplayLeaderboard() {
    if (current_game_state_ == LeaderBoard) {
        color(Color(kLeaderboardBackground));
        drawSolidRect(Rectf(vec2(0, 0), vec2(kWindowSize, kWindowSize)));
        leaderboard_.Display();
        back_.Display();
    }
}

void GameEngine::DisplayGameScreen() {
    if (current_game_state_ == GameScreen) {
        for (const Obstacle& obstacle: obstacles_) {
            obstacle.Display();
        }
        bird_.Display();
        ground_.Display();
        Font score_font = Font(kGameFont, kScoreFontSize);
        drawStringCentered(to_string(score_), vec2(kScore_X_Position, kScore_Y_Position), kGameTextColor, 
                           score_font);
    }
}

void GameEngine::DisplayGameOverScreen() {
    if (current_game_state_ == GameOverScreen) {
        color(Color(kGameOverBackground));
        drawSolidRect(Rectf(vec2(0, 0), vec2(kWindowSize, kWindowSize)));
        Font TitleFont = Font(kGameFont, kGameOverTitleFontSize);
        drawStringCentered(kGameOverTitle, vec2(kGameOverTitle_X_Position, kGameOverTitle_Y_Position), 
                           kGameTextColor, TitleFont);
        Font kScoreFont = Font(kGameFont, kFinalScoreMessageFontSize);
        drawStringCentered(kFinalScoreMessage + to_string(score_), vec2(kFinalScoreMessage_X_Position, 
                                                                        kFinalScoreMessage_Y_Position),
                           kGameTextColor, kScoreFont);
        gameover_restart_.Display();
        gameover_leaderboard_.Display();
    }
}
 
void GameEngine::AdvanceOneFrame() {
    if (current_game_state_ == GameScreen) {
        UpdateObstacles();
        UpdateObstacleVector();
        UpdateScore();
        bird_.UpdateBird();
        HandleCollision();
    }
}

void GameEngine::UpdateObstacles() {
    // Shifts obstacles to the left
    if (!has_collided_ && bird_.started_) {
        for (Obstacle &obstacle : obstacles_) {
            obstacle.upper_main_.set(obstacle.upper_main_.getX1() - ObstacleSpeed, obstacle.upper_main_.getY1(), 
                                     obstacle.upper_main_.getX2() - ObstacleSpeed, obstacle.upper_main_.getY2());
            obstacle.lower_main_.set(obstacle.lower_main_.getX1() - ObstacleSpeed, obstacle.lower_main_.getY1(), 
                                     obstacle.lower_main_.getX2() - ObstacleSpeed, obstacle.lower_main_.getY2());
            obstacle.upper_secondary_.set(obstacle.upper_secondary_.getX1() - ObstacleSpeed, 
                                          obstacle.upper_secondary_.getY1(), obstacle.upper_secondary_.getX2() - 
                                          ObstacleSpeed, obstacle.upper_secondary_.getY2());
            obstacle.lower_secondary_.set(obstacle.lower_secondary_.getX1() - ObstacleSpeed, 
                                          obstacle.lower_secondary_.getY1(), obstacle.lower_secondary_.getX2() - 
                                          ObstacleSpeed, obstacle.lower_secondary_.getY2());
        }
    }
}

void GameEngine::UpdateObstacleVector() {
    // Removes and adds obstacles as the game progresses
    if (obstacles_.empty()) {
        for (size_t i = 0; i < kNumObstaclesOnScreen; i++) {
            float lower_bound = rand() % kObstacleRange + ((kWindowSize - kGroundHeight) / kLowerBoundDivider);
            float upper_bound = lower_bound - kGapSize;
            Obstacle obstacle(Rectf(vec2(((kWindowSize / kNumObstaclesOnScreen) * i) + 
            kStartingIncrement, 0),vec2((((kWindowSize / kNumObstaclesOnScreen) * i) + kStartingIncrement) 
            + kObstacleWidth,upper_bound)),
                              Rectf(vec2(((kWindowSize / kNumObstaclesOnScreen) * i) + 
                              kStartingIncrement, lower_bound),vec2((((kWindowSize / kNumObstaclesOnScreen) * i)
                              + kStartingIncrement) + kObstacleWidth,kWindowSize - kGroundHeight)),
                              Rectf(vec2(((kWindowSize / kNumObstaclesOnScreen) * i) + 
                              kStartingIncrement - kSecondaryPipeWidth, upper_bound - kSecondaryPipeHeight),
                                    vec2((((kWindowSize / kNumObstaclesOnScreen) * i) + kStartingIncrement) + 
                                    kObstacleWidth + kSecondaryPipeWidth, upper_bound)),
                              Rectf(vec2(((kWindowSize / kNumObstaclesOnScreen) * i) + 
                              kStartingIncrement - kSecondaryPipeWidth, lower_bound),
                                    vec2((((kWindowSize / kNumObstaclesOnScreen) * i) + kStartingIncrement) + 
                                    kObstacleWidth + kSecondaryPipeWidth, lower_bound + kSecondaryPipeHeight)),
                              kObstacleColor);
            obstacles_.push_back(obstacle);
        }
    }
    if (obstacles_[0].upper_main_.getX1() == obstacles_[0].pipe_width_) {
        float lower_bound = rand() % kObstacleRange + (kWindowSize / kLowerBoundDivider);
        float upper_bound = lower_bound - kGapSize;
        Obstacle obstacle(Rectf(vec2(kWindowSize + kObstacleDelay, 0),
                                vec2(kWindowSize + kObstacleWidth + kObstacleDelay, upper_bound)),
                          Rectf(vec2(kWindowSize + kObstacleDelay, lower_bound),
                                vec2(kWindowSize + kObstacleWidth + kObstacleDelay, 
                                     kWindowSize - kGroundHeight)),
                          Rectf(vec2(kWindowSize + kObstacleDelay - kSecondaryPipeWidth, 
                                     upper_bound - kSecondaryPipeHeight),
                                vec2(kWindowSize + kObstacleWidth + kObstacleDelay + kSecondaryPipeWidth, 
                                     upper_bound)),
                          Rectf(vec2(kWindowSize + kObstacleDelay - kSecondaryPipeWidth, 
                                     lower_bound),vec2(kWindowSize + kObstacleWidth + kObstacleDelay + 
                                     kSecondaryPipeWidth, lower_bound + kSecondaryPipeHeight)), kObstacleColor);
        obstacles_.push_back(obstacle);
    }
    // this makes sure the obstacle is removed after it has moved of the screen for smooth graphics
    if (obstacles_[0].upper_main_.getX2() == -obstacles_[0].pipe_width_) {
        obstacles_.erase(obstacles_.begin());
    }
}

void GameEngine::UpdateScore() {
    // if the bird passes the pipe, the player scores a point
    if (bird_.position_.x == obstacles_[0].upper_main_.getX2()) {
        score_++;
    }
}

void GameEngine::HandleCollision() {
    if (bird_.position_.y >= kWindowSize - bird_.radius_ || bird_.position_.y <= bird_.radius_ || 
    obstacles_[0].upper_main_.contains(bird_.position_ + vec2(bird_.radius_, -bird_.radius_)) || 
    obstacles_[0].lower_main_.contains(bird_.position_ + vec2(bird_.radius_, bird_.radius_)) || 
    obstacles_[0].upper_secondary_.contains(bird_.position_ + vec2(bird_.radius_, -bird_.radius_)) || 
    obstacles_[0].lower_secondary_.contains(bird_.position_ + vec2(bird_.radius_, bird_.radius_))) {
        has_collided_ = true;
        bird_.has_collided_ = true;
        bird_.acceleration_ = kBirdDeathAcceleration;
    }
    HandleDeath();
}

void GameEngine::HandleDeath() {
    if (bird_.position_.y >= kWindowSize - kBottomHeight -kTopHeight - bird_.radius_) {
        bird_.acceleration_ = 0;
        bird_.y_velocity_ = 0;
        leaderboard_.scores_.push_back(score_);
        leaderboard_.ManageScores();
        current_game_state_ = GameOverScreen;
    }
}

void GameEngine::keyDown(const KeyEvent &event) {
    if (event.getCode() == event.KEY_SPACE && current_game_state_ == StartScreen) {
        current_game_state_ = GameScreen;
    }
    if (event.getCode() == event.KEY_SPACE && (!has_collided_) && current_game_state_ == GameScreen 
        && bird_.y_velocity_ > kFlapBoundary) {
        bird_.started_ = true;
        bird_.acceleration_ = 0;
        bird_.y_velocity_ = kFlapVelocity;
    }
    if (event.getCode() == event.KEY_SPACE && current_game_state_ == GameOverScreen) {
        ResetGame();
        current_game_state_ = StartScreen;
    }
}

void GameEngine::mouseDown(const MouseEvent &event) {
    if (current_game_state_ == StartScreen) {
        if (start_normal_.area_.contains(event.getPos()) && start_challenge_.highlighted_) {
            start_challenge_.highlighted_ = false;
            start_normal_.highlighted_ = true;
            ObstacleSpeed = kNormalObstacleSpeed;
            bird_.gravity_ = kNormalGravity;
        }
        if (start_challenge_.area_.contains(event.getPos()) && start_normal_.highlighted_) {
            start_normal_.highlighted_ = false;
            start_challenge_.highlighted_ = true;
            ObstacleSpeed = kChallengeObstacleSpeed;
            bird_.gravity_ = kChallengeGravity;
        }
        if (start_leaderboard_.area_.contains(event.getPos())) {
            current_game_state_ = LeaderBoard;
        }
        if (start_customize_.area_.contains(event.getPos())) {
            current_game_state_ = CustomizeScreen;
        }
    }
    if (current_game_state_ == CustomizeScreen) {
        if (back_.area_.contains(event.getPos())) {
            ResetGame();
            current_game_state_ = StartScreen;
        }

        if (customize_red_.area_.contains(event.getPos())) {
            customize_red_.highlighted_ = true;
            customize_yellow_.highlighted_ = false;
            customize_blue_.highlighted_ = false;
            bird_.color_ = customize_red_.color_;
        }
        if (customize_yellow_.area_.contains(event.getPos())) {
            customize_red_.highlighted_ = false;
            customize_yellow_.highlighted_ = true;
            customize_blue_.highlighted_ = false;
            bird_.color_ = customize_yellow_.color_;
        }
        if (customize_blue_.area_.contains(event.getPos())) {
            customize_red_.highlighted_ = false;
            customize_yellow_.highlighted_ = false;
            customize_blue_.highlighted_ = true;
            bird_.color_ = customize_blue_.color_;
        }

        if (customize_purple_.area_.contains(event.getPos())) {
            customize_purple_.highlighted_ = true;
            customize_orange_.highlighted_ = false;
            customize_green_.highlighted_ = false;
            kObstacleColor = customize_purple_.color_;
        }
        if (customize_orange_.area_.contains(event.getPos())) {
            customize_purple_.highlighted_ = false;
            customize_orange_.highlighted_ = true;
            customize_green_.highlighted_ = false;
            kObstacleColor = customize_orange_.color_;
        }
        if (customize_green_.area_.contains(event.getPos())) {
            customize_purple_.highlighted_ = false;
            customize_orange_.highlighted_ = false;
            customize_green_.highlighted_ = true;
            kObstacleColor = customize_green_.color_;
        }
    }
    if (current_game_state_ == GameOverScreen) {
        if (gameover_restart_.area_.contains(event.getPos())) {
            ResetGame();
            current_game_state_ = StartScreen;
        }
        if (gameover_leaderboard_.area_.contains(event.getPos())) {
            current_game_state_ = LeaderBoard;
        }
    }
    if (current_game_state_ == LeaderBoard) {
        if (back_.area_.contains(event.getPos())) {
            ResetGame();
            current_game_state_ = StartScreen;
        }
    }
}

void GameEngine::ResetGame() {
    obstacles_.clear();
    bird_.has_collided_ = false;
    has_collided_ = false;
    bird_.started_ = false;
    bird_.position_ = vec2(kX_Position, kInitialY_Position);
    bird_.acceleration_ = 0;
    bird_.y_velocity_ = 0;
    score_ = 0;
}

// Bird Constructor and Functions
GameEngine::Bird::Bird(float set_x, float set_y, const char *set_color, float set_radius) {
    position_ = vec2(set_x, set_y);
    color_ = (char *) set_color;
    radius_ = set_radius;
}

void GameEngine::Bird::Display() const {
    color(Color(color_));
    drawSolidCircle(position_, radius_);
    color(Color(kOutlineColor));
    drawStrokedCircle(position_, radius_, kOutlineWidth, 0);
}

void GameEngine::Bird::UpdateBird() {
    if (started_) {
        if (!has_collided_) {
            acceleration_ = gravity_;
        }
        y_velocity_ += acceleration_;
        position_.y += y_velocity_;
    }
}

// Ground Constructor and Functions
GameEngine::Ground::Ground(Rectf set_top, Rectf set_bottom, const char * set_top_color, const char * set_bottom_color) {
    top_ = set_top;
    bottom_ = set_bottom;
    top_color_ = (char *) set_top_color;
    bottom_color_ = (char *) set_bottom_color;
}

void GameEngine::Ground::Display() const {
    color(Color(top_color_));
    drawSolidRect(top_);
    color(Color(bottom_color_));
    drawSolidRect(bottom_);
}

// Obstacle Constructor and Functions
GameEngine::Obstacle::Obstacle(Rectf set_upper_main, Rectf set_lower_main, 
                               Rectf set_upper_secondary, Rectf set_lower_secondary, const char * set_color) {
    upper_main_ = set_upper_main;
    lower_main_ = set_lower_main;
    upper_secondary_ = set_upper_secondary;
    lower_secondary_ = set_lower_secondary;
    color_ = (char *) set_color;
}

void GameEngine::Obstacle::Display() const {
    color(Color(color_));
    drawSolidRect(upper_main_);
    drawSolidRect(lower_main_);
    drawSolidRect(upper_secondary_);
    drawSolidRect(lower_secondary_);
}


// Button Constructor and Functions
GameEngine::Button::Button(Rectf set_area, const char *set_color, string set_title, float set_font_size) {
    area_ = set_area;
    color_ = (char *) set_color;
    title_ = move(set_title);
    font_size_ = set_font_size;
}

void GameEngine::Button::Display() const {
    color(Color(color_));
    drawSolidRect(Rectf(area_.getUpperLeft(), area_.getLowerRight()));
    Font button_font = Font(kGameFont, font_size_);
    drawStringCentered(title_, vec2((area_.getX1() + area_.getX2()) / kPositionAverage, 
                                    (area_.getY1() + area_.getY2()) / kPositionAverage - (font_size_ / 
                                    kTitlePositionDivider)), 
                       kGameTextColor, button_font);
    if (highlighted_) {
        color(Color(kHighlightColor));
        drawStrokedRect(area_, (area_.getY2() - area_.getY1()) / kHighlightWidthDivider);
    }
}

// Leaderboard Constructor and Functions
GameEngine::Leaderboard::Leaderboard() = default;

void GameEngine::Leaderboard::Display() {
    Font leaderboard_font = Font(kGameFont, kLeaderboardTitleFontSize);
    drawStringCentered(kLeaderboardTitle, vec2(kLeaderboardTitleX_Position, kLeaderboardTitleY_Position), 
                       kGameTextColor, leaderboard_font);
    color(Color(kGameTextColor));
    drawLine(vec2(kFirstLineX1_Position, kFirstLineY1_Position), vec2(kFirstLineX2_Position, 
                                                                      kFirstLineY1_Position));
    size_t line_gap = kLineGap;
    for (size_t i = 0; i < kLeaderboardPositions; i++) {
        drawLine(vec2(100, 150 + line_gap), vec2(500, 150 + line_gap));
        Font kScoreFont = Font(kGameFont, 20);
        drawStringCentered(to_string(i + 1) + kDot, vec2(100 + 20, 150 + line_gap - 20), 
                           kGameTextColor, kScoreFont);
        drawStringCentered(to_string(scores_[i]), vec2(500 - 50, 150 + line_gap - 20), 
                           kGameTextColor, kScoreFont);
        line_gap += kLineGap;
    }
}

void GameEngine::Leaderboard::ManageScores() {
    sort(scores_.begin(), scores_.end(), greater<size_t >());
}

// Functions for testing
vector<GameEngine::Obstacle> GameEngine::GetObstacles() {
    return obstacles_;
}

void GameEngine::SetGameState(GameEngine::GameState game_state) {
    current_game_state_ = game_state;
}

size_t GameEngine::GetScore() const {
    return score_;
}

GameEngine::Bird GameEngine::GetBird() {
    return bird_;
}

bool GameEngine::GetHasCollided() const {
    return has_collided_;
}

void GameEngine::Bird::SetStarted(bool set_started) {
    started_ = set_started;
}

bool GameEngine::Bird::GetStarted() const {
    return started_;
}

void GameEngine::Bird::SetPosition(float x_position, float y_position) {
    position_ = vec2(x_position, y_position);
}
} // namespace flappybird
 
