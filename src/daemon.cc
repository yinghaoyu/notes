#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
  pid_t pid = fork();  // step one
  if(pid < 0)
  {
    return fasle;
  }
  else if (pid > 0)  // father exit
  {
      exit(0);
  }
  umask(0);  //step two
  pid_t sid = setsid();  // step three
  if（sid < 0）
  {
    return 0;
  }
  if(chdir("/") < 0)  // step four
  {
    return 0;
  }
  close(STDIN_FILENO);  // step five
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  open("/dev/null", O_RDONLY);  // make sure stdin, stdout , stderr to dev/null
  open("/dev/null", O_RDWR);
  open("/dev/null", O_RDWR);
  while(true)
  {
    //do something
  }
  return 0;
}
