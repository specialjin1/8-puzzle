#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <math.h>
#define SIZE SIDE*SIDE
#define SIDE 3 

typedef struct Node {
	struct Node *parent;
	char state[SIZE+1];
	int x;
	int g;
	int h;
	int f;
	struct Node *next;
}Node;

FILE *wfd1,*wfd2;		// close file, path file
Node goal;				// goal node
Node *CLOSE=NULL,*OPEN=NULL;
bool flag=false;		// 경로를 찾았는지 확인하는 flag
int count=0,count_visit=0;	// 최단경로 count , 방문 노드 count


void DAdd_OPEN(Node *child)
{	// DFS OPEN에 추가하는 함수
	if(OPEN == NULL)
	{
		child->next = NULL;
		OPEN = child;
	}
	else
	{
		child->next = OPEN;
		OPEN = child;
	}
}
void BAdd_OPEN(Node *child)
{	// BFS에서 OPEN에 노드를 추가하는 함수
	if(OPEN == NULL)
	{
		child->next = NULL;
		OPEN = child;
	}
	else
	{
		Node *temp = OPEN;
		while(temp->next!=NULL)
		{
			temp = temp->next;
		}
		temp->next = child;
		child->next = NULL;
	}
}
void Astar_OPEN(Node *child)
{	// Astar에서 OPEN에 노드를 추가하는 함수
	if(OPEN ==NULL) {
		OPEN = child;
		OPEN->next = NULL;
	}
	else
	{
		Node *temp1 = OPEN;
		Node *temp2 = temp1;
		if(OPEN->f > child->f)
		{
			child->next = OPEN;
			OPEN = child;
			return ;
		}
		if(temp1->next == NULL)
		{
			temp1->next = child;
			child->next = NULL;
		}
		else
		{
			temp1 = OPEN->next;
			while(temp1!=NULL)
			{
				if(temp1==NULL) break;
				if(temp1->f > child->f) break;
				temp1 = temp1->next;
				temp2 = temp2->next;
			}
			child->next = temp2->next;
			temp2->next = child;
		}
	}
}
void Add_CLOSE(Node *child)
{	// CLOSE에 추가하는 함수
	count_visit++;	// 방문 노드 수 증가
	if(CLOSE == NULL)
	{
		child->next = NULL;
		CLOSE = child;
	}
	else
	{
		Node *temp = CLOSE;
		while(temp->next!=NULL)
		{
			temp = temp->next;
		}
		temp->next = child;
		child->next = NULL;
	}
}
Node *Return_Node()
{	// OPEN에서 하나의 노드를 빼오는 함수
	Node *temp = OPEN;
	OPEN = OPEN->next;
	return temp;
}
bool dup_flag(Node *s)
{	// 이미 한번 돌았던 곳이 있는지 체크
	Node *temp = CLOSE;
	if(temp==NULL) return false;
	while(temp!=NULL)
	{
		if(!strcmp(s->state,temp->state)) return true;
		temp = temp->next;
	}
	return false;
}
void f_g_h(Node *s)
{	// f, g, h를 측정
	int i,j;
	s->g = s->parent->g+1;
	// A*1
	for(i=0;i<SIZE;i++)
		if(s->state[i] != goal.state[i]) s->h++;

	// A*2
	/*for(i=0;i<SIZE;i++)
	{
		if(s->state[i] != goal.state[i])
		{
			for(j=0;j<SIZE;j++)
				if(s->state[i] == goal.state[j]) break;
			s->h += abs(i-j);
		}
	}
	*/
	//
	s->f = s->g + s->h;
}
void create_child(Node *s)
{
	Node *child[4];
	int i,j,col,row;
	int x=s->x;			// x = s->x
	// ←↑→↓ 순으로 생성
	col = (s->x)/3;
	row = (s->x)%3;
	for(i=0;i<4;i++) child[i] = NULL;
	// ← 생성
	if( row != 0)
	{
		child[0] = (Node *)malloc(sizeof(Node));
		child[0]->parent = s;
		strcpy(child[0]->state,s->state);
		child[0]->x = s->x-1;
		child[0]->h = 0;
		// 완전 복사

		child[0]->state[x] = child[0]->state[x-1];
		child[0]->state[x-1] = 'x';
	}
	// ↑ 생성
	if( col != 0)
	{
		child[1] = (Node *)malloc(sizeof(Node));
		child[1]->parent = s;
		strcpy(child[1]->state,s->state);
		child[1]->x = s->x-3;
		child[1]->h = 0;
		// 완전 복사

		child[1]->state[x] = child[1]->state[x-3];
		child[1]->state[x-3] = 'x';

	}
	// → 생성
	if( row != 2)
	{
		child[2] = (Node *)malloc(sizeof(Node));
		child[2]->parent = s;
		strcpy(child[2]->state,s->state);
		child[2]->x = s->x+1;
		child[2]->h = 0;
		// 완전 복사

		child[2]->state[x] = child[2]->state[x+1];
		child[2]->state[x+1] = 'x';
	}
	// ↓ 생성
	if(col != 2)
	{
		child[3] = (Node *)malloc(sizeof(Node));
		child[3]->parent = s;
		strcpy(child[3]->state,s->state);
		child[3]->x = s->x+3;
		child[3]->h = 0;
		// 완전 복사

		child[3]->state[x] = child[3]->state[x+3];
		child[3]->state[x+3] = 'x';
	}

	for(i=3;i>=0;i--)
		if(child[i] != NULL && !dup_flag(child[i]))
		{	// 알고리즘에 따라 다르게 OPEN에 추가해주면됨
			DAdd_OPEN(child[i]);
			//f_g_h(child[i]);
			//Astar_OPEN(child[i]);
		}
}
void TestPrint(Node *s)
{	// FILE에 search 결과를 출력해주는 함수
	int i;
	for(i=0;i<9;i++)
	{
		if(flag == false)	// 최단 경로를 못 찾은 경우
		{
			fprintf(wfd1,"%c ",s->state[i]);	// close file에 저장
			if(i%3==2) fprintf(wfd1,"\n");
		}
		else if(flag == true)	// 최단 경로를 찾은 경우
		{
			fprintf(wfd2,"%c ",s->state[i]);	// pathfile에 저장
			if(i%3==2) fprintf(wfd2,"\n");
		}
	}
	if(flag == false) fprintf(wfd1,"\n");
	else if(flag == false) fprintf(wfd2,"\n");
}
void Start_search(Node *start)
{
	Node *node_ptr;
	while(flag!=true)
	{	// 경로를 찾기전까지 계속 반복
		Node *node = Return_Node();
		if(!strcmp(node->state,goal.state))
		{	// 경로를 찾은 경우
			node_ptr = node;
			flag = true;
			break;
		}
		create_child(node);
		Add_CLOSE(node);
		TestPrint(node);
	}
	fprintf(wfd1,"총 방문 수 : %d\n",count_visit);
	printf("테스트 끝\n");
	while(node_ptr->parent!=NULL)
	{
		count++;
		TestPrint(node_ptr);
		node_ptr = node_ptr->parent;	// 경로는 노드의 parent를 계속 올라가는 식으로 만듦
		fprintf(wfd2,"총 카운트 : %d\n",count);
	}
}
int main(int argc, char *argv[])
{	// argv[1] : close file , argv[2] : path file
	Node start;
	printf("Start : ");
	scanf("%s",start.state);
	printf("\nGoal  : ");
	scanf("%s",goal.state);
	wfd1 = fopen(argv[1],"w");
	wfd2 = fopen(argv[2],"w");
	start.g = 0;
	start.h = 0;
	for(int i=0;i<SIZE;i++)	// 'x' 위치 탐색
		if(start.state[i] == 'x') {start.x = i; break;}
	for(int i=0;i<SIZE;i++)
	{
		if(start.state[i] != goal.state[i]) start.h++;
	}
	start.f = start.h+start.g;
	printf("%s -> %s\n",start.state,goal.state);
	start.next = NULL;
	start.parent = NULL;
	// OPEN 초기화후
	OPEN = &start;	// OPEN 넣음
	Start_search(&start);
	fclose(wfd1);
	fclose(wfd2);
	return 0;
}