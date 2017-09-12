//
// Created by 周立海 on 2017/4/21.
//
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <math.h>

#include "AccessFun.h"
#include "class8.h"
#include "PublicFunction.h"
#include "dlt698.h"

int class8001_act127(int index, int attr_act, INT8U *data,
		Action_result *act_ret) {
	asyslog(LOG_WARNING, "投入保电\n");
	CLASS_8001 c8001;
	readCoverClass(0x8001, 0, (void *) &c8001, sizeof(CLASS_8001),
			para_vari_save);
	c8001.state = 1;
	saveCoverClass(0x8001, 0, (void *) &c8001, sizeof(CLASS_8001),
			para_vari_save);
	return 0;
}

int class8001_act128(int index, int attr_act, INT8U *data,
		Action_result *act_ret) {
	asyslog(LOG_WARNING, "解除保电\n");
	CLASS_8001 c8001;
	readCoverClass(0x8001, 0, (void *) &c8001, sizeof(CLASS_8001),
			para_vari_save);
	c8001.state = 0;
	saveCoverClass(0x8001, 0, (void *) &c8001, sizeof(CLASS_8001),
			para_vari_save);
	return 0;
}

int class8001_act129(int index, int attr_act, INT8U *data,
		Action_result *act_ret) {
	asyslog(LOG_WARNING, "解除自动保电\n");
	CLASS_8001 c8001;
	readCoverClass(0x8001, 0, (void *) &c8001, sizeof(CLASS_8001),
			para_vari_save);
	c8001.state = 2;
	saveCoverClass(0x8001, 0, (void *) &c8001, sizeof(CLASS_8001),
			para_vari_save);
	return 0;
}

int class8001_act_route(int index, int attr_act, INT8U *data,
		Action_result *act_ret) {
	switch (attr_act) {
	case 127:
		class8001_act127(1, attr_act, data, act_ret);
		break;
	case 128:
		class8001_act128(1, attr_act, data, act_ret);
		break;
	case 129:
		class8001_act129(1, attr_act, data, act_ret);
		break;
	}
}

int class8002_act127(int index, int attr_act, INT8U *data,
		Action_result *act_ret) {
	asyslog(LOG_WARNING, "催费告警投入\n");
	CLASS_8002 c8002;
	readCoverClass(0x8002, 0, (void *) &c8002, sizeof(CLASS_8002),
			para_vari_save);
	c8002.state = 1;
	saveCoverClass(0x8002, 0, (void *) &c8002, sizeof(CLASS_8002),
			para_vari_save);
	return 0;
}

int class8002_act128(int index, int attr_act, INT8U *data,
		Action_result *act_ret) {
	asyslog(LOG_WARNING, "催费告警退出\n");
	CLASS_8002 c8002;
	readCoverClass(0x8002, 0, (void *) &c8002, sizeof(CLASS_8002),
			para_vari_save);
	c8002.state = 0;
	saveCoverClass(0x8002, 0, (void *) &c8002, sizeof(CLASS_8002),
			para_vari_save);
	return 0;
}

int class8002_act_route(int index, int attr_act, INT8U *data, Action_result *act_ret) {
	switch (attr_act) {
	case 127:
		class8002_act127(1, attr_act, data, act_ret);
		break;
	case 128:
		class8002_act128(1, attr_act, data, act_ret);
		break;
	}
}
int class8000_act129(int index, int attr_act, INT8U *data,
		Action_result *act_ret) {

	ProgramInfo *shareAddr = getShareAddr();

	shareAddr->ctrls.cf205.currentState = 1;

	if(shareAddr->ctrls.cf205.currentState == 0){
		return 0;
	}

	if(data[0] != 0x01 || data[1] != 0x01){
		return -1;
	}
	if(data[2] != 0x02 || data[3] != 0x04){
		return -1;
	}

	OI_698 oi = data[5] * 256 + data[6];
	int delay = data[10];
	int limit_time = data[12] * 256 + data[13];
	int auto_act = data[15];

	asyslog(LOG_WARNING, "遥控跳闸 %d - %d - %d\n", delay, limit_time, auto_act);

	shareAddr->ctrls.control[0] = 0xEEFFEFEF;
	shareAddr->ctrls.control[1] = 0xEEFFEFEF;
	shareAddr->ctrls.control[2] = 0xEEFFEFEF;

	Event_3115(NULL, 0, getShareAddr());

	return 0;
}

int class8000_act130(int index, int attr_act, INT8U *data,
		Action_result *act_ret) {

	ProgramInfo *shareAddr = getShareAddr();

	shareAddr->ctrls.cf205.currentState = 0;

	if(shareAddr->ctrls.cf205.currentState == 0){
		return 0;
	}

	if(data[0] != 0x01 || data[1] != 0x01){
		return -1;
	}
	if(data[2] != 0x02 || data[3] != 0x04){
		return -1;
	}

	OI_698 oi = data[5] * 256 + data[6];
	int allow_or_auto = data[10];

	asyslog(LOG_WARNING, "遥控合闸 %d\n", allow_or_auto);

	shareAddr->ctrls.control[0] = 0xCCAACACA;
	shareAddr->ctrls.control[1] = 0xCCAACACA;
	shareAddr->ctrls.control[2] = 0xCCAACACA;

	Event_3115(NULL, 0, getShareAddr());

	return 0;
}


int class8000_act_route(int index, int attr_act, INT8U *data,
		Action_result *act_ret) {
	switch (attr_act) {
	case 127:
//		class8000_act127(1, attr_act, data, act_ret);
		break;
	case 128:
//		class8000_act128(1, attr_act, data, act_ret);
		break;
	case 129:
		class8000_act129(1, attr_act, data, act_ret);
		break;
	case 130:
		class8000_act130(1, attr_act, data, act_ret);
		break;
	}
}

INT64U getLongValue(INT8U *data) {
	INT64U v = 0x00;
	for (int i = 0; i < 8; ++i) {
		v += data[i + 1];
		if (i + 1 == 8) {
			break;
		}
		v = v << 8;
	}
	return v;
}

int class8100_set(int index, OAD oad, INT8U *data, INT8U *DAR) {
	CLASS_8100 c8100;
	if (data[0] != 0x14) {
		return 0;
	}

	readCoverClass(0x8100, 0, (void *) &c8100, sizeof(CLASS_8100),
			para_vari_save);
	c8100.v = getLongValue(data);
	saveCoverClass(0x8100, 0, (void *) &c8100, sizeof(CLASS_8100),
			para_vari_save);
	asyslog(LOG_WARNING, "设置终端安保定值(%lld)", c8100.v);

	return 0;
}

int class8101_set(int index, OAD oad, INT8U *data, INT8U *DAR) {
	CLASS_8101 c8101;
	if (data[0] != 0x01 || data[1] != 0x0C) {
		return 0;
	}
	readCoverClass(0x8101, 0, (void *) &c8101, sizeof(CLASS_8101),
			para_vari_save);
	for (int i = 0; i < 12; ++i) {
		c8101.time[i] = data[i * 2 + 3];
		printf("%02x\n", c8101.time[i]);
	}
	saveCoverClass(0x8101, 0, (void *) &c8101, sizeof(CLASS_8101),
			para_vari_save);
	return 0;
}

int class8102_set(int index, OAD oad, INT8U *data, INT8U *DAR) {
	CLASS_8102 c8102;
	if (data[0] != 0x01 || data[1] != 0x08) {
		return 0;
	}
	readCoverClass(0x8102, 0, (void *) &c8102, sizeof(CLASS_8102),
			para_vari_save);
	for (int i = 0; i < 8; ++i) {
		c8102.time[i] = data[i * 2 + 3];
		printf("%02x\n", c8102.time[i]);
	}
	saveCoverClass(0x8102, 0, (void *) &c8102, sizeof(CLASS_8102),
			para_vari_save);
	return 0;
}

int class8103_act3(int index, int attr_act, INT8U *data, Action_result *act_ret) {
	CLASS_8103 c8103;

	if (data[0] != 0x02 || data[1] != 0x06) {
		return 0;
	}

	if (data[2] != 0x50) {
		return 0;
	}

	c8103.list[0].index = data[3] * 256 + data[4];
	c8103.list[0].sign = data[7];

	if (data[8] != 0x02 || data[9] != 0x09) {
		return 0;
	}

	c8103.list[0].v1.n = data[12];
	c8103.list[0].v1.t1 = getLongValue(&data[13]);
	c8103.list[0].v1.t2 = getLongValue(&data[22]);
	c8103.list[0].v1.t3 = getLongValue(&data[31]);
	c8103.list[0].v1.t4 = getLongValue(&data[40]);
	c8103.list[0].v1.t5 = getLongValue(&data[49]);
	c8103.list[0].v1.t6 = getLongValue(&data[58]);
	c8103.list[0].v1.t7 = getLongValue(&data[67]);
	c8103.list[0].v1.t8 = getLongValue(&data[76]);

	if (data[85] != 0x02 || data[86] != 0x09) {
		return 0;
	}

	c8103.list[0].v2.n = data[89];
	c8103.list[0].v2.t1 = getLongValue(&data[90]);
	c8103.list[0].v2.t2 = getLongValue(&data[99]);
	c8103.list[0].v2.t3 = getLongValue(&data[108]);
	c8103.list[0].v2.t4 = getLongValue(&data[117]);
	c8103.list[0].v2.t5 = getLongValue(&data[126]);
	c8103.list[0].v2.t6 = getLongValue(&data[135]);
	c8103.list[0].v2.t7 = getLongValue(&data[144]);
	c8103.list[0].v2.t8 = getLongValue(&data[153]);

	if (data[162] != 0x02 || data[163] != 0x09) {
		return 0;
	}

	c8103.list[0].v2.n = data[166];
	c8103.list[0].v2.t1 = getLongValue(&data[175]);
	c8103.list[0].v2.t2 = getLongValue(&data[184]);
	c8103.list[0].v2.t3 = getLongValue(&data[193]);
	c8103.list[0].v2.t4 = getLongValue(&data[202]);
	c8103.list[0].v2.t5 = getLongValue(&data[211]);
	c8103.list[0].v2.t6 = getLongValue(&data[220]);
	c8103.list[0].v2.t7 = getLongValue(&data[229]);
	c8103.list[0].v2.t8 = getLongValue(&data[238]);

	c8103.list[0].para = data[248];
	printf("c8103 act 3\n");
	return 0;
}

int class8103_act6(int index, int attr_act, INT8U *data, Action_result *act_ret) {
	int oi = 0x00;
	if (data[0] != 0x50) {
		return 0;
	}

	oi = data[1] * 256 + data[2];
	asyslog(LOG_WARNING, "时段功控-控制投入[%04x]", oi);

	ProgramInfo *shareAddr = getShareAddr();
	int sindex = oi - 0x2301;

	CLASS_8103 c8103;
	readCoverClass(0x8103, 0, (void *) &c8103, sizeof(CLASS_8103),
			para_vari_save);

	c8103.enable[sindex].state = 0x01;
	shareAddr->ctrls.c8103.enable[sindex].state = 0x01;
	saveCoverClass(0x8103, 0, (void *) &c8103, sizeof(CLASS_8103),
			para_vari_save);

	return 0;
}

int class8103_act7(int index, int attr_act, INT8U *data, Action_result *act_ret) {
	int oi = 0x00;
	if (data[0] != 0x50) {
		return 0;
	}

	oi = data[1] * 256 + data[2];
	asyslog(LOG_WARNING, "时段功控-控制解除[%04x]", oi);

	ProgramInfo *shareAddr = getShareAddr();
	int sindex = oi - 0x2301;

	CLASS_8103 c8103;
	readCoverClass(0x8103, 0, (void *) &c8103, sizeof(CLASS_8103),
			para_vari_save);
	c8103.enable[sindex].state = 0x00;
	shareAddr->ctrls.c8103.enable[sindex].state = 0x00;

	saveCoverClass(0x8103, 0, (void *) &c8103, sizeof(CLASS_8103),
			para_vari_save);
	return 0;
}

int class8103_act127(int index, int attr_act, INT8U *data,
		Action_result *act_ret) {
	int oi = 0x00;
	INT8U sign = 0x00;
	INT8U numb = 0x00;

	if (data[0] != 0x02 || data[1] != 0x02) {
		return 0;
	}

	oi = data[3] * 256 + data[4];

	if (data[5] != 0x02 || data[6] != 0x02) {
		return 0;
	}

	sign = data[9];
	numb = data[11];

	asyslog(LOG_WARNING, "控制方案切换[8103-127],%04x-%02d-%02d", oi, sign, numb);

	CLASS_8103 c8103;
		readCoverClass(0x8103, 0, (void *) &c8103, sizeof(CLASS_8103),
				para_vari_save);
	c8103.sign = sign;
	c8103.numb = numb;
	saveCoverClass(0x8103, 0, (void *) &c8103, sizeof(CLASS_8103),
				para_vari_save);

	return 0;
}

int class8103_act_route(int index, int attr_act, INT8U *data,
		Action_result *act_ret) {
	switch (attr_act) {
	case 3:
		class8103_act3(1, attr_act, data, act_ret);
		break;
	case 6:
		class8103_act6(1, attr_act, data, act_ret);
		break;
	case 7:
		class8103_act7(1, attr_act, data, act_ret);
		break;
	case 127:
		class8103_act127(1, attr_act, data, act_ret);
		break;
	}
}

int class8104_act3(int index, int attr_act, INT8U *data, Action_result *act_ret) {
	asyslog(LOG_WARNING, "厂休功控-添加控制单元");
	CLASS_8104 c8104;

	if (data[0] != 0x02 || data[1] != 0x05) {
		return 0;
	}

	if (data[2] != 0x80 || data[5] != 0x14 || data[14] != 0x1C
			|| data[22] != 0x12) {
		return 0;
	}
	readCoverClass(0x8104, 0, (void *) &c8104, sizeof(CLASS_8104),
			para_vari_save);
	for (int i = 0; i < MAX_AL_UNIT; i++) {
		if (c8104.list[i].index == 0x00) {
			c8104.list[i].index = data[3] * 256 + data[4];
			INT64U v = 0x00;
			for (int j = 0; j < 8; j++) {
				v += data[6 + j] * powl(256, 7 - j);
			}
			c8104.list[i].v = v;
			c8104.list[i].start.year.data = data[15] * 256 + data[16];
			c8104.list[i].start.month.data = data[17];
			c8104.list[i].start.day.data = data[18];
			c8104.list[i].start.hour.data = data[19];
			c8104.list[i].start.min.data = data[20];

			c8104.list[i].sustain = data[23] * 256 + data[24];
			c8104.list[i].noDay = data[27];
		}
	}

	return 0;
}

int class8104_act6(int index, int attr_act, INT8U *data, Action_result *act_ret) {
	int oi = 0x00;
	if (data[0] != 0x50) {
		return 0;
	}

	oi = data[1] * 256 + data[2];
	asyslog(LOG_WARNING, "厂休功控-控制投入[%04x]", oi);

	ProgramInfo *shareAddr = getShareAddr();
	int sindex = oi - 0x2301;

	CLASS_8104 c8104;
	readCoverClass(0x8104, 0, (void *) &c8104, sizeof(CLASS_8104),
			para_vari_save);
	c8104.enable[sindex].state = 0x01;
	shareAddr->ctrls.c8104.enable[sindex].state = 0x01;

	saveCoverClass(0x8104, 0, (void *) &c8104, sizeof(CLASS_8104),
			para_vari_save);
	return 0;
}

int class8104_act7(int index, int attr_act, INT8U *data, Action_result *act_ret) {
	int oi = 0x00;
	if (data[0] != 0x50) {
		return 0;
	}

	oi = data[1] * 256 + data[2];
	asyslog(LOG_WARNING, "厂休功控-控制解除[%04x]", oi);

	ProgramInfo *shareAddr = getShareAddr();
	int sindex = oi - 0x2301;

	CLASS_8104 c8104;
	readCoverClass(0x8104, 0, (void *) &c8104, sizeof(CLASS_8104),
			para_vari_save);
	c8104.enable[sindex].state = 0x00;
	shareAddr->ctrls.c8104.enable[sindex].state = 0x00;
	saveCoverClass(0x8104, 0, (void *) &c8104, sizeof(CLASS_8104),
			para_vari_save);
	return 0;
}

int class8104_act_route(int index, int attr_act, INT8U *data,
		Action_result *act_ret) {
	switch (attr_act) {
	case 3:
		class8104_act3(1, attr_act, data, act_ret);
		break;
	case 6:
		class8104_act6(1, attr_act, data, act_ret);
		break;
	case 7:
		class8104_act7(1, attr_act, data, act_ret);
		break;
	}
}

int class8105_act3(int index, int attr_act, INT8U *data, Action_result *act_ret) {
	OI_698 oi = 0x00;
	int unit = 0;
	int	ii = 0;
	INT8U  DAR = 0;
	CLASS_8105 c8105={};

	ii += getStructure(&data[ii],NULL,&DAR);
	ii += getOI(1,&data[ii],&oi);
	asyslog(LOG_WARNING, "营业报停-添加控制单元(OI=%04x)",oi);

	if(oi>=0x2301 && oi<=0x2308) {
		unit = oi-0x2301;
	}else {
		act_ret->DAR = obj_unexist;
		return 0;
	}
	readCoverClass(0x8105, 0, (void *) &c8105, sizeof(CLASS_8105),
			para_vari_save);

	ii += getDateTimeS(1,&data[ii],(INT8U *)&c8105.list[unit].start,&DAR);
	ii += getDateTimeS(1,&data[ii],(INT8U *)&c8105.list[unit].end,&DAR);
	ii += getLong64(&data[ii],&c8105.list[unit].v);
	fprintf(stderr,"c8105.v = %lld\n",c8105.list[unit].v);
	printDataTimeS("报停起始时间",c8105.list[unit].start);
	printDataTimeS("报停结束时间",c8105.list[unit].end);
	saveCoverClass(0x8105, 0, (void *) &c8105, sizeof(CLASS_8105),
			para_vari_save);
	return 0;
}

int class8105_act6(int index, int attr_act, INT8U *data, Action_result *act_ret) {
	int oi = 0x00;
	if (data[0] != 0x50) {
		return 0;
	}

	oi = data[1] * 256 + data[2];
	asyslog(LOG_WARNING, "营业报停-控制投入[%04x]", oi);

	ProgramInfo *shareAddr = getShareAddr();
	int sindex = oi - 0x2301;

	CLASS_8105 c8105;
	readCoverClass(0x8105, 0, (void *) &c8105, sizeof(CLASS_8105),
			para_vari_save);
	c8105.enable[sindex].state = 0x01;
	shareAddr->ctrls.c8105.enable[sindex].state = 0x01;
	saveCoverClass(0x8105, 0, (void *) &c8105, sizeof(CLASS_8105),
			para_vari_save);
	return 0;
}

int class8105_act7(int index, int attr_act, INT8U *data, Action_result *act_ret) {
	int oi = 0x00;
	if (data[0] != 0x50) {
		return 0;
	}

	oi = data[1] * 256 + data[2];
	asyslog(LOG_WARNING, "营业报停-控制解除[%04x]", oi);


	ProgramInfo *shareAddr = getShareAddr();
	int sindex = oi - 0x2301;

	CLASS_8105 c8105;
	readCoverClass(0x8105, 0, (void *) &c8105, sizeof(CLASS_8105),
			para_vari_save);
	c8105.enable[sindex].state = 0x00;
	shareAddr->ctrls.c8105.enable[sindex].state = 0x00;
	saveCoverClass(0x8105, 0, (void *) &c8105, sizeof(CLASS_8105),
			para_vari_save);
	return 0;
}

int class8105_act_route(int index, int attr_act, INT8U *data,
		Action_result *act_ret) {
	switch (attr_act) {
	case 3:
		class8105_act3(1, attr_act, data, act_ret);
		break;
	case 6:
		class8105_act6(1, attr_act, data, act_ret);
		break;
	case 7:
		class8105_act7(1, attr_act, data, act_ret);
		break;
	}
}

int class8106_act3(int index, int attr_act, INT8U *data, Action_result *act_ret) {
//	OI_698 oi = 0x00;
//	int unit = 0;
//	int	ii = 0;
//	INT8U  DAR = 0;
//	CLASS_8106 c8106={};
//
//	ii += getStructure(&data[ii],NULL,&DAR);
//	ii += getOI(1,&data[ii],&oi);
//	asyslog(LOG_WARNING, "营业报停-添加控制单元(OI=%04x)",oi);
//
//	if(oi>=0x2301 && oi<=0x2308) {
//		unit = oi-0x2301;
//	}else {
//		act_ret->DAR = obj_unexist;
//		return 0;
//	}
//	readCoverClass(0x8106, 0, (void *) &c8106, sizeof(CLASS_8106),
//			para_vari_save);
//
//	saveCoverClass(0x8106, 0, (void *) &c8106, sizeof(CLASS_8106),
//			para_vari_save);
//

	return 0;
}

int class8106_act6(int index, int attr_act, INT8U *data, Action_result *act_ret) {
	int oi = 0x00;
	if (data[0] != 0x50) {
		return 0;
	}


	oi = data[1] * 256 + data[2];
	asyslog(LOG_WARNING, "功率下浮-控制投入[%04x]", oi);

	ProgramInfo *shareAddr = getShareAddr();
	int sindex = oi - 0x2301;

	CLASS_8106 c8106;
	readCoverClass(0x8106, 0, (void *) &c8106, sizeof(CLASS_8106),
			para_vari_save);

	c8106.enable[sindex].state = 0x01;
	shareAddr->ctrls.c8106.enable[sindex].state = 0x01;
	saveCoverClass(0x8106, 0, (void *) &c8106, sizeof(CLASS_8106),
			para_vari_save);

	return 0;
}

int class8106_act7(int index, int attr_act, INT8U *data, Action_result *act_ret) {
	int oi = 0x00;
	if (data[0] != 0x50) {
		return 0;
	}

	ProgramInfo *shareAddr = getShareAddr();
	oi = data[1] * 256 + data[2];
	int sindex = oi - 0x2301;
	asyslog(LOG_WARNING, "功率下浮-控制解除[%04x]", oi);
	CLASS_8106 c8106;
	readCoverClass(0x8106, 0, (void *) &c8106, sizeof(CLASS_8106),
			para_vari_save);

	c8106.enable[sindex].state = 0x00;
	shareAddr->ctrls.c8106.enable[sindex].state = 0x00;
	saveCoverClass(0x8106, 0, (void *) &c8106, sizeof(CLASS_8106),
			para_vari_save);
	return 0;
}

int class8106_act_route(int index, int attr_act, INT8U *data,
		Action_result *act_ret) {
	switch (attr_act) {
	case 3:
		class8106_act3(1, attr_act, data, act_ret);
		break;
	case 6:
		class8106_act6(1, attr_act, data, act_ret);
		break;
	case 7:
		class8106_act7(1, attr_act, data, act_ret);
		break;
	}
}

int class8107_act3(int index, int attr_act, INT8U *data, Action_result *act_ret) {
	int oi = 0x00;
	int id = 0x00;
	INT8U sign = 0x00;
	INT8U type = 0x00;
	INT64U val = 0x00;
	INT64U war_thr = 0x00;
	INT64U ctl_thr = 0x00;
	INT8U mode = 0x00;

	ProgramInfo *shareAddr = getShareAddr();

	if (data[0] != 0x02 || data[1] != 0x08) {
		return 0;
	}

	oi = data[3] * 256 + data[4];
	int sindex = oi - 0x2301;
	id = data[6] * 256 * 256 * 256 + data[7] * 256 * 256 + data[8] * 256
			+ data[9];
	shareAddr->ctrls.c8107.list[sindex].no = id;

	sign = data[11];
	shareAddr->ctrls.c8107.list[sindex].add_refresh = sign;

	type = data[13];
	shareAddr->ctrls.c8107.list[sindex].type = type;

	val = getLongValue(&data[14]);
	shareAddr->ctrls.c8107.list[sindex].v = val/100;
	war_thr = getLongValue(&data[23]);
	shareAddr->ctrls.c8107.list[sindex].alarm = war_thr/100;
	ctl_thr = getLongValue(&data[32]);
	shareAddr->ctrls.c8107.list[sindex].ctrl = ctl_thr/100;
	mode = data[42];
	shareAddr->ctrls.c8107.list[sindex].mode = mode;

	shareAddr->class23[sindex].remains += shareAddr->ctrls.c8107.list[sindex].v;

	asyslog(LOG_WARNING, "购电-添加控制单元[%04x-%d-%d-%d-%lld-%lld-%lld-%d]", oi, id,
			sign, type, val, war_thr, ctl_thr, mode);
	Event_3202(NULL,0, getShareAddr());
	return 0;
}


int class8107_act5(int index, int attr_act, INT8U *data, Action_result *act_ret) {
	int oi = 0x00;
	int id = 0x00;
	INT8U sign = 0x00;
	INT8U type = 0x00;
	INT64U val = 0x00;
	INT64U war_thr = 0x00;
	INT64U ctl_thr = 0x00;
	INT8U mode = 0x00;

	ProgramInfo *shareAddr = getShareAddr();

	if (data[0] != 0x02 || data[1] != 0x08) {
		return 0;
	}

	oi = data[3] * 256 + data[4];
	int sindex = oi - 0x2301;
	id = data[6] * 256 * 256 * 256 + data[7] * 256 * 256 + data[8] * 256
			+ data[9];
	shareAddr->ctrls.c8107.list[sindex].no = id;

	sign = data[11];
	shareAddr->ctrls.c8107.list[sindex].add_refresh = sign;

	type = data[13];
	shareAddr->ctrls.c8107.list[sindex].type = type;

	val = getLongValue(&data[14]);
	shareAddr->ctrls.c8107.list[sindex].v = val;
	war_thr = getLongValue(&data[23]);
	shareAddr->ctrls.c8107.list[sindex].alarm = war_thr;
	ctl_thr = getLongValue(&data[32]);
	shareAddr->ctrls.c8107.list[sindex].ctrl = ctl_thr;
	mode = data[42];
	shareAddr->ctrls.c8107.list[sindex].mode = mode;
	asyslog(LOG_WARNING, "购电-更新控制单元[%04x-%d-%d-%d-%lld-%lld-%lld-%d]", oi, id,
			sign, type, val, war_thr, ctl_thr, mode);

	Event_3202(NULL,0, getShareAddr());
	return 0;
}

int class8107_act6(int index, int attr_act, INT8U *data, Action_result *act_ret) {
	int oi = 0x00;
	if (data[0] != 0x50) {
		return 0;
	}

	ProgramInfo *shareAddr = getShareAddr();

	oi = data[1] * 256 + data[2];
	int sindex = oi - 0x2301;
	asyslog(LOG_WARNING, "购电-控制投入[%04x]", oi);

	CLASS_8107 c8107;
	readCoverClass(0x8107, 0, (void *) &c8107, sizeof(CLASS_8107),
			para_vari_save);

	c8107.enable[sindex].state = 0x01;
	shareAddr->ctrls.c8107.enable[sindex].state = 0x01;

	saveCoverClass(0x8107, 0, (void *) &c8107, sizeof(CLASS_8107),
			para_vari_save);
	return 0;
}

int class8107_act7(int index, int attr_act, INT8U *data, Action_result *act_ret) {
	int oi = 0x00;
	if (data[0] != 0x50) {
		return 0;
	}

	ProgramInfo *shareAddr = getShareAddr();

	oi = data[1] * 256 + data[2];
	int sindex = oi - 0x2301;
	asyslog(LOG_WARNING, "购电-控制解除[%04x]", oi);

	CLASS_8107 c8107;
	readCoverClass(0x8107, 0, (void *) &c8107, sizeof(CLASS_8107),
			para_vari_save);


	c8107.enable[sindex].state = 0x00;
	shareAddr->ctrls.c8107.enable[sindex].state = 0x00;
	shareAddr->class23[sindex].alCtlState.OutputState = 0x00;
	shareAddr->class23[sindex].alCtlState.BuyOutputState = 0x00;
	shareAddr->class23[sindex].alCtlState.ECAlarmState = 0x00;

	saveCoverClass(0x8107, 0, (void *) &c8107, sizeof(CLASS_8107),
			para_vari_save);
	return 0;
}

int class8107_act_route(int index, int attr_act, INT8U *data,
		Action_result *act_ret) {
	switch (attr_act) {
	case 3:
		class8107_act3(1, attr_act, data, act_ret);
		break;
	case 5:
		class8107_act5(1, attr_act, data, act_ret);
		break;
	case 6:
		class8107_act6(1, attr_act, data, act_ret);
		break;
	case 7:
		class8107_act7(1, attr_act, data, act_ret);
		break;
	}
}

int class8108_act3(int index, int attr_act, INT8U *data, Action_result *act_ret) {
	int oi = 0x00;
	INT64U v = 0x00;
	INT8U th = 0x00;
	INT8U fl = 0x00;

	ProgramInfo *shareAddr = getShareAddr();

	if (data[0] != 0x02 || data[1] != 0x04) {
		return 0;
	}

	oi = data[3] * 256 + data[4];
	index = oi - 0x2301;
	shareAddr->ctrls.c8108.list[index].v = getLongValue(&data[5]);
	v = getLongValue(&data[5]);
	shareAddr->ctrls.c8108.list[index].para = data[15];
	th = data[15];
	shareAddr->ctrls.c8108.list[index].flex = data[17];
	fl = data[17];

	asyslog(LOG_WARNING, "月电-添加控制单元[%04x-%lld-%d-%d]", oi, v, th, fl);
	return 0;
}

int class8108_act6(int index, int attr_act, INT8U *data, Action_result *act_ret) {
	int oi = 0x00;
	if (data[0] != 0x50) {
		return 0;
	}

	ProgramInfo *shareAddr = getShareAddr();

	oi = data[1] * 256 + data[2];
	int sindex = oi - 0x2301;
	asyslog(LOG_WARNING, "月电-控制投入[%04x]", oi);

	CLASS_8108 c8108;
	readCoverClass(0x8108, 0, (void *) &c8108, sizeof(CLASS_8108),
			para_vari_save);
	shareAddr->ctrls.c8108.enable[sindex].state = 0x01;
	c8108.enable[sindex].state = 0x01;
	saveCoverClass(0x8108, 0, (void *) &c8108, sizeof(CLASS_8108),
			para_vari_save);

	return 0;
}

int class8108_act7(int index, int attr_act, INT8U *data, Action_result *act_ret) {
	int oi = 0x00;
	if (data[0] != 0x50) {
		return 0;
	}

	ProgramInfo *shareAddr = getShareAddr();

	oi = data[1] * 256 + data[2];
	int sindex = oi - 0x2301;
	asyslog(LOG_WARNING, "月电-控制解除[%04x]", oi);

	CLASS_8108 c8108;
	readCoverClass(0x8108, 0, (void *) &c8108, sizeof(CLASS_8108),
			para_vari_save);

	c8108.enable[sindex].state = 0x00;
	shareAddr->ctrls.c8108.enable[sindex].state = 0x00;
	shareAddr->class23[sindex].alCtlState.OutputState = 0x00;
	shareAddr->class23[sindex].alCtlState.MonthOutputState = 0x00;
	shareAddr->class23[sindex].alCtlState.ECAlarmState = 0x00;

	saveCoverClass(0x8108, 0, (void *) &c8108, sizeof(CLASS_8108),
			para_vari_save);

	return 0;
}

int class8108_act_route(int index, int attr_act, INT8U *data,
		Action_result *act_ret) {
	switch (attr_act) {
	case 3:
		class8108_act3(1, attr_act, data, act_ret);
		break;
	case 6:
		class8108_act6(1, attr_act, data, act_ret);
		break;
	case 7:
		class8108_act7(1, attr_act, data, act_ret);
		break;
	}
}
