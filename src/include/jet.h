#pragma once

// Screen dimensions
const uint16_t SCREEN_WIDTH = 640;
const uint16_t SCREEN_HEIGHT = 640;

// Texture class
struct RTexture {
    RTexture();

    bool loadSprite(std::string path);

    virtual void free();

    void render(SDL_Rect* clip = NULL, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    int getWidth();
    int getHeight();

    int rWidth, rHeight, posX, posY, deg;

    SDL_Texture* rTexture;
};
class Jet: public RTexture {
    public:
        Jet();
    
        ~Jet();

        void free() override;

        void handleEvent(SDL_Event& ev);

        void move();

        static int jetCount;
    private:
        int currentJetN, velX, velY, degV;
        float vel;
        bool isBoosted;
};
class Bullet: public RTexture {
    public:
        Bullet(int posX, int posY, int deg, int velX, int velY);

        void free() override;

        void move();
    private:
        int velX, velY;
};

bool init();

bool loadMedia();

void close();