#pragma once

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "game_engine.h"

namespace flappybird {
class FlappyBirdApp : public ci::app::App {
  private:
    GameEngine game_engine_ = GameEngine();

  public:
    FlappyBirdApp();
    const int kWindowSize = 600;
    
    void draw() override;

    void update() override;
    
    void keyDown(ci::app::KeyEvent event) override;
    
    void mouseDown(ci::app::MouseEvent event) override;
};
} // namespace flappybird


