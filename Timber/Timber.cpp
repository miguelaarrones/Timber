// Include important C++ libraries here.
#include "pch.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

using namespace sf;

// Function declaration
void UpdateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

// Where is the player/branch
// Left or Right
enum class Side { LEFT, RIGHT, NONE };
Side branchPositions[NUM_BRANCHES];

int main()
{
    // Create a video mode object
    VideoMode vm(1920, 1080);

    // Create and open a window for the game
    RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

    // Create a texture to hold a graphic from the GPU
    Texture textureBackground;

    // Load a graphic into the texture
    textureBackground.loadFromFile("graphics/background.png");

    // Create a Sprite
    Sprite spriteBackground;

    // Attach the texture to the sprite
    spriteBackground.setTexture(textureBackground);

    // Set the spriteBackground to cover the screen
    spriteBackground.setPosition(0, 0);

    // Make a tree Sprite
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);

    // Prepare the bee
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);
    
    // Is the bee currently moving?
    bool beeActive = false;

    // How fast can the bee fly?
    float beeSpeed = 0.0f;

    // Clouds
    Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");

    Sprite spriteCloud1;
    Sprite spriteCloud2;
    Sprite spriteCloud3;
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);

    spriteCloud1.setPosition(0, 0);
    spriteCloud2.setPosition(0, 250);
    spriteCloud3.setPosition(0, 500);

    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;

    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;

    // Variables to control time itself
    Clock clock;

    // Time bar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    // Track whether the games is running
    bool paused = true;
    
    // Draw some text
    int score = 0;
    sf::Text messageText;
    sf::Text scoreText;
    sf::Text frameRateText;

    // Choose a font
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    // Set font to the message
    messageText.setFont(font);
    scoreText.setFont(font);
    frameRateText.setFont(font);

    // Assign the actual message
    messageText.setString("Press Enter to start!");
    scoreText.setString("Score = 0");
    frameRateText.setString("FPS = ");

    // Make it really big
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);

    // Choose a color
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);

    // Position the text
    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

    scoreText.setPosition(20, 20);

    // Prepare 6 branches
    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");

    // Set the texture for each branch sprite
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);

        // Set the sprite's origin to dead center
        // We can then spin it arround without changing its position
        branches[i].setOrigin(220, 20);
    }

    // Prepare the player
    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);

    // The player starts on the left
    Side playerSide = Side::LEFT;

    // Prepare the gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);

    // Prepare the axe
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);

    // Line the axe up with the tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // Prepare the flying log
    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);

    // Some other useful log related variables
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    // Control the player input
    bool acceptInput = false;

    // Prepare the sound
    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("sound/chop.wav");
    Sound chop;
    chop.setBuffer(chopBuffer);

    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sound/death.wav");
    Sound death;
    death.setBuffer(deathBuffer);

    // Out of time
    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("sound/out_of_time.wav");
    Sound outOfTime;
    outOfTime.setBuffer(ootBuffer);

    while (window.isOpen())
    {
        /*
        **********************************
        * Handle the players input       *
        **********************************
        */

        Event event;

        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased && !paused)
            {
                // Listen for key press again
                acceptInput = true;

                // Hide the axe
                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Return))
        {
            paused = false;

            // Reset the time and the score
            score = 0;
            timeRemaining = 6;

            // Make all the branches disappear
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                branchPositions[i] = Side::NONE;
            }

            // Make sure the gravestone is hidden
            spriteRIP.setPosition(675, 2000);

            // Move the player into position
            spritePlayer.setPosition(580, 720);

            acceptInput = true;
        }

        // Wrap the player controls to make sure we are accepting input
        if (acceptInput)
        {
            // First handle pressing the right cursor key
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                // Make sure the player is on the right
                playerSide = Side::RIGHT;

                score++;

                // Update score text
                std::string s = "Score = " + std::to_string(score);
                scoreText.setString(s);

                // Add the amount of time remaining
                timeRemaining += (2 / score) + .15;

                spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);

                spritePlayer.setPosition(1200, 720);

                // Update the branches
                UpdateBranches(score);

                // Set the log flying to the left
                spriteLog.setPosition(810, 720);
                logSpeedX = -5000;
                logActive = true;

                acceptInput = false;

                // Play a chop sound
                chop.play();
            }

            // Handle the left cursor key
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                // Make sure the player is on the right
                playerSide = Side::LEFT;

                score++;

                // Update score text
                std::string s = "Score = " + std::to_string(score);
                scoreText.setString(s);

                // Add the amount of time remaining
                timeRemaining += (2 / score) + .15;

                spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);

                spritePlayer.setPosition(580, 720);

                // Update the branches
                UpdateBranches(score);

                // Set the log flying to the left
                spriteLog.setPosition(810, 720);
                logSpeedX = 5000;
                logActive = true;

                acceptInput = false;

                // Play a chop sound
                chop.play();
            }
        }

        /*
        **********************************
        * Update the scene               *
        **********************************
        */

        if (!paused)
        {
            // Measure time elapsed since last time we updated the scene and restarted the clock (DeltaTime)
            Time dt = clock.restart();

            // Substract from the amount of time remaining
            timeRemaining -= dt.asSeconds();

            // Size up the time bar
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

            if (timeRemaining <= 0.0f)
            {
                // Pause the game
                paused = true;

                // Change the message shown to the player
                messageText.setString("Out of time!!");

                // Reposition the text based on its new size
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                // Play the out of time sound
                outOfTime.play();
            }

            if (!beeActive)
            {
                // How fast is the bee
                srand((int)time(0)); // Seed random number generator using time(0)
                beeSpeed = (rand() % 200) + 200; // Random number between 200 and 399

                // How high is the bee
                srand((int)time(0));
                float height = (rand() % 500) + 500; // Random number between 500 and 999

                spriteBee.setPosition(2000, height);
                beeActive = true;
            }
            else
            {
                // Move the bee
                spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);

                // Has the bee reached the right hand edge of the screen?
                if (spriteBee.getPosition().x < -100)
                {
                    // Set it up to be a whole new bee next time
                    beeActive = false;
                }
            }

            if (!cloud1Active)
            {
                // How fast is the cloud
                srand((int)time(0) * 10); // Seed random number generator using time(0)
                cloud1Speed = (rand() % 200); // Random number between 0 and 199

                // How high is the cloud
                srand((int)time(0) * 10);
                float height = (rand() % 150); // Random number between 0 and 149

                spriteCloud1.setPosition(-200, height);
                cloud1Active = true;
            }
            else
            {
                // Move the bee
                spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);

                // Has the bee reached the right hand edge of the screen?
                if (spriteCloud1.getPosition().x > 1920)
                {
                    // Set it up to be a whole new bee next time
                    cloud1Active = false;
                }
            }

            if (!cloud2Active)
            {
                // How fast is the cloud
                srand((int)time(0) * 20); // Seed random number generator using time(0)
                cloud2Speed = (rand() % 200); // Random number between 0 and 199

                // How high is the cloud
                srand((int)time(0) * 20);
                float height = (rand() % 300) - 150; // Random number between -150 and 149

                spriteCloud2.setPosition(-200, height);
                cloud2Active = true;
            }
            else
            {
                // Move the bee
                spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);

                // Has the bee reached the right hand edge of the screen?
                if (spriteCloud2.getPosition().x > 1920)
                {
                    // Set it up to be a whole new bee next time
                    cloud2Active = false;
                }
            }

            if (!cloud3Active)
            {
                // How fast is the cloud
                srand((int)time(0) * 30); // Seed random number generator using time(0)
                cloud3Speed = (rand() % 200); // Random number between 0 and 199

                // How high is the cloud
                srand((int)time(0) * 30);
                float height = (rand() % 450) - 150; // Random number between -150 and 299

                spriteCloud3.setPosition(-200, height);
                cloud3Active = true;
            }
            else
            {
                // Move the bee
                spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);

                // Has the bee reached the right hand edge of the screen?
                if (spriteCloud3.getPosition().x > 1920)
                {
                    // Set it up to be a whole new bee next time
                    cloud3Active = false;
                }
            }

            // Update the branch sprites
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float height = i * 150;
                if (branchPositions[i] == Side::LEFT)
                {
                    // Move sprite to the left side
                    branches[i].setPosition(610, height);

                    // Flip the sprite round the other way
                    branches[i].setRotation(180);
                }
                else if (branchPositions[i] == Side::RIGHT)
                {
                    // Move the sprite to the right side
                    branches[i].setPosition(1330, height);

                    // Set the sprite rotation to normal
                    branches[i].setRotation(0);
                }
                else
                {
                    // Hide the branch
                    branches[i].setPosition(3000, height);
                }
            }

            // Handle flying log
            if (logActive)
            {
                spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));
                
                // Has the log reached the right hand edge?
                if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
                {
                    // Set it up already to be a whole new log next frame
                    logActive = false;
                    spriteLog.setPosition(810, 720);
                }
            }

            // Has the player been squished by a branch
            if (branchPositions[5] == playerSide)
            {
                // Death
                paused = true;
                acceptInput = false;

                // Draw the gravestone
                spriteRIP.setPosition(525, 760);

                // Hide the player
                spritePlayer.setPosition(2000, 660);

                // Change the text of the message
                messageText.setString("SQUISHED!");

                // Center it on the screen
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                // Play the death sound
                death.play();
            }
        }

        /*
        **********************************
        * Draw the scene                 *
        **********************************
        */

        // Clear everything from the last frame
        window.clear();

        // Draw our game scene here
        window.draw(spriteBackground);

        // Draw clouds
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);

        // Draw the branches
        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }

        // Draw tree
        window.draw(spriteTree);

        // Draw the player
        window.draw(spritePlayer);

        // Draw the axe
        window.draw(spriteAxe);

        // Draw the flying log
        window.draw(spriteLog);

        //Draw the gravestone
        window.draw(spriteRIP);

        // Draw bee
        window.draw(spriteBee);

        // Draw score
        window.draw(scoreText);

        // Draw timebar
        window.draw(timeBar);

        if (paused)
        {
            // Draw pause message
            window.draw(messageText);
        }

        // Show everything we just drew
        window.display();
    }

    return 0;
}

void UpdateBranches(int seed) 
{
    // Move all the branches down one place
    for (int i = NUM_BRANCHES - 1; i > 0; i--)
    {
        branchPositions[i] = branchPositions[i - 1];
    }

    // Spawn a new branch at position 0
    // LEFT, RIGHT or NONE
    srand((int)time(0) + seed);
    int r = (rand() % 5);
    switch (r)
    {
        case 0:
            branchPositions[0] = Side::LEFT;
            break;
        case 1:
            branchPositions[0] = Side::RIGHT;
            break;
        default:
            branchPositions[0] = Side::NONE;
            break;
    }
}