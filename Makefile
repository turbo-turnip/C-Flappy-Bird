# makefile

OBJ_FILE=Flappy\ Bird
FRAMEWORK=SDL2
IMAGES_FRAMEWORK=SDL2_image
FONT_FRAMEWORK=SDL2_ttf

all:
	gcc -Wall -g -o $(OBJ_FILE) src/main.c -framework $(FRAMEWORK) -framework $(IMAGES_FRAMEWORK) -framework $(FONT_FRAMEWORK) && chmod a+x $(OBJ_FILE)
exec:
	./$(OBJ_FILE)
clean:
	rm -rf $(OBJ_FILE)
	rm -rf *.o && rm -rf *.exe && rm -rf Icon
setup:
	make clean
	make