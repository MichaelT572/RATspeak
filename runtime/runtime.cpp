#include "runtime.h"

const int MAX_PATHFIND = 400;

std::vector<std::unique_ptr<Rat>> rats;
std::vector<std::unique_ptr<Cheese>> cheese;
std::vector<std::unique_ptr<Pill>> pills;
std::vector<std::unique_ptr<RatGen>> ratgens;
std::vector<std::unique_ptr<CheeseGen>> cheesegens;
std::vector<std::unique_ptr<PillGen>> pillgens;
std::vector<std::unique_ptr<Wall>> walls;
std::vector<std::unique_ptr<Trash>> trash;
std::vector<std::unique_ptr<Spring>> springs;
std::vector<std::unique_ptr<String>> strings;
std::vector<std::unique_ptr<Scanner>> scanners;
std::vector<std::unique_ptr<Poison>> poison;

Rat::Rat(int x, int y, int weight) {
    pos[0] = x; pos[1] = y;
    this->weight = weight;
}
void Rat::path_find() {
    path.clear();
    path_i = 0;

    bool found = false;
    for (int i = 0; i < cheese.size(); i++) {
        if (can_smell(cheese[i]->pos[0], cheese[i]->pos[1])) {
            found = true;
            break;
        }
    }
    if (!found) {
        return;
    }

    // BFS
    std::pair<int, int> directions[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};    
    std::vector<std::pair<int, int>> visited = {{pos[0], pos[1]}};
    std::queue<std::pair<int, int>> queue;
    queue.push({pos[0], pos[1]});
    std::map<std::pair<int, int>, std::pair<int, int>> parents;
    std::pair<int, int> end;
    
    found = false;
    while ((!found) && (visited.size() < MAX_PATHFIND) && (queue.size() > 0)) {
        std::pair<int, int> v = queue.front();
        queue.pop();

        for (int i = 0; i < cheese.size(); i++) {
            if ((v.first == cheese[i]->pos[0]) && (v.second == cheese[i]->pos[1])) {
                found = true;
                end = v;

                cheese[i]->seekers.insert(this);
                seeking = cheese[i].get();
                break;
            }
        }
        if (found) {break;}

        for (int i = 0; i < 4; i++) {
            std::pair<int, int> w = v;
            w.first += directions[i].first;
            w.second += directions[i].second;

            bool blocked = false;
            for (int j = 0; j < walls.size(); j++) {
                if ((w.first == walls[j]->pos[0]) && (w.second == walls[j]->pos[1])) {
                    blocked = true;
                    break;
                }
            }
            if (blocked) {continue;}
            for (int j = 0; j < visited.size(); j++) {
                if ((w.first == visited[j].first) && (w.second == visited[j].second)) {
                    blocked = true;
                    break;
                }
            }
            if (!blocked) {
                visited.push_back(w);
                queue.push(w);
                parents[w] = v;
            }
        }
    }

    if (found) {
        std::vector<std::pair<int, int>> temp;
        std::pair<int, int> v = end;
        while (!((v.first == pos[0]) && (v.second == pos[1]))) {
            temp.push_back(v);
            v = parents[v];
        }
        temp.push_back({pos[0], pos[1]});

        for (int i = temp.size() - 1; i >= 0; i--) {
            path.push_back(temp[i]);
        }
    }
}
void Rat::move_next() {
    if (path_i < ((int) path.size()) - 1) {
        path_i++;

        pos[0] = path[path_i].first;
        pos[1] = path[path_i].second;
        moved = true;
    } else {
        moved = false;
    }
}
bool Rat::can_smell(int x, int y) {
    int dx = x - pos[0];
    int dy = y - pos[1];

    if (dx * dx + dy * dy < MAX_PATHFIND * MAX_PATHFIND) {
        return true;
    }
    return false;
}

Cheese::Cheese(int x, int y, int count) {
    pos[0] = x; pos[1] = y;
    this->count = count;
}
void Cheese::update() {
    int n = cheese.size();
    for (int i = n - 1; i >= 0; i--) {
        Cheese* c = cheese[i].get();

        if ((c->pos[0] == this->pos[0]) && (c->pos[1] == this->pos[1]) && (c != this)) {
            count += c->count;
            cheese.erase(cheese.begin() + i);
        }
    }

    std::set<Rat*> temp = seekers;
    seekers.clear();

    for (Rat* rat : temp) {
        rat->path_find();
    }
    for (int j = 0; j < rats.size(); j++) {
        if (rats[j]->can_smell(pos[0], pos[1])) {
            rats[j]->path_find();
        }
    }
}

Pill::Pill(int x, int y, int count) {
    pos[0] = x; pos[1] = y;
    this->count = count;
}
void Pill::update() {
    int n = pills.size();
    for (int i = n - 1; i >= 0; i--) {
        Pill* p = pills[i].get();

        if ((p->pos[0] == this->pos[0]) && (p->pos[1] == this->pos[1]) && (p != this)) {
            count += p->count;
            pills.erase(pills.begin() + i);
        }
    }
}

RatGen::RatGen(int x, int y, int mode) {
    pos[0] = x; pos[1] = y;
    this->mode = mode;
}
void RatGen::activate() {
    if (!mode) {
        std::unique_ptr<Rat> rat = std::make_unique<Rat>(pos[0], pos[1], strength);
        rat->path_find();
        rats.push_back(std::move(rat));
    } else {
        std::string text;
        getline(std::cin, text);

        for (int i = 0; i < text.size(); i++) {
            std::unique_ptr<Rat> rat = std::make_unique<Rat>(pos[0], pos[1] + i, text[i]);
            rat->path_find();
            rats.push_back(std::move(rat));
        }
    }
}

CheeseGen::CheeseGen(int x, int y, int mode) {
    pos[0] = x; pos[1] = y;
    this->mode = mode;
}
void CheeseGen::activate() {
    if (!mode) {
        std::unique_ptr<Cheese> c = std::make_unique<Cheese>(pos[0], pos[1], strength);
        c->update();
        cheese.push_back(std::move(c));
    } else {
        std::string text;
        getline(std::cin, text);

        for (int i = 0; i < text.size(); i++) {
            std::unique_ptr<Cheese> c = std::make_unique<Cheese>(pos[0], pos[1] + i, text[i]);
            c->update();
            cheese.push_back(std::move(c));
        }
    }
}

PillGen::PillGen(int x, int y, int mode) {    
    pos[0] = x; pos[1] = y;
    this->mode = mode;
}
void PillGen::activate() {
    if (!mode) {
        std::unique_ptr<Pill> p = std::make_unique<Pill>(pos[0], pos[1], strength);
        p->update();
        pills.push_back(std::move(p));
    } else {
        std::string text;
        getline(std::cin, text);

        for (int i = 0; i < text.size(); i++) {
            std::unique_ptr<Pill> p = std::make_unique<Pill>(pos[0], pos[1] + i, text[i]);
            p->update();
            pills.push_back(std::move(p));
        }
    }
}

Wall::Wall(int x, int y) {
    pos[0] = x; pos[1] = y;
}

Trash::Trash(int x, int y) {
    pos[0] = x; pos[1] = y;
}

Spring::Spring(int x1, int y1, int x2, int y2) {
    pos[0] = x1; pos[1] = y1;
    pos2[0] = x2; pos2[1] = y2;
}
void Spring::activate() {
    for (int i = 0; i < rats.size(); i++) {
        if ((this->pos[0] == rats[i]->pos[0]) && (this->pos[1] == rats[i]->pos[1])) {
            rats[i]->pos[0] = pos2[0];
            rats[i]->pos[1] = pos2[1];

            rats[i]->seeking->seekers.erase(rats[i].get());
            rats[i]->path_find();
        }
    }
    for (int i = 0; i < cheese.size(); i++) {
        if ((this->pos[0] == cheese[i]->pos[0]) && (this->pos[1] == cheese[i]->pos[1])) {
            Cheese* c = cheese[i].get();
            
            c->pos[0] = pos2[0];
            c->pos[1] = pos2[1];

            c->update();
        }
    }
    for (int i = 0; i < pills.size(); i++) {
        if ((this->pos[0] == pills[i]->pos[0]) && (this->pos[1] == pills[i]->pos[1])) {
            pills[i]->pos[0] = pos2[0];
            pills[i]->pos[1] = pos2[1];
            
            pills[i]->update();
        }
    }
}

String::String(int x, int y) {
    pos[0] = x; pos[1] = y;
}
void String::activate(int strength) {
    active = true;

    for (String* string : conn_strings) {
        if (!string->active) {
            string->activate(strength);
        }
    }
    for (Spring* spring : conn_springs) {
        spring->activate();
    }
    for (RatGen* rg : conn_ratgens) {
        rg->strength += strength;
    }
    for (CheeseGen* cg : conn_cheesegens) {
        cg->strength += strength;        
    }
    for (PillGen* pg : conn_pillgens) {
        pg->strength += strength;
    }

    active = false;
}

Scanner::Scanner(int x, int y, int mode) {
    this->mode = mode;

    pos[0] = x; pos[1] = y;
}
void Scanner::activate(int x) {
    if (mode) {
        std::cout << x;
    } else {
        std::cout << (char) x;
    }
}

Poison::Poison(int x, int y) {
    pos[0] = x; pos[1] = y;
}



int find_funcdec(Program* ast, std::string id) {
    // Only nodes inside Program are funcdec

    for (int i = 0; i < ast->body.size(); i++) {
        FuncDec* func = (FuncDec*) ast->body[i].get();
        if (func->id == id) {
            return i;
        }
    }
    
    return -1;
}

void call_func(Program* ast, std::string id, int offset_x, int offset_y) {
    // Only nodes inside funcdec are call expressions

    int func_i = find_funcdec(ast, id);
    if (func_i == -1) {return;}
    FuncDec* func = ((FuncDec*) ast->body[func_i].get());

    for (int i = 0; i < func->body.size(); i++) {
        CallExpression* call = (CallExpression*) func->body[i].get();
        
        if (call->id == "🐀") {
            int x = std::stoi(((Literal*) call->args[0].get())->value) + offset_x;
            int y = std::stoi(((Literal*) call->args[1].get())->value) + offset_y;
            int weight = 0;
            if (call->args.size() == 3) {
                weight = std::stoi(((Literal*) call->args[2].get())->value);
            }
            rats.push_back(std::make_unique<Rat>(x, y, weight));

        } else if (call->id == "🧀") {
            int x = std::stoi(((Literal*) call->args[0].get())->value) + offset_x;
            int y = std::stoi(((Literal*) call->args[1].get())->value) + offset_y;
            int count = 1;
            if (call->args.size() == 3) {
                count = std::stoi(((Literal*) call->args[2].get())->value);
            }
            cheese.push_back(std::make_unique<Cheese>(x, y, count));

        } else if (call->id == "💊") {
            int x = std::stoi(((Literal*) call->args[0].get())->value) + offset_x;
            int y = std::stoi(((Literal*) call->args[1].get())->value) + offset_y;
            int count = 1;
            if (call->args.size() == 3) {
                count = std::stoi(((Literal*) call->args[2].get())->value);
            }
            std::unique_ptr<Pill> pill = std::make_unique<Pill>(x, y, count);
            pill->update();
            pills.push_back(std::move(pill));

        } else if (call->id == "🕳️") {
            int x = std::stoi(((Literal*) call->args[0].get())->value) + offset_x;
            int y = std::stoi(((Literal*) call->args[1].get())->value) + offset_y;
            int mode = 0;
            if (call->args.size() == 3) {
                mode = std::stoi(((Literal*) call->args[2].get())->value);
            }
            std::unique_ptr<RatGen> rg = std::make_unique<RatGen>(x, y, mode);
            for (int i = 0; i < strings.size(); i++) {
                if ((strings[i]->pos[0] == x) && (strings[i]->pos[1] == y)) {
                    strings[i]->conn_ratgens.push_back(rg.get());
                }
            }
            ratgens.push_back(std::move(rg));
            
        } else if (call->id == "🏭") {
            int x = std::stoi(((Literal*) call->args[0].get())->value) + offset_x;
            int y = std::stoi(((Literal*) call->args[1].get())->value) + offset_y;
            int mode = 0;
            if (call->args.size() == 3) {
                mode = std::stoi(((Literal*) call->args[2].get())->value);
            }
            std::unique_ptr<CheeseGen> cg = std::make_unique<CheeseGen>(x, y, mode);
            for (int i = 0; i < strings.size(); i++) {
                if ((strings[i]->pos[0] == x) && (strings[i]->pos[1] == y)) {
                    strings[i]->conn_cheesegens.push_back(cg.get());
                }
            }
            cheesegens.push_back(std::move(cg));

        } else if (call->id == "🧪") {
            int x = std::stoi(((Literal*) call->args[0].get())->value) + offset_x;
            int y = std::stoi(((Literal*) call->args[1].get())->value) + offset_y;
            int mode = 0;
            if (call->args.size() == 3) {
                mode = std::stoi(((Literal*) call->args[2].get())->value);
            }
            std::unique_ptr<PillGen> pg = std::make_unique<PillGen>(x, y, mode);
            for (int i = 0; i < strings.size(); i++) {
                if ((strings[i]->pos[0] == x) && (strings[i]->pos[1] == y)) {
                    strings[i]->conn_pillgens.push_back(pg.get());
                }
            }
            pillgens.push_back(std::move(pg));

        } else if (call->id == "📡") {
            int x = std::stoi(((Literal*) call->args[0].get())->value) + offset_x;
            int y = std::stoi(((Literal*) call->args[1].get())->value) + offset_y;
            int mode = 0;
            if (call->args.size() == 3) {
                mode = std::stoi(((Literal*) call->args[2].get())->value);
            }
            scanners.push_back(std::make_unique<Scanner>(x, y, mode));
        
        } else if (call->id == "🧱") {
            int x1 = std::stoi(((Literal*) call->args[0].get())->value) + offset_x;
            int y1 = std::stoi(((Literal*) call->args[1].get())->value) + offset_y;
            int x2 = x1;
            int y2 = y1;
            if (call->args.size() == 4) {
                x2 = std::stoi(((Literal*) call->args[2].get())->value) + offset_x;
                y2 = std::stoi(((Literal*) call->args[3].get())->value) + offset_y;
            }

            if ((x1 != x2) && (y1 != y2)) {
                throw std::runtime_error("Invalid arguments on line " + std::to_string(call->line) +
                        " in 🧱; lines can only be made orthogonally");
            }
            
            if (x1 > x2) {
                int temp = x1;
                x1 = x2;
                x2 = temp;
            }
            if (y1 > y2) {
                int temp = y1;
                y1 = y2;
                y2 = temp;
            }

            for (int x = x1; x <= x2; x++) {
                for (int y = y1; y <= y2; y++) {
                    walls.push_back(std::make_unique<Wall>(x, y));
                }
            }

        } else if (call->id == "🗑️") {
            int x = std::stoi(((Literal*) call->args[0].get())->value) + offset_x;
            int y = std::stoi(((Literal*) call->args[1].get())->value) + offset_y;
            trash.push_back(std::make_unique<Trash>(x, y));

        } else if (call->id == "🔼") {
            int x1 = std::stoi(((Literal*) call->args[0].get())->value) + offset_x;
            int y1 = std::stoi(((Literal*) call->args[1].get())->value) + offset_y;
            int x2 = std::stoi(((Literal*) call->args[2].get())->value) + offset_x;
            int y2 = std::stoi(((Literal*) call->args[3].get())->value) + offset_y;

            std::unique_ptr<Spring> spring = std::make_unique<Spring>(x1, y1, x2, y2);
            for (int i = 0; i < strings.size(); i++) {
                if ((strings[i]->pos[0] == x1) && (strings[i]->pos[1] == y1)) {
                    strings[i]->conn_springs.push_back(spring.get());
                }
            }
            springs.push_back(std::move(spring));

        } else if (call->id == "🧵") {
            int x1 = std::stoi(((Literal*) call->args[0].get())->value) + offset_x;
            int y1 = std::stoi(((Literal*) call->args[1].get())->value) + offset_y;
            int x2 = x1;
            int y2 = y1;
            if (call->args.size() == 4) {
                x2 = std::stoi(((Literal*) call->args[2].get())->value) + offset_x;
                y2 = std::stoi(((Literal*) call->args[3].get())->value) + offset_y;
            }

            if ((x1 != x2) && (y1 != y2)) {
                throw std::runtime_error("Invalid arguments on line " + std::to_string(call->line) +
                        " in 🧵; lines can only be made orthogonally");
            }
            
            if (x1 > x2) {
                int temp = x1;
                x1 = x2;
                x2 = temp;
            }
            if (y1 > y2) {
                int temp = y1;
                y1 = y2;
                y2 = temp;
            }

            String* last_string;
            for (int x = x1; x <= x2; x++) {
                for (int y = y1; y <= y2; y++) {
                    std::unique_ptr<String> string = std::make_unique<String>(x, y);
                    if (((x == x1) && (y == y1)) || ((x == x2) && (y == y2))) {
                        for (int i = 0; i < strings.size(); i++) {
                            if ((strings[i]->pos[0] == x) && (strings[i]->pos[1] == y)) {
                                strings[i]->conn_strings.push_back(string.get());
                            }
                        }
                    }
                    if ((x != x1) || (y != y1)) {
                        last_string->conn_strings.push_back(string.get());
                        string->conn_strings.push_back(last_string);
                    }

                    last_string = string.get();
                    strings.push_back(std::move(string));
                }
            }
        } else if (call->id == "☣") {
            int x = std::stoi(((Literal*) call->args[0].get())->value) + offset_x;
            int y = std::stoi(((Literal*) call->args[1].get())->value) + offset_y;
            poison.push_back(std::make_unique<Poison>(x, y));

        } else if (call->id == "tile") {
            std::string tile_func = ((Literal*) call->args[0].get())->value;
            int x1 = std::stoi(((Literal*) call->args[1].get())->value) + offset_x;
            int y1 = std::stoi(((Literal*) call->args[2].get())->value) + offset_y;
            int x2 = std::stoi(((Literal*) call->args[3].get())->value) + offset_x;
            int y2 = std::stoi(((Literal*) call->args[4].get())->value) + offset_y;

            if (x1 > x2) {
                int temp = x1;
                x1 = x2;
                x2 = temp;
            }
            if (y1 > y2) {
                int temp = y1;
                y1 = y2;
                y2 = temp;
            }

            for (int x = x1; x <= x2; x++) {
                for (int y = y1; y <= y2; y++) {
                    call_func(ast, tile_func, x, y);
                }
            }

        } else {
            int x = std::stoi(((Literal*) call->args[0].get())->value) + offset_x;
            int y = std::stoi(((Literal*) call->args[1].get())->value) + offset_y;
            call_func(ast, call->id, x, y);
        }
    }
}

bool tick() {
    for (int i = 0; i < rats.size(); i++) {
        Rat* rat = rats[i].get();
        
        rat->move_next();

        for (int j = cheese.size() - 1; j >= 0; j--) {
            Cheese* c = cheese[j].get();
            if ((c->pos[0] == rat->pos[0]) && (c->pos[1] == rat->pos[1])) {
                rat->weight += c->count;

                std::set<Rat*> seekers = std::move(c->seekers);
                cheese.erase(cheese.begin() + j);

                for (Rat* rat2 : seekers) {
                    rat2->path_find();
                }
            }
        }

        for (int j = pills.size() - 1; j >= 0; j--) {
            Pill* p = pills[j].get();
            if ((p->pos[0] == rat->pos[0]) && (p->pos[1] == rat->pos[1])) {
                rat->weight -= p->count;
                pills.erase(pills.begin() + j);
            }
        }

        if ((rat->moved) && (rat->weight >= 0)) {
            for (int j = 0; j < strings.size(); j++) {
                if ((strings[j]->pos[0] == rat->pos[0]) && (strings[j]->pos[1] == rat->pos[1])
                    && (strings[j]->conn_ratgens.size() == 0)
                    && (strings[j]->conn_cheesegens.size() == 0)
                    && (strings[j]->conn_pillgens.size() == 0)
                    && (strings[j]->conn_springs.size() == 0)) {
                    
                        strings[j]->activate(rat->weight);
                }
            }
        }

        for (int j = 0; j < scanners.size(); j++) {
            if ((scanners[j]->pos[0] == rat->pos[0]) && (scanners[j]->pos[1] == rat->pos[1])) {
                scanners[j]->activate(rat->weight);
            }
        }

        for (int j = 0; j < poison.size(); j++) {
            if ((poison[j]->pos[0] == rat->pos[0]) && (poison[j]->pos[1] == rat->pos[1])) {
                return false;
            }
        }
    }

    for (int i = 0; i < trash.size(); i++) {
        Trash* t = trash[i].get();

        for (int j = rats.size() - 1; j >= 0; j--) {
            Rat* rat = rats[j].get();
            if ((rat->pos[0] == t->pos[0]) && (rat->pos[1] == t->pos[1])) {
                rat->seeking->seekers.erase(rat);
                rats.erase(rats.begin() + j);
            }
        }
        for (int j = cheese.size() - 1; j >= 0; j--) {
            Cheese* c = cheese[j].get();
            if ((c->pos[0] == t->pos[0]) && (c->pos[1] == t->pos[1])) {
                std::set<Rat*> seekers = std::move(c->seekers);
                cheese.erase(cheese.begin() + j);

                for (Rat* rat : seekers) {
                    rat->path_find();
                }
            }
        }
        for (int j = pills.size() - 1; j >= 0; j--) {
            Pill* p = pills[j].get();
            if ((p->pos[0] == t->pos[0]) && (p->pos[1] == t->pos[1])) {
                pills.erase(pills.begin() + j);
            }
        }
    }

    for (int i = 0; i < ratgens.size(); i++) {
        RatGen* rg = ratgens[i].get();
        if (rg->strength != 0) {
            rg->activate();
            rg->strength = 0;
        }
    }
    for (int i = 0; i < cheesegens.size(); i++) {
        CheeseGen* cg = cheesegens[i].get();
        if (cg->strength != 0) {
            cg->activate();
            cg->strength = 0;
        }
    }
    for (int i = 0; i < pillgens.size(); i++) {
        PillGen* pg = pillgens[i].get();
        if (pg->strength != 0) {
            pg->activate();
            pg->strength = 0;
        }
    }

    return true;
}

void run() {
    for (int i = 0; i < cheese.size(); i++) {
        Cheese* c = cheese[i].get();
        c->update();
    }

    int tick_n = 0;
    while (tick()) {
        // std::cout << "Tick " << tick_n << ": " << cheese.size() << std::endl;
        // for (int i = 0; i < cheese.size(); i++) {
        //     std::cout << "- " << cheese[i]->count << std::endl;
        // }

        tick_n++;
    }
}