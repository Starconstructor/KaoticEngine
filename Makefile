CXX = gcc
CFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm -lportaudio -O2
DEPS = stb_image.h GlShader.h Overlord.h
OBJ = Kaotic.o

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

Kaotic: $(OBJ)
	$(CXX) -o $@ $^ lib/glad.c $(CFLAGS)

clean:
	rm Kaotic Kaotic.o
