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
bool flag=false;		// ��θ� ã�Ҵ��� Ȯ���ϴ� flag
int count=0,count_visit=0;	// �ִܰ�� count , �湮 ��� count


void DAdd_OPEN(Node *child)
{	// DFS OPEN�� �߰��ϴ� �Լ�
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
{	// BFS���� OPEN�� ��带 �߰��ϴ� �Լ�
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
{	// Astar���� OPEN�� ��带 �߰��ϴ� �Լ�
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
{	// CLOSE�� �߰��ϴ� �Լ�
	count_visit++;	// �湮 ��� �� ����
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
void f_g_h(Node *s)
{	// f, g, h�� ����
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
		child[0]->h = 0;
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
		child[1]->h = 0;
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
		child[2]->h = 0;
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
		child[3]->h = 0;
		// ���� ����

		child[3]->state[x] = child[3]->state[x+3];
		child[3]->state[x+3] = 'x';
	}

	for(i=3;i>=0;i--)
		if(child[i] != NULL && !dup_flag(child[i]))
		{	// �˰��� ���� �ٸ��� OPEN�� �߰����ָ��
			DAdd_OPEN(child[i]);
			//f_g_h(child[i]);
			//Astar_OPEN(child[i]);
		}
}
void TestPrint(Node *s)
{	// FILE�� search ����� ������ִ� �Լ�
	int i;
	for(i=0;i<9;i++)
	{
		if(flag == false)	// �ִ� ��θ� �� ã�� ���
		{
			fprintf(wfd1,"%c ",s->state[i]);	// close file�� ����
			if(i%3==2) fprintf(wfd1,"\n");
		}
		else if(flag == true)	// �ִ� ��θ� ã�� ���
		{
			fprintf(wfd2,"%c ",s->state[i]);	// pathfile�� ����
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
	{	// ��θ� ã�������� ��� �ݺ�
		Node *node = Return_Node();
		if(!strcmp(node->state,goal.state))
		{	// ��θ� ã�� ���
			node_ptr = node;
			flag = true;
			break;
		}
		create_child(node);
		Add_CLOSE(node);
		TestPrint(node);
	}
	fprintf(wfd1,"�� �湮 �� : %d\n",count_visit);
	printf("�׽�Ʈ ��\n");
	while(node_ptr->parent!=NULL)
	{
		count++;
		TestPrint(node_ptr);
		node_ptr = node_ptr->parent;	// ��δ� ����� parent�� ��� �ö󰡴� ������ ����
		fprintf(wfd2,"�� ī��Ʈ : %d\n",count);
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
	for(int i=0;i<SIZE;i++)	// 'x' ��ġ Ž��
		if(start.state[i] == 'x') {start.x = i; break;}
	for(int i=0;i<SIZE;i++)
	{
		if(start.state[i] != goal.state[i]) start.h++;
	}
	start.f = start.h+start.g;
	printf("%s -> %s\n",start.state,goal.state);
	start.next = NULL;
	start.parent = NULL;
	// OPEN �ʱ�ȭ��
	OPEN = &start;	// OPEN ����
	Start_search(&start);
	fclose(wfd1);
	fclose(wfd2);
	return 0;
}