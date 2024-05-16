#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cstdlib>
#include <ctime>

using namespace sf;
using namespace std;

class IComponent{
    public:
    virtual ~IComponent(){  }
    virtual void Render(RenderWindow& ref){  }
    virtual void SetPosition(float x,float y){  }
};
class Sprite2DComponent:public IComponent{
    public:
    Sprite2DComponent(string filepath){
        m_texture.loadFromFile(filepath);
        m_sprite.setTexture(m_texture);
        m_sprite.setScale(0.1f,0.1f);
    }
    ~Sprite2DComponent(){

    }

    void SetPosition(float x,float y){
        m_sprite.setPosition(x,y);
    }

    void Render (RenderWindow& ref){
        ref.draw(m_sprite);
    }
    private:
    Texture m_texture;
    Sprite m_sprite;
    
    
};


class GameEntity{
    public:
    GameEntity(string name){
        m_name=name;
    }
    ~GameEntity(){

    }
    void SetPositoin(float x, float y){
        m_x=x;
        m_y=y;
        for(int i=0; i<m_components.size();i++){
            m_components[i]->SetPosition(m_x,m_y);
        }
    }
    
    float GetXPosition() const {
        return m_x;
    }
    float GetYPosition() const {
        return m_y;
    }

    void AddSpriteComponent(string filename){
        shared_ptr<Sprite2DComponent> sprite2dcomponent=make_shared<Sprite2DComponent>(filename);
        m_components.push_back(sprite2dcomponent);
    }

    void Render(RenderWindow& ref){
        for(int i=0; i<m_components.size();i++){
            m_components[i]->Render(ref);
        }
    }
    public:
    float xDirection=rand()%4-2;
    float yDirection=rand()%4-2;

    private:
    float m_x,m_y;
    string m_name;
    vector<shared_ptr<IComponent>> m_components;

};

int main(){
    RenderWindow window(VideoMode(500, 500),"Our new Game");

    Font font;
    font.loadFromFile("/home/rayen/Desktop/C/game/assests/fonts/Delistan.ttf");
    Text text("Mona lisa",font,60);
    text.move(200.0f,10.0f);
    
    srand(time(nullptr));
    vector<GameEntity> entities;
    for(int i=0; i<50;i++){
        GameEntity test("gost"+to_string(i));
        test.AddSpriteComponent("/home/rayen/Desktop/C/game/assests/images/ghost.png");
        test.SetPositoin(rand()%400,rand()%400);
        entities.push_back(test);
    }

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
 
        window.clear();
        for(int i=0; i<entities.size();i++){
            entities[i].Render(window);
            float x=entities[i].GetXPosition()+entities[i].xDirection;
            float y=entities[i].GetYPosition()+entities[i].yDirection;
            entities[i].SetPositoin(x,y);
            if(x>400){
                entities[i].xDirection*=-1;
            }
            if(x<0){
                entities[i].xDirection*=-1;
            }
            if(y>400){
                entities[i].yDirection*=-1;
            }
            if(y<0){
                entities[i].yDirection*=-1;
            }
        }
        window.display();
    }

    return 0;
}