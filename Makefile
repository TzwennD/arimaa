OBJDIR=obj

CXX=g++
CXXFLAGS=-ggdb -Wall -pedantic -pipe -Wextra -std=c++11 -W -g -MMD

OBJECTS=$(OBJDIR)/Direction.o $(OBJDIR)/Step.o $(OBJDIR)/Move.o $(OBJDIR)/Piece.o $(OBJDIR)/Square.o $(OBJDIR)/Board.o $(OBJDIR)/Observer.o $(OBJDIR)/Text_Based_GUI.o $(OBJDIR)/Player.o $(OBJDIR)/Game.o $(OBJDIR)/main.o


.PHONY: all clean

all: objdir arimaa

objdir:
	@mkdir -p $(OBJDIR)

DEPFILES:=$(OBJECTS:%.o=%.d)

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
    -include $(DEPFILES)
endif

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<


arimaa: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o $@

TAGS:
	rm -f TAGS && etags --append *.hpp && etags --append *.cpp 	

clean:
	rm -f ${OBJECTS} $(DEPFILES) arimaa *~
	test -d $(OBJDIR) && rmdir $(OBJDIR)

veryclean: clean
	rm -f TAGS
