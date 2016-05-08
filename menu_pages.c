       #define _XOPEN_SOURCE 500
       #include <ftw.h>
       #include <stdlib.h>
       #include <string.h>
       #include <stdint.h>
       #include <unistd.h>
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
       #include <stdio.h>
       #include "server.h"

       Menu_page menu_page[MAX_MENU_PAGES];
       int menu_page_number = 0;
       char* menu_dropdown_string; 

/* function called by nftw() 
 * fpath = websitec/pages/admin , set global struct of menu_page[],
 *menu_page[].id;
 *menu_page[].name;
 *menu_page[].content
*/
       int read_page_fun (const char *fpath, const struct stat *sb,
                    int tflag, struct FTW *ftwbuf)
       {
/*
           printf("%-3s %2d %7jd   %-40s %d %s\n",
               (tflag == FTW_D) ?   "d"   : (tflag == FTW_DNR) ? "dnr" :
               (tflag == FTW_DP) ?  "dp"  : (tflag == FTW_F) ?   "f" :
               (tflag == FTW_NS) ?  "ns"  : (tflag == FTW_SL) ?  "sl" :
               (tflag == FTW_SLN) ? "sln" : "???",
               ftwbuf->level, (intmax_t) sb->st_size,
               fpath, ftwbuf->base, fpath + ftwbuf->base);
*/           
           char c_id[3];
           menu_page[menu_page_number].name = (char*)malloc (strlen ( fpath + ftwbuf->base  )+1-3 ); 
           strcpy (menu_page[menu_page_number].name, fpath + ftwbuf->base + 3);
           strncpy (c_id, fpath + ftwbuf->base, 2 );
           c_id[2] = '\0';
           menu_page[menu_page_number].id = atoi ( c_id );   
           
           if ((tflag == FTW_F) && ( menu_page[menu_page_number].id > 0  ) 
                  && ( menu_page[menu_page_number].id <100 ) && ( sb->st_size < 65536 ) )
           {  
              ssize_t bytes_read;
              int fd;
              fd=open ( fpath , O_RDONLY);
              menu_page[menu_page_number].content = (char*)malloc ( sb->st_size + 1 ); 
              bytes_read = read ( fd, menu_page[menu_page_number].content , sb->st_size );
              close (fd);
              menu_page[menu_page_number].content[sb->st_size] = '\0';
           }

           menu_page_number++;   

           return 0;           /* To tell nftw() to continue */
       }

       int menu_id_cmp ( const void *v1, const void *v2 ) 
       { 
        const Menu_page *c1 = v1;
        const Menu_page *c2 = v2;
        return  (c1->id - c2->id) ; 
       }

       void generate_dropdown_menu_string () 
       { 

           char top_menu[256]; 
           char list_link[256];
           char total_link_string[8192];
           int i;

           for (i=1;i< menu_page_number ;i++) 
           { 
             int top, list;
             top = menu_page[i].id / 10;
             list = menu_page[i].id % 10; 
             if ( list == 0 ) { 

   snprintf (top_menu, sizeof(top_menu), "<li class=\"dropdown\"><a href=\"#\" class=\"dropbtn\">%s</a><div class=\"dropdown-content\">", menu_page[i].name); 
//             printf ("%s\n", top_menu );
             strncat ( total_link_string, top_menu, strlen(top_menu)+1 ); 
             int j;
             for (j=i; j< menu_page_number ; j++)  { 
                int k,m; 
                k =  menu_page[j].id % 10;
                m = menu_page[j].id / 10;
                if (k !=0 && m== top ) {
    snprintf (list_link, sizeof(list_link),  "<a href=\"%d\">%s</a>",menu_page[j].id, menu_page[j].name); 

//                 printf ("%s\n", list_link ); 
              strncat (total_link_string, list_link, strlen(list_link)+1 );
                } 
             }   
//  printf ("</div></li>\n"); 
              strncat (total_link_string, "</div></li>\n", strlen("</div></li>\n")+1 ); 
             } 
          }
        
       menu_dropdown_string = (char*) xmalloc((strlen ( total_link_string  ) + 1));  
       strncpy ( menu_dropdown_string, total_link_string, strlen ( total_link_string  ) + 1   ); 

       }


       void read_menu_pages ()
       {
/*  function nftw, walks all the directories/files under pages/admin  */
           if (nftw( "pages/admin",read_page_fun,20, 0)
                   == -1) {
               perror("nftw");
               exit(EXIT_FAILURE);
           }
/*           
           int i;
           for ( i = 0; i< menu_page_number; i++ ) { 
           printf (" id : %d --- name : %s;\n ", menu_page[i].id, menu_page[i].name ); 
           printf (" Content : \n  %s   \n-------",  menu_page[i].content );
           }
*/            
/* do qsort for the struct of menu_pages[] */ 
           qsort ( menu_page, menu_page_number , sizeof ( Menu_page ), menu_id_cmp ); 

/* write menu_page to a string of menu_dropdown_string, a serial of list <li> */ 
           generate_dropdown_menu_string(); 


//           exit(EXIT_SUCCESS);
       } 

 
