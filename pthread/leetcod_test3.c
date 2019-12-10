#include <stdio.h>
#include <pthread.h>

#include <pthread.h>

typedef struct {
	pthread_mutex_t mutex;
	pthread_cond_t cond_zero, cond;
	int flag;
	int val;
	int n;
} ZeroEvenOdd;

ZeroEvenOdd* zeroEvenOddCreate(int n) {
	ZeroEvenOdd* obj = (ZeroEvenOdd*) malloc(sizeof(ZeroEvenOdd));
	pthread_mutex_init(&obj->mutex, NULL);
	pthread_cond_init(&obj->cond_zero, NULL);
	pthread_cond_init(&obj->cond, NULL);
	obj->flag = 0;
	obj->val = 0;
	obj->n = n;
	return obj;
}

void zero(ZeroEvenOdd* obj) {
	while(1) {
		pthread_mutex_lock(&obj->mutex);
		printf("zero lock %s %d\n", __func__, __LINE__);
		usleep(1);
		if (obj->val > obj->n) {
			printf("zero break %s %d\n", __func__, __LINE__);
			pthread_mutex_unlock(&obj->mutex);
			break;
		};
		while(!(obj->flag == 0)) {
			printf("zero wait %s %d\n", __func__, __LINE__);
			pthread_cond_wait(&obj->cond, &obj->mutex);
		}
		printf("%d\n", 0);
		//printNumber(0);
		obj->val++;
		obj->flag = 1;
		printf("zero send cond signal %s %d\n", __func__, __LINE__);
		pthread_cond_broadcast(&obj->cond);
		pthread_mutex_unlock(&obj->mutex);
		printf("zero unlock %s %d\n", __func__, __LINE__);
	}

}

void even(ZeroEvenOdd* obj) {
	sleep(5);
	while(1) {
		pthread_mutex_lock(&obj->mutex);
		printf("even lock %s %d\n", __func__, __LINE__);
		if (obj->val > obj->n) {
			printf("even break %s %d\n", __func__, __LINE__);
			pthread_mutex_unlock(&obj->mutex);
			break;
		};
		while(!(obj->flag == 1 && obj->val&1 == 0)) {
			printf("even wait %s %d\n", __func__, __LINE__);
			pthread_cond_signal(&obj->cond_zero);
			pthread_cond_wait(&obj->cond, &obj->mutex);
		}
		printf("%d\n", obj->val);
		//printNumber(obj->val);
		obj->flag = 0;
		printf("even send cond_zero signal %s %d\n", __func__, __LINE__);
		pthread_mutex_unlock(&obj->mutex);
		pthread_cond_signal(&obj->cond_zero);
		printf("even unlock %s %d\n", __func__, __LINE__);
	}
}

void odd(ZeroEvenOdd* obj) {
	while(1) {
		pthread_mutex_lock(&obj->mutex);
		printf("odd lock %s %d\n", __func__, __LINE__);
		if (obj->val > obj->n) {
			printf("odd break %s %d\n", __func__, __LINE__);
			pthread_mutex_unlock(&obj->mutex);
			break;
		};
		while(!(obj->flag == 1 && obj->val&1 == 1)) {
			printf("odd wait %s %d\n", __func__, __LINE__);
			pthread_cond_signal(&obj->cond_zero);
			pthread_cond_wait(&obj->cond, &obj->mutex);
		}
		printf("%d\n", obj->val);
		//printNumber(obj->val);
		obj->flag = 0;
		printf("odd send cond_zero signal %s %d\n", __func__, __LINE__);
		pthread_mutex_unlock(&obj->mutex);
		pthread_cond_signal(&obj->cond_zero);
		printf("odd unlock %s %d\n", __func__, __LINE__);
	}
}

void zeroEvenOddFree(ZeroEvenOdd* obj) {
	free(obj);
}

int main(int argc, const char *argv[])
{
	ZeroEvenOdd * obj;
	pthread_t thread_zero, thread_odd, thread_even;

	obj = zeroEvenOddCreate(4);
	int ret = pthread_create(&thread_zero, NULL, zero, obj);
	if (ret) {
		printf("create zero thread failed\n");
		return -1;
	}

	ret = pthread_create(&thread_odd, NULL, odd, obj);
	if (ret) {
		printf("create odd thread failed\n");
		return -1;
	}

	ret = pthread_create(&thread_even, NULL, even, obj);
	if (ret) {
		printf("create even thread failed\n");
		return -1;
	}
	
	pthread_join(thread_zero, NULL);
	pthread_join(thread_odd, NULL);
	pthread_join(thread_even, NULL);

	return 0;
}
