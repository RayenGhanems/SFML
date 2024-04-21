#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace sf;
using namespace std;

enum class GameState {
    Start,
    Playing,
    GameOver
};

float G=1.5;

class Bird{
    public:
    Texture u,d,m;
    Sprite sBird;
    float x,y,dy;

    Bird(){
        u.loadFromFile("./assets/sprites/downflap.png");
        m.loadFromFile("./assets/sprites/midflap.png");
        d.loadFromFile("./assets/sprites/upflap.png");
        dy=0.0f;
        x=70.0f;
        y=200.0f;
    }
    ~Bird(){

    }
    void Render (RenderWindow& ref){
        PicDrawing();
        ref.draw(sBird);
    }
    void Flap(){
        dy=-40;
    }
    void Update(){
        dy+=G;
        y+=dy;
        sBird.setPosition(x,y);
    }
    Sprite PicDrawing(){
        if(dy<=50){
            sBird.setTexture(d);
            return sBird;
        }
        else if(dy<250){
            sBird.setTexture(m);
            return sBird;
        }
        sBird.setTexture(u);
        return sBird;
    }
    void Reset(){
        y=200.0f;
        dy=0.0f;
    }

};

int main(){
    bool enter=true;
    RenderWindow window(VideoMode(600, 800),"Flappy Bird");

    Font font;
    font.loadFromFile("./assets/fonts/FlappyBirdy.ttf");
    //Text text(to_string(score()),font,60);
    //text.setPosition(window.getSize().x / 2.0f,10.0f);
    
    srand(time(nullptr));

    Texture Back,Ground,Restart,Over;
    Back.loadFromFile("./assets/sprites/background.png");
    Ground.loadFromFile("./assets/sprites/base.png");
    Restart.loadFromFile("./assets/sprites/message.png");
    Over.loadFromFile("./assets/sprites/gameover.png");

    Sprite sBack,sGround,sRestart,sOver;
    sBack.setTexture(Back);
    sBack.setScale(static_cast<float>(window.getSize().x) / Back.getSize().x,static_cast<float>(window.getSize().y) / Back.getSize().y);

    sGround.setTexture(Ground);
    sGround.setScale(static_cast<float>(window.getSize().x) / Ground.getSize().x, 170.0f / Ground.getSize().y);
    sGround.setPosition(0, window.getSize().y - 170);
    
    sRestart.setTexture(Restart);
    sRestart.setScale(static_cast<float>(window.getSize().x) / Restart.getSize().x,static_cast<float>(window.getSize().y) / Restart.getSize().y );

    sOver.setTexture(Over);
    sOver.setScale(500.0f / Over.getSize().x, 200.0f / Over.getSize().y);
    sOver.setPosition(50, 300);


    Clock clock;
    window.setFramerateLimit(60);

    GameState gamestate=GameState::Start;
    Bird B;

    while (window.isOpen())
    {
        float currentTime=clock.restart().asSeconds();
        float fps=1.0f/(currentTime);
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event :: Closed)
                window.close();
        }
 
        window.clear();
        window.draw(sBack);
        window.draw(sGround);

        if(gamestate == GameState::Start){
            window.draw(sRestart);
            if(event.type == Event :: KeyReleased){
            gamestate =GameState::Playing;
            B.Reset();
            }
        }

        else if(gamestate==GameState::Playing){
            B.Render(window);
            if(event.type==Event::KeyReleased){
                B.Flap();
            }

            B.Update();
            if((B.y+B.dy)>630){
                gamestate=GameState::GameOver;
            }
        }

        if(gamestate == GameState::GameOver){
            window.draw(sOver);
            if(event.type == Event :: KeyReleased){
            gamestate =GameState::Start;
            }
        }
        
        
        window.display();
    }

    return 0;
}