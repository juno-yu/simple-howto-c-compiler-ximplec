int pipe(int pipefd[2]);
int main() {
  int fd[2];
  pipe(fd);
  return 0;
}
