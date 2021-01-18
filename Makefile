all : 
	gcc lab10.c -lpthread -o lab10
	./lab10 source.txt

clean :
	rm lab10
