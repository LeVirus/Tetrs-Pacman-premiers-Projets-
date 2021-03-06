#ifndef DEF_CONSTANTES
#define DEF_CONSTANTES

    #define NOMBRE_VIES         3
    #define TOTAL_POINTS        194
    #define PREMIER_SPECIAL     69
    #define DEUXIEME_SPECIAL    123
    #define TOTAL_POINTS        194
    #define TAILLE_BLOC         30
    #define NB_BLOCS_LARGEUR    21
    #define NB_BLOCS_HAUTEUR    26
    #define LIMITE_ZONE_GAUCHE  60
    #define LIMITE_ZONE_DROITE  LIMITE_ZONE_GAUCHE+TAILLE_BLOC * (NB_BLOCS_LARGEUR)
    #define LIMITE_ZONE_HAUT    50
    #define LIMITE_ZONE_BAS     LIMITE_ZONE_HAUT+TAILLE_BLOC * (NB_BLOCS_HAUTEUR)
    #define LARGEUR_CADRE_JEU   TAILLE_BLOC * NB_BLOCS_LARGEUR
    #define LONGUEUR_CADRE_JEU  TAILLE_BLOC * NB_BLOCS_HAUTEUR
    #define LARGEUR_FENETRE     TAILLE_BLOC * NB_BLOCS_LARGEUR+400
    #define HAUTEUR_FENETRE     TAILLE_BLOC * NB_BLOCS_HAUTEUR+200
    #define INIT_FANTOME_X	10*TAILLE_BLOC+LIMITE_ZONE_GAUCHE
    #define INIT_FANTOME_Y	12*TAILLE_BLOC+LIMITE_ZONE_HAUT

    #define ZONE_VIES_X		750
    #define ZONE_VIES_Y		700



    #define INIT_PAC_X		10*TAILLE_BLOC+LIMITE_ZONE_GAUCHE
    #define INIT_PAC_Y		19*TAILLE_BLOC+LIMITE_ZONE_HAUT


    #define ZONE_SCORE_Y    	LIMITE_ZONE_HAUT+100
    #define ZONE_SCORE_X    	LIMITE_ZONE_DROITE+150

    enum {DEFAULT, LEVEL_UP, PAC_BOUFFE, GAME_OVER};
    enum {ROUGE, ROSE, BLANC, BLEU};
    enum {BAS, DROITE, HAUT, GAUCHE};
    enum {VIDE, MUR, POINT, POINT_SPECIAL, FANTOME, PACMAN, CERISE};
    enum {BLOCK_VIDE, BLOCK_MUR, BLOCK_POINT, BLOCK_POINTSPE, BLOCK_FANTOMEA, BLOCK_FANTOMEB, BLOCK_FANTOMEC, BLOCK_FANTOMED};

#endif
