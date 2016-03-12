/***********************************************************************
void print_img_file (int fd, char* url_file )
***********************************************************************/

#include <fcntl.h>
#include <string.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "server.h"

/* HTML source for the start of the page we generate.  */

static char* http_start =
  "HTTP/1.0 200 OK\n"
  "Content-type: image/jpeg \n"
  "\n"; 

/* HTML source for the page indicating there was a problem opening
   /proc/issue.  */

static char* error_page = 
  "HTTP/1.0 200 OK\n"
  "Content-type: text/html \n"
  "\n" 
  "<html>\n"
  " <body>\n"
  "  <p>Error: Could not open the file.</p>\n"
  " </body>\n"
  "</html>\n";

/* HTML source indicating an error.  */

static char* error_message = "Error reading the file.";

void print_img_file (int fd, char* url_file )
{
  int input_fd;
  struct stat file_info;
  int rval;

  /* Open /etc/issue.  */
  input_fd = open (url_file+1, O_RDONLY);
  if (input_fd == -1)
    system_error ("open");
  /* Obtain file information about it.  */
  rval = fstat (input_fd, &file_info);

  if (rval == -1) 
    /* Either we couldn't open the file or we couldn't read from it.  */
    write (fd, error_page, strlen (error_page));
  else {
    int rval;
    off_t offset = 0;

    /* Write the start of the page.  */
    write (fd, http_start, strlen (http_start));
    /* Copy from /proc/issue to the client socket.  */
    rval = sendfile (fd, input_fd, &offset, file_info.st_size);
    if (rval == -1)
      /* Something went wrong sending the contents of /proc/issue.
	 Write an error message.  */
      write (fd, error_message, strlen (error_message));
    /* End the page.  */
    //write (fd, page_end, strlen (page_end));
  }

  close (input_fd);
}
