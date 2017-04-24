#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "MainWindow");
	
	while (window.isOpen())
	{
		// Event handling
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		// Game stuff here

		// draw to screen
		window.display();
	}

	return 0;
}