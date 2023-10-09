#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <vector>
using namespace sf;

#define maxEnemy 10
#define normalSpawnTime 3.f
#define flySpawnTime 25.f
#define bossSpawnTime 15.f
int playerScore = 0, bonusHp = 0;

class GUN {
public:
    CircleShape shape;
    float range, bulletSpeed, baseDamage;
    float startX;
    int direction;
    float cooldownTime;
    void setVar(char type, float x, float y) {
        if (type == 'm') {
            gun.shape.setPosition(x + 80 * direction, y + 47);
            gun.startX = x + 80 * direction;
            gun.baseDamage = 25;
            gun.range = 1000.f;
            gun.cooldownTime = 0.125;
            gun.bulletSpeed = 10.f;
            gun.shape.setRadius(10.f);
            gun.shape.setFillColor(Color::Red);
        }
        else if (type == 's') {
            if (direction == -1) gun.shape.setPosition(x - 100, y);
            else gun.shape.setPosition(x, y);
            gun.startX = gun.shape.getPosition().x;
            gun.baseDamage = 20;
            gun.range = 200.f;
            gun.cooldownTime = 0.5;
            gun.bulletSpeed = 20.f;
            gun.shape.setRadius(50.f);
            gun.shape.setFillColor(Color::Blue);
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
        item.shape.setSize(Vector2f(80, 80));
        item.shape.setTexture(text);
        item.shape.setTextureRect(IntRect(type * 32, 0, 32, 32));
        item.remainTime.restart();
    }
}item;

int main()
{
    srand(time(NULL));
    RenderWindow window(VideoMode(1000, 700), "SFML works!", Style::Default);
    window.setFramerateLimit(60);

    // Var
    char playerDirect = 'r', enemyDirect = 'l', gunType = 'm';
    int aniL = 0, aniR = 0, aniIdle = 0, aniFly = 0, flyTime = 0, aniBomb = 0, bonus = 0;
    int reduceExpCD = 0;
    int hpNow = 200, hpMax = 200;
    float playerSpeed = 5.f, enemySpeed = 1.5f;
    bool fly = 0, land = 0, isIdle = 0;
    Clock clockP, clockJ, bulletCooldown, clockSpawnNormal, clockSpawnBoss, clockMusic, clockBombCooldown, clockBombAni;
    std::string stringUsername = "Lung Tuu", stringBombCooldown = "  ";
    std::vector<GUN> bullets;
    std::vector<ENEMY> enemies;
    std::vector<ITEM> items;

    Font FONT;
    FONT.loadFromFile("Texture/BadComic-Regular.ttf");

    Texture MAP;
    MAP.loadFromFile("background/gameplay.png");

    Texture MAIN;
    MAIN.loadFromFile("background/mainmenu.png");

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
    SoundBuffer SHOTGUNSOUND;
    SHOTGUNSOUND.loadFromFile("audio/shotgun.mp3");
    SoundBuffer MACHINEGUNSOUND;
    MACHINEGUNSOUND.loadFromFile("audio/machinegun.mp3");
    SoundBuffer DINOBITE;
    DINOBITE.loadFromFile("audio/dinoBite.mp3");
    SoundBuffer EXPLOSIONSOUND;
    EXPLOSIONSOUND.loadFromFile("audio/explosionSound.mp3");

    // Define Object

    RectangleShape mainMenu(Vector2f(1000.f, 700.f));
    mainMenu.setTexture(&MAIN);

    RectangleShape Background(Vector2f(1000.f, 600.f));
    Background.setPosition(Vector2f(0.f, 50.f));
    Background.setTexture(&MAP);

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

    Sound backgroundSound;
    backgroundSound.setBuffer(BACKGROUNDSOUND);
    backgroundSound.setVolume(40);
    Sound shotgunSound;
    shotgunSound.setBuffer(SHOTGUNSOUND);
    shotgunSound.setVolume(20);
    Sound machineGunSound;
    machineGunSound.setBuffer(MACHINEGUNSOUND);
    machineGunSound.setVolume(30);
    Sound dinoBiteSound;
    dinoBiteSound.setBuffer(DINOBITE);
    dinoBiteSound.setVolume(80);
    backgroundSound.play();
    Sound explosionSound;
    explosionSound.setBuffer(EXPLOSIONSOUND);
    explosionSound.setVolume(60);

    IntRect hey(100, 200, 300, 400);
    std::cout << hey.left << " " << hey.top << " " << hey.width << " " << hey.height << "\n";
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();
        //backgroundSong
        if (clockMusic.getElapsedTime().asSeconds() >= 250) {
            backgroundSound.play();
            clockMusic.restart();
        }
        //Text
        if (gunType == 's')
            textUsername.setString("Username : " + stringUsername + "\n\n\nGuntype : Shotgun");
        else if (gunType == 'm')
            textUsername.setString("Username : " + stringUsername + "\n\n\nGuntype : Machine Gun");
        textScore.setString("Score : " + std::to_string(playerScore));

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

        if (Keyboard::isKeyPressed(Keyboard::Y)) {
            hpNow -= 5;
            hpRed.setSize(Vector2f(hpNow, 20));
        }
        if (Keyboard::isKeyPressed(Keyboard::U)) {
            if (gunType == 's') {
                playerGun.setTextureRect(IntRect(0, 0, 512, 320));
                gunType = 'm';
            }
            else {
                playerGun.setTextureRect(IntRect(512, 0, 512, 320));
                gunType = 's';
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::I)) playerScore++;
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
        if (Keyboard::isKeyPressed(Keyboard::O) || (clockSpawnBoss.getElapsedTime().asSeconds() >= bossSpawnTime && enemies.size() <= maxEnemy)) {
            enemy.enemyType = 'b';
            enemy.setVar(&BOSSENEMY, rand() % 2);
            enemies.push_back(enemy);
            clockSpawnBoss.restart();
        }
        if (Keyboard::isKeyPressed(Keyboard::P) || (clockSpawnNormal.getElapsedTime().asSeconds() >= normalSpawnTime && enemies.size() <= maxEnemy)) {
            enemy.enemyType = 'n';
            enemy.setVar(&NORMALENEMY, rand() % 2);
            enemies.push_back(enemy);
            clockSpawnNormal.restart();
        }
        if (Keyboard::isKeyPressed(Keyboard::LBracket) || (clockSpawnNormal.getElapsedTime().asSeconds() >= flySpawnTime && enemies.size() <= maxEnemy)) {
            enemy.enemyType = 'f';
            enemy.setVar(&FLYENEMY, rand() % 2);
            enemies.push_back(enemy);
            clockSpawnNormal.restart();
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
                    enemies[i].aniEnemy >= 17 && enemies[i].aniEnemy <= 20 &&
                    enemies[i].clockAttack.getElapsedTime().asSeconds() >= 2)
                {
                    enemies[i].clockAttack.restart();
                    hpNow = hpNow - enemies[i].baseDamage;
                    dinoBiteSound.play();
                    std::cout << hpNow << "\n";
                    // Death
                    if (hpNow <= 0) {
                        hpNow = 0;
                        hpRed.setSize(Vector2f(hpNow, 20));
                        enemies.clear();
                        bullets.clear();
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
                        }
                        else if (enemies[j].enemyType == 'b') {
                            playerScore += 30, bonusHp += 30;
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

        //Update Items
        for (int i = 0; i < items.size(); i++)
        {
            if (player.getGlobalBounds().intersects(items[i].shape.getGlobalBounds())) {
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
                items.erase(items.begin() + i);
                continue;
            }
            if (items[i].remainTime.getElapsedTime().asSeconds() > 4) {
                items.erase(items.begin() + i);
                continue;
            }
        }

        //Explosion
        if (Keyboard::isKeyPressed(Keyboard::K) && aniBomb == 0 && clockBombCooldown.getElapsedTime().asSeconds() - reduceExpCD >= 15) {
            std::cout << "Press K\n";
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

        //Draw
        window.clear();
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
        window.display();
    }

    return 0;
}

