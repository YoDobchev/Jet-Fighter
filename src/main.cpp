// Include every header required.
#include "include/includes.h"

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

RTexture jet;

RTexture player;

RTexture::RTexture() {
    rTexture = NULL;
    rWidth = 0;
    rHeight = 0;
    posX = 300;
    posY = 300;
    velX = 0;
    velY = 0;
    deg = 0;
    degV = 0;
}
void RTexture::free() {
    if (rTexture != NULL) {
        rTexture = NULL;
        rWidth = 0;
        rHeight = 0;
        posX = 0;
        posY = 0;
        velX = 0;
        velY = 0;
        deg = 0;
        degV = 0;
    }
}
RTexture::~RTexture() {
    free();
}
bool RTexture::loadSprite(std::string path) {
    free();
    SDL_Texture* finalTexture = NULL;
    SDL_Surface* loadFromSurface = IMG_Load(path.c_str());
    if (loadFromSurface == NULL) {
        std::cout << SDL_GetError() << std::endl;
    } else {
        finalTexture = SDL_CreateTextureFromSurface(gRenderer, loadFromSurface);
        if (finalTexture == NULL) {
            std::cout << SDL_GetError() << std::endl;
        } else {
            rWidth = loadFromSurface->w;
            rHeight = loadFromSurface->h;
        }
        SDL_FreeSurface(loadFromSurface);
    }
    rTexture = finalTexture;
    return rTexture != NULL;   
}
void RTexture::render(SDL_Rect* clip, SDL_Point* center, SDL_RendererFlip flip) {
    SDL_Rect renderRect = {posX, posY, rWidth, rHeight};
    if (clip != NULL) {
        renderRect.w = clip->w;
        renderRect.h = clip->h;
    }
    SDL_RenderCopyEx(gRenderer, rTexture, clip, &renderRect, deg, center, flip);
}
void RTexture::handleEvent(SDL_Event& ev) {
    if(ev.type == SDL_KEYDOWN && ev.key.repeat == 0) {
        std::cout << deg << std::endl;
        switch(ev.key.keysym.sym) {
            case SDLK_UP:
                velX += 2 * sin(deg);
                velY += 2 * cos(deg) ;
                break;
            case SDLK_LEFT: degV -= 2; break;
            case SDLK_RIGHT: degV += 2; break;
        }
    } else if(ev.type == SDL_KEYUP && ev.key.repeat == 0) {
        switch(ev.key.keysym.sym) {
            case SDLK_UP:
                velX = 0;
                velY = 0;
                break;
            case SDLK_LEFT: degV += 2; break;
            case SDLK_RIGHT: degV -= 2; break;
        }
    }
}
void RTexture::move() {
    posX += velX;
    posY += velY;
    if (deg >= 360 && deg >= 0)
        deg -= 360;
    if (deg <= 360 && deg <= 0)
        deg += 360;
    deg += degV;
    std::cout << deg << " | "  << sin(deg) << " | " << cos(deg) << std::endl;
}
int RTexture::getWidth() {
	return rWidth;
}

int RTexture::getHeight() {
	return rHeight;
}
bool init() {
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << SDL_GetError() << std::endl;
        success = false;
    } else {

        if(!SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
            std::cout << "LTF not enabled" << std::endl;
		}
        gWindow = SDL_CreateWindow("Jet Fighter", 650, 150, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            std::cout << SDL_GetError() << std::endl;
            success = false;
        } else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL) {
                std::cout << SDL_GetError() << std::endl;
                success = false;
            } else {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
                    std::cout << SDL_GetError() << std::endl;
                    success = false;
                }
            }
        }
    }

    return success;
}
bool loadMedia() {
    bool success = true;
    if(!jet.loadSprite("src/media/jet.png")) {
        std::cout << "Failed to load texture!" << std::endl;
        success = false;
    }
    return success;
}
void close() {
    jet.free();
    SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
    IMG_Quit();
	SDL_Quit();
}
int main(int argc, char* args[]) {
        if (!init()) {
            std::cout << "Failed to initialize!" << std::endl;
        } else {
            if (!loadMedia()) {
                std::cout << "Failed to load media!" << std::endl;
            } else {
                bool quit;
                SDL_Event ev;
                int right = 0;
                while(!quit) {
                    while(SDL_PollEvent( &ev ) != 0) {
                        if(ev.type == SDL_QUIT) {
                            quit = true;
                        }
                        jet.handleEvent(ev);
                    }
                    jet.move();
                    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                    SDL_RenderClear(gRenderer);
                    jet.render(NULL, NULL, SDL_FLIP_NONE);
                    SDL_RenderPresent(gRenderer);   
                }
            }
        }
    
    close();
    return 0;
}