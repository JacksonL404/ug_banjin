#include <stdio.h>
#include <uf.h>
#include <uf_modl.h>

#define UF_CALL(X) (report( __FILE__, __LINE__, #X, (X)))

static int report(char *file, int line, char *call, int irc)
{
	if (irc)
	{
		char    messg[133];
		printf("%s, line %d:  %s\n", file, line, call);
		(UF_get_fail_message(irc, messg)) ?
			printf("    returned a %d\n", irc) :
			printf("    returned error %d:  %s\n", irc, messg);
	}
	return(irc);
}

// 该函数对一个二维数组的每一行进行初始化
// 输入为 数组名array，初始化的行数(第几行) row, 本行的具体数值(double型) x1, x2, x3
static void myarray_init1(double array[][3],int row,double x1,double x2, double x3)
{
	array[row][0] = x1;
	array[row][1] = x2;
	array[row][2] = x3;
}

void myarray_init2(double array[][3], int row, double array1[])
{
	array[row][0] = array1[0];
	array[row][1] = array1[1];
	array[row][2] = array1[2];
}

// 该函数使用上述的二维数组，对每一个UF_CURVE_line_t line中的start_point, end_point进行赋值
// 注意二维数组的相邻两行 为线段的起点和终点坐标
// 函数输入为 指向UF_CURVE_line_t型的指针 *line, 二维数组array, 数组中线段起点坐标的行数n
//static void myline(UF_CURVE_line_t* line, double array[][3],int n)
//{
//	// 起点坐标
//	line->start_point[0] = array[n][0];
//	line->start_point[1] = array[n][1];
//	line->start_point[2] = array[n][2];
//	//由于最后起点和终点相连，所以将n=-1, 这样n+1=0为第一个坐标
//	if (n + 1 > 11) n = -1;
//	//终点坐标
//	line->end_point[0] = array[n+1][0];
//	line->end_point[1] = array[n+1][1];
//	line->end_point[2] = array[n+1][2];
//}

static void do_ugopen_api(void)
{
	UF_FEATURE_SIGN create = UF_NULLSIGN;	// 定义实体标记，表示创建实体
	UF_FEATURE_SIGN sub = UF_NEGATIVE;		// 定义实体标记，表示从目标实体中挖去一部分
	UF_FEATURE_SIGN add = UF_POSITIVE;		// 布尔 并运算

	// 创建线段数组
	UF_CURVE_line_t line[12];
	//UF_CURVE_line_t line1, line2, line3, line4,  line5,  line6;
	//UF_CURVE_line_t line7, line8, line9, line10, line11, line12;
	
	double ref_pt[12];
	// 草图线段起始点
	double direction1[3] = { -1.0,0.0,0.0 };// 拉伸方向
	double linept[12][3];					// 存储线段所有坐标的二维数组
	// 初始化二维数组中所有坐标
	myarray_init1(linept, 0, 23.0, -23.0, 12.0);
	myarray_init1(linept, 1, 23.0, -23.0, 10.0);
	myarray_init1(linept, 2, 23.0, -14.0, 10.0);
	myarray_init1(linept, 3, 23.0, -14.0, 0.0);
	myarray_init1(linept, 4, 23.0,  14.0, 0.0);
	myarray_init1(linept, 5, 23.0,  14.0, 10.0);
	myarray_init1(linept, 6, 23.0,  23.0, 10.0);
	myarray_init1(linept, 7, 23.0,  23.0, 12.0);
	myarray_init1(linept, 8, 23.0,  12.0, 12.0);
	myarray_init1(linept, 9, 23.0,  12.0, 2.0);
	myarray_init1(linept, 10,23.0, -12.0, 2.0);
	myarray_init1(linept, 11,23.0, -12.0, 12.0);
	//初始化每一条线段起点，终点坐标
	for (int i = 0; i < 12;i++)
	{
		// 起点坐标
		line[i].start_point[0] = linept[i][0];
		line[i].start_point[1] = linept[i][1];
		line[i].start_point[2] = linept[i][2];

		//特殊情况下的终点坐标（先判断）
		//由于最后起点和终点相连，所以将n=-1, 这样n+1=0为第一个坐标
		if (i + 1 > 11)
		{
			// 这里前面的line数组的序号不变，需要先存到临时变量n中
			int n = i;
			i = -1;
			line[n].end_point[0] = linept[i + 1][0];
			line[n].end_point[1] = linept[i + 1][1];
			line[n].end_point[2] = linept[i + 1][2];
			break;
		}
		//非特殊情况下的终点坐标
		line[i].end_point[0] = linept[i + 1][0];
		line[i].end_point[1] = linept[i + 1][1];
		line[i].end_point[2] = linept[i + 1][2];

		
	}
	//myline(&line1, linept, 0);
	//myline(&line2, linept, 1);
	//myline(&line3, linept, 2);
	//myline(&line4, linept, 3);
	//myline(&line5, linept, 4);
	//myline(&line6, linept, 5);
	//myline(&line7, linept, 6);
	//myline(&line8, linept, 7);
	//myline(&line9, linept, 8);
	//myline(&line10, linept, 9);
	//myline(&line11, linept, 10);
	//myline(&line12, linept, 11);

	tag_t objarray[12];
	uf_list_p_t loop_list, features;
	char *taper_angle = "0.0";		// 拉伸角度
	char *limit1[2] = { "0", "46" };// 拉伸距离
	int i, count = 12;
	// 画出草图
	for (i = 0; i < count;i++)
	{
		UF_CALL(UF_CURVE_create_line(&line[i], &objarray[i]));
	}
	//UF_CALL(UF_CURVE_create_line(&line1, &objarray[0]));
	//UF_CALL(UF_CURVE_create_line(&line2, &objarray[1]));
	//UF_CALL(UF_CURVE_create_line(&line3, &objarray[2]));
	//UF_CALL(UF_CURVE_create_line(&line4, &objarray[3]));
	//UF_CALL(UF_CURVE_create_line(&line5, &objarray[4]));
	//UF_CALL(UF_CURVE_create_line(&line6, &objarray[5]));
	//UF_CALL(UF_CURVE_create_line(&line7, &objarray[6]));
	//UF_CALL(UF_CURVE_create_line(&line8, &objarray[7]));
	//UF_CALL(UF_CURVE_create_line(&line9, &objarray[8]));
	//UF_CALL(UF_CURVE_create_line(&line10, &objarray[9]));
	//UF_CALL(UF_CURVE_create_line(&line11, &objarray[10]));
	//UF_CALL(UF_CURVE_create_line(&line12, &objarray[11]));

	// 创建线段链表
	UF_CALL(UF_MODL_create_list(&loop_list));
	for (i = 0; i < count; i++) {
		UF_CALL(UF_MODL_put_list_item(loop_list, objarray[i]));
	}
	// 拉伸面得到实体
	UF_CALL(UF_MODL_create_extruded(loop_list, taper_angle, limit1, ref_pt, direction1, create, &features));

	// 获取拉伸实体id
	tag_t feat_id = features->eid;

	// 隐藏父级项目
	//int n_unch_disp_stat, n_ch_disp_stat;
	//UF_MODL_disp_info_p_t unch_parents_disp_status = NULL;
	//UF_MODL_disp_info_p_t ch_parents_disp_status = NULL;
	//logical selectable = "true";
	//UF_CALL(UF_MODL_show_parent_curves(feat_id, selectable, &n_unch_disp_stat,
	//	&unch_parents_disp_status, &n_ch_disp_stat, &ch_parents_disp_status));
	//UF_CALL(UF_MODL_hide_parent_curves(feat_id, n_unch_disp_stat, unch_parents_disp_status,
	//	n_ch_disp_stat, &ch_parents_disp_status));

	// 删除链表，释放内存
	UF_CALL(UF_MODL_delete_list(&loop_list));
	UF_CALL(UF_MODL_delete_list(&features));

	//创建圆柱，减运算
	double origin1[3] = { 18.0, -14.0, 7.0 };
	char * height1 = "28";
	char * diam1 = "4";
	double direction2[3] = { 0.0,1.0,0.0 };
	tag_t cyl_obj_id1 = NULL_TAG;
	UF_CALL(UF_MODL_create_cyl1(sub, origin1, height1, diam1, direction2, &cyl_obj_id1));

	// 创建块，减运算
	double corner_pt[3] = { -13.0,-9.25,0.0 };	//起始角点
	char *edge_len[3] = { "26","18.5","2" };	//长方体棱长
	tag_t blk_obj_id;
	UF_CALL(UF_MODL_create_block1(sub, corner_pt, edge_len, &blk_obj_id));

	// 创建上方挖孔圆柱，减运算
	double origin2[3] = { -18.0, -19.0, 12.0 };	//底面圆心
	char * height2 = "2";						//圆柱高度
	char * diam2 = "3";							//底面直径
	double direction3[3] = { 0.0,0.0,-1.0 };	//拉伸方向
	tag_t cyl_obj_id2 = NULL_TAG;
	UF_CALL(UF_MODL_create_cyl1(sub, origin2, height2, diam2, direction3, &cyl_obj_id2));

	// 上方孔 线性阵列
	uf_list_p_t linear_list;
	tag_t linear_obj_id;
	UF_CALL(UF_MODL_create_list(&linear_list));					// 线性阵列链表
	UF_CALL(UF_MODL_put_list_item(linear_list, cyl_obj_id2));	// 孔tag号进入链表
	char * number_in_x = "3";									// x 方向实体数
	char * distance_x = "18";									// x 方向距离
	char * number_in_y = "2";									// y 方向实体数
	char * distance_y = "38";									// y 方向距离
	UF_CALL(UF_MODL_create_linear_iset(0, number_in_x, distance_x, number_in_y, distance_y,
		linear_list, &linear_obj_id));
	UF_CALL(UF_MODL_delete_list(&linear_list));

	// 边线list指针
	uf_list_p_t edgelist_all1, edgelist_all2, edgelist2, edgelist3, edgelist4;
	// 边线tag，不同尺寸的圆角tag
	tag_t edgeTag1, edgeTag2, featureR1, featureR2, featureR3;
	int list_count1, list_count2; //所有找到边的数量
	// 拉伸面 特征找边
	UF_CALL(UF_MODL_ask_feat_edges(feat_id, &edgelist_all1));		// 拉伸体找到的所有边存到edgelist_all1中
	UF_CALL(UF_MODL_ask_feat_edges(blk_obj_id, &edgelist_all2));	// 底面的切除块上所有边存入edgelist_all2中
	UF_CALL(UF_MODL_ask_list_count(edgelist_all1, &list_count1));	// 获得edgelist1中所有边的数量，存入list_count1
	UF_CALL(UF_MODL_ask_list_count(edgelist_all2, &list_count2));   // 获得edgelist2中所有边的数量，存入list_count2

	// 特征点（边线中点）
	double pts[8][3];
	// R = 1.5
	double pt1[3] = { 0.0, -14.0, 10.0 };
	double pt2[3] = { 0.0,  14.0, 10.0 };
	// R = 0.5
	double pt3[3] = { 0.0,  12.0,  2.0 };
	double pt4[3] = { 0.0, -12.0,  2.0 };
	// R = 2.0
	double pt5[3] = {  13.0,  9.25,  1.0 };
	double pt6[3] = {  13.0, -9.25,  1.0 };
	double pt7[3] = { -13.0, -9.25,  1.0 };
	double pt8[3] = { -13.0,  9.25,  1.0 };
	//将所有点放到二维数组中
	myarray_init2(pts, 0, pt1);
	myarray_init2(pts, 1, pt2);
	myarray_init2(pts, 2, pt3);
	myarray_init2(pts, 3, pt4);
	myarray_init2(pts, 4, pt5);
	myarray_init2(pts, 5, pt6);
	myarray_init2(pts, 6, pt7);
	myarray_init2(pts, 7, pt8);
	// 用来存储UF_MODL_ask_point_containment函数的返回值，便于后续判断线段归属列表
	int pt_status[8];

	// 创建两个不同的链表，用于圆角构建
	UF_CALL(UF_MODL_create_list(&edgelist2)); // R = 1.5
	UF_CALL(UF_MODL_create_list(&edgelist3)); // R = 0.5
	UF_CALL(UF_MODL_create_list(&edgelist4)); // R = 2.0

	// 两次for 循环遍历所有边
	for (int i = 0; i < list_count1; i++)
	{
		// 从链表中edgelist_all1拿一个边的tag，放到edgeTag1中
		UF_CALL(UF_MODL_ask_list_item(edgelist_all1, i, &edgeTag1));

		// 对每一个特征点进行判断，对应结果储存到pt_status中
		//for 循环遍历每一个点，由于每个点最多只会出现在一条边上，所以不需要每次对pt_status初始化
		for (int j = 0; j < 8;j++) 
		{
			UF_CALL(UF_MODL_ask_point_containment(pts[j], edgeTag1, &pt_status[j]));
		}

		// 如果该边线是圆角为1.5的边（即pt1或pt2在这条边内部）
		if (pt_status[0] == 1 || pt_status[1] == 1)
		{
			UF_CALL(UF_MODL_put_list_item(edgelist2, edgeTag1));// 放到edgelist2中
		}
		// 如果该边线是圆角为0.5的边（即pt3或pt4在这条边内部）
		if (pt_status[2] == 1 || pt_status[3] == 1)
		{
			UF_CALL(UF_MODL_put_list_item(edgelist3, edgeTag1));// 放到edgelist3中
		}
		// 如果该边线是圆角为2.0的边（即pt5或pt6或pt7或pt8在这条边内部）
		if (pt_status[4] == 1 || pt_status[5] == 1 ||
			pt_status[6] == 1 || pt_status[7] == 1)
		{
			UF_CALL(UF_MODL_put_list_item(edgelist4, edgeTag1));// 放到edgelist4中
		}
	}

	for (int i = 0; i < list_count2; i++)
	{
		// 从链表edgelist_all2中拿一个边的tag，放到edgeTag2中
		UF_CALL(UF_MODL_ask_list_item(edgelist_all2, i, &edgeTag2));

		for (int j = 0; j < 8; j++)
		{
			UF_CALL(UF_MODL_ask_point_containment(pts[j], edgeTag2, &pt_status[j]));
		}

		if (pt_status[0] == 1 || pt_status[1] == 1)
		{
			UF_CALL(UF_MODL_put_list_item(edgelist2, edgeTag2));// 放到edgelist2中
		}

		if (pt_status[2] == 1 || pt_status[3] == 1)
		{
			UF_CALL(UF_MODL_put_list_item(edgelist3, edgeTag2));// 放到edgelist3中
		}

		if (pt_status[4] == 1 || pt_status[5] == 1 ||
			pt_status[6] == 1 || pt_status[7] == 1)
		{
			UF_CALL(UF_MODL_put_list_item(edgelist4, edgeTag2));// 放到edgelist4中
		}
	}

	// 获取建模公差
	double tolerance;
	UF_MODL_ask_distance_tolerance(&tolerance);
	// 创建圆角
	UF_CALL(UF_MODL_create_blend("1.5", edgelist2, 0, 0, 0, tolerance, &featureR1));// R = 1.5
	UF_CALL(UF_MODL_create_blend("0.5", edgelist3, 0, 0, 0, tolerance, &featureR2));// R = 0.5
	UF_CALL(UF_MODL_create_blend("2.0", edgelist4, 0, 0, 0, tolerance, &featureR3));// R = 2.0
	// 删除链表，释放内存
	UF_CALL(UF_MODL_delete_list(&edgelist2));
	UF_CALL(UF_MODL_delete_list(&edgelist3));
	UF_CALL(UF_MODL_delete_list(&edgelist4));

}

/*ARGSUSED*/
void ufusr(char *param, int *retcode, int paramLen)
{


	if (!UF_CALL(UF_initialize()))
	{
		do_ugopen_api();
		UF_CALL(UF_terminate());
	}
}

int ufusr_ask_unload(void)
{
	return (UF_UNLOAD_IMMEDIATELY);
}