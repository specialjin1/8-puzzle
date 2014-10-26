#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 10

typedef struct Node {
	struct Node *parent;
	char state[10];
	//int g;
	//int h;
	//int f;
	struct Node *child;
}Node;

Node start, goal;

int main(void)
{
	printf("Start : ");
	scanf("%s",start.state);
	printf("\nGoal  : ");
	scanf("%s",goal.state);

	printf("%s -> %s\n",start.state,goal.state);

	return 0;
}