
#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

#include <cvecs.h>

#define WINDOW_START_WIDTH  1200
#define WINDOW_START_HEIGHT  800

static Color background_color;
static const char * current_dir;
static Font default_font;
static Vec current_files;

typedef enum FileType {
    File = 0,
    Directory,
    Other
} FileType;

typedef struct DirEntry {
    const char * name;
    FileType type;
} DirEntry;

void preload() {
    // In future the config will be read here
    background_color = (Color){ 25, 25, 25, 255 };
    default_font = LoadFont("assets/DejaVuSansMono.ttf");
    if (!IsFontReady(default_font)) {
        fprintf( stderr, "Default could not be loaded!\n" );
        exit(1);
    }

    current_files = createVec();
}

bool loadFilesInDir() {
    DIR * dir = opendir(current_dir);
    if (!dir ) {
        fprintf(stderr, "Directory could not be opened\n");
        return false;
    }

    struct dirent * file;
    while ((file = readdir(dir))) {
        // printf("---%s---\n", file->d_name);
        DirEntry * entry = malloc(sizeof(DirEntry));
        if (!entry) {
            fprintf(stderr, "Malloc failed %s %d\n", __FILE__, __LINE__);
            exit(1);
        }

        entry->name = copyCString(file->d_name);
        switch (file->d_type) {
            case DT_REG:
                entry->type = File;
                break;
            case DT_LNK:
            case DT_DIR:
                entry->type = Directory;
                break;
            case DT_BLK:
            case DT_CHR:
            case DT_WHT:
            case DT_FIFO:
            case DT_SOCK:
            default:
                entry->type = Other;
                break;
        }
        
        if (!appendVec(&current_files, entry)) {
            fprintf(stderr, "File list could not be created\n");
            return false;
        }
    }
    
    if (closedir(dir) < 0) {
        fprintf(stderr, "Closedir failed\n");
        return false;
    }

    return true;
}

void freeCurrentFiles() {
    for (size_t i = 0; i < current_files.count; i++) {
        free(current_files.entries[i].val);
    }
    deleteVec(&current_files, 0, current_files.count);
}

bool reloadCurrentDir(const char * new_dir) {
    if (strlen(new_dir) == 0) return false;
    current_dir = new_dir;
    freeCurrentFiles();
    if (!loadFilesInDir()) return false;
    return true;
}

bool getStrListOfFiles(StrVec * vec, bool with_hidden) {
    if (!vec) return false;

    for (size_t i = 0; i < current_files.count; i++) {
        DirEntry * entry = current_files.entries[i].val;
        const char * name = entry->name;
        if (name[0] != '.' || with_hidden) {
            if (!appendStrVec(vec, name)) return false;
        }
    }
    return true;
}

int main() {

    InitWindow(WINDOW_START_WIDTH, WINDOW_START_HEIGHT, "Window");
    SetTargetFPS(60);
    preload();

    // Load Home Directory at start
    reloadCurrentDir(TextFormat("/home/%s", getenv("USER")));

    while (!WindowShouldClose()) {

        ClearBackground( background_color );

        BeginDrawing();

            StrVec not_hidden_files = createStrVec();
            getStrListOfFiles(&not_hidden_files, false);
            for( size_t i = 0; i < not_hidden_files.count; i++) {
                DrawTextEx(default_font, not_hidden_files.vals[i], (Vector2){ 50, 30 * i + 10 + 10 * i }, 24, 3, WHITE);
            }
            freeStrVec(not_hidden_files);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
