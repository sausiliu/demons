#define MEMORY_KEY 12
#define SIZE_OF_ARRAY 10
struct wrapper
{
	int array[SIZE_OF_ARRAY];
	sem_t empty;
	sem_t pmutex;
	sem_t cmutex;
	sem_t full;
	int n;
};
