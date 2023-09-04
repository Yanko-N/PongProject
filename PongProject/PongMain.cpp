#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <cmath>

class Player {
public:
	olc::vf2d coords;
	float speed;
	float size;
	int score = 0;
	Player() {
		coords = olc::vf2d(0, 200);
		speed = 100;
		size = 20;
	}


};

class Ball {
public:
	olc::vf2d coords;
	float speed;
	float radius;
	float angle;
	olc::vf2d direction;
	bool collided = false;

	Ball() {
		srand((unsigned)time(NULL));

		coords = olc::vf2d(150, 150);
		speed = 150;
		radius = 5;
		angle = 3.14159f / (rand() % 8);
		direction = olc::vf2d(cos(angle), sin(angle));

	}
};
class Game : public olc::PixelGameEngine
{
private:
	float fTargetFrameTime = 1.0f / 100.0f; // Virtual FPS of 100fps
	float fAccumulatedTime = 0.0f;
	bool gameplay = true;



public:
	Game()
	{
		sAppName = "Pong";
	}

public:
	Player* player1 = new Player();
	Player* player2 = new Player();

	Ball* ball = new Ball();

	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		player1->coords.x = 20;
		player2->coords.x = (ScreenWidth() - 1) - 20;


		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// called once per frame
		fAccumulatedTime += fElapsedTime;
		if (fAccumulatedTime >= fTargetFrameTime)
		{
			fAccumulatedTime -= fTargetFrameTime;
			fElapsedTime = fTargetFrameTime;
		}
		else
			return true; // Don't do anything this frame
		if (player1->score >= 10) {
			gameplay = false;
		}
		if (player2->score >= 10) {
			gameplay = false;
		}

		ColisionController();
		BallController(fElapsedTime);
		MovimentController(fElapsedTime);
		GraphicsUpdate();


		return true;
	}

	void ColisionController() {
		srand((unsigned)time(NULL));

		if (intersects(ball, player1) && !ball->collided) {
			ball->collided = true;

			std::cout << " COLISION 1";
			ball->direction.x *= -1;
		}
		else if (!intersects(ball, player1))
		{
			ball->collided = false;
		}

		if (intersects(ball, player2) && !ball->collided) {
			ball->collided = true;

			std::cout << " COLISION 2";
			ball->direction.x *= -1;
		}
		else if (!intersects(ball, player2))
		{
			ball->collided = false;
		}

		if (ball->coords.y > ScreenHeight() - 5) {
			ball->direction.y *= -1;
		}


		if (ball->coords.y < 5) {
			ball->direction.y *= -1;
		}


		if (ball->coords.x < player1->coords.x) {
			player2->score++;
			ball->coords = olc::vf2d(ScreenWidth() / 2, ScreenHeight() / 2);
			ball->angle = 3.14159f / (rand() % 8);

		}
		if (ball->coords.x > player2->coords.x) {
			player1->score++;
			ball->coords = olc::vf2d(ScreenWidth() / 2, ScreenHeight() / 2);
			ball->angle = 3.14159f / (rand() % 8);
		}
	}
	void BallController(float fElapesedTime) {

		ball->coords += VectorWithNorm(ball->speed * fElapesedTime, ball->direction);

		//CLAMPING
		ball->coords.x = clamp(ball->coords.x, 0, float(ScreenHeight() - 1));
		ball->coords.y = clamp(ball->coords.y, 0, float(ScreenHeight() - 1));
	}
	void MovimentController(float fElapsedTime) {

		if (player1->coords.y >= 0 && player1->coords.y <= float(ScreenHeight() - 1)) {

			if (GetKey(olc::Key::W).bHeld) {
				player1->coords.y -= player1->speed * fElapsedTime;

			}
			if (GetKey(olc::Key::S).bHeld) {

				player1->coords.y += player1->speed * fElapsedTime;
			}
		}

		player2->coords.y = clamp(player2->coords.y, 0, float(ScreenHeight() - 1));

		if (player2->coords.y >= 0 && player2->coords.y <= float(ScreenHeight() - 1)) {

			if (GetKey(olc::Key::UP).bHeld) {
				player2->coords.y -= player2->speed * fElapsedTime;

			}
			if (GetKey(olc::Key::DOWN).bHeld) {

				player2->coords.y += player2->speed * fElapsedTime;
			}
		}

		player1->coords.y = clamp(player1->coords.y, 0, float(ScreenHeight() - 1));


	}
	void GraphicsUpdate() {

		if (gameplay) {
			//Primeiro Limpamos a tela
			Clear(olc::BLACK);
			std::string score = std::to_string(player1->score) + " : " + std::to_string(player2->score);
			DrawString(olc::vf2d(ScreenWidth() / 2 - 15, 2), score, olc::WHITE);

			//Desenhar o player1
			DrawLine(olc::vf2d(player1->coords.x, player1->coords.y - player1->size), olc::vf2d(player1->coords.x, player1->coords.y + player1->size), olc::WHITE);
			//desenhar player 2
			DrawLine(olc::vf2d(player2->coords.x, player2->coords.y - player2->size), olc::vf2d(player2->coords.x, player2->coords.y + player2->size), olc::WHITE);

			//Desenhar a bola
			FillCircle(ball->coords, ball->radius, olc::YELLOW);
		}
		else {

			if (GetKey(olc::SPACE).bHeld) {
				player1->score = 0;
				player2->score = 0;
				ball->coords = olc::vf2d(ScreenWidth() / 2, ScreenHeight() / 2);
				ball->angle = 3.14 / 4;
				gameplay = true;
			}
			if (player1->score > player2->score && player1->score >= 10) {
				Clear(olc::BLACK);

				DrawString(olc::vf2d(ScreenWidth() / 2 - 15, 2), "PLAYER 1 WINS", olc::WHITE);
			}
			else if (player1->score < player2->score && player2->score >= 10)
			{
				Clear(olc::BLACK);

				DrawString(olc::vf2d(ScreenWidth() / 2 - 15, 2), "PLAYER 2 WINS", olc::WHITE);
			}
			else {
				Clear(olc::BLACK);

				DrawString(olc::vf2d(ScreenWidth() / 2 - 15, 2), "DRAW", olc::WHITE);
			}
		}


	}
	bool intersects(const Ball* circle, const Player* rect)
	{
		double circleDistanceX = std::abs(circle->coords.x - rect->coords.x);
		double circleDistanceY = std::abs(circle->coords.y - rect->coords.y);

		if (circleDistanceX > (circle->radius)) {
			return false;
		}

		if (circleDistanceY > (rect->size / 2 + circle->radius)) {
			return false;
		}

		if (circleDistanceX <= (rect->size / 2)) {
			return true;
		}

		if (circleDistanceY <= (rect->size / 2)) {
			return true;
		}

		double cornerDistanceSq = std::pow(circleDistanceX - rect->size / 2, 2) +
			std::pow(circleDistanceY - rect->size / 2, 2);

		return (cornerDistanceSq <= std::pow(circle->radius, 2));
	}
	float clamp(float val, int min1, float max1) {

		return std::fmin(std::fmax(val, min1), max1);
	}
	float GetNorma(olc::vf2d vector) {
		return std::sqrt(std::pow(vector.x, 2) + std::pow(vector.y, 2));
	}
	olc::vf2d VectorWithNorm(float norma, olc::vf2d vector) {

		float h = GetNorma(vector) / norma;
		olc::vf2d vetor;
		if (h != 0) {
			vetor = olc::vf2d(vector.x / h, vector.y / h);
		}

		return vetor;
	}

};


int main()
{
	Game game;
	if (game.Construct(256, 240, 4, 4))
		game.Start();

	return 0;
}
