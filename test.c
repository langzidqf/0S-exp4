#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

//packet a func to explain the certain file

void print_file(struct stat * buf, char * name ,int space_num);
void print_dir(char * name,int root);
void print_dir1(char * name,int root);
int main(void)
{
  printf("please input the path:");
  char path[255];
  scanf("%s",path);
  print_dir(path,0);
  //print_dir("/home/dqf/文档/file_of_study/OS/",0);
  return 0;
}


void print_file(struct stat * buf, char * name ,int space_num)
{
  for(int i = 0;i<space_num * 4;i++)
    printf(".");
  int flag = buf->st_mode & S_IFDIR;

  //creator's
  if(flag == S_IFDIR) printf("d");
  else printf("-");

  flag = buf->st_mode & S_IRUSR;
  if(flag == S_IRUSR) printf("r");
  else printf("-");

  flag = buf->st_mode & S_IWUSR;
  if(flag == S_IWUSR) printf("w");
  else printf("-");

  flag = buf->st_mode & S_IXUSR;
  if(flag == S_IXUSR) printf("x");
  else printf("-");

  //group members
  flag = buf->st_mode & S_IRGRP;
  if(flag == S_IRGRP) printf("r");
  else printf("-");

  flag = buf->st_mode & S_IWGRP;
  if(flag == S_IWGRP) printf("w");
  else printf("-");

  flag = buf->st_mode & S_IXGRP;
  if(flag == S_IXGRP) printf("x");
  else printf("-");

  //other members
  flag = buf->st_mode & S_IROTH;
  if(flag == S_IROTH) printf("r");
  else  printf("-");

  flag = buf->st_mode & S_IWOTH;
  if(flag == S_IWOTH) printf("w");
  else printf("-");

  flag = buf->st_mode & S_IXOTH;
  if(flag == S_IXOTH) printf("x");
  else printf("-");

  printf(" %ld",buf->st_nlink);

  //handle the user's information
  uid_t my_uid = buf->st_uid;
  printf(" %s",getpwuid(my_uid)->pw_name);
  gid_t my_gid = buf->st_gid;
  printf(" %s",getgrgid(my_gid)->gr_name);

  printf(" %ld ",buf->st_size);
  time_t change_time = buf->st_mtime;
  //printf("%d",strlen(ctime(&change_time)));
  char * str = ctime(&change_time);
  str[24] = '\0';
  printf("%s",str);
  printf(" %s\n",name);
}

void print_dir(char * name,int root)
{
  DIR * dirptr = NULL;
  struct dirent * entry;
  dirptr = opendir(name);
  if(dirptr == NULL)
  {
    printf("error dir_name\n");
    exit(1);
  }

  while(entry = readdir(dirptr))
  {
    if(strcmp(entry->d_name,".") ==0)
      continue;
    if(strcmp(entry->d_name,"..") == 0)
      continue;
    char file_name[255];
    strcpy(file_name,name);
    strcat(file_name,entry->d_name);
    struct stat buf;
    lstat(file_name,&buf);
    print_file(&buf,entry->d_name,root);
    if((buf.st_mode & S_IFDIR) == S_IFDIR)
    {
      //printf("%d\n",strlen(file_name));
      strcat(file_name,"/");
      print_dir(file_name,root+1);
    }
  }
  closedir(dirptr);
  return ;
}
