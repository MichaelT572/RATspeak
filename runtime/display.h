#ifndef DISPLAY_H
#define DISPLAY_H

#include "runtime.h"

extern int VIEW_X;
extern int VIEW_Y;
extern int VIEW_W;
extern int VIEW_H;

void render();
void clear_screen();

// OS DEPENDENT
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
enum class Key {L, R, U, D, NEXT, QUIT, NONE};
void init_input();
void shutdown_input();
Key poll_key();

#endif