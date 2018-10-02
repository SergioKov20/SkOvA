/*
 * entry.h - Definició del punt d'entrada de les crides al sistema
 */

#ifndef __ENTRY_H__
#define __ENTRY_H__

#endif  /* __ENTRY_H__ */

void keyboard_handler();
void clock_handler();
void syscall_handler_sysenter();

void writeMSR(long msrnum, long newvalue);
