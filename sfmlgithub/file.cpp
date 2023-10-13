#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Clock Pause");

    sf::Clock clock;
    bool isPaused = false;
    sf::Time elapsedTime;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::P) {
                    if (isPaused) {
                        // Resume the clock
                        elapsedTime = clock.getElapsedTime();
                        isPaused = false;
                    }
                    else {
                        // Pause the clock
                        std::cout << "Pause";
                        clock.restart();
                        isPaused = true;
                    }
                }
            }
        }

        if (!isPaused) {
            
            // Update your game logic using the elapsed time
            sf::Time deltaTime = clock.restart();
            std::cout << elapsedTime.asSeconds() << "\n";
        }
        else {
            // You can keep track of the paused time if needed
        }
        window.clear();
        // Render your game objects
        // ...
        window.display();
    }

    return 0;
}