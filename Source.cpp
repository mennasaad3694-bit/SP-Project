#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include<vector>
  
//version one

using namespace std;
using namespace sf;

void mainMenu();
void leveltwo ();
void updateleveltwo(float dt);
void drawLevelTwo(RenderWindow &window);
void game();
void resetgame();
void loselife();


void updateBoss(float dt);
void drawboss(RenderWindow& window);


// functions
void start();
void update();
void draw();



SoundBuffer click, chicken, back, over, shootbuf, gain, win;
Texture egg, bigg;  //bigg for boss bird
Sound setclick, setchicken, setback, setover, shootsound, setgain, setwin;
Sprite setegg, bigone;
//bigone for boss bird

Font font;
Text GameText, birdsScore, lifeText, levelText, Wintext, healthScore, dodoname;
RenderWindow window(VideoMode(1000, 800), "Chicken Invaders");
bool isgameover = false;
bool isopen = true;
bool ismainmenu = true;
bool isWin = false;
int counter_shoot = 0;
int health = 30;
int counter_shoot2 = 0;
float bossSpeed = 80.f;
bool winsound = false;
bool oversound = false;
bool menuopen = true;
bool level2 = false;
bool bosslevel = false;
bool reset = false;

//pause menu 
const int num = 2;
Text main_menu[2];
string main_menu_string[2] = { "Start Game", "Exit" };
int counter = 0;

Texture shoottexture, bigshoot;
Sprite shootsprite, setbig; //set bg for new shoot

vector<Sprite> shoots; 
vector<Sprite>prizes;
vector<Sprite>eggs;
vector<Sprite>bigs;
vector<Sprite>newshoots;

int level = 1;
bool isboss = false;
bool iswinner = false;
float newbirdcounter = 26;

const int num_of_text = 3;
Text pause_menu[num_of_text];
string pause_menu_string[num_of_text] = { "Resume", "Restart", "Main menu" };
int counter2 = 0;

float eggcounter = 0;


Texture ship_texture, bird_texture, background_texture, prize_texture, new_bird;
Sprite ship, background, prize, newBird;

bool ispaused = false;
Clock clock1;
float    DT = 0, spawncounter = 0, prizecounter = 0;
int current_bird = 0, score = 0, lives = 5;
RectangleShape ground;

struct bird {
    Sprite sprite;
    float gravity;
    bool isactive = false;

    void spawn() {
        sprite.setPosition(rand() % 700 + 50, -50);
        gravity = rand() % 200 + 100; 
        isactive = true;
    }
    void falldown(float dt) {
        if (isactive) sprite.move(0, gravity * dt);
    }
    void initialize(Texture& t) {
        sprite.setTexture(t);
        sprite.setScale(0.2f, 0.2f);
        isactive = false;
    }
    void reset() {
        isactive = false;
        sprite.setPosition(2000, 2000);
    }


};

bird Bird[20]; //1D array

struct newbird {
    float speedx = 50;
    Sprite sprite;
    bool isactive = false;
    
    void initialize(Texture& t) {
        sprite.setTexture(t);
        sprite.setScale(.2f, .2f);
        isactive = false;

    }

    void animate(float dt, float startx , float endx) {
        if (isactive)
            sprite.move(speedx * dt, 0);

        if (sprite.getPosition().x<10|| sprite.getPosition().x > 50+900)
            speedx = -speedx;
    }


    void spawn(float x, float y) {
        sprite.setPosition(x, y);
        isactive = true;


    }
    void reset (){
        isactive = false;
        sprite.setPosition(2000, 2000);
    }
         
};

int rows = 3;
int cols = 20;

newbird birdlevel2[3][20];
  
// gap x,y to set space  
float gapx=100, gapy = 80, startx = 100, starty = 100;
 
int main() {
    window.setFramerateLimit(60);
    start();
    while (window.isOpen()) {
        mainMenu();

        resetgame();
        
        game();

   
    }
         return 0;
}


void resetgame() {
    health = 30;
    lives = 5;
    score = 0;
    level = 1;

    counter_shoot = 0;
    counter_shoot2 = 0;
    newbirdcounter = 30;
    isgameover = false;
    isWin = false;
    ispaused = false;
   current_bird = 0;
   
   oversound = false;
   winsound = false;

    shoots.clear();
    newshoots.clear();
    eggs.clear();
    prizes.clear();

    ship.setPosition(500, 700);

    for (int i = 0; i < 20; i++) Bird[i].reset();

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            birdlevel2[r][c].reset();
        }
    }


    bigone.setPosition(500, 200);
    bossSpeed = 80.f;

    birdsScore.setString("Score: 0");
    lifeText.setString("Lives: 5");
    levelText.setString("Level 1");
	healthScore.setString("Health: 15");



}

void mainMenu() {
    


    click.loadFromFile("click.ogg");
    setclick.setBuffer(click);

    Font menuFont;
    menuFont.loadFromFile("font.TTF (1)");
	Texture menuBackgroundTexture;
	menuBackgroundTexture.loadFromFile("1763568192554.jpg");
	Sprite menuBackground;
	menuBackground.setTexture(menuBackgroundTexture);
    menuBackground.setScale(
        1000.f / menuBackgroundTexture.getSize().x,
        800.f / menuBackgroundTexture.getSize().y
    );

    const int items = 3;
    Text menu[items];
    string menuStr[items] = { "Play", "Settings", "Exit" };
    int selected = 0;

    for (int i = 0; i < items; i++) {
        menu[i].setFont(menuFont);
        menu[i].setString(menuStr[i]);
        menu[i].setCharacterSize(40);
        menu[i].setPosition(200 , 350 + i * 70);
        menu[i].setFillColor(Color::Black);
		menu[i].setOutlineColor(Color::White);
		menu[i].setOutlineThickness(2);
		menu[i].setStyle(Text::Italic);
    }
    menu[0].setFillColor(Color::Red);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Up) {
					setclick.play();                       
                    menu[selected].setFillColor(Color::Black);
                    selected = (selected - 1 + items) % items;
                    menu[selected].setFillColor(Color::Red);
                }

                if (event.key.code == Keyboard::Down) {
                    setclick.play();
                    menu[selected].setFillColor(Color::Black);
                    selected = (selected + 1) % items;
                    menu[selected].setFillColor(Color::Red);
                }

                if (event.key.code == Keyboard::Enter) {
                    setclick.play();
                    if (selected == 0) { 
                        resetgame();

                        game();
                        menuopen = false;
                        return;
                    }
                    if (selected == 2) {   
                        window.close();
                        menuopen = false;
                    }
                }
            }
        }

       window.clear();
        window.draw(menuBackground);
        for (int i = 0; i < items; i++)
            window.draw(menu[i]);
        window.display();
    }
}




void game() {
    start();
    //window.setFramerateLimit(60); // same speed for all computers 
	
    while (window.isOpen()) {
        isopen = true;
        DT = clock1.restart().asSeconds();  //to change dt

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                isopen = false;
            }

            if (event.type == Event::KeyPressed) {




                if (event.key.code == Keyboard::P) {
                    ispaused = !ispaused;
                    setback.pause();
                    if (!ispaused) setback.play();
                }

                if (ispaused) {
                    if (event.key.code == Keyboard::Up) {
                        setclick.play();
                        pause_menu[counter2].setFillColor(Color::White);
                        counter2 = (counter2 - 1 + num_of_text) % num_of_text;
                        pause_menu[counter2].setFillColor(Color::Red);
                    }
                    if (event.key.code == Keyboard::Down) {
                        setclick.play();
                        pause_menu[counter2].setFillColor(Color::White);
                        counter2 = (counter2 + 1) % num_of_text;
                        pause_menu[counter2].setFillColor(Color::Red);
                    }
                    if (event.key.code == Keyboard::Enter) {
                        setclick.play();
				
                        if (counter2 == 0) {
                            ispaused = false;
                            setback.play();

                            

                        }

                        // restart game at any level
                        if (counter2 == 1) {
							reset = true;
                            // level one
                            lives = 5; score = 0; ispaused = false;
                            for (int i = 0; i < 20; i++) Bird[i].reset();

                            //level 2
                            if (newbirdcounter > 0) {
                                for (int r = 0; r < rows; r++) {
                                    for (int c = 0; c < cols; c++) {
                                        birdlevel2[r][c].reset();
                                        leveltwo();
                                    }
                                }
                            }

                            //the last level
                            bigone.move(0, bossSpeed * DT);
							health = 15;
                            if (bigone.getPosition().y > 280) bossSpeed = -abs(bossSpeed);
                            else if (bigone.getPosition().y < 120) bossSpeed = abs(bossSpeed);

                            setback.play();
                        }
                        if (counter2 == 2) { // Main Menu
                            resetgame();
                            mainMenu(); 
                            // open maim menu agine
                            return;
                        }
                    }
                }
            }
        }
        // to prevent any debug 
        if(!ispaused&& !isgameover && !isWin && level==1)             update();

        if (!ispaused && !isgameover && !isWin && level == 2)          updateleveltwo(DT);


        if (!ispaused && !isgameover && !isWin && newbirdcounter <= 0)    updateBoss(DT);
        
        window.clear();
        if (level == 1) {
           // update();
            draw();
        }
        else if (level == 2) {
          //  updateleveltwo(DT);
            drawLevelTwo(window);
           

        }

        if (newbirdcounter <= 0) {

           // updateBoss(DT);
            drawboss(window);
        }
        window.display();
    }
   
    



}
// download all your needs here no actions 
void start() {
    srand(time(NULL));  //to set every part random
    font.loadFromFile("font.TTF (1)");
	levelText.setFont(font);
	levelText.setString("Level 1");
	levelText.setPosition(850, 10);
	levelText.setCharacterSize(30);

    Wintext.setFont(font);
    Wintext.setString("YOU WIN");
    Wintext.setPosition(600, 400);
    Wintext.setCharacterSize(100);
    Wintext.setFillColor(Color::Cyan);
    Wintext.setOutlineThickness(20);



    ;
    healthScore.setFont(font);
   // healthScore.setString("Level 1");
   // healthScore.setPosition(20, 100);
    healthScore.setCharacterSize(30);




    win.loadFromFile("you-win-sequence-2-183949 (1).wav");
    setwin.setBuffer(win);

    click.loadFromFile("click.ogg");
    setclick.setBuffer(click);
    back.loadFromFile("allsound.wav");
	setback.setBuffer(back);
    over.loadFromFile("over.wav");
    setover.setBuffer(over);
      
    egg.loadFromFile("egg33.png");
    setegg.setTexture(egg);
    setegg.setOrigin(setegg.getGlobalBounds().width / 2, setegg.getGlobalBounds().height / 2);
    setegg.setScale(.05f, .05f);

    new_bird.loadFromFile("bird2.png");
    newBird.setTexture(new_bird);
    newBird.setScale(.4f, .4f);

	shootbuf.loadFromFile("shooting.wav");
	shootsound.setBuffer(shootbuf);
    
	chicken.loadFromFile("chicken.ogg");
	setchicken.setBuffer(chicken);
    
	gain.loadFromFile("gain.wav");
    setgain.setBuffer(gain);

    bigg.loadFromFile("kindpng_6586073.png");
    bigone.setTexture(bigg);
    bigone.setScale(.5f, .5f);
   // bigone.setPosition(300, 150);
    bigone.setPosition(500, -50);

	setback.setLoop(true);
	setback.play();

	prize_texture.loadFromFile("prize.png");
    prize.setTexture(prize_texture);
    prize.setOrigin(prize.getGlobalBounds().width / 2, prize.getGlobalBounds().height / 2);
	prize.setScale(0.03f, 0.03f);

    bigshoot.loadFromFile("PngItem_1625645.png");
    setbig.setTexture(bigshoot);
    setbig.setOrigin(setbig.getGlobalBounds().width / 2, setbig.getGlobalBounds().height / 2);
    setbig.setScale(0.2, 0.2);


    background_texture.loadFromFile("back ground.jpg");
    background.setTexture(background_texture);
    background.setScale(
        1000.f / background_texture.getSize().x,
        800.f / background_texture.getSize().y
    );
    
	shoottexture.loadFromFile("shooting.png");
	shootsprite.setTexture(shoottexture);
    shootsprite.setScale(0.5f, .5f);

	shootsprite.setOrigin(shootsprite.getGlobalBounds().width / 2, shootsprite.getGlobalBounds().height / 2);


     
    GameText.setFont(font);
    GameText.setString("GAME OVER");
    GameText.setPosition(400, 300);
    GameText.setCharacterSize(50);
    GameText.setFillColor(Color::Red);

    birdsScore.setFont(font);
    birdsScore.setString("SCORE: ");
    birdsScore.setPosition(20, 30);
    birdsScore.setCharacterSize(30);

    lifeText.setFont(font);
    lifeText.setString("LIVES:  ");
    lifeText.setPosition(20, 70);
    lifeText.setCharacterSize(30);

    ship_texture.loadFromFile("ship1 (1).png");
    bird_texture.loadFromFile("pngegg.png");
    background_texture.loadFromFile("back ground.jpg");
    ship.setTexture(ship_texture);
    ship.setPosition(500, 700);
    ship.setScale(0.3, 0.3);

    for (int i = 0; i < num; i++) {
        main_menu[i].setFont(font);
        main_menu[i].setString(main_menu_string[i]);
        main_menu[i].setPosition(500, 250 + (i * 60));
        main_menu[i].setFillColor(Color::White);
    }




    for (int i = 0; i < num_of_text; i++) {
        pause_menu[i].setFont(font);
        pause_menu[i].setString(pause_menu_string[i]);
        pause_menu[i].setPosition(500, 250 + (i * 60));
        pause_menu[i].setFillColor(Color::White);
    }
    pause_menu[0].setFillColor(Color::Red);

    for (int i = 0; i < 20; i++) Bird[i].initialize(bird_texture);   //to start falling

    ground.setSize(Vector2f(800, 10));
    ground.setPosition(0, 580);
}

void update() {
    if (!ispaused && lives > 0) {
        spawncounter += DT;

       
        if (Keyboard::isKeyPressed(Keyboard::Left)) ship.move(-300 * DT, 0);
        if (Keyboard::isKeyPressed(Keyboard::Right)) ship.move(300 * DT, 0);


        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            counter_shoot++;
            if (counter_shoot >= 10) {
                shoots.push_back(shootsprite);
                shoots.back().setPosition(ship.getPosition().x, ship.getPosition().y);
                counter_shoot = 0;
            }
        }
		
        for (int i = 0; i < shoots.size(); i++) {
            shoots[i].move(0, -500 * DT);
            if (shoots[i].getPosition().y < 0) {
                shoots.erase(shoots.begin() + i);
                i--;
            }

        }
        eggcounter = +DT;
		prizecounter += DT;  
        if(prizecounter>=3.f){
            prize.setPosition(rand() % 900 + 50, -50); // to put it in a random position
            prizes.push_back(prize);  // to put it in a vector
            prizecounter = 0;
		}
        for(int i =0 ; i<prizes.size(); i++){
            prizes[i].move(0, 150 * DT);
            if(prizes[i].getGlobalBounds().intersects(ship.getGlobalBounds())){
                prizes.erase(prizes.begin() + i);      
                i--;
                score += 5;
                setgain.play();
            }
            else if(prizes[i].getPosition().y > 800){  //we have to put i-- after every erase.
                prizes.erase(prizes.begin() + i);
                i--;
                
            }
		}
        
        
        if (spawncounter >= 0.7f){
            Bird[current_bird].spawn();
            current_bird = (current_bird + 1) % 20;
            spawncounter = 0;
        }

        
        for (int i = 0; i < 20; i++) {
            if (Bird[i].isactive) {
                Bird[i].falldown(DT);


                if (Bird[i].sprite.getGlobalBounds().intersects(ship.getGlobalBounds())) {
                    Bird[i].reset();
                    loselife();
                }

                if (Bird[i].sprite.getPosition().y > 600) Bird[i].reset();
            }
        }
        if (ship.getPosition().x < 0) ship.setPosition(0, ship.getPosition().y);

        if (ship.getPosition().x > 900) ship.setPosition(899, ship.getPosition().y);
    }
    birdsScore.setString("Score: " + to_string(score));
    lifeText.setString("Lives: " + to_string(lives));

    if (lives <= 0 && !oversound) {
        lives = 0;
        isgameover = true;
        oversound = true;
        setback.stop();
        setover.play();
        for (int i = 0; i < num_of_text; i++) pause_menu[i].setFillColor(Color::White);
        counter2 = 0;
        return;
    }
    /* (Keyboard::isKeyPressed(Keyboard::Enter) && isgameover) {
        isgameover = false; lives = 5; score = 0;
        for (int i = 0; i < 20; i++) Bird[i].reset();
        setback.play();
     
    }
    */

    if (score >= 200){
        level = 2;
        score = 0;
        levelText.setString("level 2");

        leveltwo();
    }
}


void draw() {
    //window.clear();
    window.draw(background);

    window.draw(ship);
    for (int i = 0; i < 20; i++) {
        if (Bird[i].isactive) window.draw(Bird[i].sprite);
    }
    window.draw(birdsScore);
    window.draw(lifeText);


   
        for (int i = 0; i < shoots.size(); i++) {
            window.draw(shoots[i]);
            shootsound.play();
            for (int j = 0; j < 20; j++) {
                if (Bird[j].isactive && shoots[i].getGlobalBounds().intersects(Bird[j].sprite.getGlobalBounds())) {
                    Bird[j].reset();
                    setchicken.play();
                    shoots.erase(shoots.begin() + i);
                    score += 15;
                  
                    i--;
                    break;
                }
            }
        }
    

        window.draw(levelText);


        for (int i = 0; i < prizes.size(); i++) {
            window.draw(prizes[i]);
		}






        if (isgameover) {
			//setover.play();
            window.draw(GameText);

        }


         
        if (ispaused) {
            for (int i = 0; i < num_of_text; i++) window.draw(pause_menu[i]);
        }

        if (Keyboard::isKeyPressed(Keyboard::Enter) && isgameover) {
            isgameover = false; lives = 5; score = 0;
            for (int i = 0; i < 20; i++) Bird[i].reset();
            setback.play();

        }

      //  window.display();
    }

void leveltwo() {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            birdlevel2[r][c].initialize(new_bird);
			birdlevel2[r][c].spawn(startx + c * gapx, starty + r * gapy);  //to set position of each bird
        }
    }
}



int death_counter = 0;
void updateleveltwo(float dt) {
  
    if (!ispaused && lives > 0) {
   
        if (Keyboard::isKeyPressed(Keyboard::Left)) ship.move(-400 * dt, 0);
        if (Keyboard::isKeyPressed(Keyboard::Right)) ship.move(400 * dt, 0);

        if (ship.getPosition().x < 0) ship.setPosition(0, ship.getPosition().y);
        if (ship.getPosition().x > 890) ship.setPosition(889, ship.getPosition().y);
       
        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            counter_shoot2++;
            if (counter_shoot2 >= 10) {
                newshoots.push_back(setbig);
                newshoots.back().setPosition(ship.getPosition().x + 40, ship.getPosition().y);
                shootsound.play();
                counter_shoot2 = 0;
            }
        }

       
		float endx = startx + (cols - 1) * gapx; //to set the end x position of the last bird
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                birdlevel2[r][c].animate(dt, startx, endx);
            }
        }

            
        for (int i = 0; i < prizes.size(); i++) {
            prizes[i].move(0, 150 * dt);
        } 
        for (int i = 0; i < prizes.size(); i++) {
            if (prizes[i].getGlobalBounds().intersects(ship.getGlobalBounds())) {
                prizes.erase(prizes.begin() + i);
                i--;
                score += 5;
                setgain.play();
            }
            else if (prizes[i].getPosition().y > 800) {
                prizes.erase(prizes.begin() + i);
                i--;
            }
        }


        eggcounter += dt;
        if (eggcounter >= 0.5f) {
            int r = rand() %  rows;
            int c = rand() %cols;
            if (birdlevel2[r][c].isactive){
                Vector2f pos = birdlevel2[r][c].sprite.getPosition();
                setegg.setPosition(pos.x + 20, pos.y + 40);
                eggs.push_back(setegg);


            }

            eggcounter = 0.f;
        }

        for (int i = 0; i < eggs.size(); i++) {
            eggs[i].move(0, 200 * dt);

            if (eggs[i].getGlobalBounds().intersects(ship.getGlobalBounds())) {
                eggs.erase(eggs.begin() + i);

                i--;
                loselife();
            }
        }


        if (lives <= 0 && !oversound) {
            lives = 0;
            isgameover = true;
            oversound = true;
            setover.play();
            return;
        }
       
        // more than one loop i have to make bool to save me from any crash 
        for (int i = 0; i < newshoots.size(); i++) {
            newshoots[i].move(0, -600 * dt);

            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < cols; c++) {
                    if (birdlevel2[r][c].isactive &&
                        newshoots[i].getGlobalBounds().intersects(birdlevel2[r][c].sprite.getGlobalBounds())) {
                        Vector2f pos = birdlevel2[r][c].sprite.getPosition();  // before reset
                        birdlevel2[r][c].reset();
                        setchicken.play();
                        score += 20;
                        death_counter++;
                        newbirdcounter-=1.15;

                        if (death_counter >= 5) {
                            prize.setPosition(pos);
                            prizes.push_back(prize);
                            death_counter = 0;
                        }

                        newshoots.erase(newshoots.begin() + i);
                        i--;
                        goto next_shoot;  //like break but for more than one loop
                    }
                }
            }

            if (newshoots[i].getPosition().y < 0) {
                newshoots.erase(newshoots.begin() + i);
                i--;
            }
        next_shoot:;
        }
    }


    birdsScore.setString("Score: " + to_string(score));
    lifeText.setString("Lives: " + to_string(lives));


    if (isgameover && Keyboard::isKeyPressed(Keyboard::Enter)) {
       
        mainMenu();
        resetgame();
        return;
    }
}

void drawLevelTwo(RenderWindow& window) { // to open in the same window 

    //window.clear();
    window.draw(background);

    if (newbirdcounter <= 0)
        drawboss(window);


    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (birdlevel2[r][c].isactive) {
                window.draw(birdlevel2[r][c].sprite);
            }
        }
    }
    if (!isWin) {
        for (int i = 0; i < prizes.size(); i++)
            window.draw(prizes[i]);
    }
    if (!isWin) {
        for (int i = 0; i < newshoots.size(); i++)
            window.draw(newshoots[i]);
    }

    window.draw(ship);


    window.draw(birdsScore);
    window.draw(lifeText);
    window.draw(levelText);

    for (int i = 0; i < eggs.size(); i++) {
        window.draw(eggs[i]);
    }




    if (isgameover) {
      
        window.draw(GameText);
        setback.stop();
        

    }
    if (ispaused) {
        for (int i = 0; i < num_of_text; i++) window.draw(pause_menu[i]);
    }


    if (Keyboard::isKeyPressed(Keyboard::Enter) && isgameover) {
        setback.stop();
       // resetgame();
        leveltwo();
        mainMenu();
        return;

    }

   // window.display();
}

//int Gaincounter = 0;
float downspeed = 40;

void updateBoss(float dt) {
   

    if (!ispaused) {

        if (Keyboard::isKeyPressed(Keyboard::Left)) ship.move(-400 * dt, 0);
        if (Keyboard::isKeyPressed(Keyboard::Right)) ship.move(400 * dt, 0);

        healthScore.setPosition(bigone.getPosition().x+50, bigone.getPosition().y);

        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            counter_shoot2++;
            if (counter_shoot2 >= 10) {
                newshoots.push_back(setbig);
                newshoots.back().setPosition(ship.getPosition().x + 40, ship.getPosition().y);
                counter_shoot2 = 0;
            }
        }

        // move player bullets
        for (int i = 0; i < newshoots.size(); i++) {
            newshoots[i].move(0, -500 * dt);
            if (newshoots[i].getPosition().y < 0) { newshoots.erase(newshoots.begin() + i); i--; }
            else if (newshoots[i].getGlobalBounds().intersects(bigone.getGlobalBounds())) {
                health -= 1;
                newshoots.erase(newshoots.begin() + i);
                score += 10;
                setchicken.play();
                i--;
            }
        }
 
        if (newbirdcounter <= 0 && bigone.getPosition().y < 250) bigone.move(0, downspeed* dt);
        if (bigone.getPosition().y >= 250) {

            bigone.move(bossSpeed * dt, 0);
            if (bigone.getPosition().x > 850|| bigone.getPosition().x < 50)
                bossSpeed = -bossSpeed;

        }

        eggcounter += dt;
        if (eggcounter >= 0.5f) {
            setegg.setPosition(bigone.getPosition().x + bigone.getGlobalBounds().width / 2 - setegg.getGlobalBounds().width / 2,
                bigone.getPosition().y + bigone.getGlobalBounds().height / 2);
            eggs.push_back(setegg);
            eggcounter = 0.f;
        }
		if (newbirdcounter <= 0) levelText.setString("Boss Level");
		if (newbirdcounter <= 0) healthScore.setString("Boss Health: " + to_string(health));

        if (newbirdcounter <= 0) {
            for (int i = 0; i < eggs.size(); i++) {
                eggs[i].move(0, 200 * dt);
                if (eggs[i].getGlobalBounds().intersects(ship.getGlobalBounds())) {
                    loselife();
                    eggs.erase(eggs.begin() + i); i--;
             
                }
                else if (eggs[i].getPosition().y > 800) { eggs.erase(eggs.begin() + i); i--; }
            }
        }

         if (health <= 0 && !winsound) {
            health = 0;
            isWin = true;
            winsound = true;
            setwin.play();
            return;
        }
         /*
        if (lives <= 0 && !oversound) {
            lives = 0;
            isgameover = true;
            oversound = true;
            setover.play();
            return;


        }
        */
 }
    
}
void drawboss(RenderWindow& window) {
  //  window.clear();
    
    window.draw(background);

    if (!isWin) {
        window.draw(bigone);
    }
   
    window.draw(ship);

     
    for (int i = 0; i < newshoots.size(); i++) {
        window.draw(newshoots[i]);
    }

    if (!isWin ) {
        for (int i = 0; i < eggs.size(); i++) {
            window.draw(eggs[i]);
        }

    }
   /*if (isWin) {
        for (int i = 0; i < prizes.size(); i++) {
            window.draw(prizes[i]);
        }
	}
    */
    if(isWin){
        for (int i = 0; i < eggs.size(); i++) {
			eggs[i].setPosition(2000, 2000); 
        }
	}
    
    window.draw(birdsScore);
    window.draw(lifeText);
    window.draw(levelText);

   
    if (isWin && !isgameover) {
    
        window.draw(Wintext);
        setback.stop();
      
        
    }

    
    if (isgameover ) {
        window.draw(GameText);
        setback.stop();
    
           
    }
	if (newbirdcounter <= 0 && !isWin ) window.draw(healthScore);

 
    if (ispaused) {
        for (int i = 0; i < num_of_text; i++)
            window.draw(pause_menu[i]);
    }
    
    if (Keyboard::isKeyPressed(Keyboard::Enter) && isgameover) {
        setback.stop();
        
        mainMenu();
        resetgame();
        return;

    }


    if (Keyboard::isKeyPressed(Keyboard::Enter) && isWin ) {
        setback.stop();
      
        mainMenu();
        resetgame();
        return;

    }


   // window.display();
}

void loselife() {
    if (isgameover  || lives<=0) return;

    lives--;
  

    if (lives <= 0) {
        lives = 0;
        isgameover = true;
        oversound = true;
        setback.stop();
        setover.play();

    }

        }





