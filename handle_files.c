/*###############################################################################
# Module handle_admin.c (connection-fd) 
###############################################################################
*/
#include <stdio.h>
#include <string.h>
//#include "server.h"

static char*  print_file_head =  
"HTTP/1.0 200 OK\n"
"Content-type: image/jpeg\n";

static char*  error_page_template =  
"HTTP/1.0 200 OK\n"
"Content-type: text/html\n";
"Content-length: %d \n"
"\n"
"<html><body> <p> Do not find file;</p></body></html>";

void print_file (int fd, char* img_file )
{

  int input_fd 

 
  FILE *file;
  if (file = fopen(img_file, "r"))
  {
    
    fclose(file);
  } else { 
    
    write (fd,error_page_template,strlen (error_page_template));

  }


  size_t page_size;

  char *output = malloc(strlen(buffer)+1);
  urldecode2(output, buffer);

  page_size = strlen(admin_page_template) + strlen(output)+strlen(page_content) ; 
  snprintf(page_res, sizeof(page_res),admin_page_template, output , page_content, page_size ); 
  write (fd,page_res,strlen (page_res ));
  free(output);
}


