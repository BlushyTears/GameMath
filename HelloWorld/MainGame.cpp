#define PLAY_IMPLEMENTATION
#define PLAY_USING_GAMEOBJECT_MANAGER

#include "Play.h"
#include "constants.h"
#include "Classes.h"

#include <vector>;

// Initial ideas for simulation:
// Multiple objects with names (red circle, maybe blue box etc)
// move with arrow keys, rotate with WASD
// There's a head thing around the circle body representing where player is facing
// There is also a cone representing FOV
// If an object is within FOV then it's under that list
// If it's behind the player or in front of, then it's under that particular list
// The player has its own cordinate space for calculating these things
// We can (hopefully with this framework) display that cordinate space by pressing P or something

// The entry point for a PlayBuffer program

Player* p = nullptr;
std::vector<AGameObject> mgoList;

void initDaPositions() {
	p = new Player(BasisVector(200, 200), 0, 250, 250);

	for (int i = 0; i < 5; i++) {
		AGameObject mgo;

		mgo.pos.x = Play::RandomRollRange(0, 800); // limit ourselves some margin for the text
		mgo.pos.y = Play::RandomRollRange(0, 700);
		mgo.radius = 5;
		mgo.objColor.r = Play::RandomRollRange(0, 255);
		mgo.objColor.g = Play::RandomRollRange(0, 255);
		mgo.objColor.b = Play::RandomRollRange(0, 255);

		mgoList.push_back(mgo);
	}
}

void MainGameEntry( PLAY_IGNORE_COMMAND_LINE )
{
	initDaPositions();
	Play::CreateManager(DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE);
}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate( float elapsedTime )
{
	Play::ClearDrawingBuffer( Play::cBlack );

	Play::DrawLine({ 700, 1000 }, { 700, 0 }, Play::cWhite);

	p->draw();
	p->move(4.0);
	p->rotate(20.0, 20.0);

	// dynamically list objects to the right
	for (int i = 0; i < mgoList.size(); i++) {

		Play::DrawDebugText({ 900, 600 }, "Behind of", Play::cMagenta, true);
		Play::DrawDebugText({ 900, 500 }, "In front of", Play::cBlue, true);
		
		if (p->checkIfObjIsBehind(mgoList[i])) {
			Play::DrawCircle({ 900 - (i * 25), 550 }, 5,
				{ mgoList[i].objColor.r, mgoList[i].objColor.g, mgoList[i].objColor.b });
		}
		else if(!p->checkIfObjIsBehind(mgoList[i])) {
			Play::DrawCircle({ 900 - (i * 25), 450 }, 5,
				{ mgoList[i].objColor.r, mgoList[i].objColor.g, mgoList[i].objColor.b });
		}

		Play::DrawDebugText({ 900, 400 }, "In FOV", Play::cCyan, true);
		Play::DrawDebugText({ 900, 300 }, "Outside FOV", Play::cCyan, true);

		if (p->isWithinFOV(mgoList[i])) {
			Play::DrawCircle({ 900 - (i * 25), 350 }, 5,
				{ mgoList[i].objColor.r, mgoList[i].objColor.g, mgoList[i].objColor.b });
		}
		else if (!p->isWithinFOV(mgoList[i])) {
			Play::DrawCircle({ 900 - (i * 25), 250 }, 5,
				{ mgoList[i].objColor.r, mgoList[i].objColor.g, mgoList[i].objColor.b });
		}

		// draw the objects here
		mgoList[i].draw();
	}

	Play::PresentDrawingBuffer();
	return Play::KeyDown(Play::KeyboardButton::KEY_ESCAPE);
}

// Gets called once when the player quits the game
int MainGameExit( void )
{
	Play::DestroyManager();
	delete[] p;
	return PLAY_OK;
}

// debug stuff (useful to have somewhere close)
// std::string tempString = std::to_string(snake.snakeparts);
// const char* charText = tempString.c_str();
// Play::DrawDebugText({ 50, 50 }, charText, Play::cBlue, true);
