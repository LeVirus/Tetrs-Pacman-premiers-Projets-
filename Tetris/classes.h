#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <fmodex/fmod.h>
#include <sstream>
#include "constante.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <ctime>

void debutD(SDL_Surface *ecran);
SDL_Surface* creerInterface();


class zoneJeu;

struct elementTableau
	{
		int etat, image;
	};
	

struct paramTimer
	{
		SDL_Surface *ecranC;
		zoneJeu *pointClass;
	};

class zoneJeu
	{
		private:
		bool deplacementEnCour, finJeu, blitFin;
		elementTableau** matrice;
		int TetraActuel, posX,posY, score, HScore, intervalleTimer,changerVitesse;
		SDL_Surface *vide, *cubeA, *cubeB, *cubeC, *cubeD,*cubeE,*cubeF,*cubeG, *highScore, *ScoreImage, *g;
		SDL_Rect positionCurseur;
		SDL_TimerID timer;
		FMOD_SYSTEM *system;
		FMOD_SOUND *musique,*ligneSon,*tempsSon;
		FMOD_CHANNEL *channel;

		public:
		zoneJeu();
		void rebootMatrice();
		void debug();
		void afficheMatrice();
		int verifAcceleration();
		void ecrireScore(SDL_Surface *ecran);
		void deplacerElement(elementTableau& emmeteur, elementTableau& recepteur, SDL_Surface *ecran, int direction);
		void generateurTetra(SDL_Surface *ecran);
		void descendreCubeTimer(SDL_Surface *ecran);
		void deplacerTetra(int direction, SDL_Surface *ecran);
		void pivoterTetra(SDL_Surface *ecran, bool trigo);
		void verifLignes(SDL_Surface *ecran, int YMAX, int YMIN);
		bool verifGameOver();
		void afficherScore(SDL_Surface *ecran);
		~zoneJeu();
	};

