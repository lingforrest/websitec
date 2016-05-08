
#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <sys/types.h>
#include <ftw.h>
#include <sys/stat.h>

/*** Symbols defined in common.c.  ************************************/

/* The name of this program.  */
extern const char* program_name;

/* If non-zero, print verbose messages.  */
extern int verbose;

/* Like malloc, except aborts the program if allocation fails.  */
extern void* xmalloc (size_t size);

/* Like realloc, except aborts the program if allocation fails.  */
extern void* xrealloc (void* ptr, size_t size);

/* Like strdup, except aborts the program if allocation fails.  */
extern char* xstrdup (const char* s);

/* Print an error message for a failed call OPERATION, using the value
   of errno, and end the program.  */
extern void system_error (const char* operation);

/* Print an error message for failure involving CAUSE, including a
   descriptive MESSAGE, and end the program.  */
extern void error (const char* cause, const char* message);

/* Return the directory containing the running program's executable.
   The return value is a memory buffer which the caller must deallocate
   using free.  This function calls abort on failure.  */
extern char* get_self_executable_directory ();


/*** Symbols defined in module.c  **************************************/

/* An instance of a loaded server module.  */
struct server_module {
  /* The shared library handle corresponding to the loaded module.  */
  void* handle;
  /* A name describing the module.  */
  const char* name;
  /* The function which generates the HTML results for this module.  */
  void (* generate_function) (int);
};

/* The directory from which modules are loaded.  */
extern char* module_dir;

/* Attempt to load a server module with the name MODULE_PATH.  If a
   server module exists with this path, loads the module and returns a
   server_module structure representing it.  Otherwise, returns NULL.  */
extern struct server_module* module_open (const char* module_path);

/* Close a server module and deallocate the MODULE object.  */
extern void module_close (struct server_module* module);


/*** Symbols defined in server.c.  ************************************/

/* Run the server on LOCAL_ADDRESS and PORT.  */
extern void server_run (struct in_addr local_address, uint16_t port);
/* My function */
extern void my_handle_connection (int connection_fd);

/* common.c */
extern const char* g_page_start;
extern const char* g_page_menu_admin;
extern const char* g_page_menu_pub;
extern const char* g_page_end;
/* handle_adimin.c */
extern void admin_add_user (int fd, char* buffer);
extern void urldecode2(char *dst, const char *src);
/* print_file.c*/
extern void print_image_file (int fd, char* url_file);
/* Data Struc defined for Menu_pages   */
typedef struct _menu_page {
          unsigned char id;
          char* name;
          char* content;
} Menu_page;

#define MAX_MENU_PAGES 50
extern Menu_page menu_page[MAX_MENU_PAGES];
extern int menu_page_number;
extern char* menu_dropdown_string;  

extern int read_page_fun (const char *fpath, const struct stat *sb,int tflag, struct FTW *ftwbuf);
extern void read_menu_pages ();

#endif  /* SERVER_H */
