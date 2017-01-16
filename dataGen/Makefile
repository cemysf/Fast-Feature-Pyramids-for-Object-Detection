FILE='main.cpp'
OUT='main'

INCLUDE='/usr/include/opencv2/'
LIBS=`pkg-config --libs opencv`
all:
	g++ $(FILE) -o $(OUT) -I $(INCLUDE) $(LIBS) -g
