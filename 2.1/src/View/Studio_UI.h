#ifndef STUDIO_UI_H_
#define STUDIO_UI_H_
#include"../Service/Studio.h"

#include<gtk/gtk.h>

gint studio_now;//放映厅当前行数
gint studio_num;//放映厅列表总行数

GtkWidget* studio_list;

GtkWidget* text_name;
GtkWidget* text_row;
GtkWidget* text_cols;


//放映厅管理页面
GtkWidget* Studio_UI_MgtEntry();

//演出厅列表初始化
void Studio_Clist_Make(GtkWidget *Clist, studio_list_t head);

//定位放映厅列表界面选中行
void selection_made_studio( GtkWidget *list,gint row,gint column,GdkEventButton *event,gpointer userdate);

//创建放映厅添加页面
GtkWidget* Studio_UI_Add();

//创建放映厅修改页面
GtkWidget* Studio_UI_Change();

//将新增数据加入列表
void to_add_list_studio(GtkWidget *button ,gpointer userdate);

//将修改的数据加入列表
void to_change_list_studio(GtkWidget *button ,gpointer userdate);

//从列表中删除数据
void list_delete_studio(GtkButton *button, gpointer userdate);

//创建功能回调函数
GtkWidget* Studio_Menu(GtkWidget *button,gpointer data);

//进入座位管理页面
void studio_to_seat();

#endif
