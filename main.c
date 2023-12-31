#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>

// sanity limit for states and edges.
#define mxn 100000 
#define NaN -117
#define ll long long

FILE* in;
FILE* out;

struct Edge 
{
    int stateFromId, stateToId, symbolId;
    int nxtEdgeIndex;
} automaton[mxn], automaton2[mxn]; int automatonSize = 1;
int s0;
int g[mxn], g2[mxn];
int isFinal[mxn];

struct Edge edgeConstruct(int from, int to, int how) {
    struct Edge ret;
    ret.stateFromId = from;
    ret.stateToId = to;
    ret.symbolId = how;
    return ret;
}


void addEdge(int from, int how, int to) {
    automaton[automatonSize] = edgeConstruct(from, to, how);
    automaton[automatonSize].nxtEdgeIndex = g[from];
    automaton2[automatonSize] = edgeConstruct(to, from, how);
    automaton2[automatonSize].nxtEdgeIndex = g2[to];
    g[from] = g2[to] = automatonSize++;
}

bool test(int edge) {
    int state = s0;
    int i = g[state];
    bool found = false;
    while (i != 0) {
        if (automaton[i].symbolId == edge) {
            state = automaton[i].stateToId;
            found = true;
            break;
        }
        i = automaton[i].nxtEdgeIndex;
    }
    if (!found) return false;
    return isFinal[state];
}

bool isDigitChar(char c) {
    return ('0' <= c && c <= '9');
}
bool isSpace(char c) {
    return (c == ' ' || c == '\n' || c == '\t');
}

int getInt() {
    int ret = 0; int sgn = 1;
    char c = fgetc(in);
    while (!isDigitChar(c)) {
        c = fgetc(in);
        if (c == EOF) return NaN;
    }
    do {
        ret = ret * 10 + (c - '0');
        c = fgetc(in);
    } while (isDigitChar(c));

    return ret;
}
char* getStr() {
    char c = fgetc(in);
    while(isSpace(c)) c = fgetc(in);
    char* buffer = malloc(1000); // make constant
    int i = 0;
    do {
        buffer[i++] = c;
        c = fgetc(in);
    } while (! isSpace(c));
    return buffer;
}
void putChar(char c) {
    fputc(c, out);
}

int used1[mxn];
int used2[mxn];

int dfs(int v) {
    used1[v] = 1;
    int i = g[v];
    while (i != 0) {
        
        int to = automaton[i].stateToId;
        if (!used1[to]) dfs(to);
        i = automaton[i].nxtEdgeIndex;
    } 
}

int dfs2(int v) {
    used2[v] = 1;
    int i = g2[v];
    while (i != 0) {
        
        int to = automaton2[i].stateToId;
        if (!used2[to]) dfs2(to);
        i = automaton2[i].nxtEdgeIndex;
    } 
}

bool canUseLetter[100];


int main() {
    in = fopen("input.txt", "r");
    out = fopen("output.txt", "w");
    if (in == NULL) {
        printf("Error, no such file: input.txt");
        return 0;
    }

    int alphabetSize = getInt();
    int stateSize = getInt();
    if (alphabetSize > 26) {
        printf("Error, too large alphabet.");
        return 0;
    }
    if (stateSize > mxn) {
        printf("Error, too large state size.");
        return 0;
    }
    s0 = getInt();
    int finalStatesSize = getInt(); 
    for (int i = 0; i < finalStatesSize; i ++ ) {
        int stateId = getInt();
        isFinal[stateId] = 1;
    }
    while (true) {
        int s1 = getInt();
        if (s1 == NaN) {
            break;
        }
        char a = getStr()[0];
        int s2 = getInt();

        addEdge(s1, (int)(a - 'a'), s2);
    }

    dfs(s0);
    for (int i = 1; i <= stateSize; i ++ ) {
        if (!used2[i] && isFinal[i]) dfs2(i);
    }

    for (int i = 0; i < automatonSize; i ++ ) {
        int from = automaton[i].stateFromId;
        int to = automaton[i].stateToId;
        if (used1[from] && used1[from] && used2[to] && used2[to]) {
            canUseLetter[automaton[i].symbolId] = true;
        }
    }

    for (int i = 0; i < alphabetSize; i ++) {
        if (!canUseLetter[i]) fprintf(out, "%c\n", ('a' + i));
    }
}