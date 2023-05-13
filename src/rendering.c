#include <math.h>
#include <limits.h>
#include "../inc/header.h"
#include "../inc/constants.h"



float distanceBetweenPoints(float x1, float y1, float x2, float y2)
{
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}


void renderMap(SDL_Instance instance)
{
	int i, j;
	int tileX, tileY, tileColor;

	for (i = 0; i < MAP_NUM_ROWS; i++)
	{
		for (j = 0; j < MAP_NUM_COLS; j++)
		{
			tileX = j * TILE_SIZE;
			tileY = i * TILE_SIZE;
			tileColor = map[i][j] != 0 ? 255 : 0;

			SDL_SetRenderDrawColor(instance.renderer, tileColor, tileColor, tileColor, 255);
			SDL_Rect mapTileRect = {
				tileX * MINIMAP_SCALE_FACTOR,
				tileY * MINIMAP_SCALE_FACTOR,
				TILE_SIZE * MINIMAP_SCALE_FACTOR,
				TILE_SIZE * MINIMAP_SCALE_FACTOR
			};
			SDL_RenderFillRect(instance.renderer, &mapTileRect);
		}
	}
}


void renderText(SDL_Instance instance, struct textPrint tp)
{
	// Render title
    SDL_Color colorTitle1 = {255, 255, 255, 255};
    TTF_Font* fontTitle1 = TTF_OpenFont("fonts/Raleway-SemiBoldItalic.ttf", 30);
    SDL_Surface *surfaceTitle1 = TTF_RenderText_Solid(fontTitle1, tp.title, colorTitle1);
    SDL_Texture *textureTitle1 = SDL_CreateTextureFromSurface(instance.renderer, surfaceTitle1);
    // calculate the width and height of the text
    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(textureTitle1, NULL, NULL, &texW, &texH);
	int x = (WINDOW_WIDTH - texW) / 2;
    SDL_Rect dstrect = { x, 0, texW, texH };
    SDL_RenderCopy(instance.renderer, textureTitle1, NULL, &dstrect);
    SDL_FreeSurface(surfaceTitle1);
    SDL_DestroyTexture(textureTitle1);
    TTF_CloseFont(fontTitle1);

	// Render sub-title 1
	SDL_Color color1 = {255, 255, 255, 255};
    TTF_Font* font1 = TTF_OpenFont("fonts/Raleway-Regular.ttf", 25);
    SDL_Surface *surface1 = TTF_RenderText_Solid(font1, tp.subTitles[0], color1);
    SDL_Texture *texture1 = SDL_CreateTextureFromSurface(instance.renderer, surface1);
    int texW1 = 0;
    int texH1 = 0;
    SDL_QueryTexture(texture1, NULL, NULL, &texW1, &texH1);
	int x1 = (WINDOW_WIDTH - texW1) / 2;
    SDL_Rect dstrect1 = { x1, 2*texH, texW1, texH1};
    SDL_RenderCopy(instance.renderer, texture1, NULL, &dstrect1);
    SDL_FreeSurface(surface1);
    SDL_DestroyTexture(texture1);
    TTF_CloseFont(font1);


    // Render sub-titles
    TTF_Font* subTitleFont = TTF_OpenFont("fonts/Raleway-Medium.ttf", 20);
    SDL_Color subTitleColor = {200, 200, 200, 255};
    int yOffset = texH1 + 4*texH; // Offset for sub-titles position
    for (int i = 1; i < tp.numSubTexts-1; i++) {
        SDL_Surface* subTitleSurface = TTF_RenderText_Solid(subTitleFont, tp.subTitles[i], subTitleColor);
        SDL_Texture* subTitleTexture = SDL_CreateTextureFromSurface(instance.renderer, subTitleSurface);
        int subTitleW = 0;
        int subTitleH = 0;
        SDL_QueryTexture(subTitleTexture, NULL, NULL, &subTitleW, &subTitleH);
		int xsub = (WINDOW_WIDTH - subTitleW) / 2;
        SDL_Rect subTitleRect = {xsub, yOffset, subTitleW, subTitleH};
        SDL_RenderCopy(instance.renderer, subTitleTexture, NULL, &subTitleRect);
        SDL_FreeSurface(subTitleSurface);
        SDL_DestroyTexture(subTitleTexture);
        yOffset += subTitleH + 5; // Increase offset for the next sub-title
    }
    TTF_CloseFont(subTitleFont);

	// render copywrite in the end of the screen
	SDL_Color color2 = {255, 255, 255, 255};
	TTF_Font* font2 = TTF_OpenFont("fonts/Raleway-Regular.ttf", 15);
	SDL_Surface *surface2 = TTF_RenderText_Solid(font2, tp.subTitles[tp.numSubTexts-1], color2);
	SDL_Texture *texture2 = SDL_CreateTextureFromSurface(instance.renderer, surface2);
	int texW2 = 0;
	int texH2 = 0;
	SDL_QueryTexture(texture2, NULL, NULL, &texW2, &texH2);
	int x2 = (WINDOW_WIDTH - texW2) / 2;
	SDL_Rect dstrect2 = { x2, WINDOW_HEIGHT - texH2, texW2, texH2};
	SDL_RenderCopy(instance.renderer, texture2, NULL, &dstrect2);
	SDL_FreeSurface(surface2);
	SDL_DestroyTexture(texture2);
	TTF_CloseFont(font2);

}
