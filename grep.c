// Simple grep.  Only supports ^ . * $ operators.

#include "types.h"
#include "stat.h"
#include "user.h"

char buf[1024];
int match(char*, char*);

void change_to_lowercase(char *s);


//now supports "-i"(ignore case), "-n"(line number), "-v"(invert match)
void
grep(char *pattern, int fd, int ignore_case, int show_line_number, int invert_match)
{
  int n, m;
  char *p, *q;

  int line_num = 1;  //for "-n" flag
  char line_buffer[1024];
  char pattern_buffer[128];

  strcpy(pattern_buffer, pattern); //copy pattern to pattern_buffer
  //if "-i" flag passed, lowercase it all
  if (ignore_case) {
    change_to_lowercase(pattern_buffer);
  }

  m = 0;
  while((n = read(fd, buf+m, sizeof(buf)-m-1)) > 0){
    m += n;
    buf[m] = '\0';
    p = buf;
    while((q = strchr(p, '\n')) != 0){
      *q = 0;
      strcpy(line_buffer, p); //copy p into line_buffer
      //if "-i" flag passed, lowercase it all
      if (ignore_case) {
        change_to_lowercase(line_buffer);
      } 
      
      int pattern_match = match(pattern_buffer, line_buffer); //holds if they are a match

      //if "-v" flag detected, invert
      if ((pattern_match && !invert_match) || (!pattern_match && invert_match)) {
        //if "-n" show line numbers also
        if (show_line_number) {
          printf(1, "%d: ", line_num);
        }
        printf(1, "%s\n", p);
      }
      line_num++; //increment for each iteration so prints correct line
      p = q + 1;
    }

    if(p == buf)
      m = 0;
    if(m > 0){
      m -= p - buf;
      memmove(buf, p, m);
    }
  }
}

int
main(int argc, char *argv[])
{
  int fd = 0;
  char *pattern = 0;
  int ignore_case = 0;  //"-i" flag
  int show_line_number = 0; //"-n" flag
  int invert_match = 0; //"-v" flag
  int argi = 1; //holds how many arguments are passed so multiple flags can be processed

  //now supports flags
  if(argc < 2){
    printf(2, "usage: grep pattern [file ...]\n");
    exit();
  }

  //look for flags
  while(argi < argc && argv[argi][0] == '-') {
    if (strchr(argv[argi], 'i')) ignore_case = 1; //flips to true
    if (strchr(argv[argi], 'n')) show_line_number = 1;  //flips to true
    if (strchr(argv[argi], 'v')) invert_match = 1;  //flips to true
    argi++;  //increases to show many arguments are passed
  }

  //checks if pattern passed
  if(argi >= argc) {
    printf(2, "grep error: no pattern passed\n");
    exit();
  }

  pattern = argv[argi++];
  //read file
  for(argi; argi < argc; argi++){
    if((fd = open(argv[argi], 0)) < 0){
      printf(1, "grep: cannot open %s\n", argv[argi]);
      exit();
    }
    grep(pattern, fd, ignore_case, show_line_number, invert_match); //more arguments need to be passed now
    close(fd);
  }
  exit();
}

// Regexp matcher from Kernighan & Pike,
// The Practice of Programming, Chapter 9.

int matchhere(char*, char*);
int matchstar(int, char*, char*);

int
match(char *re, char *text)
{
  if(re[0] == '^')
    return matchhere(re+1, text);
  do{  // must look at empty string
    if(matchhere(re, text))
      return 1;
  }while(*text++ != '\0');
  return 0;
}

// matchhere: search for re at beginning of text
int matchhere(char *re, char *text)
{
  if(re[0] == '\0')
    return 1;
  if(re[1] == '*')
    return matchstar(re[0], re+2, text);
  if(re[0] == '$' && re[1] == '\0')
    return *text == '\0';
  if(*text!='\0' && (re[0]=='.' || re[0]==*text))
    return matchhere(re+1, text+1);
  return 0;
}

// matchstar: search for c*re at beginning of text
int matchstar(int c, char *re, char *text)
{
  do{  // a * matches zero or more instances
    if(matchhere(re, text))
      return 1;
  }while(*text!='\0' && (*text++==c || c=='.'));
  return 0;
}

//for "-i" flag, convert to lowercase for case-insensitive
void change_to_lowercase(char *s) {
  for (int i = 0; s[i]; i++) {
    if (s[i] >= 'A' && s[i] <= 'Z') {   //uses values of ASCII characters
      s[i] = s[i] - 'A' + 'a';    //got the solution for this from reddit https://www.reddit.com/r/C_Programming/comments/icpuvn/function_to_convert_uppercase_letters_to_lowercase/
    }
  }
}