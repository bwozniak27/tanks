
//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// In order to load the resources like cute_image.png, you have to set up
// your target scheme:
//
// - Select "Edit Scheme…" in the "Product" menu;
// - Check the box "use custom working directory";
// - Fill the text field with the folder path containing your resources;
//        (e.g. your project folder)
// - Click OK.
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "tanks.h"




int main(int argc, char const** argv)
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window",
                            sf::Style::Close | sf::Style::Titlebar);
    Player player;

    CPU player2;
    
    Borders level;
    level.set_borders();
    
    

    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile("sansation.ttf")) {
        return EXIT_FAILURE;
    }
    
    //text
    sf::Text name("Tanks!", font, 50);
    name.setFillColor(sf::Color::Green);
    sf::Text health("Player: " + std::to_string(player.getHealth()), font, 30);
    health.setPosition(200.0, 15.0);
    health.setFillColor(sf::Color::Magenta);
    sf::Text health2("CPU: " + std::to_string(player2.getHealth()), font, 30);
    health2.setPosition(400.0, 15.0);
    health2.setFillColor(sf::Color::Magenta);

    
    //helper variables
    bool mouseHeld = false;
    int hit = 0;
    int hit2 = 0;
    //int shot_timer = 500;
    int color_timer = 0;
    int color_timer2 = 0;
    
    
    // Start the game loop
    while (window.isOpen())
    {
        
        //not hit
        hit = -1;
        hit2 = -1;
        
        //barrel rotation
        player.rotate(sf::Mouse::getPosition(window));
        player2.rotate(static_cast<sf::Vector2i>(player.getLocation()));
        
        // Process events
        sf::Event evnt;
        while (window.pollEvent(evnt))
        {
            // Escape pressed: exit
            if (evnt.type == sf::Event::KeyPressed && evnt.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
        
        //shoot
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (!mouseHeld) {
                player.fire(sf::Mouse::getPosition(window));
                mouseHeld = true;
            }
        } else {
            mouseHeld = false;
        }
        
        //bullets continue to move
        player.move_bullets(player2.getBounds());
        
        //movement
        player.move_player(level, player2);
        player2.action();
        
        //hitting the player
        hit = player.hit();
        player.update(hit);
        health.setString("Player: " + std::to_string(player.getHealth()));
        if (hit >= 0) {
            player.damaged();
            color_timer = 0;
        } else {
            if (color_timer > 150) {
                player.Unhit();
            }
        }
        
        //hitting the cpu
        hit2 = player2.hit();
        player.update(hit2);
        health2.setString("CPU: " + std::to_string(player2.getHealth()));
        if (hit2 >= 0) {
            player2.damaged();
            color_timer2 = 0;
        } else {
            if (color_timer2 > 150) {
                player2.Unhit();
            }
        }
        
        
        
        
        //shot_timer++;
        color_timer++;
        color_timer2++;
        
        // Clear screen
        window.clear();

        // Draw the sprite
        level.draw(window);
        player.draw(window);
        player2.draw(window);
        
        // Draw the string
        window.draw(name);
        window.draw(health);
        window.draw(health2);

        // Update the window
        window.display();
    }
    
    return EXIT_SUCCESS;
}


