CXX = gcc
<<<<<<< HEAD
CFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm -g
DEPS = GlShader.h
=======
CFLAGS = -lSDL2 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm -lportaudio
DEPS = stb_image.h GlShader.h
>>>>>>> SDL
OBJ = Kaotic.o

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

Kaotic: $(OBJ)
	$(CXX) -o $@ $^ src/glad.c $(CFLAGS)

clean:
	rm Kaotic Kaotic.o
