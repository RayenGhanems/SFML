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

float G=0.7;

class Pipe{
    public:
    float yu,yd ,speed;
    Texture upipe,dpipe;
    Sprite sU,sD;
    string name;
    float x=550.0f;
    Pipe(string ai){
        upipe.loadFromFile("./assets/sprites/upipe.png");
        dpipe.loadFromFile("./assets/sprites/dpipe.png");
        sU.setTexture(upipe);
        sD.setTexture(dpipe);
        yu=rand()%400 +175;
        yd =yu-upipe.getSize().y-150;
        name=ai;
        sD.setPosition(x,yd);
        sU.setPosition(x,yu);
        speed=-5.0f;
    }
    ~Pipe(){    }
    void Render (RenderWindow& ref){
        ref.draw(sD);
        ref.draw(sU);
    }
    
    void Update(){
        x+=speed;
        setPos(x);
    }
    
    void setPos(float k){
        upipe.loadFromFile("./assets/sprites/upipe.png");
        dpipe.loadFromFile("./assets/sprites/dpipe.png");
        sU.setTexture(upipe);
        sD.setTexture(dpipe);
        sD.setPosition(k,yd);
        sU.setPosition(k,yu);
        
    }

};

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
        dy=-14;
    }
    void Update(){
        dy+=G;
        y+=dy;
        sBird.setPosition(x,y);
    }
    Sprite PicDrawing(){
        if(dy>0){
            sBird.setTexture(d);
            return sBird;
        }
        else if(dy<-8){
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
    int c,a,score;
    score=0;
    RenderWindow window(VideoMode(600, 800),"Flappy Bird");

    Font font;
    font.loadFromFile("./assets/fonts/FlappyBirdy.ttf");
    Text text(to_string(score),font,60);
    text.setPosition(window.getSize().x / 2.0f,10.0f);
    
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

    GameState gamestate=GameState::Start;
    Bird B;
    Pipe p("test");

    vector<Pipe>pipes;

    Clock clock;
    window.setFramerateLimit(60);



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
        a++;
 
        window.clear();
        window.draw(sBack);

        if(gamestate == GameState::Start){
            window.draw(sRestart);
            if(event.type == Event :: KeyReleased){
            gamestate =GameState::Playing;
            B.Reset();
            c=0;
            }
        }

        else if(gamestate==GameState::Playing){
            B.Render(window);
            c++;
            if(event.type==Event::KeyReleased){
                B.Flap();
            }
            if(c%100==0){
                pipes.push_back("t"+to_string(a));
            }

            B.Update();
            for(Pipe& P:pipes){
                P.Update();
                P.Render(window);
                if (B.sBird.getGlobalBounds().intersects(P.sU.getGlobalBounds()) || 
                    B.sBird.getGlobalBounds().intersects(P.sD.getGlobalBounds())) {
                    gamestate = GameState::GameOver;
                }
                
            }
            if((B.y+B.dy)>630){
                gamestate=GameState::GameOver;
            }
        }

        if(gamestate == GameState::GameOver){
            window.draw(sOver);
            pipes.clear();
            if(event.type == Event :: KeyReleased){
            gamestate =GameState::Start;
            }
        }
        
        window.draw(sGround);
        window.draw(text);
        window.display();
    }

    return 0;
}
