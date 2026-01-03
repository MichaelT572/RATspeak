#include "display.h"

void clear_screen() {
    std::cout << "\x1b[2J\x1b[H";
}
void hide_cursor() {
    std::cout << "\x1b[?25l";
}
void show_cursor() {
    std::cout << "\x1b[?25h";
}
std::string visible_at(int x, int y) {
    // trash
    // cheese
    // poison
    // rats
    // pills
    // scanners
    // ratgens
    // cheesegens
    // pillgens
    // springs
    // walls
    // strings
    for (int i = 0; i < trash.size(); i++) {
        Trash* t = trash[i].get();
        if ((t->pos[0] == x) && (t->pos[1] == y)) {
            return t->emoji;
        }
    }
    for (int i = 0; i < cheese.size(); i++) {
        Cheese* c = cheese[i].get();
        if ((c->pos[0] == x) && (c->pos[1] == y)) {
            return c->emoji;
        }
    }
    for (int i = 0; i < poison.size(); i++) {
        Poison* p = poison[i].get();
        if ((p->pos[0] == x) && (p->pos[1] == y)) {
            return p->emoji;
        }
    }
    for (int i = 0; i < rats.size(); i++) {
        Rat* r = rats[i].get();
        if ((r->pos[0] == x) && (r->pos[1] == y)) {
            return r->emoji;
        }
    }
    for (int i = 0; i < pills.size(); i++) {
        Pill* p = pills[i].get();
        if ((p->pos[0] == x) && (p->pos[1] == y)) {
            return p->emoji;
        }
    }
    for (int i = 0; i < scanners.size(); i++) {
        Scanner* s = scanners[i].get();
        if ((s->pos[0] == x) && (s->pos[1] == y)) {
            return s->emoji;
        }
    }
    for (int i = 0; i < ratgens.size(); i++) {
        RatGen* rg = ratgens[i].get();
        if ((rg->pos[0] == x) && (rg->pos[1] == y)) {
            return rg->emoji;
        }
    }
    for (int i = 0; i < cheesegens.size(); i++) {
        CheeseGen* cg = cheesegens[i].get();
        if ((cg->pos[0] == x) && (cg->pos[1] == y)) {
            return cg->emoji;
        }
    }
    for (int i = 0; i < pillgens.size(); i++) {
        PillGen* pg = pillgens[i].get();
        if ((pg->pos[0] == x) && (pg->pos[1] == y)) {
            return pg->emoji;
        }
    }
    for (int i = 0; i < springs.size(); i++) {
        Spring* s = springs[i].get();
        if ((s->pos[0] == x) && (s->pos[1] == y)) {
            return s->emoji;
        }
    }
    for (int i = 0; i < walls.size(); i++) {
        Wall* wall = walls[i].get();
        if ((wall->pos[0] == x) && (wall->pos[1] == y)) {
            return wall->emoji;
        }
    }
    for (int i = 0; i < strings.size(); i++) {
        String* s = strings[i].get();
        if ((s->pos[0] == x) && (s->pos[1] == y)) {
            return s->emoji;
        }
    }
    return "  ";
}

int VIEW_X;
int VIEW_Y;
int VIEW_W;
int VIEW_H;

bool initialized = false;
struct termios old_termios;
void init_input() {
    if (initialized) return;

    tcgetattr(STDIN_FILENO, &old_termios);

    struct termios raw = old_termios;
    raw.c_lflag &= ~(ICANON | ECHO); // Raw input
    raw.c_cc[VMIN]  = 0;             // Non-blocking
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    // Non-blocking
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    initialized = true;
}
void shutdown_input() {
    if (!initialized) return;
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);

    // Blocking
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL, flags & ~O_NONBLOCK);
    
    initialized = false;
}
Key poll_key() {
    unsigned char c;
    if (read(STDIN_FILENO, &c, 1) != 1) {
        return Key::NONE;
    }

    if (c == 'q') {
        return Key::QUIT;
    } else if (c == ' ') {
        return Key::NEXT;
    } else if (c == 0x1b) {
        unsigned char seq[2];
        if ((read(STDIN_FILENO, &seq[0], 1) != 1) || (read(STDIN_FILENO, &seq[1], 1) != 1)) {
            return Key::NONE;
        }
    
        if (seq[0] == '[') {
            if (seq[1] == 'A') {
                return Key::U;
            } else if (seq[1] == 'B') {
                return Key::D;
            } else if (seq[1] == 'C') {
                return Key::R;
            } else if (seq[1] == 'D') {
                return Key::L;
            }
        }
    }

    return Key::NONE;
}



void render() {
    hide_cursor();

    std::cout << "\n";
    for (int i = 0; i < VIEW_W + 2; i++) {
        std::cout << "⬜";
    }
    std::cout << "\n";

    for (int y = VIEW_H - 1; y >= 0; y--) {
        std::cout << "⬜";
        for (int x = 0; x < VIEW_W; x++) {
            std::cout << visible_at(x + VIEW_X, y + VIEW_Y);
        }
        std::cout << "⬜\n";
    }

    for (int i = 0; i < VIEW_W + 2; i++) {
        std::cout << "⬜";
    }
    std::cout << "\n";

    std::cout.flush();
    show_cursor();
}

