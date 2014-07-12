#include "classesPM.h"	


using namespace std;

void initPerso(ToutPerso *i);
void initImages(ImagesCommunes *i);

int main(int argc, char* args[])
	{
		bool deplacementenCours=true, boucle=true;
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
		TTF_Init();
		SDL_Surface *ecran=NULL;
		ecran=SDL_SetVideoMode(LARGEUR_FENETRE,HAUTEUR_FENETRE,32,SDL_HWSURFACE| SDL_DOUBLEBUF);
		SDL_ShowCursor(SDL_DISABLE);
		SDL_Event event;
		Niveau a;

		ImagesCommunes i;
		initImages(&i);

		ToutPerso t;
		initPerso(&t);

		StructTimerPac *pointeurVoid;
		pointeurVoid=new StructTimerPac;
		pointeurVoid->n=&a;
		pointeurVoid->event=&event;
		pointeurVoid->Pac=t.P;
		a.initNiveau(ecran, &t, &i);
		a.chargerNiveau(false);
		a.initialiserPacEtFantomes();


		t.P->timerPac(pointeurVoid);
		while(deplacementenCours)
			{
				a.miseEnPause();
				SDL_WaitEvent(&event);
				boucle=true;
				a.miseEnPause();
				while(boucle){
					SDL_PollEvent(&event);
					boucle=a.StructPerso->P->verifContinu();
					switch(event.type)
						{
							case SDL_QUIT:
							boucle=false;
							deplacementenCours=false;
							break;
							case SDL_KEYDOWN:
								switch(event.key.keysym.sym)
										{
											case SDLK_p:
											a.miseEnPause();
											SDL_WaitEvent(&event);
											break;
								}
							break;
					}
					SDL_Flip(ecran);
				}
		}
		SDL_FreeSurface(ecran);
		SDL_FreeSurface(i.point);
		SDL_FreeSurface(i.pointSpe);
		SDL_FreeSurface(i.fantomePoursuite);
		SDL_FreeSurface(i.vide);
		SDL_FreeSurface(i.cerise);
		TTF_Quit() ;
		SDL_Quit();
		return 0;	
}

void initPerso(ToutPerso *i)
	{
		i->P=new PacMan;
		i->rouge=new Fantome;
		i->bleu=new Fantome;
		i->rose=new Fantome;
		i->blanc=new Fantome;
	}

void initImages(ImagesCommunes *i)
	{
		i->point=IMG_Load("Images/point.jpeg");
		i->pointSpe=IMG_Load("Images/pointSpecial.jpeg");
		i->fantomePoursuite=IMG_Load("Images/fantomeCible.jpeg");
		i->vide=SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_BLOC, TAILLE_BLOC, 32, 0, 0, 0, 0);
		i->mur=IMG_Load("Images/mur.jpg");
		i->cerise=IMG_Load("Images/cerise.png");
	}
