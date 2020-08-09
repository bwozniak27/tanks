//
//  amo.h
//  first_SFML
//
//  Created by Ben Wozniak on 7/18/20.
//  Copyright © 2020 Ben Wozniak. All rights reserved.
//

#ifndef amo_h
#define amo_h
#include "game_map.h"


float angle = 7;


// this is for individual bullets
class Bullets {
public:
    Bullets() {
        bullet.setRadius(3.0f);
        bullet.setOrigin(3.0f, 3.0f);
        bullet.setFillColor(sf::Color::Cyan);
        direction = sf::Vector2f(0, 0);
        bounced = false;
    }
    
    // sets the direction of the bullets to where the click was
    void fire(sf::Vector2f begin, sf::Vector2i target) {
        sf::Vector2f temp(target.x - begin.x, target.y - begin.y);
        float scale = sqrt((temp.x * temp.x) + (temp.y * temp.y));
        temp = sf::Vector2f(temp.x / (scale / shot_speed), temp.y / (scale / shot_speed));
        direction = temp;
        //150 is the distance to get it to start in the barrel and not in the sprite
        bullet.setPosition(begin.x + (direction.x * 150), begin.y + (direction.y * 150));
    }
    
    
    sf::Vector2f getDirection() {
        return direction;
    }
    
    // so the bullet continues to move
    void move() {
        bullet.move(direction);
    }
    
    
    bool getBounced() {
        return bounced;
    }
    
    // handles the bullet bouncing off a wall
    void bounce() {
        sf::RectangleShape left(sf::Vector2f(1.0, 1.0));
        left.setPosition(bullet.getPosition().x - 4, bullet.getPosition().y);
        
        sf::RectangleShape right(sf::Vector2f(1.0, 1.0));
        right.setPosition(bullet.getPosition().x + 4, bullet.getPosition().y);
        
        sf::RectangleShape top(sf::Vector2f(1.0, 1.0));
        top.setPosition(bullet.getPosition().x, bullet.getPosition().y - 4);
        
        sf::RectangleShape bottom(sf::Vector2f(1.0, 1.0));
        bottom.setPosition(bullet.getPosition().x, bullet.getPosition().y + 4);
        
        if (walls.collide(left.getGlobalBounds()) || walls.collide(right.getGlobalBounds())) {
            direction = sf::Vector2f(direction.x * -1, direction.y);
        } else if (walls.collide(top.getGlobalBounds()) || walls.collide(bottom.getGlobalBounds())) {
            direction = sf::Vector2f(direction.x, direction.y * -1);
        }
        bounced = true;
    }
    
    sf::FloatRect getBounds() {
        return bullet.getGlobalBounds();
    }
    
    // determines if bullet will hit the sprite by finding the distance from the bullet
    // to the 4 corners and center of the sprite, then checking all 5 distances along
    // the direction vector of the bullet. Mostly works, but seems to have issues with
    // the bottom left corner
    sf::Vector2f will_hit(sf::RectangleShape target) {
        sf::Vector2f size = target.getSize();
        // center point
        sf::Vector2f goal = target.getPosition();
        sf::Vector2f position = bullet.getPosition();
        sf::Vector2f tradectory = sf::Vector2f(goal.x - position.x, goal.y - position.y);
        float distance = sqrt((tradectory.x * tradectory.x) + (tradectory.y * tradectory.y));
        float scale = distance / sqrt((direction.x * direction.x) + (direction.y * direction.y));
        sf::Vector2f end = sf::Vector2f(position.x + (direction.x * scale), position.y + (direction.y * scale));
        sf::CircleShape endShape;
        endShape.setRadius(10.0f);
        endShape.setOrigin(3.0, 3.0);
        endShape.setPosition(end);
        if (target.getGlobalBounds().intersects(endShape.getGlobalBounds())) {
            return direction;
        }
        
        // top left corner
        goal = sf::Vector2f(goal.x - (size.x / 2.0), goal.y - (size.y / 2.0));
        position = bullet.getPosition();
        tradectory = sf::Vector2f(goal.x - position.x, goal.y - position.y);
        distance = sqrt((tradectory.x * tradectory.x) + (tradectory.y * tradectory.y));
        scale = distance / sqrt((direction.x * direction.x) + (direction.y * direction.y));
        end = sf::Vector2f(position.x + (direction.x * scale), position.y + (direction.y * scale));
        endShape.setPosition(end);
        if (target.getGlobalBounds().intersects(endShape.getGlobalBounds())) {
            return direction;
        }
        
        // top right corner
        goal = sf::Vector2f(goal.x + size.x, goal.y);
        position = bullet.getPosition();
        tradectory = sf::Vector2f(goal.x - position.x, goal.y - position.y);
        distance = sqrt((tradectory.x * tradectory.x) + (tradectory.y * tradectory.y));
        scale = distance / sqrt((direction.x * direction.x) + (direction.y * direction.y));
        end = sf::Vector2f(position.x + (direction.x * scale), position.y + (direction.y * scale));
        endShape.setPosition(end);
        if (target.getGlobalBounds().intersects(endShape.getGlobalBounds())) {
            return direction;
        }
        
        // bottom right corner
        goal = sf::Vector2f(goal.x, goal.y + size.y);
        position = bullet.getPosition();
        tradectory = sf::Vector2f(goal.x - position.x, goal.y - position.y);
        distance = sqrt((tradectory.x * tradectory.x) + (tradectory.y * tradectory.y));
        scale = distance / sqrt((direction.x * direction.x) + (direction.y * direction.y));
        end = sf::Vector2f(position.x + (direction.x * scale), position.y + (direction.y * scale));
        endShape.setPosition(end);
        if (target.getGlobalBounds().intersects(endShape.getGlobalBounds())) {
            return direction;
        }
        
        // bottom left corner
        goal = sf::Vector2f(goal.x - size.x, goal.y);
        position = bullet.getPosition();
        tradectory = sf::Vector2f(goal.x - position.x, goal.y - position.y);
        distance = sqrt((tradectory.x * tradectory.x) + (tradectory.y * tradectory.y));
        scale = distance / sqrt((direction.x * direction.x) + (direction.y * direction.y));
        end = sf::Vector2f(position.x + (direction.x * scale), position.y + (direction.y * scale));
        endShape.setPosition(end);
        if (target.getGlobalBounds().intersects(endShape.getGlobalBounds())) {
            return direction;
        }
        
        //returns <0, 0> if none intersect
        return sf::Vector2f(0.0, 0.0);
    }
    
    void draw(sf::RenderWindow &window) {
        window.draw(bullet);
    }
    
private:
    Borders walls;
    sf::CircleShape bullet;
    sf::Vector2f direction;
    bool bounced;
};













// this holds all the individual bullets
class Ammunition {
public:
    Ammunition() {
        
    }
    
    // creates and fires a bullet
    void fire(sf::Vector2f begin, sf::Vector2i target) {
        if (amo.size() <= 4) {
            Bullets shot;
            shot.fire(begin, target);
            amo.push_back(shot);
        }
    }
    
    // moves all bullets on screen
    void moveBullets(sf::FloatRect obsticle) {
        for (int i = 0; i < amo.size(); i++) {
            if (!amo[i].getBounds().intersects(obsticle)) {
                if (!walls.collide(amo[i].getBounds())) {
                    amo[i].move();
                } else {
                    if (!amo[i].getBounced()) {
                        amo[i].bounce();
                        amo[i].move();
                    } else {
                        update(i);
                    }
                }
            }
        }
    }
    
    // determines which, if any, bullets will hit the sprite. -1 otherwise
    sf::Vector2f will_hit(sf::RectangleShape target) {
        for (int i = 0; i < amo.size(); i++) {
            if (amo[i].will_hit(target) != sf::Vector2f(0.0, 0.0)) {
                return amo[i].getDirection();
            }
        }
        return sf::Vector2f(0.0, 0.0);
    }
    
    // draws bullets
    void draw(sf::RenderWindow &window) {
        for (auto i:amo) {
            i.draw(window);
        }
    }
    
    //determines if any bullets have hit any walls
    int collide(sf::FloatRect main) {
        for (int i = 0; i < amo.size(); i++) {
            if (main.intersects(amo[i].getBounds())) {
                return i;
            }
        }
        return -1;
    }
    
    // gets rid of a bullet if it has hit a sprite or hit a wall after its one bounce
    void update(int position) {
        if (position >= 0) {
            amo.erase(amo.begin() + position);
        }
    }
    
private:
    // static so all objects have the same bullets
    static std::vector<Bullets> amo;
    Borders walls;
};

std::vector<Bullets> Ammunition::amo = {};


#endif /* amo_h */
