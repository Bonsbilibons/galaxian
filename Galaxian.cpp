#include <iostream>
#include <ctime>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <thread>
#include <chrono>
#include <future>
#include <mutex>
#include <SFML/Audio.hpp>
using namespace std;

enum Colors { RED = 12, YELLOW = 14, BLUE = 3, PINK = 13, GREEN = 2, WHITE = 2 | 3 | 4 };
enum ShaceShipModel { BASIC = 'A', FIGHTER = 'H', FUTURISTIC = 'W', FALCON = 'O' };
enum EnemyModel { E_BASIC = 'X', E_FIGHTER = 'Y', E_DESTROYER = 'V' };
enum ControlKeys { LEFT = 75, RIGHT = 77, DOWN = 80, UP = 72, ENTER = 13 , FIRE = 32};
enum Characters { SPACE = 0, WALL = 1, ENEMY = 2, FIRSTAIDKIT = 3, STAR = 4 , HERO = 5};

struct SpaceShip
{
    int X;
    int Y;
    int score;
    int health;
    int demage;
    Colors colour;
    ShaceShipModel model;
    bool shield;
};

struct EnemyShip
{
    int X;
    int Y;
    int health;
    int demage;
    EnemyModel model;
    Colors colour;
};

void ActionDetection(SpaceShip& MainShip, int* maze[], int code, int mazeHeight, int mazeWidth, int& action)
{
    //cout << code << "\n" << MainShip.X << "\n" << MainShip.Y;
    //cout << code;
    if (code == RIGHT && maze[MainShip.Y][MainShip.X + 1] != WALL)
    {
        action = RIGHT;
    }
    else if (code == LEFT && maze[MainShip.Y][MainShip.X - 1] != WALL)
    {
        action = LEFT;
    }
    else if (code == UP && maze[MainShip.Y - 1][MainShip.X] != WALL)
    {
        action = UP;
    }
    else if (code == DOWN && maze[MainShip.Y + 1][MainShip.X] != WALL)
    {
        action = DOWN;
    }
    else if (code == FIRE)
    {
        action = UP;
        action = FIRE;
    }
}

void MoveHeroPosition(SpaceShip& MainShip, int* maze[], int& action)
{
    if (action == RIGHT)
    {
        MainShip.X++;
    }
    else if (action == LEFT)
    {
        MainShip.X--;
    }
    else if (action == UP)
    {
        MainShip.Y--;
    }
    else if (action == DOWN)
    {
        MainShip.Y++;
    }
    maze[MainShip.Y][MainShip.X] = HERO;
}

void SetCursor(int x, int y, Colors color, string text)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position;
    position.X = x;
    position.Y = y;
    SetConsoleCursorPosition(h, position);
    SetConsoleTextAttribute(h, color);
    cout << text;
}

void SetCursor(int x, int y, Colors color, char text)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position;
    position.X = x;
    position.Y = y;
    SetConsoleCursorPosition(h, position);
    SetConsoleTextAttribute(h, color);
    cout << text;
}

void FarAwayAnimation(string text, bool is_tread, int delay) {
    if (delay != 0) {
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
    COORD opening;
    opening.X = 40;
    opening.Y = 1;
    SetCursor(opening.X, opening.Y, YELLOW, text);
    while (opening.Y <= 28) {
        SetCursor(opening.X, opening.Y, YELLOW, "                                             ");
        opening.Y++;
        SetCursor(opening.X, opening.Y, YELLOW, text);
        if (is_tread) {
            this_thread::sleep_for(chrono::milliseconds(200));
        }
        else {
            Sleep(200);
        }
    }
    //SetCursor(opening.X, opening.Y, YELLOW, "                                             ");
}

void OpeningAnimation(string text, bool is_tread, int delay) {
    /*if (delay != 0) {
        this_thread::sleep_for(chrono::milliseconds(delay));
    }*/
    COORD opening;
    opening.X = 26;
    opening.Y = 1;

    SetCursor(opening.X, opening.Y, YELLOW, text);
    while (opening.Y <= 27) {
        SetCursor(opening.X, opening.Y, YELLOW, "                                                                             ");
        opening.Y++;
        SetCursor(opening.X, opening.Y, YELLOW, text);
        if (is_tread) {
            this_thread::sleep_for(chrono::milliseconds(500));
        }
        else {
            Sleep(500);
        }
    }
    SetCursor(opening.X, opening.Y, YELLOW, "                                                                                 ");
}

void StarShipAnimation(string text, bool is_tread, int stop) {
    COORD opening;
    opening.X = 85;
    opening.Y = 28;

    SetCursor(opening.X, opening.Y, YELLOW, text);
    while (opening.Y != stop) {
        SetCursor(opening.X, opening.Y, YELLOW, "                  ");
        opening.Y--;
        SetCursor(opening.X, opening.Y, YELLOW, text);
        if (is_tread) {
            this_thread::sleep_for(chrono::milliseconds(200));
        }
        else {
            Sleep(200);
        }
    }
}

void SetBarrier(int* maze[], int& height, int& width)
{
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x == 0 or y == 0 or y == height - 1 or x == width - 1) {
                maze[y][x] = WALL;
            }
        }
    }
}

void DrawBarrier(int* maze[], int& height, int& width)
{
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x == 0 or y == 0 or y == height - 1 or x == width - 1) {
                SetCursor(x, y, BLUE, "#");
            }
        }
    }
}

void GenerateAndDrawStars(int* maze[], int x_start, int x_end, int y_start, int y_end)
{
    for (int y = y_start; y < y_end; y++) {
        for (int x = x_start; x < x_end; x++) {
            SetCursor(x, y, WHITE, " ");
            int star = rand() % 5;
            if (star == STAR) {
                maze[y][x] = STAR;
                SetCursor(x, y, WHITE, ".");
                Sleep(1);
            }
        }
    }
}

void GenerateAndDrawStar(int* maze[], int x, int y)
{
    maze[y][x] = SPACE;
    SetCursor(x, y, WHITE, " ");
    int star = rand() % 5;
    if (star == STAR) {
        maze[y][x] = STAR;
        SetCursor(x, y, WHITE, ".");
    }
}

void ShootFromShip(int* maze[], int& height, int& width, SpaceShip& MainShip, EnemyShip& target)
{
    COORD bulletposition;
    bulletposition.X = MainShip.X;
    bulletposition.Y = MainShip.Y - 1;

    SetCursor(bulletposition.X, bulletposition.Y, YELLOW, '|');
    while ((maze[bulletposition.Y - 1][bulletposition.X] != WALL) && (maze[bulletposition.Y - 1][bulletposition.X] != ENEMY)) {
        GenerateAndDrawStar(maze, bulletposition.X, bulletposition.Y);
        bulletposition.Y--;
        SetCursor(bulletposition.X, bulletposition.Y, YELLOW, '|');
        Sleep(10);
    }
    GenerateAndDrawStar(maze, bulletposition.X, bulletposition.Y);
    if (maze[bulletposition.Y - 1][bulletposition.X] == ENEMY)
    {
        target.X = bulletposition.X;
        target.Y = bulletposition.Y - 1;
    }
}

void ShootFromEnemy(int* maze[], int& height, int& width, EnemyShip& Enemy, EnemyShip& target_ship)
{
    COORD bulletposition;
    bulletposition.X = Enemy.X;
    bulletposition.Y = Enemy.Y + 1;

    SetCursor(bulletposition.X, bulletposition.Y, RED, '|');
    while ((maze[bulletposition.Y + 1][bulletposition.X] != WALL) && (maze[bulletposition.Y + 1][bulletposition.X] != HERO)) {
        GenerateAndDrawStar(maze, bulletposition.X, bulletposition.Y);
        bulletposition.Y++;
        SetCursor(bulletposition.X, bulletposition.Y, RED, '|');
        Sleep(1);
    }
    GenerateAndDrawStar(maze, bulletposition.X, bulletposition.Y);
    if (maze[bulletposition.Y + 1][bulletposition.X] == HERO)
    {
        target_ship.X = bulletposition.X;
        target_ship.Y = bulletposition.Y + 1;
    }
}

void GenerateEnemies(int* maze[], EnemyShip* quarter, int count_of_enemies, int GameDifficulty, EnemyShip& enemy_target)
{
    for (int i = 0; i < count_of_enemies; i++)
    {
        switch (GameDifficulty)
        {
        case 1:
        {
            quarter[i].health = 50;
            enemy_target.demage = 25;
            break;
        }
        case 2:
        {
            quarter[i].health = 100;
            enemy_target.demage = 50;
            break;
        }
        case 3:
        {
            quarter[i].health = 150;
            enemy_target.demage = 100;
            break;
        }
        default:
            break;
        }
        quarter[i].X = 20 + (rand() & 80);
        quarter[i].Y = 1 + (rand() % 10);
        for (int j = 0; j < i; j++)
        {
            while (quarter[i].X == quarter[j].X)
            {
                quarter[i].X = 20 + (rand() % 80);
                j = 0; 
            }
        }
        quarter[i].colour = RED;
        quarter[i].model = E_FIGHTER;
    }
}

void ShowEnemies(int* maze[], EnemyShip* quarter, int count_of_enemies)
{
    for (int i = 0; i < count_of_enemies; i++)
    {
        if (quarter[i].health > 0)
        {
            SetCursor(quarter[i].X, quarter[i].Y, quarter[i].colour, quarter[i].model);
        }
    }
}

void MoveEnemies(int* maze[], EnemyShip* quarter, int count_of_enemies)
{
    for (int i = 0; i < count_of_enemies; i++)
    {
        if (quarter[i].health > 0)
        {
            int vector = rand() % 8;
            if (vector >= 0 and vector <= 2 and maze[quarter[i].Y][quarter[i].X - 1] != WALL and maze[quarter[i].Y][quarter[i].X - 1] != ENEMY)
            {
                GenerateAndDrawStar(maze, quarter[i].X, quarter[i].Y);
                maze[quarter[i].Y][quarter[i].X - 1] = ENEMY;
                quarter[i].X--;
                //cout << quarter[i].Y << " " << quarter[i].X;
                //cout << "LEFT";
            }
            else if (vector == 3 and maze[quarter[i].Y - 1][quarter[i].X] != WALL and maze[quarter[i].Y - 1][quarter[i].X] != ENEMY)
            {
                GenerateAndDrawStar(maze, quarter[i].X, quarter[i].Y);
                maze[quarter[i].Y - 1][quarter[i].X] = ENEMY;
                quarter[i].Y--;
                //cout << quarter[i].Y << " " << quarter[i].X;
                //cout << "UP";
            }
            else if (vector >= 4 and vector <= 6 and maze[quarter[i].Y][quarter[i].X + 1] != WALL and maze[quarter[i].Y - 1][quarter[i].X] != ENEMY)
            {
                GenerateAndDrawStar(maze, quarter[i].X, quarter[i].Y);
                maze[quarter[i].Y][quarter[i].X + 1] = ENEMY;
                quarter[i].X++;
                //cout << quarter[i].Y << " " << quarter[i].X;
                //cout << "RIGHT";
            }
            else if (vector == 7 and maze[quarter[i].Y + 1][quarter[i].X] != WALL and maze[quarter[i].Y - 1][quarter[i].X] != ENEMY and ((quarter[i].Y + 1) < 17))
            {
                GenerateAndDrawStar(maze, quarter[i].X, quarter[i].Y);
                maze[quarter[i].Y + 1][quarter[i].X] = ENEMY;
                quarter[i].Y++;
                //cout << quarter[i].Y << " " << quarter[i].X;
                //cout << "DOWN";
            }
        }
    }
}

int CheckTheHits(int* maze[], EnemyShip* quarter, int count_of_enemies, EnemyShip& target)
{
    for (int i = 0; i < count_of_enemies; i++)
    {
        if (target.X == quarter[i].X && (target.Y) == quarter[i].Y)
        {
            quarter[i].health -= target.demage;
            SetCursor(quarter[i].X, quarter[i].Y, YELLOW, quarter[i].model);
            Sleep(100);
            if (quarter[i].health <= 0)
            {
                GenerateAndDrawStar(maze, quarter[i].X, quarter[i].Y);
                return 50;
            }
            else
            {
                SetCursor(quarter[i].X, quarter[i].Y, quarter[i].colour, quarter[i].model);
                return 10;
            }
            break;
        }
    }
    return 0;
}

void CheckTheHitsFromEnemy(int* maze[], SpaceShip& MainShip, EnemyShip& enemy_target)
{
    if (enemy_target.X == MainShip.X && (enemy_target.Y) == MainShip.Y)
    {
        MainShip.health -= enemy_target.demage;
        SetCursor(MainShip.X, MainShip.Y, RED, MainShip.model);
        Sleep(100);
        SetCursor(MainShip.X, MainShip.Y, MainShip.colour, MainShip.model);
    }
}

void EnemyShootingLoop(int* maze[], int& height, int& width, EnemyShip* quarter, int& count_of_enemies,  SpaceShip& MainShip, EnemyShip& enemy_target) {
    for (int i = 0; i < count_of_enemies; i++) {
        enemy_target.X = 0;
        enemy_target.Y = 0;
        if (quarter[i].health > 0)
        {
            bool is_shooting = rand() % 2;
            if (is_shooting) {
                ShootFromEnemy(maze, height, width, quarter[i], enemy_target);
                CheckTheHitsFromEnemy(maze, MainShip, enemy_target);
            }
        }
    }
}

bool IsAllEnemiesDefeated(EnemyShip* quarter, int& count_of_enemies)
{
    int common_health = 0;
    for (int i = 0; i < count_of_enemies; i++) {
        common_health += quarter[i].health;
    }

    if (common_health < 0)
    {
        return true;
    }
    return false;
}

int main() {
    /* project setings */
    srand(time(0));
    system("title Galaxian!");

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO info;
    info.bVisible = false;
    info.dwSize = 100;
    SetConsoleCursorInfo(h, &info);
    /*  ____  */

    /* game area */
    int width = 120;
    int height = 30;
    int** maze = new int* [height];
    for (int i = 0; i < height; i++)
    {
        maze[i] = new int[width];
    }

    SetBarrier(maze, height, width);
    DrawBarrier(maze, height, width);
    /* ______ */

    /* Sound Settings*/
    sf::SoundBuffer FarFarBuffer;
    FarFarBuffer.loadFromFile("C:/Users/HP/source/repos/C++Shag/Galaxian/audio/StarWarsIntro.mp3");
    sf::Sound FarFarMusic(FarFarBuffer);
    FarFarMusic.setVolume(5);

    sf::SoundBuffer ChooseBuffer;
    ChooseBuffer.loadFromFile("C:/Users/HP/source/repos/C++Shag/Galaxian/audio/GameMenuSelectSound.wav");
    sf::Sound Choose(ChooseBuffer);
    Choose.setVolume(50);

    sf::SoundBuffer BlusterV1Buffer;
    BlusterV1Buffer.loadFromFile("C:/Users/HP/source/repos/C++Shag/Galaxian/audio/BlasterShotV1.wav");
    sf::Sound BlasterV1(BlusterV1Buffer);
    BlasterV1.setVolume(50);

    sf::SoundBuffer BlusterV2Buffer;
    BlusterV2Buffer.loadFromFile("C:/Users/HP/source/repos/C++Shag/Galaxian/audio/BlasterShotV2.wav");
    sf::Sound BlasterV2(BlusterV2Buffer);
    BlasterV2.setVolume(50);

    sf::SoundBuffer DuelOfTheFatesBuffer;
    DuelOfTheFatesBuffer.loadFromFile("C:/Users/HP/source/repos/C++Shag/Galaxian/audio/DuelOfTheFates.wav");
    sf::Sound DuelOfTheFates(DuelOfTheFatesBuffer);
    DuelOfTheFates.setVolume(50);

    sf::SoundBuffer BattleOfTheHeroesSuiteBuffer;
    BattleOfTheHeroesSuiteBuffer.loadFromFile("C:/Users/HP/source/repos/C++Shag/Galaxian/audio/BattleOfTheHeroesSuite.wav");
    sf::Sound BattleOfTheHeroesSuite(BattleOfTheHeroesSuiteBuffer);
    BattleOfTheHeroesSuite.setVolume(50);
    
    sf::SoundBuffer TheAsteroidFieldBuffer;
    TheAsteroidFieldBuffer.loadFromFile("C:/Users/HP/source/repos/C++Shag/Galaxian/audio/TheAsteroidField.wav");
    sf::Sound TheAsteroidField(TheAsteroidFieldBuffer);
    TheAsteroidField.setVolume(50);
    
    sf::SoundBuffer TheForceSuiteBuffer;
    TheForceSuiteBuffer.loadFromFile("C:/Users/HP/source/repos/C++Shag/Galaxian/audio/TheForceSuite.wav");
    sf::Sound TheForceSuite(TheForceSuiteBuffer);
    TheForceSuite.setVolume(50);
    
    sf::SoundBuffer AcrossTheStarsBuffer;
    AcrossTheStarsBuffer.loadFromFile("C:/Users/HP/source/repos/C++Shag/Galaxian/audio/AcrossTheStars.wav");
    sf::Sound AcrossTheStars(AcrossTheStarsBuffer);
    AcrossTheStars.setVolume(50);
    
    sf::SoundBuffer CantinaBandBuffer;
    CantinaBandBuffer.loadFromFile("C:/Users/HP/source/repos/C++Shag/Galaxian/audio/CantinaBand.wav");
    sf::Sound CantinaBand(CantinaBandBuffer);
    CantinaBand.setVolume(50);



    /* ___________ */

    /* Start menu */

    int choose = 0;

    SetCursor(59, 13, YELLOW, "NEW GAME");
    SetCursor(59, 14, YELLOW, "  EXIT");
    COORD cursor;
    cursor.X = 58;
    cursor.Y = 13;
    SetCursor(cursor.X, cursor.Y, BLUE, ">");

    while (choose == 0) {
        int code = _getch();
        if (code == 224) {
            code = _getch();
        }
        if (code == DOWN and cursor.Y == 13) {
            SetCursor(cursor.X, cursor.Y, BLUE, " ");
            cursor.Y++;
            SetCursor(cursor.X, cursor.Y, BLUE, ">");
        }
        if (code == UP and cursor.Y == 14) {
            SetCursor(cursor.X, cursor.Y, BLUE, " ");
            cursor.Y--;
            SetCursor(cursor.X, cursor.Y, BLUE, ">");
        }
        if (code == ENTER and cursor.Y == 13) {
            choose = 1;
        }
        if (code == ENTER and cursor.Y == 14) {
            choose = 2;
        }
        Choose.play();
    }

    if (choose == 2) {
        system("taskkill /F /IM Galaxian_alpha.exe");
        return 0;
    }

    FarFarMusic.play();


    /* End of start menu */

    /* Opening animation */
    string StarWarsTitle[] = {
    "         Striving to fix bugs, but not always does success bestow.        ",
    "      With each loop, it becomes more intriguing, debugging is an art,    ",
    "  They leave comments and gather 'pushes' on GitHub, playing their part.  ",
    "                                                                          ",
    "Enthralled by programming languages, like Jedi with lightsabers so bright,",
    "        They code through the night, like the last star's light.          ",
    "     Star Wars in the coding galaxy is the battle of light and dark,      ",
    "   But coders always seek the light, to find an escape from this spark.   ",
    "                                                                          ",
    "    Dozens of coffee cups, learning frameworks, algorithms with grace,    ",
    "    True programmers, just like Jedi, hold knowledge in their embrace.    "
    };

    thread animation1(FarAwayAnimation, "A long time ago in a galaxy far, far away....", true, 0);
    animation1.join();

    thread threads[11];
    for (int i = 0; i < 11; i++) {
        threads[i] = thread(OpeningAnimation, StarWarsTitle[i], true, 2000);
        Sleep(2500);
    }

    Sleep(6000);
    

    GenerateAndDrawStars(maze, 1, width - 1, 1, height - 1);

    string Space_Ship[] = {
    "        /\\",
    "       /  \\",
    "      /    \\",
    "     /------\\",
    "    / |    | \\",
    "   /  |    |  \\",
    "  /   |    |   \\",
    " /----|----|----\\",
    " |    | /\\ |    |",
    " |    |/  \\|    |",
    " |    |    |    |",
    " |----|----|----|"
    };

    
    thread threads2[12];
    for (int i = 0; i < 12; i++) {
        threads2[i] = thread(StarShipAnimation, Space_Ship[i], true, 5 + i);
        Sleep(1000);
    }

    Sleep(6000);
    

    /* end of opening animation */


    GenerateAndDrawStars(maze, 103, width - 1, 1, height - 1);
    GenerateAndDrawStars(maze, 75, width - 1, 16, height - 1);
    DrawBarrier(maze, height, width);


    /* main menu  */

    choose = 0;
    SetCursor(15, 13, YELLOW, "CHOOSE LEVEL OF DIFFICULTY");
    SetCursor(15, 14, YELLOW, "  EXIT ");
    cursor.X = 14;
    cursor.Y = 13;
    SetCursor(cursor.X, cursor.Y, BLUE, ">");


    while (choose == 0) {
        int code = _getch();
        if (code == 224) {
            code = _getch();
        }
        if (code == DOWN and cursor.Y == 13) {
            SetCursor(cursor.X, cursor.Y, BLUE, " ");
            cursor.Y++;
            SetCursor(cursor.X, cursor.Y, BLUE, ">");
        }
        if (code == UP and cursor.Y == 14) {
            SetCursor(cursor.X, cursor.Y, BLUE, " ");
            cursor.Y--;
            SetCursor(cursor.X, cursor.Y, BLUE, ">");
        }
        if (code == ENTER and cursor.Y == 13) {
            choose = 1;
        }
        if (code == ENTER and cursor.Y == 14) {
            choose = 2;
        }
        Choose.play();
    }

    if (choose == 2) {
        system("taskkill /F /IM Galaxian_alpha.exe");
        return 0;
    }

    GenerateAndDrawStars(maze, 15, 50, 13, 15);

    choosing_of_game_difficulty:

    int GameDifficulty;

    choose = 0;
    SetCursor(15, 13, YELLOW, "HARD");
    SetCursor(15, 14, YELLOW, "MIDDLE");
    SetCursor(15, 15, YELLOW, "EASY");
    SetCursor(15, 16, YELLOW, "EXIT");
    cursor.X = 14;
    cursor.Y = 13;
    SetCursor(cursor.X, cursor.Y, BLUE, ">");

    while (choose == 0) {
        int code = _getch();
        if (code == 224) {
            code = _getch();
        }
        if (code == DOWN and cursor.Y < 16) {
            SetCursor(cursor.X, cursor.Y, BLUE, " ");
            cursor.Y++;
            SetCursor(cursor.X, cursor.Y, BLUE, ">");
        }
        if (code == UP and cursor.Y > 13) {
            SetCursor(cursor.X, cursor.Y, BLUE, " ");
            cursor.Y--;
            SetCursor(cursor.X, cursor.Y, BLUE, ">");
        }
        if (code == ENTER and cursor.Y == 13) {
            choose = 3;
        }
        if (code == ENTER and cursor.Y == 14) {
            choose = 2;
        }
        if (code == ENTER and cursor.Y == 15) {
            choose = 1;
        }
        if (code == ENTER and cursor.Y == 16) {
            choose = 4;
        }
        Choose.play();
    }
    GameDifficulty = choose;

    if (choose == 4) {
        system("taskkill /F /IM Galaxian_alpha.exe");
        return 0;
    }

    GenerateAndDrawStars(maze, 13, 50, 13, 17);

    int level_of_diffeculty = choose;

    SpaceShip MainShip;

    choose = 0;
    SetCursor(45, 10, YELLOW, "SELECT YOUR MODEL OF SPACESHIP");
    SetCursor(45, 15, YELLOW, BASIC);
    SetCursor(55, 15, YELLOW, FIGHTER);
    SetCursor(65, 15, YELLOW, FUTURISTIC);
    SetCursor(75, 15, YELLOW, FALCON);
    cursor.X = 45;
    cursor.Y = 13;
    SetCursor(cursor.X, cursor.Y, BLUE, ">");

    while (choose == 0) {
        int code = _getch();
        if (code == 224) {
            code = _getch();
        }
        if (code == LEFT and cursor.X > 45) {
            SetCursor(cursor.X, cursor.Y, BLUE, " ");
            cursor.X-=10;
            SetCursor(cursor.X, cursor.Y, BLUE, "V");
        }
        if (code == RIGHT and cursor.X < 75) {
            SetCursor(cursor.X, cursor.Y, BLUE, " ");
            cursor.X+=10;
            SetCursor(cursor.X, cursor.Y, BLUE, "V");
        }
        if (code == ENTER and cursor.X == 45) {
            MainShip.model = BASIC;
            choose = 1;
        }
        if (code == ENTER and cursor.X == 55) {
            MainShip.model = FIGHTER;
            choose = 2;
        }
        if (code == ENTER and cursor.X == 65) {
            MainShip.model = FUTURISTIC;
            choose = 3;
        }
        if (code == ENTER and cursor.X == 75) {
            MainShip.model = FALCON;
            choose = 4;
        }
        Choose.play();
    }

    GenerateAndDrawStars(maze, 44, 76, 10, 20);

    choose = 0;
    SetCursor(40, 15, YELLOW, MainShip.model);
    SetCursor(15, 10, YELLOW, "SELECT YOUR COLOUR");
    SetCursor(15, 12, YELLOW, "YELLOW");
    SetCursor(15, 14, BLUE, "BLUE");
    SetCursor(15, 16, PINK, "PINK");
    SetCursor(15, 18, GREEN, "GREEN");
    cursor.X = 14;
    cursor.Y = 12;
    SetCursor(cursor.X, cursor.Y, BLUE, ">");

    while (choose == 0) {
        int code = _getch();
        if (code == 224) {
            code = _getch();
        }
        if (code == DOWN and cursor.Y < 18) {
            SetCursor(cursor.X, cursor.Y, BLUE, " ");
            cursor.Y+=2;
            SetCursor(cursor.X, cursor.Y, BLUE, ">");
        }
        if (code == UP and cursor.Y > 12) {
            SetCursor(cursor.X, cursor.Y, BLUE, " ");
            cursor.Y-=2;
            SetCursor(cursor.X, cursor.Y, BLUE, ">");
        }
        if (code == ENTER and cursor.Y == 12) {
            MainShip.colour = YELLOW;
            choose = 1;
        }
        if (code == ENTER and cursor.Y == 14) {
            MainShip.colour = BLUE;
            choose = 2;
        }
        if (code == ENTER and cursor.Y == 16) {
            MainShip.colour = PINK;
            choose = 3;
        }
        if (code == ENTER and cursor.Y == 18) {
            MainShip.colour = GREEN;
            choose = 4;
        }
        Choose.play();
    }

    GenerateAndDrawStars(maze, 14 , 35, 9, 19 );
    GenerateAndDrawStars(maze, 39 , 41, 14, 16 );
    GenerateAndDrawStars(maze, 70, 110, 5, 20);

    /* start of game logic  */

    MainShip.score = 0;
    MainShip.X = 60;
    MainShip.Y = 15;
    int count_of_enemies = 10;
    
start_of_game:
    short randMusic = rand() % 2;
    switch (GameDifficulty)
    {
    case 1:
    {
        if (randMusic == 0)
        {
            AcrossTheStars.play();
        }
        else {
            CantinaBand.play();
        }
        break;
    }
    case 2:
    {
        if (randMusic == 0)
        {
            TheForceSuite.play();
        }
        else {
            TheAsteroidField.play();
        }
        break;
    }
    case 3:
    {
        if (randMusic == 0)
        {
            BattleOfTheHeroesSuite.play();
        }
        else {
            DuelOfTheFates.play();
        }
        break;
    }
    default:
        break;
    }

    MainShip.demage = 55;
    MainShip.health = 100;
    MainShip.shield = false;

    
    int action = 0;

    maze[MainShip.Y][MainShip.X] = HERO;
    SetCursor(MainShip.X, MainShip.Y, MainShip.colour, MainShip.model);

    EnemyShip target;
    target.X = 0;
    target.Y = 0;
    target.demage = MainShip.demage;

    EnemyShip enemy_target;
    enemy_target.X = 0;
    enemy_target.Y = 0;

    EnemyShip* quarter = new EnemyShip[count_of_enemies];

    newCercle:
    GenerateEnemies(maze, quarter, count_of_enemies, GameDifficulty, enemy_target);
    ShowEnemies(maze, quarter, count_of_enemies);

    while (MainShip.health > 0)
    {
        target.X = 0;
        target.Y = 0;
        
        GenerateAndDrawStars(maze, width - 23, width - 1, 2, 5);
        SetCursor(width - 22, 3, YELLOW, "Your score:");
        SetCursor(width - 10, 3 , YELLOW, to_string(MainShip.score));
        SetCursor(width - 22, 4, GREEN, "Your Health:");
        SetCursor(width - 10, 4, GREEN, to_string(MainShip.health));

        if (_kbhit())
        {
            int code = _getch();
            if (code == 224) {
                code = _getch();
            }

            
            ActionDetection(MainShip, maze, code, height, width, action);
            if (action != FIRE)
            {
                GenerateAndDrawStar(maze, MainShip.X, MainShip.Y);
                MoveHeroPosition(MainShip, maze, action);
                SetCursor(MainShip.X, MainShip.Y, MainShip.colour, MainShip.model);
            } 

            if (action == FIRE)
            {
                int sound = rand() % 2;
                if (sound == 0)
                {
                    BlasterV1.play();
                }
                if (sound == 1)
                {
                    BlasterV2.play();
                }
                action = 0;
                ShootFromShip(maze, height, width, MainShip, target);
                MainShip.score += CheckTheHits(maze, quarter, count_of_enemies, target);
            }
        }
        else
        {
            bool is_moving = rand() % 2;
            if (is_moving)
            {
                MoveEnemies(maze, quarter, count_of_enemies);
                ShowEnemies(maze, quarter, count_of_enemies);
                std::thread EnemyShootingLoopThread([&]() {
                    EnemyShootingLoop(maze, height, width, quarter, count_of_enemies, MainShip, enemy_target);
                    });
                EnemyShootingLoopThread.join();
            } 
        }
        if (IsAllEnemiesDefeated(quarter, count_of_enemies))
        {
            count_of_enemies += 5;
            goto start_of_game;
        }
    }


    choose = 0;
    SetCursor(59, 12, YELLOW, "GAME OVER!");
    SetCursor(59, 13, YELLOW, "NEW GAME");
    SetCursor(59, 14, YELLOW, "  EXIT");
    cursor.X = 58;
    cursor.Y = 13;
    SetCursor(cursor.X, cursor.Y, BLUE, ">");

    while (choose == 0) {
        int code = _getch();
        if (code == 224) {
            code = _getch();
        }
        if (code == DOWN and cursor.Y == 13) {
            SetCursor(cursor.X, cursor.Y, BLUE, " ");
            cursor.Y++;
            SetCursor(cursor.X, cursor.Y, BLUE, ">");
        }
        if (code == UP and cursor.Y == 14) {
            SetCursor(cursor.X, cursor.Y, BLUE, " ");
            cursor.Y--;
            SetCursor(cursor.X, cursor.Y, BLUE, ">");
        }
        if (code == ENTER and cursor.Y == 13) {
            choose = 1;
        }
        if (code == ENTER and cursor.Y == 14) {
            choose = 2;
        }
        Choose.play();
    }

    if (choose == 1) {
        GenerateAndDrawStars(maze, 1, width - 1, 1, height - 1);
        goto choosing_of_game_difficulty;
    }
    if (choose == 2) {
        system("taskkill /F /IM Galaxian_alpha.exe");
        return 0;
    }


    Sleep(INFINITE);
}