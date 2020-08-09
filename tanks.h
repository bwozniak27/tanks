//
//  tanks.h
//  first_SFML
//
//  Created by Ben Wozniak on 7/18/20.
//  Copyright © 2020 Ben Wozniak. All rights reserved.
//

#ifndef tanks_h
#define tanks_h
#include "amo.h"

// the angle the sprite should turn at
float turnAngle = 0.034 / 4.0;

// for comparing vectors
bool operator==(const sf::Vector2f& first, const sf::Vector2f& second);
bool operator!=(const sf::Vector2f& first, const sf::Vector2f& second);



class CPU {
public:
    
    //determines start direction
    sf::Vector2f make_direction() {
        srand(time(NULL));
        int number = rand() % 4;
        if (number == 0) {
            return sf::Vector2f(0.0, move_speed);
        } else if (number == 1) {
            return sf::Vector2f(0.0, -move_speed);
        } else if (number == 2) {
            return sf::Vector2f(move_speed, 0.0);
        } else {
            return sf::Vector2f(-move_speed, 0.0);
        }
    }
    
    CPU() {
        cpu.setSize(sf::Vector2f(30.0, 30.0));
        cpu.setFillColor(sf::Color::Yellow);
        cpu.setOrigin(15.0f, 15.0f);
        cpu.setPosition(500.0f, 100.0f);
        barrel.setSize(sf::Vector2f(8.0, 35.0));
        barrel.setFillColor(sf::Color::Green);
        barrel.setOrigin(5.0, 5.0);
        barrel.setPosition(cpu.getPosition());
        health = 100;
        direction = make_direction();
        still_hit = false;
        dodge = sf::Vector2f(0.0, 0.0);
        pause = 10;
        escape_mode = false;
    }
    
    // checks if sprite will hit a wall via a loop. Currently not being used to minimize
    // computation for loading speed
    int detect_walls() {
        sf::RectangleShape test(sf::Vector2f(35.0, 35.0));
        test.setOrigin(15.0, 15.0);
        test.setPosition(cpu.getPosition());
        //movement is only by 0.1 and walls are 5 thick, so 20 * 40 = 800 / 10 = 80 px
        for (int i = 20 ; i > 0; i--) {
            test.move(sf::Vector2f(direction.x * 40, direction.y * 40));
            if (level.collide(test.getGlobalBounds())) {
                return (i * 4);
            }
        }
        return -1;
    }
    
    // decides whether to turn right or left
    int which_turn(int proximity) {
        if (proximity == right) {
            if (direction.y <= 0) {
                return left;
            } else {
                return right;
            }
        } else if (proximity == left) {
            if (direction.y <= 0) {
                return right;
            } else {
                return left;
            }
        } else if (proximity == up) {
            if (direction.x <= 0) {
                return left;
            } else {
                return right;
            }
        } else if (proximity == down) {
            if (direction.y <= 0) {
                return right;
            } else {
                return left;
            }
        }
        
        return proximity;
    }
    
    // iteration 5 of creating random movement smooth movement while avoiding walls and
    // loading quickly. Doesn't work well
    
    // changes angle on direction vector
    // close tells which wall it is close to.
    // see 'private' for variable descriptions
    void change_direction(int close) {
        // first 4 are for corners
        if (close % up == 0 && close % left == 0) {
            if (!escape_mode) {
                pause = 0;
                escape_mode = true;
                direction = sf::Vector2f(0.70710678118 * 2.0, 0.70710678118 * 2.0);
            }
        } else if (close % up == 0 && close % right == 0) {
            if (!escape_mode) {
                pause = 0;
                escape_mode = true;
                direction = sf::Vector2f(-0.70710678118 * 2.0, 0.70710678118 * 2.0);
            }
        } else if (close % down == 0 && close % left == 0) {
            if (!escape_mode) {
                pause = 0;
                escape_mode = true;
                direction = sf::Vector2f(-0.70710678118 * 2.0, 0.70710678118 * 2.0);
            }
        } else if (close % down == 0 && close % right == 0) {
            if (!escape_mode) {
                pause = 0;
                escape_mode = true;
                direction = sf::Vector2f(-0.70710678118 * 2.0, 0.70710678118 * 2.0);
            }
        // for avoinding standard wall
        } else if (close >= 0) {
            escape_mode = false;
            float angle;
            float newAngle;
            float hypotenuse = sqrt((direction.x * direction.x) + (direction.y * direction.y));
            angle = asin(direction.y / hypotenuse);
            if (close == right) {
                newAngle = angle + turnAngle;
            } else {
                newAngle = angle - turnAngle;
            }
            direction.y = hypotenuse * sin(newAngle);
            direction.x = hypotenuse * cos(newAngle);
            float angle_in_degrees = newAngle * rad_to_degrees;
            if (angle_in_degrees > 0 && angle_in_degrees <= 90) {
                direction.x *= -1;
            } else if (angle_in_degrees > 90 && angle_in_degrees <= 180) {
                direction.x *= -1;
                direction.y *= -1;
            } else if (angle_in_degrees > 180 && angle_in_degrees <= 270) {
                direction.y *= -1;
            }
        } else {
            escape_mode = false;
        }
    }
    
    //determines movement
    void move() {
        pause++;
        if (pause > 10) {
            change_direction(which_turn(level.check_distance(cpu.getPosition())));
            cpu.move(direction);
            barrel.move(direction);
            
            
            //collision side detection
            if (level.collide(cpu.getGlobalBounds())) {
                
                sf::RectangleShape left(sf::Vector2f(1.0, 1.0));
                left.setPosition(cpu.getPosition().x - 16, cpu.getPosition().y);
                
                sf::RectangleShape right(sf::Vector2f(1.0, 1.0));
                right.setPosition(cpu.getPosition().x + 16, cpu.getPosition().y);
                
                sf::RectangleShape top(sf::Vector2f(1.0, 1.0));
                top.setPosition(cpu.getPosition().x, cpu.getPosition().y - 16);
                
                sf::RectangleShape bottom(sf::Vector2f(1.0, 1.0));
                bottom.setPosition(cpu.getPosition().x, cpu.getPosition().y + 16);
                
                if (level.collide(left.getGlobalBounds()) || level.collide(right.getGlobalBounds())) {
                    direction = sf::Vector2f(direction.x * -1, direction.y);
                } else if (level.collide(top.getGlobalBounds()) || level.collide(bottom.getGlobalBounds())) {
                    direction = sf::Vector2f(direction.x, direction.y * -1);
                }
                
                //direction = sf::Vector2f(-direction.x, -direction.y);
                cpu.move(direction);
                barrel.move(direction);
                cpu.move(direction);
                barrel.move(direction);
            }
        }
    }
    
    // rotates barrel to where the player is
    void rotate(sf::Vector2i location) {
        sf::Vector2f pointer = sf::Vector2f(location.x - barrel.getPosition().x, location.y - barrel.getPosition().y);
        float angle;
        float hypotenuse = sqrt((pointer.x * pointer.x) + (pointer.y * pointer.y));
        pointer = sf::Vector2f(pointer.x, pointer.y * -1);
        if (pointer.x > 0 && pointer.y > 0) {
            angle = asin(pointer.y / hypotenuse);
            angle *= (360.0 / 6.28);
            angle = 90.0 - angle;
            barrel.setRotation(angle + 180.0);
        } else if (pointer.x < 0 && pointer.y > 0) {
            angle = asin(pointer.y / hypotenuse);
            angle *= (360.0 / 6.28);
            barrel.setRotation(angle + 90.0);
        } else if (pointer.x < 0 && pointer.y < 0) {
            angle = asin(pointer.y / hypotenuse);
            angle *= (360.0 / 6.28);
            barrel.setRotation(angle + 90.0);
        } else if (pointer.x > 0 && pointer.y < 0) {
            angle = asin(pointer.y / hypotenuse);
            angle *= (360.0 / 6.28);
            angle = 90.0 - angle;
            barrel.setRotation(angle + 180.0);
        } else if (pointer.x == 0 && pointer.y < 0) {
            barrel.setRotation(0.0);
        } else if (pointer.x == 0 && pointer.y > 0) {
            barrel.setRotation(1800.0);
        } else if (pointer.x < 0 && pointer.y == 0) {
            barrel.setRotation(90.0);
        } else if (pointer.x > 0 && pointer.y == 0) {
            barrel.setRotation(270.0);
        }
        
    }
    
    // iteration 3 of avoiding bullet while limiting computation
    // works by checking if bullet is moving more horizontally or vertically, and moving
    // the perpendicular direction away from the nearest wall
    sf::Vector2f evasive_action(sf::Vector2f bullet) {
        int proximity;
        if (abs(bullet.x) >= abs(bullet.y)) {
            proximity = level.check_proximity(cpu.getPosition(), hor);
            if (proximity == up) {
                return sf::Vector2f(0.0, move_speed);
            } else if (proximity == down) {
                return sf::Vector2f(0.0, -move_speed);
            }
        } else {
            proximity = level.check_proximity(cpu.getPosition(), vert);
            if (proximity == left) {
                return sf::Vector2f(move_speed, 0.0);
            } else if (proximity == right) {
                return sf::Vector2f(-move_speed, 0.0);
            }
        }
    }
    
    // moves normally is not in danger, otherwise goes into dodge mode
    void reaction() {
        
        if (!level.collide(cpu.getGlobalBounds())) {
            cpu.move(dodge);
            barrel.move(dodge);
        }
        if (level.collide(cpu.getGlobalBounds())) {
            cpu.move(sf::Vector2f(dodge.x * -1, dodge.y * -1));
            barrel.move(sf::Vector2f(dodge.x * -1, dodge.y * -1));
        }
    }
    
    
    // where all the magic happens i.e handles all actions
    void action() {
        sf::Vector2f bullet = weapons.will_hit(cpu);
        if (bullet != sf::Vector2f(0.0, 0.0)) {
            if (!still_hit) {
                dodge = evasive_action(bullet);
                still_hit = true;
            }
            reaction();
            //cpu.move(bullet);
        } else {
            move();
            still_hit = false;
        }
    }
    
    
    int hit() {
        return weapons.collide(cpu.getGlobalBounds());
    }
    
    
    void damaged() {
        cpu.setFillColor(sf::Color::Red);
        health -= 10;
    }
    
    
    int getHealth() {
        return health;
    }
    
    
    void Unhit() {
        cpu.setFillColor(sf::Color::Yellow);
    }
    
    
    sf::FloatRect getBounds() {
        return cpu.getGlobalBounds();
    }
    
    
    void draw(sf::RenderWindow &window) {
        window.draw(cpu);
        window.draw(barrel);
    }
    
    
private:
    Ammunition weapons; // to access bullets
    sf::RectangleShape cpu; // sprite
    sf::RectangleShape barrel; // barrel
    sf::Vector2f direction; // for movement
    sf::Vector2f dodge; // for direction vector to dodge bullets
    bool still_hit; // tells whether bullet will hit it
    Borders level; // to access walls
    int health;
    int pause; // to delay motion
    bool escape_mode; // for the when sprite is close to the wall but is moving away
};












class Player {
public:
    
    Player() {
        player.setSize(sf::Vector2f(30.0f, 30.0f));
        player.setFillColor(sf::Color::Blue);
        player.setOrigin(15.0f, 15.0f);
        player.setPosition(100.0f, 100.0f);
        barrel.setSize(sf::Vector2f(8.0, 35.0));
        barrel.setFillColor(sf::Color::Green);
        barrel.setOrigin(5.0, 5.0);
        barrel.setPosition(player.getPosition());
        health = 100;
    }
    
    //  movement while not allowing intersection with walls or other sprites
    void move_player(Borders level, CPU opponent) {
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
            if (!player.getGlobalBounds().intersects(opponent.getBounds())
                && !level.collide(player.getGlobalBounds())) {
                player.move(move_speed, 0.0f);
                barrel.move(move_speed, 0.0f);
            }
            if (player.getGlobalBounds().intersects(opponent.getBounds())
                || level.collide(player.getGlobalBounds())) {
                player.move(-move_speed, 0.0f);
                barrel.move(-move_speed, 0.0f);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
            if (!player.getGlobalBounds().intersects(opponent.getBounds())
                && !level.collide(player.getGlobalBounds())) {
                player.move(-move_speed, 0.0f);
                barrel.move(-move_speed, 0.0f);
            }
            if (player.getGlobalBounds().intersects(opponent.getBounds())
                || level.collide(player.getGlobalBounds())) {
                player.move(move_speed, 0.0f);
                barrel.move(move_speed, 0.0f);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
            if (!player.getGlobalBounds().intersects(opponent.getBounds())
                && !level.collide(player.getGlobalBounds())) {
                player.move(0.0f, -move_speed);
                barrel.move(0.0f, -move_speed);
            }
            if (player.getGlobalBounds().intersects(opponent.getBounds())
                || level.collide(player.getGlobalBounds())) {
                player.move(0.0f, move_speed);
                barrel.move(0.0f, move_speed);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
            if (!player.getGlobalBounds().intersects(opponent.getBounds())
                && !level.collide(player.getGlobalBounds())) {
                player.move(0.0f, move_speed);
                barrel.move(0.0f, move_speed);
            }
            if (player.getGlobalBounds().intersects(opponent.getBounds())
                || level.collide(player.getGlobalBounds())) {
                player.move(0.0f, -move_speed);
                barrel.move(0.0f, -move_speed);
            }
        }
        
    }
    
    // rotates barrel to where the mouse is
    void rotate(sf::Vector2i location) {
        sf::Vector2f direction = sf::Vector2f(location.x - barrel.getPosition().x, location.y - barrel.getPosition().y);
        float angle;
        float hypotenuse = sqrt((direction.x * direction.x) + (direction.y * direction.y));
        direction = sf::Vector2f(direction.x, direction.y * -1);
        if (direction.x > 0 && direction.y > 0) {
            angle = asin(direction.y / hypotenuse);
            angle *= (360.0 / 6.28);
            angle = 90.0 - angle;
            barrel.setRotation(angle + 180.0);
        } else if (direction.x < 0 && direction.y > 0) {
            angle = asin(direction.y / hypotenuse);
            angle *= (360.0 / 6.28);
            barrel.setRotation(angle + 90.0);
        } else if (direction.x < 0 && direction.y < 0) {
            angle = asin(direction.y / hypotenuse);
            angle *= (360.0 / 6.28);
            //angle = 90.0 - angle;
            barrel.setRotation(angle + 90.0);
        } else if (direction.x > 0 && direction.y < 0) {
            angle = asin(direction.y / hypotenuse);
            angle *= (360.0 / 6.28);
            angle = 90.0 - angle;
            barrel.setRotation(angle + 180.0);
        } else if (direction.x == 0 && direction.y < 0) {
            barrel.setRotation(0.0);
        } else if (direction.x == 0 && direction.y > 0) {
            barrel.setRotation(1800.0);
        } else if (direction.x < 0 && direction.y == 0) {
            barrel.setRotation(90.0);
        } else if (direction.x > 0 && direction.y == 0) {
            barrel.setRotation(270.0);
        }
        
    }
    
    int hit() {
        return weapons.collide(player.getGlobalBounds());
    }
    
    void damaged() {
        player.setFillColor(sf::Color::Red);
        health -= 10;
    }
    
    int getHealth() {
        return health;
    }
    
    void Unhit() {
        player.setFillColor(sf::Color::Blue);
    }
    
    void update(int position) {
        weapons.update(position);
    }
    
    void fire(sf::Vector2i target) {
        weapons.fire(player.getPosition(), target);
    }
    
    void move_bullets(sf::FloatRect obsticle) {
        weapons.moveBullets(obsticle);
    }
    
    sf::FloatRect getBounds() {
        return player.getGlobalBounds();
    }
    
    sf::Vector2f getLocation() {
        return player.getPosition();
    }
    
    void draw(sf::RenderWindow &window) {
        window.draw(player);
        window.draw(barrel);
        weapons.draw(window);
    }
    
private:
    sf::RectangleShape player;
    sf::RectangleShape barrel;
    Ammunition weapons;
    int health;
};




bool operator==(const sf::Vector2f &first, const sf::Vector2f &second) {
    if (first.x == second.x && first.y == second.y) {
        return true;
    } else {
        return false;
    }
}

bool operator!=(const sf::Vector2f &first, const sf::Vector2f &second) {
    if (first == second) {
        return false;
    } else {
        return true;
    }
}

#endif /* tanks_h */
