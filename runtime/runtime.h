#ifndef RUNTIME_H
#define RUNTIME_H

#include "../front/parser.h"
#include "display.h"
#include <queue>
#include <map>

extern std::unique_ptr<Node> ast;
void build_func(std::string id, int offset_x, int offset_y);
void run(bool visualize);

class Obj {
public:
    int pos[2];
    std::string emoji;
};

class Cheese;
class Rat : public Obj {
public:
    int weight;
    std::vector<std::pair<int, int>> path;
    int path_i;
    Cheese* seeking;
    bool moved;

    Rat(int x, int y, int weight);
    void path_find();
    void move_next();
    bool can_smell(int x, int y);
};

class Cheese : public Obj {
public:
    int count;
    std::set<Rat*> seekers;

    Cheese(int x, int y, int count);
    void update();
};

class Pill : public Obj {
public:
    int count;

    Pill(int x, int y, int count);
    void update();
};

class RatGen : public Obj {
public:
    int mode;
    int strength = 0;

    RatGen(int x, int y, int mode);
    void activate();
};

class CheeseGen : public Obj {
public:
    int mode;
    int strength = 0;

    CheeseGen(int x, int y, int mode);
    void activate();
};

class PillGen : public Obj {
public:
    int mode;
    int strength = 0;

    PillGen(int x, int y, int mode);
    void activate();
};

class Wall : public Obj {
public:
    Wall(int x, int y);
};

class Trash : public Obj {
public:
    Trash(int x, int y);
};

class Spring : public Obj {
public:
    int pos2[2];
    
    Spring(int x1, int y1, int x2, int y2);
    void activate();
};

class String : public Obj {
public:
    bool active;
    
    std::vector<String*> conn_strings;
    std::vector<Spring*> conn_springs;
    std::vector<RatGen*> conn_ratgens;
    std::vector<CheeseGen*> conn_cheesegens;
    std::vector<PillGen*> conn_pillgens;

    String(int x, int y);
    void activate(int strength);
    void deactivate();
};

class Scanner : public Obj {
public:
    int mode;

    Scanner(int x, int y, int mode);
    void activate(int x);
};

class Poison : public Obj {
public:
    Poison(int x, int y);
};



extern std::vector<std::unique_ptr<Rat>> rats;
extern std::vector<std::unique_ptr<Cheese>> cheese;
extern std::vector<std::unique_ptr<Pill>> pills;
extern std::vector<std::unique_ptr<RatGen>> ratgens;
extern std::vector<std::unique_ptr<CheeseGen>> cheesegens;
extern std::vector<std::unique_ptr<PillGen>> pillgens;
extern std::vector<std::unique_ptr<Wall>> walls;
extern std::vector<std::unique_ptr<Trash>> trash;
extern std::vector<std::unique_ptr<Spring>> springs;
extern std::vector<std::unique_ptr<String>> strings;
extern std::vector<std::unique_ptr<Scanner>> scanners;
extern std::vector<std::unique_ptr<Poison>> poison;

#endif