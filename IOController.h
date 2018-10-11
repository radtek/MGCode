#pragma once
class CIOController
{
public:
	CIOController(void);
	~CIOController(void);

	void SetOnline(BOOL bOnline);

	BOOL INPUT_MARK();				//启动按钮信号
	BOOL INPUT_RESET_ERROR();		//复位按钮
	BOOL INPUT_SAFEDOOR();			//安全门
	BOOL INPUT_GOLOADPT();			//去上料点
	BOOL INPUT_GOUNLOADPT();			//去下料点
	BOOL INPUT_WORKBENCH_DISABLE();	//平台丢使能
	BOOL INPUT_MACHINE_READY();		//激光就绪

	void OUTPUT_LIGHT_RED(BOOL b);			//三色灯（红）信号
	void OUTPUT_LIGHT_YELLOW(BOOL b);		//三色灯（黄）信号
	void OUTPUT_LIGHT_GREEN(BOOL b);			//三色灯（绿）信号
	void OUTPUT_LIGHT_HUMMER(BOOL b);		//三色灯（蜂鸣）信号
	void OUTPUT_AIR_BLOW(BOOL b);			//打标吹气信号
	void OUTPUT_SMOKE_FILTER(BOOL b);		//烟尘过滤器启动信号
	void OUTPUT_LOADPT(BOOL b);				//在上料位
	void OUTPUT_UNLOADPT(BOOL b);			//在下料位
	void OUTPUT_MARKEND(BOOL b);				//打标结束
	void OUTPUT_COD_BARCODE_ERROR(BOOL b);	//CCD扫码失败
	void OUTPUT_CCD_LOCATE_ERROR(BOOL b);	//CCD定位失败
	void OUTPUT_SOFT_OK(BOOL b);			//软件就绪
	void OUTPUT_CLOSE_DOOR(BOOL b);			//关闭进出料口

private:
	BOOL m_bOnLine;		//在线信号（有PLC）
	BOOL m_bOffLine;	//离线信号(无PLC)
};

extern CIOController g_IO;