SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system
FLAGS = -O3

all:
	./sfml

fields: sfml.o draws.o field.o charge.o circle_charge.o square_charge.o custom_charge.o point.o button.o vector.o
	g++ $(FLAGS) sfml.o draws.o charge.o circle_charge.o square_charge.o custom_charge.o field.o point.o button.o vector.o -o sfml $(SFML_LIBS)

%.o: 
	g++ $(FLAGS) -c $*.cpp

commit: fields clean
	scp *.* paolo.biglioli@tolab.fisica.unimi.it:Fields/

clean: 
	rm *.o
