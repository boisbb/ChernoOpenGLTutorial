#OBJS specifies which files to compile as part of the project
OBJS = Application.cpp

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS =

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lglfw -lGL -lGLU -lrt -lm -ldl -lXrandr -lXinerama -lXcursor -lXext -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp -lGLEW -lGLU -lGL -lm -ldl -lXdamage -lXxf86vm -lXfixes -lXext -lX11 -lpthread -lxcb -lXau -lXdmcp

#OBJ_NAMEs specifies the name of our *.o
OBJ_NAMES = Renderer.o IndexBuffer.o VertexBufferLayout.o VertexBuffer.o VertexArray.o Application.o

#EXEC_NAME name of executable
EXEC_NAME = Application

#This is the target that compiles our executable
all : $(OBJ_NAMES)
	$(CC) $(COMPILER_FLAGS) $(OBJ_NAMES) -o $(EXEC_NAME) $(LINKER_FLAGS)

Renderer.o: Renderer.cpp Renderer.h
	g++ -c $(COMPILER_FLAGS) $(LINKER_FLAGS) Renderer.cpp

VertexBuffer.o: VertexBuffer.cpp VertexBuffer.h
	g++ -c $(COMPILER_FLAGS) $(LINKER_FLAGS) VertexBuffer.cpp

IndexBuffer.o: IndexBuffer.cpp IndexBuffer.h
	g++ -c $(COMPILER_FLAGS) $(LINKER_FLAGS) IndexBuffer.cpp

VertexArray.o: VertexArray.cpp VertexArray.h
	g++ -c $(COMPILER_FLAGS) $(LINKER_FLAGS) VertexArray.cpp

VertexBufferLayout.o: VertexBufferLayout.cpp VertexBufferLayout.h
	g++ -c $(COMPILER_FLAGS) $(LINKER_FLAGS) VertexBufferLayout.cpp

Application.o: Application.cpp
	g++ -c $(COMPILER_FLAGS) $(LINKER_FLAGS) Application.cpp
run :
	./$(EXEC_NAME)

clean:
	rm $(OBJ_NAMES)
