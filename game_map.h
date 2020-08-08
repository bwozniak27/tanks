//
//  game_map.h
//  first_SFML
//
//  Created by Ben Wozniak on 5/11/20.
//  Copyright © 2020 Ben Wozniak. All rights reserved.
//

#ifndef game_map_h
#define game_map_h
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <vector>


float EPSILON = 0.0001;
float shot_speed = 0.2;
float move_speed = 0.1;
int vert = 1;
int hor = 2;
int both = 6;
int none = -1;
int left = 3;
int right = 4;
int up = 5;
int down = 7;
float rad_to_degrees = 360.0 / 6.28;
float fit_to_rad = (6.28 / 8.0) / 800;
float sprite_size = 30.0;


class Borders {
public:
    
    Borders() {
        
    }
    
    void set_borders() {
        sf::RectangleShape farLeft(sf::Vector2f(5.0f, 500.0f));
        farLeft.setFillColor(sf::Color::White);
        farLeft.setPosition(60.0f, 60.0f);
        walls.push_back(farLeft);
        vert_indexes.push_back(0);
        
        sf::RectangleShape bottom(sf::Vector2f(500.0f, 5.0f));
        bottom.setFillColor(sf::Color::White);
        bottom.setPosition(60.0f, 560.0f);
        walls.push_back(bottom);
        hor_indexes.push_back(1);
        
        sf::RectangleShape farRight(sf::Vector2f(5.0f, 505.0f));
        farRight.setFillColor(sf::Color::White);
        farRight.setPosition(560.0f, 60.0f);
        walls.push_back(farRight);
        vert_indexes.push_back(2);
        
        sf::RectangleShape top(sf::Vector2f(500.0f, 5.0f));
        top.setFillColor(sf::Color::White);
        top.setPosition(60.0f, 60.0f);
        walls.push_back(top);
        hor_indexes.push_back(3);
        
        sf::RectangleShape middleTop(sf::Vector2f(300.0f, 5.0f));
        middleTop.setFillColor(sf::Color::White);
        middleTop.setPosition(150.0f, 185.0f);
        walls.push_back(middleTop);
        hor_indexes.push_back(4);
        
        sf::RectangleShape middleBottom(sf::Vector2f(300.0f, 5.0f));
        middleBottom.setFillColor(sf::Color::White);
        middleBottom.setPosition(150.0f, 400.0f);
        walls.push_back(middleBottom);
        hor_indexes.push_back(5);
    }
    
    
    int getSize() {
        return walls.size();
    }
    
    
    // checks if sprite is close enough to warrant a turn, and returns which direction
    // the wall is, or -1 if it isnt close enough. Threshold is 30
    int check_distance(sf::Vector2f position) {
        float min_distance = 30;
        int proximity = none;  // this tells where the closest wall is, NOT where the player
        // should move
        for (auto i:vert_indexes) {
            if (position.y + (sprite_size / 2.0) >= walls[i].getPosition().y
                && position.y + (sprite_size / 2.0) <= walls[i].getPosition().y + walls[i].getSize().y) {
                float temp = position.x - walls[i].getPosition().x;
                if ((abs(temp) < min_distance)) {
                    //min_distance = abs(temp);
                    if (temp < 0) {
                        proximity *= right;
                    } else {
                        proximity *= left;
                    }
                }
            }
        }
        for (auto i:hor_indexes) {
            if (position.x + (sprite_size / 2.0) >= walls[i].getPosition().x
                && position.x + (sprite_size / 2.0) <= walls[i].getPosition().x + walls[i].getSize().x) {
                float temp = position.y - walls[i].getPosition().y;
                if ((abs(temp) < min_distance)) {
                    //min_distance = abs(temp);
                    if (temp < 0) {
                        proximity *= down;
                    } else {
                        proximity *= up;
                    }
                }
            }
        }
        return abs(proximity);
    }
    
    
    // checks direction of closest wall for dodging
    int check_proximity(sf::Vector2f position, int orientation) {
        float min_distance = 100000;
        int proximity;  // this tells where the closest wall is, NOT where the player
                        // should move
        if (orientation == vert) {
            for (auto i:vert_indexes) {
                if (position.y + (sprite_size / 2.0) >= walls[i].getPosition().y
                    && position.y + (sprite_size / 2.0) <= walls[i].getPosition().y + walls[i].getSize().y) {
                    float temp = position.x - walls[i].getPosition().x;
                    if ((abs(temp) < min_distance)) {
                        min_distance = abs(temp);
                        if (temp < 0) {
                            proximity = right;
                        } else {
                            proximity = left;
                        }
                    }
                }
            }
        } else if (orientation == hor) {
            for (auto i:hor_indexes) {
                if (position.x + (sprite_size / 2.0) >= walls[i].getPosition().x
                    && position.x + (sprite_size / 2.0) <= walls[i].getPosition().x + walls[i].getSize().x) {
                    float temp = position.y - walls[i].getPosition().y;
                    if ((abs(temp) < min_distance)) {
                        min_distance = abs(temp);
                        if (temp < 0) {
                            proximity = down;
                        } else {
                            proximity = up;
                        }
                    }
                }
            }
        }
        return proximity;
    }
    
    
    bool collide(sf::FloatRect main) {
        for (auto i:walls) {
            if (main.intersects(i.getGlobalBounds())) {
                return true;
            }
        }
        return false;
    }
    
    
    void draw(sf::RenderWindow &window) {
        for (auto i:walls) {
            window.draw(i);
        }
    }
    
private:
    static std::vector<sf::RectangleShape> walls;
    static std::vector<int> vert_indexes;
    static std::vector<int> hor_indexes;
};


std::vector<sf::RectangleShape> Borders::walls = {};
std::vector<int> Borders::vert_indexes = {};
std::vector<int> Borders::hor_indexes = {};






#endif /* game_map_h */
