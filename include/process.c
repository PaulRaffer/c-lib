#include "process.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "conditional.h"


bool is_child(pid_t pid)
{
	return pid == 0;
}

bool is_parent(pid_t pid)
{
	return pid > 0;
}


// erstellt einen Kindprozess und führt in diesem eine Funktion aus
pid_t create_process(proc_t * proc)
{
	// Kindprozess erzeugen
	proc->pid = fork();
	if (is_child(proc->pid)) {
		// Der Kindprozess führt seine Funktion (proc->action)
		// mit dem eigenen Namen als Argument aus
		// und speichert das Ergebnis in proc->result
		// wenn ein Fehler aufgetreten ist, wird proc->result zurückgegeben
		IF(is_error, err_t, return, proc->result = proc->action(proc->name))
	}
	// Prozessnummer wird zurückgegeben
	return proc->pid;
}

// wartet auf Kindprozess
pid_t wait_for_process(proc_t const * proc)
{
	// auf Kindprozess warten
	return waitpid(proc->pid, NULL, 0);
}

// erstellt mehrere Prozesse, die gleichzeitig ausgeführt werden sollen
pid_t create_concurrent_processes(proc_t proc[], size_t num_proc)
{
	// für alle Prozesse im Array:
	for (size_t p = 0; p < num_proc; ++p) {
		// Es wird ein neuer Prozess erstellt, somit gibt es nun 2 in dieser Schleife.
		// Der Kindprozess verlässt jedoch die Schleife und nur der Vaterprozess
		// erzeugt weitere Kindprozesse.
		// Auch bei einem Fehler wird die Schleife abgebrochen.
		IF(!is_parent, pid_t, return, create_process(&proc[p]))
	}
	// Am Ende der Schleife haben alle erzeugten Kinder bereits die Schleife verlassen,
	// daher wird der Code nur vom Vaterprozess ausgeführt, was auch dem Aufrufer mitgeteilt wird.
	return parent;
}

// wartet auf mehrere Prozesse
pid_t wait_for_concurrent_processes(proc_t const proc[], size_t num_proc)
{
	err_t res = parent;
	// für alle Prozesse im Array:
	for (size_t p = 0; p < num_proc; ++p) {
		// Der Vaterprozess wartet auf alle Kindprozesse,
		// bei deren Erstellung kein Fehler aufgetreten ist
		if (is_parent(proc[p].pid)) {
			// Wenn beim Warten ein Fehler auftritt wird
			// zwar der Fehler in eine Hilfsvariable gespeichert, welche
			// später zurückgegeben wird, aber die Schleife nicht beendet,  
			// da noch auf weitere Prozesse gewartet werden muss
			IF(is_error, err_t, res =, wait_for_process(&proc[p]))
		}
	}
	return res;
}

// erstellt mehere Prozesse, die gleichzeitig ausgeführt werden sollen und wartet anschließend auf diese
pid_t create_and_wait_for_concurrent_processes(proc_t proc[], size_t num_proc)
{
	// mehrere Prozesse erstellen und alle Kindprozesse und Fehler an der Aufrufer weitergeben
	IF(!is_parent, pid_t, return, create_concurrent_processes(proc, num_proc))
	// Vaterprozess wartet auf alle Kindprozesse
	return wait_for_concurrent_processes(proc, num_proc);
}

// erstellt einen Prozess und wartet anschließend auf diesen
pid_t create_and_wait_for_sequenced_process(proc_t * proc)
{
	// Prozess erstellen und Kindprozess und Fehler an Aufrufer weitergeben
	IF(!is_parent, pid_t, return, create_process(proc))
	// Vaterprozess wartet auf den Kindprozess
	return wait_for_process(proc);
}

// erstellt mehrere Prozesse, die nacheinander ausgeführt werden sollen und wartet auf diese
pid_t create_and_wait_for_sequenced_processes(proc_t proc[], size_t num_proc)
{
	// für alle Prozesse im Array:
	for (size_t p = 0; p < num_proc; ++p) {
		// Kindprozess erstellen und darauf warten
		// Kindprozesse und Fehler an Aufrufer weitergeben
		IF(!is_parent, pid_t, return, create_and_wait_for_sequenced_process(&proc[p]))
	}
	// Dies ist der Vaterprozess, bereits alle Kindprozesse die Schleife verlassen haben
	return parent;
}

pid_t create_and_wait_for_sequenced_processes_and_call_a_function_after_each_of_them(proc_t proc[], size_t num_proc, err_t (*after)(size_t p))
{
        for (size_t p = 0; p < num_proc; ++p) {
                IF(!is_parent, pid_t, return, create_and_wait_for_sequenced_process(&proc[p]))
                after(p);
        }
        return parent;
}

pid_t create_and_wait_for_sequenced_processes_and_sleep_after_each_of_them(proc_t proc[], size_t num_proc, int s)
{
        for (size_t p = 0; p < num_proc; ++p) {
                IF(!is_parent, pid_t, return, create_and_wait_for_sequenced_process(&proc[p]))
                sleep(s);
        }
        return parent;
}

// gibt den Namen und die Nummer eines Prozesses und ein Label (z.B. BEGIN, END) aus
err_t print_process(str_t name, str_t label)
{
        return printf("PROCESS %s (pid: %d) %s\n", name, getpid(), label);
}
