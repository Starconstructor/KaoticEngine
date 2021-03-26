CXX = gcc
CFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm -lportaudio -lcore
DEPS = stb_image.h GlShader.h Overlord.h
OBJ = Kaotic.o

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< -I./nappgui/inc/ -L./nappgui/v142_x64/Release $(CFLAGS)

Kaotic: $(OBJ)
	$(CXX) -o $@ $^ src/glad.c -I./nappgui/inc/ -L./nappgui/v142_x64/Release $(CFLAGS)

clean:
	rm Kaotic Kaotic.o
