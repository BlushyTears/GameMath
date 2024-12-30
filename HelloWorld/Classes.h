#pragma once
#include "Play.h"
#include "constants.h"
#include "math.h"

struct Position {
	float x;
	float y;
}; 

// An object that isn't the player in this case
struct AGameObject {
	Position pos;
	Color objColor;
	int radius;
	void draw();
};

// Basis vectors that make up the object space
struct BasisVector {
	float x;
	float y;

	BasisVector() : x(0), y(0) {}
	BasisVector(float x, float y) : x(x), y(y) {}
};

struct Player {
	Position pos;
	BasisVector origin;
	BasisVector basisI;
	BasisVector basisJ;
	bool debugMode;
	int radius = 5; // I am aware it's ideal to initialize these in the constructor instead
	int headSize = 3;
	float circleRotationAngle = 0.1;
	
	Player(BasisVector origin, float rotationAngle, int xpos, int ypos);

	bool checkIfObjIsBehind(AGameObject& ago);
	bool isWithinFOV(AGameObject& ago);

	void PlayerRotate(float rotationAngle);
	void draw();
	void move(float speed);
	void rotate(float freq, float ampl);
};

