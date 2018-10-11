#pragma once
class CIOController
{
public:
	CIOController(void);
	~CIOController(void);

	void SetOnline(BOOL bOnline);

	BOOL INPUT_MARK();				//������ť�ź�
	BOOL INPUT_RESET_ERROR();		//��λ��ť
	BOOL INPUT_SAFEDOOR();			//��ȫ��
	BOOL INPUT_GOLOADPT();			//ȥ���ϵ�
	BOOL INPUT_GOUNLOADPT();			//ȥ���ϵ�
	BOOL INPUT_WORKBENCH_DISABLE();	//ƽ̨��ʹ��
	BOOL INPUT_MACHINE_READY();		//�������

	void OUTPUT_LIGHT_RED(BOOL b);			//��ɫ�ƣ��죩�ź�
	void OUTPUT_LIGHT_YELLOW(BOOL b);		//��ɫ�ƣ��ƣ��ź�
	void OUTPUT_LIGHT_GREEN(BOOL b);			//��ɫ�ƣ��̣��ź�
	void OUTPUT_LIGHT_HUMMER(BOOL b);		//��ɫ�ƣ��������ź�
	void OUTPUT_AIR_BLOW(BOOL b);			//��괵���ź�
	void OUTPUT_SMOKE_FILTER(BOOL b);		//�̳������������ź�
	void OUTPUT_LOADPT(BOOL b);				//������λ
	void OUTPUT_UNLOADPT(BOOL b);			//������λ
	void OUTPUT_MARKEND(BOOL b);				//������
	void OUTPUT_COD_BARCODE_ERROR(BOOL b);	//CCDɨ��ʧ��
	void OUTPUT_CCD_LOCATE_ERROR(BOOL b);	//CCD��λʧ��
	void OUTPUT_SOFT_OK(BOOL b);			//�������
	void OUTPUT_CLOSE_DOOR(BOOL b);			//�رս����Ͽ�

private:
	BOOL m_bOnLine;		//�����źţ���PLC��
	BOOL m_bOffLine;	//�����ź�(��PLC)
};

extern CIOController g_IO;