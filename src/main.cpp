#include <stdio.h>
#include <math.h>
#include <raylib.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define INF 1000000000
#define N  105
#define MAX_LOG_DISPLAY 20

// --- DINH NGHIA MAU CONSOLE ---
#define RESET     "\033[0m"
#define C_RED     "\033[31m" 
#define C_GREEN   "\033[32m"
#define C_YELLOW  "\033[33m"
#define C_CYAN    "\033[36m"
#define C_BOLD    "\033[1m"
#define C_WHITE   "\033[37m"

typedef enum { STATE_FIND_MIN, STATE_RELAX_EDGES, STATE_FINISHED } AlgoState;

struct Graph {
    int n;
    int A[N][N];
    Vector2 toado[N];
    int L[N];
    int T[N];    
    int par[N];  
};

// --- CAC HAM TRO GIUP ---

void MyDelay(float seconds) {
    clock_t start_time = clock(); 
    float duration = seconds * CLOCKS_PER_SEC;     
    while (clock() < start_time + duration); 
}

void AddLog(const char* msg, char stepLogs[300][128], int *logCount) {
    if (*logCount < 300) {
        strcpy(stepLogs[*logCount], msg);
        (*logCount)++;
    }
}

void PrintCentered(const char* text, const char* color) {
    int width = 80; 
    int len = strlen(text);
    int padding = (width - len) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s%s%s\n", color, text, RESET);
}

void UIConsole() {
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

void filename(char file[128], char path[256]) {
    printf("\n%s[%s?%s] Nhap duong dan thu muc (vd: D:/data): %s", C_YELLOW, C_RED, C_YELLOW, RESET);
    scanf("%s", path);
    int len = strlen(path);
    if (len > 0 && path[len - 1] != '/' && path[len - 1] != '\\') {
        strcat(path, "/");
    }
    printf("%s[%s?%s] Nhap ten file (vd: input.dat): %s", C_YELLOW, C_RED, C_YELLOW, RESET);
    scanf("%s", file);
    strcat(path, file);
}

void inputdata(struct Graph *g, int *u, int *v, const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) { 
        printf("\n%s[!] Loi: Khong tim thay file tai: %s%s\n", C_RED, path, RESET);
        MyDelay(2.0f);
        return; 
    }
    fscanf(f, "%d", &g->n);
    for (int i = 1; i <= g->n; i++)
        for (int j = 1; j <= g->n; j++)
            fscanf(f, "%d", &g->A[i][j]);
    for (int i = 1;i <= g->n; i++)
        for (int j = 1; j <= g->n; j++)
            if (g->A[i][j]==0)
                g->A[i][j]=-1;

    fscanf(f, "%d", u);
    fscanf(f, "%d", v);
    fclose(f);
}

void drawArrow(Vector2 start, Vector2 end, float rDinh, Color mau, int trongSo, float thickness) {
    float angle = atan2f(end.y - start.y, end.x - start.x);
    Vector2 tip = { end.x - (rDinh + 5) * cosf(angle), end.y - (rDinh + 5) * sinf(angle) };
    DrawLineEx(start, tip, thickness, mau);
    
    float arrowSize = 14.0f;
    Vector2 p1 = { tip.x + arrowSize * cosf(angle + PI - 0.45f), tip.y + arrowSize * sinf(angle + PI - 0.45f) };
    Vector2 p2 = { tip.x + arrowSize * cosf(angle + PI + 0.45f), tip.y + arrowSize * sinf(angle + PI + 0.45f) };
    DrawTriangle(tip, p2, p1, mau); 

    if (trongSo != -1) {
        char buf[10]; sprintf(buf, "%d", trongSo);
        Vector2 mid = { (start.x + tip.x) / 2, (start.y + tip.y) / 2 };
        DrawRectangle(mid.x - 2, mid.y - 2, MeasureText(buf, 20) + 8, 24, Fade(RAYWHITE, 0.7f));
        DrawText(buf, mid.x + 2, mid.y, 20, (mau.r > 200 && mau.g < 100) ? RED : DARKGRAY);
    }
}

// Ham tra ve 1: Chon file moi, tra ve 0: Thoat
int UIRaylib_Dijkstra(struct Graph g, int u_start, int v_target) {
    SetTraceLogLevel(LOG_NONE);
    char stepLogs[300][128];
    int logCount = 0;

    InitWindow(1400, 900, "PBL1 - Mo phong Dijkstra Step-by-Step - DUT");
    
    Vector2 center = { 430, 400 }; 
    float radius = 280.0f;
    
    // Ham reset thong so thuat toan
    auto ResetAlgo = [&]() {
        for (int i = 1; i <= g.n; i++) {
            float angle = (i - 1) * 2 * PI / g.n;
            g.toado[i] = (Vector2){ center.x + radius * cosf(angle), center.y + radius * sinf(angle) };
            g.L[i] = INF; g.T[i] = 1; g.par[i] = -1;
        }
        g.L[u_start] = 0;
        logCount = 0;
    };

    ResetAlgo();
    AlgoState state = STATE_FIND_MIN;
    int currentV = -1, neighborIdx = 1;
    float timer = 0.0f, speed = 1.2f; 
    char pathResult[512] = "";
    bool onPath[N] = { false };
    bool isStarted = false;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        
        // --- XU LY MENU SAU KHI KET THUC ---
        if (state == STATE_FINISHED) {
            if (IsKeyPressed(KEY_R)) { // Chay lai voi file hien tai
                ResetAlgo();
                state = STATE_FIND_MIN;
                memset(onPath, false, sizeof(onPath));
                isStarted = false;
                timer = 0;
            }
            if (IsKeyPressed(KEY_N)) { // Chon file moi
                CloseWindow();
                return 1; 
            }
        }

        if (!isStarted) {
            if (IsKeyPressed(KEY_SPACE)) {
                isStarted = true;
                AddLog("BAT DAU: Khoi tao L[nguon]=0, cac dinh khac = INF", stepLogs, &logCount);
            }
        } else {
            timer += GetFrameTime();
            if (timer >= speed && state != STATE_FINISHED) {
                timer = 0;
                if (state == STATE_FIND_MIN) {
                    int min_d = INF; currentV = -1;
                    for (int i = 1; i <= g.n; i++)
                        if (g.T[i] && g.L[i] < min_d) { min_d = g.L[i]; currentV = i; }

                    if (currentV == -1 || currentV == v_target) {
                        state = STATE_FINISHED;
                        if (g.L[v_target] != INF) {
                            int t = v_target;
                            int temp[N], count = 0;
                            while (t != -1) { onPath[t] = true; temp[count++] = t; t = g.par[t]; }
                            sprintf(pathResult, "LO TRINH: ");
                            for (int i = count - 1; i >= 0; i--) {
                                char b[15]; sprintf(b, "%d%s", temp[i], (i == 0 ? "" : " -> "));
                                strcat(pathResult, b);
                            }
                        } else sprintf(pathResult, "KHONG TIM THAY DUONG DI");
                        AddLog(">>> KET THUC: Da xac dinh lo trinh.", stepLogs, &logCount);
                    } else {
                        g.T[currentV] = 0; neighborIdx = 1; state = STATE_RELAX_EDGES;
                        AddLog(TextFormat("Chon dinh %d (L=%d) nho nhat", currentV, g.L[currentV]), stepLogs, &logCount);
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
                                AddLog(TextFormat("  + Toi uu L[%d] = %d", neighborIdx, newD), stepLogs, &logCount);
                            } else AddLog(TextFormat("  - Canh %d->%d khong toi uu", currentV, neighborIdx), stepLogs, &logCount);
                            neighborIdx++; found = true; break;
                        }
                        neighborIdx++;
                    }
                    if (!found) state = STATE_FIND_MIN;
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawRectangle(900, 0, 500, 900, Fade(LIGHTGRAY, 0.3f));
        DrawLineEx((Vector2){900, 0}, (Vector2){900, 900}, 2.0f, DARKGRAY);
        DrawText("NHAT KY THUAT TOAN", 940, 30, 24, DARKBLUE);
        
        if (!isStarted) DrawText("NHAN [SPACE] DE BAT DAU MO PHONG", 925, 80, 20, MAROON);

        for (int i = 0; i < logCount; i++) {
            if (i >= logCount - MAX_LOG_DISPLAY) {
                int pos = i - (logCount > MAX_LOG_DISPLAY ? logCount - MAX_LOG_DISPLAY : 0);
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
            else if (i == currentV) c = YELLOW;
            else if (g.T[i] == 0) c = LIME;
            DrawCircleV(g.toado[i], 32, c);
            DrawCircleLinesV(g.toado[i], 32, BLACK);
            DrawText(TextFormat("%d", i), g.toado[i].x - 10, g.toado[i].y - 12, 26, (onPath[i] && state==STATE_FINISHED)? WHITE : BLACK);
            if (g.L[i] < INF) DrawText(TextFormat("L=%d", g.L[i]), g.toado[i].x - 25, g.toado[i].y + 38, 18, MAROON);
        }

        if (state == STATE_FINISHED) {
            DrawRectangle(40, 680, 820, 200, Fade(GOLD, 0.9f));
            DrawRectangleLines(40, 680, 820, 200, ORANGE);
            DrawText("KET QUA CUOI CUNG:", 60, 695, 26, DARKBLUE);
            DrawText(pathResult, 60, 730, 22, BLACK);
            if (g.L[v_target] != INF) DrawText(TextFormat("TONG TRONG SO: %d", g.L[v_target]), 60, 765, 26, RED);
            
            DrawText("[R] CHAY LAI   [N] CHON FILE MOI   [ESC] THOAT", 60, 830, 24, DARKBLUE);
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}

int main() {
    int mode = 1;
    while (mode == 1) { 
        UIConsole();
        struct Graph g; 
        int u, v; 
        char path[256], file[128];
        filename(file, path);
        inputdata(&g, &u, &v, path);

        FILE* check = fopen(path, "r");
        if (!check) continue;
        fclose(check);

        printf("\n%sDang chuan bi he thong do hoa", C_CYAN);
        for(int i = 0; i < 40; i++) { printf("."); fflush(stdout); MyDelay(0.02f); }
        system("cls");

        mode = UIRaylib_Dijkstra(g, u, v); 
    }
    return 0;
}