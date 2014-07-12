/*#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ctime>*/
//#include <sstream>
//#include <string>
//#include "constante.h"
/*#include "SDL/SDL.h"
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
//#include "fonctions.cpp"
#include <FMOD/fmod.h>*/
#include "classes.h"


using namespace std;

int main(int argc, char* args[])
	{
		bool continuer=true,recommencer=true;
		SDL_Init(SDL_INIT_VIDEO| SDL_INIT_TIMER);
		if (SDL_Init(SDL_INIT_VIDEO) == -1)
   			{
       				 cout<<"Erreur d'initialisation de la SDL"<<endl;
       				 exit(EXIT_FAILURE);
    			}
		TTF_Init();
		SDL_ShowCursor(SDL_DISABLE);
		SDL_Event event;
		zoneJeu a;
		SDL_Surface *ecran=creerInterface(); 
		SDL_WM_SetCaption("Tetris",NULL);
		a.generateurTetra(ecran);
		a.descendreCubeTimer(ecran);
		SDL_EnableKeyRepeat(1, 50);
		a.ecrireScore(ecran);
		while(recommencer)
		{
			if(!continuer)
				{
					continuer=true;
					a.rebootMatrice();
					SDL_FreeSurface(ecran);
					SDL_Surface *ecran=creerInterface();
					SDL_WM_SetCaption("Tetris",NULL);
					a.ecrireScore(ecran);
					a.debug();
					a.generateurTetra(ecran);
				}
			while(continuer)
				{
					SDL_WaitEvent(&event);
						
							if(!a.verifGameOver()){continuer=false; break;}
				
							switch(event.type)
								{	
									case SDL_QUIT:
									continuer=false;
									recommencer=false;
									break;

									case SDL_KEYDOWN:
										switch(event.key.keysym.sym)
											{
												case SDLK_DOWN:	
												a.deplacerTetra(BAS,ecran);
												break;

												case SDLK_LEFT:SDL_EnableKeyRepeat(1, 50);
												a.deplacerTetra(GAUCHE,ecran);
												break;

												case SDLK_RIGHT:SDL_EnableKeyRepeat(1, 50);
												a.deplacerTetra(DROITE,ecran);
												break;

												case SDLK_a:SDL_EnableKeyRepeat(1, 50);
												a.pivoterTetra(ecran,true);
												break;

												case SDLK_z:SDL_EnableKeyRepeat(1, 50);
												a.pivoterTetra(ecran,false);
												break;
											}
									break;
								
								}
				}
//ECRIRE ECRAN DE SELECTION
		}
		SDL_Quit();
		TTF_Quit() ;
		return EXIT_SUCCESS;
	}
