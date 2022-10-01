#include "../headers.h"
#include "history.h"

void exit_shell() {
    add_to_history("exit");
    save_history();
    exit(0);
}