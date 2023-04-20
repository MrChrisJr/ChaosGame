// Include important C++ libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
//test
// Make code easier to type with "using namespace"

using namespace sf;
using namespace std;
struct RGBVal {
	short r;
	short g;
	short b;
};

// Code retrieved from StackOverflow -- How to determine if a point is in a 2d triangle
float sign(Vector2f p1, Vector2f p2, Vector2f p3) {
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}
bool PointInTriangle(Vector2f pt, Vector2f v1, Vector2f v2, Vector2f v3) {
	float d1, d2, d3;
	bool has_neg, has_pos;

	d1 = sign(pt, v1, v2);
	d2 = sign(pt, v2, v3);
	d3 = sign(pt, v3, v1);

	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
}
// End of code pulled from StackOverflow

//calculateRGB calculates an int value for the distance that insidePoint is from cornerPoint, given the other 2 points on a triangle (If parallel, does not lie within triangle -> returns 0)
int calculateRGB(Vector2f insidePoint, Vector2f cornerPoint, Vector2f A, Vector2f B) {
	//This calculation is something like (1 - distance to point / distance to intersection on line AB) * 255 to return the proper 255 RGB value
	double smallerDistance = sqrt(pow(cornerPoint.x - insidePoint.x, 2) - pow(cornerPoint.y - insidePoint.y, 2));

	// Pulled from Tutorialspoint -- program for point of intersection of two lines in cplusplus (Rewritten slightly)
	float lineA, lineB, Ay, Ax, By, Bx, det;
	Ax = (insidePoint.x - cornerPoint.x);
	Ay = (cornerPoint.y - insidePoint.y);
	lineA = (Ay * insidePoint.x) + (Ax * insidePoint.y);

	Bx = (A.x - B.x);
	By = (B.y - A.y);
	lineB = (Bx * A.x) + (By * A.y);

	det = Ax * By - Ay * Bx;
	if (det == 0) return 0;
	Vector2f intersection = { (By * lineA - Ax * lineB) / det, (Ay * lineB - By * lineA) / det };
	// End of Tutorialspoint code ************************************************************************************

	double largerDistance = sqrt(pow(cornerPoint.x - intersection.x, 2) - pow(cornerPoint.y - intersection.y, 2));
	return ((1 - (smallerDistance / largerDistance)) * 255);
}

int main() {
	float width = sf::VideoMode::getDesktopMode().width;
	float height = sf::VideoMode::getDesktopMode().height;
	sf::View view(sf::FloatRect(0.f, 0.f, width, height));
	// Create a video mode object
	VideoMode vm(width, height);
	//VideoMode vm(800, 600);
	// Create and open a window for the game
	RenderWindow window(vm, "Chaos Game!", Style::Default);
	//Initializing Side counter to be 3 for Triangle, Later may add the option to change from 3 to a different number depending on user input
	const int sideCount = 3;
	// Creating an array of 2float items called Corners, these will be the first spots that someone clicks on the screen
	Vector2f corners[sideCount];
	// pointHistory is a vector of points (self-described) that will be used in building the final program.
	vector<Vector2f> pointHistory;
	//RGBValues is a vector that stores the RGB Values of each point in pointHistory that falls within the triangle
	vector<RGBVal> RGBValues;
	int RGBStoring[3] = { 0, 0, 0 };
	int placer = 0;
	/* -- Making 3 Boolians
	- needingCorners will be changed to false once the first x sides of the shape have been established and the first point, so that we will only find corners 1 time
	- isTriangle will be changed to true if sideCount remains 3 so that we can potentially do RGB Stuff
	- isInside is only needed when isTriangle is true; will be used in finding out if/when the latest point in pointHistory
	becomes inside the triangle. At which point we are able to assume that no matter where the point goes, it will remain confined to the triangle.
	*/
	bool needingCorners = true, isTriangle = false, isInside = false;
	Text messageText;
	// We need to choose a font ***************************************************
	Font font;
	font.loadFromFile("./fonts/Roboto-Black.ttf");
	messageText.setFont(font);
	messageText.setCharacterSize(22);
	//End of Fonts ****************************************************************
	int randCorner;

	while (window.isOpen()) {
		/*
		****************************************
		Handle the players input
		****************************************
		*/
		Event event;
		if (needingCorners) {
			for (int i = 0; i < sideCount; ++i) {
				while (window.pollEvent(event)) {
					if (event.type == sf::Event::Closed)
						window.close();
					if (event.type == sf::Event::MouseButtonPressed) {
						if (event.mouseButton.button == sf::Mouse::Left) {
							std::cout << "the left button was pressed" << std::endl;
							std::cout << "mouse x: " << event.mouseButton.x << std::endl;
							std::cout << "mouse y: " << event.mouseButton.y << std::endl;

							Vector2i pixelPos = { event.mouseButton.x, event.mouseButton.y };
							Vector2f worldPos = window.mapPixelToCoords(pixelPos, view);
							corners[i] = worldPos;
							//vertices.push_back(Vector2f((float)event.mouseButton.x,
							//	(float)event.mouseButton.y));
						}
					}
				}
				if (Keyboard::isKeyPressed(Keyboard::Escape)) {
					window.close();
				}
			}
			while (window.pollEvent(event)) {
				Vector2i pixelPos = { event.mouseButton.x, event.mouseButton.y };
				Vector2f worldPos = window.mapPixelToCoords(pixelPos, view);
				pointHistory.push_back(worldPos);
			}
			if (sideCount == 3) isTriangle = true;
			needingCorners = false;
		}
		/*****************************************
		Update the scene
		*****************************************/
		// Measure time
		//Time dt = clock.restart();

		// Update the text
		std::stringstream ss;
		ss << "Vertex count = " << sideCount;
		messageText.setString(ss.str());
		FloatRect textRect = messageText.getLocalBounds();
		messageText.setOrigin(textRect.left +
			textRect.width / 2.0f,
			textRect.top +
			textRect.height / 2.0f);
		messageText.setPosition(width / 2.0f, 100);

		/*****************************************
		Draw the scene
		*****************************************/
		window.setView(view);
		// Clear everything from the last run frame
		window.clear();
		// Draw our game scene here
		//Checking to make sure that user does not want to leave the scene
		if (event.type == sf::Event::Closed) { window.close(); }
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}
		randCorner = rand() % sideCount;
		Vector2f tempPoint;
		tempPoint.x = pointHistory.at(pointHistory.size() - 1).x + corners[randCorner].x;
		tempPoint.y = pointHistory.at(pointHistory.size() - 1).y + corners[randCorner].y;
		pointHistory.push_back(tempPoint);
		pointHistory.at(pointHistory.size() - 1).x /= 2;
		pointHistory.at(pointHistory.size() - 1).y /= 2;
		RectangleShape shape{ Vector2f{ 4,4 } };
		shape.setFillColor(Color::Cyan);
		if (isTriangle) {
			if (!isInside) {
				//Case: Triangle but last known location was outside the triangle need to check if isInside is true now
				isInside = PointInTriangle(pointHistory.at(pointHistory.size() - 1), corners[0], corners[1], corners[2]);
				if (isInside) { placer = pointHistory.size(); }
				// Can safely assume corners will have up to (and only 3) points due to it being a triangle
			}
			if (isInside) {
				//Case: Triangle and the current location is inside the triangle / Can't put before other if statement, the first instance that the point moves into the triangle is excluded from this
				//The corner that the point moved toward should become more of that one color (1/2 of the remaining 255 positive value)
				if (RGBStoring[randCorner] == 0) {
					//this is in the case that this is the first point within the triangle, we'll need to calculate this value (in addition to the other ones)
					RGBStoring[randCorner] = calculateRGB(pointHistory.at(pointHistory.size() - 1), corners[randCorner], corners[(randCorner + 1) % 3], corners[(randCorner + 2) % 3]);
				}
				else {
					RGBStoring[randCorner] += 255;
					RGBStoring[randCorner] /= 2;
				}
				RGBStoring[(randCorner + 1) % 3] = calculateRGB(pointHistory.at(pointHistory.size() - 1), corners[(randCorner + 1) % 3], corners[(randCorner + 2) % 3], corners[(randCorner) % 3]);
				RGBStoring[(randCorner + 2) % 3] = calculateRGB(pointHistory.at(pointHistory.size() - 1), corners[(randCorner + 2) % 3], corners[(randCorner) % 3], corners[(randCorner + 1) % 3]);
				RGBVal temp;
				temp.r = RGBStoring[0];
				temp.g = RGBStoring[1];
				temp.b = RGBStoring[2];
				RGBValues.push_back(temp);
			}
		}
		for (size_t i = 0; i < pointHistory.size(); i++) {
			shape.setFillColor (sf::Color(RGBValues.at(i - placer).r, RGBValues.at(i - placer).g, RGBValues.at(i - placer).b));
			shape.setPosition(pointHistory.at(i));
			window.draw(shape);
		}

		window.draw(messageText);
		// Show everything we just drew
		window.display();
	}

	return 0;
}