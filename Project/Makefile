CC = clang
FLAGS = -Wall -Wextra -Wpedantic -Werror -std=c17
OBJ = Project/includes/Image.o Project/includes/LinearAlgebra.o Project/Renderer/Solid.o

# TODO: Clean out needless files

main: Project/main.c $(OBJ)
	$(CC) $(FLAGS) -o Project/main $(OBJ) Project/main.c -lm

Image: Project/includes/Image.c
	$(CC) -c $(FLAGS) -o Project/includes/Image.o Project/includes/Image.c

LinearAlgebra: Project/includes/LinearAlgebra.c
	$(CC) -C $(FLAGS) -o Project/includes/LinearAlgebra.o Project/includes/LinearAlgebra.c

Solid: Project/Renderer/Solid.c
	$(CC) -C $(FLAGS) -o Project/Renderer/Solid.o Project/Renderer/Solid.c
