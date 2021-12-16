#pragma once

// Screen dimensions
const uint16_t SCREEN_WIDTH = 640;
const uint16_t SCREEN_HEIGHT = 480;

// Texture class
class RTexture {
    public:
        RTexture();

        ~RTexture();

        bool loadImage(std::string path);

        void freeM();

        void render(int x, int y, float angle, SDL_Point* center, SDL_RendererFlip flip);

        int getWidth();
        int getHeight();
    private:
        int rWidth;
        int rHeight;

        SDL_Texture* rTexture;
};

bool init();

bool loadMedia();

void close();