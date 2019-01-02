#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum Direction {UP=0,LEFT,DOWN,RIGHT};
typedef enum Direction Direction;
typedef struct Node Node;
typedef struct Tree Tree;
typedef struct QNode QNode;
struct Node{
    Node** Children;
    int* info;
    int deg;
    Node* Parent;
};
struct Tree{
    Node* Root;
    int size;
    int m;
};
struct QNode{
    int priority;
    Node* node;
    QNode* next;
};
void insertQ(QNode** f,Node* n, int* end){
    int i,j,p = 0;
    for(i = 0;i<9;i++){
        for(j=0;j<9;j++){
            if(n->info[i] == end[j])
                break;
        }
        p += abs(i/3 - j/3);
        p += abs(i%3 - j%3);
    }
    QNode* qn = malloc(sizeof(QNode));
    qn->priority = p;
    qn->node = n;
    qn->next = *f;
    *f = qn;
    return;
}
Node* deleteQ(QNode** f){
    Node* r;
    if(*f == 0)
        return 0;
    QNode* t = *f;
    QNode* m = *f;
    while(t){
        if(t->priority <= m->priority){
            m = t;
        }
        t = t->next;
    }
    if(m == *f){
        *f = (*f)->next;
        r = m->node;
        free(m);
        return r;
    }
    if(!(m->next)){
        t = *f;
        while(t->next->next != 0)
            t = t->next;
        r = m->node;
        free(m);
        t->next = 0;
        return r;
    }
    r = m->node;
    m->node = m->next->node;
    m->priority = m->next->priority;
    t = m->next;
    m->next = t->next;
    free(t);
    return r;
}
Node* GETNODE(int m){
    Node* n = (Node*)malloc(sizeof(Node));
    n->info = (int*)calloc(9,sizeof(int));
    n->deg = 0;
    n->Children = (Node**)calloc(m,sizeof(Node*));
    n->Parent = 0;
    return n;
}
Tree* INIT_TREE(int m){
    Tree* T = malloc(sizeof(Tree));
    T->Root=0;
    T->size = 0;
    T->m = m;
    return T;
}
Node* Add(Tree* T,const int* x,Node* p){
    Node* n = GETNODE(T->m);
    int i;
    for(i = 0;i<9;i++){
        n->info[i] = x[i];
    }
    if(T->size == 0){
        T->Root = n;
        n->Parent = 0;
        T->size = 1;
        return n;
    }
    p->Children[p->deg++] = n;
    n->Parent = p;
    T->size++;
    return n;
}
void FREENODE(Node* n){
    free(n->info);
    free(n->Children);
    free(n);
}
void freeQ(QNode* f){
    QNode* t;
    while(f){
        t = f;
        f = f->next;
        free(t);
    }
}
void preorder(Tree* T, void (*P)(Node*)){
    Node** S = calloc(T->size,sizeof(Node*));
    int s = 0;
    S[s] = T->Root;
    s++;
    Node* next;
    while(s>0){
        s--;
        next = S[s];
        for(int i = next->deg-1;i>=0;i--){
            S[s] = next->Children[i];
            s++;
        }
        (*P)(next);
    }
}
void Delete(Tree* T){
    preorder(T,&FREENODE);
    free(T);
}
int random(int a, int b){
    double t = ((double)rand()/(RAND_MAX+1))*(b-a) + a;
    return floor(t);
}
void shuffle(int* yates,int n){
    int i = n;
    for(i = n-1;i>0;i--){
        int r = random(0,i+1);
        int t = yates[r];
        yates[r] = yates[i];
        yates[i] = t;
    }
}
int move(int* pos,Direction d){
    int *p,*q;
    int i,j;
    for(i = 0;i<9;i++){
        if(pos[i] == 0){
            p = &pos[i];
            break;
        }
    }
    j = i%3;
    i /= 3;
    switch(d){
        case LEFT:
            if(j == 0)
                return 0;
            q = &pos[i*3 + j - 1];
            break;
        case RIGHT:
            if(j == 2)
                return 0;
            q = &pos[i*3 + j + 1];
            break;
        case UP:
            if(i == 0)
                return 0;
            q = &pos[(i-1)*3 + j];
            break;
        case DOWN:
            if(i == 2)
                return 0;
            q = &pos[(i+1)*3 + j];
    }
    *p = *q;
    *q = 0;
    return 1;
}
int isPossible(int* start, int* end){
    int i,j,i1,j1,i2,j2;
    int temp[9];
    for(i = 0;i<9;i++){
        temp[i] = start[i];
    }
    for(i1 = 0;i1<9;i1++){
        if(start[i1] == 0){
            break;
        }
    }
    j1 = i1%3;
    i1 /= 3;
    for(i2 = 0;i2<9;i2++){
        if(end[i2] == 0){
            break;
        }
    }
    j2 = i2%3;
    i2 /= 3;
    int dx = j2-j1;
    Direction d = RIGHT;
    if(dx<0){
        d = LEFT;
        dx = -dx;
    }
    for(i = 0;i<dx;i++)
        move(temp,d);
    dx = i2-i1;
    d = DOWN;
    if(dx<0){
        d = UP;
        dx = -dx;
    }
    for(i = 0;i<dx;i++)
        move(temp,d);
    int count = 0;
    int t;
    for(i = 0;i<9;i++){
        for(j = 0;j<9;j++){
            if((temp[j] == end[i])&& (j!=i)){
                t = temp[j];
                temp[j] = temp[i];
                temp[i] = t;
                count++;
                break;
            }
        }
    }
    return !(count%2);
}
void writePuzzle(int* p){
    int i,j;
    for(i = 0;i<3;i++){
        for(j = 0;j<3;j++){
            printf("%d",p[3*i+j]);
        }
        printf("\n");
    }
    printf("\n");
}
void writeRoute(Node* n){
    Node* t = n;
    printf("\n");
    while(t){
        writePuzzle(t->info);
        t = t->Parent;
    }
}
void simulate(int* start,int* end){
    Tree* T = INIT_TREE(4);
    int i,j,x[9];
    Node *temp,*next;
    QNode* f = 0;
    for(i=0;i<9;i++){
        x[i] = start[i];
    }
    next = Add(T,x,0);
    for(i = UP;i<=RIGHT;i++)
        if(move(x,i)){
            temp = Add(T,x,next);
            for(j = 0;j<9;j++){
                if(x[j]!=end[j])
                    break;
            }
            if(j==9){
                writeRoute(temp);
                freeQ(f);
                Delete(T);
                return;
            }
            insertQ(&f,temp,end);
            move(x,(i+2)%4);
        }
    while(next = deleteQ(&f)){
        for(i=0;i<9;i++){
            x[i] = next->info[i];
        }
        for(i = UP;i<=RIGHT;i++){
            if(move(x,i)){
                j = 0;
                temp = next->Parent;
                while((temp!=0)){
                    for(j = 0;j<9;j++){
                        if(x[j]!= temp->info[j]){
                            break;
                        }
                    }
                    if(j==9)
                        break;
                    temp = temp->Parent;
                }
                if(j!=9){
                    temp = Add(T,x,next);
                    for(j = 0;j<9;j++){
                        if(x[j]!=end[j])
                            break;
                    }
                    if(j==9){
                        writeRoute(temp);
                        freeQ(f);
                        Delete(T);
                        return;
                    }
                insertQ(&f,temp,end);
                }
                move(x,(i+2)%4);
            }
        }
    }
}
int main(int argc, char** argv) {
    int finish = 0;
    int isReady = 0;
    int input;
    QNode* f;
    srand(time(0));
    int i,j;
    int *start,*end;
    while(!finish){
        printf("0. Close\n1. Init\n2. Random\n3. Is it solvable?\n4. Solve it\n");
        scanf("%d",&input);
        switch(input){
            case 0:
                finish = 1;
                break;
            case 1:
                start = malloc(9*sizeof(int));
                end = malloc(9*sizeof(int));
                printf("Start:\n");
                for(i = 0; i < 9;i++){
                    scanf("%d",&start[i]);
                    j = 0;
                    if((start[i]<0) || (start[i]>8)){
                        printf("Input not valid\n");
                        j = -1;
                        break;                        
                    }
                    for(j = 0; j<i; j++){
                        if((start[i] == start[j])){
                            printf("Input not valid\n");
                            j = -1;
                            break;
                        }
                    }
                    if(j == -1)
                        break;
                }
                if(j == -1)
                    break;
                printf("End:\n");
                for(i = 0; i < 9;i++){
                    j = 0;
                    scanf("%d",&end[i]);
                    if((end[i]<0) || (end[i]>8)){
                        printf("Input not valid\n");
                        j = -1;
                        break;                        
                    }
                    for(j = 0; j<i; j++){
                        if((end[i] == end[j])){
                            printf("Input not valid\n");
                            j = -1;
                            break;
                        }
                    }
                    if(j == -1)
                        break;
                }
                if(j!=-1)
                    isReady = 1;
                break;
            case 2:
                start = malloc(9*sizeof(int));
                end = malloc(9*sizeof(int));
                for(i = 0;i<9;i++)
                    start[i] = end[i] = i;
                shuffle(start,9);
                shuffle(end,9);
                isReady = 1;
                writePuzzle(start);
                printf("\n");
                writePuzzle(end);
                break;
            case 3:
                if(isReady){
                    if(isPossible(start,end))
                        printf("It is possible to solve\n");
                    else
                        printf("No solution available\n");
                }else{
                    printf("No game loaded.\n");
                }
                break;
            case 4:
                if(isReady){
                    if(!isPossible(start,end)){
                        printf("No solution available\n");
                    }else{
                        simulate(end,start);
                    }
                }
                else{
                    printf("No game loaded.\n");
                }
        }
    }
    return (EXIT_SUCCESS);
}