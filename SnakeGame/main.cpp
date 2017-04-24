#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "MainWindow");
	sf::RectangleShape shape(sf::Vector2f(300, 100));
	shape.setSize(100, 100);
	window.draw(shape);
	window.display();
	while (true);
	return 0;
}