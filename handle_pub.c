/*###############################################################################
# Module handle_pub.c (connection-fd) 
###############################################################################
*/
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include "server.h"

static char*  pub_page_template=  
"HTTP/1.0 200 OK\n"
"Content-type: text/html;charset=UTF-8\n"
"\n"
"<!DOCTYPE html>\n"
"<html>\n"
"<head>\n"
"<meta charset=\"UTF-8\" />\n"
"<style type=\"text/css\">\n"
"<!--\n"
"body {\n"
"  margin: 0;\n"
"}\n"
"h1 {\n"
"  background-color:aliceblue;\n"
"  background-image: url(/img/openhpc.png);\n"
"  background-repeat: no-repeat;\n"
"  background-position: 5px center;\n"
"  text-align:center;\n"
"  margin-top:0;\n"
"  margin-bottom:3px;\n"
"  color: #000080;\n"
"  padding:15px 5px 15px 5px;\n"
"}\n"
"h2 {\n"
"  color:#2b3856;\n"
"  padding-top:0;\n"
"  margin-top:0;\n"
"}\n"
"#content {\n"
"  margin-left:11em;\n"
"  background-color:white;\n"
"  margin-bottom: 10px;\n"
"  border: 1px dashed #C2DFFF;\n"
"  padding-top: 1px; \n"
"}\n"
"#foot {\n"
"  clear:both;\n"
"  text-align:center; \n"
"  background-color:aliceblue;\n"
"  padding: 4px 1px 4px 1px;  \n"
"  color: #2b3856;\n"
"}\n"
"ul {\n"
"    list-style-type: none;\n"
"    margin: 0;\n"
"    padding: 0;\n"
"    overflow: hidden;\n"
"    background-color: #333;\n"
"}\n"
"li {\n"
"    float: left;\n"
"}\n"
"li a, .dropbtn {\n"
"    display: inline-block;\n"
"    color: white;\n"
"    text-align: center;\n"
"    padding: 14px 16px;\n"
"    text-decoration: none;\n"
"}\n"
"li a:hover, .dropdown:hover .dropbtn {\n"
"    background-color: red;\n"
"}\n"
"li.dropdown {\n"
"    display: inline-block;\n"
"}\n"
".dropdown-content {\n"
"    display: none;\n"
"    position: absolute;\n"
"    background-color: #f9f9f9;\n"
"    min-width: 160px;\n"
"    box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);\n"
"}\n"
"\n"
".dropdown-content a {\n"
"    color: black;\n"
"    padding: 12px 16px;\n"
"    text-decoration: none;\n"
"    display: block;\n"
"    text-align: left;\n"
"}\n"
"\n"
".dropdown-content a:hover {background-color: #f1f1f1}\n"
"\n"
".dropdown:hover .dropdown-content {\n"
"    display: block;\n"
"}\n"
"-->\n"
"</style>\n"
"\n"
"<title>OpenHPC 开放高性能计算</title>\n"
"</head>\n"
"<body>\n"
"<h1>OpenHPC 开放高性能计算</h1>\n"
"<ul>\n"
"%s \n"
"</ul>\n"
"<div id=\"content\" title=\"内容\" >\n"
"%s"
"</div>\n"
"<div id=\"foot\">邮箱：<a href=\"mailto:lingweicai@sohu.com\">lingweicai@sohu.com</a> <br />\n"
" 联系电话：18600622522<br/> 京ICP备16007807号-1 %d </div>\n"
"</body>\n"
"</html>";

void print_home (int fd, char* buffer)
{

  char page_res[8192];
  char page_content[]= "Linux Foundation" ;
  size_t page_size;

  char *output = xmalloc(strlen(buffer)+1);
  urldecode2(output, buffer);

  page_size = strlen(pub_page_template) + strlen(output)+ strlen(page_content) ; 
  snprintf(page_res, sizeof(page_res),pub_page_template, output , page_content, page_size ); 
  write (fd,page_res,strlen (page_res ));
  free(output);
}
void print_menu_page (int fd, char* url)
{

  char page_res[8192];
  char page_content[]= "Linux Foundation" ;
  size_t page_size;

  unsigned char url_number;
  url_number = (unsigned char) atoi(url);
  if ( url_number  <1 || url_number > 100 ) return;     

/*
typedef struct _menu_page {
          unsigned char id;
          char* name;
          char* content;
} Menu_page;
extern Menu_page menu_page[MAX_MENU_PAGES];
*/
  int i;
  for ( i=1;i < menu_page_number; i++ ) {
    if ( url_number == menu_page[i].id )  
    { 
      page_size = strlen(pub_page_template) + strlen(menu_dropdown_string)+ strlen( menu_page[i].content ) ; 
      snprintf(page_res, sizeof(page_res),pub_page_template, menu_dropdown_string , menu_page[i].content, menu_page_number );
    }
  
  }  

//  page_size = strlen(pub_page_template) + strlen(url)+ strlen( menu_page[2].content ) ; 
  
  write (fd,page_res,strlen (page_res ));
}

