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

// �ú�����һ����ά�����ÿһ�н��г�ʼ��
// ����Ϊ ������array����ʼ��������(�ڼ���) row, ���еľ�����ֵ(double��) x1, x2, x3
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

// �ú���ʹ�������Ķ�ά���飬��ÿһ��UF_CURVE_line_t line�е�start_point, end_point���и�ֵ
// ע���ά������������� Ϊ�߶ε������յ�����
// ��������Ϊ ָ��UF_CURVE_line_t�͵�ָ�� *line, ��ά����array, �������߶�������������n
//static void myline(UF_CURVE_line_t* line, double array[][3],int n)
//{
//	// �������
//	line->start_point[0] = array[n][0];
//	line->start_point[1] = array[n][1];
//	line->start_point[2] = array[n][2];
//	//������������յ����������Խ�n=-1, ����n+1=0Ϊ��һ������
//	if (n + 1 > 11) n = -1;
//	//�յ�����
//	line->end_point[0] = array[n+1][0];
//	line->end_point[1] = array[n+1][1];
//	line->end_point[2] = array[n+1][2];
//}

static void do_ugopen_api(void)
{
	UF_FEATURE_SIGN create = UF_NULLSIGN;	// ����ʵ���ǣ���ʾ����ʵ��
	UF_FEATURE_SIGN sub = UF_NEGATIVE;		// ����ʵ���ǣ���ʾ��Ŀ��ʵ������ȥһ����
	UF_FEATURE_SIGN add = UF_POSITIVE;		// ���� ������

	// �����߶�����
	UF_CURVE_line_t line[12];
	//UF_CURVE_line_t line1, line2, line3, line4,  line5,  line6;
	//UF_CURVE_line_t line7, line8, line9, line10, line11, line12;
	
	double ref_pt[12];
	// ��ͼ�߶���ʼ��
	double direction1[3] = { -1.0,0.0,0.0 };// ���췽��
	double linept[12][3];					// �洢�߶���������Ķ�ά����
	// ��ʼ����ά��������������
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
	//��ʼ��ÿһ���߶���㣬�յ�����
	for (int i = 0; i < 12;i++)
	{
		// �������
		line[i].start_point[0] = linept[i][0];
		line[i].start_point[1] = linept[i][1];
		line[i].start_point[2] = linept[i][2];

		//��������µ��յ����꣨���жϣ�
		//������������յ����������Խ�n=-1, ����n+1=0Ϊ��һ������
		if (i + 1 > 11)
		{
			// ����ǰ���line�������Ų��䣬��Ҫ�ȴ浽��ʱ����n��
			int n = i;
			i = -1;
			line[n].end_point[0] = linept[i + 1][0];
			line[n].end_point[1] = linept[i + 1][1];
			line[n].end_point[2] = linept[i + 1][2];
			break;
		}
		//����������µ��յ�����
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
	char *taper_angle = "0.0";		// ����Ƕ�
	char *limit1[2] = { "0", "46" };// �������
	int i, count = 12;
	// ������ͼ
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

	// �����߶�����
	UF_CALL(UF_MODL_create_list(&loop_list));
	for (i = 0; i < count; i++) {
		UF_CALL(UF_MODL_put_list_item(loop_list, objarray[i]));
	}
	// ������õ�ʵ��
	UF_CALL(UF_MODL_create_extruded(loop_list, taper_angle, limit1, ref_pt, direction1, create, &features));

	// ��ȡ����ʵ��id
	tag_t feat_id = features->eid;

	// ���ظ�����Ŀ
	//int n_unch_disp_stat, n_ch_disp_stat;
	//UF_MODL_disp_info_p_t unch_parents_disp_status = NULL;
	//UF_MODL_disp_info_p_t ch_parents_disp_status = NULL;
	//logical selectable = "true";
	//UF_CALL(UF_MODL_show_parent_curves(feat_id, selectable, &n_unch_disp_stat,
	//	&unch_parents_disp_status, &n_ch_disp_stat, &ch_parents_disp_status));
	//UF_CALL(UF_MODL_hide_parent_curves(feat_id, n_unch_disp_stat, unch_parents_disp_status,
	//	n_ch_disp_stat, &ch_parents_disp_status));

	// ɾ�������ͷ��ڴ�
	UF_CALL(UF_MODL_delete_list(&loop_list));
	UF_CALL(UF_MODL_delete_list(&features));

	//����Բ����������
	double origin1[3] = { 18.0, -14.0, 7.0 };
	char * height1 = "28";
	char * diam1 = "4";
	double direction2[3] = { 0.0,1.0,0.0 };
	tag_t cyl_obj_id1 = NULL_TAG;
	UF_CALL(UF_MODL_create_cyl1(sub, origin1, height1, diam1, direction2, &cyl_obj_id1));

	// �����飬������
	double corner_pt[3] = { -13.0,-9.25,0.0 };	//��ʼ�ǵ�
	char *edge_len[3] = { "26","18.5","2" };	//�������ⳤ
	tag_t blk_obj_id;
	UF_CALL(UF_MODL_create_block1(sub, corner_pt, edge_len, &blk_obj_id));

	// �����Ϸ��ڿ�Բ����������
	double origin2[3] = { -18.0, -19.0, 12.0 };	//����Բ��
	char * height2 = "2";						//Բ���߶�
	char * diam2 = "3";							//����ֱ��
	double direction3[3] = { 0.0,0.0,-1.0 };	//���췽��
	tag_t cyl_obj_id2 = NULL_TAG;
	UF_CALL(UF_MODL_create_cyl1(sub, origin2, height2, diam2, direction3, &cyl_obj_id2));

	// �Ϸ��� ��������
	uf_list_p_t linear_list;
	tag_t linear_obj_id;
	UF_CALL(UF_MODL_create_list(&linear_list));					// ������������
	UF_CALL(UF_MODL_put_list_item(linear_list, cyl_obj_id2));	// ��tag�Ž�������
	char * number_in_x = "3";									// x ����ʵ����
	char * distance_x = "18";									// x �������
	char * number_in_y = "2";									// y ����ʵ����
	char * distance_y = "38";									// y �������
	UF_CALL(UF_MODL_create_linear_iset(0, number_in_x, distance_x, number_in_y, distance_y,
		linear_list, &linear_obj_id));
	UF_CALL(UF_MODL_delete_list(&linear_list));

	// ����listָ��
	uf_list_p_t edgelist_all1, edgelist_all2, edgelist2, edgelist3, edgelist4;
	// ����tag����ͬ�ߴ��Բ��tag
	tag_t edgeTag1, edgeTag2, featureR1, featureR2, featureR3;
	int list_count1, list_count2; //�����ҵ��ߵ�����
	// ������ �����ұ�
	UF_CALL(UF_MODL_ask_feat_edges(feat_id, &edgelist_all1));		// �������ҵ������бߴ浽edgelist_all1��
	UF_CALL(UF_MODL_ask_feat_edges(blk_obj_id, &edgelist_all2));	// ������г��������бߴ���edgelist_all2��
	UF_CALL(UF_MODL_ask_list_count(edgelist_all1, &list_count1));	// ���edgelist1�����бߵ�����������list_count1
	UF_CALL(UF_MODL_ask_list_count(edgelist_all2, &list_count2));   // ���edgelist2�����бߵ�����������list_count2

	// �����㣨�����е㣩
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
	//�����е�ŵ���ά������
	myarray_init2(pts, 0, pt1);
	myarray_init2(pts, 1, pt2);
	myarray_init2(pts, 2, pt3);
	myarray_init2(pts, 3, pt4);
	myarray_init2(pts, 4, pt5);
	myarray_init2(pts, 5, pt6);
	myarray_init2(pts, 6, pt7);
	myarray_init2(pts, 7, pt8);
	// �����洢UF_MODL_ask_point_containment�����ķ���ֵ�����ں����ж��߶ι����б�
	int pt_status[8];

	// ����������ͬ����������Բ�ǹ���
	UF_CALL(UF_MODL_create_list(&edgelist2)); // R = 1.5
	UF_CALL(UF_MODL_create_list(&edgelist3)); // R = 0.5
	UF_CALL(UF_MODL_create_list(&edgelist4)); // R = 2.0

	// ����for ѭ���������б�
	for (int i = 0; i < list_count1; i++)
	{
		// ��������edgelist_all1��һ���ߵ�tag���ŵ�edgeTag1��
		UF_CALL(UF_MODL_ask_list_item(edgelist_all1, i, &edgeTag1));

		// ��ÿһ������������жϣ���Ӧ������浽pt_status��
		//for ѭ������ÿһ���㣬����ÿ�������ֻ�������һ�����ϣ����Բ���Ҫÿ�ζ�pt_status��ʼ��
		for (int j = 0; j < 8;j++) 
		{
			UF_CALL(UF_MODL_ask_point_containment(pts[j], edgeTag1, &pt_status[j]));
		}

		// ����ñ�����Բ��Ϊ1.5�ıߣ���pt1��pt2���������ڲ���
		if (pt_status[0] == 1 || pt_status[1] == 1)
		{
			UF_CALL(UF_MODL_put_list_item(edgelist2, edgeTag1));// �ŵ�edgelist2��
		}
		// ����ñ�����Բ��Ϊ0.5�ıߣ���pt3��pt4���������ڲ���
		if (pt_status[2] == 1 || pt_status[3] == 1)
		{
			UF_CALL(UF_MODL_put_list_item(edgelist3, edgeTag1));// �ŵ�edgelist3��
		}
		// ����ñ�����Բ��Ϊ2.0�ıߣ���pt5��pt6��pt7��pt8���������ڲ���
		if (pt_status[4] == 1 || pt_status[5] == 1 ||
			pt_status[6] == 1 || pt_status[7] == 1)
		{
			UF_CALL(UF_MODL_put_list_item(edgelist4, edgeTag1));// �ŵ�edgelist4��
		}
	}

	for (int i = 0; i < list_count2; i++)
	{
		// ������edgelist_all2����һ���ߵ�tag���ŵ�edgeTag2��
		UF_CALL(UF_MODL_ask_list_item(edgelist_all2, i, &edgeTag2));

		for (int j = 0; j < 8; j++)
		{
			UF_CALL(UF_MODL_ask_point_containment(pts[j], edgeTag2, &pt_status[j]));
		}

		if (pt_status[0] == 1 || pt_status[1] == 1)
		{
			UF_CALL(UF_MODL_put_list_item(edgelist2, edgeTag2));// �ŵ�edgelist2��
		}

		if (pt_status[2] == 1 || pt_status[3] == 1)
		{
			UF_CALL(UF_MODL_put_list_item(edgelist3, edgeTag2));// �ŵ�edgelist3��
		}

		if (pt_status[4] == 1 || pt_status[5] == 1 ||
			pt_status[6] == 1 || pt_status[7] == 1)
		{
			UF_CALL(UF_MODL_put_list_item(edgelist4, edgeTag2));// �ŵ�edgelist4��
		}
	}

	// ��ȡ��ģ����
	double tolerance;
	UF_MODL_ask_distance_tolerance(&tolerance);
	// ����Բ��
	UF_CALL(UF_MODL_create_blend("1.5", edgelist2, 0, 0, 0, tolerance, &featureR1));// R = 1.5
	UF_CALL(UF_MODL_create_blend("0.5", edgelist3, 0, 0, 0, tolerance, &featureR2));// R = 0.5
	UF_CALL(UF_MODL_create_blend("2.0", edgelist4, 0, 0, 0, tolerance, &featureR3));// R = 2.0
	// ɾ�������ͷ��ڴ�
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