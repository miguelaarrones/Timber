// Include important C++ libraries here.
#include "pch.h"
#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;
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

    // Choose a font
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    // Set font to the message
    messageText.setFont(font);
    scoreText.setFont(font);

    // Assign the actual message
    messageText.setString("Press Enter to start!");
    scoreText.setString("Score = 0");

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

    while (window.isOpen())
    {
        /*
        **********************************
        * Handle the players input       *
        **********************************
        */

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Enter))
        {
            paused = false;

            // Reset the time and the score
            score = 0;
            timeRemaining = 6;
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

            // Update score text
            std::string s = "Score = " + std::to_string(score);
            scoreText.setString(s);
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

        // Draw tree
        window.draw(spriteTree);

        // Draw bee
        window.draw(spriteBee);

        // Draw score
        window.draw(scoreText);

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