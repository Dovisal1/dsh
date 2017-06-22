#ifndef BUILTIN_H
#define BUILTIN_H

#include "cmd.h"

int is_builtin(struct cmd *cmd);
size_t builtin_num(struct cmd *cmd);

void run_echo(struct cmd *cmd);
void run_cd(struct cmd *cmd);
void run_history(struct cmd *cmd);

#endif
