LINK   = `root-config --cflags` `root-config --glibs` -lMinuit
LINKC  = `root-config --cflags` 
DBG    = -g
COMP   = -c
WARN   = -Wall
CC     = g++

Main     : testold.C Line.h rt.o michiganrt.o dc.o hough.o readdata.o random.o segment.o chambergeometry.o draw.o fit.o  defines.h
	   $(CC) $(WALL) -o $@ $(DBG) testold.C rt.o michiganrt.o dc.o hough.o readdata.o random.o segment.o chambergeometry.o draw.o fit.o $(LINK)
#%.o      : %.C %.h
#	   $(CC) $(WALL) $(LINKC) $(COMP) $(DBG) $^ -o $@
chambergeometry.o : chambergeometry.C chambergeometry.h defines.h
	   g++ $(WALL) $(LINKC) $(COMP) $(DBG) $^
draw.o   : draw.C draw.h  defines.h
	   g++ $(WALL) $(LINKC) $(COMP) $(DBG) $^
#pattern.o : pattern.C pattern.h
#	   g++ $(WALL) $(LINKC) $(COMP) $(DBG) $^
hough.o  : hough.C hough.h
	   g++ $(WALL) $(LINKC) $(COMP) $(DBG) $^
readdata.o : readdata.C readdata.h
	   g++ $(WALL) $(LINKC) $(COMP) $(DBG) $^
random.o : random.C random.h
	   g++ $(WALL) $(LINKC) $(COMP) $(DBG) $^
segment.o: segment.C segment.h
	   g++ $(WALL) $(LINKC) $(COMP) $(DBG) $^
rt.o     : rt.C rt.h
	   g++ $(WALL) $(LINKC) $(COMP) $(DBG) $^
michiganrt.o : michiganrt.C michiganrt.h
	   g++ $(WALL) $(LINKC) $(COMP) $(DBG) $^
dc.o     : dc.C dc.h
	   g++ $(WALL) $(LINKC) $(COMP) $(DBG) $^
fit.o    : fit.C fit.h rt.h Line.h
	   g++ $(WALL) $(LINKC) $(COMP) $(DBG) $^
clean    :
	  rm -rf *.o *~ *.gch *.rootmap *.so Main Main.dSYM

