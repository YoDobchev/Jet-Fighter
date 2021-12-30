// Include every header required.
#include "include/includes.h"

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

int Jet::jetCount = 0;

std::vector<Jet> jets;

RTexture::RTexture() {
    rTexture = NULL;
    rWidth = 0;
    rHeight = 0;
    posX = 0;
    posY = 0;
    deg = 0;
}
void RTexture::free() {
    if (rTexture != NULL) {
        rTexture = NULL;
        rWidth = 0;
        rHeight = 0;
        posX = 0;
        posY = 0;
        deg = 0;
    }
}
bool RTexture::loadSprite(std::string path) {
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

int RTexture::getWidth() {
	return rWidth;
}

int RTexture::getHeight() {
	return rHeight;
}

Jet::Jet() {
    posX = 300;
    posY = 300;
    currentJetN = 0;
    velX = 0;
    velY = 0;
    degV = 0;
    vel = 0;
    isBoosted = false;
    jetCount++;
    currentJetN = jetCount;
    boxCollider.w = 22;
    boxCollider.h = 16;
}
Jet::~Jet() {
    free();
}
void Jet::free() {
    if (rTexture != NULL) {
        rTexture = NULL;
        rWidth = 0;
        rHeight = 0;
        posX = 0;
        posY = 0;
        deg = 0;
        currentJetN = 0;
        velX = 0;
        velY = 0;
        degV = 0;
        vel = 0;
        isBoosted = false;
        jetCount--;
        currentJetN = 0;
    }
}
void Jet::handleEvent(SDL_Event& ev) {
    if(ev.type == SDL_KEYDOWN && ev.key.repeat == 0) {
        switch(ev.key.keysym.sym) {
            case SDLK_UP: if (currentJetN == 1) isBoosted = true; break;
            case SDLK_LEFT: if (currentJetN == 1) degV -= 3; break;
            case SDLK_RIGHT: if (currentJetN == 1) degV += 3; break;
            case SDLK_KP_0:
                if (currentJetN == 1) {
                    float radians = deg * M_PI/180;
                    this->bullets.emplace_back(posX, posY, deg, 7 * sin(radians), -7 * cos(radians), currentJetN - 1);
                    this->bullets[this->bullets.size() - 1].loadSprite("src/media/bullet.png");
                }
                break;

            case SDLK_w: if (currentJetN == 2) isBoosted = true; break;
            case SDLK_a: if (currentJetN == 2) degV -= 3; break;
            case SDLK_d: if (currentJetN == 2) degV += 3; break;
            case SDLK_SPACE: 
                if (currentJetN == 2) {
                    float radians = deg * M_PI/180;
                    this->bullets.emplace_back(posX, posY, deg, 7 * sin(radians), -7 * cos(radians), currentJetN - 1);
                    this->bullets[this->bullets.size() - 1].loadSprite("src/media/bullet.png");
                }
                break;
        }
    } else if(ev.type == SDL_KEYUP && ev.key.repeat == 0) {
        switch(ev.key.keysym.sym) {
            case SDLK_UP: if (currentJetN == 1) isBoosted = false; break;
            case SDLK_LEFT: if (currentJetN == 1) degV += 3; break;
            case SDLK_RIGHT: if (currentJetN == 1) degV -= 3; break;

            case SDLK_w: if (currentJetN == 2) isBoosted = false; break;
            case SDLK_a: if (currentJetN == 2) degV += 3; break;
            case SDLK_d: if (currentJetN == 2) degV -= 3; break;
        }
    }
}
void Jet::move() {
    if (isBoosted && vel <= 6) {
        vel += 0.1f;
    } else if (vel > 0) {
        vel -= 0.05f;
    }
    float radians = deg * M_PI/180;
    velX = vel * sin(radians);
    velY = -vel * cos(radians);
    posX += velX;
    posY += velY;
    boxCollider.x = posX + 1;
    boxCollider.y = posY;
    if (posX < -rWidth/2)
        posX = SCREEN_WIDTH + rWidth/2;
    if (posX > SCREEN_WIDTH + rWidth/2)
        posX = -rWidth/2;

    if (posY < -rHeight/2)
        posY = SCREEN_HEIGHT + rHeight/2;
    if (posY > SCREEN_HEIGHT + rHeight/2)
        posY = -rHeight/2;
    deg += degV;
}
void Bullet::free() {
     if (rTexture != NULL) {
        rTexture = NULL;
        rWidth = 0;
        rHeight = 0;
        posX = 0;
        posY = 0;
        deg = 0;
        velX = 0;
        velY = 0;
        jets[jetN].bullets.erase(jets[jetN].bullets.begin());
     }
}
Bullet::Bullet(int posX, int posY, int deg, int velX, int velY, int jetN) {
    this->deg = deg; this->velX = velX; this->velY = velY; this->jetN = jetN;
    float radians = deg * M_PI/180;
    this->posX = (posX + jets[jetN].rWidth/2 + jets[jetN].rHeight/2 * sin(radians)) - 3;
    this->posY = (posY + jets[jetN].rHeight/2 - jets[jetN].rHeight/2 * cos(radians)) - 3;
    boxCollider.w = 6;
    boxCollider.h = 6;
}
void Bullet::move() {
    posX += velX;
    posY += velY;
    boxCollider.x = posX;
    boxCollider.y = posY;  
    if (posX < -rWidth || posX > SCREEN_WIDTH + rWidth || posY < -rHeight || posY > SCREEN_HEIGHT + rHeight)
        free();
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
                jets.reserve(2);
                jets.emplace_back();
                jets.emplace_back();
            }
        }
    }

    return success;
}
bool loadMedia() {
    bool success = true;
    for (Jet& jet: jets) {
        if(!jet.loadSprite("src/media/jet.png")) {
            std::cout << "Failed to load texture!" << std::endl;
            success = false;
        }
    }
    return success;
}
bool checkCollision(SDL_Rect a, SDL_Rect b) {
    if (a.y + a.h <= b.y || a.y >= b.y + b.h || a.x + a.w <= b.x || a.x >= b.x + b.w)
        return false;
    return true;
}
void close() {
    for (Jet& jet: jets) {
        jet.free();
        for (Bullet& bullet: jet.bullets) {
            bullet.free();
        }
    }
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
                    while(SDL_PollEvent(&ev) != 0) {
                        if(ev.type == SDL_QUIT) {
                            quit = true;
                        }
                        for (Jet& jet: jets) {
                            jet.handleEvent(ev);
                        }
                    }
                    for (Jet& jet: jets) {
                        jet.move();
                        for (Bullet& bullet: jet.bullets) {
                            bullet.move();
                            
                        }
                    }
                    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                    SDL_RenderClear(gRenderer);
                    for (Jet& jet: jets) {
                        for(Jet& otherJet: jets) {
                            if (otherJet.currentJetN != jet.currentJetN) {
                                for (Bullet& bullet: otherJet.bullets) {
                                    if (checkCollision(jet.boxCollider, bullet.boxCollider))
                                        bullet.free();
                                }
                            }
                        }
                    }
                    for (Jet& jet: jets) {
                        jet.render();
                        // SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );		
                        // SDL_RenderDrawRect(gRenderer, &jet.boxCollider);
                        // SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                        for (Bullet& bullet: jet.bullets) {
                            bullet.render();
                            // SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );		
                            // SDL_RenderDrawRect(gRenderer, &bullet.boxCollider);
                            // SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                        }
                    }
                    SDL_RenderPresent(gRenderer);   
                }
            }
        }
    
    close();
    return 0;
}