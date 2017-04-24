#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "MainWindow");
	constexpr auto cellSize = 50;
	sf::Vector2f headPos(0, 0);
	sf::RectangleShape head(sf::Vector2f(cellSize, cellSize));


	while (window.isOpen())
	{
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
				switch (event.key.code)
				{
				case sf::Keyboard::Up:
				{
					headPos.y -= cellSize;
				} break;

				case sf::Keyboard::Down:
				{
					headPos.y += cellSize;
				} break;

				case sf::Keyboard::Left:
				{
					headPos.x -= cellSize;
				} break;

				case sf::Keyboard::Right:
				{
					headPos.x += cellSize;
				} break;

				} 
			} break;
			default:
				break;
			}
		}
		window.clear();

		// Draw grid
		auto windowSize = window.getSize();
		for (auto i = 0u; i < windowSize.y; i += cellSize)
		{
			sf::Vertex line[] = { sf::Vertex(sf::Vector2f(0.0, i)), sf::Vertex(sf::Vector2f(windowSize.x, i)) };
			window.draw(line, 2, sf::PrimitiveType::Lines);
		}

		for (auto i = 0u; i < windowSize.x; i += cellSize)
		{
			sf::Vertex line[] = { sf::Vertex(sf::Vector2f(i, 0.0)), sf::Vertex(sf::Vector2f(i, windowSize.y)) };
			window.draw(line, 2, sf::PrimitiveType::Lines);
		}

		head.setPosition(headPos);
		window.draw(head);

		window.display();
	}
	return 0;
}