#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#define N 10

/*
Задание: написать и отладить программу, реализующую родительский процесс, 
* вызывающий и отслеживающий состояние порожденных потоков. Добавить в написанную программу синхронизацию обращения потоков к какому-либо общему ресурсу, используя взаимные исключения.
*/

/* 8. Статистический анализ. Имеется несколько массивов данных (разного размера). 
 * Требуется определить математическое ожидание в каждом массиве. 
 * Обработка каждого массива выполняется в отдельном процессе.
 * 
 * Рассчитываем как среднее арифметическое, поскольку для расчета мат. ожидания необходимы вероятности.
*/

struct DATA{
	int size;
	int * array;
};
typedef struct DATA data;

struct {
	pthread_mutex_t mutex;	
} shared = {
	PTHREAD_MUTEX_INITIALIZER
};

void *thread_func(void *arg){
	sleep(rand()%2);
	pthread_mutex_lock(&shared.mutex);
	data d = *(data *)arg;
	float math = 0;
	for (int i = 0; i < d.size; i++){
			math += d.array[i];
	}
	math = math / d.size;
	printf("Result is: %f\n", math);
	pthread_mutex_unlock(&shared.mutex);
	pthread_exit(NULL);
}


 
int main(int argc, char * argv[], char * envp[]){
	
	int ** mas = (int **) malloc(N * sizeof(int *));
	
	int count;
	data dm[N];
	
	FILE * myfile;
	myfile = fopen("source.txt", "r");
	
	fscanf(myfile, "%d", &count);
	
	for (int i = 0; i < count; i++){		
		int size;
		
		fscanf(myfile, "%d", &size);
		mas[i] = (int *) malloc(size * sizeof(int));
		
		for(int j = 0; j < size; j++){
			fscanf(myfile, "%d", &mas[i][j]);
		}
		dm[i].size = size;
		dm[i].array = mas[i];
	}
	fclose(myfile);
	printf("file is read\n");
	
	int result = 0;
	
	pthread_t thread[N];
	
	
	for (int i = 0; i < count; i++){
		result = pthread_create(&thread[i], NULL, thread_func, &dm[i]);
		if (result != 0) {
			perror("Creating the thread");
			return EXIT_FAILURE;
		}
	}
	
	for (int i = 0; i < count; i++){
		result = pthread_join(thread[i], NULL);
		if (result != 0) {
			perror("Joining the thread");
			return EXIT_FAILURE;
		}
	}

	printf("Done\n");
	return EXIT_SUCCESS;
}
