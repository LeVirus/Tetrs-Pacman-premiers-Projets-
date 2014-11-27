tetris: tetris.o fonctions.o classes.o
	g++ -pg tetris.o fonctions.o classes.o  -lSDL -lSDL_image -lSDL_ttf  -lfmodex -o tetris #-lSDL_mixer -lSDLmain
tetris.o: tetris.cpp classes.cpp fonctions.cpp
	g++ -c tetris.cpp -o tetris.o
classes.o: classes.cpp
	g++ -c classes.cpp -o classes.o
fonctions.o: fonctions.cpp tetris.cpp
	g++ -c fonctions.cpp -o fonctions.o


clean:
	rm -f ./*.o

mrproper: clean
	rm -f ./tetris
