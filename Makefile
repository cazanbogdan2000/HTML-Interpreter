build: Interpret.o queue.o main.o tree.o ConstructTree.o Operations.o Stack.o AppendStyle.o Override.o DeleteRecursively.o
	gcc -Wall -std=c99 $^ -g -o tema3

Interpret.o: Interpret.c
	gcc -Wall -std=c99 -c -g Interpret.c

queue.o: queue.c
	gcc -Wall -std=c99 -c -g queue.c

main.o: main.c
	gcc -Wall -std=c99 -c -g main.c

tree.o: tree.c
	gcc -Wall -std=c99 -c -g tree.c

ConstructTree.o: ConstructTree.c
	gcc -Wall -std=c99 -c -g ConstructTree.c

Operations.o: Operations.c
	gcc -Wall -std=c99 -c -g Operations.c

Stack.o: Stack.c
	gcc -Wall -std=c99 -c -g Stack.c

AppendStyle.o: AppendStyle.c
	gcc -Wall -std=c99 -c -g AppendStyle.c

Override.o: Override.c
	gcc -Wall -std=c99 -c -g Override.c

DeleteRecursively.o: DeleteRecursively.c
	gcc -Wall -std=c99 -c -g DeleteRecursively.c

run: tema3 input.html commands.in commands.out
	./tema3 input.html commands.in commands.out

memcheck: tema3 input.html commands.in commands.out
	valgrind --verbose --leak-check=full --show-leak-kinds=all ./tema3 input.html commands.in commands.out
clean:
	rm -f *.o tema3