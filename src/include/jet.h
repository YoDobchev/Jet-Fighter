#pragma once

// Screen dimensions
const uint16_t SCREEN_WIDTH = 640;
const uint16_t SCREEN_HEIGHT = 640;

// Texture class
struct Entity {
    Entity();

    bool loadSprite(std::string path);

    virtual void free();

    void render(SDL_Rect* clip = NULL, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    int getWidth();
    int getHeight();

    int rWidth, rHeight, posX, posY, deg;

    SDL_Texture* rTexture;
};

class Bullet: public Entity {
    public:
        Bullet(int posX, int posY, int deg, int velX, int velY, int jetN);

        void free() override;

        void move();

        SDL_Rect boxCollider;
    private:
        int jetN;
        float velX, velY;
};

class Jet: public Entity {
    public:
        Jet();
    
        ~Jet();

        void free() override;

        void handleEvent(SDL_Event& ev);

        void move();
        
        std::vector<Bullet> bullets;

        static int jetCount;

        SDL_Rect boxCollider;

        int currentJetN, score;
    private:
        int velX, velY, degV;
        float vel;
        bool isBoosted;
};

struct Font: public Entity {
    bool loadFont(std::string text);
};

std::string formatScores();

bool init();

bool loadMedia();

bool checkCollision(SDL_Rect a, SDL_Rect b);

void close();