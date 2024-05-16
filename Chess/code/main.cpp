#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace sf;
using namespace std;

void Draw(vector<vector<int>>& array,RenderWindow& ref,Sprite SBB,Sprite SBN,Sprite SBQ,Sprite SBK,Sprite SBP,Sprite SBR,Sprite SWP,Sprite SWK,Sprite SWN,Sprite SWQ,Sprite SWB,Sprite SWR,float X,float Y);
void SetPos(Sprite a,int x,int y,float X,float Y,RenderWindow& ref);
void Restart(vector<std::vector<int>>& array);
void Move(vector<vector<int>>& array,int x,int y,int a,int b);
vector<pair<int,int>> PossibleMoves(vector<vector<int>> array,int x,int y,RenderWindow &ref,CircleShape c);
void DrawCircle(int a,int b, RenderWindow& ref,CircleShape c);
int Castel(vector<vector<int>> a,bool k,bool r);
vector<bool>Check(vector<vector<int>> a,int turn,bool k,bool l,bool r);
void MoveK(int x,int y,int turn);

float sz=600,block=sz/8,rad=10;
int xw=7,yw=4,xb=0,yb=4;

enum GameState{
    Start,
    Playing,
    GameOver
};


int main(){
    RenderWindow window(VideoMode(sz, sz),"Chess");

    int x,y,newx,newy;

    bool wk=1,bk=1,wl=1,wr=1,bl=1,br=1;

    Texture BK,BQ,BN,BP,BR,BB,WB,WK,WN,WQ,WR,WP,Board;
    string path="./Assets/sprites/";
    BK.loadFromFile(path+"BK.png");
    BQ.loadFromFile(path+"BQ.png");
    BN.loadFromFile(path+"BN.png");
    BR.loadFromFile(path+"BR.png");
    BP.loadFromFile(path+"BP.png");
    BB.loadFromFile(path+"BB.png");
    WB.loadFromFile(path+"WB.png");
    WK.loadFromFile(path+"WK.png");
    WN.loadFromFile(path+"WN.png");
    WQ.loadFromFile(path+"WQ.png");
    WP.loadFromFile(path+"WP.png");
    WR.loadFromFile(path+"WR.png");
    Board.loadFromFile(path+"Board.png");
    
    Sprite SBK,SBN,SBQ,SBR,SBP,SBB,SWB,SWN,SWK,SWQ,SWR,SWP,SBoard;
    float X=static_cast<float>(window.getSize().x)/(9*BK.getSize().x);
    float Y=static_cast<float>(window.getSize().y)/(9*BK.getSize().y);
    SBK.setTexture(BK); SBK.setScale(X,Y);
    SBN.setTexture(BN); SBN.setScale(X,Y);
    SBQ.setTexture(BQ); SBQ.setScale(X,Y);
    SBR.setTexture(BR); SBR.setScale(X,Y);
    SBP.setTexture(BP); SBP.setScale(X,Y);
    SBB.setTexture(BB); SBB.setScale(X,Y);
    SWK.setTexture(WK); SWK.setScale(X,Y);
    SWQ.setTexture(WQ); SWQ.setScale(X,Y);
    SWR.setTexture(WR); SWR.setScale(X,Y);
    SWP.setTexture(WP); SWP.setScale(X,Y);
    SWB.setTexture(WB); SWB.setScale(X,Y);
    SWN.setTexture(WN); SWN.setScale(X,Y);
    SBoard.setTexture(Board);
    SBoard.setScale(static_cast<float>(window.getSize().x)/Board.getSize().x,
                    static_cast<float>(window.getSize().y)/Board.getSize().y);
    
    CircleShape circle;
    circle.setRadius(rad);
    
    
    GameState gamestate=GameState::Start;

    vector<vector<int>> A;
    Restart(A);

    vector<pair<int,int>> P;

    int turn=-1;//Whites turn
    bool rep=false;

    Clock clock;
    window.setFramerateLimit(60);

    while(window.isOpen()){

        float currentTime=clock.restart().asSeconds();
        float fps=1.0f/(currentTime);


        Vector2i pos = Mouse::getPosition(window);
        int gridX = (pos.x / block);
        int gridY = (pos.y / block);


        Event event;
        while (window.pollEvent(event))

        {
            if (event.type == Event :: Closed)
                window.close();
        }
        if(gamestate==GameState::Start){
            cout<<"Start\n";
            if (event.type == Event::KeyReleased){
                if(event.key.code ==Keyboard::Space){
                    cout<<"Plying\n";
                    gamestate=GameState::Playing;
                }
            }
        }

        window.clear();
        window.draw(SBoard);

        if(gamestate==GameState::Playing){
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    if(A[gridY][gridX]!=0&&A[gridY][gridX]*turn>0){
                        x=gridX;
                        y=gridY;
                        newx=x;
                        newy=y;
                        P=PossibleMoves(A,x,y,window,circle);
                    }
                    if(A[gridY][gridX]*turn<=0){
                        newx=gridX;
                        newy=gridY;
                        pair a=make_pair(newy,newx);
                        for(pair i :P){
                            if(a==i){
                                rep=true;
                                break;
                            }
                        }
                    }
                    if((x!=newx||y!=newy)&&rep){
                        if(abs(A[gridY][gridX])==6){
                            MoveK(x,y,turn);
                        }
                        Move(A,x,y,newx,newy);
                        P.clear();
                        rep=false;
                        turn*=-1;
                    }
                    if(A[xw][yw]!=-6||A[xb][yb]!=6){
                        gamestate=GameState::GameOver;
                    }
                }
                
            }
            
        }


        if(gamestate==GameState::GameOver){
            if (event.type == Event::KeyReleased){
                if(event.key.code ==Keyboard::Space){
                    gamestate=GameState::Start;
                    Restart(A);
                }
            }
            cout<<"GameOver\n";
        }
        

        Draw(A,window,SBB,SBN,SBQ,SBK,SBP,SBR,SWP,SWK,SWN,SWQ,SWB,SWR,X,Y);
        for(pair a:P){
            DrawCircle(a.first,a.second,window,circle);
        }
        window.display();
    }
    return 0;
}

void Draw(vector<vector<int>>& array,RenderWindow& ref,Sprite SBB,Sprite SBN,Sprite SBQ,Sprite SBK,Sprite SBP,Sprite SBR,Sprite SWP,Sprite SWK,Sprite SWN,Sprite SWQ,Sprite SWB,Sprite SWR,float X,float Y){
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            int a=array[i][j];
            if(a<0){
                if(a==-1)SetPos(SWP,i,j,X,Y,ref);
                else if(a==-2)SetPos(SWB,i,j,X,Y,ref);
                else if(a==-3)SetPos(SWN,i,j,X,Y,ref);
                else if(a==-4)SetPos(SWR,i,j,X,Y,ref);
                else if(a==-5)SetPos(SWQ,i,j,X,Y,ref);
                else if(a==-6)SetPos(SWK,i,j,X,Y,ref);
            }
            else if(a>0){
                if(a==1)SetPos(SBP,i,j,X,Y,ref);
                else if(a==2)SetPos(SBB,i,j,X,Y,ref);
                else if(a==3)SetPos(SBN,i,j,X,Y,ref);
                else if(a==4)SetPos(SBR,i,j,X,Y,ref);
                else if(a==5)SetPos(SBQ,i,j,X,Y,ref);
                else if(a==6)SetPos(SBK,i,j,X,Y,ref);
            }
        }
    }
}

void SetPos(Sprite a,int x,int y,float X,float Y,RenderWindow& ref){
    a.setPosition(y*block+X,x*block+Y);
    ref.draw(a);
}

void Restart(vector<vector<int>>& array) {
    array = {//P1,B2,N3,R4,Q5,K6
        {4, 3, 2, 5, 6, 2, 3, 4},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
       {-1,-1,-1,-1,-1,-1,-1,-1},
       {-4,-3,-2,-5,-6,-2,-3,-4}
    };
}

void Move(vector<vector<int>>& array,int x,int y,int a,int b){
    array[b][a]=array[y][x];
    array[y][x]=0;
}

vector<pair<int,int>> PossibleMoves(vector<vector<int>> array,int x,int y,RenderWindow &ref,CircleShape c){
    int i=x,j=y;
    vector<pair<int,int>> pos;
    int a=array[y][x];
    if(abs(a)==4){//Rook
        j++;while(j<8&&a*array[j][i]<=0){pos.push_back(make_pair(j,i));if(a*array[j][i]<0){break;} j++;}i=x,j=y;
        i++;while(i<8&&a*array[j][i]<=0){pos.push_back(make_pair(j,i));if(a*array[j][i]<0){break;} i++;}i=x,j=y;
        j--;while(j>-1&&a*array[j][i]<=0){pos.push_back(make_pair(j,i));if(a*array[j][i]<0){break;}j--;}i=x,j=y;
        i--;while(i>-1&&a*array[j][i]<=0){pos.push_back(make_pair(j,i));if(a*array[j][i]<0){break;}i--;}i=x,j=y;
    }
    else if(abs(a)==5){//Queen
        j++;while(j<8&&a*array[j][i]<=0){pos.push_back(make_pair(j,i));if(a*array[j][i]<0){break;} j++;}i=x,j=y;
        i++;while(i<8&&a*array[j][i]<=0){pos.push_back(make_pair(j,i));if(a*array[j][i]<0){break;} i++;}i=x,j=y;
        j--;while(j>-1&&a*array[j][i]<=0){pos.push_back(make_pair(j,i));if(a*array[j][i]<0){break;}j--;}i=x,j=y;
        i--;while(i>-1&&a*array[j][i]<=0){pos.push_back(make_pair(j,i));if(a*array[j][i]<0){break;}i--;}i=x,j=y;

        j++;i++;while(j<8&&i<8&&a*array[j][i]<=0){pos.push_back(make_pair(j,i));if(a*array[j][i]<0){break;} j++;i++;}i=x,j=y;
        i++;j--;while(i<8&&j>-1&&a*array[j][i]<=0){pos.push_back(make_pair(j,i));if(a*array[j][i]<0){break;} i++;j--;}i=x,j=y;
        j++;i--;while(j<8&&i>-1&&a*array[j][i]<=0){pos.push_back(make_pair(j,i));if(a*array[j][i]<0){break;}j++;i--;}i=x,j=y;
        i--;j--;while(i>-1&&j>-1&&a*array[j][i]<=0){pos.push_back(make_pair(j,i));if(a*array[j][i]<0){break;}i--;j--;}i=x,j=y;
    }
    else if(abs(a)==2){//Bishop
        j++;i++;while(j<8&&i<8&&a*array[j][i]<=0){pos.push_back(make_pair(j,i));if(a*array[j][i]<0){break;} j++;i++;}i=x,j=y;
        i++;j--;while(i<8&&j>-1&&a*array[j][i]<=0){pos.push_back(make_pair(j,i));if(a*array[j][i]<0){break;} i++;j--;}i=x,j=y;
        j++;i--;while(j<8&&i>-1&&a*array[j][i]<=0){pos.push_back(make_pair(j,i));if(a*array[j][i]<0){break;}j++;i--;}i=x,j=y;
        i--;j--;while(i>-1&&j>-1&&a*array[j][i]<=0){pos.push_back(make_pair(j,i));if(a*array[j][i]<0){break;}i--;j--;}i=x,j=y;
    }
    else if(abs(a)==3){//Knight
        int dx[] = {1, 1, -1, -1, 2, 2, -2, -2};
        int dy[] = {2, -2, 2, -2, 1, -1, 1, -1};
        for (int k = 0; k < 8; ++k) {
            int i = x + dx[k];j = y + dy[k];
            if (i >= 0 && i < 8 && j >= 0 && j < 8 && a * array[j][i] <= 0) {
                pos.push_back(make_pair(j, i));
                
            }
        }
    }
    else if(abs(a)==6){//King
        int dx[] = {1,1,1,-1,-1,-1,0,0,0};
        int dy[] = {1,-1,0,1,-1,0,1,-1,0};
        for (int k = 0; k < 8; ++k) {
            int i = x + dx[k];j = y + dy[k];
            if (i >= 0 && i < 8 && j >= 0 && j < 8 && a * array[j][i] <= 0) {
                pos.push_back(make_pair(j, i));
            }
        }
    }
    else {//Pawn
        int direction = (a == 1) ? 1 : -1;
        int i = x;
        int j = y + direction;
        if (j >= 0 && j < 8 && array[j][i] == 0) {
            pos.push_back(make_pair(j, i));
        }
        if ((a == 1 && y == 1) || (a == -1 && y == 6)) {
            j = y + 2 * direction;
            if (j >= 0 && j < 8 && array[j][i] == 0 && array[j - direction][i] == 0) {
                pos.push_back(make_pair(j, i));
            }
        }
        i = x - 1;
        if (i >= 0 && j >= 0 && array[j][i] * a < 0) {
            pos.push_back(make_pair(j, i));
        }
        i = x + 1;
        if (i < 8 && j >= 0 && array[j][i] * a < 0) {
            pos.push_back(make_pair(j, i));
        }
    }
    return pos;
}

void DrawCircle(int a,int b, RenderWindow& ref,CircleShape c){
    c.setPosition((b*block)+(block/2)-rad,(a*block)+(block/2)-rad);
    c.setFillColor(Color::Green);
    ref.draw(c);
}

int Castel(vector<vector<int>> a,bool k,bool r,bool l,int i){
    if(k){
        if(r){
            
        }
    }
    return 0;
}

vector<bool>Check(vector<vector<int>> a,int turn,bool k,bool r,bool l){
    vector<bool> out;
    if(k&&(r||l)){
        if(turn=-1){
            if(a[7][4]==-6) k=true;
            else k=false;
            if(r&&a[7][7]==-4) r=true;
            else r=false;
            if(l&&a[7][0]==-4) l=true;
            else l=false;
        }
        else{
            if(a[0][4]==6) k=true;
            else k=false;
            if(r&&a[0][7]==4) r=true;
            else r=false;
            if(l&&a[0][0]==4) l=true;
            else l=false;
        }
    }
    else{ k=0;l=0;r=0;}
    out.push_back(k);
    out.push_back(r);
    out.push_back(l);
    return out;
}

void MoveK(int x,int y,int turn){
    if(turn==-1){
        xw=y;
        yw=x;
    }
    else{
        xb=y;
        yb=x;
    }
}