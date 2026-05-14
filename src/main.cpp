#include <stdio.h>
#include <math.h>
#include <raylib.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define INF 1000000000
#define N  105
#define MAX_LOG_DISPLAY 20

#define RESET     "\033[0m"
#define C_RED     "\033[31m" 
#define C_GREEN   "\033[32m"
#define C_YELLOW  "\033[33m"
#define C_CYAN    "\033[36m"
#define C_BOLD    "\033[1m"
#define C_WHITE   "\033[37m"

typedef enum {STATE_FIND_MIN,STATE_RELAX_EDGES,STATE_FINISHED} AlgoState;

struct Graph {
    int n;
    int A[N][N];
    Vector2 toado[N];
    int L[N];
    int T[N];    
    int par[N];  
};



void AddLog(const char* msg,char stepLogs[300][128],int *logCount) {
    if (*logCount<300) {
        strcpy(stepLogs[*logCount],msg);
        (*logCount)++;
    }
}

void PrintCentered(const char* text,const char* color) {
    int width=80; 
    int len=strlen(text);
    int padding=(width-len)/2;
    for (int i=0; i<padding;i++) 
        printf(" ");
    printf("%s%s%s\n",color,text,RESET);
}

void inputdata(struct Graph *g,int *u,int *v,const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) { 
        printf("Loi: Khong mo duoc file du lieu!\n"); 
        exit(1); 
    }
    fscanf(f,"%d",&g->n);
    for (int i=1;i<=g->n;i++)
        for (int j=1;j<=g->n;j++)
            fscanf(f,"%d",&g->A[i][j]);  
    fscanf(f,"%d",u); 
    fscanf(f,"%d",v);
    fclose(f);
}

void drawArrow(Vector2 start,Vector2 end,float rDinh,Color mau,int trongSo,float thickness) {
    float angle=atan2f(end.y-start.y,end.x-start.x);
    Vector2 tip={end.x-(rDinh+5)*cosf(angle),end.y-(rDinh+5)*sinf(angle)};
    DrawLineEx(start,tip,thickness,mau);

    float arrowSize=14.0f;
    Vector2 p1={tip.x+arrowSize*cosf(angle+PI-0.45f),tip.y+arrowSize*sinf(angle+PI-0.45f)};
    Vector2 p2={tip.x+arrowSize*cosf(angle+PI+0.45f),tip.y+arrowSize*sinf(angle+PI+0.45f)};
    DrawTriangle(tip,p2,p1,mau); 

    if (trongSo!=-1) {
        char buf[10]; 
        sprintf(buf,"%d",trongSo);
        Vector2 mid={(start.x+tip.x)/2,(start.y+tip.y)/2};
        DrawRectangle(mid.x-2,mid.y-2,MeasureText(buf,20)+8,24,Fade(RAYWHITE, 0.7f));
        DrawText(buf,mid.x+2,mid.y,20,(mau.r>200&&mau.g<100) ? RED : DARKGRAY);
    }
}

void UIRaylib_Dijkstra(struct Graph g, int u_start, int v_target,char stepLogs[300][128],int *logCount) {
    InitWindow(1800, 900, "PBL1 - Mo phong Dijkstra Step-by-Step - DUT");
    
    Vector2 center = { 430, 400 }; 
    float radius = 280.0f;
    for (int i = 1; i <= g.n; i++) {
        float angle = (i - 1) * 2 * PI / g.n;
        g.toado[i] = (Vector2){ center.x + radius * cosf(angle), center.y + radius * sinf(angle) };
        g.L[i] = INF; g.T[i] = 1; g.par[i] = -1;
    }
    g.L[u_start] = 0;

    AlgoState state = STATE_FIND_MIN;
    int currentV = -1, neighborIdx = 1;
    float timer = 0.0f;
    float speed = 1.2f; 
    char pathResult[512] = "";
    bool onPath[N] = { false };

    AddLog("BAT DAU: Khoi tao L[nguon]=0, cac dinh khac = INF",stepLogs,logCount);

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        timer += GetFrameTime();

        if (timer >= speed && state != STATE_FINISHED) {
            timer = 0;
            if (state == STATE_FIND_MIN) {
                int min_d = INF; currentV = -1;
                for (int i = 1; i <= g.n; i++)
                    if (g.T[i] && g.L[i] < min_d) { 
                        min_d = g.L[i]; 
                        currentV = i; 
                    }

                if (currentV == -1 || currentV == v_target) {
                    state = STATE_FINISHED;
                    if (g.L[v_target] != INF) {
                        int t = v_target;
                        int temp[N], count = 0;
                        while (t != -1) { 
                            onPath[t] = true; 
                            temp[count++] = t; 
                            t = g.par[t]; }
                        sprintf(pathResult, "LO TRINH: ");
                        for (int i = count - 1; i >= 0; i--) {
                            char b[15]; 
                            sprintf(b, "%d%s", temp[i], (i==0 ? "" : " -> "));
                            strcat(pathResult, b);
                        }
                    } else sprintf(pathResult, "KET QUA: KHONG TIM THAY DUONG DI");
                    AddLog(">>> KET THUC: Da xac dinh lo trinh.",stepLogs,logCount);
                } else {
                    g.T[currentV] = 0; neighborIdx = 1; state = STATE_RELAX_EDGES;
                    AddLog(TextFormat("Chon dinh %d (L=%d) nho nhat", currentV, g.L[currentV]),stepLogs,logCount);
                }
            } 
            else if (state == STATE_RELAX_EDGES) {
                bool found = false;
                while (neighborIdx <= g.n) {
                    if (g.A[currentV][neighborIdx] != -1 && g.T[neighborIdx]) {
                        int newD = g.L[currentV] + g.A[currentV][neighborIdx];
                        if (newD < g.L[neighborIdx]) {
                            g.L[neighborIdx] = newD;
                            g.par[neighborIdx] = currentV;
                            AddLog(TextFormat("  + Toi uu L[%d] xuong con %d", neighborIdx, newD),stepLogs,logCount);
                        } else AddLog(TextFormat("  - Canh %d->%d khong toi uu", currentV, neighborIdx),stepLogs,logCount);
                        neighborIdx++; found = true; break;
                    }
                    neighborIdx++;
                }
                if (!found) state = STATE_FIND_MIN;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangle(900, 0, 500, 900, Fade(LIGHTGRAY, 0.3f));
        DrawLineEx((Vector2){900, 0}, (Vector2){900, 900}, 2.0f, DARKGRAY);
        DrawText("MO PHONG THUAT TOAN", 940, 30, 24, DARKBLUE);
        for (int i = 0; i < *logCount; i++) {
            if (i >= *logCount - MAX_LOG_DISPLAY) {
                int pos = i - (*logCount > MAX_LOG_DISPLAY ? *logCount - MAX_LOG_DISPLAY : 0);
                DrawText(stepLogs[i], 925, 80 + pos * 35, 20, DARKGREEN);
            }
        }

        for (int i = 1; i <= g.n; i++)
            for (int j = 1; j <= g.n; j++)
                if (g.A[i][j] != -1) {
                    Color c = LIGHTGRAY; float thick = 1.5f;
                    if (state == STATE_FINISHED && onPath[i] && onPath[j] && g.par[j] == i) { c = RED; thick = 5.0f; }
                    else if (g.par[j] == i) { c = DARKBLUE; thick = 2.5f; }
                    drawArrow(g.toado[i], g.toado[j], 32.0f, c, g.A[i][j], thick);
                }

        for (int i = 1; i <= g.n; i++) {
            Color c = SKYBLUE;
            if (state == STATE_FINISHED && onPath[i]) c = RED;
            else if (i == u_start) c = ORANGE;
            else if (i == v_target) c = RED;
            else if (i == currentV) c = YELLOW;
            else if (g.T[i] == 0) c = LIME;

            DrawCircleV(g.toado[i], 32, c);
            DrawCircleLinesV(g.toado[i], 32, BLACK);
            DrawText(TextFormat("%d", i), g.toado[i].x - 10, g.toado[i].y - 12, 26, (onPath[i] && state==STATE_FINISHED)? WHITE : BLACK);
            if (g.L[i] < INF) DrawText(TextFormat("L=%d", g.L[i]), g.toado[i].x - 25, g.toado[i].y + 38, 18, MAROON);
        }

        if (state == STATE_FINISHED) {
            DrawRectangle(40, 720, 820, 150, Fade(GOLD, 0.9f));
            DrawRectangleLines(40, 720, 820, 150, ORANGE);
            DrawText("KET QUA CUOI CUNG:", 60, 740, 26, DARKBLUE);
            DrawText(pathResult, 60, 780, 22, BLACK);
            if (g.L[v_target] != INF)
                DrawText(TextFormat("TONG DO DAI DUONG DI: %d", g.L[v_target]), 60, 820, 26, RED);
        }

        EndDrawing();
    }
    CloseWindow();
}
void UIconsole() {
    system("cls"); 
    PrintCentered("TRUONG DAI HOC BACH KHOA", C_BOLD C_WHITE);
    PrintCentered("KHOA CONG NGHE THONG TIN", C_BOLD C_WHITE);
    PrintCentered("---------- o0o ----------", C_WHITE);
    printf("\n\n\n");
    PrintCentered("DO AN LAP TRINH TINH TOAN", C_YELLOW C_BOLD);
    printf("\n");
    PrintCentered("XAY DUNG CHUONG TRINH TIM DUONG DI NGAN NHAT", C_CYAN C_BOLD);
    PrintCentered("TREN DO THI CO HUONG CO TRONG SO", C_CYAN C_BOLD);
    printf("\n\n\n");
    printf("%s%40s %-30s%s\n", C_WHITE, "Nguoi huong dan:", "ThS. TRAN HO THUY TIEN", RESET);
    printf("%s%40s %-30s%s\n", C_WHITE, "Sinh vien thuc hien:", "Tran Van Truong Vu", RESET);
    printf("%s%40s %-30s%s\n", C_WHITE, "", "Tran Ngoc Bao Quyen", RESET);
    printf("%s%40s %-30s%s\n", C_WHITE, "Lop:", "25Nh11", RESET);
    printf("\n\n\n");
    PrintCentered("Da Nang, 06/2026", C_WHITE);
    printf("\n%s%s%s\n", C_YELLOW, "================================================================================", RESET);
}
void filename(char file[128],char path[256]) {
    printf("\nNhap duong dan thu muc (vd: D:/data): "); 
    scanf("%s",path);
    if (path[strlen(path)-1]!='/' && path[strlen(path)-1]!='\\') 
        strcat(path, "/");
    printf("Nhap ten file (vd: input.dat): "); 
    scanf("%s", file); 
    strcat(path, file);
}
int main() {
    char stepLogs[300][128];
    int logCount = 0;
    UIconsole();
    struct Graph g; 
    int u,v; 
    char path[256],file[128];
    filename(file,path);
    inputdata(&g,&u,&v,path);
    UIRaylib_Dijkstra(g,u,v,stepLogs,&logCount);
    return 0;
}