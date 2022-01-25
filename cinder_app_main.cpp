#include "flappy_bird_app.h"
#include "cinder/app/RendererGl.h"


using flappybird::FlappyBirdApp;

void prepareSettings(FlappyBirdApp::Settings* settings) {
    settings->setResizable(false);
}

// This line is a macro that expands to a "int main()" function.
CINDER_APP(FlappyBirdApp, ci::app::RendererGl, prepareSettings);
