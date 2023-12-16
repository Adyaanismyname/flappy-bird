#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

using namespace sf;
using namespace std;
bool collides(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
	if (x1 + w1 >= x2 && x1 <= x2 + w2 && y1 + h1 >= y2 && y1 <= y2 + h2) {
		return true;
	}
	return false;
}
int main() {
	
	RenderWindow window(VideoMode(1920,1080),"Flappybird");
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);
	struct Textures
	{
		Texture pipe;
		Texture background;
		Texture flappy[3];
		Texture logo;
		Texture gameover;
	};
	Textures textures;
	Sprite background;
	Sprite logo;
	Sprite game_over;
	Text press_c;
	Font font;
	Text score;

	textures.pipe.loadFromFile("./images/pipe.png");
	textures.background.loadFromFile("./images/background.png");
	textures.flappy[0].loadFromFile("./images/flappy1.png");
	textures.flappy[1].loadFromFile("./images/flappy2.png");
	textures.flappy[2].loadFromFile("./images/flappy3.png");
	textures.logo.loadFromFile("./images/logo.png");
	textures.gameover.loadFromFile("./images/gameover.png");

	background.setTexture(textures.background);
	logo.setTexture(textures.logo);
	logo.setPosition(400,100);
	game_over.setTexture(textures.gameover);
	game_over.setPosition(500,100);
	game_over.setScale(3.5,3.5);
	font.loadFromFile("./fonts/flappy.ttf");
	press_c.setString("Press C to continue");
	press_c.setFont(font);
	press_c.setPosition(100,900);
	press_c.setScale(2.3,2.3);
	
	
	press_c.setPosition(500,800);
	struct Flappy {
		double speed = 0;
		int frame = 0; // for changing the texture of flappy

		Sprite sprite;
	};

	vector<Sprite> pipes;

	enum Gamestate {
		waiting,started,gameover
	};

	Gamestate gamestate = waiting;
	Flappy flappy;

	flappy.sprite.setPosition(480,400);
	flappy.sprite.setScale(3,3);

	struct Game {
		int frames = 0;
		int score = 0;
	};
	Game game;
	score.setFont(font);
	
	score.setPosition(0,8);
	score.setScale(2,2);

	
	
	while(window.isOpen()) {
		score.setString("Score: " + to_string(game.score/26));
		flappy.sprite.setTexture(textures.flappy[1]);
		Event event;
		// animating the bird

		
		if(gamestate == waiting || gamestate == started) {
			if(game.frames%6 == 0) {
				flappy.frame++;
				
			}
			if(flappy.frame == 3) {
				flappy.frame = 0;
			}
		}
		flappy.sprite.setTexture(textures.flappy[flappy.frame]);
		// checking the y and x values of the bird
		float fx = flappy.sprite.getPosition().x;
		float fy = flappy.sprite.getPosition().y;
		float fw = 34 * flappy.sprite.getScale().x;
		float fh = 24 * flappy.sprite.getScale().y;

		
		// moving flappy
		if(gamestate == started) {
			flappy.sprite.move(0,flappy.speed);
			
			flappy.speed+= 0.5;
		}
		// checking if flappy goes below the screen
		if(gamestate == started) {
			if(fy<0) {
				flappy.sprite.setPosition(480,0);
				flappy.speed = 0;
			}
			if(fy>1080) {
			gamestate = gameover;
			flappy.speed = 0;
			
		}
		}

		
		
		while(window.pollEvent(event)) {
			if(event.type == Event::Closed) {
				window.close();
				exit(EXIT_SUCCESS);
			}
			if(event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
				if(gamestate == waiting) {
					gamestate = started;
				}

				if(gamestate == started) {
					flappy.speed = -8;
					
				}
			}
			else if (event.type == Event::KeyPressed &&
					   event.key.code == Keyboard::C &&
					   gamestate == gameover) {

						gamestate = waiting;
						flappy.sprite.setPosition(480, 400);
				
						pipes.clear();
						game.score = 0;
			}
		}

		// Creating pipes

		if(gamestate == started && game.frames%150 == 0) {
			int gap = 200;
			int r = rand() % (1080-gap-310);

			
			// lower pipe
			Sprite PipeL;
			PipeL.setTexture(textures.pipe);
			PipeL.setPosition(1920, r+gap);
			PipeL.setScale(2, 3);

			// upper pipe
			Sprite pipeU;
			pipeU.setTexture(textures.pipe);
			pipeU.setPosition(1920,r);
			pipeU.setScale(2, -3);

			// push to the array
			pipes.push_back(PipeL);
			pipes.push_back(pipeU);
			

			

		}

		// moving pipes 

		for(vector<Sprite>::iterator itr = pipes.begin(); itr < pipes.end();itr++) {
			

			(*itr).move(-3.8,0);

			
		}

		
		window.clear();
		window.draw(background);
		window.draw(flappy.sprite);
		window.draw(score);
		if(gamestate == waiting) {
			window.draw(logo);
		}
		if(gamestate == gameover ) {
			window.draw(game_over);
			window.draw(press_c);
			
		}
		// showing pipes
		if(gamestate == started) {
			for(vector<Sprite>::iterator itr = pipes.begin();itr<pipes.end();itr++) {
			window.draw(*itr);
		}
		}
		// collision detection
		if (gamestate == started) {
			for (vector<Sprite>::iterator itr = pipes.begin(); itr != pipes.end(); itr++) {

				float px, py, pw, ph;

				if ((*itr).getScale().y > 0) {
					px = (*itr).getPosition().x;
					py = (*itr).getPosition().y;
					pw = 52 * (*itr).getScale().x;
					ph = 320 * (*itr).getScale().y;
				} else {
					pw = 52 * (*itr).getScale().x;
					ph = -320 * (*itr).getScale().y;
					px = (*itr).getPosition().x;
					py = (*itr).getPosition().y - ph;
				}

				if (collides(fx, fy, fw, fh, px, py, pw, ph)) {
					gamestate = gameover;
					
				}
			}
		}
		

		// counting scores
		
		if (gamestate == started) {
			bool passed_pipe = false;
    		for (vector<Sprite>::iterator itr = pipes.begin(); itr < pipes.end(); itr += 2) {
        		if ((*itr).getPosition().x < 500 && (*itr).getPosition().x > 400) {
            		passed_pipe = true;
					break;	
        		}
    		}

			if(passed_pipe == true) {
				game.score++;
			}
		}

		

		window.display();

		game.frames++;
		
		
	}


	




	
}

