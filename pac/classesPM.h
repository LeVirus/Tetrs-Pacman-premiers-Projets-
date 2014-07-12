#ifndef DEF_CLASSEGENERALH
#define DEF_CLASSEGENERALH

#include <unistd.h>
#include <iostream>
#include <string>
#include <SDL/SDL_ttf.h>
#include "constantePM.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <fmod.h>
#include <sstream>
#include <fstream>

Uint32 deplacerPacManBis(Uint32 intervalle, void *parametre);

class PacMan;
class Niveau;
class Fantome;
struct ToutPerso;
struct ImagesCommunes;
struct StructTimerPac;


class Niveau
	{
		private:
		bool deplacementEnCour, finJeu, blitFin, pause, ceriseAct;
		int matrice[26][21];
		int posX, posY, score, Hscore, nombreFantomeBouffe, nbPointAct, tempsCerise,intervalleCerise, pointCerise;
		SDL_Surface *vieN, *ecranN, *texteScore, *viderScore;
		SDL_Color couleurBleu;
		TTF_Font *police, *policeB;
		SDL_Rect positionCurseur, posVies; 
		std::ostringstream oss;
		std::string recepteur;
		FMOD_SYSTEM *system;
		FMOD_SOUND *musique,*pacMeur,*fantomeBouffe, *pointSpecial, *fantomeOut;
		FMOD_CHANNEL *channel;
		ImagesCommunes *images;

		public:
		ToutPerso *StructPerso;		
		Niveau();
		void miseEnPause();
		void jouerSon();
		void specialPoint();
		void resetNiveau(short choose);
		void initialiserPacEtFantomes();
		void appliquerFantomeBouffe(bool B, SDL_Rect *positionFantome);
		int numMatrice(int X, int Y);
		void afficherMatrice();
		void chargerNiveau(bool debug);
		void ecrireScore();
		void initNiveau(SDL_Surface *ecran, ToutPerso *tp, ImagesCommunes *imagesN);
		bool verifMatrice(int x, int y, int direction);
		void verifPoints(int x, int y, bool &continu);
		~Niveau();		
	};

class PacMan
	{
		private:
		Niveau *niv;
		SDL_Rect positionPacMan, positionReBlit, casePrecedante, blitBordDroite, blitBordGauche, blitTrans;
		SDL_Surface *pac, *pmA, *pmB, *pmC, *ecranP;
		SDL_Surface *pha, *phb, *pda, *pdb, *pga, *pgb, *pba, *pbb;
		int pacX, pacY, intervalleTimer;
		int timeChangePac, TAPac, TPPac;
		bool deplacementenCours, continu, pause;
		bool activeVerif, vertical, horizontal, transition, immobile;
		int direction, reBlitActuel;
		ImagesCommunes *images;
		SDL_TimerID timerP;
		short numBlit, nombreVies;

		public:
		PacMan();
		void pauseF();
		bool verifPause();
		bool verifContinu();
		void resetPacMan();
		void timerPac(StructTimerPac *pointeurVoid);
		void initialiser(SDL_Surface *ecran, Niveau *pointeurN, ImagesCommunes *imagesN);
		void verifCollision(SDL_Rect colFantome, bool modeBleu, Fantome *a, bool transF);
		void deplacerPacMan(SDL_Event *event, Niveau *n);
		int IAFantome(int FX, int FY, int dirInt, bool retour, bool modeBleu);
		~PacMan();
	};


class Fantome
	{
		private:
		Niveau *niv;
		PacMan *pacM;
		SDL_Rect positionFantome, curseurReBlit, blitBordDroiteF, blitBordGaucheF, blitTransFantome;
		SDL_Surface *fantome, *fantomeActuel, *ecranF;
		int FX, FY, FPixelX, FPixelY, direction, nbPixelDeplace;
		int tempsModeBleuDebut, tempsModeBleuActuel, intervalleTempsModeBleu, tempsLancement, tempsDebut, NUM;
		Uint32 intervalleTimer;
		bool deplacementenCours, lancer, timerLance, modeBleu, retour, pause, lancementEnCours;
		bool activeVerif, vertical, horizontal, transFantome, modePoursuite;
		SDL_TimerID timer;
		ImagesCommunes *images;
		SDL_Rect positionCurseur;

		public:
		Fantome();
		void pauseF();
		void reblitSurface();
		bool verifPause();
		void correctTime();
		void initialiserFantome(short choose);
		void pointeurs(SDL_Surface *ecran, Niveau *pointeurN, PacMan *pointeurPM, ImagesCommunes *imagesN);
		void timerFantome();
		void lancerFantome();
		void cibleFantome(bool what);
		void personnaliser(int choix);
		bool verifModeBleu();
		int verifAcceleration();
		void fantomeBouffe();
		void deplacerFantome();
		~Fantome();
	};

struct ToutPerso
	{
		Fantome *rouge, *bleu, *blanc, *rose;
		PacMan *P;
	};

struct ImagesCommunes
	{
		SDL_Surface *point, *pointSpe, *fantomePoursuite, *vide, *mur, *cerise; 
	};

struct StructTimerPac
	{
		Niveau *n;
		SDL_Event *event;
		PacMan *Pac;
	};

#endif



