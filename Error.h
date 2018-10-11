#ifndef ERROR_HEAD
#define ERROR_HEAD

//错误代码定义
#define FUN_OK 0
#define STRING_FUN_OK "Funtion Execute Ok"
#define BUF_LEN_ERR 1 //取错误代码时缓冲长度不够
#define STRING_BUF_LEN_ERR "Char Buffer length is not enough"

//打标流程错误代码
#define MARK_STOP 10 //上位软件已经中止打标
#define STRING_MARK_STOP "Mark Task is stoped by user"
#define MARK_FIFO_BREAK 12	//断流
#define STRING_MARK_FIFO_BREAK "data stream to CARD is broken off"
#define MARK_FIFO_FULL  13	//FIFO满
#define STRING_MARK_FIFO_FULL "FIFO is full"
#define LIST_DELAY_TIME_ERR 14 //LIST指令中延时数据错误
#define STRING_LIST_DELAY_TIME_ERR "List Dealy Time Error"
#define ALREADY_CMD_PAUSE 15 //已经处于立即指令暂停状态
#define STRING_ALREADY_CMD_PAUSE "Already Command Pause"
#define ALREADY_FIFO_PAUSE 16 //已经处于断点暂停状态
#define STRING_ALREADY_FIFO_PAUSE "Already FIFO PAUSE"


#define MARK_TASK_NO_START 20 //打标任务还未开始
#define STRING_MARK_TASK_NO_START "Marking Task no Start"
#define MARK_NO_STRAT		21 //ARM7并未开始执行FIFO打标数据
#define STRING_MARK_NO_STRAT "CARD does'nt Start to Execute Marking Task"
#define UNSUPPERED_STROKE_TYPE 22 //不支持的线条类型
#define STRING_UNSUPPERED_STROKE_TYPE "UnSuppered stroke type"
#define MARK_DOT_NUM_ERR	23 //打点个数错误
#define STRING_MARK_DOT_NUM_ERR "Marking Dot Num. Error"
#define MARK_Vector_NUM_ERR 24 //线条数目错误
#define STRING_MARK_Vector_NUM_ERR "Marking vector Num. Error"
#define MARK_BMP_NUM_ERR	25 //打BMP像素点个数错误
#define STRING_MARK_BMP_NUM_ERR "Marking BITMAP pixel Num. Error"
#define LAYER_NUM_OVER		26 //层参数数目超过最大值
#define STRING_LAYER_NUM_OVER "Layer Num. is Over Allowed 64 layers"
//卡功能错误代码
#define SOFT_ADJ_CURRENT_ERR 30 //设备软件调电流测试失败
#define STRING_SOFT_ADJ_CURRENT_ERR "Soft Adjust Current Test Error,Please Check Hardware!"
#define SOFT_ADJ_CURRENT_DISABLE 31 //配置文件中未启用软件调电流
#define STRING_SOFT_ADJ_CURRENT_DISABLE "Soft Adjust is disable by cofiguration file"
#define SAME_CURRENT 32			//与卡当前电流值相同
#define STRING_SAME_CURRENT	"Same Current"

//卡初始化错误代码
#define INI_ERR_CARD_NOFIND  40 //初始化错误，卡未找到
#define STRING_INI_ERR_CARD_NOFIND "Can't Find PCI3000 Maring Card, Please Check connetion!"
#define INI_ERR_CANNOT_RESET 41 //不能成功复位
#define STRING_INI_ERR_CANNOT_RESET "Can't Reset PCI3000 Card,Please Check CARD!"
#define INI_ERR_CARD_VER_ERR 42 //PCI3000卡版本号与本程序要求的版本号不同
#define STRING_INI_ERR_CARD_VER_ERR "Card Ver Error,Please Download the suitable Verion Card program(*.Hex)"
#define INI_ERR_CARD_TYPE	 43 //非固体打标卡
#define STRING_INI_ERR_CARD_TYPE "CARD program Error!"
#define UNOPEN_CARD 44		//卡还未打开
#define STRING_UNOPEN_CARD "Please First Open the Card before this operation"
#define CARDINDEX_OVER 45 //卡索引超界
#define STRING_CARDINDEX_OVER "CARD Index is over"
#define CARD_OPENED	46	//卡已经打开
#define STRING_CARD_OPENED "CARD has opened"
#define OPEN_CARD_FAILED 47//卡打开失败，因为内存分配不够
#define STRING_OPEN_CARD_FAILED "Open Card Failed because of not enough Memory"
//卡参数设置错误代码
#define CARDPARA_FILE_CANNOT_OPEN 50 //卡参数设置文件不能打开
#define STRING_CARDPARA_FILE_CANNOT_OPEN "Can't Open the CARD configuration File,please check the path!"

//校正相关错误代码
#define CALI_FILE_CANNOT_OPEN 60	//校正文件不能打开
#define STRING_CALI_FILE_CANNOT_OPEN "Can't open the Calibration FILE,Please CHECK the file is exist"
#define UNSUPPERED_CALI_TYPE  61	//不支持的校正类型
#define STRING_UNSUPPERED_CALI_TYPE "UnSupported calibration type"

//函数接口错误代码
#define PARA_LENGTH_NOENOUGH	70 //字符串参数长度不够
#define STRING_PARA_LENGTH_NOENOUGH "Parameter Length is not enough"
#define PARA_POLYGON_NUM_ERR	71 ////拐弯表数目不对
#define STRING_PARA_POLYGON_NUM_ERR "Polygon list Num. Error"
#define PARA_CURRENT_GRAY_NUM_ERR 72 //灰度电流数不对
#define STRING_PARA_CURRENT_GRAY_NUM_ERR "Gray Current List Num. Error"

//机型功能支持代码
#define UNSUPPORT_BMP_MARK		80 //此机型不支持打灰度位图
#define STRING_UNSUPPORT_BMP_MARK "Can't support Gray BITMAP  Marking"

#endif