// Include important C++ libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
using namespace sf;
using namespace std;
int main()
{
	float width = sf::VideoMode::getDesktopMode().width;
	float height = sf::VideoMode::getDesktopMode().height;
	View mainView(FloatRect(0.0f,0.0f,width, height));
	//width = 800; height = 600;
	// Create a video mode object
	VideoMode vm(width, height);
	// Create and open a window for the game
	RenderWindow window(vm, "Chaos Game!!", Style::Default);
	vector<Vector2f> vertices;
	vector<Vector2f> points;
	int pointAt = 0;

	Font f;
	if (!f.loadFromFile("./fonts/Roboto-Black.ttf")) cout << "failed to load font..." << endl;
	Text instructions;
	instructions.setFont(f);
	instructions.setCharacterSize(46);
	instructions.setFillColor(Color::Green);
	Text iterText;
	iterText.setFont(f);
	iterText.setCharacterSize(24);
	iterText.setFillColor(Color::White);

	int corners = 0; //increments in window.isOpen to get initial corners
	int randCorner;

	while (window.isOpen())
	{
		/*
		****************************************
		Handle the players input
		****************************************
		*/
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					std::cout << "the right button was pressed" << std::endl;
					std::cout << "mouse x: " << event.mouseButton.x << std::endl;
					std::cout << "mouse y: " << event.mouseButton.y << std::endl;

					if(corners == 3)
					{
						// get the current mouse position in the window
						sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

						// convert it to world coordinates
						sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, mainView);
						points.push_back(worldPos);
						corners++;
					}
					if(corners < 3)
					{
						// get the current mouse position in the window
						sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

						// convert it to world coordinates
						sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, mainView);
						vertices.push_back(worldPos);
						corners++;
					}
						
				}
			}

		}
		// Handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		/*****************************************
			Update the scene
		*****************************************/
		//generate interior points

		ostringstream oss;
		oss << "Chaos Game" << endl;
		instructions.setString(oss.str());

		FloatRect textRect = instructions.getLocalBounds();
		instructions.setOrigin(textRect.left +
			textRect.width / 2.0f,
			textRect.top +
			textRect.height / 2.0f);

		instructions.setPosition(width / 2, 50);

		ostringstream itero;
		itero << "Points Drawn: " << (points.size() + corners);
		iterText.setString(itero.str());

		FloatRect textRect2 = iterText.getLocalBounds();
		iterText.setOrigin(textRect2.left +
			textRect2.width / 2.0f,
			textRect2.top +
			textRect2.height / 2.0f);

		iterText.setPosition(160, 50);

		if(corners == 4)
		{
			//srand(time(0));
			randCorner = (rand() % 3);
			Vector2f midPoint;
			midPoint.x = ((vertices.at(randCorner).x + points.at(pointAt).x)/2);
			midPoint.y = ((vertices.at(randCorner).y + points.at(pointAt).y)/2);
			points.push_back(midPoint);
			pointAt++;
		}

		/*
		****************************************
		Draw the scene
		****************************************
		*/
		// Clear everything from the last run frame
		window.clear();
		// Draw our game scene here
		//RectangleShape r{ Vector2f{4,4} }; ///width, height.  Center uninitialized
		window.setView(mainView);
		CircleShape r(2);
		r.setFillColor(Color::Red);
		for (size_t i = 0; i < vertices.size(); i++)
		{
			r.setPosition(Vector2f{ vertices.at(i).x, vertices.at(i).y });
			window.draw(r);
		}
		for (size_t i = 0; i < points.size(); i++)
		{
			r.setPosition(Vector2f{ points.at(i).x, points.at(i).y });
			window.draw(r);
		}
		window.draw(instructions);
		window.draw(iterText);
		// Show everything we just drew
		window.display();
	}

	return 0;
}
