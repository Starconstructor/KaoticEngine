CXX = g++
CFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lportaudio -std=c++17
DEPS = stb_image.h GlShader.h
OBJ = Kaotic.o

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

Kaotic: $(OBJ)
	$(CXX) -o $@ $^ src/glad.c $(CFLAGS)

clean:
	rm Kaotic Kaotic.o
