#define OLC_PGE_APPLICATION
#include <stdlib.h>
#include "olcPixelGameEngine.h"

// Override base class with your custom functionality 
class Snake : public olc::PixelGameEngine
{
public:
	Snake()
	{
		// Name your application
		sAppName = "Snake";
	}

private:
	std::vector<olc::vf2d> vBody = { { 0.0f, 0.0f }, { 0.0f, 0.0f } };
	std::vector<olc::vf2d> vLife = { { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f } };
	olc::vf2d vPrev1 = { 0.0f, 0.0f };
	olc::vf2d vPrev2 = { 0.0f, 0.0f };
	olc::vf2d vFruit = { 0.0f, 0.0f };

	int nTails = 0;
	short int lives = 3;

	int reward = 100;
	int score = 0;
	int highScore;

	float fSnakeSpeed = 1.0f;

	bool WKeyPress;
	bool SKeyPress;
	bool AKeyPress;
	bool DKeyPress;

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		srand(time(0));
		vBody[0] = { ScreenWidth() / 2.0f, (ScreenHeight() - 100.0f) / 2.0f };
		vFruit = { float((rand() % ScreenWidth())), float((rand() % ScreenHeight())) };
		vLife[0] = { 120.0f, ScreenHeight() - 73.0f };
		vLife[1] = { 145.0f, ScreenHeight() - 73.0f };
		vLife[2] = { 170.0f, ScreenHeight() - 73.0f };

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Handle User Input
		if (GetKey(olc::Key::A).bPressed)
		{
			vBody[0].x -= fSnakeSpeed * fElapsedTime; AKeyPress = true; DKeyPress = false; WKeyPress = false; SKeyPress = false;
		}
		if (GetKey(olc::Key::D).bPressed)
		{
			vBody[0].x += fSnakeSpeed * fElapsedTime; AKeyPress = false; DKeyPress = true; WKeyPress = false; SKeyPress = false;
		}
		if (GetKey(olc::Key::W).bPressed)
		{
			vBody[0].y -= fSnakeSpeed * fElapsedTime; AKeyPress = false; DKeyPress = false; WKeyPress = true; SKeyPress = false;
		}
		if (GetKey(olc::Key::S).bPressed)
		{
			vBody[0].y += fSnakeSpeed * fElapsedTime; AKeyPress = false; DKeyPress = false; WKeyPress = false; SKeyPress = true;
		}

		if (AKeyPress) vBody[0].x -= fSnakeSpeed;
		if (DKeyPress) vBody[0].x += fSnakeSpeed;
		if (WKeyPress) vBody[0].y -= fSnakeSpeed;
		if (SKeyPress) vBody[0].y += fSnakeSpeed;

		// Erase previous frame
		Clear(olc::BLACK);

		// Draw Game Boundary
		DrawRect(10, 10, ScreenWidth() - 20, ScreenHeight() - 110, olc::WHITE);

		//Draw Info Boundry
		DrawRect(10, ScreenHeight() - 90, ScreenWidth() - 20, 80);

		// Draw Head
		FillCircle(int(vBody[0].x), int(vBody[0].y), 6, olc::WHITE);

		// Draw Fruit
		FillCircle(int(vFruit.x), int(vFruit.y), 6, olc::MAGENTA);

		// Draw Lives
		FillCircle(int(vLife[0].x), int(vLife[0].y), 8, olc::RED);
		FillCircle(int(vLife[1].x), int(vLife[1].y), 8, olc::RED);
		FillCircle(int(vLife[2].x), int(vLife[2].y), 8, olc::RED);

		// Fruit Collision Event
		if (vBody[0].x <= vFruit.x + 12 && vBody[0].y <= vFruit.y + 12 && vBody[0].x >= vFruit.x - 12 && vBody[0].y >= vFruit.y - 12)
		{
			nTails = 4 + nTails;
			if (nTails > 20)
			{
				reward = (int((nTails) / 20) * 5) + 100;
				fSnakeSpeed = (floor(nTails / 20.0f) * 0.1f) + 1.0f;
			}
			score += reward;

			for (int i = 0; i <= nTails; i++)
				vBody.push_back({ 0.0f, 0.0f });

			vFruit = { float(((rand() % ScreenWidth()) + 1)), float(((rand() % ScreenHeight()) + 1)) };

		}

		// Draw Tails
		vPrev1 = vBody[1];
		vBody[1] = vBody[0];
		for (int i = 1; i <= nTails; i++)
		{
			vPrev2 = vBody[i];
			vBody[i] = vPrev1;
			vPrev1 = vPrev2;

			if (i % 4 == 0)
				FillCircle(vBody[i], 5, olc::WHITE);
		}

		// Tail Collision Event
		for (int i = 4; i <= nTails; i++)
		{
			if (vBody[0].x >= vBody[i].x - 1.0f && vBody[0].y >= vBody[i].y - 1.0f && vBody[0].x <= vBody[i].x + 1.0f && vBody[0].y <= vBody[i].y + 1.0f)
			{
				vBody[0] = { ScreenWidth() / 2.0f, ScreenHeight() / 2.0f };
				nTails = 0;
				lives--;
				score -= 500;
				fSnakeSpeed = 1.0f;
				vFruit = { float(((rand() % ScreenWidth()) + 1)), float(((rand() % ScreenHeight()) + 1)) };
			}
		}

		// Border Collision Event
		if (vBody[0].x > ScreenWidth() - 15.0f || vBody[0].x < 15.0f || vBody[0].y > ScreenHeight() - 105.0f || vBody[0].y < 15.0f)
		{
			vBody[0] = { ScreenWidth() / 2.0f, ScreenHeight() / 2.0f };
			nTails = 0;
			lives--;
			score -= 500;
			fSnakeSpeed = 1.0f;
		}

		// Redraw fruit if out of bounds
		if (vFruit.x < 20 || vFruit.x > ScreenWidth() - 20 || vFruit.y < 20 || vFruit.y > ScreenHeight() - 120)
			vFruit = { float(((rand() % ScreenWidth()) + 1)), float(((rand() % ScreenHeight()) + 1)) };

		// Check Lives
		if (lives == 2) vLife[2] = { -10, -10 };
		if (lives == 1) vLife[1] = { -10, -10 };
		if (lives <= 0)
		{
			fSnakeSpeed = 0.0f;
			vLife[0] = { -10, -10 };
			vFruit = { -10, -10 };
			score = 0;
			DrawString((ScreenWidth() - 70) / 2, (ScreenHeight() - 100) / 2, "GAME OVER!", olc::WHITE, 1);
			DrawString((ScreenWidth() - 120) / 2, (ScreenHeight() - 80) / 2, "TRY AGAIN? [Y/N]", olc::WHITE, 1);
			if (GetKey(olc::Key::Y).bPressed)  // Restart Game
			{
				AKeyPress = false; DKeyPress = false; WKeyPress = false; SKeyPress = false;
				vLife[0] = { 120.0f, ScreenHeight() - 73.0f };
				vLife[1] = { 145.0f, ScreenHeight() - 73.0f };
				vLife[2] = { 170.0f, ScreenHeight() - 73.0f };
				vFruit = { float(((rand() % ScreenWidth()) + 1)), float(((rand() % ScreenHeight()) + 1)) };
				fSnakeSpeed = 1.0f;
				lives = 3;
			}
			if (GetKey(olc::Key::N).bPressed) exit(0);  // Close Window
		}

		// Set High Score
		if (highScore <= score)
			highScore = score;

		// Info Text
		DrawString(20, ScreenHeight() - 75, "Lives Left: ", olc::WHITE, 1);
		DrawString(20, ScreenHeight() - 52, "Current Score: ", olc::WHITE, 1);
		DrawString(140, ScreenHeight() - 52, std::to_string(score), olc::WHITE, 1);
		DrawString(20, ScreenHeight() - 30, "Highest Score: ", olc::WHITE, 1);
		DrawString(140, ScreenHeight() - 30, std::to_string(highScore), olc::WHITE, 1);

		// Controls
		DrawString(360, ScreenHeight() - 82, "W = Up", olc::WHITE, 1);
		DrawString(360, ScreenHeight() - 62, "A = Left", olc::WHITE, 1);
		DrawString(360, ScreenHeight() - 42, "S = Down", olc::WHITE, 1);
		DrawString(360, ScreenHeight() - 22, "D = Right", olc::WHITE, 1);

		return true;
	};
};

int main()
{
	Snake demo;
	if (demo.Construct(480, 360, 1, 1, false, true))
		demo.Start();
	return 0;
}
