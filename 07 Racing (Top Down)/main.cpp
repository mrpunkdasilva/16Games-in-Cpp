#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <string>
#include <iomanip>
using namespace sf;

enum GameState { MENU, PLAYING };

const int num = 8; // checkpoints
int points[num][2] = {
	300, 610,
	1270, 430,
	1380, 2380,
	1900, 2460,
	1970, 1700,
	2550, 1680,
	2560, 3150,
	500, 3300
};

struct Car {
	float x, y, speed, angle;
	int n;
	int lastCheckpoint;

	Car() {
		speed = 2;
		angle = 0;
		n = 0;
		lastCheckpoint = 0;
	}
	
	void move() {
		x += sin(angle) * speed;
		y -= cos(angle) * speed;
	}

	void findTarget() {
		float tx = points[n][0];
		float ty = points[n][1];
		float beta = angle - atan2(tx-x, -ty+y);

		if (sin(beta) < 0) angle += 0.005 * speed;
		else angle -= 0.005 * speed;
		if ((x - tx) * (x - tx) + (y - ty) * (y - ty) < 25 * 25) {
			lastCheckpoint = n;
			n = (n+1) %num;
		}
	}
};

int main() {
	RenderWindow app(VideoMode(640, 480), "Car Racing Game!");
	app.setFramerateLimit(60);

	GameState gameState = MENU;

	Font font;
	if (!font.loadFromFile("fonts/Carlito-Regular.ttf")) {
		// Handle error
	}

	Text menuText;
	menuText.setFont(font);
	menuText.setString("Press Enter to Start");
	menuText.setCharacterSize(50);
	menuText.setFillColor(Color::White);
	menuText.setPosition(app.getSize().x / 2 - menuText.getGlobalBounds().width / 2, app.getSize().y / 2 - menuText.getGlobalBounds().height / 2);

	Text speedText;
	speedText.setFont(font);
	speedText.setCharacterSize(20);
	speedText.setFillColor(Color::Black);
	speedText.setPosition(10, 10);

	Text positionText;
	positionText.setFont(font);
	positionText.setCharacterSize(20);
	positionText.setFillColor(Color::Black);
	positionText.setPosition(10, 40);

	int currentCheckpoint = 0;
	int laps = 0;

	Text lapText;
	lapText.setFont(font);
	lapText.setCharacterSize(20);
	lapText.setFillColor(Color::Black);
	lapText.setPosition(10, 70);

	Texture t1, t2, t3;
	t1.loadFromFile("images/background.png");
	t2.loadFromFile("images/car.png");
	t1.setSmooth(true);
	t2.setSmooth(true);

	Sprite sBackground(t1), sCar(t2);
	sBackground.scale(2, 2);

	sCar.setOrigin(22, 22);
	float R = 22;

	const int N = 5;
	Car car[N];
	for(int i = 0; i < N; i++) {
		car[i].x = 300 + i * 50;
		car[i].y = 1700 + i *80;
		car[i].speed=7+i;
	}

	float speed = 0, angle = 0;
	float maxSpeed = 8.0;
	float acc = 0.3, dec = 0.5;
	float turnSpeed = 0.05;

	int offsetX = 0, offsetY = 0;

	while (app.isOpen()) {
		Event e;

		while (app.pollEvent(e)) {
			if (e.type == Event::Closed) {
				app.close();
			}
			if (e.type == Event::KeyPressed && e.key.code == Keyboard::Enter && gameState == MENU) {
				gameState = PLAYING;
			}
		}

		if (gameState == MENU) {
			app.clear(Color::Black);
			app.draw(menuText);
			app.display();
			continue;
		}

		if (gameState == PLAYING) {
			bool Up = 0, Right = 0, Down = 0, Left = 0;
			if (Keyboard::isKeyPressed(Keyboard::Up)) Up = 1;
			if (Keyboard::isKeyPressed(Keyboard::Right)) Right = 1;
			if (Keyboard::isKeyPressed(Keyboard::Down)) Down = 1;
			if (Keyboard::isKeyPressed(Keyboard::Left)) Left = 1;

			//car movement
			if (Up && speed < maxSpeed) {
				if (speed < 0) speed += dec;
				else speed += acc;
			}

			if (Down && speed > -maxSpeed) {
				if (speed > 0) speed -= dec;
				else speed -= acc;
			}

			if (!Up && !Down) {
				if (speed - dec > 0) speed -= dec;
				else if (speed + dec < 0) speed += dec;
				else speed = 0;
			}

			if (Right && speed != 0) angle += turnSpeed * speed/maxSpeed;
			if (Left && speed != 0) angle -= turnSpeed * speed/maxSpeed;

			car[0].speed = speed;
			car[0].angle = angle;

			for(int i=0; i<N; i++) car[i].move();
			for(int i=1; i<N; i++) car[i].findTarget();

			// Colission
			for (int i = 0; i < N; i++) {
				for (int j=0; j<N; j++) {
					int dx=0, dy=0;
					while (dx*dx+dy*dy<4*R*R) {
						car[i].x += dx/10.0;
						car[i].x += dy/10.0;
						car[j].x -= dx/10.0;
						car[j].y -= dy/10.0;
						dx = car[i].x - car[j].x;
						dy = car[i].y - car[j].y;
						if (!dx && !dy) break;
					}
				}
			}

			app.clear(Color::White);

			if (car[0].x > 320) offsetX = car[0].x - 320;
			if (car[0].y > 240) offsetY = car[0].y - 240;

			sBackground.setPosition(-offsetX, -offsetY);
			app.draw(sBackground);

			Color colors[10] = {
				Color::Red,
				Color::Green,
				Color::Magenta,
				Color::Blue,
				Color::White
			};

			for (int i = 0; i < N; i++) {
				sCar.setPosition(car[i].x-offsetX,car[i].y-offsetY);
				sCar.setRotation(car[i].angle*180/3.141593);
				sCar.setColor(colors[i]);
				app.draw(sCar);
			}

			// Update and draw speed and position monitors
			std::string speedStr = "Speed: " + std::to_string(static_cast<int>(car[0].speed * 10));
			speedText.setString(speedStr);
			app.draw(speedText);

			std::string positionStr = "Checkpoint: " + std::to_string(car[0].n + 1) + "/" + std::to_string(num);
			positionText.setString(positionStr);
			app.draw(positionText);

			// Lap counting
			if (car[0].n == 0 && car[0].lastCheckpoint == num - 1) {
				laps++;
				car[0].lastCheckpoint = 0; // Reset for next lap
			}

			std::string lapStr = "Laps: " + std::to_string(laps);
			lapText.setString(lapStr);
			app.draw(lapText);

			app.display();
		}
	}
	
	return 0;
}
