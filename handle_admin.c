/*###############################################################################
# Module handle_admin.c (connection-fd) 
###############################################################################
*/
#include <stdio.h>
#include <string.h>
//#include "server.h"

static char*  admin_page_template=  
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
"#menu {\n"
"  float:left;\n"
"  background-color:aliceblue;\n"
"  width:10em;\n"
"  margin: 0px 0px 10px 0px;\n"
"  padding: 0;\n"
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
"#menu ul {\n"
"  list-style-type: none;\n"
"  padding:0;\n"
"  margin:0;\n"
"}\n"
"#menu ul li {\n"
" display: inline;\n"
" border:1px;\n"
" padding:0;\n"
" margin:0;\n"
"}\n"
"#menu ul li a {\n"
"  display: block;\n"
"  width: 8em;\n"
"  text-decoration: none;\n"
"  color:#728fce;\n"
"  border: 1px dashed #C2DFFF;\n"
"  padding: 5px 15px 5px 15px;\n"
"}\n"
"#menu ul li a:hover {\n"
"  background-color:#659ec7;\n"
"  color:white;\n"
"  font-weight:bold;\n"
"}\n"
"* html #menu ul li a {\n"
"  height: 1px;\n"
"}\n"
"#content form .label {\n"
"  float:left;\n"
"  width:230px;\n"
"  margin: 2px 10px 2px auto;\n"
"  text-align:right;\n"
"  font-family: Arial, Helvetica, tahoma, verdana, 宋体, SimSun, 华文细黑, STXihei, sans-serif;\n"
"  color:#2b3856;\n"
"}\n"
"#content form input {\n"
"  margin: 2px auto 2px 2px;\n"
"  background-color: azure;\n"
"  }\n"
"\n"
"#content form input#submit {\n"
"  border-radius: 20px;\n"
"  width:80px;\n"
"  margin-left: 240px;\n"
"  background-color:#98AFC7;\n"
"  padding: 6px 4px 6px 4px;\n"
"  color:#2b3856;\n"
"}\n"
"#content form input#submit:hover {\n"
"  background-color:#3090C7;\n"
"  color:white;\n"
"  font-weight:bold;\n"
"}\n"
"-->\n"
"</style>\n"
"\n"
"<title>OpenHPC 开放高性能计算</title>\n"
"</head>\n"
"<body>\n"
"<h1>OpenHPC 开放高性能计算</h1>\n"
"<div id=\"menu\" title=\"菜单\" >\n"
"<ul>\n"
"<li><a href=\"admin_add\">添加用户</a></li>\n"
"<li><a href=\"admin_del\">删除用户</a></li>\n"
"<li><a href=\"admin_edit\">修改用户</a></li>\n"
"<li><a href=\"admin_list\">显示用户列表</a></li>\n"
"<li><a href=\"admin_view\">显示用户信息</a></li>\n"
"</ul>\n"
"</div>\n"
"<div id=\"content\" title=\"内容\" >\n"
"<h2>管理员界面</h2>\n"
"%s \n"
"%s"
"</div>\n"
"<div id=\"foot\">邮箱：<a href=\"mailto:lingweicai@sohu.com\">lingweicai@sohu.com</a> <br />\n"
" 联系电话：18600622522<br/> 京ICP备16007807号-1 %d </div>\n"
"</body>\n"
"</html>";

void admin_add_user (int fd, char* buffer)
{

  char page_res[8192];
  char page_content[]=
"<form id=\"reg\" action=\"admin_add_user\" method=\"POST\" accept-charset=\"UTF-8\" >\n"
"<label for=\"name\" class=\"label\">请输入登录名:</label><input type=\"text\" name=\"user_name\" value=\"\" maxlength=\"16\" size=\"16\" /> <br />\n"
"<label for=\"passwd\" class=\"label\">请输入密码:</label><input type=\"password\" name=\"user_pwd\" value=\"\" maxlength=\"16\" size=\"16\" /><br />\n"
"<label for=\"passwd1\" class=\"label\">再次请输入密码:</label><input type=\"password\" name=\"user_pwd1\" value=\"\"   maxlength=\"16\" size=\"16\" /><br />\n"
"<label for=\"email\" class=\"label\">邮箱地址:</label><input type=\"text\" name=\"user_email\" value=\"\" maxlength=\"32\" size=\"32\" /><br />\n"
"<input type=\"submit\" name=\"submit\" id=\"submit\" value=\"添加\" /><br />\n"
"</form>\n"
"<p><img src=\"img/fengjing.jpg\" />images</p>\n"
"\n";

  size_t page_size;

  char *output = malloc(strlen(buffer)+1);
  urldecode2(output, buffer);

  page_size = strlen(admin_page_template) + strlen(output)+strlen(page_content) ; 
  snprintf(page_res, sizeof(page_res),admin_page_template, output , page_content, page_size ); 
  write (fd,page_res,strlen (page_res ));
  free(output);
}

void admin_create_file (int fd, char* buffer)
{

  char page_res[8192];
  char page_content[]=
"<form id=\"reg\" action=\"admin_create_file\" method=\"POST\" accept-charset=\"UTF-8\">\n"
"<label for=\"name\" class=\"label\"> File Name:</label><input type=\"text\" name=\"user_name\" value=\"\" maxlength=\"16\" size=\"16\" /> <br />\n"
"<input type=\"submit\" name=\"submit\" id=\"submit\" value=\"Create File\" /><br />\n"
"</form>\n";

  int create_file_fd;
  char file_result[48];
  size_t page_size;
  create_file_fd = creat ("user.data", 0664);
  if ( create_file_fd == -1 ) {
    strncpy(file_result, "error create file !", sizeof(file_result)); 
  }else {
    strncpy(file_result, "Congrates create file!", sizeof(file_result)); 
  } 

  page_size = strlen(admin_page_template) + strlen(file_result)+strlen(page_content) ; 
  snprintf(page_res, sizeof(page_res),admin_page_template, file_result , page_content, page_size ); 
  write (fd,page_res,strlen (page_res ));
}
