#include"Studio_UI.h"
#include<gtk/gtk.h>
#include"Window_UI.h"
#include"Studio_UI.h"
#include"../Service/Seat.h"
#include "../Common/List.h"
#include<stdio.h>
#include<string.h>

//座位管理界面
GtkWidget*  Seat_UI_MgtEntry(int roomID)
{
	GtkWidget *button;
	GtkWidget *button1;
	GtkWidget *label;
	GtkWidget *frame;
	GtkWidget *fbox;
	GtkWidget *dbox;
	GtkWidget *boxs;
	GtkWidget *box;
	GtkWidget *image;

	GtkWidget *window;
	
	int row, column , i,j,type;
	char userName[30];
	char str[3];

	studio_t temp;
	seat_list_t head;
	seat_node_t *pos;

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(destroy),(gpointer)window);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_container_set_border_width(GTK_CONTAINER(window),10);
	gtk_window_set_title(GTK_WINDOW(window),"guanli");

	box = gtk_vbox_new(FALSE,30);
	gtk_container_add(GTK_CONTAINER(window),box);

	button = gtk_button_new_with_label("荧幕");
	gtk_widget_set_size_request(GTK_WIDGET(button),100,80); 
	gtk_box_pack_start(GTK_BOX(box),button,FALSE,FALSE,0);

	dbox = gtk_vbox_new(FALSE,10);
	gtk_container_add(GTK_CONTAINER(box),dbox);

	frame = gtk_frame_new("");
	gtk_box_pack_start(GTK_BOX(dbox),frame,FALSE,FALSE,0);

	List_Init(head, seat_node_t);
	List_Init(pos, seat_node_t);

	//载入数据
	Seat_Srv_FetchByRoomID( head, roomID);
	//Seat_Srv_SortSeatList(head);
	Studio_Srv_FetchByID(roomID, &temp);
	//显示数据
	for(i=1;i<=temp.rowsCount;i++)
	{
		boxs = gtk_hbox_new(FALSE,10);
		gtk_container_add(GTK_CONTAINER(dbox),boxs);
		for(j=1;j<=temp.colsCount;j++)
		{
			pos = Seat_Srv_FindByRowCol(head,i, j);

			str[0] = ' ';
			if(pos!=NULL)				
			{
				if(pos->data.status == 0)
				{
					str[0] = 'O';
				}
				if(pos->data.status == 1)
				{
					str[0] = 'H';
				}
				if(pos->data.status == 9)
				{
       					str[0] = 'x';
				}
			}	
			str[1]='\0';
			button = gtk_button_new_with_label(str);
			gtk_widget_set_size_request(GTK_WIDGET(button),30,30); 
			gtk_box_pack_start(GTK_BOX(boxs),button,FALSE,FALSE,10);
		}
	}	
	//List_Destroy(head, seat_node_t);
	boxs = gtk_hbox_new(TRUE,10);
	gtk_container_add(GTK_CONTAINER(box),boxs);
	
	button = gtk_button_new_with_label("添加");
	//g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(Studio_Menu),(gpointer)1);
	gtk_box_pack_start(GTK_BOX(boxs),button,FALSE,FALSE,5);
	
	button = gtk_button_new_with_label("修改");
	//g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(Studio_Menu),(gpointer)2);
	gtk_box_pack_start(GTK_BOX(boxs),button,FALSE,FALSE,5);

	button = gtk_button_new_with_label("删除");
	//g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(list_delete_studio),NULL);
	gtk_box_pack_start(GTK_BOX(boxs),button,FALSE,FALSE,5);

	return window;
}
