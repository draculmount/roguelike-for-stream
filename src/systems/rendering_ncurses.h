#ifndef RENDERING_NCURSES_H 
#define RENDERING_NCURSES_H 

#include "../components/components.h"
#include "systems.h"

void rendering_ncurses_init(struct Database_Handle dbh);
void rendering_ncurses_update(struct Database_Handle dbh);
void rendering_ncurses_cleanup(struct Database_Handle dbh);

struct Point get_offset();

void print_room_ncurses(struct Graph* dag, int x, int y);
void print_debug_room_info(struct Graph* dag, int x, int y);

static struct subsystem rendering_ncurses = {
	.name = "rendering_ncurses",
	.init_ptr = &rendering_ncurses_init,
	.update_ptr = &rendering_ncurses_update,
	.cleanup_ptr = &rendering_ncurses_cleanup
};

#endif
