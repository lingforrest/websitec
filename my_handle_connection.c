/***********************************************************************
* my_handle_connection                                                 *
***********************************************************************/

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "server.h"

/* HTTP response and header for a successful request.  */

extern const char* g_page_start;
extern const char* g_page_menu_admin;
extern const char* g_page_menu_pub;
extern const char* g_page_end;

static char* ok_response_1 =
  "HTTP/1.0 200 OK\n"
  "Content-type: text/html\n"
  "\n" 
  "<html><head><title>OpenHPC in China</title></head>\n"
  "<body style=\"background:aqua; text-align:center; \"><h1>OpenHPC in China</h1><h3>Under Development </h3>\n"
  "<h4>Contact Information: </h4>"
  "<p>E-mail:<a href=\"mailto:lingweicai\@sohu.com\" > lingweicai\@sohu.com</a></p>"
  "<p>Tel: 18600622522 </p>"
  "</body></html>";

static char* ok_response_0 =
  "HTTP/1.0 200 OK\n"
  "Content-type: text/html\n"
  "\n" 
  "<html><head><title>OpenHPC in China</title></head>\n"
  "<body style=\"background:aqua; text-align:center; \"><h1>OpenHPC in China</h1><h3>Under Development  response 0 </h3>\n"
  "</body></html>";


/* HTTP response, header, and body indicating that the we didn't
   understand the request.  */

/* Handle a client connection on the file descriptor CONNECTION_FD.  */

void my_handle_connection (int connection_fd)
{
  char buffer[4096];
  ssize_t bytes_read;
  char response[8192];
  ssize_t len;

  /* Read some data from the client.  */
  bytes_read = read (connection_fd, buffer, sizeof (buffer) - 1);

  if (bytes_read > 0) {
    char method[sizeof (buffer)];
    char url[sizeof (buffer)];
    char protocol[sizeof (buffer)];

    buffer[bytes_read] = '\0';

    sscanf (buffer, "%s %s %s", method, url, protocol);

    if (bytes_read == -1) {
      close (connection_fd);
      return;
    }
    /* Check the protocol field.  We understand HTTP versions 1.0 and
       1.1.  */
    if (strcmp (protocol, "HTTP/1.0") && strcmp (protocol, "HTTP/1.1")) {
      /* We don't understand this protocol.  Report a bad response.  */
      snprintf (response, sizeof (response),"%s", ok_response_1 );
      write (connection_fd, response, strlen (response));
    }
    else {
      /* This server only implements the GET method.  The client
	 specified some other method, so report the failure.  */
      if (strstr(url+1,"admin") == NULL) {
      snprintf (response, sizeof (response), "%s", ok_response_1);
      write (connection_fd, response, strlen (response));
      } else {
         handle_admin ( connection_fd );     
      } 
      
    }
  }
  else if (bytes_read == 0)
    /* The client closed the connection before sending any data.
       Nothing to do.  */
    ;
  else 
    /* The call to read failed.  */
    system_error ("read");
}
