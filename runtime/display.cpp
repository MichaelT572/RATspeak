#include "display.h"

int VIEW_X;
int VIEW_Y;
const int VIEW_W = 40;
const int VIEW_H = 20;

std::string viewport[VIEW_W * VIEW_H];

void clear_screen() {
    std::cout << "\x1b[2J\x1b[H";
}
void hide_cursor() {
    std::cout << "\x1b[?25l";
}
void show_cursor() {
    std::cout << "\x1b[?25h";
}

void render_viewport() {
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

    for (int y = 0; y < VIEW_H; y++) {
        for (int x = 0; x < VIEW_W; x++) {
            viewport[y*VIEW_W + x] = "  ";
        }    
    }



    for (int i = 0; i < strings.size(); i++) {
        String* s = strings[i].get();
        if (((s->pos[0] - VIEW_X) < VIEW_W) && ((s->pos[0] - VIEW_X) >= 0) 
        && ((s->pos[1] - VIEW_Y) < VIEW_H) && ((s->pos[1] - VIEW_Y) >= 0)) {
            viewport[(s->pos[1] - VIEW_Y) * VIEW_W + (s->pos[0] - VIEW_X)] = s->emoji;
        }
    }
    for (int i = 0; i < walls.size(); i++) {
        Wall* wall = walls[i].get();
        if (((wall->pos[0] - VIEW_X) < VIEW_W) && ((wall->pos[0] - VIEW_X) >= 0) 
        && ((wall->pos[1] - VIEW_Y) < VIEW_H) && ((wall->pos[1] - VIEW_Y) >= 0)) {
            viewport[(wall->pos[1] - VIEW_Y) * VIEW_W + (wall->pos[0] - VIEW_X)] = wall->emoji;
        }
    }
    for (int i = 0; i < springs.size(); i++) {
        Spring* s = springs[i].get();
        if (((s->pos[0] - VIEW_X) < VIEW_W) && ((s->pos[0] - VIEW_X) >= 0) 
        && ((s->pos[1] - VIEW_Y) < VIEW_H) && ((s->pos[1] - VIEW_Y) >= 0)) {
            viewport[(s->pos[1] - VIEW_Y) * VIEW_W + (s->pos[0] - VIEW_X)] = s->emoji;
        }
    }
    for (int i = 0; i < pillgens.size(); i++) {
        PillGen* pg = pillgens[i].get();
        if (((pg->pos[0] - VIEW_X) < VIEW_W) && ((pg->pos[0] - VIEW_X) >= 0) 
        && ((pg->pos[1] - VIEW_Y) < VIEW_H) && ((pg->pos[1] - VIEW_Y) >= 0)) {
            viewport[(pg->pos[1] - VIEW_Y) * VIEW_W + (pg->pos[0] - VIEW_X)] = pg->emoji;
        }
    }
    for (int i = 0; i < cheesegens.size(); i++) {
        CheeseGen* cg = cheesegens[i].get();
        if (((cg->pos[0] - VIEW_X) < VIEW_W) && ((cg->pos[0] - VIEW_X) >= 0) 
        && ((cg->pos[1] - VIEW_Y) < VIEW_H) && ((cg->pos[1] - VIEW_Y) >= 0)) {
            viewport[(cg->pos[1] - VIEW_Y) * VIEW_W + (cg->pos[0] - VIEW_X)] = cg->emoji;
        }
    }
    for (int i = 0; i < ratgens.size(); i++) {
        RatGen* rg = ratgens[i].get();
        if (((rg->pos[0] - VIEW_X) < VIEW_W) && ((rg->pos[0] - VIEW_X) >= 0) 
        && ((rg->pos[1] - VIEW_Y) < VIEW_H) && ((rg->pos[1] - VIEW_Y) >= 0)) {
            viewport[(rg->pos[1] - VIEW_Y) * VIEW_W + (rg->pos[0] - VIEW_X)] = rg->emoji;
        }
    }
    for (int i = 0; i < scanners.size(); i++) {
        Scanner* s = scanners[i].get();
        if (((s->pos[0] - VIEW_X) < VIEW_W) && ((s->pos[0] - VIEW_X) >= 0) 
        && ((s->pos[1] - VIEW_Y) < VIEW_H) && ((s->pos[1] - VIEW_Y) >= 0)) {
            viewport[(s->pos[1] - VIEW_Y) * VIEW_W + (s->pos[0] - VIEW_X)] = s->emoji;
        }
    }
    for (int i = 0; i < pills.size(); i++) {
        Pill* p = pills[i].get();
        if (((p->pos[0] - VIEW_X) < VIEW_W) && ((p->pos[0] - VIEW_X) >= 0) 
        && ((p->pos[1] - VIEW_Y) < VIEW_H) && ((p->pos[1] - VIEW_Y) >= 0)) {
            viewport[(p->pos[1] - VIEW_Y) * VIEW_W + (p->pos[0] - VIEW_X)] = p->emoji;
        }
    }
    for (int i = 0; i < rats.size(); i++) {
        Rat* r = rats[i].get();
        if (((r->pos[0] - VIEW_X) < VIEW_W) && ((r->pos[0] - VIEW_X) >= 0) 
        && ((r->pos[1] - VIEW_Y) < VIEW_H) && ((r->pos[1] - VIEW_Y) >= 0)) {
            viewport[(r->pos[1] - VIEW_Y) * VIEW_W + (r->pos[0] - VIEW_X)] = r->emoji;
        }
    }
    for (int i = 0; i < poison.size(); i++) {
        Poison* p = poison[i].get();
        if (((p->pos[0] - VIEW_X) < VIEW_W) && ((p->pos[0] - VIEW_X) >= 0) 
        && ((p->pos[1] - VIEW_Y) < VIEW_H) && ((p->pos[1] - VIEW_Y) >= 0)) {
            viewport[(p->pos[1] - VIEW_Y) * VIEW_W + (p->pos[0] - VIEW_X)] = p->emoji;
        }
    }
    for (int i = 0; i < cheese.size(); i++) {
        Cheese* c = cheese[i].get();
        if (((c->pos[0] - VIEW_X) < VIEW_W) && ((c->pos[0] - VIEW_X) >= 0) 
        && ((c->pos[1] - VIEW_Y) < VIEW_H) && ((c->pos[1] - VIEW_Y) >= 0)) {
            viewport[(c->pos[1] - VIEW_Y) * VIEW_W + (c->pos[0] - VIEW_X)] = c->emoji;
        }
    }
    for (int i = 0; i < trash.size(); i++) {
        Trash* t = trash[i].get();
        if (((t->pos[0] - VIEW_X) < VIEW_W) && ((t->pos[0] - VIEW_X) >= 0) 
        && ((t->pos[1] - VIEW_Y) < VIEW_H) && ((t->pos[1] - VIEW_Y) >= 0)) {
            viewport[(t->pos[1] - VIEW_Y) * VIEW_W + (t->pos[0] - VIEW_X)] = t->emoji;
        }
    }
}

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
    } else if (c == 's') {
        return Key::DEBUG;
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
    render_viewport();

    hide_cursor();

    std::cout << "\n";
    for (int i = 0; i < VIEW_W + 2; i++) {
        std::cout << "⬜";
    }
    std::cout << "\n";

    for (int y = VIEW_H - 1; y >= 0; y--) {
        std::cout << "⬜";
        for (int x = 0; x < VIEW_W; x++) {
            std::cout << viewport[VIEW_W * y + x];
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

