/* 
TODO:
    | | Players
        |x| Basics
        | | Distinguish players based on different textures, colors, etc.
    |x| Movement
    |x| Shooting
    | | Collision
        |x| Basics
        | | More complex collision detection using the Separating Axis Theorem (SAT)
    |x| TTF
    | | Asteroids
        | | Movement/Rotation
        | | Texture
        | | Ability to break into smaller pieces
            | | Logic
            | | Textures
        | | Chance of a power-up spawn upon explosion
    | | Sound
        | | Sound effects
            | | Bullet
                | | Bullet spawn
                | | Bullet explosion
            | | Jet
                | | Jet explosion
                | | Jet crash
            | | Asteroid explosion
        | | OST
    | | Animations
        |X| Bullet explosion
        | | Jet explosion
        | | Asteroid explosion
    | | Start-up menu
        | | Settings
    | | Multiplayer
        |x| Local (with only one keyboard)
        | | Network
            | | Basics
            | | 4 player capacity
    | | Singleplayer
        | | AI
            | | Basics
            | | Difficulty options
    | | Power-ups
    | | Gamemodes
*/

// Include every header required.
#include "include/includes.h"

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

TTF_Font* glFont = NULL;

Font score;

int Jet::jetCount = 0;

std::vector<Jet> jets;

AnimatedEntity::AnimatedEntity() { 
    frame = 0;
}
AnimatedEntity::~AnimatedEntity() {
    frame = 0;
}
Entity::Entity() {
    rTexture = NULL;
    rWidth = 0;
    rHeight = 0;
    posX = 0;
    posY = 0;
    deg = 0;
}

void Entity::free() {
    if (rTexture != NULL) {
        rTexture = NULL;
        rWidth = 0;
        rHeight = 0;
        posX = 0;
        posY = 0;
        deg = 0;
    }
}

bool Entity::loadSprite(std::string path) {
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

void Entity::render(SDL_Rect* clip) {
    SDL_Rect renderRect = {posX, posY, rWidth, rHeight};
    if (clip != NULL) {
        renderRect.w = clip->w;
        renderRect.h = clip->h;
    }
    SDL_RenderCopyEx(gRenderer, rTexture, clip, &renderRect, deg, NULL, SDL_FLIP_NONE);
}

int Entity::getWidth() {
	return rWidth;
}

int Entity::getHeight() {
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
    score = 0;
    frameOffset = 24;
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
    if (ev.type == SDL_KEYDOWN && ev.key.repeat == 0) {
        switch (ev.key.keysym.sym) {
            case SDLK_UP: if (currentJetN == 1) isBoosted = true; break;
            case SDLK_LEFT: if (currentJetN == 1) degV -= 3; break;
            case SDLK_RIGHT: if (currentJetN == 1) degV += 3; break;
            case SDLK_KP_0:
                if (currentJetN == 1) {
                    float radians = deg * M_PI/180;
                    this->bullets.emplace_back(posX, posY, deg, 7 * sin(radians), -7 * cos(radians), currentJetN - 1);
                    this->bullets[this->bullets.size() - 1].loadSprite(relativeMediaPath + "bullet-sheet.png");
                }
                break;

            case SDLK_w: if (currentJetN == 2) isBoosted = true; break;
            case SDLK_a: if (currentJetN == 2) degV -= 3; break;
            case SDLK_d: if (currentJetN == 2) degV += 3; break;
            case SDLK_SPACE: 
                if (currentJetN == 2) {
                    float radians = deg * M_PI/180;
                    this->bullets.emplace_back(posX, posY, deg, 7 * sin(radians), -7 * cos(radians), currentJetN - 1);
                    this->bullets[this->bullets.size() - 1].loadSprite(relativeMediaPath + "bullet-sheet.png");
                }
                break;
        }
    } else if (ev.type == SDL_KEYUP && ev.key.repeat == 0) {
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

Bullet::Bullet(int posX, int posY, int deg, int velX, int velY, int jetN) {
    this->deg = deg; this->velX = velX; this->velY = velY; this->jetN = jetN;
    float radians = deg * M_PI/180;
    this->posX = (posX + jets[jetN].rWidth/2 + jets[jetN].rHeight/2 * sin(radians)) - 3;
    this->posY = (posY + jets[jetN].rHeight/2 - jets[jetN].rHeight/2 * cos(radians)) - 3;
    boxCollider.w = 6;
    boxCollider.h = 6;
    frameOffset = 8;
    for (int i = 0; i < 48 / frameOffset; ++i) {
        clips.emplace_back();
        clips[i].x = i * frameOffset;
        clips[i].y = 0;
        clips[i].w = 8;
        clips[i].h = 8;
    }
}

void Bullet::free() {
     if (rTexture != NULL) {
        rWidth = 0;
        rHeight = 0;
        posX = 0;
        posY = 0;
        deg = 0;
        rTexture = NULL;
        jets[jetN].bullets.erase(jets[jetN].bullets.begin());
     }
}

void Bullet::move() {
    posX += velX;
    posY += velY;
    boxCollider.x = posX;
    boxCollider.y = posY;  
    if (posX < -rWidth || posX > SCREEN_WIDTH + rWidth || posY < -rHeight || posY > SCREEN_HEIGHT + rHeight)
        free();
}

bool Font::loadFont(std::string text) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(glFont, text.c_str(), {0, 0, 0});
    if (textSurface == NULL) {
        std::cout << SDL_GetError() << std::endl;
    } else {
        rTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if (rTexture == NULL) {
            std::cout << SDL_GetError() << std::endl;
        } else {
            rWidth = textSurface->w;
            rHeight = textSurface->h;
        }
        SDL_FreeSurface(textSurface);
    }
    return rTexture != NULL;
}

std::string formatScores() {
    std::string finalScore = "";
    for (int i = 0; i < Jet::jetCount; ++i) {
        finalScore += std::to_string(jets[i].score); 
        if (i != Jet::jetCount - 1) 
            finalScore += " - ";
    }
    return finalScore;
}

bool init() {
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << SDL_GetError() << std::endl;
        success = false;
    } else {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
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
                if (TTF_Init() == -1) {
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
        if (!jet.loadSprite(relativeMediaPath + "jet.png")) {
            std::cout << "Failed to load texture!" << std::endl;
            success = false;
        }
    }
    glFont = TTF_OpenFont("src/fonts/LibreFranklin-VariableFont_wght.ttf", 80);
    if (glFont == NULL) {
        std::cout << SDL_GetError() << std::endl;
        success = false;
    } else {
        if (!score.loadFont(formatScores())) {
            std::cout << SDL_GetError() << std::endl;
            success = false;
        }
    }
    return success;
}

bool checkCollision(SDL_Rect a, SDL_Rect b) {
    return !(a.y + a.h <= b.y || a.y >= b.y + b.h || a.x + a.w <= b.x || a.x >= b.x + b.w);
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
    TTF_Quit();
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
                while (!quit) {
                    while (SDL_PollEvent(&ev) != 0) {
                        if (ev.type == SDL_QUIT) {
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
                    score.render();
                    for (Jet& jet: jets) {
                        for (Jet& otherJet: jets) {
                            if (otherJet.currentJetN != jet.currentJetN) {
                                for (Bullet& bullet: otherJet.bullets) {
                                    if (checkCollision(jet.boxCollider, bullet.boxCollider)) {
                                        if (bullet.frame == 0)
                                            jet.score++;
                                        bullet.frame++;
                                        bullet.velX = 0;
                                        bullet.velY = 0;
                                        score.loadFont(formatScores());
                                    }
                                }
                            }
                        }
                    }
                    for (Jet& jet: jets) {
                        for (Bullet& bullet: jet.bullets) {
                            if (bullet.frame != 0 && bullet.frame <= 48)
                                bullet.frame++;
                            if (bullet.frame >= 48)
                                bullet.free();
                            bullet.render(&bullet.clips[bullet.frame / 8]);
                        }
                        jet.render(&jet.clips[jet.frame / 8]);
                    }
                    SDL_RenderPresent(gRenderer);   
                }
            }
        }
    
    close();
    return 0;
}
