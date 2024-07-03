
#include <raylib.h>

int main() {

    InitWindow(700, 600, "Window");

    while (!WindowShouldClose()) {
        BeginDrawing();
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
