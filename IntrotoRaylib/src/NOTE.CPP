#include <stdio.h>
#include <math.h>
#include <raylib.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define INF 1000000000
#define N  105
#define RESET     "\033[0m"
#define C_RED     "\033[31m" 
#define C_GREEN   "\033[32m"
#define C_YELLOW  "\033[33m"
#define C_CYAN    "\033[36m"
#define C_BOLD    "\033[1m"
#define C_WHITE   "\033[37m"

struct Graph {
    int n;
    int A[N][N];
    Vector2 toado[N];
    int path[N];
    int L[N];
};
typedef struct Graph Graph;

void dijkstra(Graph *g, int l, int r,int *count) {
    int T[N],par[N];
    par[l]=-1;
    for (int i=1;i<=g->n;i++) {
        g->L[i]=INF;
        T[i]=1;
    }
    g->L[l]=0;
    while (1) {
        int v=-1;
        int min_e=INF;
        for (int i=1;i<=g->n;i++)
            if (T[i] && g->L[i]<min_e) {
                min_e=g->L[i];
                v=i;
            }
        if (v==-1) {
            g->L[r]=-1;
            return;
        }
        T[v]=0;
        if (v==r){
            *count=1;
            int tmp=r;
            while (tmp!=-1) {
                g->path[*count]=tmp;
                (*count)++;
                tmp=par[tmp];
            }
            return;
        }
        for (int j=1;j<=g->n;j++) {
            if (T[j] && g->A[v][j]!=-1) {
                if (g->L[j]>g->A[v][j]+g->L[v]) {
                    g->L[j]=g->A[v][j]+g->L[v];
                    par[j]=v;
                }
            }
        }
    }
}
void MyDelay(float seconds) {
    clock_t start_time=clock(); 
    float duration=seconds*CLOCKS_PER_SEC;     
    while (clock()<start_time+duration); 
}
void PrintCentered(const char* text, const char* color) {
    int width = 80; 
    int len = strlen(text);
    int padding = (width - len) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s%s%s\n", color, text, RESET);
}
void LoadingEffect(const char *msg) {
    printf("%s%s", C_CYAN, msg);
    for (int i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout); 
        MyDelay(0.15f);  
    }
    printf("%s\n", RESET);
}
void nhapdulieu(Graph *g,int *u,int *v,const char *path) {
    FILE *f = fopen(path, "r");
    printf("Nhap so dinh: ");
    fscanf(f,"%d",&g->n);
    // printf("Nhap ma tran ke: (Nhap -1 neu khong co canh): \n");
    for (int i=1;i<=g->n;i++)
        for (int j=1;j<=g->n;j++)
            fscanf(f,"%d",&g->A[i][j]);  
    // printf("Nhap dinh xuat phat: ");
    fscanf(f,"%d",u);
    // printf("Nhap dinh dich: ");
    fscanf(f,"%d",v);
    fclose(f);
}
void VeMuiTen(Vector2 start, Vector2 end, float rDinh, Color mau, int trongSo, float offsetDist) {
    float angle = atan2f(end.y - start.y, end.x - start.x);
    Vector2 offset = { -sinf(angle) * offsetDist, cosf(angle) * offsetDist };
    Vector2 st = { start.x + offset.x, start.y + offset.y };
    Vector2 en = { end.x + offset.x, end.y + offset.y };
    Vector2 tip = { en.x - 31 * cosf(angle), en.y - 31 * sinf(angle) };
    float thickness = (mau.r > 150) ? 1.5f : 4.0f;
    DrawLineEx(st, tip, thickness, mau);
        float arrowSize = 12.0f;
    Vector2 p1 = { tip.x + arrowSize * cosf(angle + PI - 0.4f), tip.y + arrowSize * sinf(angle + PI - 0.4f) };
    Vector2 p2 = { tip.x + arrowSize * cosf(angle + PI + 0.4f), tip.y + arrowSize * sinf(angle + PI + 0.4f) };
    DrawTriangle(tip, p2, p1, mau); 
    char buf[10]; sprintf(buf, "%d", trongSo);
    int tw = MeasureText(buf, 16);
    Vector2 textPos = { (st.x + tip.x) / 2 + offset.x * 2.5f, (st.y + tip.y) / 2 + offset.y * 2.5f };
    DrawRectangle(textPos.x - tw/2 - 2, textPos.y - 8, tw + 4, 18, Fade(RAYWHITE, 0.8f));
    DrawText(buf, textPos.x - tw/2, textPos.y - 8, 16, (mau.r > 150) ? DARKGRAY : MAROON);
}

void vedothi(Graph g, int u, int v, int count) {
    InitWindow(1000, 750, "Mo phong Dijkstra PBL1 - DUT");
    
    Vector2 tam = { 500, 375 };
    float rad = 250.0f;
    for (int i = 1; i <= g.n; i++) {
        float goc = (i - 1) * 2 * PI / g.n;
        g.toado[i] = (Vector2){ tam.x + rad * cosf(goc), tam.y + rad * sinf(goc) };
    }

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing(); 
        ClearBackground(RAYWHITE);

        for (int i = 1; i <= g.n; i++)
            for (int j = 1; j <= g.n; j++)
                if (g.A[i][j] != -1 && i != j)
                    VeMuiTen(g.toado[i], g.toado[j], 30.0f, GRAY, g.A[i][j], 10.0f);

        if (g.L[v] != -1 && count > 0) {
            for (int i = count - 1; i > 1; i--) {
                int nodeU = g.path[i];
                int nodeV = g.path[i-1];
                if (nodeU > 0 && nodeV > 0)
                    VeMuiTen(g.toado[nodeU], g.toado[nodeV], 30.0f, DARKBLUE, g.A[nodeU][nodeV], 10.0f);
            }
        }

        for (int i = 1; i <= g.n; i++) {
            Color nodeColor = SKYBLUE;
            if (i == u) nodeColor = ORANGE;
            else if (i == v) nodeColor = RED;
            else if (count > 0) {
                for (int k = 1; k < count; k++) if (g.path[k] == i) nodeColor = GREEN;
            }
            DrawCircleV(g.toado[i], 30, nodeColor);
            DrawCircleLinesV(g.toado[i], 30, BLACK);
            int tw = MeasureText(TextFormat("%d", i), 20);
            DrawText(TextFormat("%d", i), g.toado[i].x - tw/2, g.toado[i].y - 10, 20, BLACK);
        }

    DrawRectangle(15, 15, 600, 90, Fade(SKYBLUE, 0.2f)); 
    DrawRectangleLines(15, 15, 600, 90, DARKBLUE);
    DrawText(TextFormat("Dinh xuat phat: %d  |  Dinh dich: %d", u, v), 30, 30, 22, DARKBLUE);
    if (g.L[v] == -1 || g.L[v] == INF) {
        DrawText("Trang thai: Khong tim thay duong di!", 30, 65, 20, MAROON);
    } else {
        char pathBuf[256] = "Duong di: ";
        for (int i = count - 1; i >= 1; i--) {
            strcat(pathBuf, TextFormat("%d", g.path[i]));
            if (i > 1) strcat(pathBuf, " -> ");
        }
        DrawText(pathBuf, 30, 65, 20, DARKGREEN);
        DrawText(TextFormat("|  Tong trong so: %d", g.L[v]), 35 + MeasureText(pathBuf, 20), 65, 20, RED);
    }
        EndDrawing();
    }
    CloseWindow();
}
int main() {
    system("cls"); 

    printf("\n\n");
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
    printf("%s%40s %-30s%s\n", C_WHITE, "Lop:", "25T_DT2", RESET);

    printf("\n\n\n");
    PrintCentered("Da Nang, 05/2026", C_WHITE);
    printf("\n%s%s%s", C_YELLOW, "================================================================================", RESET);
    Graph g;
    int u,v,c=0;
    char path[105],file[105];
    printf("\n%s[%s?%s] Nhap duong dan chua thu muc du lieu: %s", C_YELLOW, C_RED, C_YELLOW, RESET);
    scanf("%s",path);
    while (path[c]!='\0') {
        c++;
    }
    if (path[c-1]!='/') {
        path[c]='/';
        path[c+1]='\0';
    }
    printf("%s[%s?%s] Nhap ten file: %s", C_YELLOW, C_RED, C_YELLOW, RESET);
    scanf("%s",file);
    strcat(path,file);
    printf("\n");
    LoadingEffect("Dang kiem tra file");
    nhapdulieu(&g,&u,&v,path);
    int count;
    dijkstra(&g,u,v,&count);
    vedothi(g,u,v,count);
    return 0;
}