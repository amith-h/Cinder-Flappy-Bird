#include "catch2/catch.hpp"
#include <game_engine.h>

using flappybird::GameEngine;

TEST_CASE("UpdateObstacleVector") {
    GameEngine game_engine;
  SECTION("Check No Obstacles at Start") {
    game_engine.SetGameState(flappybird::GameEngine::GameScreen);
    REQUIRE(game_engine.GetObstacles().empty());
  }
  SECTION("Check Obstacles Created after Starting") {
     game_engine.SetGameState(flappybird::GameEngine::GameScreen);
     game_engine.AdvanceOneFrame();
     REQUIRE(game_engine.GetObstacles().size() == 2);
  }
  SECTION("Check Obstacles Vector Always Contains Two Obstacles after Starting") {
    game_engine.SetGameState(flappybird::GameEngine::GameScreen);
    for (size_t i = 0; i <= 2000; i++) {
        game_engine.AdvanceOneFrame();
    }
    REQUIRE(game_engine.GetObstacles().size() == 2);
  }
}

TEST_CASE("UpdateObstacles") {
    GameEngine game_engine;
    game_engine.SetGameState(flappybird::GameEngine::GameScreen);
    game_engine.GetBird().SetStarted(true);
    game_engine.AdvanceOneFrame();
    game_engine.AdvanceOneFrame();
  SECTION("Check Upper Main Rectangle Moves Correctly") {
    REQUIRE(game_engine.GetObstacles()[0].upper_main_.getUpperLeft() == vec2(700,0));
    REQUIRE(game_engine.GetObstacles()[0].upper_main_.getLowerLeft() == vec2(700,320));
    REQUIRE(game_engine.GetObstacles()[0].upper_main_.getUpperRight() == vec2(750,0));
    REQUIRE(game_engine.GetObstacles()[0].upper_main_.getLowerRight() == vec2(750,320));
  }
  SECTION("Check Lower Main Rectangle Moves Correctly") {
    REQUIRE(game_engine.GetObstacles()[0].lower_main_.getUpperLeft() == vec2(700,141));
    REQUIRE(game_engine.GetObstacles()[0].lower_main_.getLowerLeft() == vec2(700,552));
    REQUIRE(game_engine.GetObstacles()[0].lower_main_.getUpperRight() == vec2(750,141));
    REQUIRE(game_engine.GetObstacles()[0].lower_main_.getLowerRight() == vec2(750,552));
  }
}

TEST_CASE("UpdateScore") {
  SECTION("Check Score is Being Counted Correctly") {
    GameEngine game_engine;
    game_engine.SetGameState(flappybird::GameEngine::GameScreen);
    game_engine.GetBird().SetStarted(true);
    for (size_t i = 0; i <= 1000; i++) {
        game_engine.AdvanceOneFrame();
    }
    // the bird dies without input so the score is zero
    REQUIRE(game_engine.GetScore() == 0);
  }
}

TEST_CASE("ResetGame") {
  SECTION("Check Variables Are Reset Correctly") {
    GameEngine game_engine;
    game_engine.SetGameState(flappybird::GameEngine::GameScreen);
    game_engine.GetBird().SetStarted(true);
    for (size_t i = 0; i <= 10000; i++) {
        game_engine.AdvanceOneFrame();
    }
    // the bird would have died by now
    REQUIRE(game_engine.GetBird().GetStarted() == false);
  }
}

TEST_CASE("HandleCollision") {
  SECTION("Has Collided Is True After Collision") {
    GameEngine game_engine;
    game_engine.SetGameState(flappybird::GameEngine::GameScreen);
    game_engine.GetBird().SetStarted(true);
    game_engine.AdvanceOneFrame();
    game_engine.GetBird().SetPosition(725, 200);
    game_engine.AdvanceOneFrame();
    REQUIRE(game_engine.GetHasCollided() == true);
  }
}
