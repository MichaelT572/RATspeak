#ifndef DISPLAY_H
#define DISPLAY_H

#include "runtime.h"

extern int VIEW_X;
extern int VIEW_Y;
extern const int VIEW_W;
extern const int VIEW_H;

void render();
void clear_screen();

// OS DEPENDENT
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
enum class Key {L, R, U, D, NEXT, DEBUG, QUIT, NONE};
void init_input();
void shutdown_input();
Key poll_key();

#endif