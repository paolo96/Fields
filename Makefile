sfml: sfml.o charge.o field.o point.o
	g++ sfml.o charge.o field.o point.o -o sfml -lsfml-graphics -lsfml-window -lsfml-system
	./sfml

sfml.o: sfml.cpp
	g++ -c sfml.cpp

charge.o: charge.cpp
	g++ -c charge.cpp

field.o: field.cpp
	g++ -c field.cpp

point.o: point.cpp
	g++ -c point.cpp

execute:
	./sfml

clean: 
	rm *.o
