#include "command.h"
#include "interface.h"
#include "cpu.h"

static char *feedback = "";

void commandSize(char **token, size_t token_len) {

}

void commandSave(char **token, size_t token_len) {

}

void commandRun(void) {
	setRunning(true);
	feedback = "Running...";
}

void commandHalt(void) {
	setRunning(false);
	feedback = "Halted system.";
}

void commandReset(void) {
	resetCPU();
	feedback = "Reset CPU.";
}

void commandMem(char **token, size_t token_len) {

}

char *getFeedback(void) {
	return feedback;
}

void setFeedback(char *f) {
	feedback = f;
}