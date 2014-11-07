#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
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

FILE *wfd1,*wfd2;
Node goal;
Node *CLOSE=NULL,*OPEN=NULL;
Node *top = NULL;
bool flag=false;
int count=0,countpath=0;

void DAdd_OPEN(Node *child)
{	// DFS OPEN�� �߰��ϴ� �Լ�
	countpath++;
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
{
	countpath++;
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
void Astar_OPEN1(Node *child)
{

}
void Add_CLOSE(Node *child)
{	// CLOSE�� �߰��ϴ� �Լ�
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
{	// OPEN���� �ϳ��� ��带 ������ �Լ�
	Node *temp = OPEN;
	OPEN = OPEN->next;
	return temp;
}
bool dup_flag(Node *s)
{	// �̹� �ѹ� ���Ҵ� ���� �ִ��� üũ
	Node *temp = CLOSE;
	if(temp==NULL) return false;
	while(temp!=NULL)
	{
		if(!strcmp(s->state,temp->state)) return true;
		temp = temp->next;
	}
	return false;
}
void create_child(Node *s)
{
	Node *child[4];
	int i,col,row;
	int x=s->x;			// x = s->x
	// ����� ������ ����
	col = (s->x)/3;
	row = (s->x)%3;
	for(i=0;i<4;i++) child[i] = NULL;
	// �� ����
	if( row != 0)
	{
		child[0] = (Node *)malloc(sizeof(Node));
		child[0]->parent = s;
		strcpy(child[0]->state,s->state);
		child[0]->x = s->x-1;
		// ���� ����

		child[0]->state[x] = child[0]->state[x-1];
		child[0]->state[x-1] = 'x';
	}
	// �� ����
	if( col != 0)
	{
		child[1] = (Node *)malloc(sizeof(Node));
		child[1]->parent = s;
		strcpy(child[1]->state,s->state);
		child[1]->x = s->x-3;
		// ���� ����

		child[1]->state[x] = child[1]->state[x-3];
		child[1]->state[x-3] = 'x';

	}
	// �� ����
	if( row != 2)
	{
		child[2] = (Node *)malloc(sizeof(Node));
		child[2]->parent = s;
		strcpy(child[2]->state,s->state);
		child[2]->x = s->x+1;
		// ���� ����

		child[2]->state[x] = child[2]->state[x+1];
		child[2]->state[x+1] = 'x';
	}
	// �� ����
	if(col != 2)
	{
		child[3] = (Node *)malloc(sizeof(Node));
		child[3]->parent = s;
		strcpy(child[3]->state,s->state);
		child[3]->x = s->x+3;
		// ���� ����

		child[3]->state[x] = child[3]->state[x+3];
		child[3]->state[x+3] = 'x';
	}
	for(i=3;i>=0;i--)
		if(child[i] != NULL && !dup_flag(child[i])) DAdd_OPEN(child[i]);	// �ߺ��� �ƴ� ��� OPEN�� ����
		// DFS search
	//else
	//	create_child(Return_Node());
}
void TestPrint(Node *s)
{
	int i;
	for(i=0;i<9;i++)
	{
		if(flag == false)
		{
			fprintf(wfd1,"%c ",s->state[i]);
			if(i%3==2) fprintf(wfd1,"\n");
		}
		else if(flag == true)
		{
			fprintf(wfd2,"%c ",s->state[i]);
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
	{
		Node *node = Return_Node();
		if(!strcmp(node->state,goal.state))
		{
			node_ptr = node;
			flag = true;
			break;
		}
		create_child(node);
		Add_CLOSE(node);
		TestPrint(node);
		// Sleep(50);
		system("cls");
	}
	fprintf(wfd1,"�� OPEN �� : %d\n",countpath);
	printf("�׽�Ʈ ��\n");
	while(node_ptr->parent!=NULL)
	{
		count++;
		TestPrint(node_ptr);
		node_ptr = node_ptr->parent;
		fprintf(wfd2,"�� ī��Ʈ : %d\n",count);
	}
}



int main(int argc, char *argv[])
{
	Node start;
	printf("Start : ");
	scanf("%s",start.state);
	printf("\nGoal  : ");
	scanf("%s",goal.state);
	wfd1 = fopen(argv[1],"w");
	wfd2 = fopen(argv[2],"w");
	for(int i=0;i<SIZE;i++)	// 'x' ��ġ Ž��
		if(start.state[i] == 'x') {start.x = i; break;}
	printf("%s -> %s\n",start.state,goal.state);
	start.next = NULL;
	start.parent = NULL;
	OPEN = &start;
	Start_search(&start);
	fclose(wfd1);
	fclose(wfd2);
	return 0;
}