#ifndef PROCESS_H
#define PROCESS_H


#include <stdbool.h>
#include <sys/types.h>

#include "any.h"
#include "error.h"
#include "str.h"


enum {
	child = 0,
	parent = 1,
};

bool is_child(pid_t pid);
bool is_parent(pid_t pid);


typedef err_t (*action_t)(str_t name, any_t data);
typedef struct {
	// Prozessnummer
	pid_t pid;
	// Funktion die der Prozess ausführen soll
	action_t action;
	// Rückgabewert den Funktion
	err_t result;
	// Name des Prozesses
	str_t name;
	// Daten die an die Funktion übergeben werden sollen
	any_t data;
} proc_t;
	
pid_t create_process(proc_t * proc);
pid_t wait_for_process(proc_t const * proc);
pid_t create_concurrent_processes(proc_t proc[], size_t num_proc);
pid_t wait_for_concurrent_processes(proc_t const proc[], size_t num_proc);
pid_t create_and_wait_for_concurrent_processes(proc_t proc[], size_t num_proc);
pid_t create_and_wait_for_sequenced_process(proc_t * proc);
pid_t create_and_wait_for_sequenced_processes(proc_t proc[], size_t num_proc);
pid_t create_and_wait_for_sequenced_processes_and_call_a_function_after_each_of_them(proc_t proc[], size_t num, err_t (*after)(size_t p));
pid_t create_and_wait_for_sequenced_processes_and_sleep_after_each_of_them(proc_t proc[], size_t num_proc, int s);

err_t print_process(str_t name, str_t label);


#endif // PROCESS_H
