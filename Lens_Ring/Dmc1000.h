#ifndef _LEISAI_
#define _LEISAI_

//#ifdef __cplusplus
extern "C" {
//#endif

//��һ������Ķ�  //This segment should not be modified
#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;

typedef unsigned char  INT8U;                   /* defined for unsigned 8-bits integer variable 	�޷���8λ���ͱ���  */
typedef signed   char  INT8S;                   /* defined for signed 8-bits integer variable		�з���8λ���ͱ���  */
typedef unsigned short INT16U;                  /* defined for unsigned 16-bits integer variable 	�޷���16λ���ͱ��� */
typedef signed   short INT16S;                  /* defined for signed 16-bits integer variable 		�з���16λ���ͱ��� */
typedef unsigned int   INT32U;                  /* defined for unsigned 32-bits integer variable 	�޷���32λ���ͱ��� */
typedef int			   INT32S;                  /* defined for signed 32-bits integer variable 		�з���32λ���ͱ��� */
typedef float		   FP32;                    /* single precision floating point variable (32bits) �����ȸ�������32λ���ȣ� */
typedef double		   FP64;                    /* double precision floating point variable (64bits) ˫���ȸ�������64λ���ȣ� */


#define DMC1000_API __declspec(dllexport)


#define WINAPI __stdcall





///////////////////////////////////////////////////////////
/////////////////////DMC1380 V1.2�����б�//////////////////
///////////////////////////////////////////////////////////

//////////////////��ʼ������////////////////////
//---------------------   �忨��ʼ�����ú���  ----------------------
/********************************************************************************
** ��������: d1000_board_init
** ��������: ���ư��ʼ�������ó�ʼ�����ٶȵ�����
** �䡡  ��: ��
** �� �� ֵ: 0���޿��� 1-8���ɹ�(ʵ�ʿ���) 
** ��    ��:  
*********************************************************************************/
DMC1000_API DWORD WINAPI d1000_board_init(void);

DMC1000_API DWORD WINAPI d1000_board_close(void);
//DMC1000_API DWORD WINAPI d1000_get_base_addr(short CardNo);
//DMC1000_API DWORD WINAPI d1000_get_irq_channel(short CardNo);
//////////////////����������ú���//////////////
DMC1000_API DWORD WINAPI d1000_set_pls_outmode(short axis,short pls_outmode);

//////////////////�ٶ�ģʽ�˶�����//////////////
DMC1000_API DWORD WINAPI d1000_start_tv_move(short axis,long StrVel,long MaxVel,double Tacc);

DMC1000_API DWORD WINAPI d1000_get_speed(short axis);
//DMC1000_API DWORD WINAPI d1000_change_speed(short axis,long NewVel,double Tadc);
DMC1000_API DWORD WINAPI d1000_change_speed(short axis,long NewVel);
DMC1000_API DWORD WINAPI d1000_decel_stop(short axis);
DMC1000_API DWORD WINAPI d1000_immediate_stop(short axis);

//////////////////����λ��ģʽ����//////////////
DMC1000_API DWORD WINAPI d1000_start_t_move(short axis,long Dist, long StrVel,long MaxVel,double Tacc);
DMC1000_API DWORD WINAPI d1000_start_ta_move(short axis,long Pos, double StrVel,double MaxVel,double Tacc);

DMC1000_API DWORD WINAPI d1000_start_s_move(short axis,long Dist, long StrVel,long MaxVel,double Tacc);
DMC1000_API DWORD WINAPI d1000_start_sa_move(short axis,long Pos, long StrVel,long MaxVel,double Tacc);

DMC1000_API DWORD WINAPI d1000_start_sv_move(short axis,long StrVel,long MaxVel,double Tacc);

DMC1000_API DWORD WINAPI d1000_set_s_profile(short axis,double s_para);
DMC1000_API DWORD WINAPI d1000_get_s_profile(short axis,double* s_para);

//////////////////���Բ岹����//////////////////
DMC1000_API DWORD WINAPI d1000_start_t_line(short TotalAxis,short *AxisArray,long *DistArray,long StrVel, long MaxVel, double Tacc);
DMC1000_API DWORD WINAPI d1000_start_ta_line(short TotalAxis,short *AxisArray,long *DistArray,long StrVel, long MaxVel, double Tacc);

//////////////////��ԭ�㺯��////////////////////
DMC1000_API DWORD WINAPI d1000_home_move(short axis,long StrVel,long MaxVel,double Tacc);

//////////////////�˶�״̬��⺯��//////////////
DMC1000_API DWORD WINAPI d1000_check_done(short axis);

//////////////////λ���趨�Ͷ�ȡ����////////////
DMC1000_API long WINAPI d1000_get_command_pos(short axis);
DMC1000_API DWORD WINAPI d1000_set_command_pos(short axis,double Pos);

//////////////////ͨ��I/O����///////////////////
DMC1000_API DWORD WINAPI d1000_out_bit(short BitNo,short BitData);
DMC1000_API DWORD WINAPI d1000_in_bit(short BitNo);
DMC1000_API DWORD WINAPI d1000_get_outbit(short BitNo);
DMC1000_API void WINAPI d1000_in_enable(DWORD CardNo,DWORD InputEn);


//////////////////ר��I/O�ӿں���///////////////
DMC1000_API DWORD WINAPI d1000_set_sd(short axis ,short SdMode);
DMC1000_API DWORD WINAPI d1000_get_axis_status(short axis);

DMC1000_API void  WINAPI d1000_WriteDWord(DWORD addr, DWORD data);
DMC1000_API DWORD WINAPI d1000_ReadDWord(DWORD addr);


//#ifdef __cplusplus
}
//#endif

#endif 
