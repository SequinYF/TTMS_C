#include "Play_UI.h"
#include "Schedule_UI.h"

#include "../Common/List.h"
#include "../Service/Play.h"
#include "../Service/EntityKey.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//翻页输出已有剧目信息
void Play_UI_ShowList(play_list_t list,Pagination_t paging) 
{
	int i,j;
	char choice;
	play_node_t *pos;
	//List_Init(list, play_node_t);
	//paging.offset = 0;
	//paging.pageSize = 2;
	

	//载入数据
	//paging.totalRecords = Play_Srv_FetchAll(list);
	//Paging_Locate_FirstPage(list, paging);
	//do{
		/*system("cls");*/
		printf("\n=======================================================\n");
		printf("********************* 剧目信息 ***************************\n");
		printf("\n=======================================================\n");
		//显示数据
		Paging_ViewPage_ForEach(list, paging, play_node_t, pos, i)
		{
			printf("剧目ID[%d]\n", pos->data.id);
			printf("剧目名称[%s]\n",pos->data.name);
			printf("剧目产地[%s]\n",pos->data.area);
			j=pos->data.type;
			switch(j)
			{
				case 1:printf("剧目类型[%s]\n","电影类型");break;
				case 2:printf("剧目类型[%s]\n","歌剧类型");break;
				case 3:printf("剧目类型[%s]\n","音乐会类型");break;
			}
			j=pos->data.rating;
			switch(j)
			{
				case 1:printf("剧目等级[%s]\n","儿童剧");break;
				case 2:printf("剧目等级[%s]\n","少年剧");break;
				case 3:printf("剧目等级[%s]\n","成人剧");break;
			}
			printf("剧目时长[%d]\n",pos->data.duration);
			printf("剧目上映日期[%d.%d.%d]\n",pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day);
			printf("结束放映日期[%d.%d.%d]\n",pos->data.end_date.year,pos->data.end_date.month,pos->data.end_date.day);
			printf("票价[%d]\n",pos->data.price);
			printf("\n");	
		
		}

		/*printf("------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));
		
		printf("[P]上一页    [N]下一页    [R]返回");
		printf("\n=======================================================\n");
		printf("请选择:");
		fflush(stdin);
		scanf("%c", &choice);
		getchar();
		switch (choice) 
		{
			case 'p':
			case 'P':
				if (!Pageing_IsFirstPage(paging)) 
				{
					paging.totalRecords = Play_Srv_FetchAll(list);
					Paging_Locate_OffsetPage(list, paging, -1, play_node_t);
				}
				break;
			case 'n':
			case 'N':
				if (!Pageing_IsLastPage(paging)) 
				{
					paging.totalRecords = Play_Srv_FetchAll(list);
					Paging_Locate_OffsetPage(list, paging, 1, play_node_t);
				}
				break;
		}
	} while (choice != 'r' && choice != 'R');*/
	//}
	//List_Destroy(list, play_node_t);
}

//剧目管理界面
void Play_UI_MgtEntry(int flag)
{
	int id,play_id;
	char choice;

	play_list_t head;
	Pagination_t paging;

	List_Init(head, play_node_t);
	paging.offset = 0;
	paging.pageSize = 2;

	//载入数据
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);
	
	if(flag == 0)
	{		
		do
		{	
			Play_UI_ShowList(head,paging);
			printf("------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));
						
			printf("*********************************************************\n");
			printf("[S]进入演出计划  [A]添加  [Q]查找  [D]删除   [U]修改   [R]返回  [p]上一页  [n]下一页");
			printf("\n=======================================================\n");
			printf("请选择:");
			//fflush(stdin);
			choice = getchar();
puts("asasasasasasasas");
			switch (choice) 
			{
			case 's':
			case 'S':
				Schedule_UI_MgtEntry(1);
				break;
			case 'a':
			case 'A':
				if (Play_UI_Add()) //新添加成功，跳到最后一页显示
				{
					paging.totalRecords = Play_Srv_FetchAll(head);
					Paging_Locate_LastPage(head, paging, play_node_t);
				}
				break;
			case 'd':
			case 'D':
				printf("请输入剧目ID:");
				scanf("%d", &id);
				getchar();
				if (Play_UI_Delete(id)) 
				{	//从新载入数据
					paging.totalRecords = Play_Srv_FetchAll(head);
					List_Paging(head, paging, play_node_t);
				}
				break;
			case 'u':
			case 'U':
				printf("请输入剧目ID:");
				scanf("%d", &id);
				if (Play_UI_Modify(id)) 
				{	//从新载入数据
					paging.totalRecords = Play_Srv_FetchAll(head);
					List_Paging(head, paging, play_node_t);
				}
				break;
			case 'p':
			case 'P':
				if (!Pageing_IsFirstPage(paging)) 
				{
					paging.totalRecords = Play_Srv_FetchAll(head);
					Paging_Locate_OffsetPage(head, paging, -1, play_node_t);
				}
				break;
			case 'n':
			case 'N':
				if (!Pageing_IsLastPage(paging)) 
				{
					paging.totalRecords = Play_Srv_FetchAll(head);
					Paging_Locate_OffsetPage(head, paging, 1, play_node_t);
				}
				break;
			case 'Q':
			case 'q':
				printf("请输入剧目ID:");
				scanf("%d", &id);
				if (Play_UI_Query(id)) 
				{	//从新载入数据
					paging.totalRecords = Play_Srv_FetchAll(head);
					List_Paging(head, paging, play_node_t);
				}
				break;
			//case 's':
			//case 'S':
				
				//翻页输出已有剧目信息
			//	Play_UI_ShowList(head,paging);
			//	break;	
			}
		}while(choice != 'r' && choice != 'R');
		List_Destroy(head, play_node_t);
	}
	/*else
	{
		do
		{	
			printf("------- Total Records:%2d ----------------------- Page %2d/%2d ----\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));
			printf("******************************************************************\n");
			printf("[Q]查找  [R]返回   [S]查看");
			printf("\n==================================================================\n");
			printf("请选择:");
			fflush(stdin);
			scanf("%c", &choice);
			fflush(stdin);
			switch (choice) 	
			{
				case 'Q':
				case 'q':
					printf("请输入剧目ID:");
					scanf("%d", &id);
					if (Play_UI_Query(id)) 
					{	//从新载入数据
						paging.totalRecords = Play_Srv_FetchAll(head);
						List_Paging(head, paging, play_node_t);
					}
					break;
				case 's':
				case 'S':
					
					//翻页输出已有剧目信息
					Play_UI_ShowList(head,paging);
					break;
			}
		}while(choice != 'r' && choice != 'R');
	List_Destroy(head, play_node_t);*/
}

//添加新剧目界面
int Play_UI_Add(void)
{
	play_t data;
	char choice;
	int newRecCount = 0,type,rating;
	do{	/*system("cls");*/
		printf("\n=======================================================\n");
		printf("********************  添加新的剧目信息  ********************\n");
		printf("-------------------------------------------------------\n");
		printf("请输入剧目名称:");
		scanf("%s",data.name);
		printf("请输入剧目产地:");
		scanf("%s",data.area);
		printf("请输入剧目时长(以分钟为单位,输入数字):");
		scanf("%d",&data.duration);
		printf("[电影类型]输入1,[歌剧类型]输入2,[音乐会类型]输入3:");
		scanf("%d",&type);
		if(type==1 || type==2 || type==3)
		{	
			data.type=(play_type_t)type;
		}
		else
		{
			while(type!=1 && type!=2 && type!=3)
			{	printf("输入有误,请重新输入!\n");
				printf("[电影类型]输入1,[歌剧类型]输入2,[音乐会类型]输入3:");
				scanf("%d",&type);
			}
			data.type=(play_type_t)type;
		}
		printf("[儿童剧]输入1,[少年剧]输入2,[成人剧]输入3:");
		scanf("%d",&rating);
		if(rating==1 || rating==2 || rating==3)
		{	
			data.rating=(play_rating_t)rating;
		}
		else
		{
			while(rating!=1 && rating!=2 && rating!=3)
			{	printf("输入有误,请重新输入!");
				printf("[儿童剧]输入1,[少年剧]输入2,[成人剧]输入3:");
				scanf("%d",&rating);
			}
			data.rating=(play_rating_t)rating;
		}
		printf("请输入开始放映日期(如2014 7 2):");
		scanf("%d %d %d",&data.start_date.year,&data.start_date.month,&data.start_date.day);
		printf("请输入放映结束日期(如2014 7 2):");
		scanf("%d %d %d",&data.end_date.year,&data.end_date.month,&data.end_date.day);
		printf("请输入票价:");
		scanf("%d",&data.price);
		getchar();	
		printf("=======================================================\n");

		//获取主键
		data.id = EntKey_Srv_CompNewKey("Play");
		
		if (Play_Srv_Add(&data)) {
			newRecCount += 1;
			printf("新剧目添加成功!\n");
		} else
			printf("新剧目添加失败!\n");
		printf("-------------------------------------------------------\n");
		printf("[A]添加      [R]返回\n");
		fflush(stdin);
		scanf("%c", &choice);
		getchar();
	} while ('a' == choice || 'A' == choice);
	return newRecCount;

}

//根据ID修改剧目界面
int Play_UI_Modify(int id)
{	int type,rating,i;
	play_t data;
	int rtn=0;
	char newname[31],newarea[9];
	int newduration,newprice,newSyear,newSmonth,newSday,newEyear,newEmonth,newEday,newrate,newtype;

	/*Load record*/
	if (!Play_Srv_FetchByID(id, &data)) 
	{
		printf("剧目不存在,按确认键返回\n");
		getchar();
		return 0;
	}
	printf("\n=======================================================\n");
	printf("**********************  修改剧目信息  **********************\n");
	printf("-------------------------------------------------------\n");
	printf("剧目ID[%d]\n", data.id);
	printf("剧目名称[%s]\n", data.name);
	i=data.type;
	switch(i)
	{
		case 1:printf("剧目类型[%s]\n","电影类型");break;
		case 2:printf("剧目类型[%s]\n","歌剧类型");break;
		case 3:printf("剧目类型[%s]\n","音乐会类型");break;
	}
	printf("剧目出品地区[%s]\n",data.area);
	i=data.rating;
	switch(i)
	{
		case 1:printf("剧目等级[%s]\n","儿童剧");break;
		case 2:printf("剧目等级[%s]\n","少年剧");break;
		case 3:printf("剧目等级[%s]\n","成人剧");break;
	}
	printf("剧目时长[%d分钟]\n",data.duration);
	printf("剧目放映日期[%d.%d.%d]\n",data.start_date.year,data.start_date.month,data.start_date.day);
	printf("放映结束日期[%d.%d.%d]\n",data.end_date.year,data.end_date.month,data.end_date.day);
	printf("剧目票价[%d]\n",data.price);
	printf("-------------------------------------------------------\n");	
	fflush(stdin);
	
	if(Play_Srv_FetchByID(id,&data))
	{	
		
		printf("请输入剧目名称:");
		scanf("%s",newname);
		printf("请输入剧目产地:");
		scanf("%s",newarea);
		printf("请输入剧目时长(以分钟为单位,输入数字):");
		scanf("%d",&newduration);
		printf("[电影类型]输入1,[歌剧类型]输入2,[音乐会类型]输入3:");
		scanf("%d",&newtype);
		if(newtype==1 || newtype==2 || newtype==3)
		{	
			data.type=(play_type_t)newtype;
		}
		else
		{
			while(newtype!=1 && newtype!=2 && newtype!=3)
			{	printf("输入有误,请重新输入!:");
				printf("[电影类型]输入1,[歌剧类型]输入2,[音乐会类型]输入3:");
				scanf("%d",&newtype);
			}
			data.type=(play_type_t)newtype;
		}
		printf("[儿童剧]输入1,[少年剧]输入2,[成人剧]输入3:");
		scanf("%d",&newrate);
		if(newrate==1 || newrate==2 || newrate==3)
		{	
			data.rating=(play_rating_t)newrate;
		}
		else
		{
			while(newrate!=1 && newrate!=2 && newrate!=3)
			{	printf("输入有误,请重新输入!\n");
				printf("[儿童剧]输入1,[少年剧]输入2,[成人剧]输入3:");
				scanf("%d",&newrate);
			}
			data.rating=(play_rating_t)newrate;
		}
		printf("请输入开始放映日期(如2014 7 2):");
		scanf("%d %d %d",&newSyear,&newSmonth,&newSday);
		printf("请输入放映结束日期(如2014 7 2):");
		scanf("%d %d %d",&newEyear,&newEmonth,&newEday);
		printf("请输入票价:");
		scanf("%d",&newprice);
		strcpy(data.name,newname);
		strcpy(data.area,newarea);
		data.duration=newduration;
		data.price=newprice;
		data.start_date.year=newSyear;
		data.start_date.month=newSmonth;
		data.start_date.day=newSday;
		data.end_date.year=newEyear;
		data.end_date.month=newEmonth;
		data.end_date.day=newEday;	
	}
	else
	{
		//获取主键
		data.id = EntKey_Srv_CompNewKey("Play");
		
		printf("请输入剧目名称:");
		scanf("%s",newname);
		printf("请输入剧目产地:");
		scanf("%s",newarea);
		printf("请输入剧目时长(以分钟为单位,输入数字):");
		scanf("%d",&newduration);
		printf("电影类型输入1,歌剧类型输入2,音乐会类型输入3:");
		scanf("%d",&type);
		if(type==1 || type==2 || type==3)
		{	
			data.type=type;
		}
		else
		{
			while(type!=1 && type!=2 && type!=3)
			{	printf("输入有误,请重新输入!\n");
				printf("电影类型输入1,歌剧类型输入2,音乐会类型输入3:");
				scanf("%d",&type);
			}
			data.type=type;
		}
		printf("儿童剧输入1,少年剧输入2,成人剧输入3:");
		scanf("%d",&rating);
		if(rating==1 || rating==2 || rating==3)
		{	
			data.rating=rating;
		}
		else
		{
			while(rating!=1 && rating!=2 && rating!=3)
			{	printf("输入有误,请重新输入!\n");
				printf("儿童剧输入1,少年剧输入2,成人剧输入3:");
				scanf("%d",&rating);
			}
			data.rating=rating;
		}
		printf("请输入开始放映日期(如2014 7 2):");
		scanf("%d %d %d",&data.start_date.year,&data.start_date.month,&data.start_date.day);
		printf("请输入放映结束日期(如2014 7 2):");
		scanf("%d %d %d",&data.end_date.year,&data.end_date.month,&data.end_date.day);
		printf("请输入票价:");
		scanf("%d",&data.price);	
		printf("=======================================================\n");
;	
	
	}
	
	printf("-------------------------------------------------------\n");

	if (Play_Srv_Modify(&data)) 
	{
		rtn = 1;
		printf("剧目修改成功!\n按确认键返回!\n");
	} 
	else
	{
		printf("剧目修改失败!\n按确认键返回!\n");
	}
	getchar();
	return rtn;
}

//根据ID删除剧目界面
int Play_UI_Delete(int id)
{
	int rtn = 0;

	if (Play_Srv_DeleteByID(id)) 
	{
		printf("剧目成功删除!\n按确认键返回!\n");
		rtn=1;
	}
	else
	{
		printf("剧目删除失败!\n按确认键返回!\n");
	}
	getchar();
	return rtn;
}

//根据ID查找剧目界面
int Play_UI_Query(int id)
{
	play_t data;
	int rtn=0,i;
	if(Play_Srv_FetchByID(id,&data))
	{
		printf("剧目ID [%d]\n", data.id);
		printf("剧目名称 [%s]\n", data.name);
		i=data.type;
		switch(i)
		{
			case 1:printf("剧目类型[%s]\n","电影类型");break;
			case 2:printf("剧目类型[%s]\n","歌剧类型");break;
			case 3:printf("剧目类型[%s]\n","音乐会类型");break;
		}
		printf("剧目出品地区 [%s]\n",data.area);
		i=data.rating;
		switch(i)
		{
			case 1:printf("剧目等级[%s]\n","儿童剧");break;
			case 2:printf("剧目等级[%s]\n","少年剧");break;
			case 3:printf("剧目等级[%s]\n","成人剧");break;
		}
		printf("剧目时长 [%d]\n",data.duration);
		printf("剧目放映日期 [%d.%d.%d]\n",data.start_date.year,data.start_date.month,data.start_date.day);
		printf("放映结束日期 [%d.%d.%d]\n",data.end_date.year,data.end_date.month,data.end_date.day);	
		rtn=1;
	}
	else
	{
		printf("没有找到该剧目的信息!\n");
	}
	return rtn;
}

