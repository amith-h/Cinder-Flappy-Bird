#include <flappy_bird_app.h>

namespace flappybird {
FlappyBirdApp::FlappyBirdApp()  {
    ci::app::setWindowSize(kWindowSize, kWindowSize);
}

// creates the background and makes the game engine display
void FlappyBirdApp::draw() {
    ci::Color background_color("dodgerblue"); 
    ci::gl::clear(background_color);
    game_engine_.Display();
}

// advances the game one frame
void FlappyBirdApp::update() {
    game_engine_.AdvanceOneFrame();
}

void FlappyBirdApp::keyDown(cinder::app::KeyEvent event) {
    game_engine_.keyDown(event);
}

void FlappyBirdApp::mouseDown(cinder::app::MouseEvent event) {
    game_engine_.mouseDown(event);
}

}

