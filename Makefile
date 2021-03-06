CC = g++
LD = g++

CFLAGS = -c -pedantic -std=c++11 -Wall -fno-elide-constructors
LFLAGS = -pedantic -Wall

OBJS = p4_main.o Ship.o Tanker.o Track_base.o Navigation.o Geometry.o Utility.o Island.o Cruiser.o Warship.o View.o Model.o Controller.o Ship_factory.o
PROG = p4exe

default: $(PROG)

$(PROG): $(OBJS)
	$(LD) $(LFLAGS) $(OBJS) -o $(PROG)

p4_main.o: p4_main.cpp Sim_object.h Model.h Controller.h
	$(CC) $(CFLAGS) p4_main.cpp

Ship.o: Ship.cpp Ship.h Sim_object.h Track_base.h
	$(CC) $(CFLAGS) Ship.cpp

Tanker.o: Tanker.cpp Tanker.h Ship.h Model.h
	$(CC) $(CFLAGS) Tanker.cpp

Track_base.o: Track_base.cpp Track_base.h Geometry.h Navigation.h
	$(CC) $(CFLAGS) Track_base.cpp

Navigation.o: Navigation.cpp Navigation.h Geometry.h
	$(CC) $(CFLAGS) Navigation.cpp

Utility.o: Utility.cpp Utility.h Sim_object.h Ship.h Warship.h Cruiser.h Island.h
	$(CC) $(CFLAGS) Utility.cpp

Geometry.o: Geometry.cpp Geometry.h 
	$(CC) $(CFLAGS) Geometry.cpp

Island.o: Island.cpp Island.h Sim_object.h
	$(CC) $(CFLAGS) Island.cpp

Cruiser.o: Cruiser.cpp Cruiser.h Warship.h Ship.h Sim_object.h
	$(CC) $(CFLAGS) Cruiser.cpp

Warship.o: Warship.cpp Warship.h Sim_object.h Ship.h Model.h
	$(CC) $(CFLAGS) Warship.cpp

View.o: View.cpp View.h Geometry.h
	$(CC) $(CFLAGS) View.cpp

Controller.o: Controller.cpp Controller.h Model.h View.h Utility.h Ship.h
	$(CC) $(CFLAGS) Controller.cpp

Model.o: Model.cpp Model.h Island.h Ship.h View.h
	$(CC) $(CFLAGS) Model.cpp

Ship_factory.o: Ship_factory.cpp Ship_factory.h
	$(CC) $(CFLAGS) Ship_factory.cpp

clean:
	rm -f *.o

real_clean:
	rm -f *.o
	rm -f *exe

