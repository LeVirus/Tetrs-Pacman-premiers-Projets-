#include "classesPM.h"	

Uint32 deplacerFantomeBis(Uint32 intervalle, void *parametre);

using namespace std;

Fantome::Fantome()
	{
		images=NULL;
		blitBordGaucheF.y=12*TAILLE_BLOC+LIMITE_ZONE_HAUT;
		blitBordGaucheF.x=LIMITE_ZONE_GAUCHE-TAILLE_BLOC;
		blitBordDroiteF.y=12*TAILLE_BLOC+LIMITE_ZONE_HAUT;
		blitBordDroiteF.x=NB_BLOCS_LARGEUR*TAILLE_BLOC+LIMITE_ZONE_GAUCHE;
		fantome=NULL;
	}

void Fantome::pointeurs(SDL_Surface *ecran, Niveau *pointeurN, PacMan *pointeurPM, ImagesCommunes *imagesN){
	ecranF=ecran;
	niv=pointeurN;
	pacM=pointeurPM;
	if(!images)images=imagesN;
}

void Fantome::pauseF(){
	pause=!pause;
	if(!lancer){tempsDebut=SDL_GetTicks()-tempsDebut;}
	if(pause  &&  modeBleu){
		tempsModeBleuActuel=SDL_GetTicks();
		tempsModeBleuActuel-=tempsModeBleuDebut;
	}
	else if(!pause  &&  modeBleu) {
		tempsModeBleuDebut=SDL_GetTicks()-tempsModeBleuActuel;
	}
	
}

void Fantome::reblitSurface(){
	SDL_BlitSurface(images->vide,NULL,ecranF,&positionFantome);
	if(direction==GAUCHE  &&  FY==12  &&  FX<4  ||  direction==DROITE  &&  FX>NB_BLOCS_LARGEUR-4)return;
	for(int i=0;i<2;i++){
			short a=niv->numMatrice(FX,FY);
			if(  a==POINT  )SDL_BlitSurface(images->point,NULL,ecranF,&curseurReBlit);
			else if(  a==POINT_SPECIAL  )SDL_BlitSurface(images->pointSpe,NULL,ecranF,&curseurReBlit);
			else if(a==BLOCK_MUR)SDL_BlitSurface(images->mur,NULL,ecranF,&curseurReBlit);
			if(i==1)return;
			switch(direction){
				case GAUCHE:
				FX--;
				curseurReBlit.x-=30;
				break;
				case DROITE:
				FX++;
				curseurReBlit.x+=30;
				break;
				case BAS:
				FY++;
				curseurReBlit.y+=30;
				break;
				case HAUT:
				FY--;
				curseurReBlit.y-=30;
				break;
			}
	}
}

bool Fantome::verifPause(){
	return pause;
}

void Fantome::initialiserFantome(short choose)//si true nouveau niveau
	{
		if(choose==PAC_BOUFFE){
			reblitSurface();
		}
		else if(choose==LEVEL_UP){
			if(intervalleTimer>40)intervalleTimer-=10;
			if(tempsLancement>1000)tempsLancement-=1000;
			if(intervalleTempsModeBleu>4000)intervalleTempsModeBleu-=1000;
		}

		else if(choose==GAME_OVER){
			personnaliser(NUM);
		}
		if(transFantome)SDL_BlitSurface(images->vide,NULL,ecranF,&blitTransFantome);
		tempsDebut=SDL_GetTicks();
		nbPixelDeplace=15;
		positionCurseur.x=INIT_FANTOME_X;
		positionCurseur.y=INIT_FANTOME_Y-TAILLE_BLOC;
		positionFantome.y=12*TAILLE_BLOC+LIMITE_ZONE_HAUT;
		positionFantome.x=9*TAILLE_BLOC+LIMITE_ZONE_GAUCHE;
		FPixelX=positionFantome.x;
		FPixelY=positionFantome.y;
		SDL_BlitSurface(fantome,NULL,ecranF,&positionFantome);
		curseurReBlit.x=positionFantome.x;
		curseurReBlit.y=positionFantome.y;
		FX=9;
		FY=12;
		blitTransFantome.y=positionFantome.y;
		fantomeActuel=fantome;
		deplacementenCours=false;
		modePoursuite=false;
		vertical=false, horizontal=true;
		retour=false; lancer=false; modeBleu=false;
		activeVerif=true;
		pause=false;
		transFantome=false;
		lancementEnCours=false;
	}

void Fantome::personnaliser(int choix)
	{
		switch(choix)
			{
				case ROUGE:
				NUM=ROUGE;
				tempsLancement=1000;
				intervalleTimer=140;
				if(!fantome)fantome=IMG_Load("Images/fantomeROUGE.jpeg");
				fantomeActuel=fantome;
				break;

				case ROSE:
				NUM=ROSE;
				tempsLancement=3000;
				intervalleTimer=150;
				if(!fantome)fantome=IMG_Load("Images/fantomeROSE.png");
				fantomeActuel=fantome;
				break;

				case BLANC:
				NUM=BLANC;
				tempsLancement=5000;
				intervalleTimer=160;
				if(!fantome)fantome=IMG_Load("Images/fantomeJAUNE.png");
				fantomeActuel=fantome;
				break;

				case BLEU:
				NUM=BLEU;
				tempsLancement=7000;
				intervalleTimer=170;
				if(!fantome)fantome=IMG_Load("Images/fantomeBLEU.png");
				fantomeActuel=fantome;
				break;
			}
		intervalleTempsModeBleu=6000;
		lancementEnCours=false;
	}

void Fantome::deplacerFantome()	
	{
		if(modeBleu)
			{
				tempsModeBleuActuel=SDL_GetTicks();
				if(tempsModeBleuActuel-tempsModeBleuDebut>intervalleTempsModeBleu-1900  &&  tempsModeBleuActuel-tempsModeBleuDebut<intervalleTempsModeBleu-1800)niv->jouerSon();
				if(tempsModeBleuActuel-tempsModeBleuDebut>=intervalleTempsModeBleu)this->cibleFantome(false);
			}
		if(!retour  &&  !lancer  &&  SDL_GetTicks()-tempsDebut>tempsLancement){lancerFantome();return;}
			if(activeVerif){
					int dirInt;
					switch(direction)
						{
							case GAUCHE:
							dirInt=DROITE;
							break;
							case DROITE:
							dirInt=GAUCHE;
							break;
							case HAUT:
							dirInt=BAS;
							break;
							case BAS:
							dirInt=HAUT;
							break;
							default:
							break;
						}
					direction=pacM->IAFantome(FX, FY, dirInt, retour, modeBleu);
						if( retour  &&  FX==10  &&  FY==12){retour=false; lancer=false; modeBleu=false;}
					
				}
						SDL_BlitSurface(images->vide, NULL, ecranF, &positionFantome);


						if(  niv->numMatrice(FX,FY)==POINT  )SDL_BlitSurface(images->point,NULL,ecranF,&curseurReBlit);
						else if(  niv->numMatrice(FX,FY)==POINT_SPECIAL  )SDL_BlitSurface(images->pointSpe,NULL,ecranF,&curseurReBlit);
						else if(  niv->numMatrice(FX,FY)==CERISE  )SDL_BlitSurface(images->cerise,NULL,ecranF,&curseurReBlit);
						else if(niv->numMatrice(FX,FY)==BLOCK_MUR)SDL_BlitSurface(images->mur,NULL,ecranF,&curseurReBlit);


						switch(direction)
							{
//________________________________________________________________________________________________
								case GAUCHE:
										positionFantome.x-=nbPixelDeplace;
										vertical=false; horizontal=true;
										deplacementenCours=true;activeVerif=false;
										if(positionFantome.x+30==FPixelX)
											{
												FX--; 
												FPixelX=positionFantome.x;
												activeVerif=true;
												curseurReBlit.x=positionFantome.x;
												curseurReBlit.y=positionFantome.y;
												if(FX==1  && FY==12)nbPixelDeplace=6;
												else if(FX==NB_BLOCS_LARGEUR-2  && FY==12)nbPixelDeplace=15;
												else if(FX==-1  && FY==12)
													{
														transFantome=false;
														positionFantome.x=LIMITE_ZONE_DROITE-TAILLE_BLOC;
														FPixelX=positionFantome.x;
														FX=20;
														transFantome=false;	
														SDL_BlitSurface(images->vide,NULL,ecranF,&blitTransFantome);				
													}
											}
								break;

//________________________________________________________________________________________________
								case DROITE:
										positionFantome.x+=nbPixelDeplace;
										vertical=false; horizontal=true;
										deplacementenCours=true;activeVerif=false;
										if(positionFantome.x-30==FPixelX)
											{
												FX++; 
												FPixelX=positionFantome.x;
												activeVerif=true;
												curseurReBlit.x=positionFantome.x;
												curseurReBlit.y=positionFantome.y;
												if(FX==1  && FY==12)nbPixelDeplace=15;
												else if(FX==NB_BLOCS_LARGEUR-2  && FY==12)nbPixelDeplace=6;
												if(FX==21  && FY==12)
													{
														transFantome=false;
														positionFantome.x=LIMITE_ZONE_GAUCHE;
														FPixelX=positionFantome.x;
														FX=0;
														transFantome=false;	
														SDL_BlitSurface(images->vide,NULL,ecranF,&blitTransFantome);								
													}
											}

								break;

//________________________________________________________________________________________________
								case BAS:
										positionFantome.y+=nbPixelDeplace;
										vertical=true; horizontal=false;
										deplacementenCours=true;activeVerif=false;
										if(positionFantome.y-30==FPixelY)
											{
												FY++;
												FPixelY=positionFantome.y;
												activeVerif=true; 
												curseurReBlit.x=positionFantome.x;
												curseurReBlit.y=positionFantome.y;
											}

								break;

//________________________________________________________________________________________________
								case HAUT:
										positionFantome.y-=nbPixelDeplace;
 										vertical=true; horizontal=false;
										deplacementenCours=true;activeVerif=false;
										if(positionFantome.y+30==FPixelY)
											{
												FY--;
												FPixelY=positionFantome.y;
												activeVerif=true; 
												curseurReBlit.x=positionFantome.x;
												curseurReBlit.y=positionFantome.y;
											}

								break;
//________________________________________________________________________________________________
								default:
								break;
							}
						if(!retour  &&  lancer)pacM->verifCollision(this->positionFantome, modeBleu, this, transFantome);
						SDL_BlitSurface(fantomeActuel,NULL,ecranF,&positionFantome);
						if(activeVerif  &&  direction==GAUCHE  &&  FY==12  &&  FX==0  ||  direction==DROITE  &&  FY==12  &&  FX==NB_BLOCS_LARGEUR-1)
							{
								transFantome=true;	
								activeVerif=false;
								deplacementenCours=true;
							}
						if(transFantome)
							{
								if(FX==0)blitTransFantome.x=positionFantome.x+LARGEUR_CADRE_JEU;
								else if(FX==NB_BLOCS_LARGEUR-1)blitTransFantome.x=positionFantome.x-LARGEUR_CADRE_JEU;

								SDL_BlitSurface(fantomeActuel,NULL,ecranF,&blitTransFantome);

								SDL_BlitSurface(images->vide,NULL,ecranF,&blitBordDroiteF);
								blitBordDroiteF.x+=TAILLE_BLOC;
								SDL_BlitSurface(images->vide,NULL,ecranF,&blitBordDroiteF);
								blitBordDroiteF.x-=TAILLE_BLOC;
								SDL_BlitSurface(images->vide,NULL,ecranF,&blitBordGaucheF);
								blitBordGaucheF.x-=TAILLE_BLOC;
								SDL_BlitSurface(images->vide,NULL,ecranF,&blitBordGaucheF);
								blitBordGaucheF.x+=TAILLE_BLOC;
							}
			}

void Fantome::lancerFantome()
	{
		if(lancer)return;
		lancementEnCours=true;
		if(positionFantome.x!=INIT_FANTOME_X)
			{
						SDL_BlitSurface(images->vide,NULL,ecranF,&positionFantome);
						if(positionFantome.x<INIT_FANTOME_X)positionFantome.x++;
						if(positionFantome.x>INIT_FANTOME_X)positionFantome.x--;
						SDL_BlitSurface(fantome,NULL,ecranF,&positionFantome);
						return;
			}
		if(positionFantome.y!=INIT_FANTOME_Y-2*TAILLE_BLOC)
			{
						SDL_BlitSurface(images->vide,NULL,ecranF,&positionFantome);
						positionFantome.y--;
						SDL_BlitSurface(images->mur,NULL,ecranF,&positionCurseur);
						SDL_BlitSurface(fantomeActuel,NULL,ecranF,&positionFantome);
						return;
			}
		FPixelX=positionFantome.x;
		FPixelY=positionFantome.y;
		FY=10;
		FX=10;
		curseurReBlit.x=positionFantome.x;
		curseurReBlit.y=positionFantome.y;
		lancer=true;
		lancementEnCours=false;
	}

void Fantome::cibleFantome(bool what)
	{
		if(retour)return;
		if(what)
			{
				tempsModeBleuDebut=SDL_GetTicks();
				fantomeActuel=images->fantomePoursuite;
				modeBleu=true;
			}
		else
			{
				modeBleu=false;
				fantomeActuel=fantome;
			}
	}

void Fantome::correctTime(){
		tempsModeBleuDebut+=1100;
}

void Fantome::fantomeBouffe()
	{
		if(retour)return;
		SDL_BlitSurface(images->vide,NULL,ecranF,&positionFantome);
		retour=true;
		modeBleu=false;
		fantomeActuel=fantome;
		niv->StructPerso->rouge->correctTime();
		niv->StructPerso->bleu->correctTime();
		niv->StructPerso->blanc->correctTime();
		niv->StructPerso->rose->correctTime();
		niv->appliquerFantomeBouffe(true, &positionFantome);
	}

void Fantome::timerFantome()
	{
		timer=SDL_AddTimer(intervalleTimer, deplacerFantomeBis, this);
	}

bool Fantome::verifModeBleu()
	{
		return modeBleu;
	}

int Fantome::verifAcceleration()
	{
		if(retour)return 20;
		if(lancementEnCours)return 5;
		if(modeBleu)return intervalleTimer+50;
		return intervalleTimer;
	}
			
Fantome::~Fantome()
	{
		SDL_FreeSurface(fantome);
	}

Uint32 deplacerFantomeBis(Uint32 intervalle, void *parametre)
	{
		Fantome *F=(Fantome*)parametre;
		if( !F->verifPause() ){
				intervalle=F->verifAcceleration();
				F->deplacerFantome();
		}
		return intervalle;
	}
	
