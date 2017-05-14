#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <deque>
#include <random>
#include <chrono>
#include <thread>
#include <algorithm>

#include "main.h"

int windowWidth = 800;
int windowHeight = 600;
constexpr auto cellSize = 50;
std::random_device randomDevice;
std::uniform_int_distribution<int> widthDist(0, (windowWidth / cellSize) - 1);
std::uniform_int_distribution<int> heightDist(0, (windowHeight / cellSize) - 1);

constexpr auto fps = 60;
constexpr auto mspf = 1000 / fps;

void drawGrid(sf::RenderWindow& window) {
	auto windowSize = window.getSize();
	for (auto i = 0u; i < windowSize.y; i += cellSize)
	{
		sf::Vertex line[] = { sf::Vertex(sf::Vector2f(0.0f, static_cast<float>(i))), sf::Vertex(sf::Vector2f(static_cast<float>(windowSize.x), static_cast<float>(i))) };
		window.draw(line, 2, sf::PrimitiveType::Lines);
	}

	for (auto i = 0u; i < windowSize.x; i += cellSize)
	{
		sf::Vertex line[] = { sf::Vertex(sf::Vector2f(static_cast<float>(i), 0.0f)), sf::Vertex(sf::Vector2f(static_cast<float>(i), static_cast<float>(windowSize.y))) };
		window.draw(line, 2, sf::PrimitiveType::Lines);
	}
}


sf::Vector2f randFoodPos(const std::deque<sf::RectangleShape>& snake) {
	sf::Vector2f randPos(0.0f, 0.0f);
	do {
		auto x = widthDist(randomDevice);
		auto y = heightDist(randomDevice);
		randPos = sf::Vector2f(static_cast<float>(x * cellSize), static_cast<float>(y * cellSize));
	} while (std::any_of(snake.begin(), snake.end(), [&randPos](auto& node) { return node.getPosition() == randPos; }));

	return randPos;
}

inline bool insideWindow(const sf::Vector2f& pos)
{
	return pos.x >= 0 && pos.x < windowWidth && pos.y >= 0 && pos.y < windowHeight;
}

enum class Direction
{
	Up,
	Down,
	Left,
	Right
};

auto getVelocity(const Direction& direction)
{
	switch (direction)
	{
	case Direction::Up:
	{
		return sf::Vector2f(0.0f, -static_cast<float>(cellSize));
	} break;
	
	case Direction::Down:
	{
		return sf::Vector2f(0.0f, static_cast<float>(cellSize));
	} break;
	
	case Direction::Left:
	{
		return sf::Vector2f(-static_cast<float>(cellSize), 0.0f);
	} break;

	default:
	case Direction::Right:
	{
		return sf::Vector2f(static_cast<float>(cellSize), 0.0f);
	} break;
	}
}

inline bool isDirectionKey(const sf::Keyboard::Key& key)
{
	return key == sf::Keyboard::Up || key == sf::Keyboard::Down || key == sf::Keyboard::Left || key == sf::Keyboard::Right;
}

inline bool opposite(const Direction& dir1, const Direction& dir2)
{
	return (dir1 == Direction::Up && dir2 == Direction::Down) ||
			(dir1 == Direction::Down && dir2 == Direction::Up) ||
			(dir1 == Direction::Left && dir2 == Direction::Right) ||
			(dir1 == Direction::Right && dir2 == Direction::Left);
}


int main()
{
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "MainWindow");
	
	std::deque<sf::RectangleShape> snakeVector;
	// FUCKING RECTANGLE SHAPE TAKES SIZE NOT POSITION 
	snakeVector.emplace_back(sf::Vector2f(static_cast<float>(cellSize), static_cast<float>(cellSize)));
	
	auto headPos = sf::Vector2f(0.0f, 0.0f);
	
	auto foodPos = randFoodPos(snakeVector);
	sf::RectangleShape food(sf::Vector2f(static_cast<float>(cellSize), static_cast<float>(cellSize)));
	food.setFillColor(sf::Color::Red);
	
	auto lastMoveTime = std::chrono::steady_clock::now();
	auto currentDirection = Direction::Right;


	while (window.isOpen())
	{
		// begin frame time
		auto startFrameTime = std::chrono::steady_clock::now();

		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				case sf::Event::Closed:
				{
					window.close();
				} break;

				case sf::Event::KeyPressed:
				{
					if (isDirectionKey(event.key.code))
					{
						auto tempDirection = Direction::Left;
						switch (event.key.code)
						{
							case sf::Keyboard::Up:
							{
								tempDirection = Direction::Up;
							} break;

							case sf::Keyboard::Down:
							{
								tempDirection = Direction::Down;
							} break;

							case sf::Keyboard::Left:
							{
								tempDirection = Direction::Left;
							} break;

							case sf::Keyboard::Right:
							{
								tempDirection = Direction::Right;
							} break;
						}

						if (!opposite(tempDirection, currentDirection))
						{
							currentDirection = tempDirection;
						}
					}
				}
				break;
				default:
					break;
			}
		}
	
		window.clear();
		drawGrid(window);
		
		// velocity
		if (startFrameTime - lastMoveTime >= std::chrono::milliseconds{ 250 })
		{
			headPos += getVelocity(currentDirection);
			// Check wall
			if (!insideWindow(headPos))
			{
				window.close();
				break;
			}

			// Check body
			if (std::any_of(snakeVector.begin(), snakeVector.end(), [&headPos](auto& node) {return node.getPosition() == headPos; }))
			{
				window.close();
				break;
			}

			sf::RectangleShape newHead(sf::Vector2f(static_cast<float>(cellSize), static_cast<float>(cellSize)));
			newHead.setPosition(headPos);
			snakeVector.push_back(newHead);

			if (headPos == foodPos) {
				foodPos = randFoodPos(snakeVector);
			}
			else
			{
				snakeVector.pop_front();
			}

			lastMoveTime = startFrameTime;
		}

		for (const sf::RectangleShape& snakePart : snakeVector) {
			window.draw(snakePart);
		}
		
		food.setPosition(foodPos);

		window.draw(food);

		// End frame time
		auto endFrameTime = std::chrono::steady_clock::now();
		auto timeElapsed = endFrameTime - startFrameTime;
		std::this_thread::sleep_for(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::milliseconds{ mspf } - timeElapsed));
		window.display();
	}
	return 0;
}
