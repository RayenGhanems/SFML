#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace sf;
using namespace std;

int nbr=13;
int windo=975;
int block=windo/nbr;//window.getSize().x/number of blocks
int I,J;// Head of the snake
pair food=make_pair(-1,-1);

enum class GameState {
    Start,
    Playing,
    GameOver
};

bool Eats(){
    if(make_pair(I,J)==food){
        return true;
    }
    return false;
}

void Draw(RenderWindow& ref,RectangleShape rect,int i,int j,int k){
    rect.setPosition(Vector2f((i*block)+(2.5*k),(j*block)+(2.5*k)));
    ref.draw(rect);
}

void Food(RenderWindow& ref,vector<pair<int, int>>S,RectangleShape rect){
    bool d=false;
    int i,j;
    do{
        i=rand()%nbr;
        j=rand()%nbr;
        food=make_pair(i,j);
        for(pair a :S){
            if(a==food){
                d=true;
                break;
            }
        }
    }while(d);
    Draw(ref,rect,i,j,0);
}

void DrawSnake(RenderWindow& ref,vector<pair<int, int>>S,RectangleShape rect){
    for(pair a:S){
        Draw(ref,rect,a.first,a.second,1);
    }
}

void Move(int d){
    if(d==1){
        I++;
    }
    else if(d==2){
        J--;
    }
    else if(d==3){
        I--;
    }
    else if(d==4){
        J++;
    }
}

bool Check(vector<pair<int, int>>S){
    pair b=make_pair(I,J);
    for(pair a :S){
        if(a==b)return true;
    }
    if(I<0||I==nbr||J<0||J==nbr){
        return true;
    }
    return false;
}

void DrawG(RenderWindow& ref,RectangleShape rect,RectangleShape frect){
    for(int i=0;i<nbr;i++){
        for(int j=0;j<nbr;j++){
            Draw(ref,rect,i,j,1);
        }
    }
    Draw(ref,frect,food.first,food.second,0);
}

int main(){
    RenderWindow window(sf::VideoMode(windo, windo), "Snake Game");

    srand(time(0));

    Font font;
    font.loadFromFile("font.ttf");
    Text rtext("Start", font);
    rtext.setCharacterSize(40);
    rtext.setStyle(sf::Text::Bold);
    rtext.setFillColor(sf::Color::Red);

    Text otext("Game Over", font);
    otext.setCharacterSize(40);
    otext.setStyle(sf::Text::Bold);
    otext.setFillColor(sf::Color::Red);
    
    int SCORE=0;
    Text score(to_string(SCORE), font);
    score.setCharacterSize(40);
    score.setStyle(sf::Text::Bold);
    score.setFillColor(sf::Color::Green);
    

    RectangleShape grect,frect,srect;
    grect.setSize(Vector2f(block-5, block-5));
    grect.setFillColor(Color::Black);
    grect.setOutlineThickness(2.5);
    grect.setOutlineColor(Color::White);

    srect.setSize(Vector2f(block-5, block-5));
    srect.setFillColor(Color::Blue);
    srect.setOutlineThickness(2.5);
    srect.setOutlineColor(Color(169, 169, 169));//Grey

    frect.setSize(Vector2f(block, block));
    frect.setFillColor(Color::Red);


    vector<pair<int, int>>first;
    vector<pair<int, int>>Snake;
    first.push_back(make_pair(1,nbr/2));
    first.push_back(make_pair(2,nbr/2 ));

    Snake=first;

    int direction=1;    //1--> 2up 3<-- 4down;

    I=2;//Head of the snake
    J=nbr/2 ;

    GameState gamestate=GameState::Start;

    
    Clock clock;
    window.setFramerateLimit(6);
    while (window.isOpen()){

        float currentTime=clock.restart().asSeconds();
        float fps=1.0f/(currentTime);

        Event event;
        
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        DrawG(window,grect,frect);
        if(gamestate==GameState::Start){
            window.draw(rtext);
            DrawSnake(window,Snake,srect);
            if (event.type == Event::KeyReleased){
                Food(window,Snake,frect);
                gamestate=GameState::Playing;
            }
        }

        else if(gamestate==GameState::Playing){
            if (event.type == Event::KeyReleased){
                if(event.key.code == Keyboard::Up){
                    direction =2;
                }
                else if(event.key.code == Keyboard::Down){
                    direction=4;
                }
                else if(event.key.code == Keyboard::Right){
                    direction=1;
                }
                else if(event.key.code == Keyboard::Left){
                    direction=3;
                }
            Move(direction);

            if(Check(Snake)){
                gamestate=GameState::GameOver;
            }

            if(Eats()){
                Snake.push_back(make_pair(I,J));
                SCORE++;
                score.setString(to_string(SCORE));
                Food(window,Snake,frect);
            }
            else{
                Snake.push_back(make_pair(I,J));
                Snake.erase(Snake.begin());   
            }
            
            DrawSnake(window,Snake,srect);
            }
            window.draw(score);
        }

        if(gamestate==GameState::GameOver){
            DrawSnake(window,Snake,srect);
            window.draw(otext);
            if (event.type == Event::KeyReleased){
                if(event.key.code ==Keyboard::Space){
                    gamestate=GameState::Start;
                    Snake=first;
                    I=2;
                    J=4;
                    direction=1;
                    SCORE=0;
                    score.setString(to_string(SCORE));
                }
                
            }
        }
        
        window.display();
    }
    return 0;
}