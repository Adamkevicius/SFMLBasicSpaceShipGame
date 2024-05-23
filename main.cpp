#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <sstream>
#include <fstream>
using namespace sf;
using namespace std;

enum GameState { MENU, PLAYING, PAUSED, GAME_OVER };

class Menu
{
private:
    Texture* menuTexture1;
    Texture *menuTexture2;
    Texture* menuTexture3;
    Texture* scoreBoardTexture;
    Texture *menuBackGround;
    RectangleShape *bak;
    Sprite* menu1;
    Sprite *menu2; 
    Sprite* menu3;
    Sprite* scoreBoard;
    Font font;
    Text text;
    ifstream dataFile;
    string line;
    vector<string> scores;
    bool isMenu;
    int menuNum;
    int reverseIndex;

public:
    Menu() 
    {
        menuTexture1 = new Texture();
        menuTexture2 = new Texture();
        menuTexture3 = new Texture();
        scoreBoardTexture = new Texture();
        menuBackGround = new Texture();
        bak = new RectangleShape();
        menu1 = new Sprite();
        menu2 = new Sprite();
        menu3 = new Sprite();
        scoreBoard = new Sprite();
        reverseIndex = 0;

        menuTexture1->loadFromFile("Images/Play.png");
        menuTexture2->loadFromFile("Images/ScoreBoard.png");
        menuTexture3->loadFromFile("Images/Exit.png");
        scoreBoardTexture->loadFromFile("Images/Inventory.png");
        menuBackGround->loadFromFile("Images/menuBak.jpg");
        font.loadFromFile("Images/arial_narrow_7.ttf");

        bak->setSize(Vector2f(1280, 720));
        bak->setTexture(*&menuBackGround);

        menu1->setTexture(*menuTexture1);
        menu2->setTexture(*menuTexture2);
        menu3->setTexture(*menuTexture3);
        scoreBoard->setTexture(*scoreBoardTexture);

        menu1->setPosition(550, 200);
        menu2->setPosition(522, 250);
        menu3->setPosition(550, 280);

        text.setFont(font);
    }
    ~Menu()
    {
        delete menuTexture1;
        delete menuTexture2;
        delete menuTexture3;
        delete scoreBoardTexture;
        delete menuBackGround;
        delete menu1;
        delete menu2;
        delete menu3;
        delete scoreBoard;
        dataFile.close();

    }

    void showMenu(RenderWindow& window, GameState& state) 
    {
        isMenu = true;
        menuNum = 0;

        while (isMenu) {
            menu1->setColor(Color::White);
            menu2->setColor(Color::White);
            menu3->setColor(Color::White);
            menuNum = 0;
            window.clear(Color(129, 181, 221));

            Vector2i mousePos = Mouse::getPosition(window);
            if (IntRect(550, 200, menuTexture1->getSize().x, menuTexture1->getSize().y).contains(mousePos)) 
            {
                menu1->setColor(Color::Red);
                menuNum = 1;
            }
            if (IntRect(522, 250, menuTexture2->getSize().x, menuTexture2->getSize().y).contains(mousePos)) 
            {
                menu2->setColor(Color::Red);
                menuNum = 2;
            }
            if (IntRect(550, 280, menuTexture3->getSize().x, menuTexture3->getSize().y).contains(mousePos)) 
            {
                menu3->setColor(Color::Red);
                menuNum = 3;
            }

            if (Mouse::isButtonPressed(Mouse::Left)) 
            {
                if (menuNum == 1) 
                {
                    state = PLAYING;
                    isMenu = false;
                }
                if (menuNum == 2)
                {

                    RenderWindow scoreWindow(VideoMode(10, 900), "Scoreboard");


                    dataFile.open("Data.txt");
                    

                    if (dataFile.is_open())
                    {
                        while (getline(dataFile, line))
                        {
                            scores.push_back(line);
                        }
                        dataFile.close();
                    }

                    while (scoreWindow.isOpen())
                    {
                        Event event;
                        while (scoreWindow.pollEvent(event))
                        {
                            if (event.type == Event::Closed)
                                scoreWindow.close();
                            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                                scoreWindow.close();
                        }


                        scoreWindow.clear();

                        text.setCharacterSize(20);
                        text.setFillColor(Color::White);
                        text.setPosition(20, 20);

                        /*for (int i = 0; i < scores.size(); ++i)
                        {
                            reverseIndex = scores.size() - 1 - i;
                            text.setString(to_string(i + 1) + ". " + scores[reverseIndex]);
                            scoreWindow.draw(text);
                            text.move(0, 30);
                        }*/
                        for (int i = 0; i < scores.size(); ++i)
                        {
                            text.setString(to_string(i + 1) + ". " + scores[i]);
                            scoreWindow.draw(text);
                            text.move(0, 30);
                        }

                        scoreWindow.display();
                    }
                }
                if (menuNum == 3)
                {
                    window.close();
                    return;
                }

            }

            window.draw(*bak);
            window.draw(*menu1);
            window.draw(*menu2);
            window.draw(*menu3);
            window.display();
        }
    }
};

class GameOverMenu
{
private:
    Texture overTexture1, overTexture2;
    Sprite over1, over2;
    Text score_text;
    Text over_text;
    Font fontText;
    stringstream ss;
    bool isOver;
    int overNum;

public:
    GameOverMenu() 
    {
        overTexture1.loadFromFile("Images/Restart.png");
        overTexture2.loadFromFile("Images/Exit.png");
        over1.setTexture(overTexture1);
        over2.setTexture(overTexture2);
        over1.setPosition(550, 350);
        over2.setPosition(540, 390);

        fontText.loadFromFile("Images/arial_narrow_7.ttf");

        over_text.setFont(fontText);
        over_text.setCharacterSize(50);
        over_text.setFillColor(Color::Red);
        over_text.setPosition(537, 200);
        over_text.setString("GameOver");

        score_text.setFont(fontText);
        score_text.setCharacterSize(24);
        score_text.setFillColor(Color::Yellow);
        score_text.setPosition(555, 290);
    }


    void displayGameOver(RenderWindow& window, GameState& state, int gameScore) 
    {
        ss.str("");
        ss << gameScore;
        score_text.setString("Your score: " + ss.str());

        isOver = true;
        overNum = 0;

        Vector2i mousePosition = Mouse::getPosition(window);
        if (IntRect(550, 350, overTexture1.getSize().x, overTexture1.getSize().y).contains(mousePosition)) 
        {
            over1.setColor(Color::Red);
            overNum = 1;
        }
        if (IntRect(540, 390, overTexture2.getSize().x, overTexture2.getSize().y).contains(mousePosition)) 
        {
            over2.setColor(Color::Red);
            overNum = 2;
        }

        if (Mouse::isButtonPressed(Mouse::Left)) 
        {
            if (overNum == 1) 
            {
                state = PLAYING;
                isOver = false;
            }
            if (overNum == 2) 
            {
                state = MENU;
                isOver = false;
            }
        }

        window.clear();
        window.draw(over1);
        window.draw(over2);
        window.draw(over_text);
        window.draw(score_text);
        window.display();
    }
};

class Player
{
private:
    Texture* playerTexture;
    Sprite* playerSprite;

public:
    Player() : playerTexture(nullptr), playerSprite(nullptr) {
        playerTexture = new Texture;
        playerSprite = new Sprite;
        playerTexture->loadFromFile("Images/hero.png");
        playerSprite->setTexture(*playerTexture);
        playerSprite->scale(0.15, 0.15);
        playerSprite->setPosition(Vector2f(80, 380));
    }

    void playerMove(float time) {
        if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
            playerSprite->setRotation(0), playerSprite->move(-0.8 * time, 0);
        if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
            playerSprite->setRotation(0), playerSprite->move(1 * time, 0);
        if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up))
            playerSprite->setRotation(-25), playerSprite->move(0, -1. * time);
        if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down))
            playerSprite->setRotation(25), playerSprite->move(0, 1. * time);


        if (playerSprite->getPosition().x < 0)
            playerSprite->setPosition(0, playerSprite->getPosition().y);
        if (playerSprite->getPosition().x + playerSprite->getGlobalBounds().width > 1280)
            playerSprite->setPosition(1280 - playerSprite->getGlobalBounds().width, playerSprite->getPosition().y);
        if (playerSprite->getPosition().y < 0)
            playerSprite->setPosition(playerSprite->getPosition().x, 0);
        if (playerSprite->getPosition().y + playerSprite->getGlobalBounds().height > 720)
            playerSprite->setPosition(playerSprite->getPosition().x, 720 - playerSprite->getGlobalBounds().height);
    }

    void displayPlayer(RenderWindow& window) {
        window.draw(*playerSprite);
    }

    Sprite getSprite() {
        return *playerSprite;
    }
};

class BackGround
{
private:
    Texture *backTexture;
    RectangleShape *gamingBackground;
    RectangleShape *gamingBackground2;

public:
    BackGround() {
        backTexture = new Texture();
        gamingBackground = new RectangleShape();
        gamingBackground2 = new RectangleShape();
        backTexture->loadFromFile("Images/BackGround.jpg");
        gamingBackground->setTexture(*&backTexture);
        gamingBackground->setSize(Vector2f(1280, 720));
        gamingBackground2->setTexture(*&backTexture);
        gamingBackground2->setSize(Vector2f(1280, 720));
        gamingBackground2->setPosition(Vector2f(1280, 0));
    }

    void BackGroundMove(float time) {
        gamingBackground->move(-0.2 * time, 0);
        gamingBackground2->move(-0.2 * time, 0);
        if (gamingBackground->getPosition().x < -1280)
            gamingBackground->setPosition(1280, 0);
        if (gamingBackground2->getPosition().x < -1280)
            gamingBackground2->setPosition(1280, 0);
    }

    void displayBackGround(RenderWindow& window) {
        window.draw(*gamingBackground);
        window.draw(*gamingBackground2);
    }
};

class Enemy
{
private:
    const int size = 5;
    Texture* enemyTextures;

public:
    Enemy() {
        enemyTextures = new Texture[size];
        for (int i = 0; i < size; ++i) {
            enemyTextures[i].loadFromFile("Images/asteroid" + to_string(i + 2) + ".png");
        }
    }

    ~Enemy() {
        delete[] enemyTextures;
    }

    void spawnEnemies(vector<Sprite>& enemies, float& spawnTimer, float spawnTimerMax) {
        spawnTimer += 1 ;
        if (spawnTimer >= spawnTimerMax) {
            Sprite enemy;
            enemy.setTexture(enemyTextures[rand() % size]);
            enemy.setPosition(1280, static_cast<float>(rand() % 700));
            enemies.push_back(enemy);
            spawnTimer = 0;
        }
    }

    void moveEnemies(vector<Sprite>& enemies, float time) {
        for (size_t i = 0; i < enemies.size(); ++i) {
            enemies[i].move(-1 * time, 0);
            if (enemies[i].getPosition().x < -enemies[i].getGlobalBounds().width) {
                enemies.erase(enemies.begin() + i);
                --i;
            }
        }
    }

    void displayEnemies(RenderWindow& window, const vector<Sprite>& enemies) {
        for (const auto& enemy : enemies) {
            window.draw(enemy);
        }
    }
};

class scoreBoard
{
private:
    vector<int>score;
    ofstream Data;
public:

    scoreBoard()
    {
        Data.open("Data.txt", ios::app);
    }
    ~scoreBoard()
    {
        Data.close();
    }

    void addScoreToFile(int gameScore)
    {
        if (find(score.begin(), score.end(), gameScore) == score.end())
        {
            score.push_back(gameScore);
            Data << gameScore << endl;
        }
    }
};


class GameStart
{
private:
    RenderWindow window;
    GameState state;
    Menu menu;
    Player player;
    BackGround background;
    Enemy enemyManager;
    GameOverMenu gameOver;
    Clock clock;
    float spawnTimer;
    float spawnTimerMax;
    bool showExplosion;
    int gameScore;
    Font font;
    Text scoreText;
    Texture texExplosion;
    Sprite explosion;
    vector<Sprite> enemies;
    Image icon;
    scoreBoard score_Board;

public:
    GameStart(): window(VideoMode(1280, 720), "SpaceGame", Style::Default),state(MENU),spawnTimer(0), spawnTimerMax(25),showExplosion(false) 
    {
        srand(static_cast<unsigned>(time(NULL)));
        window.setFramerateLimit(60);
        int gameScore = 0;
        showExplosion = false;
        spawnTimer = 0;
        spawnTimerMax = 20;

        icon.loadFromFile("Images/icon.png");
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

        texExplosion.loadFromFile("Images/explosion.png");
        explosion.setTexture(texExplosion);
        explosion.scale(0.15, 0.15);

        font.loadFromFile("Images/arial_narrow_7.ttf");
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(Color::Yellow);
        scoreText.setPosition(640, 10);
    }

    void restartGame() {
        
        player = Player();

        enemies.clear();

        background = BackGround();


        spawnTimer = 0;
        gameScore = 0;
        showExplosion = false;

        state = PLAYING;
    }

    void runGame() {
        while (window.isOpen()) 
        {
            Event event;
            float time = clock.restart().asMicroseconds() / 6000.0;

            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape && state == PLAYING)
                    state = MENU;
            }

            switch (state) {
            case MENU:
                menu.showMenu(window, state);
                break;
            case PLAYING:
                update(time);
                displayGame();
                break;
            case PAUSED:
            case GAME_OVER:
                score_Board.addScoreToFile(gameScore);
                gameOver.displayGameOver(window, state, gameScore);
                if (state == PLAYING) {
                    restartGame();
                }
                break;
            }
        }
    }


    void update(float time) {
        background.BackGroundMove(time);
        player.playerMove(time);

        enemyManager.spawnEnemies(enemies, spawnTimer, spawnTimerMax);
        enemyManager.moveEnemies(enemies, time);

        gameScore++;

        if (checkCollisions()) {
            explosion.setPosition(player.getSprite().getPosition());
            showExplosion = true;
            state = PAUSED;
        }

        stringstream ss;
        ss << gameScore;
        scoreText.setString("Score: " + ss.str());
    }

    void displayGame() {
        window.clear();
        background.displayBackGround(window);
        player.displayPlayer(window);
        enemyManager.displayEnemies(window, enemies);
        window.draw(scoreText);
        if (showExplosion) {
            window.draw(explosion);
            showExplosion = false;
        }
        window.display();
    }

    bool checkCollisions() {
        for (const auto& enemy : enemies) {
            if (player.getSprite().getGlobalBounds().intersects(enemy.getGlobalBounds())) {
                return true;
            }
        }
        return false;
    }
};


int main() {
    GameStart gameStart;
    gameStart.runGame();
    return 0;
}
