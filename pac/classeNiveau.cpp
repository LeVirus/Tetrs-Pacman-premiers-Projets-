#include "classesPM.h"	

using namespace std;

Niveau::Niveau()
	{
		images=NULL;
		viderScore=SDL_CreateRGBSurface(SDL_HWSURFACE, 250, 30, 32, 0, 0, 0, 0);
		string nomFichier="Images/PLAN.txt";
		score=0;
		Hscore=10000;
		nbPointAct=0;
		police = TTF_OpenFont("Children.ttf", 30);
		policeB = TTF_OpenFont("Children.ttf", 13);
		vieN=IMG_Load("Images/pacMan.png");	
		couleurBleu = {90, 150, 250};
		pause=false;
		pointCerise=100;
		intervalleCerise=8000;
		posVies.x=ZONE_VIES_X;
		posVies.y=ZONE_VIES_Y;

		FMOD_System_Create(&system);
		FMOD_System_Init(system, 2, FMOD_INIT_NORMAL, NULL);
		FMOD_System_CreateSound(system, "Sons/pacDie.mp3", FMOD_CREATESAMPLE, 0, &pacMeur);
		FMOD_System_CreateSound(system, "Sons/fantomeBouffe.mp3", FMOD_CREATESAMPLE, 0, &fantomeBouffe);
		FMOD_System_CreateSound(system, "Sons/pointSpe.mp3", FMOD_CREATESAMPLE, 0, &pointSpecial);
		FMOD_System_CreateSound(system, "Sons/fantomeOutBleu.WAV", FMOD_CREATESAMPLE, 0, &fantomeOut);
		FMOD_System_CreateSound(system, "Sons/glitch.mp3", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM|FMOD_LOOP_NORMAL, 0, &musique);
		FMOD_Sound_SetLoopCount(musique, -1);
	}

void Niveau::afficherMatrice()
	{
		for(int y=0;y<NB_BLOCS_HAUTEUR;y++)
			for(int x=0;x<NB_BLOCS_LARGEUR;x++)
				{cout<<matrice[y][x]; if(x==20)cout<<endl;}
	}

void Niveau::ecrireScore(){
				bool ret=false;
				positionCurseur.y=ZONE_SCORE_Y+30;
				for(int i=0; i<2;i++){
					positionCurseur.x=ZONE_SCORE_X-150;
					SDL_BlitSurface(viderScore,NULL,ecranN,&positionCurseur);
					positionCurseur.x+=200;
					oss << score;
					recepteur = oss.str();
					oss.str("");
					size_t size = recepteur.size() + 1;
					char * t = new char[size];
					strncpy( t, recepteur.c_str(), size );
					texteScore = TTF_RenderText_Solid(police, t, couleurBleu);
					positionCurseur.x-=texteScore->w;
					SDL_BlitSurface(texteScore,NULL,ecranN,&positionCurseur);
					delete t;
					if(ret){score=0;return;}
					if(score==0){score=Hscore;ret=true;}
					else if(score<Hscore)return;
					positionCurseur.y+=62;
				}
}

void Niveau::initNiveau(SDL_Surface *ecran, ToutPerso *tp, ImagesCommunes *imagesN){
		FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, musique, 0, NULL);
		if(!images)images=imagesN;
		StructPerso=tp;
		ecranN=ecran;
		TTF_Font *policeA=NULL;
		texteScore=NULL;
		policeA = TTF_OpenFont("ImpactLabel.ttf", 30);

			for(int y=0;y<3;y++){
				SDL_BlitSurface(vieN,NULL,ecran,&posVies);
				if(y!=2)posVies.x+=30;
			}

				std::string SA="SCORE", SB="HIGHSCORE";
				positionCurseur.x=ZONE_SCORE_X-30;
				positionCurseur.y=ZONE_SCORE_Y;
				for(int i=0;i<2;i++){
					size_t size = SA.size() + 1;
					char * t = new char[size];
					strncpy( t, SA.c_str(), size );
					texteScore = TTF_RenderText_Solid(policeA, t, couleurBleu);
					delete t;
					SDL_BlitSurface(texteScore,NULL,ecran,&positionCurseur);
					positionCurseur.y+=60;
					positionCurseur.x-=70;
					SDL_FreeSurface(texteScore);
					SA=SB;
				}
				ecrireScore();
				SDL_Flip(ecran);
}

void Niveau::jouerSon(){
	FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, fantomeOut, 0, NULL);
}

void Niveau::miseEnPause(){
	if(ceriseAct)tempsCerise=SDL_GetTicks()-tempsCerise;
	StructPerso->P->pauseF();
	StructPerso->rose->pauseF();
	StructPerso->rouge->pauseF();
	StructPerso->bleu->pauseF();
	StructPerso->blanc->pauseF();
}

void Niveau::initialiserPacEtFantomes()
	{
		StructPerso->P->initialiser(ecranN, this, images);

		StructPerso->rouge->personnaliser(ROUGE);
		StructPerso->rouge->pointeurs(ecranN, this, StructPerso->P, images);
		StructPerso->rouge->initialiserFantome(DEFAULT);
		StructPerso->rouge->timerFantome();

		StructPerso->rose->personnaliser(ROSE);
		StructPerso->rose->pointeurs(ecranN, this, StructPerso->P, images);
		StructPerso->rose->initialiserFantome(DEFAULT);
		StructPerso->rose->timerFantome();

		StructPerso->blanc->personnaliser(BLANC);
		StructPerso->blanc->pointeurs(ecranN, this, StructPerso->P, images);
		StructPerso->blanc->initialiserFantome(DEFAULT);
		StructPerso->blanc->timerFantome();

		StructPerso->bleu->personnaliser(BLEU);
		StructPerso->bleu->pointeurs(ecranN, this, StructPerso->P, images);
		StructPerso->bleu->initialiserFantome(DEFAULT);
		StructPerso->bleu->timerFantome();
	}

void Niveau::resetNiveau(short choose){
	tempsCerise-=intervalleCerise;
	if(choose==GAME_OVER  ||  choose==PAC_BOUFFE)
		FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, pacMeur, 0, NULL);
	if(choose==PAC_BOUFFE){
		SDL_BlitSurface(images->vide,NULL,ecranN,&posVies);
		posVies.x-=30;
	}
	StructPerso->rouge->initialiserFantome(choose);
	StructPerso->bleu->initialiserFantome(choose);
	StructPerso->blanc->initialiserFantome(choose);
	StructPerso->rose->initialiserFantome(choose);
	StructPerso->P->resetPacMan();
	if(choose==LEVEL_UP  ||  choose==GAME_OVER){
		nbPointAct=0;chargerNiveau(true);
		if(choose==GAME_OVER){
			score=0;
			ecrireScore();
			pointCerise=100;
			intervalleCerise=8000;
			posVies.x=ZONE_VIES_X;
			posVies.y=ZONE_VIES_Y;
			for(int i=0;i<3;i++){
				SDL_BlitSurface(vieN,NULL,ecranN,&posVies);
				if(i!=2)posVies.x+=30;
			}
		}
		else pointCerise+=100;
	}
}

void Niveau::appliquerFantomeBouffe(bool B, SDL_Rect *positionFantome)
	{
		if(B)
			{
				FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, fantomeBouffe, 0, NULL);
				std::string b;
				switch(nombreFantomeBouffe)
					{
						case 0:
						score+=200;
						b="200";
						break;
						case 1:
						score+=400;
						b="400";
						break;
						case 2:
						score+=800;
						b="800";
						break;
						case 3:
						score+=1600;
						b="1600";
						break;
					}
				nombreFantomeBouffe++;
					size_t size = b.size() + 1;
					char * t = new char[size];
					strncpy( t, b.c_str(), size );
					texteScore = TTF_RenderText_Solid(policeB, t, couleurBleu);
					positionFantome->x+=7;
					positionFantome->y+=10;
					SDL_BlitSurface(texteScore,NULL,ecranN,positionFantome);
					positionFantome->x-=7;
					positionFantome->y-=10;
					if(ceriseAct)tempsCerise+=1100;
					sleep(1);
					delete t;
			}
		else nombreFantomeBouffe=0;
		if(score>Hscore)Hscore=score;
		ecrireScore();
	}

void Niveau::chargerNiveau(bool debug)
	{
		ceriseAct=false;
		int y;
		string nomFichier="Images/PLAN.txt", ligne;
		ifstream flux(nomFichier.c_str());
		if(flux)
			{
				int caract;
				positionCurseur.x=LIMITE_ZONE_GAUCHE; positionCurseur.y=LIMITE_ZONE_HAUT;
				for(y=0;y<NB_BLOCS_HAUTEUR;y++)
					{						
						getline(flux, ligne);
						for(int x=0;x<NB_BLOCS_LARGEUR;x++)
							{
								caract=ligne[x];
								caract-=48;	
								matrice[y][x]=caract;
								switch(caract)
									{
										case VIDE:
										SDL_BlitSurface(images->vide,NULL,ecranN,&positionCurseur);
										break;
										case MUR:
										SDL_BlitSurface(images->mur,NULL,ecranN,&positionCurseur);
										break;
										case POINT:
										SDL_BlitSurface(images->point,NULL,ecranN,&positionCurseur);
										break;
										case POINT_SPECIAL:
										SDL_BlitSurface(images->pointSpe,NULL,ecranN,&positionCurseur);
										break;

										default:break;

									}
								if(positionCurseur.x+TAILLE_BLOC>=LIMITE_ZONE_DROITE)
									{
										positionCurseur.x=LIMITE_ZONE_GAUCHE;
										positionCurseur.y+=TAILLE_BLOC;
									}
								else 
										positionCurseur.x+=TAILLE_BLOC;
							}
						}
				if(!debug)SDL_Flip(ecranN);
				sleep(1);
			}
		else cout<<"erreur flux"<<endl;
	}

bool Niveau::verifMatrice(int x, int y, int direction)
	{
				switch(direction)
					{
						case GAUCHE:
						if(matrice[y][x-1]==MUR) return false;
						return true;
						break;

						case DROITE:
						if(matrice[y][x+1]==MUR)return false;
						return true;
						break;

						case BAS:
						if(matrice[y+1][x]==MUR)return false;
						return true;
						break;

						case HAUT:
						if(matrice[y-1][x]==MUR)return false;
						return true;
						break;
					}
	}

void Niveau::verifPoints(int x, int y, bool &continu)
	{
		if(ceriseAct  &&  intervalleCerise<SDL_GetTicks()-tempsCerise){
			ceriseAct=false;
			positionCurseur.x=INIT_PAC_X;
			positionCurseur.y=INIT_PAC_Y-5*TAILLE_BLOC;
			SDL_BlitSurface(images->vide,NULL,ecranN,&positionCurseur);

			matrice[14][10]=VIDE;
		}
		if(matrice[y][x]==VIDE)return;
		else if(matrice[y][x]==POINT)
			{
				nbPointAct++;
				score+=10;
				matrice[y][x]=VIDE;
				ecrireScore();
			}
		else if(matrice[y][x]==POINT_SPECIAL)
			{
				FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, pointSpecial, 0, NULL);
				nbPointAct++;
				score+=50;
				matrice[y][x]=VIDE;
				StructPerso->rouge->cibleFantome(true);
				StructPerso->bleu->cibleFantome(true);
				StructPerso->rose->cibleFantome(true);
				StructPerso->blanc->cibleFantome(true);
				this->appliquerFantomeBouffe(false, NULL);
				ecrireScore();
			}
		else if(matrice[y][x]==CERISE){
				FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, fantomeBouffe, 0, NULL);
				score+=pointCerise;
				matrice[y][x]=VIDE;
				tempsCerise+=intervalleCerise;
				ceriseAct=false;
				ecrireScore();
		}
		if(score>Hscore)Hscore=score;
		if(nbPointAct==PREMIER_SPECIAL  ||  nbPointAct==DEUXIEME_SPECIAL)specialPoint();
		else if(nbPointAct==TOTAL_POINTS)resetNiveau(LEVEL_UP);
	}

void Niveau::specialPoint(){
	positionCurseur.x=INIT_PAC_X;
	positionCurseur.y=INIT_PAC_Y-5*TAILLE_BLOC;
	SDL_BlitSurface(images->cerise,NULL,ecranN,&positionCurseur);
	matrice[14][10]=CERISE;
	tempsCerise=SDL_GetTicks();
	ceriseAct=true;
}

int Niveau::numMatrice(int X, int Y)
	{
		return matrice[Y][X];
	}

Niveau::~Niveau()
		{
			SDL_FreeSurface(vieN);
			SDL_FreeSurface(viderScore);
			SDL_FreeSurface(texteScore);
		}
