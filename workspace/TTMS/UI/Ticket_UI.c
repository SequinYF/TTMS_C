  

#include "Ticket_UI.h"
#include "../Service/Seat.h"
#include "../Common/list.h"
#include "../Service/Ticket.h"
#include "../Service/Schedule.h"
#include "../Service/Play.h"
#include "../Service/Studio.h"
#include "../Service/Sale.h"
#include <stdio.h>
#include <stdlib.h>

//根据状态返回对应表示状态符号
inline char Ticket_UI_Status2Char(ticket_status_t status) {

	char statusChar;
	switch (status) {
	case TICKET_AVL:		//有座位
		statusChar = '#';
		break;
	case TICKET_SOLD:	//损坏的座位
		statusChar = '&';
		break;
	case TICKET_RESV:
		statusChar = '@';
	default:
		statusChar = '!';
		break;
	}
	return statusChar;
}

void Ticket_UI_Print(int ID){
	//根据ID载入票
	ticket_t ticket;
	if(!Ticket_Srv_FetchByID(ID, &ticket)){
    		printf("The ticket with ID:%d does not exist! \n Press [Enter] key to return!\n ", ID);
		return ;
	}

	//载入演技计划及剧目
	schedule_t sch;
	play_t play;
	Schedule_Srv_FetchByID(ticket.schedule_id, &sch);
	Play_Srv_FetchByID(sch.play_id, &play);

	//载入座位及演出厅
	seat_t seat;
	studio_t room;
	Seat_Srv_FetchByID(ticket.seat_id, &seat);
	Studio_Srv_FetchByID(seat.roomID, &room);

	//显示票信息
    system("clear");
	printf("\n\n\n\t\t\t--------------------------------------------------\n");
	printf("\t\t\t|%9s%-10d%15s%-5d%5s%-4d|\n", "Ticket ID:", ticket.id,
			"Row:", seat.row, "Col:", seat.column );
	printf("\t\t\t|%9s%-39s|\n", "Play Name:", play.name);
	printf("\t\t\t|%9s%-11d %16d-%2d-%2d %2d:%2d|\n", "Price:", play.price,
				sch.date.year, sch.date.month, sch.date.day,
				sch.time.hour, sch.time.minute);
	printf("\t\t\t--------------------------------------------------\n");
    sleep(2);
}

//根据计划ID显示所有票
void Ticket_UI_ListBySch(const schedule_t *sch,	ticket_list_t tickList, seat_list_t seatList) {
	assert(NULL!=sch && NULL!=tickList && NULL!=seatList);
	int i, j;
	int row,column,t=0;
	studio_t studioRec;
	play_t   playRec;
	ticket_node_t *pTicket;
	seat_node_t *pSeat;

	if (!Studio_Srv_FetchByID(sch->studio_id, &studioRec)) {  //获得对应id放映厅的信息
		printf("The room does not exist!\nPress [Enter] key to return!\n");
		getchar();
		return;
	}

	if (!Play_Srv_FetchByID(sch->play_id, &playRec)) {  //获得对应id剧目的信息
		printf("The play does not exist!\nPress [Enter] key to return!\n");
		getchar();
		return;
	}
    system("clear");
	printf("\n\n\n			=================================================================");
	printf("\n			---------------------%5s剧目售票表------------------------------",playRec.name);
	printf("%5c", ' ');
	printf("\n			*****************************************************************");
		printf("\n	\t\t   列：%c", ' ');
		for (i = 1; i <= studioRec.colsCount; i++) {
			printf("%3d", i);
		}
		printf("\n	\t\t=================================================================");
	//显示数据
	for (i = 1; i <= studioRec.rowsCount; i++) {
			j = 1;
			printf("\n	\t\t%3d行：%3c", i, ' ');
			List_ForEach(tickList,pTicket)
			{
				pSeat=Seat_Srv_FindByID(seatList, pTicket->data.seat_id);
				if(NULL!=pSeat && pSeat->data.row == i) {
				while (pSeat->data.column != j) {
						printf("%3c", ' ');
						j++;
					}
				printf("%3c", Ticket_UI_Status2Char(pTicket->data.status));
				j++;
			}
		}
	}
	printf("\n			******************************************************************");
	 printf("\n	\t\t请输入座位的列号:");
	while(1){
				if(scanf("%d", &(row))==1){ cl_stdin(); break;}
				else {cl_stdin(); printf("\n	                 		     **输入有误！清重新输入**  ");}
				}
	printf("	\t\t请输入座位的行号:");
	while(1){
				if(scanf("%d", &(column))==1){ cl_stdin(); break;}
				else {cl_stdin();printf("\n	                 		     **输入有误！清重新输入**  ");}
				}
	List_ForEach(seatList, pSeat){
							if(row==pSeat->data.row && column==pSeat->data.column){
								ticket_node_t *td;
								td= Ticket_Srv_FindBySeatID(tickList, pSeat->data.id);
							//	if(QueryTicket(td->data.id)==1){
									if(1/*UpdateTicket(td->data.id)*/ ){
										sale_t sdata;
							       			sdata.id=EntKey_Srv_CompNewKey("sale");
							       		//	sdata.user_id= 0 ;       /*gl_CurUsr.id*/	
                                     //   pSeat->data.status=SEAT_GOOD;
                                        sdata.ticket_id=td->data.id;
							       			sdata.date=DateNow();
							       			sdata.time=TimeNow();
							       			sdata.value=td->data.price;
							       			sdata.type=SALE_SELL;
							       			Sale_Srv_Add(&sdata);
											printf("\n\t\t\t系统正在出票，请稍候。。。\n");
											sleep(1);
											Ticket_UI_Print(td->data.id);
											t=1;
											break;
									}else{
											printf("\n\t\t\t售票失败！");
											t=1;
											break;
									}
									
									
						/*		}else{
								
									printf("\n\t\t\t该票已预留或售出");
									
									break;
								}*/
							}
					}
}

void ListTickets(void){
	int i, id, schedule_id;
	schedule_t schedule_rec;
	play_t play_rec;
	char choice;
    seat_t seat_rec;
	ticket_list_t head;
	ticket_node_t *pos;
	Pagination_t paging;

	List_Init(head, ticket_node_t);
	paging.offset = 0;
	paging.pageSize = TICKET_PAGE_SIZE;

	printf("please input the schedule id to list the tickets!\n");
	scanf("%d",&schedule_id);

	//载入数据
	paging.totalRecords = Ticket_Srv_FetchBySchID(head,schedule_id);
	Paging_Locate_FirstPage(head, paging);
	//需要增加查找座位信息
	Seat_Srv_FetchByID(schedule_id,&seat_rec);
	Schedule_Srv_FetchByID(schedule_id,&schedule_rec);
	Play_Srv_FetchByID(schedule_rec.play_id,&play_rec);
	do {
		printf("\n\n\n\t\t\t=======================================================\n");
		printf("\t\t\t****************  Ticket Information List  ****************\n");
		printf("\t\t\tID\tPlay Name\t Date\tTime\tPrice\tStatus\n");
		printf("\t\t\t-------------------------------------------------------\n");
		//显示数据
		for (i = 0, pos = (ticket_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("\t\t\t%d\t%s\t%d\t%d-%d-%d\t%d:%d\t%d\t%s\n", pos->data.id, play_rec.name,
					schedule_rec.date.year,schedule_rec.date.month,
					schedule_rec.date.day,schedule_rec.time.hour,
					schedule_rec.time.minute,pos->data.price,
					pos->data.status==0?"to sell":"sold");
			pos = pos->next;
		}
		printf(
				"== Total Records:%d =========================== Page %d/%d ==\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("[P]revPage|[N]extPage | [U]pdate | [R]eturn ");
		fflush(stdin);
		scanf("%c", &choice);
		switch (choice) {
		case 'u':
		case 'U':
			printf("Input the ID:");
			scanf("%d", &id);
			if (UpdateTicket(id)) {	//从新载入数据
				paging.totalRecords = Ticket_Srv_FetchBySchID(head,schedule_rec.id);
				List_Paging(head, paging, ticket_node_t);
			}
			break;
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, ticket_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, ticket_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, ticket_node_t);
	}


int UpdateTicket(int id){
	ticket_t rec;
	schedule_t schedule_rec;
	play_t play_rec;
	int rtn = 0;


	/*Load record*/
	if (!Ticket_Srv_FetchByID(id, &rec)) {
		printf("The play does not exist!\nPress [Enter] key to return!\n");
		getchar();
		return 0;
	}
	Schedule_Srv_FetchByID(rec.schedule_id,&schedule_rec);
	Play_Srv_FetchByID(schedule_rec.play_id,&play_rec);
	//需要增加查找座位信息

	printf("\n=======================================================\n");
	printf("****************  Update Ticket Information  ****************\n");
	printf("-------------------------------------------------------\n");
	printf("Ticket ID:%d\n", rec.id);
	printf("Play Name[%s]:", play_rec.name);
	//需要输出座位的行列号
	printf("Schedule date(yyyy-mm-dd)[%d-%d-%d]:",schedule_rec.date.year,
			schedule_rec.date.month,schedule_rec.date.day);
	printf("Schedule time[%d:%d]:",schedule_rec.time.hour,schedule_rec.time.minute);
	printf("Ticket price[%d]:",rec.price);
	scanf("%d",&(rec.price));
	printf("Ticket status[%s](0.to sell,1.sold):",rec.status==1?"to sell":"sold");
	scanf("%d",&rec.status);
	printf("-------------------------------------------------------\n");

	if (Ticket_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"The ticket information updated successfully!\nPress [Enter] key to return!\n");
	} else
		printf("The ticket information updated failed!\nPress [Enter] key to return!\n");

	getchar();
	return rtn;
}


int QueryTicket(int id){

	int rtn=0;
	schedule_t schedule_rec;
	play_t play_rec;
	ticket_t rec;
	if (Ticket_Srv_FetchByID(id,&rec)) {
		printf("\n\n\n\t\t\t=======================================================\n");
		printf("\t\t\t*******************  Ticket Information   *******************\n");
		printf("\t\t\tID\tPlay Name\tDate\tTime\tPrice\tStatus\n");
		printf("\t\t\t-------------------------------------------------------\n");
		Schedule_Srv_FetchByID(id,&schedule_rec);
		Play_Srv_FetchByID(schedule_rec.play_id,&play_rec);
		//添加查找座位行列信息的函数
		printf("\t\t\t%d\t%s\\t%d-%d-%d\t%d:%d\t%d\t%d\n"/*t%d\t%d*/, rec.id, play_rec.name,
//					seat_rec.row, seat_rec.column,
				schedule_rec.date.year,schedule_rec.date.month,
				schedule_rec.date.day,schedule_rec.time.hour,
				schedule_rec.time.minute,rec.price,rec.status);
		rtn=1;
	}
	else
	{
		printf("\t\t\tThe ticket does not exist!\nPress [Enter] key to return!\n");
	}
	printf("\t\t\t-------------------------------------------------------\n");

	system("pause");
	return rtn;
}
