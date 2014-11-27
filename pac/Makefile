LDFLAGS=-lSDL  -lSDL_image -lSDL_ttf -lfmodex #-lSDLmain
PH=constantePM.h classesPM.h
aa: 	PuckMan.o classeFantome.o classeNiveau.o classePacMan.o
	g++ -pg $^ $(LDFLAGS) -o $@ 
PuckMan.o: PuckMan.cpp $(PH)
	g++ -pg -c $< -o $@
classePacMan.o: classePacMan.cpp $(PH)
	g++ -pg -c $< -o $@
classeFantome.o: classeFantome.cpp $(PH)
	g++ -pg -c $< -o $@
classeNiveau.o: classeNiveau.cpp $(PH)
	g++ -pg -c $< -o $@


clean:
	rm -f ./*.o

mrproper: clean
	rm -f ./tetris
