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
using namespace sf;

#define maxEnemy 20
#define normalSpawnTime 1.5f
#define flySpawnTime 30.f
#define bossSpawnTime 15.f
#define MAX_STATE 8
#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27
int playerScore = 0, bonusHp = 0;

class GUN {
private:
    Texture BULLET;
public:
    CircleShape shape;
    float range, bulletSpeed, baseDamage;
    float startX;
    int direction;
    float cooldownTime;
    GUN() {
        BULLET.loadFromFile("Texture/bullet.png");
        gun.shape.setTexture(&BULLET);
    }
    void setVar(char type, float x, float y) {   
        gun.shape.setScale(Vector2f(direction, 1));
        if (type == 'm') {
            gun.shape.setPosition(x + 80 * gun.direction, y + 47);
            gun.startX = x + 80 * direction;
            gun.baseDamage = 25;
            gun.range = 1000.f;
            gun.cooldownTime = 0.125;
            gun.bulletSpeed = 10.f;
            gun.shape.setRadius(10.f);
            //gun.shape.setFillColor(Color::Red);
        }
        else if (type == 's') {
            if(gun.direction == 1) gun.shape.setPosition(x + 50, y);
            else gun.shape.setPosition(x-40, y);
            gun.startX = gun.shape.getPosition().x;
            gun.baseDamage = 20;
            gun.range = 150.f;
            gun.cooldownTime = 0.5;
            gun.bulletSpeed = 20.f;
            gun.shape.setRadius(50.f);
            //gun.shape.setFillColor(Color::Blue);
        }
    }
}gun;
class ENEMY {
public:
    RectangleShape shape;
    RectangleShape hpRed, hpBlack;
    int sizeX, sizeY;
    int hpMax, hpNow;
    float baseDamage;
    float enemySpeed;
    char direction, enemyType;
    int aniEnemy = 0, maxAni;
    Clock clockAniEne, clockAttack;
    void setVar(Texture* texture, bool random) {
        int bonus = int(playerScore / 10);
        if (bonus > 100)
            bonus = 100;
        if (enemyType == 'n') {
            enemy.hpMax = enemy.hpNow = 80 + bonus * 3;
            enemy.baseDamage = 20 + int(bonus / 2), enemy.enemySpeed = 1.5f;
            enemy.direction = 'l';
            enemy.sizeX = 24, enemy.sizeY = 24, enemy.maxAni = 24;
            enemy.shape.setSize(Vector2f(96, 96));
            enemy.shape.setTextureRect(IntRect(0, 0, 24, 24));
            enemy.shape.setPosition(-100.f + random * 1150, 580.f);

            enemy.hpRed.setPosition(-148.f + random * 1150, 530.f);
            enemy.hpRed.setSize(Vector2f(96, 5));
            enemy.hpBlack.setPosition(-148.f + random * 1150, 530.f);
            enemy.hpBlack.setSize(Vector2f(96, 5));
        }
        else if (enemyType == 'b') {
            enemy.hpMax = enemy.hpNow = 200 + int(bonus * 3);
            enemy.baseDamage = 40 + int(bonus / 1.5), enemy.enemySpeed = 1.f;
            enemy.direction = 'l';
            enemy.sizeX = 24, enemy.sizeY = 24, enemy.maxAni = 24;
            enemy.shape.setSize(Vector2f(144, 144));
            enemy.shape.setTextureRect(IntRect(0, 0, 24, 24));
            enemy.shape.setPosition(-200.f + random * 1200, 560.f);

            enemy.hpRed.setPosition(-290.f + random * 1200, 470.f);
            enemy.hpRed.setSize(Vector2f(180, 15));
            enemy.hpBlack.setPosition(-290.f + random * 1200, 470.f);
            enemy.hpBlack.setSize(Vector2f(180, 15));
        }
        else if (enemyType == 'f') {
            enemy.hpMax = enemy.hpNow = 20;
            enemy.baseDamage = 0, enemy.enemySpeed = 7.f;
            enemy.direction = 'l';
            enemy.sizeX = 1125, enemy.sizeY = 874, enemy.maxAni = 4;
            enemy.shape.setSize(Vector2f(96, 96));
            enemy.shape.setTextureRect(IntRect(0, 0, 1125, 874));
            enemy.shape.setPosition(-100.f + random * 1200, 440.f);
            enemy.shape.setScale(1 - 2 * random, 1);

            enemy.hpRed.setPosition(-125.f + random * 1200, 100.f);
            enemy.hpRed.setSize(Vector2f(50, 5));
            enemy.hpBlack.setPosition(-125.f + random * 1200, 100.f);
            enemy.hpBlack.setSize(Vector2f(50, 5));
        }
        enemy.shape.setTexture(texture);
        enemy.shape.setOrigin(enemy.shape.getGlobalBounds().width / 2, enemy.shape.getGlobalBounds().height / 2);
        enemy.hpRed.setFillColor(Color::Red);
        enemy.hpRed.setOutlineColor(Color::Black);
        enemy.hpRed.setOutlineThickness(1);
        enemy.hpBlack.setFillColor(Color(64, 64, 64));
        enemy.hpBlack.setOutlineColor(Color::Black);
        enemy.hpBlack.setOutlineThickness(2);
    }
}enemy;
class ITEM {
public:
    RectangleShape shape;
    Clock remainTime;
    int type;
    void setVar(Texture* text) {
        item.shape.setTexture(text);
        if (item.type < 100) {
            item.shape.setSize(Vector2f(70, 70));
            item.shape.setTextureRect(IntRect(type * 32, 0, 32, 32));
        }
        else {
            item.shape.setSize(Vector2f(112, 70));
            item.shape.setTextureRect(IntRect((type - 100) * 512, 0, 512, 320));
        }
        item.remainTime.restart();
    }
}item;
class Menu {
private:
    Font font1;
    Font font2;
    Texture TEXTBOX;
    Text txt;
    std::vector<Text> pause;
    RectangleShape textbox, inputbox, exitbox;
    SoundBuffer SOUNDBUFFER[6];
    std::vector<std::pair<Text, Text>> scoreUser;
    std::vector<RectangleShape> textboxs;
public:
    Menu(float width, float height);
    void draw(RenderWindow& window, int windowState);
    void ExitBox(RenderWindow& window);
    void MoveUp(int windowState);
    void MoveDown(int windowState);
    void CreateScoreboard();
    std::vector<Text> text[MAX_STATE];
    std::vector<RectangleShape> resumebox;
    std::set<std::pair<int, std::string>, std::greater<std::pair<int, std::string>>> rank;
    Sound sound[6];
    int selectedItemIndex = 0;
};
Menu::Menu(float width, float height) {
    TEXTBOX.loadFromFile("Texture/Textbox3.png");
    SOUNDBUFFER[0].loadFromFile("audio/mainmenu/pointer.mp3");
    SOUNDBUFFER[1].loadFromFile("audio/mainmenu/confirm.mp3");
    SOUNDBUFFER[2].loadFromFile("audio/mainmenu/cancle.mp3");
    SOUNDBUFFER[3].loadFromFile("audio/mainmenu/popup.mp3");
    SOUNDBUFFER[4].loadFromFile("audio/mainmenu/popdown.mp3");
    SOUNDBUFFER[5].loadFromFile("audio/mainmenu/type.mp3");
    for (int i = 0; i < 6; i++) {
        sound[i].setBuffer(SOUNDBUFFER[i]);
        sound[i].setVolume(25);
    }
    font1.loadFromFile("Texture/Kaph-Regular.ttf");
    font2.loadFromFile("Texture/MachineGunk.ttf");

    inputbox.setSize(Vector2f(500, 60));
    inputbox.setFillColor(Color::White);
    inputbox.setOutlineColor(Color::Black);
    inputbox.setOutlineThickness(4);
    inputbox.setPosition(500, 260);
    inputbox.setOrigin(inputbox.getGlobalBounds().width / 2, inputbox.getGlobalBounds().height / 2);

    exitbox.setSize(Vector2f(480, 300));
    exitbox.setOrigin(Vector2f(exitbox.getGlobalBounds().width / 2, 0));
    exitbox.setPosition(Vector2f(500, 200));
    exitbox.setFillColor(Color::Green);
    exitbox.setOutlineColor(Color::Black);
    exitbox.setOutlineThickness(4);

    textbox.setSize(Vector2f(400, 80));
    textbox.setFillColor(Color(190, 190, 190));
    textbox.setOutlineColor(Color::Black);
    textbox.setOutlineThickness(2);
    textbox.setOrigin(Vector2f(textbox.getGlobalBounds().width / 2, textbox.getGlobalBounds().height / 2));
    textbox.setPosition(Vector2f(width / 2, 310));
    resumebox.push_back(textbox);
    textbox.setFillColor(Color(230, 230, 230));
    textbox.setOrigin(Vector2f(textbox.getGlobalBounds().width / 2, textbox.getGlobalBounds().height / 2));
    textbox.setPosition(Vector2f(width / 2, 410));
    resumebox.push_back(textbox);

    textbox.setSize(Vector2f(450, 110));
    textbox.setOutlineThickness(0);
    textbox.setOrigin(Vector2f(textbox.getGlobalBounds().width / 2, 0));
    textbox.setTexture(&TEXTBOX);
    textbox.setPosition(Vector2f(width / 2, 55));
    textboxs.push_back(textbox);
    textbox.setPosition(Vector2f(width / 2, 195));
    textboxs.push_back(textbox);
    textbox.setPosition(Vector2f(width / 2, 335));
    textboxs.push_back(textbox);
    textbox.setPosition(Vector2f(width / 2, 475));
    textboxs.push_back(textbox);

    txt.setFont(font1);
    txt.setOutlineThickness(5);
    txt.setOutlineColor(Color::Black);
    txt.setCharacterSize(40);

    txt.setFillColor(Color::Red);
    txt.setString("Play");
    txt.setPosition(Vector2f(width / 2, 120));
    txt.setOrigin(Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
    text[1].push_back(txt);

    txt.setFillColor(Color::White);
    txt.setString("How To Play");
    txt.setPosition(Vector2f(width / 2, 260));
    txt.setOrigin(Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
    text[1].push_back(txt);

    txt.setFillColor(Color::White);
    txt.setString("Scoreboard");
    txt.setPosition(Vector2f(width / 2, 400));
    txt.setOrigin(Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
    text[1].push_back(txt);

    txt.setString("Exit");
    txt.setPosition(Vector2f(width / 2, 540));
    txt.setOrigin(Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
    text[1].push_back(txt);
    
    txt.setCharacterSize(70);
    txt.setString("How to play");
    txt.setFillColor(Color::Yellow);
    txt.setOrigin(Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
    txt.setPosition(Vector2f(width / 2, 100));
    text[3].push_back(txt);

    txt.setCharacterSize(80);
    txt.setString("Game Over!");
    txt.setFillColor(Color::Red);
    txt.setOrigin(Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
    txt.setPosition(Vector2f(width / 2, 200));
    text[0].push_back(txt);

    txt.setCharacterSize(50);
    txt.setFillColor(Color::Yellow);
    txt.setString("Enter Your Name");
    txt.setOrigin(Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
    txt.setPosition(Vector2f(width / 2, 120));
    text[2].push_back(txt);

    txt.setFont(font2);
    txt.setFillColor(Color::White);
    txt.setString("Press Enter To Start Game");
    txt.setOrigin(Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
    txt.setPosition(Vector2f(width / 2, 350));
    text[2].push_back(txt);

    txt.setOutlineColor(Color::Black);
    txt.setFillColor(Color::Yellow);
    txt.setFont(font1);
    txt.setCharacterSize(60);
    txt.setString("Paused");
    txt.setOrigin(Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
    txt.setPosition(Vector2f(width / 2, 200));
    text[7].push_back(txt);

    txt.setFillColor(Color::Red);
    txt.setFont(font2);
    txt.setCharacterSize(50);
    txt.setString("Resume");
    txt.setOrigin(Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
    txt.setPosition(Vector2f(width / 2, 300));
    text[7].push_back(txt);

    txt.setFillColor(Color::White);
    txt.setString("Back to Mainmenu");
    txt.setOrigin(Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
    txt.setPosition(Vector2f(width / 2, 400));
    text[7].push_back(txt);

    txt.setCharacterSize(70);
    txt.setFillColor(Color::Yellow);
    txt.setPosition(Vector2f(width / 2, 350));
    text[0].push_back(txt);

    txt.setFillColor(Color::White);
    txt.setCharacterSize(40);
    txt.setString("Press Esc To Back To Mainmenu");
    txt.setOrigin(Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
    txt.setPosition(Vector2f(500, 625));
    text[0].push_back(txt);
    text[2].push_back(txt);
    text[3].push_back(txt);
}
void Menu::draw(RenderWindow& window, int windowState) {
    if (windowState == 2) {
        window.draw(inputbox); 
    }else if (windowState == 1) {
        for (int i = 0; i < textboxs.size(); i++)
            window.draw(textboxs[i]);
    }
    for (int i = 0; i < text[windowState].size(); i++) {
        window.draw(text[windowState][i]);
    }
}
void Menu::CreateScoreboard() {
    txt.setFont(font1);
    txt.setCharacterSize(70);
    txt.setFillColor(Color::Yellow);
    txt.setString("Scoreboard");
    txt.setOrigin(Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
    txt.setPosition(Vector2f(500, 100));
    text[5].push_back(txt);

    int i = 0;
    txt.setOrigin(0, 0);
    txt.setFont(font2);
    txt.setCharacterSize(50);
    for (auto itr : rank) {
        if (i > 4) break;
        txt.setFillColor(Color(240,240,240));
        txt.setString(itr.second);
        txt.setPosition(Vector2f(250, 180 + 80 * i));
        text[5].push_back(txt);
        txt.setString(std::to_string(itr.first));
        txt.setPosition(Vector2f(640, 180 + 80 * i));
        text[5].push_back(txt);
        i++;
    }    
    txt.setFillColor(Color::White);
    txt.setCharacterSize(40);
    txt.setString("Press Esc To Back To Mainmenu");
    txt.setOrigin(Vector2f(txt.getGlobalBounds().width / 2, txt.getGlobalBounds().height / 2));
    txt.setPosition(Vector2f(500, 625));
    text[5].push_back(txt);
}
void Menu::ExitBox(RenderWindow& window) {
    window.draw(exitbox);
    for (auto itr : resumebox)
        window.draw(itr);
    for (int i = 0; i < text[7].size(); i++) {
        window.draw(text[7][i]);
    }
}
void Menu::MoveUp(int windowState) {
    if (selectedItemIndex == 0)
        return;
    sound[0].play();
    text[windowState][selectedItemIndex].setFillColor(Color::White);
    selectedItemIndex--;
    text[windowState][selectedItemIndex].setFillColor(Color::Red);
}
void Menu::MoveDown(int windowState) {
    if (selectedItemIndex == text[1].size() - 1)
        return;
    sound[0].play();
    text[windowState][selectedItemIndex].setFillColor(Color::White);
    selectedItemIndex++;
    text[windowState][selectedItemIndex].setFillColor(Color::Red);
}

int main()
{
    srand(time(NULL));
    RenderWindow window(VideoMode(1000, 700), "Save Your Life", Style::Default);
    window.setFramerateLimit(60);

    // Var
    int windowState = 1; // 0 = close game ----- 1 = Mainmenu ---- 2 = Ready to Play game ---- 3 = How To Play
    //  4 = ScoreBoard ---- 5 = Show Score ---- 6 = Gameplay --- 7 = Pause Game
    char playerDirect = 'r', enemyDirect = 'l', gunType = 'm';
    int aniL = 0, aniR = 0, aniIdle = 0, aniFly = 0, flyTime = 0, aniBomb = 0, bonus = 0;
    int reduceExpCD = 0;
    int hpNow = 200, hpMax = 200;
    float playerSpeed = 5.f, enemySpeed = 1.5f;
    bool fly = 0, land = 0, isIdle = 0;
    Clock clockTyped, clockP, clockJ, bulletCooldown, clockSpawnFly, clockSpawnNormal, clockSpawnBoss, clockBombCooldown, clockBombAni;
    std::string stringUsername, stringBombCooldown = "  ";
    std::vector<GUN> bullets;
    std::vector<ENEMY> enemies;
    std::vector<ITEM> items;
    Menu menu(window.getSize().x, window.getSize().y);

    Font FONT;
    FONT.loadFromFile("Texture/BadComic-Regular.ttf");

    Texture MAP;
    MAP.loadFromFile("background/gameplay.png");
    Texture MAINMENU;
    MAINMENU.loadFromFile("background/Mainmenu.jpg");
    Texture HOWTOPLAY;
    HOWTOPLAY.loadFromFile("background/Howtoplay.png");

    Texture PLAYER;
    PLAYER.loadFromFile("sprite/Run.png");
    Texture JUMP;
    JUMP.loadFromFile("sprite/Jump.png");
    Texture IDLE;
    IDLE.loadFromFile("sprite/Idle.png");
    Texture PLAYERGUN;
    PLAYERGUN.loadFromFile("Texture/gun-transformed.png");

    Texture EXPLOSION;
    EXPLOSION.loadFromFile("sprite/explosion.png");
    Texture EXPLOSIONICON;
    EXPLOSIONICON.loadFromFile("Texture/explosionIcon.png");

    Texture NORMALENEMY;
    NORMALENEMY.loadFromFile("sprite/normalEnemy.png");
    Texture BOSSENEMY;
    BOSSENEMY.loadFromFile("sprite/bossEnemy.png");
    Texture FLYENEMY;
    FLYENEMY.loadFromFile("sprite/flyEnemy.png");

    Texture DROPICON;
    DROPICON.loadFromFile("Texture/dropIcon.png");

    SoundBuffer BACKGROUNDSOUND;
    BACKGROUNDSOUND.loadFromFile("audio/startmenu.mp3");
    SoundBuffer THEMESOUND;
    THEMESOUND.loadFromFile("audio/mainmenu/themesong.mp3");

    SoundBuffer SHOTGUNSOUND;
    SHOTGUNSOUND.loadFromFile("audio/shotgun.mp3");
    SoundBuffer MACHINEGUNSOUND;
    MACHINEGUNSOUND.loadFromFile("audio/machinegun.mp3");
    SoundBuffer DINOBITE;
    DINOBITE.loadFromFile("audio/dinoBite.mp3");
    SoundBuffer EXPLOSIONSOUND;
    EXPLOSIONSOUND.loadFromFile("audio/explosionSound.mp3");

    // Define Object

    RectangleShape mainmenu(Vector2f(1000, 700));
    mainmenu.setTexture(&MAINMENU);

    RectangleShape Background(Vector2f(1000.f, 600.f));
    Background.setPosition(Vector2f(0.f, 50.f));
    Background.setTexture(&MAP);

    RectangleShape howtoplayBack(Vector2f(650.f, 400.f));
    howtoplayBack.setOrigin(Vector2f(howtoplayBack.getGlobalBounds().width / 2, howtoplayBack.getGlobalBounds().height / 2));
    howtoplayBack.setPosition(Vector2f(500.f, 360.f));
    howtoplayBack.setTexture(&HOWTOPLAY);
    howtoplayBack.setTextureRect(IntRect(0, 84, 1000, 532));
    howtoplayBack.setOutlineColor(Color::Black);
    howtoplayBack.setOutlineThickness(5);

    RectangleShape transparency(Vector2f(1000.f, 600.f));
    transparency.setPosition(Vector2f(0.f, 50.f));
    transparency.setFillColor(Color(256, 256, 256, 60));

    RectangleShape player(Vector2f(200, 200));
    player.setTexture(&PLAYER);
    player.setTextureRect(IntRect(0, 0, 128, 128));
    player.setPosition(200.f, 515.f);
    player.setOrigin(Vector2f(player.getGlobalBounds().width / 2, player.getGlobalBounds().height / 2));

    RectangleShape playerGun(Vector2f(80, 50));
    playerGun.setTexture(&PLAYERGUN);
    playerGun.setTextureRect(IntRect(0, 0, 512, 320));
    playerGun.setPosition(205.f, 545.f);

    RectangleShape explosion(Vector2f(250, 250));
    explosion.setTexture(&EXPLOSION);
    explosion.setTextureRect(IntRect(0, 0, 32, 32));
    RectangleShape explosionIcon(Vector2f(70, 70));
    explosionIcon.setTexture(&EXPLOSIONICON);
    explosionIcon.setPosition(700, 130);

    RectangleShape normalEnemy(Vector2f(96, 96));
    normalEnemy.setTexture(&NORMALENEMY);
    normalEnemy.setTextureRect(IntRect(0, 0, 24, 24));
    normalEnemy.setPosition(-10.f, 530.f);

    RectangleShape bossEnemy(Vector2f(96, 96));
    bossEnemy.setTexture(&BOSSENEMY);
    bossEnemy.setTextureRect(IntRect(0, 0, 24, 24));
    bossEnemy.setPosition(-10.f, 530.f);

    RectangleShape hpRed(Vector2f(hpNow, 20));
    hpRed.setFillColor(Color::Green);
    hpRed.setPosition(20, 120);
    hpRed.setOutlineColor(Color::Black);
    hpRed.setOutlineThickness(1);
    RectangleShape hpBlack(Vector2f(hpMax, 20));
    hpBlack.setFillColor(Color(64, 64, 64));
    hpBlack.setPosition(20, 120);
    hpBlack.setOutlineColor(Color::Black);
    hpBlack.setOutlineThickness(2);

    Text textUsername;
    textUsername.setFont(FONT);
    textUsername.setCharacterSize(26);
    textUsername.setFillColor(Color::Red);
    textUsername.setPosition(20, 70);
    textUsername.setOutlineColor(Color::Black);
    textUsername.setOutlineThickness(2);

    Text textScore;
    textScore.setFont(FONT);
    textScore.setCharacterSize(26);
    textScore.setFillColor(Color::Blue);
    textScore.setPosition(700, 70);
    textScore.setOutlineColor(Color::Black);
    textScore.setOutlineThickness(2);

    Text textExpCD;
    textExpCD.setFont(FONT);
    textExpCD.setCharacterSize(40);
    textExpCD.setFillColor(Color::White);
    textExpCD.setPosition(710, 140);
    textExpCD.setOutlineColor(Color::Black);
    textExpCD.setOutlineThickness(2);
    textExpCD.setString("15");

    Text text;
    text.setFont(FONT);
    text.setCharacterSize(40);
    text.setFillColor(Color::Red);
    text.setOutlineThickness(4);
    text.setOutlineColor(Color::Black);
    text.setPosition(Vector2f(1000 / 2, 230));

    Sound backgroundSound;
    backgroundSound.setBuffer(BACKGROUNDSOUND);
    backgroundSound.setVolume(40);
    backgroundSound.setLoop(true);

    Sound themeSound(THEMESOUND);
    themeSound.setVolume(100);
    themeSound.setLoop(true);
    themeSound.play();

    Sound shotgunSound;
    shotgunSound.setBuffer(SHOTGUNSOUND);
    shotgunSound.setVolume(20);
    Sound machineGunSound;
    machineGunSound.setBuffer(MACHINEGUNSOUND);
    machineGunSound.setVolume(30);
    Sound dinoBiteSound;
    dinoBiteSound.setBuffer(DINOBITE);
    dinoBiteSound.setVolume(80);
    Sound explosionSound;
    explosionSound.setBuffer(EXPLOSIONSOUND);
    explosionSound.setVolume(60);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (windowState == 2) {
                if (event.type == Event::TextEntered) {
                    if (event.text.unicode == DELETE_KEY) {
                        stringUsername = stringUsername.substr(0, stringUsername.size() - 1);
                    }
                    else if (event.text.unicode != ENTER_KEY && event.text.unicode != ESCAPE_KEY && stringUsername.size() < 12) {
                        stringUsername += event.text.unicode;
                        menu.sound[5].play();
                    }
                }
            }
        }
        // MainMenu
        if (windowState == 0 && clockTyped.getElapsedTime().asSeconds() > 0.125) {
            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                windowState = 1;
                menu.selectedItemIndex = 0;
                clockTyped.restart();
            }
        }
        else if (windowState == 1 && clockTyped.getElapsedTime().asSeconds() > 0.125) {
            if (Keyboard::isKeyPressed(Keyboard::W)) {
                menu.MoveUp(windowState);
                clockTyped.restart();
            }
            else if (Keyboard::isKeyPressed(Keyboard::S)) {
                menu.MoveDown(windowState);
                clockTyped.restart();
            }
            else if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                while (menu.selectedItemIndex != 3)
                    menu.MoveDown(windowState);
                clockTyped.restart();
            }
            if (menu.selectedItemIndex == 0 && Keyboard::isKeyPressed(Keyboard::Enter)) {
                menu.sound[1].play();
                windowState = 2;
                clockTyped.restart();
            }
            else if (menu.selectedItemIndex == 1 && Keyboard::isKeyPressed(Keyboard::Enter)) {
                menu.sound[1].play();
                windowState = 3;
                clockTyped.restart();
            }
            else if (menu.selectedItemIndex == 2 && Keyboard::isKeyPressed(Keyboard::Enter)) {
                menu.sound[1].play();
                windowState = 4;
                clockTyped.restart();
            }
            else if (menu.selectedItemIndex == 3 && Keyboard::isKeyPressed(Keyboard::Enter)) {
                menu.sound[1].play();
                clockTyped.restart();
                window.close();
            }
        }
        // Input Name
        else if (windowState == 2 && clockTyped.getElapsedTime().asSeconds() > 0.125) {
            if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                menu.sound[1].play();
                themeSound.stop();
                backgroundSound.play();
                clockTyped.restart();
                //clear everything
                enemies.clear();
                bullets.clear();
                items.clear();
                playerScore = 0, bonusHp = 0;
                playerDirect = 'r', enemyDirect = 'l', gunType = 's';
                aniL = 0, aniR = 0, aniIdle = 0, aniFly = 0, flyTime = 0, aniBomb = 0, bonus = 0;
                reduceExpCD = 0;
                hpNow = 200, hpMax = 200;
                fly = 0, land = 0, isIdle = 0;
                clockP.restart(), clockSpawnFly.restart(), clockSpawnNormal.restart(), clockSpawnBoss.restart(), clockBombCooldown.restart();
                windowState = 6;
            }
            else if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                menu.sound[2].play();
                windowState = 1;
                clockTyped.restart();
            }
        }
        // How to Play
        else if (windowState == 3) {
            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                menu.sound[2].play();
                windowState = 1;
                clockTyped.restart();
            }
        }
        // Scoreboard
        else if (windowState == 4) {
            windowState = 5;
            FILE* fp;
            fp = fopen("scoreboard.txt", "r");

            while (!feof(fp)) {
                char name[20];
                int score;
                fscanf(fp, "%s %d", &name, &score);
                menu.rank.insert({ score,name });
            }
            fclose(fp);
            menu.CreateScoreboard();
        }
        // Show Scoreboard
        if (windowState == 5 && clockTyped.getElapsedTime().asSeconds() > 0.125) {
            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                menu.sound[2].play();
                windowState = 1;
                menu.text[5].clear();
                clockTyped.restart();
            }
        }
        // Gameplay
        else if (windowState == 6) {
            if (Keyboard::isKeyPressed(Keyboard::Escape) && clockTyped.getElapsedTime().asSeconds() > 0.125) {
                menu.sound[3].play();
                windowState = 7;
                menu.selectedItemIndex = 1;
                backgroundSound.pause();
                clockTyped.restart();
            }

            //PlayerMovement
            isIdle = true;
            if (Keyboard::isKeyPressed(Keyboard::A)) {
                player.setTexture(&PLAYER);
                player.setScale(-1, 1);
                playerGun.setScale(-1, 1);
                playerDirect = 'l', isIdle = false;
                if (clockP.getElapsedTime().asSeconds() >= 0.0625) {
                    player.setTextureRect(IntRect(0 + 128 * aniL, 0, 128, 128));
                    aniL++;
                    if (aniL > 7)
                        aniL = 0;
                    clockP.restart();
                }
                aniR = 0, aniIdle = 0;
                player.move(-playerSpeed, 0);
                playerGun.move(-playerSpeed, 0);
            }
            if (Keyboard::isKeyPressed(Keyboard::D)) {
                player.setTexture(&PLAYER);
                player.setScale(1, 1);
                playerGun.setScale(1, 1);
                playerDirect = 'r', isIdle = false;
                if (clockP.getElapsedTime().asSeconds() >= 0.0625) {
                    player.setTextureRect(IntRect(0 + 128 * aniR, 0, 128, 128));
                    aniR++;
                    if (aniR > 7)
                        aniR = 0;
                    clockP.restart();
                }
                aniL = 0, aniIdle = 0;
                player.move(playerSpeed, 0);
                playerGun.move(playerSpeed, 0);
            }
            if (isIdle) {
                player.setTexture(&IDLE);
                if (clockP.getElapsedTime().asSeconds() >= 0.0625) {
                    player.setTextureRect(IntRect(0 + 128 * aniIdle, 0, 128, 128));
                    aniIdle++;
                    if (aniIdle > 5)
                        aniIdle = 0;
                    clockP.restart();
                }
                aniL = 0, aniR = 0;
            }
            if (Keyboard::isKeyPressed(Keyboard::W) && !fly && !land)
                fly = 1, land = 0;
            if (fly) {
                player.setTexture(&JUMP);
                player.move(0.f, -10.f);
                playerGun.move(0.f, -10.f);
                player.setTextureRect(IntRect(0 + 128 * aniFly, 0, 128, 128));
                if (flyTime == 3)
                    aniFly++, flyTime = 0;
                else flyTime++;
                if (player.getPosition().y == 515.f - 160)
                    fly = 0, land = 1;
            }
            else if (land) {
                player.setTexture(&JUMP);
                playerGun.move(0.f, 10.f);
                player.move(0.f, 10.f);
                player.setTextureRect(IntRect(0 + 128 * aniFly, 0, 128, 128));
                if (flyTime == 3)
                    aniFly++, flyTime = 0;
                else flyTime++;
                if (player.getPosition().y == 515.f) {
                    fly = 0, land = 0, aniFly = 0;
                    player.setTexture(&IDLE);
                    player.setTextureRect(IntRect(0, 0, 128, 128));
                }
            }


            float playerNowX = player.getPosition().x;
            float playerNowY = player.getPosition().y;

            // Enemy
            //SpawnEnimies
            if ((clockSpawnBoss.getElapsedTime().asSeconds() >= bossSpawnTime && enemies.size() <= maxEnemy)) {
                enemy.enemyType = 'b';
                enemy.setVar(&BOSSENEMY, rand() % 2);
                enemies.push_back(enemy);
                clockSpawnBoss.restart();
            }
            if ((clockSpawnNormal.getElapsedTime().asSeconds() >= normalSpawnTime && enemies.size() <= maxEnemy)) {
                enemy.enemyType = 'n';
                enemy.setVar(&NORMALENEMY, rand() % 2);
                enemies.push_back(enemy);
                clockSpawnNormal.restart();
            }
            if ((clockSpawnFly.getElapsedTime().asSeconds() >= flySpawnTime && enemies.size() <= maxEnemy)) {
                enemy.enemyType = 'f';
                enemy.setVar(&FLYENEMY, rand() % 2);
                enemies.push_back(enemy);
                clockSpawnFly.restart();
            }


            //Enemies Movement Attack Player-Death Animation
            for (int i = 0; i < enemies.size(); i++) {
                // Movement
                if (enemies[i].enemyType == 'f') {
                    enemies[i].shape.move(enemies[i].enemySpeed * enemies[i].shape.getScale().x, 0);
                    if (enemies[i].shape.getPosition().x < -200 || enemies[i].shape.getPosition().x >1200) {
                        enemies.erase(enemies.begin() + i);
                        continue;
                    }
                }
                else if (enemies[i].shape.getPosition().x + enemies[i].shape.getSize().x - 50 < playerNowX) {
                    enemies[i].direction = 'r';
                    enemies[i].shape.setScale(1, 1);
                    enemies[i].shape.move(enemies[i].enemySpeed, 0);
                    enemies[i].hpRed.move(enemies[i].enemySpeed, 0);
                    enemies[i].hpBlack.move(enemies[i].enemySpeed, 0);
                }
                else if (enemies[i].shape.getPosition().x + 50 > playerNowX + player.getSize().x / 2)
                {
                    enemies[i].direction = 'l';
                    enemies[i].shape.setScale(-1, 1);
                    enemies[i].shape.move(-enemies[i].enemySpeed, 0);
                    enemies[i].hpRed.move(-enemies[i].enemySpeed, 0);
                    enemies[i].hpBlack.move(-enemies[i].enemySpeed, 0);
                }

                // Animation and Attack
                if (enemies[i].clockAniEne.getElapsedTime().asSeconds() >= 0.125) {
                    int sizeX = enemies[i].sizeX;
                    int sizeY = enemies[i].sizeY;
                    enemies[i].shape.setTextureRect(IntRect(sizeX * enemies[i].aniEnemy, 0, sizeX, sizeY));
                    if (enemies[i].shape.getGlobalBounds().intersects(player.getGlobalBounds()) &&
                        enemies[i].aniEnemy >= 17 && enemies[i].aniEnemy <= 18 &&
                        enemies[i].clockAttack.getElapsedTime().asSeconds() >= 2)
                    {
                        enemies[i].clockAttack.restart();
                        hpNow = hpNow - enemies[i].baseDamage;
                        dinoBiteSound.play();
                        // Death
                        if (hpNow <= 0) {
                            hpNow = 0;
                            hpRed.setSize(Vector2f(hpNow, 20));
                            backgroundSound.stop(), themeSound.play();
                            windowState = 0;
                            char arr[20];
                            strcpy(arr, stringUsername.c_str());
                            FILE* fp;
                            fp = fopen("scoreboard.txt", "a");
                            fprintf(fp, "%s %d\n", arr, playerScore);
                            fclose(fp);
                            break;
                        }
                    }
                    enemies[i].aniEnemy++;
                    if (enemies[i].aniEnemy >= enemies[i].maxAni)
                        enemies[i].aniEnemy = 0;
                    enemies[i].clockAniEne.restart();
                }
            }


            //Bullet
            //IfShoot
            if (Keyboard::isKeyPressed(Keyboard::J)) {
                gun.direction = 1;
                if (playerDirect == 'l')
                    gun.direction = -1;
                gun.setVar(gunType, playerNowX, playerNowY);
                if (bulletCooldown.getElapsedTime().asSeconds() > gun.cooldownTime) {
                    if (gunType == 's') shotgunSound.play();
                    else machineGunSound.play();
                    bullets.push_back(gun);
                    bulletCooldown.restart();
                }
            }

            //BulletMovement And When bang the enemies
            for (int i = 0; i < bullets.size(); i++) {
                bullets[i].shape.move(bullets[i].direction * bullets[i].bulletSpeed, 0);
                if (bullets[i].shape.getPosition().x <0 || bullets[i].shape.getPosition().x > window.getSize().x ||
                    (bullets[i].shape.getPosition().x > bullets[i].startX + bullets[i].range && bullets[i].direction == 1) ||
                    (bullets[i].shape.getPosition().x < bullets[i].startX - bullets[i].range && bullets[i].direction == -1))
                    bullets.erase(bullets.begin() + i);
                else {
                    for (int j = 0; j < enemies.size(); j++) {
                        if (!bullets[i].shape.getGlobalBounds().intersects(enemies[j].shape.getGlobalBounds()))
                            continue;
                        bonus = int(playerScore / 100);
                        if (bonus > 16)
                            bonus = 16;
                        if (gunType == 's')
                            bonus /= 2;
                        enemies[j].hpNow -= (bullets[i].baseDamage + bonus);
                        enemies[j].hpRed.setSize(Vector2f(enemies[j].shape.getSize().x * enemies[j].hpNow / enemies[j].hpMax, enemies[j].hpBlack.getSize().y));
                        if (enemies[j].hpNow <= 0) {
                            if (enemies[j].enemyType == 'n') {
                                playerScore += 10, bonusHp += 10;
                                if (rand() % 20 == 0) {
                                    if (gunType == 'm') // Change to Shotgun
                                        item.type = 101;
                                    else if (gunType == 's') // Change to Machine gun
                                        item.type = 100;
                                    item.shape.setPosition(Vector2f(enemies[j].shape.getPosition().x, 550));
                                    item.setVar(&PLAYERGUN);
                                    items.push_back(item);
                                }
                            }
                            else if (enemies[j].enemyType == 'b') {
                                playerScore += 30, bonusHp += 30;
                                if (rand() % 10 == 0) {
                                    if (gunType == 'm')
                                        item.type = 101;
                                    else if (gunType == 's')
                                        item.type = 100;
                                    item.shape.setPosition(Vector2f(enemies[j].shape.getPosition().x, 550));
                                    item.setVar(&PLAYERGUN);
                                    items.push_back(item);
                                }
                            }
                            else if (enemies[j].enemyType == 'f') {
                                playerScore += 80, bonusHp += 80;
                                item.type = rand() % 3;
                                item.shape.setPosition(Vector2f(enemies[j].shape.getPosition().x, 550));
                                item.setVar(&DROPICON);
                                items.push_back(item);
                            }
                            enemies.erase(enemies.begin() + j);
                        }
                        if (gunType == 'm') {
                            bullets.erase(bullets.begin() + i);
                            break;
                        }
                    }
                }
            }


            //Update && Collect Items
            for (int i = 0; i < items.size(); i++) {
                if (player.getGlobalBounds().intersects(items[i].shape.getGlobalBounds()) && Keyboard::isKeyPressed(Keyboard::U)) {
                    if (items[i].type == 0) {
                        if (hpNow + 40 <= hpMax)

                            hpNow += 40;
                        else
                            hpNow = hpMax;
                    }
                    else if (items[i].type == 1) {
                        if (hpMax + 30 <= 500)
                            hpMax += 30;
                        else
                            hpMax = 500;
                        if (hpNow + 30 <= hpMax)
                            hpNow += 30;
                        else
                            hpNow = hpMax;
                    }
                    else if (items[i].type == 2) {
                        reduceExpCD = 5;
                    }
                    else if (items[i].type == 101) {
                        gunType = 's';
                    }
                    else if (items[i].type == 100) {
                        gunType = 'm';
                    }
                    items.erase(items.begin() + i);
                    continue;
                }
                if (items[i].remainTime.getElapsedTime().asSeconds() > 4) {
                    items.erase(items.begin() + i);
                    continue;
                }
            }

            //Explosion
            if (Keyboard::isKeyPressed(Keyboard::K) && aniBomb == 0 && clockBombCooldown.getElapsedTime().asSeconds() + reduceExpCD >= 15) {
                aniBomb = 1, reduceExpCD = 0;
                explosionSound.play();
                if (playerDirect == 'l') explosion.setPosition(Vector2f(playerNowX - 500, playerNowY - 125));
                else explosion.setPosition(Vector2f(playerNowX + 300, playerNowY - 125));
                clockBombCooldown.restart();

            }
            if (aniBomb != 0 && clockBombAni.getElapsedTime().asSeconds() >= 0.125) {
                explosion.setTextureRect(IntRect(32 * (aniBomb - 1), 0, 32, 32));
                for (int j = 0; j < enemies.size(); j++) {
                    if (!explosion.getGlobalBounds().intersects(enemies[j].shape.getGlobalBounds()))
                        continue;
                    enemies[j].hpNow -= 30;
                    enemies[j].hpRed.setSize(Vector2f(enemies[j].shape.getSize().x * enemies[j].hpNow / enemies[j].hpMax, enemies[j].hpBlack.getSize().y));
                    if (enemies[j].hpNow <= 0) {
                        if (enemies[j].enemyType == 'n')
                            playerScore += 10;
                        enemies.erase(enemies.begin() + j);
                    }
                }
                aniBomb++;
                if (aniBomb == 9)
                    aniBomb = 0;
            }

            //Cooldown time Explosion
            int timeBombCooldown = 15 - clockBombCooldown.getElapsedTime().asSeconds() - reduceExpCD;
            if (timeBombCooldown >= 10)     stringBombCooldown = std::to_string(timeBombCooldown);
            else if (timeBombCooldown > 0)  stringBombCooldown[0] = '0', stringBombCooldown[1] = '0' + timeBombCooldown;
            else  stringBombCooldown = "  ";
            textExpCD.setString(stringBombCooldown);

            //Update Hp
            if (bonusHp > 100) {
                if (hpMax < 500) {
                    if (hpMax + (bonusHp / 8) >= 500)
                        hpMax = 500;
                    else
                        hpMax += (bonusHp / 8);
                }
                if (hpNow + (bonusHp / 8) >= hpMax)
                    hpNow = hpMax;
                else
                    hpNow += (bonusHp / 8);
                bonusHp %= 5;
            }
            hpRed.setSize(Vector2f(hpNow, 20));
            hpBlack.setSize(Vector2f(hpMax, 20));

            //Text
            if (gunType == 's') {
                textUsername.setString("Username : " + stringUsername + "\n\n\nGuntype : Shotgun");
                playerGun.setTextureRect(IntRect(512, 0, 512, 320));
            }
            else if (gunType == 'm') {
                textUsername.setString("Username : " + stringUsername + "\n\n\nGuntype : Machine Gun");
                playerGun.setTextureRect(IntRect(0, 0, 512, 320));
            }
            textScore.setString("Score : " + std::to_string(playerScore));
            menu.text[0][1].setString("Score  :  " + std::to_string(playerScore));
            menu.text[0][1].setOrigin(Vector2f(menu.text[0][1].getGlobalBounds().width / 2, menu.text[0][1].getGlobalBounds().height / 2));

        }
        // PauseGame
        else if (windowState == 7 && clockTyped.getElapsedTime().asSeconds() > 0.125) {
            if (Keyboard::isKeyPressed(Keyboard::W) && menu.selectedItemIndex == 2) {
                menu.MoveUp(windowState);
                menu.resumebox[0].setFillColor(Color(200, 200, 200));
                menu.resumebox[1].setFillColor(Color(230, 230, 230));
                menu.text[7][2].setFillColor(Color::White);
                clockTyped.restart();
            }
            else if (Keyboard::isKeyPressed(Keyboard::S) && menu.selectedItemIndex == 1) {
                menu.MoveDown(windowState);
                menu.resumebox[1].setFillColor(Color(190, 190, 190));
                menu.resumebox[0].setFillColor(Color(230, 230, 230));
                menu.text[7][1].setFillColor(Color::White);
                clockTyped.restart();
            }
            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                if (menu.selectedItemIndex == 2) {
                    menu.MoveUp(windowState);
                    menu.resumebox[0].setFillColor(Color(200, 200, 200));
                    menu.resumebox[1].setFillColor(Color(230, 230, 230));
                    menu.text[7][2].setFillColor(Color::White);
                }
                menu.sound[4].play();
                backgroundSound.play();
                windowState = 6;
                clockTyped.restart();
            }
            else if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                if (menu.selectedItemIndex == 1) {
                    menu.sound[4].play();
                    backgroundSound.play();
                    windowState = 6;
                }
                else if (menu.selectedItemIndex == 2) {
                    menu.MoveUp(windowState);
                    menu.resumebox[0].setFillColor(Color(200, 200, 200));
                    menu.resumebox[1].setFillColor(Color(230, 230, 230));
                    menu.text[7][2].setFillColor(Color::White);
                    menu.sound[1].play();
                    backgroundSound.stop();
                    themeSound.play();
                    menu.selectedItemIndex = 0;
                    windowState = 1;
                }
                clockTyped.restart();
            }
        }

        //Draw
        window.clear();
        // Menu Case
        if (windowState < 6) {
            window.draw(mainmenu);
            menu.draw(window, windowState);
            if (windowState == 2) {
                text.setString(stringUsername);
                text.setOrigin(Vector2f(text.getGlobalBounds().width / 2, 0));
                window.draw(text);
            }
            else if (windowState == 3) {
                window.draw(howtoplayBack);
            }
        }
        // Gameplay Case
        else {
            window.draw(Background);
            window.draw(explosionIcon);
            window.draw(textUsername);
            window.draw(textScore);
            window.draw(textExpCD);
            for (int i = 0; i < items.size(); i++)
            {
                window.draw(items[i].shape);
            }
            window.draw(player);
            window.draw(playerGun);
            for (int i = 0; i < enemies.size(); i++)
            {
                window.draw(enemies[i].hpBlack);
                window.draw(enemies[i].hpRed);
                window.draw(enemies[i].shape);
            }
            for (int i = 0; i < bullets.size(); i++)
            {
                window.draw(bullets[i].shape);
            }
            window.draw(hpBlack);
            window.draw(hpRed);
            if (aniBomb > 0)
                window.draw(explosion);
            if (windowState == 7) {
                window.draw(transparency);
                menu.ExitBox(window);
            }
        }

        window.display();
    }

    return 0;
}