#ifndef SHELL_H_
#define SHELL_H_
void welcome_message(const char *version);
void progress_bar(float iteration, float total);
void clear_c();
void aurora_shell(const char* userName, const char* hostName, const char * rootDir,char* currentDir, int exitCode);
#endif