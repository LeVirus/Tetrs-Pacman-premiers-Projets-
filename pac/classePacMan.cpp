#include "classesPM.h"	

using namespace std;

PacMan::PacMan()
	{
		images=NULL;
		ecranP=NULL;
		pha=IMG_Load("Images/pac2B.jpeg");
		phb=IMG_Load("Images/pac3B.jpeg");
		pda=IMG_Load("Images/pac2A.jpeg");
		pdb=IMG_Load("Images/pac3A.jpeg");
		pga=IMG_Load("Images/pac2C.jpeg");
		pgb=IMG_Load("Images/pac3C.jpeg");
		pba=IMG_Load("Images/pac2D.jpeg");
		pbb=IMG_Load("Images/pac3D.jpeg");
		pmC=IMG_Load("Images/pac0.jpeg");
		pmA=pga;
		pmB=pgb;
		pac=pmA;
		nombreVies=NOMBRE_VIES;
		numBlit=0;
		pacX=10;
		pacY=19;
		transition=false;
		deplacementenCours=false;
		activeVerif=true, vertical=false, horizontal=true;
		direction=GAUCHE;
	}

void PacMan::initialiser(SDL_Surface *ecran, Niveau *pointeurN, ImagesCommunes *imagesN)
	{
		pause=false;
		if(!ecranP)ecranP=ecran;
		if(!images)images=imagesN;
		niv=pointeurN;
		continu=true;
		intervalleTimer=100;
		blitBordGauche.y=12*TAILLE_BLOC+LIMITE_ZONE_HAUT;
		blitBordGauche.x=-1*TAILLE_BLOC+LIMITE_ZONE_GAUCHE;

		blitBordDroite.y=12*TAILLE_BLOC+LIMITE_ZONE_HAUT;
		blitBordDroite.x=21*TAILLE_BLOC+LIMITE_ZONE_GAUCHE;

		positionPacMan.y=19*TAILLE_BLOC+LIMITE_ZONE_GAUCHE-10;
		positionPacMan.x=10*TAILLE_BLOC+LIMITE_ZONE_GAUCHE;
		casePrecedante.x=positionPacMan.x;
		casePrecedante.y=positionPacMan.y;
		blitTrans.y=LIMITE_ZONE_HAUT+12*TAILLE_BLOC;
		SDL_BlitSurface(pac,NULL,ecranP,&positionPacMan);
	}

void PacMan::pauseF(){
	pause=!pause;
}

bool PacMan::verifPause(){
	return pause;
}

void PacMan::resetPacMan(){
if(transition)SDL_BlitSurface(images->vide,NULL,ecranP,&blitTrans);
		pacX=10;
		pacY=19;
		direction=GAUCHE;
		if(transition)SDL_BlitSurface(images->vide,NULL,ecranP,&blitTrans);
		transition=false;
		SDL_BlitSurface(images->vide,NULL,ecranP,&positionPacMan);
		switch(reBlitActuel)
			{
				case VIDE:
				break;
				case POINT:
				SDL_BlitSurface(images->point,NULL,ecranP,&positionReBlit);
				break;
				case POINT_SPECIAL:
				SDL_BlitSurface(images->pointSpe,NULL,ecranP,&positionReBlit);
				break;
			}
		positionPacMan.y=INIT_PAC_Y;
		positionPacMan.x=INIT_PAC_X;
		casePrecedante.x=positionPacMan.x;
		casePrecedante.y=positionPacMan.y;
		activeVerif=true;
		pmA=pga;
		pmB=pgb;
		sleep(1);
		SDL_BlitSurface(pac,NULL,ecranP,&positionPacMan);
		sleep(1);
		continu=true;
}

bool PacMan::verifContinu()
		{
			if(!continu){continu=true;return false;}
			return true;
		}

void PacMan::verifCollision(SDL_Rect colFantome, bool modeBleu, Fantome *a, bool transF)
	{
		short c=5,b=25;
		if(modeBleu){c=-1;b=31;}
		if( transition  &&  !transF  ||  transF && !transition )return;
		if(colFantome.y+b<positionPacMan.y+c  ||  colFantome.y+c>positionPacMan.y+b)return;
		else if(!transition  &&  colFantome.x+b<positionPacMan.x+c  ||  colFantome.x+c>positionPacMan.x+b)return;
				if(!modeBleu){
					nombreVies--;
					if(nombreVies==0){niv->resetNiveau(GAME_OVER);continu=false;nombreVies=NOMBRE_VIES;return;}
					niv->resetNiveau(PAC_BOUFFE);
				}
				else a->fantomeBouffe();
	}

int PacMan::IAFantome(int FX, int FY, int dirInt, bool retour, bool modeBleu)
	{
		bool act=true;
		int X, Y,dirA, dirB, dirC, finalDirection, trancheDirection;//dirA= choix direction primaire dirB=choix direction secondaire
		if(!retour){
			X=FX-pacX;//FX FY ==position fantome
			Y=FY-pacY;//pacX pac Y ""		pacman
		}
		else {
			X=FX-10;//direction centre
			Y=FY-12;
			if(FX==10  &&  FY==10)return BAS;
		}
		if(X==0){
			if(!modeBleu)act=false;
			if(Y<0)dirA=BAS;
			else dirA=HAUT;
			dirB=DROITE;
			dirC=GAUCHE;
		}
		else if(Y==0){
			if(!modeBleu)act=false;
			if(X<0)dirA=DROITE;
			else dirA=GAUCHE;
			if(X>12  ||  X<-12){
				if(dirA==GAUCHE)dirA=DROITE;
				if(dirA==DROITE)dirA=GAUCHE;
			}
			dirB=HAUT;
			dirC=BAS;
		}

		else if(X>0&&Y>0)trancheDirection=0;//pacman en haut à gauche 
		else if(X<0&&Y>0)trancheDirection=1;//pacman en haut à droite 
		else if(X<0&&Y<0)trancheDirection=2;//pacman en bas à droite 
		else if(X>0&&Y<0)trancheDirection=3;//pacman en bas à gauche 
		if(modeBleu){
			switch(trancheDirection){
			case 0:
			trancheDirection=2;
			break;
			case 1:
			trancheDirection=3;
			break;
			case 2:
			trancheDirection=0;
			break;
			case 3:
			trancheDirection=1;
			break;
			default:
			break;
			}
		}
		if(X>12  ||  X<-12){
			if(trancheDirection==0)trancheDirection=1;
			if(trancheDirection==1)trancheDirection=0;
			if(trancheDirection==2)trancheDirection=3;
			if(trancheDirection==3)trancheDirection=2;
		}

		if(act){
		if(X<0)X+=2*X;
		if(Y<0)Y+=2*Y;
		switch(trancheDirection)
			{
				case 0:
				if(X>Y){dirA=GAUCHE;dirB=HAUT;}
				else {dirA=HAUT;dirB=GAUCHE;}
				dirC=BAS;
				break;

				case 1:
				if(X>Y){dirA=DROITE;dirB=HAUT;}
				else {dirA=HAUT;dirB=DROITE;}
				dirC=GAUCHE;
				break;

				case 2:
				if(X>Y){dirA=DROITE;dirB=BAS;}
				else {dirA=BAS;dirB=DROITE;}
				dirC=HAUT;
				break;

				case 3:
				if(X>Y){dirA=GAUCHE;dirB=BAS;}
				else {dirA=BAS;dirB=GAUCHE;}
				dirC=DROITE;
				break;
			}
		}
		if(dirA!=dirInt  &&  niv->verifMatrice(FX, FY, dirA))return dirA;//direction la plus favorable
		else if(dirB!=dirInt  &&  niv->verifMatrice(FX, FY, dirB))return dirB;//sinon la 2eme direction plus favorable
		else if(dirC!=dirInt  &&  niv->verifMatrice(FX, FY, dirC))return dirC;//sinon
		else 
			for(short i=0;i<4;i++)
				if(dirA!=i&&dirB!=i  &&  niv->verifMatrice(FX, FY, i))return i;//sion 3eme direction autre que dirA et dirB
			return -1;
	}

void PacMan::deplacerPacMan(SDL_Event *event, Niveau *n)
	{
		deplacementenCours=true;
				switch(event->type)
					{	
						case SDL_KEYDOWN:
							switch(event->key.keysym.sym)
								{
									case SDLK_LEFT:	
									if(horizontal||activeVerif  &&  n->verifMatrice(pacX,pacY,GAUCHE)){
									direction=GAUCHE;
									pmA=pga;
									pmB=pgb;
									}
									break;

									case SDLK_RIGHT:
									if(horizontal||activeVerif  &&  n->verifMatrice(pacX,pacY,DROITE)){
									direction=DROITE;
									pmA=pda;
									pmB=pdb;
									}
									break;

									case SDLK_DOWN:	
									if(vertical||activeVerif  &&  n->verifMatrice(pacX,pacY,BAS)){
									direction=BAS;
									pmA=pba;
									pmB=pbb;
									}
									break;

									case SDLK_UP:	
									if(vertical||activeVerif  &&  n->verifMatrice(pacX,pacY,HAUT)){
									direction=HAUT;
									pmA=pha;
									pmB=phb;
									}
									break;

									default:
									break;
								}
													
						break;
					}
						SDL_BlitSurface(images->vide,NULL,ecranP,&positionPacMan);
						switch(reBlitActuel)
							{
								case VIDE:
								break;
								case POINT:
								SDL_BlitSurface(images->point,NULL,ecranP,&positionReBlit);
								break;
								case POINT_SPECIAL:
								SDL_BlitSurface(images->pointSpe,NULL,ecranP,&positionReBlit);
								break;
								case CERISE:
								SDL_BlitSurface(images->cerise,NULL,ecranP,&positionReBlit);
								break;
							}
						switch(direction)
							{//___________________________________________________________________________________________
								case GAUCHE:
								if(activeVerif  &&  !n->verifMatrice(pacX,pacY,GAUCHE)){immobile=true;break;}
								else 
									{
										immobile=false;
										positionPacMan.x-=15;
										vertical=false; horizontal=true;
										activeVerif=false; 
										if(positionPacMan.x==casePrecedante.x)activeVerif=true;
									}

								if(positionPacMan.x+30==casePrecedante.x)
									{
										pacX--; 
										casePrecedante.x=positionPacMan.x; 
										activeVerif=true;	
										n->verifPoints(pacX, pacY, continu);	
									}
								if(pacX==-1  && pacY==12)
									{
										positionPacMan.x=LIMITE_ZONE_DROITE-30;
										casePrecedante.x=positionPacMan.x;
										pacX=20;
										transition=false;
									}

								if(positionPacMan.x<=casePrecedante.x  &&  positionReBlit.x!=casePrecedante.x-30)
									{	
										positionReBlit.x=casePrecedante.x-30;
										positionReBlit.y=positionPacMan.y;
										reBlitActuel=niv->numMatrice(pacX-1, pacY);
										if(transition){transition=false; SDL_BlitSurface(images->vide,NULL,ecranP,&blitTrans);}
									}
								break;
//________________________________________________________________________________________________________
								case DROITE:
								if(activeVerif  &&  !n->verifMatrice(pacX,pacY,DROITE)){immobile=true;break;}
								else 
									{
										immobile=false;
										positionPacMan.x+=15;
										vertical=false; horizontal=true;
										activeVerif=false; 
										if(positionPacMan.x==casePrecedante.x)activeVerif=true;
									}	

								if(positionPacMan.x-30==casePrecedante.x)
									{
										pacX++;
										casePrecedante.x=positionPacMan.x; 
										activeVerif=true;	
										n->verifPoints(pacX, pacY, continu);	
									}

								if(pacX==21  && pacY==12)
									{
										positionPacMan.x=LIMITE_ZONE_GAUCHE;
										casePrecedante.x=positionPacMan.x;
										pacX=0;
										transition=false;
									}

								if(positionPacMan.x>=casePrecedante.x  &&  positionReBlit.x!=casePrecedante.x+30)
									{	
										positionReBlit.x=casePrecedante.x+30;
										positionReBlit.y=positionPacMan.y;
										reBlitActuel=niv->numMatrice(pacX+1, pacY);
										if(transition){transition=false; SDL_BlitSurface(images->vide,NULL,ecranP,&blitTrans);}
									}
								break;
//________________________________________________________________________________________________________
								case BAS:
								if(activeVerif  &&  !n->verifMatrice(pacX,pacY,BAS)){immobile=true;break;}
								else 
									{
										immobile=false;
										positionPacMan.y+=15;
										vertical=true; horizontal=false;
										activeVerif=false;
										if(positionPacMan.y==casePrecedante.y)activeVerif=true;
									}	
								if(positionPacMan.y-30==casePrecedante.y)
									{
										pacY++;
										casePrecedante.y=positionPacMan.y; 
										activeVerif=true;	
										n->verifPoints(pacX, pacY, continu);
									}

								if(positionPacMan.y>=casePrecedante.y  &&  positionReBlit.y!=casePrecedante.y+30)
									{	
										positionReBlit.y=casePrecedante.y+30;
										positionReBlit.x=positionPacMan.x;
										reBlitActuel=niv->numMatrice(pacX, pacY+1);
									}
								break;
//________________________________________________________________________________________________________
								case HAUT:
								if(activeVerif  &&  !n->verifMatrice(pacX,pacY,HAUT)){immobile=true;break;}
								else 
									{
										immobile=false;
										positionPacMan.y-=15;
										vertical=true; horizontal=false;
										activeVerif=false;
										if(positionPacMan.y==casePrecedante.y)activeVerif=true;
									}	
								if(positionPacMan.y+30==casePrecedante.y)
									{
										pacY--;
										casePrecedante.y=positionPacMan.y; 
										activeVerif=true;	
										n->verifPoints(pacX, pacY, continu);
									}

								if(positionPacMan.y<=casePrecedante.y  &&  positionReBlit.y!=casePrecedante.y-30)
									{	
										positionReBlit.y=casePrecedante.y-30;
										positionReBlit.x=positionPacMan.x;
										reBlitActuel=niv->numMatrice(pacX, pacY-1);
									}
								break;
	
//_________________________________________________________________________________________________________
								default:
								break;
							}
						switch(reBlitActuel)
							{
								case VIDE:
								break;
								case POINT:
								SDL_BlitSurface(images->point,NULL,ecranP,&positionReBlit);
								break;
								case POINT_SPECIAL:
								SDL_BlitSurface(images->pointSpe,NULL,ecranP,&positionReBlit);
							}
						if(direction==GAUCHE  &&  pacY==12  &&  pacX==0  ||  direction==DROITE  &&  pacY==12  &&  pacX==NB_BLOCS_LARGEUR-1)
							{
								transition=true;	
								activeVerif=false;
							}
						SDL_BlitSurface(pac,NULL,ecranP,&positionPacMan);
						if(!immobile)
						switch(numBlit)
							{
								case 0:
								numBlit=1;
								pac=pmB;
								break;
								case 1:
								numBlit=2;
								pac=pmC;
								break;
								case 2:
								numBlit=0;
								pac=pmA;
								break;
								default:
								break;
							}
						else pac=pmA;
						if(transition)
							{
								
										if(pacX==0)
											{
												blitTrans.x=positionPacMan.x+LARGEUR_CADRE_JEU;
												blitBordDroite.x-=TAILLE_BLOC;
												SDL_BlitSurface(images->vide,NULL,ecranP,&blitBordDroite);
												blitBordDroite.x+=TAILLE_BLOC;
											}
										else if(pacX==NB_BLOCS_LARGEUR-1)
											{
												blitTrans.x=positionPacMan.x-LARGEUR_CADRE_JEU;
												blitBordGauche.x+=TAILLE_BLOC;
												SDL_BlitSurface(images->vide,NULL,ecranP,&blitBordGauche);
												blitBordGauche.x-=TAILLE_BLOC;
											}
								SDL_BlitSurface(pac,NULL,ecranP,&blitTrans);

								SDL_BlitSurface(images->vide,NULL,ecranP,&blitBordDroite);
								blitBordDroite.x+=TAILLE_BLOC;
								SDL_BlitSurface(images->vide,NULL,ecranP,&blitBordDroite);
								blitBordDroite.x-=TAILLE_BLOC;
								SDL_BlitSurface(images->vide,NULL,ecranP,&blitBordGauche);
								blitBordGauche.x-=TAILLE_BLOC;
								SDL_BlitSurface(images->vide,NULL,ecranP,&blitBordGauche);
								blitBordGauche.x+=TAILLE_BLOC;
							}
			}

void PacMan::timerPac(StructTimerPac *pointeurVoid)
	{
		timerP=SDL_AddTimer(intervalleTimer, deplacerPacManBis, pointeurVoid);
	}

Uint32 deplacerPacManBis(Uint32 intervalle, void *parametre)
	{
		StructTimerPac *P=(StructTimerPac*)parametre;
		if( !P->Pac->verifPause() )P->Pac->deplacerPacMan(P->event, P->n);
		return intervalle;
	}
			

PacMan::~PacMan()
	{
		SDL_FreeSurface(pmC);
		SDL_FreeSurface(pha);
		SDL_FreeSurface(phb);
		SDL_FreeSurface(pba);
		SDL_FreeSurface(pbb);
		SDL_FreeSurface(pda);
		SDL_FreeSurface(pdb);
		SDL_FreeSurface(pga);
		SDL_FreeSurface(pgb);
	}
