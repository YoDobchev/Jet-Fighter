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

        void render(SDL_Rect* clip, SDL_Point* center, SDL_RendererFlip flip);

  		void handleEvent(SDL_Event& ev);
        
        void move();

        int getWidth();
        int getHeight();
    private:
        int rWidth, rHeight, posX, posY, velX, velY, deg, degV;

        SDL_Texture* rTexture;
};

bool init();

bool loadMedia();

void close();