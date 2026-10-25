/*
 RideShare C data-structure core (console/backend foundation)
 Compile: gcc rideshare_core.c -o rideshare_core
 This is a standalone C implementation of Graph + Dijkstra, hashing,
 and FIFO pending-request queue. The HTML prototype is not connected
 to this file yet.
*/
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define LOC 5
#define MAX 100
#define HS 211
#define INF 1000000

typedef struct { char id[20], name[50]; int loc, available; } Driver;
typedef struct { char id[20], name[50]; int loc; } Passenger;
typedef struct { char id[20]; int index, used; } HashEntry;
typedef struct { int rideId, passengerIndex, pickup, destination; } Request;

static const char locationNames[LOC] = {'A','B','C','D','E'};
static int graph[LOC][LOC] = {
    {0,4,7,0,0}, {4,0,2,5,0}, {7,2,0,3,6},
    {0,5,3,0,2}, {0,0,6,2,0}
};
static Driver drivers[MAX]; static int driverCount;
static Passenger passengers[MAX]; static int passengerCount;
static HashEntry driverHash[HS], passengerHash[HS];
static Request queue[MAX]; static int front, rear, qCount;

static unsigned hashId(const char *s) {
    unsigned h=0;
    while (*s) h=(h*31u+(unsigned char)*s++)%HS;
    return h;
}
static int hashFind(HashEntry t[], const char *id) {
    unsigned p=hashId(id), start=p;
    while (t[p].used) {
        if (!strcmp(t[p].id,id)) return t[p].index;
        p=(p+1)%HS; if(p==start) break;
    }
    return -1;
}
static int hashAdd(HashEntry t[], const char *id, int index) {
    unsigned p=hashId(id), start=p;
    while(t[p].used) {
        if(!strcmp(t[p].id,id)) return 0;
        p=(p+1)%HS; if(p==start) return 0;
    }
    strcpy(t[p].id,id); t[p].index=index; t[p].used=1; return 1;
}
static int shortestDistance(int source, int target) {
    int d[LOC], seen[LOC]={0}, i,j,u,best;
    for(i=0;i<LOC;i++) d[i]=INF;
    d[source]=0;
    for(i=0;i<LOC;i++) {
        u=-1; best=INF;
        for(j=0;j<LOC;j++) if(!seen[j] && d[j]<best) best=d[j],u=j;
        if(u<0) break;
        seen[u]=1;
        for(j=0;j<LOC;j++) if(graph[u][j]>0 && !seen[j] && d[u]+graph[u][j]<d[j])
            d[j]=d[u]+graph[u][j];
    }
    return d[target];
}
static int enqueue(Request r) {
    if(qCount==MAX) return 0;
    queue[rear]=r; rear=(rear+1)%MAX; qCount++; return 1;
}
static int dequeue(Request *r) {
    if(!qCount) return 0;
    *r=queue[front]; front=(front+1)%MAX; qCount--; return 1;
}
int main(void) {
    int i;
    puts("RideShare C core ready.");
    puts("Locations: A B C D E");
    puts("Road distances from A:");
    for(i=0;i<LOC;i++)
        printf("A -> %c : %d units\n", locationNames[i], shortestDistance(0,i));
    puts("\nGraph, Dijkstra, hash-table and FIFO queue functions are implemented.");
    puts("This core can be expanded into the full console application or a C web backend.");
    return 0;
}
