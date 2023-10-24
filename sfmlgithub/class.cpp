#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>
#include <utility> 
#include <vector>
#include <set>

#define maxEnemy  10
#define normalSpawnTime 3.f
#define bossSpawnTime 8.f
#define MAX_ITEMS 4
#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

int scoreNow = 0, state = 0;
int bonusHp = 0, bonusSp = 0, bonusScore = 0;
using namespace sf;

class Mainmenu
{
public:
	Mainmenu(float width, float height);
	~Mainmenu();

	void draw(RenderWindow& window, int state);
	void MoveUp();
	void MoveDown();
	void drawScoreboard();
	int selectedItemIndex = 0;
	//int GetPressedItem() { return selectedItemIndex; }
	std::vector<Text> text;
	std::set<std::pair<int, std::string>, std::greater<std::pair<int, std::string>>> rank;

private:
	Font font, scoreBoardFont;
	Text menu[MAX_ITEMS], enterName, gameName, howToPlayScoreBoard[2], scoreBoardName;
	Texture BLUEBOX, REDBOX, YELLOWBOX, WASD;
	Texture ENTERNAMEBACKGROUND[2];
	RectangleShape mainBackground[2], wasdPic;
	RectangleShape textBox[MAX_ITEMS], nameBox, howToPlayScoreBoardBox[2];
	std::vector<std::pair<Text, Text>> scorePlayer;

};
Mainmenu::Mainmenu(float width, float height) {

	// LOAD
	font.loadFromFile("Prae/font/CollegiateFLF.ttf");
	scoreBoardFont.loadFromFile("Prae/font/super_plants.ttf"); 
	BLUEBOX.loadFromFile("Prae/background/text_box.png");
	REDBOX.loadFromFile("Prae/background/text_box_2.png");
	YELLOWBOX.loadFromFile("Prae/background/name_box.png");
	ENTERNAMEBACKGROUND[0].loadFromFile("Prae/background/dark_crop.png");
	ENTERNAMEBACKGROUND[1].loadFromFile("Prae/background/forest_crop.png");
	WASD.loadFromFile("Prae/background/wasd.png");


	for (int i = 0; i < 2; i++) {
		mainBackground[i].setTexture(&ENTERNAMEBACKGROUND[i]);
		mainBackground[i].setSize(Vector2f(960 - (60 * i), 540 - (60 * i)));
		mainBackground[i].setPosition(Vector2f(0 + (30 * i), 0 + (30 * i)));
	}

	// Text Setting
	// mainmenu
	gameName.setFont(font);
	gameName.setCharacterSize(70);
	gameName.setFillColor(Color::Red);
	gameName.setOutlineColor(Color::Black);
	gameName.setOutlineThickness(7);
	gameName.setString("ZERO FIGHT GUARD");
	gameName.setOrigin(Vector2f(gameName.getGlobalBounds().width / 2, gameName.getGlobalBounds().height / 2));
	gameName.setPosition(Vector2f(width / 2, 80));

	menu[0].setFont(font);
	menu[0].setCharacterSize(50);
	menu[0].setFillColor(Color::Yellow);
	menu[0].setOutlineColor(Color::Black);
	menu[0].setOutlineThickness(4);
	menu[0].setString(" play ");
	menu[0].setOrigin(Vector2f(menu[0].getGlobalBounds().width / 2, menu[0].getGlobalBounds().height / 2));
	menu[0].setPosition(Vector2f(width / 2, height / (MAX_ITEMS + 2) * 2));

	textBox[0].setTexture(&REDBOX);
	textBox[0].setSize(Vector2f(menu[0].getGlobalBounds().width * 1.8, menu[0].getGlobalBounds().height * 3));
	textBox[0].setOrigin(Vector2f(textBox[0].getGlobalBounds().width / 2, textBox[0].getGlobalBounds().height / 2));
	textBox[0].setPosition(Vector2f(width / 2 - 8, height / (MAX_ITEMS + 2) * 2 + 12));

	menu[1].setString(" how to play ");
	menu[2].setString(" scoreboard ");
	menu[3].setString("  exit  ");

	for (int i = 1; i < MAX_ITEMS; i++) {
		menu[i].setFont(font);
		menu[i].setCharacterSize(45);
		menu[i].setFillColor(Color::White);
		menu[i].setOutlineColor(Color::Black);
		menu[i].setOutlineThickness(4);
		menu[i].setOrigin(Vector2f(menu[i].getGlobalBounds().width / 2, menu[i].getGlobalBounds().height / 2));
		menu[i].setPosition(Vector2f(width / 2, height / (MAX_ITEMS + 2) * (i + 2)));

		textBox[i].setTexture(&BLUEBOX);
		textBox[i].setSize(Vector2f(menu[i].getGlobalBounds().width * 1.5, menu[i].getGlobalBounds().height * 3));
		textBox[i].setOrigin(Vector2f(textBox[i].getGlobalBounds().width / 2, textBox[i].getGlobalBounds().height / 2));
		textBox[i].setPosition(Vector2f(width / 2 - 6, height / (MAX_ITEMS + 2) * (i + 2) + 11));
	}

	// enter name
	enterName.setFont(font);
	enterName.setCharacterSize(45);
	enterName.setFillColor(Color::Yellow);
	enterName.setOutlineColor(Color::Black);
	enterName.setOutlineThickness(4);
	enterName.setString(" ENTER YOUR NAME ");
	enterName.setOrigin(Vector2f(enterName.getGlobalBounds().width / 2, enterName.getGlobalBounds().height / 2));
	enterName.setPosition(Vector2f(width / 2, height / 3 + 10));

	nameBox.setTexture(&YELLOWBOX);
	nameBox.setSize(Vector2f(enterName.getGlobalBounds().width * 1.5, enterName.getGlobalBounds().height * 4));
	nameBox.setOrigin(Vector2f(nameBox.getGlobalBounds().width / 2, nameBox.getGlobalBounds().height / 2));
	nameBox.setPosition(Vector2f(width / 2 - 8, height / 7 + 237));

	// how to play & scoreboard
	howToPlayScoreBoard[0].setString(" HOW TO PLAY ");
	howToPlayScoreBoard[1].setString(" SCOREBOARD ");

	for (int i = 0; i < 2; i++) {
		howToPlayScoreBoard[i].setFont(font);
		howToPlayScoreBoard[i].setCharacterSize(45);
		howToPlayScoreBoard[i].setFillColor(Color::White);
		howToPlayScoreBoard[i].setOutlineColor(Color::Black);
		howToPlayScoreBoard[i].setOutlineThickness(4);
		howToPlayScoreBoard[i].setOrigin(Vector2f(howToPlayScoreBoard[i].getGlobalBounds().width / 2, howToPlayScoreBoard[i].getGlobalBounds().height / 2));
		howToPlayScoreBoard[i].setPosition(Vector2f(width / 2, 50));

		howToPlayScoreBoardBox[i].setTexture(&YELLOWBOX);
		howToPlayScoreBoardBox[i].setSize(Vector2f(howToPlayScoreBoard[i].getGlobalBounds().width * 1.5, howToPlayScoreBoard[i].getGlobalBounds().height * 3.5));
		howToPlayScoreBoardBox[i].setOrigin(Vector2f(howToPlayScoreBoardBox[i].getGlobalBounds().width / 2, howToPlayScoreBoardBox[i].getGlobalBounds().height / 2));
		howToPlayScoreBoardBox[i].setPosition(Vector2f(width / 2 - 8, 55));
	}

	// how to play
	wasdPic.setTexture(&WASD);
	wasdPic.setSize(Vector2f(200, 200));
	wasdPic.setPosition(Vector2f(180, 190));

	selectedItemIndex = 0;
}

Mainmenu::~Mainmenu() {

}
void Mainmenu::draw(RenderWindow& window, int state) {
	if (state == 0) { // Mainmenu
		window.draw(gameName);
		for (int i = 0; i < MAX_ITEMS; i++) {
			window.draw(textBox[i]);
			window.draw(menu[i]);
		}
	}
	else if (state == 1) { // Enter Name
		for (int i = 0; i < 2; i++)
			window.draw(mainBackground[i]);
		window.draw(nameBox);
		window.draw(enterName);
		window.draw(scoreBoardName);
	}
	else if (state == 2) { // How To Play
		for (int i = 0; i < 2; i++)
			window.draw(mainBackground[i]);
		window.draw(howToPlayScoreBoardBox[0]);
		window.draw(howToPlayScoreBoard[0]);
		window.draw(wasdPic);
	}
	else if (state == 3) { // Scoreboard
		for (int i = 0; i < 2; i++)
			window.draw(mainBackground[i]);
		window.draw(howToPlayScoreBoardBox[1]);
		window.draw(howToPlayScoreBoard[1]);
	}
}
void Mainmenu::drawScoreboard() {

	int i = 0;
	scoreBoardName.setOrigin(0, 0);
	scoreBoardName.setFont(scoreBoardFont);
	scoreBoardName.setCharacterSize(50);
	for (auto itr : rank) {
		if (i > 4) break;
		scoreBoardName.setFillColor(Color::Black);
		scoreBoardName.setString(itr.second);
		scoreBoardName.setPosition(Vector2f(150, 90 + 90 * i));
		text.push_back(scoreBoardName);
		scoreBoardName.setString(std::to_string(itr.first));
		scoreBoardName.setPosition(Vector2f(500, 90 + 90 * i));
		text.push_back(scoreBoardName);
		i++;
	}
}
void Mainmenu::MoveUp() {
	if (selectedItemIndex - 1 >= 0) {
		menu[selectedItemIndex].setFillColor(Color::White);
		selectedItemIndex--;
		menu[selectedItemIndex].setFillColor(Color::Yellow);
	}

}
void Mainmenu::MoveDown() {
	if (selectedItemIndex + 1 < MAX_ITEMS) {
		menu[selectedItemIndex].setFillColor(Color::White);
		selectedItemIndex++;
		menu[selectedItemIndex].setFillColor(Color::Yellow);
	}
}
class MONSTER {
public:
	RectangleShape shape;
	RectangleShape hpRed, hpBlack;
	int sizeX, sizeY, hpMax, hpNow;
	int aniEnemy = 0, eneY = 0, aniDead = 0;
	int maxAniDead = 6, maxAni = 10;
	float baseDamage, enemySpeed;
	char enemydrt, enemyType;
	bool isDead = false;
	Clock clockAniEne, clockAttack, clockDeadEne;

	void setVar(Texture* texture, bool random, float positionY) {
		int bonus = int(scoreNow / 15);
		if (bonus > 200)
			bonus = 200;
		if (enemyType == 'n') {
			enemy.baseDamage = 10 + int(bonus/10), enemy.enemySpeed = 1.5 + bonus/100;
			enemy.enemydrt = 'l', enemy.eneY = 0;
			enemy.sizeX = 128, enemy.sizeY = 128;
			enemy.shape.setSize(Vector2f(90, 90));
			enemy.shape.setTextureRect(IntRect(0, 0, 128, 128));
			enemy.shape.setPosition(-90.f + random * 1050, 220.f + positionY);

			enemy.hpMax = enemy.hpNow = 60 + int(bonus*1.5);
			enemy.hpRed.setPosition(-115.f + random * 1060, 220.f + positionY);
			enemy.hpRed.setSize(Vector2f(40, 4));
			enemy.hpBlack.setOutlineThickness(1);
			enemy.hpBlack.setPosition(-115.f + random * 1060, 220.f + positionY);
			enemy.hpBlack.setSize(Vector2f(40, 4));
		}
		else if (enemyType == 'b') {
			enemy.baseDamage = 30 + int(bonus/5), enemy.enemySpeed = 1.f + bonus / 100;
			enemy.enemydrt = 'l', enemy.eneY = 40;
			enemy.sizeX = 128, enemy.sizeY = 128;
			enemy.shape.setSize(Vector2f(200, 200));
			enemy.shape.setTextureRect(IntRect(0, 0, 128, 128));
			enemy.shape.setPosition(-200.f + random * 1160, 220.f + positionY);

			enemy.hpMax = enemy.hpNow = 100 + int(bonus*3);
			enemy.hpRed.setPosition(-255.f + random * 1180, 225.f + positionY);
			enemy.hpRed.setSize(Vector2f(85, 8));
			enemy.hpBlack.setOutlineThickness(2);
			enemy.hpBlack.setPosition(-255.f + random * 1180, 225.f + positionY);
			enemy.hpBlack.setSize(Vector2f(85, 8));
		}
		enemy.shape.setTexture(texture);
		enemy.shape.setOrigin(enemy.shape.getGlobalBounds().width / 2, enemy.shape.getGlobalBounds().height / 2);
		enemy.hpRed.setFillColor(Color(200, 10, 5));
		enemy.hpRed.setOutlineColor(Color::Black);
		enemy.hpRed.setOutlineThickness(1);
		enemy.hpBlack.setFillColor(Color(64, 64, 64));
		enemy.hpBlack.setOutlineColor(Color::Black);
	}

} enemy;

int main() {
	srand(time(NULL));
	RenderWindow window(VideoMode(960, 540), "Zero Fight Guard");
	window.setFramerateLimit(60);
	Mainmenu menu(window.getSize().x, window.getSize().y);

	// VARIABLE
	char playerdrt = 'r';
	float playerSpeed = 5.f;
	int hpNow = 600, hpMax = 600, spNow = 500, spMax = 500;
	int skillBaseDamage = 70, hitBaseDamage = 30;
	bool hit = false, run = false, skill = false;
	bool playerDead = false, bonusTime = false;
	std::vector<MONSTER> enemies;
	std::string scoreString;
	std::string username;

	// TIME
	Clock clock, clockDead, clockTyped;
	Clock clockSpawnNormal, clockSpawnBoss;

	// BACKGROUND
	Texture MAINMENUBACKGROUND;
	MAINMENUBACKGROUND.loadFromFile("Prae/background/forest.png");
	Texture MAP;	MAP.loadFromFile("Prae/background/background_2.png");

	RectangleShape menuBackground(Vector2f(960.f, 540.f));
	menuBackground.setPosition(Vector2f(0.f, 0.f));
	menuBackground.setTexture(&MAINMENUBACKGROUND);
	RectangleShape gameBackground(Vector2f(960.f, 540.f));
	gameBackground.setPosition(Vector2f(0.f, 0.f));
	gameBackground.setTexture(&MAP);

	// SOUND
	SoundBuffer MENUSOUND;			MENUSOUND.loadFromFile("Prae/sound/mainmenu_sound.mp3");
	SoundBuffer GAMEPLAYSOUND;		GAMEPLAYSOUND.loadFromFile("Prae/sound/background_gameplay.mp3");
	SoundBuffer SWORDSKILLSOUND;	SWORDSKILLSOUND.loadFromFile("Prae/sound/slash_sound.mp3");
	SoundBuffer SWORDATTACKSOUND;	SWORDATTACKSOUND.loadFromFile("Prae/sound/attack_sound.mp3");
	SoundBuffer SLIMEATTACKSOUND;	SLIMEATTACKSOUND.loadFromFile("Prae/sound/slime_attack_sound.mp3");
	SoundBuffer SLIMEDEADSOUND;		SLIMEDEADSOUND.loadFromFile("Prae/sound/slime_dead.mp3");

	Sound menuSound;
	menuSound.setBuffer(MENUSOUND);
	menuSound.setVolume(40);
	menuSound.setLoop(true);
	menuSound.play();
	Sound gameplaySound;
	gameplaySound.setBuffer(GAMEPLAYSOUND);
	gameplaySound.setVolume(15);
	gameplaySound.setLoop(true);
	Sound skillSound;
	skillSound.setBuffer(SWORDSKILLSOUND);
	skillSound.setVolume(45);
	Sound attackSound;
	attackSound.setBuffer(SWORDATTACKSOUND);
	attackSound.setVolume(30);
	Sound slimeAttackSound;
	slimeAttackSound.setBuffer(SLIMEATTACKSOUND);
	slimeAttackSound.setVolume(20);
	Sound slimeDeadSound;
	slimeDeadSound.setBuffer(SLIMEDEADSOUND);
	slimeDeadSound.setVolume(60);



	// FONT & TEXT
	Font SCOREFONT;		SCOREFONT.loadFromFile("Prae/font/super_plants.ttf");
	Text Score;
	Score.setFont(SCOREFONT);
	Score.setCharacterSize(35);
	Score.setFillColor(Color::Black);
	Score.setPosition(620, 15);
	Score.setOutlineColor(Color::Yellow);
	Score.setOutlineThickness(3);

	Text text;
	text.setFont(SCOREFONT);
	text.setCharacterSize(50);
	text.setFillColor(Color::Black);
	text.setPosition(Vector2f(430, 280));

	Text HP;
	HP.setFont(SCOREFONT);
	HP.setCharacterSize(35);
	HP.setFillColor(Color::Black);
	HP.setPosition(12, 1);
	HP.setOutlineColor(Color::Green);
	HP.setOutlineThickness(3);
	HP.setString("HP");

	Text SP;
	SP.setFont(SCOREFONT);
	SP.setCharacterSize(35);
	SP.setFillColor(Color::Black);
	SP.setPosition(12, 35);
	SP.setOutlineColor(Color::Blue);
	SP.setOutlineThickness(3);
	SP.setString("SP");

	// UI
	// Hp
	RectangleShape playerHp(Vector2f(hpNow, 15));
	playerHp.setFillColor(Color::Green);
	playerHp.setPosition(70, 10);
	playerHp.setOutlineColor(Color::Black);
	playerHp.setOutlineThickness(1);

	RectangleShape playerHpBlack(Vector2f(hpMax, 15));
	playerHpBlack.setFillColor(Color(64, 64, 64));
	playerHpBlack.setPosition(70, 10);
	playerHpBlack.setOutlineColor(Color::Black);
	playerHpBlack.setOutlineThickness(2);

	// Sp
	RectangleShape playerSp(Vector2f(spNow, 15));
	playerSp.setFillColor(Color::Blue);
	playerSp.setPosition(70, 50);
	playerSp.setOutlineColor(Color::Black);
	playerSp.setOutlineThickness(1);

	RectangleShape playerSpBlack(Vector2f(spMax, 15));
	playerSpBlack.setFillColor(Color(64, 64, 64));
	playerSpBlack.setPosition(70, 50);
	playerSpBlack.setOutlineColor(Color::Black);
	playerSpBlack.setOutlineThickness(2);

	// PLAYER ACTION
	Texture IDLE;		IDLE.loadFromFile("Prae/sprite/Knight/Idle.png"); // Idle 
	Texture RUN;		RUN.loadFromFile("Prae/sprite/Knight/Run.png"); // Run 
	Texture ATTACK_S;	ATTACK_S.loadFromFile("Prae/sprite/Knight/Attack_1.png"); // Attack Skill
	Texture ATTACK_N;	ATTACK_N.loadFromFile("Prae/sprite/Knight/Attack_4.png"); // Normal attack
	Texture DEAD;		DEAD.loadFromFile("Prae/sprite/Knight/Dead.png"); // Dead

	IntRect rectRun(0, 0, 128, 128);
	IntRect rectIdle(0, 0, 128, 128);
	IntRect rectSkill(0, 0, 128, 128);
	IntRect rectHit(0, 0, 128, 128);
	IntRect rectDead(0, 0, 128, 128);

	// MONSTETR ACTION
	Texture blueSlime;	blueSlime.loadFromFile("Prae/sprite/Blue_Slime/Run+Attack.png");
	Texture redSlime;	redSlime.loadFromFile("Prae/sprite/Red_Slime/Run+Attack.png");
	Texture greenSlime; greenSlime.loadFromFile("Prae/sprite/Green_Slime/Run+Attack.png");
	Texture blueDead;	blueDead.loadFromFile("Prae/sprite/Blue_Slime/Hurt.png");
	Texture redDead;	redDead.loadFromFile("Prae/sprite/Red_Slime/Hurt.png");
	Texture greenDead; greenDead.loadFromFile("Prae/sprite/Green_Slime/Dead.png");
	//Texture whiteWolf; whiteWolf.loadFromFile("Prae/sprite/White_Werewolf/Run+Attack.png");

	// START ACTION
	// Player Start
	RectangleShape player;
	player.setTexture(&RUN);
	player.setTextureRect(rectRun);
	player.setSize(Vector2f(165, 165));
	player.setPosition(Vector2f(450.f, 300.f));
	player.setOrigin(Vector2f(player.getGlobalBounds().width / 2, player.getGlobalBounds().height / 2));

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::EventType::Closed)
				window.close();
			//if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
				//window.close();
			if (state == 1) {
				if (event.type == Event::TextEntered) {
					if (event.text.unicode == DELETE_KEY) {
						username = username.substr(0, username.size() - 1);
					}
					else if (event.text.unicode != ENTER_KEY && event.text.unicode != ESCAPE_KEY && username.size() < 15) {
						username += event.text.unicode;
						//menu.sound[5].play();
					}
				}
			}
		}

		// MAINMENU
		if (state == 5 && clockTyped.getElapsedTime().asSeconds() > 0.125) {
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				state = 0;
				menu.selectedItemIndex = 0;
				clockTyped.restart();
			}
		}
		else if (state == 0 && clockTyped.getElapsedTime().asSeconds() > 0.125) {
			if (Keyboard::isKeyPressed(Keyboard::Up)) {
				menu.MoveUp();
				clockTyped.restart();
			}
			else if (Keyboard::isKeyPressed(Keyboard::Down)) {
				menu.MoveDown();
				clockTyped.restart();
			}
			else if (Keyboard::isKeyPressed(Keyboard::Left)) {
				while (menu.selectedItemIndex != 3)
					menu.MoveDown();
				clockTyped.restart();
			}
			if (menu.selectedItemIndex == 0 && Keyboard::isKeyPressed(Keyboard::Return)) {
				//menu.sound[1].play();
				state = 1;
				clockTyped.restart();
			}
			else if (menu.selectedItemIndex == 1 && Keyboard::isKeyPressed(Keyboard::Return)) {
				//menu.sound[1].play();
				state = 2;
				clockTyped.restart();
			}
			else if (menu.selectedItemIndex == 2 && Keyboard::isKeyPressed(Keyboard::Return)) {
				//menu.sound[1].play();
				state = 3;
				clockTyped.restart();
			}
			else if (menu.selectedItemIndex == 3 && Keyboard::isKeyPressed(Keyboard::Enter)) {
				//menu.sound[1].play();
				clockTyped.restart();
				window.close();
			}
		}

		// Enter Name before start the game
		else if (state == 1 && clockTyped.getElapsedTime().asSeconds() > 0.125) {
			if (Keyboard::isKeyPressed(Keyboard::Return)) {
				//menu.sound[1].play();
				menuSound.stop();
				gameplaySound.play();
				clockTyped.restart();

				//clear everything
				enemies.clear();
				playerdrt = 'r';
				scoreNow = 0, bonusHp = 0, bonusSp = 0, bonusScore = 0;
				hpNow = 600, hpMax = 600, spNow = 500, spMax = 500;
				hit = false, run = false, skill = false;
				playerDead = false;
				clock.restart(), clockDead.restart();
				clockSpawnNormal.restart(), clockSpawnBoss.restart();
				state = 4;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Left)) {
				//menu.sound[2].play();
				state = 0;
				clockTyped.restart();
			}
		}

		// How to Play
		else if (state == 2) {
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				//menu.sound[2].play();
				state = 0;
				clockTyped.restart();
			}
		}

		// Scoreboard
		else if (state == 3) {
			//state = 5;
			FILE* fp;
			fp = fopen("ScoreBoard.txt", "r");

			while (!feof(fp)) {
				char name[30];
				int score;
				fscanf(fp, "%s %d", &name, &score);
				menu.rank.insert({ score,name });
			}
			fclose(fp);
			menu.drawScoreboard();

			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				//menu.sound[2].play();
				state = 0;
				menu.text.clear();
				clockTyped.restart();
			}
		}

		// Show Scoreboard
		//if (state == 5 && clockTyped.getElapsedTime().asSeconds() > 0.125) {
		//	if (Keyboard::isKeyPressed(Keyboard::Left)) {
		//		//menu.sound[2].play();
		//		state = 0;
		//		menu.text.clear();
		//		clockTyped.restart();
		//	}
		//}

	// UPDATE PLAYER
	// Run Right-Left
	run = false;
	if (Keyboard::isKeyPressed(Keyboard::D)) {
		playerdrt = 'r', run = true;
		if (clock.getElapsedTime().asSeconds() > 0.125f) {
			rectRun.left += 128;
			if (rectRun.left == 768)
				rectRun.left = 0;
			clock.restart();
		}
		rectIdle.left = 0;
		player.move(playerSpeed, 0.f);
	}
	else if (Keyboard::isKeyPressed(Keyboard::A)) {
		playerdrt = 'l', run = true;
		if (clock.getElapsedTime().asSeconds() > 0.125f) {
			rectRun.left += 128;
			if (rectRun.left == 768)
				rectRun.left = 0;
			clock.restart();
		}
		rectIdle.left = 0;
		player.move(-playerSpeed, 0.f);
	}

	// Set scale player
	if (playerdrt == 'r')
		player.setScale(1, 1);
	else if (playerdrt == 'l')
		player.setScale(-1, 1);

	// Run Up-Down
	if (Keyboard::isKeyPressed(Keyboard::W)) {
		run = true;
		if (clock.getElapsedTime().asSeconds() > 0.125f) {
			rectRun.left += 128;
			if (rectRun.left == 768)
				rectRun.left = 0;
			clock.restart();
		}
		rectIdle.left = 0;
		player.move(0, -playerSpeed);
	}
	else if (Keyboard::isKeyPressed(Keyboard::S)) {
		run = true;
		if (clock.getElapsedTime().asSeconds() > 0.125f) {
			rectRun.left += 128;
			if (rectRun.left == 768)
				rectRun.left = 0;
			clock.restart();
		}
		rectIdle.left = 0;
		player.move(0, playerSpeed);
	}

	// Set player's map margin
	if (player.getPosition().x < 25)
		player.setPosition(Vector2f(25, player.getPosition().y)); // left
	else if (player.getPosition().x > 930)
		player.setPosition(Vector2f(930, player.getPosition().y)); // right
	if (player.getPosition().y < 140)
		player.setPosition(Vector2f(player.getPosition().x, 140)); // top
	else if (player.getPosition().y > 390)
		player.setPosition(Vector2f(player.getPosition().x, 390)); // bottom

	// Bonus Hp Sp and Score
	bonusHp = int(scoreNow / 100); // bonus hp
	if (bonusHp > 25)
		bonusHp = 25;

	bonusSp = int(scoreNow / 100); // bonus sp
	if (bonusSp > 20)
		bonusSp = 20;

	bonusScore = int(scoreNow / 50); // bonus score
	if (bonusScore > 45)
		bonusSp = 45;

	// Attack
	// atttack Skill
	if (Keyboard::isKeyPressed(Keyboard::K) && !skill && spNow > 0 && state == 4) {
		skillSound.play();
		skill = true, run = false, rectRun.left = 0;
		if (spNow - 35 + int(bonusSp / 4) >= 0)
			spNow = spNow - 25 + int(bonusSp / 4);
		else
			spNow = 0;
	}

	// normal Attack
	if (Keyboard::isKeyPressed(Keyboard::J) && !hit && state == 4) {
		attackSound.play();
		hit = true, run = false, rectRun.left = 0;
		if (spNow + 5 + int(bonusSp / 4) < spMax)
			spNow = spNow + 5 + int(bonusSp / 4);
		else
			spNow = spMax;
	}

	// when attack slime  
	if (skill) {
		player.setTexture(&ATTACK_S);
		player.setTextureRect(rectSkill);
		if (clock.getElapsedTime().asSeconds() > 0.0625f) {
			for (int i = 0; i < enemies.size(); i++) {
				if (enemies[i].shape.getGlobalBounds().intersects(player.getGlobalBounds()) && rectSkill.left == 384 && playerdrt != enemies[i].enemydrt) {
					if (enemies[i].hpNow - skillBaseDamage > 0)
						enemies[i].hpNow -= skillBaseDamage;
					else
						enemies[i].hpNow = 0;
					enemies[i].hpRed.setSize(Vector2f(enemies[i].hpRed.getSize().x * enemies[i].hpNow / enemies[i].hpMax, enemies[i].hpBlack.getSize().y));
				}
			}
			if (rectSkill.left == 512) {
				rectSkill.left = 0;
				skill = false;
			}
			else
				rectSkill.left += 128;
			clock.restart();
		}
	}
	else if (hit) {
		player.setTexture(&ATTACK_N);
		player.setTextureRect(rectHit);
		if (clock.getElapsedTime().asSeconds() > 0.0625f) {
			for (int i = 0; i < enemies.size(); i++) {
				if (enemies[i].shape.getGlobalBounds().intersects(player.getGlobalBounds()) && rectHit.left == 384 && playerdrt != enemies[i].enemydrt) {
					if (enemies[i].hpNow - hitBaseDamage > 0)
						enemies[i].hpNow -= hitBaseDamage;
					else
						enemies[i].hpNow = 0;
					enemies[i].hpRed.setSize(Vector2f(enemies[i].hpRed.getSize().x * enemies[i].hpNow / enemies[i].hpMax, enemies[i].hpBlack.getSize().y));
				}
			}
			if (rectHit.left == 512) {
				rectHit.left = 0;
				hit = false;
			}
			else
				rectHit.left += 128;
			clock.restart();
		}
	}

	// Run
	else if (run) {
		player.setTexture(&RUN);
		player.setTextureRect(rectRun);
	}

	// Idle
	else {
		player.setTexture(&IDLE);
		player.setTextureRect(rectIdle);
		if (clock.getElapsedTime().asSeconds() > 0.0625f) {
			if (rectIdle.left == 640)
				rectIdle.left = 0;
			else
				rectIdle.left += 128;
			clock.restart();
		}
	}

	// UPDATE MONSTER
	// Spawn Slime
	if ((clockSpawnBoss.getElapsedTime().asSeconds() >= bossSpawnTime && enemies.size() <= maxEnemy)) {
		enemy.enemyType = 'b';
		enemy.setVar(&redSlime, rand() % 2, rand() % 181);
		enemies.push_back(enemy);
		clockSpawnBoss.restart();
	}
	if ((clockSpawnNormal.getElapsedTime().asSeconds() >= normalSpawnTime && enemies.size() <= maxEnemy)) {
		enemy.enemyType = 'n';
		enemy.setVar(&blueSlime, rand() % 2, rand() % 181);
		enemies.push_back(enemy);
		clockSpawnNormal.restart();
	}

	for (int i = 0; i < enemies.size(); i++) {

		// Dead slime animation & update hp and score
		// update hp and score
		if (enemies[i].hpNow == 0 && !enemies[i].isDead) {
			slimeDeadSound.play();
			if (enemies[i].enemyType == 'n') {
				enemies[i].shape.setTexture(&blueDead);
				scoreNow = scoreNow + 10 + int(bonusScore * 2 / 3);
				if (hpNow + 5 + bonusHp < hpMax)
					hpNow = hpNow + 5 + bonusHp;
				else
					hpNow = hpMax;
			}
			else if (enemies[i].enemyType == 'b') {
				enemies[i].shape.setTexture(&redDead);
				scoreNow = scoreNow + 25 + bonusScore;
				if (hpNow + 10 + int(bonusHp * 2) < hpMax)
					hpNow = hpNow + 10 + int(bonusHp * 2);
				else
					hpNow = hpMax;
			}
			enemies[i].isDead = true;
		}

		// dead slime animation
		if (enemies[i].isDead) {
			if (enemies[i].clockDeadEne.getElapsedTime().asSeconds() >= 0.175) {
				enemies[i].shape.setTextureRect(IntRect(enemies[i].sizeX * enemies[i].aniDead, 0, enemies[i].sizeX, enemies[i].sizeY));
				enemies[i].aniDead++;
				if (enemies[i].aniDead >= enemies[i].maxAniDead) {
					enemies[i].aniDead = 0;
					enemies.erase(enemies.begin() + i);
					continue;
				}
				enemies[i].clockDeadEne.restart();
			}
			enemies[i].aniEnemy = 0;
			continue;
		}

		// Slime Movement and Attack
		// slime movement
		if (enemies[i].shape.getPosition().x + 60 < player.getPosition().x) {
			enemies[i].enemydrt = 'r';
			enemies[i].shape.setScale(1, 1);
			enemies[i].shape.move(enemies[i].enemySpeed, 0);
			enemies[i].hpRed.move(enemies[i].enemySpeed, 0);
			enemies[i].hpBlack.move(enemies[i].enemySpeed, 0);
		}
		else if (enemies[i].shape.getPosition().x - 60 > player.getPosition().x) {
			enemies[i].enemydrt = 'l';
			enemies[i].shape.setScale(-1, 1);
			enemies[i].shape.move(-enemies[i].enemySpeed, 0);
			enemies[i].hpRed.move(-enemies[i].enemySpeed, 0);
			enemies[i].hpBlack.move(-enemies[i].enemySpeed, 0);
		}
		if (enemies[i].shape.getPosition().y - 35 + 1 + enemies[i].eneY < player.getPosition().y) {
			enemies[i].shape.move(0, enemies[i].enemySpeed);
			enemies[i].hpRed.move(0, enemies[i].enemySpeed);
			enemies[i].hpBlack.move(0, enemies[i].enemySpeed);
		}
		else if (enemies[i].shape.getPosition().y - 35 - 1 + enemies[i].eneY > player.getPosition().y) {
			enemies[i].shape.move(0, -enemies[i].enemySpeed);
			enemies[i].hpRed.move(0, -enemies[i].enemySpeed);
			enemies[i].hpBlack.move(0, -enemies[i].enemySpeed);
		}

		// slime attack
		if (enemies[i].clockAniEne.getElapsedTime().asSeconds() >= 0.125) {
			enemies[i].shape.setTextureRect(IntRect(enemies[i].sizeX * enemies[i].aniEnemy, 0, enemies[i].sizeX, enemies[i].sizeY));
			if (enemies[i].shape.getGlobalBounds().intersects(player.getGlobalBounds()) &&
				enemies[i].aniEnemy == 3 && enemies[i].clockAttack.getElapsedTime().asSeconds() >= 2) {
				slimeAttackSound.play();
				enemies[i].clockAttack.restart();
				if (hpNow - enemies[i].baseDamage > 0)
					hpNow = hpNow - enemies[i].baseDamage;
				else
					hpNow = 0;
				std::cout << hpNow << "\n";
			}
			enemies[i].aniEnemy++;
			if (enemies[i].aniEnemy >= enemies[i].maxAni)
				enemies[i].aniEnemy = 0;
			enemies[i].clockAniEne.restart();
		}
	}

	// Player Dead Animation
	if (hpNow == 0 && !playerDead)
		playerDead = true;
	if (playerDead) {
		player.setTexture(&DEAD);
		player.setTextureRect(rectDead);
		if (clockDead.getElapsedTime().asSeconds() >= 0.625) {
			rectDead.left += 128;
			if (rectDead.left >= 512) {
				rectDead.left = 0;
				gameplaySound.stop(), menuSound.play();
				state = 3;
				char playerName[30];
				strcpy(playerName, username.c_str());
				FILE* fp;
				fp = fopen("ScoreBoard.txt", "a");
				fprintf(fp, "%s %d\n", playerName, scoreNow);
				fclose(fp);
				break;
			}
			clockDead.restart();
		}
		rectIdle.left = 0;
	}

	// UPDATE UI
	// Update Score
	if (scoreNow >= 0)
		Score.setString("SCORE  :   " + std::to_string(scoreNow));
	//menu.text.setString("Score  :  " + std::to_string(scoreNow));
	//menu.text.setOrigin(Vector2f(menu.text.getGlobalBounds().width / 2, menu.text.getGlobalBounds().height / 2));

	// Update Hp
	playerHp.setSize(Vector2f(hpNow / 2, 15));
	playerHpBlack.setSize(Vector2f(hpMax / 2, 15));

	// Update Sp
	playerSp.setSize(Vector2f(spNow / 2, 15));
	playerSpBlack.setSize(Vector2f(spMax / 2, 15));

	// DRAW
	window.clear();

	if (state < 4) {
		window.draw(menuBackground);
		menu.draw(window, state);
		if (state == 1) {
			menu.draw(window, state);
			text.setString(username);
			text.setOrigin(Vector2f(text.getGlobalBounds().width / 2, 0));
			window.draw(text);
		}
		else if (state == 3) {
			menu.draw(window, state);
		}
	}
	else if (state == 4) {
		window.draw(gameBackground);
		window.draw(player);

		for (int i = 0; i < enemies.size(); i++)
		{
			if (!enemies[i].isDead) {
				window.draw(enemies[i].hpBlack);
				window.draw(enemies[i].hpRed);
			}
			window.draw(enemies[i].shape);
		}

		window.draw(playerHpBlack);
		window.draw(playerHp);
		window.draw(playerSpBlack);
		window.draw(playerSp);
		window.draw(Score);
		window.draw(HP);
		window.draw(SP);
	}
	window.display();
	}
}