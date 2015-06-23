#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <git2.h>

#define BRIGHT_GREEN "10"
#define CYAN "14"
#define YELLOW "11"
#define BLACK "0"
#define WHITE "8"
#define END_ARG ((char *) NULL)

int runProgram(char **args);
void ttyColor(char *color);
void ttyReset(void);
char *directory(void);

char hostname[256];
char cwd[1024];

int main() {
  char *gitstate = getenv("__git_ps1");
  gethostname(hostname, sizeof(hostname));


  ttyColor(CYAN);
  printf("@%s", hostname);

  ttyColor(YELLOW);
  printf(" %s %s", directory(), gitstate ? gitstate : "");

  ttyReset();
  printf("$ ");
  fflush(stdout);
}

char *directory() {
  char *homeDirectory();
  char *dir = homeDirectory();
  int len = strlen(dir);
  int i = 0;

  if (len > 25) {

    for (i = len - 1; dir[i] != '/'; --i) {
      ;
    }
    return "\\W";//dir + (i+1);

  }

  return "\\w";//dir;
}

char *homeDirectory() {
  int inHome = 1;
  int i = 0;
  char *home = getenv("HOME");
  int len = strlen(home);
  getcwd(cwd, sizeof(cwd));

  for (; inHome && i < len; ++i) {
    inHome = tolower(home[i]) == tolower(cwd[i]);
  }
  if (inHome) {
    cwd[len - 1] = '~';
    return cwd+(len - 1);
  }
  return cwd;
}

void ttyColor(char *color) {
  char* args[] = {"tput", "setaf", END_ARG, END_ARG};
  args[2] = color;
  runProgram(args);
  fflush(stdout);
}

void ttyReset(void) {
  char* args[] = {"tput", "sgr0", END_ARG};
  runProgram(args);
  fflush(stdout);
}

int runProgram(char **args) {
  int status;
  fflush(stdout);
  pid_t pid = fork();

  if (pid == -1) {
    printf(" no fork ");
    perror("fork error");
    exit(EXIT_FAILURE);
  }
  else if (pid == 0) {
    /* start child */
    printf(" A ");
    execvp(*args, args);
    printf(" C ");
    exit(1);
  }
  else {
    /* Wait for child*/
    if ((pid = wait(&status)) == -1) {
      perror("wait error");
    }
    else {
      if (WIFEXITED(status) != 0) {
        return WEXITSTATUS(status);
      }
      else {
        printf(" h ");
        return -1;
      }
    }
  }
  return 0;
}
