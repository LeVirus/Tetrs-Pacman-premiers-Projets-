#include "classes.h"
//#include "constante.h"
//#include <iostream>

using namespace std;

Uint32 deplacerTetraBasBis(Uint32 intervalle, void *parametre);

//CPP_ZONEJEU=============================================================================
zoneJeu::zoneJeu()
	{
		matrice= new elementTableau*[NB_BLOCS_HAUTEUR];
		for(int t=0;t<NB_BLOCS_HAUTEUR;t++)matrice[t]=new elementTableau[NB_BLOCS_LARGEUR];
		for(int y=0;y<NB_BLOCS_HAUTEUR;y++)
			for(int x=0;x<NB_BLOCS_LARGEUR;x++)
				{
					if(y==NB_BLOCS_HAUTEUR-1||x==0||x==NB_BLOCS_LARGEUR-1)matrice[y][x].etat=LIMITE;
					else matrice[y][x].etat=VIDE;
					matrice[y][x].image=CARRE_O;
				}
		cubeA=IMG_Load("Images/cubeA.png");
		cubeB=IMG_Load("Images/cubeB.jpg");
		cubeC=IMG_Load("Images/cubeC.jpg");
		cubeD=IMG_Load("Images/cubeD.jpg");
		cubeE=IMG_Load("Images/cubeE.jpg");
		cubeF=IMG_Load("Images/cubeF.png");
		cubeG=IMG_Load("Images/cubeG.png");
		vide= SDL_CreateRGBSurface(SDL_HWSURFACE, 25, 25, 32, 0, 0, 0, 0);
		SDL_FillRect(vide, NULL, SDL_MapRGB(vide->format, 0, 0, 0));
		score=0;
		HScore=10000;
		intervalleTimer=1000;
		changerVitesse=4000;
		deplacementEnCour=false; 
		finJeu=false;		
		blitFin=false;	

		
		FMOD_System_Create(&system);
		FMOD_System_Init(system, 2, FMOD_INIT_NORMAL, NULL);
		FMOD_System_CreateSound(system, "Musiques/pacLigne.wav", FMOD_CREATESAMPLE, 0, &ligneSon);
		FMOD_System_CreateSound(system, "Musiques/pacVitesse.mp3", FMOD_CREATESAMPLE, 0, &tempsSon);
		FMOD_System_CreateSound(system, "Musiques/FEZ.mp3", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM|FMOD_LOOP_NORMAL, 0, &musique);
		FMOD_Sound_SetLoopCount(musique, -1);
	}

void zoneJeu::debug()
	{
		 finJeu=false;	
	}	

void zoneJeu::afficheMatrice()
	{
		for(int y=0;y<NB_BLOCS_HAUTEUR;y++)
			for(int x=0;x<NB_BLOCS_LARGEUR;x++)
				{
					cout<<matrice[y][x].etat<<" ";
					if(x==11)cout<<endl;
				}
		cout<<"FIN AFFICHAGE"<<endl;
		for(int yy=0;yy<NB_BLOCS_HAUTEUR;yy++)
			for(int xx=0;xx<NB_BLOCS_LARGEUR;xx++)
				{
					cout<<matrice[yy][xx].image<<" ";
					if(xx==11)cout<<endl;
				}
		cout<<"FIN AFFICHAGE"<<endl;
	}

void zoneJeu::rebootMatrice()
	{
					for(int t=0;t<NB_BLOCS_HAUTEUR;t++)matrice[t]=new elementTableau[NB_BLOCS_LARGEUR];
					for(int y=0;y<NB_BLOCS_HAUTEUR;y++)
						for(int x=0;x<NB_BLOCS_LARGEUR;x++)
							{
								if(y==NB_BLOCS_HAUTEUR-1||x==0||x==NB_BLOCS_LARGEUR-1)matrice[y][x].etat=LIMITE;
								else matrice[y][x].etat=VIDE;
								matrice[y][x].image=CARRE_O;
							} 		
	}

void zoneJeu::verifLignes(SDL_Surface *ecran, int YMAX, int YMIN)
	{
		int compteur[4],nbrLigneSupr=0,niv=0;
		for(int i=YMIN;i<=YMAX;i++)
			for(int j=1;j<NB_BLOCS_LARGEUR-1;j++)
				{
					if(matrice[i][j].etat==VIDE)break;
					if(j==10&&matrice[i][j].etat==CARRE_FIXE)
						{
							FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, ligneSon, 0, NULL);
							compteur[nbrLigneSupr]=i;nbrLigneSupr++;
							positionCurseur.x=LIMITE_ZONE_GAUCHE;
							positionCurseur.y=LIMITE_ZONE_HAUT+i*TAILLE_BLOC;
							for(int x=1;x<NB_BLOCS_LARGEUR-1;x++)
								{
									matrice[i][x].etat=VIDE;
									matrice[i][x].image=CARRE_O;
									SDL_BlitSurface(vide,NULL,ecran,&positionCurseur);
									positionCurseur.x+=TAILLE_BLOC;
								}
						}
				}
			if(nbrLigneSupr>0)
				{
					nbrLigneSupr--;
					int yRecept=1;
					bool yyyStay=false;
					for(int yyy=compteur[nbrLigneSupr];yyy-yRecept>=0;yyy--)
							{
								if(yyyStay){yyy++;yyyStay=false;}
								if((nbrLigneSupr-yRecept)>=0	&&	yyy-yRecept==compteur[nbrLigneSupr-yRecept]){yRecept++;yyyStay=true;}
								positionCurseur.y=LIMITE_ZONE_HAUT+yyy*TAILLE_BLOC;	
								positionCurseur.x=LIMITE_ZONE_GAUCHE;
								posX=LIMITE_ZONE_GAUCHE;
								posY=positionCurseur.y-TAILLE_BLOC*yRecept;

								for(int xxx=1;xxx<NB_BLOCS_LARGEUR-1;xxx++)
									{
								if(matrice[yyy-yRecept][xxx].etat==CARRE_FIXE)deplacerElement(matrice[yyy-yRecept][xxx], matrice[yyy][xxx],ecran, AUTRE);
										positionCurseur.y=LIMITE_ZONE_HAUT+yyy*TAILLE_BLOC;SDL_Flip(ecran);
										positionCurseur.x+=TAILLE_BLOC;
										posX+=TAILLE_BLOC;					
									}
							}
					nbrLigneSupr++;
					score+=nbrLigneSupr*100;
					if(nbrLigneSupr==4)score+=600;
					if(nbrLigneSupr==3)score+=200;
					if(nbrLigneSupr==2)score+=100;
					ecrireScore(ecran);
				}
	}

void zoneJeu::ecrireScore(SDL_Surface *ecran)
	{
		SDL_Surface *viderScore=NULL;
		SDL_Color couleurBlanche = {255, 255, 255};
		TTF_Font *police = TTF_OpenFont("Children.ttf", 30);
		std::ostringstream oss;
		string recepteur;
		bool H=false,debut=true;
		do
			{
				if(H&&score!=0)HScore=score;
				if(H&&score==0)oss << HScore;
				else oss << score;
				recepteur = oss.str();
				oss.str("");
				size_t size = recepteur.size() + 1;
				char * t = new char[size];
				strncpy( t, recepteur.c_str(), size );
				ScoreImage = TTF_RenderText_Solid(police, t, couleurBlanche);
				delete t;
				positionCurseur.y=ZONE_SCORE_Y+5;
				if(H)positionCurseur.y+=ScoreImage->h;
				positionCurseur.x=ZONE_SCORE_X-ScoreImage->w;
				viderScore= SDL_CreateRGBSurface(SDL_HWSURFACE, ScoreImage->w+4, ScoreImage->h, 32, 0, 0, 0, 0);
				SDL_FillRect(viderScore, NULL, SDL_MapRGB(vide->format, 0, 0, 0));
								positionCurseur.x-=4;
								SDL_BlitSurface(viderScore,NULL,ecran,&positionCurseur);
								positionCurseur.x+=4;
								SDL_BlitSurface(ScoreImage,NULL,ecran,&positionCurseur);
								SDL_FreeSurface(ScoreImage);
								SDL_FreeSurface(viderScore);
				if(H){H=false;break;}
				if(score==0||score>HScore)H=true;
			}while(H); 
				if(intervalleTimer>200&&score>=changerVitesse)
						{	
							FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, tempsSon, 0, NULL);
							intervalleTimer-=200; changerVitesse+=5000;		
						}
						SDL_Flip(ecran);
	}

int zoneJeu::verifAcceleration()
	{
		return intervalleTimer;
	}

void zoneJeu::generateurTetra(SDL_Surface *ecran)
	{
		int x=5, y=0,image=0;
		positionCurseur.x=LIMITE_ZONE_GAUCHE+TAILLE_BLOC*(x-1);
		positionCurseur.y=LIMITE_ZONE_HAUT;
		SDL_Surface *pointSurface=NULL;
		TetraActuel=0;
		srand(time(NULL));
		TetraActuel=rand()%7;
		switch(TetraActuel)
			{
				case 0:
				pointSurface=cubeA;
				image=CARRE_A;
				break;

				case 1:
				pointSurface=cubeB;
				image=CARRE_B;
				break;

				case 2:
				pointSurface=cubeC;
				image=CARRE_C;
				break;

				case 3:
				pointSurface=cubeD;
				image=CARRE_D;
				break;

				case 4:
				pointSurface=cubeE;
				image=CARRE_E;
				break;

				case 5:
				pointSurface=cubeF;
				image=CARRE_F;
				break;

				case 6:
				pointSurface=cubeG;
				image=CARRE_G;
				break;

			}
		for(int c=0;c<4;c++)
			{
				if(c==0&&(TetraActuel==4||TetraActuel==6)){positionCurseur.y+=TAILLE_BLOC;y++;}

				if(c==1){positionCurseur.x+=TAILLE_BLOC;x++;}

				if(c==2&&(TetraActuel==0||TetraActuel==2||TetraActuel==3||TetraActuel==5)){positionCurseur.y+=TAILLE_BLOC;y++;}
				if(c==2&&(TetraActuel==1||TetraActuel==6)){positionCurseur.x+=TAILLE_BLOC;x++;}
				if(c==2&&TetraActuel==3){positionCurseur.x-=TAILLE_BLOC;x--;}
				if(c==2&&TetraActuel==4){positionCurseur.y-=TAILLE_BLOC;y--;}

				if(c==3&&(TetraActuel==0||TetraActuel==1||TetraActuel==4)){positionCurseur.x+=TAILLE_BLOC;x++;}
				if(c==3&&(TetraActuel==2||TetraActuel==6)){positionCurseur.x-=TAILLE_BLOC;x--;}
				if(c==3&&(TetraActuel==3||TetraActuel==5)){positionCurseur.y+=TAILLE_BLOC;y++;}
				if(c==3&&TetraActuel==6){positionCurseur.y-=TAILLE_BLOC;y--;}

				SDL_BlitSurface(pointSurface,NULL,ecran,&positionCurseur);
				if(matrice[y][x].etat==CARRE_FIXE)
				{
					blitFin=true;
					finJeu=true;
					positionCurseur.x=LIMITE_ZONE_GAUCHE;positionCurseur.y=LIMITE_ZONE_HAUT;
					for(int y=0;y<NB_BLOCS_HAUTEUR-1;y++)
						for(int x=0;x<NB_BLOCS_LARGEUR-2;x++)
							{
								SDL_BlitSurface(cubeA,NULL,ecran,&positionCurseur);
								positionCurseur.x=TAILLE_BLOC*x+LIMITE_ZONE_GAUCHE;
								positionCurseur.y=TAILLE_BLOC*y+LIMITE_ZONE_HAUT;
								SDL_Flip(ecran);
							}
					blitFin=false;
					return;
				}
				matrice[y][x].etat=CARRE_MOUVANT;
				matrice[y][x].image=image;
			}
		SDL_Flip(ecran);
	}

bool zoneJeu::verifGameOver()
	{
		if(!finJeu)return true;
		score=0;
		changerVitesse=4000;
		intervalleTimer=1000;
		return false;
	}

void zoneJeu::descendreCubeTimer(SDL_Surface *ecran)
	{
		paramTimer *z;
		z=new paramTimer;
		FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, musique, 0, NULL);
		z->ecranC=ecran;
		z->pointClass=this;
		timer=SDL_AddTimer(intervalleTimer, deplacerTetraBasBis, z);
	}

void zoneJeu::deplacerElement(elementTableau& emmeteur, elementTableau& recepteur, SDL_Surface *ecran, int direction)
	{
		switch(emmeteur.image)	
			{
				case CARRE_O:
				SDL_BlitSurface(vide,NULL,ecran,&positionCurseur);
				break;
				case CARRE_A:
				SDL_BlitSurface(cubeA,NULL,ecran,&positionCurseur);
				break;
				case CARRE_B:
				SDL_BlitSurface(cubeB,NULL,ecran,&positionCurseur);
				break;
				case CARRE_C:
				SDL_BlitSurface(cubeC,NULL,ecran,&positionCurseur);
				break;
				case CARRE_D:
				SDL_BlitSurface(cubeD,NULL,ecran,&positionCurseur);
				break;
				case CARRE_E:
				SDL_BlitSurface(cubeE,NULL,ecran,&positionCurseur);
				break;
				case CARRE_F:
				SDL_BlitSurface(cubeF,NULL,ecran,&positionCurseur);
				break;
				case CARRE_G:
				SDL_BlitSurface(cubeG,NULL,ecran,&positionCurseur);
				break;
			}
		if(direction==BAS)positionCurseur.y-=TAILLE_BLOC;
		else if(direction==GAUCHE)positionCurseur.x+=TAILLE_BLOC;
		else if(direction==DROITE)positionCurseur.x-=TAILLE_BLOC;
		else if(direction==AUTRE) {positionCurseur.x=posX;positionCurseur.y=posY;}
		SDL_BlitSurface(vide,NULL,ecran,&positionCurseur);
		recepteur.etat=emmeteur.etat;recepteur.image=emmeteur.image;
		emmeteur.etat=VIDE;
		emmeteur.image=CARRE_O;
	}

void zoneJeu::deplacerTetra(int direction, SDL_Surface *ecran)
	{
		if(deplacementEnCour||finJeu||blitFin)return;
		deplacementEnCour=true;
		int compteur=0;
		bool limite=false;
		int X[4],Y[4];
		for(int y=0;y<NB_BLOCS_HAUTEUR-1;y++)
			for(int x=1;x<NB_BLOCS_LARGEUR-1;x++)
				{
					if(matrice[y][x].etat==CARRE_MOUVANT)
						{
							switch(direction)
								{
									case BAS:
									if(matrice[y+1][x].etat==LIMITE||matrice[y+1][x].etat==CARRE_FIXE)limite=true;
									break;

									case DROITE:
									if(matrice[y][x+1].etat==LIMITE||matrice[y][x+1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
									break;

									case GAUCHE:
									if(matrice[y][x-1].etat==LIMITE||matrice[y][x-1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
									break;
								}
							X[compteur]=x;
							Y[compteur]=y;
							compteur++;
							if(compteur>=4)break;
						}	
					if(compteur>=4)break;
				}
			if(direction!=GAUCHE)
				{
					for(compteur=3;compteur>=0;compteur--)
						{
									positionCurseur.x=LIMITE_ZONE_GAUCHE+(X[compteur]-1)*TAILLE_BLOC;
									positionCurseur.y=LIMITE_ZONE_HAUT+Y[compteur]*TAILLE_BLOC;
									switch(direction)
											{
												case BAS:
												if(limite)matrice[Y[compteur]][X[compteur]].etat=CARRE_FIXE;
												else
													{
														positionCurseur.y+=TAILLE_BLOC;
														deplacerElement(matrice[Y[compteur]][X[compteur]],matrice[Y[compteur]+1][X[compteur]],ecran,BAS);
													}
												break;

												case DROITE:
												positionCurseur.x+=TAILLE_BLOC;
												deplacerElement(matrice[Y[compteur]][X[compteur]],matrice[Y[compteur]][X[compteur]+1],ecran,DROITE);
												break;
											}
						}
					if(limite){verifLignes(ecran,Y[3], Y[0]); if(!finJeu)generateurTetra(ecran);}
					}

				else
					{
						for(compteur=0;compteur<4;compteur++)
											{
														positionCurseur.x=LIMITE_ZONE_GAUCHE+(X[compteur]-1)*TAILLE_BLOC;
														positionCurseur.y=LIMITE_ZONE_HAUT+Y[compteur]*TAILLE_BLOC;
														switch(direction)
																{
																	case GAUCHE:
																	positionCurseur.x-=TAILLE_BLOC;
																	deplacerElement(matrice[Y[compteur]][X[compteur]],matrice[Y[compteur]][X[compteur]-1],ecran,GAUCHE);
																	break;
																}
											}
					}
		SDL_Flip(ecran);
		deplacementEnCour=false;
	}

void zoneJeu::pivoterTetra(SDL_Surface *ecran, bool trigo)
	{
		if(deplacementEnCour||finJeu||blitFin){deplacementEnCour=false;return;}
		deplacementEnCour=true;
		if(TetraActuel==2){deplacementEnCour=false;return;}
		int compteur=0,xActu,yActu,v;
		bool limite=false;
		int X[4],Y[4], xMultiPosition[4], yMultiPosition[4];
		for(int y=0;y<NB_BLOCS_HAUTEUR-1;y++)
			for(int x=1;x<NB_BLOCS_LARGEUR-1;x++)
				{
					if(matrice[y][x].etat==CARRE_MOUVANT)
						{
							X[compteur]=x;
							Y[compteur]=y;
							compteur++;
							if(compteur>=4)break;
						}	
					if(compteur>=4)break;
				}
		switch(TetraActuel)
			{
				case 0:
				if(Y[0]==Y[1])
					{
						if(Y[0]-1<0||matrice[Y[0]-1][X[0]+1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
						if(matrice[Y[3]][X[3]-2].etat==CARRE_FIXE||matrice[Y[3]][X[3]-2].etat==LIMITE){deplacementEnCour=false;return;}

						for(v=0;v<4;v++)
							{
								if(v==0){xActu=1;yActu=-1;}
								else if(v==2){xActu=-1;yActu=-1;}
								else if(v==3){xActu=-2;yActu=0;}
								if(v!=1)
									{
										positionCurseur.x=LIMITE_ZONE_GAUCHE+(X[v]+xActu-1)*TAILLE_BLOC;
										positionCurseur.y=LIMITE_ZONE_HAUT+(Y[v]+yActu)*TAILLE_BLOC;
										posX=LIMITE_ZONE_GAUCHE+(X[v]-1)*TAILLE_BLOC;
										posY=LIMITE_ZONE_HAUT+(Y[v])*TAILLE_BLOC;
										deplacerElement(matrice[Y[v]][X[v]],matrice[Y[v]+yActu][X[v]+xActu],ecran,AUTRE);
									}
							}
					}
				else
					{
						if(matrice[Y[1]+1][X[1]+1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
						if(matrice[Y[3]][X[3]+2].etat==CARRE_FIXE){deplacementEnCour=false;return;}

						for(v=3;v>=0;v--)
													{
														if(v==0){xActu=-1;yActu=1;}
														else if(v==1){xActu=1;yActu=1;}
														else if(v==3){xActu=2;yActu=0;}
														if(v!=2)
															{
																positionCurseur.x=LIMITE_ZONE_GAUCHE+(X[v]+xActu-1)*TAILLE_BLOC;
																positionCurseur.y=LIMITE_ZONE_HAUT+(Y[v]+yActu)*TAILLE_BLOC;
																posX=LIMITE_ZONE_GAUCHE+(X[v]-1)*TAILLE_BLOC;
																posY=LIMITE_ZONE_HAUT+(Y[v])*TAILLE_BLOC;
																deplacerElement(matrice[Y[v]][X[v]],matrice[Y[v]+yActu][X[v]+xActu],ecran,AUTRE);
															}
													}
					}
				break;//000000000000000000000000000000000000000000000000000000000000000000000000000000000000000

				case 1:
				if(Y[0]==Y[1])
					{
						if(Y[0]-1<0||matrice[Y[0]-1][X[0]+1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
						if(matrice[Y[2]+1][X[2]-1].etat==CARRE_FIXE||matrice[Y[2]+1][X[2]-1].etat==LIMITE){deplacementEnCour=false;return;}
						if(matrice[Y[3]+2][X[3]-2].etat==CARRE_FIXE||matrice[Y[3]+2][X[3]-2].etat==LIMITE){deplacementEnCour=false;return;}
					}
				else
					{
						if(matrice[Y[0]+1][X[0]-1].etat==CARRE_FIXE||matrice[Y[0]+1][X[0]-1].etat==LIMITE){deplacementEnCour=false;return;}
						if(matrice[Y[2]-1][X[2]+1].etat==CARRE_FIXE||matrice[Y[2]-1][X[2]+1].etat==LIMITE){deplacementEnCour=false;return;}
						if(matrice[Y[3]-2][X[3]+2].etat==CARRE_FIXE||matrice[Y[3]-2][X[3]+2].etat==LIMITE){deplacementEnCour=false;return;}
					}


						for(v=0;v<4;v++)
													{
														if(Y[0]==Y[1])
															{
																if(v==0){xActu=1;yActu=-1;}
																else if(v==2){xActu=-1;yActu=1;}
																else if(v==3){xActu=-2;yActu=2;}
															}
														else
															{
																if(v==0){xActu=-1;yActu=1;}
																else if(v==2){xActu=1;yActu=-1;}
																else if(v==3){xActu=2;yActu=-2;}
															}
														if(v!=1)
															{
																positionCurseur.x=LIMITE_ZONE_GAUCHE+(X[v]+xActu-1)*TAILLE_BLOC;
																positionCurseur.y=LIMITE_ZONE_HAUT+(Y[v]+yActu)*TAILLE_BLOC;
																posX=LIMITE_ZONE_GAUCHE+(X[v]-1)*TAILLE_BLOC;
																posY=LIMITE_ZONE_HAUT+(Y[v])*TAILLE_BLOC;
																deplacerElement(matrice[Y[v]][X[v]],matrice[Y[v]+yActu][X[v]+xActu],ecran,AUTRE);
															}
													}
				break;//1111111111111111111111111111111111111111111111111111111111111111111111111


				case 3:
				if(X[0]==X[3]&&X[0]==X[2])
					{
						if(!trigo)
							{
								if(matrice[Y[2]][X[2]+1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								if(matrice[Y[3]][X[3]+1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								if(matrice[Y[2]][X[2]-1].etat==CARRE_FIXE||matrice[Y[2]][X[2]-1].etat==LIMITE){deplacementEnCour=false;return;}
								xMultiPosition[0]=1;yMultiPosition[0]=1;
								xMultiPosition[1]=0;yMultiPosition[1]=2;
								xMultiPosition[3]=-1;yMultiPosition[3]=-1;
							}
						else
							{
								if(matrice[Y[2]][X[2]-1].etat==CARRE_FIXE||matrice[Y[2]][X[2]-1].etat==LIMITE){deplacementEnCour=false;return;}
								if(matrice[Y[0]][X[0]-1].etat==CARRE_FIXE||matrice[Y[0]][X[0]-1].etat==LIMITE){deplacementEnCour=false;return;}
								if(matrice[Y[0]][X[0]+1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								xMultiPosition[0]=-1;yMultiPosition[0]=1;
								xMultiPosition[1]=-2;yMultiPosition[1]=0;
								xMultiPosition[3]=1;yMultiPosition[3]=-1;
							}
					}

				else if(Y[0]==Y[1]&&Y[0]==Y[2])
					{
						if(!trigo)
							{
								if(matrice[Y[2]+1][X[2]-1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								if(matrice[Y[3]][X[3]-2].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								if(Y[0]-1<0||matrice[Y[0]-1][X[0]+1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								xMultiPosition[0]=1;yMultiPosition[0]=-1;
								xMultiPosition[2]=-1;yMultiPosition[2]=1;
								xMultiPosition[3]=-2;yMultiPosition[3]=0;
							}
						else
							{
								if(Y[2]-1<0||matrice[Y[2]-1][X[2]-1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								if(matrice[Y[3]-2][X[3]].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								if(matrice[Y[0]+1][X[0]+1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								xMultiPosition[0]=1;yMultiPosition[0]=1;
								xMultiPosition[2]=-1;yMultiPosition[2]=-1;
								xMultiPosition[3]=0;yMultiPosition[3]=-2;
							}

					}

				else if(X[0]==X[1]&&X[0]==X[3])
					{
						if(!trigo)
							{
								if(matrice[Y[2]-2][X[2]].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								if(matrice[Y[3]-1][X[3]-1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								if(matrice[Y[0]+1][X[0]+1].etat==CARRE_FIXE||matrice[Y[0]+1][X[0]+1].etat==LIMITE){deplacementEnCour=false;return;}
								xMultiPosition[0]=1;yMultiPosition[0]=1;
								xMultiPosition[2]=0;yMultiPosition[2]=-2;
								xMultiPosition[3]=-1;yMultiPosition[3]=-1;
							}
						else
							{
								if(matrice[Y[2]][X[2]+2].etat==CARRE_FIXE||matrice[Y[2]][X[2]+2].etat==LIMITE){deplacementEnCour=false;return;}
								if(matrice[Y[3]-1][X[3]+1].etat==CARRE_FIXE||matrice[Y[3]-1][X[3]+1].etat==LIMITE){deplacementEnCour=false;return;}
								if(matrice[Y[0]+1][X[0]-1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								xMultiPosition[0]=-1;yMultiPosition[0]=1;
								xMultiPosition[2]=2;yMultiPosition[2]=0;
								xMultiPosition[3]=1;yMultiPosition[3]=-1;
							}

					}

				else if(Y[1]==Y[2]&&Y[1]==Y[3])
					{
						if(!trigo)
							{
								if(matrice[Y[0]][X[0]+2].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								if(matrice[Y[1]-1][X[1]+1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								if(matrice[Y[3]+1][X[3]-1].etat==CARRE_FIXE||matrice[Y[3]+1][X[3]-1].etat==LIMITE){deplacementEnCour=false;return;}
								xMultiPosition[0]=2;yMultiPosition[0]=0;
								xMultiPosition[1]=1;yMultiPosition[1]=-1;
								xMultiPosition[3]=-1;yMultiPosition[3]=1;
							}
						else
							{
								if(matrice[Y[0]+2][X[0]].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								if(matrice[Y[1]+1][X[1]+1].etat==CARRE_FIXE||matrice[Y[1]+1][X[1]+1].etat==LIMITE){deplacementEnCour=false;return;}
								if(matrice[Y[3]-1][X[3]-1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								xMultiPosition[0]=0;yMultiPosition[0]=2;
								xMultiPosition[1]=1;yMultiPosition[1]=1;
								xMultiPosition[3]=-1;yMultiPosition[3]=-1;
							}

					}
				for(v=0;v<4;v++)
					{
						if( !(v==1&& ( (Y[0]==Y[1]&&Y[0]==Y[2]) || (X[0]==X[3]&&X[0]==X[1]) ) ) )
						if( !(v==2&& ( (Y[1]==Y[2]&&Y[1]==Y[3]) || (X[0]==X[3]&&X[0]==X[2]) ) ) )
							{
								positionCurseur.x=LIMITE_ZONE_GAUCHE+(X[v]+xMultiPosition[v]-1)*TAILLE_BLOC;
								positionCurseur.y=LIMITE_ZONE_HAUT+(Y[v]+yMultiPosition[v])*TAILLE_BLOC;
								posX=LIMITE_ZONE_GAUCHE+(X[v]-1)*TAILLE_BLOC;
								posY=LIMITE_ZONE_HAUT+(Y[v])*TAILLE_BLOC;
								deplacerElement(matrice[Y[v]][X[v]],matrice[Y[v]+yMultiPosition[v]][X[v]+xMultiPosition[v]],ecran,AUTRE);
							}
					}

				break;//333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333


				case 4:

				if(Y[0]==Y[1])
					{
						if(Y[1]-1<0||matrice[Y[1]-1][X[1]-1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
						if(matrice[Y[2]][X[2]+2].etat==CARRE_FIXE){deplacementEnCour=false;return;}

						for(v=0;v<4;v++)
							{
								if(v==1){xActu=-1;yActu=-1;}
								else if(v==2){xActu=+2;yActu=0;}
								else if(v==3){xActu=1;yActu=-1;}
								if(v!=0)
									{
										positionCurseur.x=LIMITE_ZONE_GAUCHE+(X[v]+xActu-1)*TAILLE_BLOC;
										positionCurseur.y=LIMITE_ZONE_HAUT+(Y[v]+yActu)*TAILLE_BLOC;
										posX=LIMITE_ZONE_GAUCHE+(X[v]-1)*TAILLE_BLOC;
										posY=LIMITE_ZONE_HAUT+(Y[v])*TAILLE_BLOC;
										deplacerElement(matrice[Y[v]][X[v]],matrice[Y[v]+yActu][X[v]+xActu],ecran,AUTRE);
									}
							}
					}
				else
					{
						if(matrice[Y[2]+1][X[2]-1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
						if(matrice[Y[3]][X[3]-2].etat==CARRE_FIXE||matrice[Y[3]][X[3]-2].etat==LIMITE){deplacementEnCour=false;return;}

						for(v=3;v>=0;v--)
													{
														if(v==0){xActu=1;yActu=1;}
														else if(v==2){xActu=-1;yActu=1;}
														else if(v==3){xActu=-2;yActu=0;}
														if(v!=1)
															{
																positionCurseur.x=LIMITE_ZONE_GAUCHE+(X[v]+xActu-1)*TAILLE_BLOC;
																positionCurseur.y=LIMITE_ZONE_HAUT+(Y[v]+yActu)*TAILLE_BLOC;
																posX=LIMITE_ZONE_GAUCHE+(X[v]-1)*TAILLE_BLOC;
																posY=LIMITE_ZONE_HAUT+(Y[v])*TAILLE_BLOC;
																deplacerElement(matrice[Y[v]][X[v]],matrice[Y[v]+yActu][X[v]+xActu],ecran,AUTRE);
															}
													}
					}
				break;//444444444444444444444444444444444444444444444444444444444444444444444444

				case 5:
				if(X[1]==X[3]&&X[1]==X[2])
					{
						if(!trigo)
							{
								if(matrice[Y[0]][X[0]+2].etat==CARRE_FIXE||matrice[Y[0]][X[0]+2].etat==LIMITE){deplacementEnCour=false;return;}
								if(matrice[Y[1]+1][X[1]+1].etat==CARRE_FIXE||matrice[Y[1]+1][X[1]+1].etat==LIMITE){deplacementEnCour=false;return;}
								if(matrice[Y[3]-1][X[3]-1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								xMultiPosition[0]=2;yMultiPosition[0]=0;
								xMultiPosition[1]=1;yMultiPosition[1]=1;
								xMultiPosition[3]=-1;yMultiPosition[3]=-1;
							}
						else
							{
								if(matrice[Y[0]+2][X[0]].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								if(matrice[Y[1]+1][X[1]-1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								if(matrice[Y[3]-1][X[3]+1].etat==CARRE_FIXE||matrice[Y[3]-1][X[3]+1].etat==LIMITE){deplacementEnCour=false;return;}
								xMultiPosition[0]=0;yMultiPosition[0]=2;
								xMultiPosition[1]=-1;yMultiPosition[1]=1;
								xMultiPosition[3]=1;yMultiPosition[3]=-1;
							}
					}

				else if(Y[1]==Y[2]&&Y[1]==Y[3])
					{
						if(!trigo)
							{
								if(matrice[Y[0]+2][X[0]].etat==CARRE_FIXE||matrice[Y[0]+2][X[0]].etat==LIMITE){deplacementEnCour=false;return;}
								if(matrice[Y[1]-1][X[1]+1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								if(matrice[Y[3]+1][X[3]-1].etat==CARRE_FIXE||matrice[Y[3]+1][X[3]-1].etat==LIMITE){deplacementEnCour=false;return;}
								xMultiPosition[0]=0;yMultiPosition[0]=2;
								xMultiPosition[1]=1;yMultiPosition[1]=-1;
								xMultiPosition[3]=-1;yMultiPosition[3]=1;
							}
						else
							{
								if(matrice[Y[0]][X[0]-2].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								if(matrice[Y[1]-1][X[1]+1].etat==CARRE_FIXE||matrice[Y[1]-1][X[1]+1].etat==LIMITE){deplacementEnCour=false;return;}
								if(matrice[Y[3]-1][X[3]+1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								xMultiPosition[0]=-2;yMultiPosition[0]=0;
								xMultiPosition[1]=1;yMultiPosition[1]=1;
								xMultiPosition[3]=-1;yMultiPosition[3]=-1;
							}

					}

				else if(X[0]==X[1]&&X[0]==X[2])
					{
						if(!trigo)
							{
								if(matrice[Y[0]+1][X[0]+1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								if(matrice[Y[2]-1][X[2]-1].etat==CARRE_FIXE||matrice[Y[2]-1][X[2]-1].etat==LIMITE){deplacementEnCour=false;return;}
								if(matrice[Y[3]][X[3]-2].etat==CARRE_FIXE||matrice[Y[3]][X[3]-2].etat==LIMITE){deplacementEnCour=false;return;}
								xMultiPosition[0]=1;yMultiPosition[0]=1;
								xMultiPosition[2]=-1;yMultiPosition[2]=-1;
								xMultiPosition[3]=-2;yMultiPosition[3]=0;
							}
						else
							{
								if(matrice[Y[0]+1][X[0]-1].etat==CARRE_FIXE||matrice[Y[0]+1][X[0]-1].etat==LIMITE){deplacementEnCour=false;return;}
								if(matrice[Y[2]-1][X[2]+1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								if(matrice[Y[3]-2][X[3]].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								xMultiPosition[0]=-1;yMultiPosition[0]=1;
								xMultiPosition[2]=1;yMultiPosition[2]=-1;
								xMultiPosition[3]=0;yMultiPosition[3]=-2;
							}

					}

				else if(Y[0]==Y[1]&&Y[0]==Y[2])
					{
						if(!trigo)
							{
								if(Y[0]-1<0||matrice[Y[0]-1][X[0]+1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								if(matrice[Y[2]+1][X[2]-1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								if(Y[3]-2<0||matrice[Y[3]-2][X[3]].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								xMultiPosition[0]=1;yMultiPosition[0]=-1;
								xMultiPosition[2]=-1;yMultiPosition[2]=1;
								xMultiPosition[3]=0;yMultiPosition[3]=-2;
							}
						else
							{
								if(matrice[Y[0]+1][X[0]+1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								if(Y[2]-1<0||matrice[Y[2]-1][X[2]-1].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								if(matrice[Y[3]][X[3]+2].etat==CARRE_FIXE){deplacementEnCour=false;return;}
								xMultiPosition[0]=1;yMultiPosition[0]=1;
								xMultiPosition[2]=-1;yMultiPosition[2]=-1;
								xMultiPosition[3]=2;yMultiPosition[3]=0;
							}

					}
				for(v=0;v<4;v++)
					{
						if( !(v==1&& ( (Y[0]==Y[1]&&Y[0]==Y[2]) || (X[0]==X[1]&&X[0]==X[2]) ) ) )
						if( !(v==2&& ( (Y[1]==Y[2]&&Y[1]==Y[3]) || X[1]==X[3]&&X[1]==X[2] ) ) )
							{
								positionCurseur.x=LIMITE_ZONE_GAUCHE+(X[v]+xMultiPosition[v]-1)*TAILLE_BLOC;
								positionCurseur.y=LIMITE_ZONE_HAUT+(Y[v]+yMultiPosition[v])*TAILLE_BLOC;
								posX=LIMITE_ZONE_GAUCHE+(X[v]-1)*TAILLE_BLOC;
								posY=LIMITE_ZONE_HAUT+(Y[v])*TAILLE_BLOC;
								deplacerElement(matrice[Y[v]][X[v]],matrice[Y[v]+yMultiPosition[v]][X[v]+xMultiPosition[v]],ecran,AUTRE);
							}
					}

				break;//555555555555555555555555555555555555555555555555555555555555555555555555555555555555555



				case 6:
				if(Y[1]==Y[3]&&Y[1]==Y[2])
					{
						if(matrice[Y[2]+1][X[2]].etat==CARRE_FIXE||matrice[Y[2]+1][X[2]].etat==LIMITE){deplacementEnCour=false;return;}

						positionCurseur.x=LIMITE_ZONE_GAUCHE+(X[1]+1-1)*TAILLE_BLOC;
						positionCurseur.y=LIMITE_ZONE_HAUT+(Y[1]+1)*TAILLE_BLOC;

						if(trigo)
							{
								posX=LIMITE_ZONE_GAUCHE+(X[1]-1)*TAILLE_BLOC;
								posY=LIMITE_ZONE_HAUT+(Y[1])*TAILLE_BLOC;
								deplacerElement(matrice[Y[1]][X[1]],matrice[Y[1]+1][X[1]+1],ecran,AUTRE);
							}
						else
							{
								posX=LIMITE_ZONE_GAUCHE+(X[3]-1)*TAILLE_BLOC;
								posY=LIMITE_ZONE_HAUT+(Y[3])*TAILLE_BLOC;
								deplacerElement(matrice[Y[3]][X[3]],matrice[Y[3]+1][X[3]-1],ecran,AUTRE);
							}
					}
				else if(X[0]==X[1]&&X[0]==X[3])
					{
						if(matrice[Y[1]][X[1]-1].etat==CARRE_FIXE||matrice[Y[1]][X[1]-1].etat==LIMITE){deplacementEnCour=false;return;}

						positionCurseur.x=LIMITE_ZONE_GAUCHE+(X[1]-1-1)*TAILLE_BLOC;
						positionCurseur.y=LIMITE_ZONE_HAUT+(Y[1])*TAILLE_BLOC;

						if(trigo)
							{
								posX=LIMITE_ZONE_GAUCHE+(X[0]-1)*TAILLE_BLOC;
								posY=LIMITE_ZONE_HAUT+(Y[0])*TAILLE_BLOC;
								deplacerElement(matrice[Y[0]][X[0]],matrice[Y[0]+1][X[0]-1],ecran,AUTRE);
							}
						else
							{
								posX=LIMITE_ZONE_GAUCHE+(X[3]-1)*TAILLE_BLOC;
								posY=LIMITE_ZONE_HAUT+(Y[3])*TAILLE_BLOC;
								deplacerElement(matrice[Y[3]][X[3]],matrice[Y[3]-1][X[3]-1],ecran,AUTRE);
							}
					}

				else if(Y[0]==Y[1]&&Y[0]==Y[2])
					{
						if(Y[1]-1<0||matrice[Y[1]-1][X[1]].etat==CARRE_FIXE||matrice[Y[1]-1][X[1]].etat==LIMITE){deplacementEnCour=false;return;}

						positionCurseur.x=LIMITE_ZONE_GAUCHE+(X[1]-1)*TAILLE_BLOC;
						positionCurseur.y=LIMITE_ZONE_HAUT+(Y[1]-1)*TAILLE_BLOC;

						if(trigo)
							{
								posX=LIMITE_ZONE_GAUCHE+(X[2]-1)*TAILLE_BLOC;
								posY=LIMITE_ZONE_HAUT+(Y[2])*TAILLE_BLOC;
								deplacerElement(matrice[Y[2]][X[2]],matrice[Y[2]-1][X[2]-1],ecran,AUTRE);
							}
						else
							{
								posX=LIMITE_ZONE_GAUCHE+(X[0]-1)*TAILLE_BLOC;
								posY=LIMITE_ZONE_HAUT+(Y[0])*TAILLE_BLOC;
								deplacerElement(matrice[Y[0]][X[0]],matrice[Y[0]-1][X[0]+1],ecran,AUTRE);
							}
					}

				else if(X[0]==X[3]&&X[0]==X[2])
					{
						if(matrice[Y[2]][X[2]+1].etat==CARRE_FIXE||matrice[Y[2]][X[2]+1].etat==LIMITE){deplacementEnCour=false;return;}

						positionCurseur.x=LIMITE_ZONE_GAUCHE+(X[2]+1-1)*TAILLE_BLOC;
						positionCurseur.y=LIMITE_ZONE_HAUT+(Y[2])*TAILLE_BLOC;

						if(trigo)
							{
								posX=LIMITE_ZONE_GAUCHE+(X[3]-1)*TAILLE_BLOC;
								posY=LIMITE_ZONE_HAUT+(Y[3])*TAILLE_BLOC;
								deplacerElement(matrice[Y[3]][X[3]],matrice[Y[3]-1][X[3]+1],ecran,AUTRE);
							}
						else
							{
								posX=LIMITE_ZONE_GAUCHE+(X[0]-1)*TAILLE_BLOC;
								posY=LIMITE_ZONE_HAUT+(Y[0])*TAILLE_BLOC;
								deplacerElement(matrice[Y[0]][X[0]],matrice[Y[0]+1][X[0]+1],ecran,AUTRE);
							}
					}
				break;//666666666666666666666666666666666666666666666666666666666666666666666
				
			}
		SDL_Flip(ecran);
		deplacementEnCour=false;
	}

zoneJeu::~zoneJeu()
	{
		for(int i=0;i<NB_BLOCS_LARGEUR;i++)
			delete matrice [i];
		delete matrice;
		SDL_FreeSurface(cubeA);
		SDL_FreeSurface(cubeB);
		SDL_FreeSurface(cubeC);
		SDL_FreeSurface(cubeD);
		SDL_FreeSurface(cubeE);
		SDL_FreeSurface(vide); 				
	}


//CPP_ELEMENTTABLEAU=============================================================================
Uint32 deplacerTetraBasBis(Uint32 intervalle, void *parametre)
	{
		paramTimer *ecranR=(paramTimer*)parametre;
		intervalle=ecranR->pointClass->verifAcceleration();
		ecranR->pointClass->deplacerTetra(BAS,ecranR->ecranC);
		return intervalle;
	}
