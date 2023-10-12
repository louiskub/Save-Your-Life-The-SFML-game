#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <vector>
using namespace sf;

//#define MAX_NUMBERS_OF_ITEMS 5
#define MAX_STATE 5
#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

class Menu {
public:
	Menu(float width, float height);
	void draw(RenderWindow& window, int windowState);
	void MoveUp();
	void MoveDown();
	Text txt;
	std::vector<Text> text[MAX_STATE];
	int selectedItemIndex = 0;
private:
	Font font1;
	Font font2;
	Texture TEXTBOX;
	RectangleShape textbox, inputbox;
	std::vector<RectangleShape> textboxs;
};
Menu::Menu(float width, float height) {
	TEXTBOX.loadFromFile("Texture/Textbox3.png");

	font1.loadFromFile("Texture/Kaph-Regular.ttf");
	font2.loadFromFile("Texture/MachineGunk.ttf");

	inputbox.setSize(Vector2f(500, 60));
	inputbox.setFillColor(Color::White);
	inputbox.setOutlineColor(Color::Black);
	inputbox.setOutlineThickness(4);
	inputbox.setPosition(500, 260);
	inputbox.setOrigin(inputbox.getGlobalBounds().width / 2, inputbox.getGlobalBounds().height / 2);

	textbox.setSize(Vector2f(450, 110));
	textbox.setOrigin(Vector2f(textbox.getGlobalBounds().width / 2, 0));
	textbox.setTexture(&TEXTBOX);
	textbox.setPosition(Vector2f(width/2, 55));
	textboxs.push_back(textbox);
	textbox.setPosition(Vector2f(width / 2, 195));
	textboxs.push_back(textbox);
	textbox.setPosition(Vector2f(width / 2, 335));
	textboxs.push_back(textbox);
	textbox.setPosition(Vector2f(width / 2, 475));
	textboxs.push_back(textbox);

	txt.setFont(font1);
	txt.setOutlineThickness(5);
	txt.setOutlineColor(Color::White);
	txt.setCharacterSize(40);

	txt.setFillColor(Color::Red);
	txt.setString("Play");
	txt.setPosition(Vector2f(width / 2, 120));
	txt.setOrigin(Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
	text[1].push_back(txt);

	txt.setFillColor(Color::Black);
	txt.setString("How To Play");
	txt.setPosition(Vector2f(width / 2, 260));
	txt.setOrigin(Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
	text[1].push_back(txt);

	txt.setFillColor(Color::Black);
	txt.setString("Scoreboard");
	txt.setPosition(Vector2f(width / 2, 400));
	txt.setOrigin(Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
	text[1].push_back(txt);

	txt.setString("Exit");
	txt.setPosition(Vector2f(width / 2, 540));
	txt.setOrigin(Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
	text[1].push_back(txt);

	txt.setFont(font2);
	txt.setCharacterSize(60);
	txt.setString("Enter Your Name");
	txt.setOrigin(Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
	txt.setPosition(Vector2f(width / 2, 120));
	text[2].push_back(txt);

	txt.setString("Press Enter To Start Game");
	txt.setCharacterSize(50);
	txt.setOrigin(Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
	txt.setPosition(Vector2f(width / 2, 350));
	text[2].push_back(txt);
}
void Menu::draw(RenderWindow& window ,int windowState) {
	if (windowState == 2) {
		window.draw(inputbox);
	}
	else if (windowState == 1) {
		for (int i = 0; i < textboxs.size(); i++) 
			window.draw(textboxs[i]);
	}
	for (int i = 0; i < text[windowState].size(); i++) {
		window.draw(text[windowState][i]);
	}
}
void Menu::MoveUp() {
	if (selectedItemIndex == 0) 
		return;
	text[1][selectedItemIndex].setFillColor(Color::Black);
	selectedItemIndex--;
	text[1][selectedItemIndex].setFillColor(Color::Red);
}
void Menu::MoveDown() {
	if (selectedItemIndex == text[1].size() - 1)
		return;
	text[1][selectedItemIndex].setFillColor(Color::Black);
	selectedItemIndex++;
	text[1][selectedItemIndex].setFillColor(Color::Red);	
}



int main() {
	srand(time(NULL));
	RenderWindow window(VideoMode(1000, 700), "SFML works!", Style::Default);
	window.setFramerateLimit(60);


	while (window.isOpen()) {
		Event event;
			

		
		std::cout << windowState << "\n";
		text.setString(str);
		text.setOrigin(Vector2f(text.getGlobalBounds().width / 2, 0));
		window.clear();
		window.draw(mainmenu);
		menu.draw(window, windowState);
		if (windowState == 2) {
			window.draw(text);
		}
		window.display();
	}
	return 0;
}
