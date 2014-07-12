/*#include "SDL/SDL.h"
#include "constante.h"
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <string>

void debutD(SDL_Surface *ecran);*/
#include "classes.h"

SDL_Surface* creerInterface()
	{
		TTF_Font *police = TTF_OpenFont("Impact Label.ttf", 30);
		SDL_Color couleurNoire = {0, 0, 0};
		SDL_Rect positionTemp;
		SDL_Surface *ecran=NULL, *zone=NULL, *cadreScore=NULL, *texteScore=NULL,*fond=NULL;
		ecran=SDL_SetVideoMode(LARGEUR_FENETRE,HAUTEUR_FENETRE,32,SDL_HWSURFACE| SDL_DOUBLEBUF);
		debutD(ecran);
		fond=IMG_Load("Images/arbre.jpg");
		positionTemp.x=0;positionTemp.y=0;
		SDL_BlitSurface(fond,NULL,ecran,&positionTemp);

		positionTemp.x=LIMITE_ZONE_GAUCHE;positionTemp.y=LIMITE_ZONE_HAUT;		
		zone= SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_BLOC * (NB_BLOCS_LARGEUR-2), TAILLE_BLOC * (NB_BLOCS_HAUTEUR-1), 32, 0, 0, 0, 0);
		SDL_FillRect(zone, NULL, SDL_MapRGB(zone->format, 0, 0, 0));
		SDL_BlitSurface(zone,NULL,ecran,&positionTemp);

		positionTemp.x=ZONE_SCORE_X-140;positionTemp.y=ZONE_SCORE_Y;
		cadreScore= SDL_CreateRGBSurface(SDL_HWSURFACE, 150, 70, 32, 0, 0, 0, 0);
		SDL_FillRect(zone, NULL, SDL_MapRGB(zone->format, 0, 0, 0));
		SDL_BlitSurface(cadreScore,NULL,ecran,&positionTemp);

				std::string SA="SCORE", SB="HIGHSCORE";
				size_t size = SA.size() + 1;
				char * t = new char[size];
				strncpy( t, SA.c_str(), size );
				texteScore = TTF_RenderText_Solid(police, t, couleurNoire);
				delete t;
				positionTemp.x=ZONE_SCORE_X-texteScore->w/2-cadreScore->w/2+10;
				positionTemp.y=ZONE_SCORE_Y-texteScore->h;
				SDL_BlitSurface(texteScore,NULL,ecran,&positionTemp);
				SDL_FreeSurface(texteScore);

				size = SB.size() + 1;
				t = new char[size];
				strncpy( t, SB.c_str(), size );
				texteScore = TTF_RenderText_Solid(police, t, couleurNoire);
				delete t;
				positionTemp.x=ZONE_SCORE_X-texteScore->w/2-cadreScore->w/2+10;
				positionTemp.y=ZONE_SCORE_Y+cadreScore->h;
				SDL_BlitSurface(texteScore,NULL,ecran,&positionTemp);

		SDL_FreeSurface(texteScore);
		SDL_FreeSurface(zone);
		return ecran;
	}

void debutD(SDL_Surface *ecran)
	{
		SDL_Event event;
		bool continuer=true;
		SDL_Surface *debut=NULL;
		debut=IMG_Load("Images/arbre.jpg");
		SDL_Rect positionD;positionD.x=0;positionD.y=0;
		SDL_BlitSurface(debut,NULL,ecran,&positionD);
		SDL_Flip(ecran);
		SDL_FreeSurface(debut);
		while(continuer)
			{
				SDL_WaitEvent(&event); 
				switch(event.type)
					{
						case SDL_KEYDOWN:
						continuer=false;
						break;
					}
			}
		
	}
