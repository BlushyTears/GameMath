#include "Classes.h"
#include "constants.h"
#include "Play.h"

#include "math.h"
#include <stdexcept>

Player::Player(BasisVector origin, float rotationAngle, int xpos, int ypos)
	: origin(origin) {
	basisI = { cos(rotationAngle), sin(rotationAngle) };
	basisJ = { -sin(rotationAngle), cos(rotationAngle) };

	pos.x = xpos;
	pos.y = ypos;


	debugMode = false;
}

void Player::draw() {
	Play::DrawCircle({ pos.x, pos.y}, radius, Play::cRed);

	float theta = 3.14159 / 4.f; // 45 degree turn

	float rotatedX = basisI.x * cos(theta) - basisI.y * sin(theta);
	float rotatedY = basisI.x * sin(theta) + basisI.y * cos(theta);	

	// Draws facing direction
	Play::DrawLine({ pos.x , pos.y },
		{ rotatedX * 50 + pos.x, rotatedY * 50 + pos.y },
		Play::cWhite);

	//// //Draws facing direction
	float theta2 = -(3.14159 / 4.f); // 45 degree turn
	float rotatedX2 = basisI.x * cos(theta2) - basisI.y * sin(theta2);
	float rotatedY2 = basisI.x * sin(theta2) + basisI.y * cos(theta2);

	Play::DrawLine({ pos.x , pos.y },
		{ rotatedX2 * 50 + pos.x, rotatedY2 * 50 + pos.y },
		Play::cWhite);

	// These uncommented lines make up a 2d object space 

	if (debugMode) {
		Play::DrawLine({ pos.x , pos.y },
			{ (basisI.x) * 80 + pos.x, (basisI.y) * 80 + pos.y }, Play::cGreen);

		Play::DrawLine({ pos.x, pos.y },
			{ basisI.x * 80 + pos.x , basisI.y * 80 + pos.y }, Play::cBlue);

		Play::DrawLine({ pos.x, pos.y },
			{ -basisI.y * 80 + pos.x , basisI.x * 80 + pos.y }, Play::cOrange);

		Play::DrawLine({ pos.x, pos.y },
			{ -basisI.x * 80 + pos.x , -basisI.y * 80 + pos.y }, Play::cGreen);

		Play::DrawLine({ pos.x, pos.y },
			{ basisI.y * 80 + pos.x  , -basisI.x * 80 + pos.y }, Play::cMagenta);
	}

	// Press P to turn on lines
	if (Play::KeyPressed(Play::KEY_P))
		debugMode = !debugMode;
}

void AGameObject::draw() {
	Play::DrawCircle({ pos.x, pos.y }, radius, 
	{this->objColor.r, this->objColor.g, this->objColor.b });
}

void Player::move(float speed) {
	if (Play::KeyDown(Play::KEY_UP)) {
		pos.y += speed;
	}
	if (Play::KeyDown(Play::KEY_DOWN)) {
		pos.y -= speed;
	}
	if (Play::KeyDown(Play::KEY_LEFT)) {
		pos.x -= speed;
	}
	if (Play::KeyDown(Play::KEY_RIGHT)) {
		pos.x += speed;
	}
}

void Player::rotate(float freq, float ampl) {
	float theta = (circleRotationAngle - 0.05) * freq;

	float cosTheta = cos(theta);
	float sinTheta = sin(theta);

	if (Play::KeyDown(Play::KEY_D)) {
		PlayerRotate(-0.04f);
		circleRotationAngle += 0.001;
	}
	if (Play::KeyDown(Play::KEY_A)) {
		PlayerRotate(0.04f);
		circleRotationAngle -= 0.001;
	}
}

bool Player::isWithinFOV(AGameObject & ago) {
	float objMag = sqrt(pow(ago.pos.x, 2) + (ago.pos.y, 2));
	float playerMag = sqrt(pow(pos.x, 2) + (pos.y, 2));
	float directionMag = sqrt(pow(basisI.x, 2) + (basisI.y, 2));

	float r1 = ago.pos.x - pos.x; // row 1 of vector
	float r2 = ago.pos.y - pos.y; // row 2 of vector

	float numerator = basisI.x * r1 + basisI.y * r2;
	float denom = directionMag * sqrt(pow(r1, 2) + pow(r2, 2));
	float dot = numerator / denom;

	if (dot >= 0.40f)
		return true;
	else
		return false;

	// dangerous to not have a base return statement in case the above aren't called
	return false;
}

void Player::PlayerRotate(float rotationAngle) {
	// Compute rotation matrix components
	float cosTheta = cos(rotationAngle);
	float sinTheta = sin(rotationAngle);

	// 2D rotation (Somewhere in chapter 4 or 5 I think for reference)
	BasisVector newBasisI = {
		basisI.x * cosTheta - basisI.y * sinTheta,
		basisI.x * sinTheta + basisI.y * cosTheta
	};

	BasisVector newBasisJ = {
		basisJ.x * cosTheta - basisJ.y * sinTheta,
		basisJ.x * sinTheta + basisJ.y * cosTheta
	};

	// Update the basis vectors
	basisI = newBasisI;
	basisJ = newBasisJ;
}

bool Player::checkIfObjIsBehind(AGameObject& ago) {
	// ago - pos of player gives a vector k
	// dot product of vector k and 
	// forward vector: { basisI.x + pos.x , basisI.y + pos.y } 

	float i = ago.pos.x - pos.x;
	float j = ago.pos.y - pos.y;
	float dot = i * basisI.x + j * basisI.y;

	if (dot >= 0)
		return false;
	else if (dot < 0)
		return true;

	// dangerous to not have a base return statement in case the above aren't called
	return false;

	//pseudo code:
	// fov = (a * b) / (||a||||b||) * cos(phi) 
	// if(fov / 2 > cos(90)) then  we are within FOV
}
