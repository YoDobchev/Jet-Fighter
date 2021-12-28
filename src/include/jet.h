#pragma once

// Screen dimensions
const uint16_t SCREEN_WIDTH = 640;
const uint16_t SCREEN_HEIGHT = 640;

// Texture class
class RTexture {
    public:
        RTexture();

        ~RTexture();

        bool loadSprite(std::string path);

        void free();

        void render(SDL_Rect* clip = NULL, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

  		void handleEvent(SDL_Event& ev);
        
        void move();

        int getWidth();
        int getHeight();

        static int jetCount;
    private:
        int currentJetN, rWidth, rHeight, posX, posY, velX, velY, deg, degV;
        float vel;
        bool isBoosted;

        SDL_Texture* rTexture;
};

bool init();

bool loadMedia();

void close();