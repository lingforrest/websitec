/***********************************************************************
* my_handle_connection                                                 *
***********************************************************************/

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>

#include "server.h"

/* route variables */
extern void admin_add_user (int fd, char* buffer);
extern void admin_create_file (int fd, char* url_file);
extern void print_img_file (int fd, char* url_file);
extern void print_home (int fd, char* url_file);
extern void print_menu_page (int fd, char* url_file);

struct route {
        char *url;
        void (*fpath)(int, char*);
};
static struct route routes[] = {
        { "/admin_add_user/", admin_add_user },
        { "/admin_create_file/", admin_create_file },
        { "/home.html", print_home },
};
static int route_compare(const void *l, const void *r)
{
        return strcmp(((struct route *) l)->url,
                        ((struct route *) r)->url);
}


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
  "<p>E-mail:<a href=\"mailto:services@openhpc.cn\" > services@openhpc.cn</a></p>"
  "<p>E-mail:<a href=\"mailto:forrest_wc_ling@dell.com\" > forrest_wc_ling@dell.com</a></p>"
  "<p>Tel: 18600622522 </p>"
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

  if (bytes_read > 0) 
  {
    char method[sizeof (buffer)];
    char url[sizeof (buffer)];
    char protocol[sizeof (buffer)];

    buffer[bytes_read] = '\0';

    sscanf (buffer, "%s %s %s", method, url, protocol);

    if ( strstr(url,"/img/" ) || strstr(url,"/css/" ) || strstr(url,"/js/" )
            || strstr(url,"/docs/" )  ) { print_img_file ( connection_fd, url); return;} 

   /* Check the protocol field.  We understand HTTP versions 1.0 and
       1.1.  */
    if (strcmp (protocol, "HTTP/1.0") && strcmp (protocol, "HTTP/1.1")) {
      /* We don't understand this protocol.  Report a bad response.  */
      snprintf (response, sizeof (response),"%s", ok_response_1 );
      write (connection_fd, response, strlen (response));
    } else
    {
      /* Start route  */

        ssize_t count;
        struct route needle, *route;
        char *path, *end;
//        static char buf[4096];

//        path = url;
        
        needle.url = url;
        route = bsearch(&needle, routes, sizeof(routes) / sizeof(struct route),
                        sizeof(struct route), route_compare);
        unsigned char url_page_id ;
        url_page_id = atoi (url+1); 
        if ( url_page_id >0 && url_page_id <100 ) 
        {
           print_menu_page ( connection_fd, url+1 ); 
        } else if ( route ) 
        {
          route->fpath( connection_fd, url );
        } else {
           snprintf (response, sizeof (response), "%s", ok_response_1);
           write (connection_fd, response, strlen (response));
        } 
      /* end of route */
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

