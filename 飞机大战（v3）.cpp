#include <graphics.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define wide 1000
#define high 700								//���򴰿ڴ�С��͸�
#define sizeofplane 70							//�ɻ���С
#define ammo 50									//�������ӵ��� 
#define numofenm 20 							//���л���
#define ammo_enm 10								//�����ӵ��� 
#define xdistofPE abs(ximgofplane-enemy[i].x)	//�л����ҷ��ɻ���x�����
#define ydistofPE abs(yimgofplane-enemy[i].y)	//�л����ҷ��ɻ���y����� 

//ͼƬ��ȫ�ֱ��� 
PIMAGE beginimg; 			//��ʼ���� 			�� 
PIMAGE img_gamestart;		//��ʼ��ϷͼƬ 		�� 
PIMAGE img_gameover;		//�˳���ϷͼƬ		�� 
PIMAGE gameback;			//����ͼƬ			��
PIMAGE gameback1;			//������ͼƬ��Ϊ1 	�� 
PIMAGE gameback2; 			//������ͼƬ��Ϊ2 	��
PIMAGE wd_back;				//���ֱ���ͼ		�� 
PIMAGE planetextimg;		//��ʼ��������		��
PIMAGE imgofplane;			//�ɻ�ͼƬ 			�� 
PIMAGE bullet1; 			//�ӵ�ͼƬ1			�� 
PIMAGE bullet3;				//�ӵ�ͼƬ3			��
PIMAGE bullet_enm;			//�л��ӵ�			�� 
PIMAGE enemy1;      		//�л�ͼƬ			�� 
PIMAGE Gameover;			//��Ϸ��������		��  
PIMAGE boom[9];				//��ըͼƬ����9��	�� 
PIMAGE load[11]; 			//����ͼƬ����11��	��
PIMAGE boom_self[10];		//�����ɻ���ըͼƬ  ��
PIMAGE img_update;			//����				��

//�û�����ȫ�ֱ��� 
bool isClick;				//����Ƿ��� 		
int xClick, yClick;			//����������� 	
int mouse_x,mouse_y;		//����ʵʱ���� 	
int isclosed = 0;			//�Ƿ�رմ��� 
int isback = 0;				//�Ƿ񷵻����˵�
int isrunning = 1;			//�Ƿ��ڽ�����Ϸ		

//���ֽṹ�� 
MUSIC bgm_fight; 	//��������
MUSIC bgm_wait;		//�ȴ�ʱ���� 
MUSIC sound_hit; 	//�ӵ���������
MUSIC crashed;		//׹��ʱ������ 
MUSIC sound_click;	//���ʱ������ 


//����ͼƬy������ 
float yofback1 = -high; 
float yofback2 = 0;
double spdofback = 0.5; //�����ƶ��ٶ� 


//�ӵ����ɻ����л���λ�ñ������ٶȱ��� 
//���ڷɻ�λ�ü��ƶ��ı��� 
float ximgofplane = wide*0.5 - sizeofplane/2 + 100 ,  yimgofplane = high - sizeofplane;	//�����ͣ���ȷС�ɻ�λ��,�����ʼ��ΪС�ɻ�������  
float speed = 10.0f;	//�ƶ��ٶȣ�����������������ٶȿ���
float xspeeds[4] = { -speed, 0, speed, 0 }; 
float yspeeds[4] = { 0, -speed, 0, speed }; 
int keys[4] = {'A', 'W', 'D', 'S' };	//0��1��2��3�ֱ�������ϣ��ң���
int directkeys[4] = {key_left, key_up, key_right, key_down }; 
int blood_x ;		//��ʾ�ɻ�Ѫ����x���� 

//�л����Լ�λ��
struct enm {
	int x;					//x���� 
	int y;					//y���� 
	int s;					//״̬�ж� 1�����������λ�ô���һ�ܵл���0����δ��л��������´�һ��
	int iskilled;			//�л��Ƿ񱻻�ɱ 
	int pboom; 				//�л��ı�ըͼƬ˳�򴢴� 
	int blood;				//�л���Ѫ��
	//�ڵ��˽ṹ�����ж���һ���ṹ��������ʾ���˵��ӵ� 
	struct {
		int blt_enm_x; 			//���˵��ӵ���x���� 
		int blt_enm_y; 			//���˵��ӵ���y����
		int s_blt;				//���˵��ӵ�״̬�� 
	}blt[ammo_enm]; 
 } enemy[numofenm];
double spdofenemy = 1.5;	//�л��ٶ�
double spdofenmblt = 2.0;	//�л��ӵ��ٶ� 
double gapofenemy = 3.5;	//�л����ּ��ʱ�� 
int numofenm_now = 5;		//��ǰ���л��� 

//�ӵ����Լ�λ�� 
struct  bullet_plane {
	int x;		//x���� 
	int y;		//y���� 
	int s;		//״̬�ж� 1�����������λ�ô���һö�ӵ���0����δ���ӵ��������´�һö
	int _up;	//�ж��Ƿ�Ϊ�����ӵ� 
} bullet[ammo];
double spdofblt = 3.0;		//�ӵ��ٶ�
double gapofbullet = 0.5; 	//�ӵ����ֵĳ�ʼ���ʱ��  
int isExploded; 			//�жϷɻ��Ƿ�ը��ֵ 
int score = 0; 				//�÷�
 
//������λ��
int x_up,y_up;
int speed_up = 2;
int dx = speed_up, dy = speed_up;
bool isupexist = false;				//�Ƿ���������� 
int isupdated = 0;					//�Ƿ����� 
double t_up, dur;
bool isrefresh = false;

//���������� 
void setimage();					//����ͼƬ��С�ĺ���
void imgandsound_init();			//��ϷͼƬ��ʼ��
void game_init();					//��Ϸ���ݳ�ʼ�� 
void menu ();						//���˵�
void gameover ();					//��Ϸ����ʱ�Ĵ���
void ifhit();						//����ӵ��Ƿ�����
void isagainst();					//�жϷɻ����л�֮���Ƿ���ײ
void isattacked();					//�Ƿ񱻹��� 
void moving ();						//�ɻ��ƶ�WASD/��������
void bulletget();					//�����ӵ�
void enemy_bullet();				//�л��ӵ� 				
void enemyappear();					//���ɵл�	
void update ();						//���� 
void harder();						//��ʱ������ӵл����� 
void boomgif( int , int , int );	//��ʾ��ը�Ķ�ͼ
void load_gif();					//������
void crashed_gif();					//�ɻ�׹�� 
void show();						//��ʾ����	
void refreshback();					//ˢ�±���
void p_pause ();					//��ͣ
void onceagain ();					//��Ϸʧ��ʱ��ѡ����
void music_get();					//���ֻ�ȡ				
void mainbody();					//��װ��ʽ��Ϸ�����к��� 

int main() {
		initgraph( wide, high, 0 );	//��ʼ��ͼ�ν���
		setcaption("�ɻ���ս");		//���ñ���
		
		imgandsound_init();			//��ϷͼƬ�����ֳ�ʼ��	
		load_gif();					//��ʾ���ػ��� 
		//������Ϸ���� 
		while (1) {
		bgm_wait.SetVolume(0.2);			//�������� 
		bgm_wait.Play();					//���ű������� 
		menu();								//�û��˵�	
		
		//��ʼ��Ϸ 
		while ( isclosed == 0 ) {
			game_init();						//��Ϸ���ݳ�ʼ�� 
			bgm_wait.Pause();
			bgm_fight.SetVolume(0.2);
			bgm_fight.Play(0);
			load_gif();
			if (isrunning)
			for( ; is_run(); delay_fps(60) ) {
				mainbody();						//��Ϸ���� 
				if(isExploded) {
					crashed.Play(0);			//��ը����			
					bgm_fight.Stop();			//ֹͣ�������� 
					//ǿ������ 
					isupdated = 0;				
					isrefresh = false;
					
					crashed_gif();				//�ɻ�׹�ٻ��� 
					onceagain();				//����ɻ�׹�٣�����ѡ���� 	
					if (isback==0) {
					break;						//���¿�ʼ 
			}
					if (isback) {
					isrunning = 0;				//�������˵� 
					break;
				}
			}
		}
			if (isrunning==0) break;
			delay_fps(60);
	}
		
		//������Ϸ 
		if ( isclosed ) {
			break;
		}
	
	}
	gameover();			//��Ϸ������Ĵ��� 
	closegraph();
	return 0;
}

//��װ��ʽ��Ϸ���庯�� 
void mainbody() { 
	moving ();						//�����ɻ��ƶ� 
	bulletget();					//�����ӵ� 
	enemyappear();					//���ɵл�
	enemy_bullet();					//�л��ӵ����� 
	refreshback();					//ˢ�±������γ��ƶ��ı���
	update();						//����
	harder();						//���ӵл����� 
	isagainst();					//�ж��Ƿ���л���ײ
	isattacked();					//����Ƿ񱻹��� 
	show();							//��ʾ����	
	p_pause();						//��p��ͣ��Ϸ
}

//����ͼƬ��С�ĺ��� 
void setimage(int pwidth, int pheight, PIMAGE img_1) {
	//��ȡ����ͼƬ����Ŀ��
	int whidth = getwidth(img_1), height = getheight(img_1);

	//����һ����ʱͼƬ��������µ�ͼ�����Ŀ��ΪҪ�������õ�ͼ��Ŀ��
	PIMAGE tempimg = newimage(pwidth, pheight);

	putimage(tempimg, 0, 0, pwidth, pheight, img_1, 0, 0, whidth, height);	//��ԭ��img�е�ͼ��������Ƶ�img_2��

	getimage(img_1, tempimg, 0, 0, pwidth, pheight);		 //img�ٻ�ȡtemp�е�ͼ��

	delimage(tempimg);  //ʹ����Ͻ��ͷŵ�
}

//��ϷͼƬ��ʼ��	
void imgandsound_init() {
	
	//��ʼ��ͼƬ 
	beginimg 		= newimage();
	planetextimg 	= newimage();
	gameback 		= newimage();
	gameback1 		= newimage();
	gameback2 		= newimage();
	wd_back			= newimage();
	imgofplane 		= newimage();
	bullet1 		= newimage();
	bullet3			= newimage();
	bullet_enm		= newimage();
	enemy1 			= newimage();
	Gameover 		= newimage();
	img_gamestart  	= newimage();
	img_gameover 	= newimage();
	img_update		= newimage(); 
	
	//ѭ����������б�ը��ͼƬ
	char fileofboom[100];		//���ڱ����ļ���
	for (int i = 0; i<9; i++) {
		boom[i] = newimage();
		sprintf(fileofboom,"ͼ��/��ը%d.png",i);
		getimage(boom[i],fileofboom);
		setimage(100,100,boom[i]);
	}
	
	//ѭ����������м��ص�ͼƬ
	char fileofload[100];
	for (int i = 0; i<11; i++) {
		load[i] = newimage();
		sprintf(fileofload,"ͼ��/����%d.jpg",i);
		getimage(load[i],fileofload);
		setimage(wide,high,load[i]);
	}
	
	//ѭ�����������׹�ٵ�ͼƬ
	char fileofboom_self[100];	//���ڱ����ļ���
	for (int i = 1; i<10; i++) {
		boom_self[i] = newimage();
		sprintf(fileofboom_self,"ͼ��/׹��%d.png",i);
		getimage(boom_self[i],fileofboom_self);
		setimage(100,100,boom_self[i]);
	} 
	
	getimage(beginimg,"ͼ��/��ʼ����.jpg");	    	//��ȡ��ʼ����ͼƬ
	setimage(wide, high, beginimg);
	
	getimage(planetextimg, "ͼ��/�ɻ���ս.png");
	setimage(450, 150, planetextimg);												//��ȡ��ʼ��Ϸ����ͼƬ 
	
	getimage(imgofplane,"ͼ��/С�ɻ�1.0.png");		//��ȡ�ɻ�ͼƬ 
	setimage(sizeofplane,sizeofplane,imgofplane);
	
	getimage(gameback,"ͼ��/����.jpg");   			//��ȡ��Ϸ����ͼ 
	setimage(wide-200, 2*high, gameback);
	
	getimage(gameback1,gameback,0,0,wide-200,high); 								//���õ�����Ϸ����ͼһ��Ϊ�� 
	
	getimage(gameback2,gameback,0,high,wide-200,high); 								//���õ�����Ϸ����ͼһ��Ϊ�� 
	
	getimage(wd_back,"ͼ��/wd_bk.png");				//��ȡ���ֱ���ͼ 
	setimage(200, high+200, wd_back);
	
	getimage(bullet1,"ͼ��/�ӵ�1.png");				//��ȡ�ӵ�1ͼƬ		
	setimage(50,50,bullet1);
	
	getimage(bullet3,"ͼ��/�ӵ�3.png");				//��ȡ�ӵ�3ͼƬ 
	setimage(50,50,bullet3);
	
	getimage(bullet_enm,"ͼ��/�л��ӵ�0.png");		//��ȡ�л��ӵ�ͼƬ
	setimage(15,15,bullet_enm);
	 
	getimage(enemy1,"ͼ��/�л�1.png");				//��ȡ�л�1ͼƬ
	setimage(100,100,enemy1);
	
	getimage(Gameover,"ͼ��/gameover.png");			//��ȡ��Ϸ�������� 
	setimage(wide,high,Gameover);
	
	getimage(img_gamestart,"ͼ��/��ʼ��Ϸ.png");	//��ʼ��Ϸ��ť 
	setimage(200,200,img_gamestart);
	
	getimage(img_gameover,"ͼ��/�˳���Ϸ.png");		//������Ϸ��ť 
	setimage(200,200,img_gameover);
	
	getimage(img_update,"ͼ��/����.png");			//������ͼƬ 
	setimage(50,40,img_update);
	
	music_get();																	//���� 
}	 

//���˵� 
void menu () {
	
	cleardevice();
	
	putimage(0, 0, beginimg);							//��ʾ����ͼ

	putimage_withalpha(NULL, planetextimg, 300, 0);		//��ʾ����ͼƬ
	
	putimage_withalpha(NULL, img_gamestart, 400, 200);	//��ʼ��Ϸ��ť	
	
	putimage_withalpha(NULL, img_gameover, 400, 350);	//�˳���Ϸ��ť 
	
	for (; is_run(); delay_fps(60)) {
		//��־λ��Ϊfalse, ���Ǳ�Ҫ�Ĳ���
		isClick = false;
		
		//��ȡ�����Ϣ
		while (mousemsg()) {
			//���������Ϣ�ṹ�� 
			mouse_msg msg = getmouse();
			//����Ϣ����ѭ�����жϵ��
			if (msg.is_left() && msg.is_up()) {
				isClick = true;
				xClick = msg.x;
				yClick = msg.y;
			}
		}
		
		//������ͨ����־λ���ж�	
		if ( isClick ) {
			if ( xClick >= 400 && xClick <=580 && yClick>=200 && yClick <=300 ) {			//��ʼ��Ϸ 
				isrunning = 1;
				sound_click.Play(1100);
				break;
			}
			if ( xClick >= 400 && xClick <=580 && yClick>=400 && yClick <=500 ) {			//�˳���Ϸ 
				sound_click.Play(1100);
				isclosed = 1;
				break;
			}
		}	
	}
} 

//��Ϸ����ʱ�Ĵ���
void gameover () {
	//ɾ��ͼƬ���ͷ��ڴ�	
	delimage(beginimg);
	delimage(planetextimg);
	delimage(gameback);
	delimage(gameback1);
	delimage(gameback2);
	delimage(imgofplane);
	delimage(bullet1);
	delimage(bullet3);
	delimage(enemy1);
	delimage(bullet_enm);
	delimage(Gameover);
	delimage(img_gamestart);
	delimage(img_gameover);
	delimage(wd_back);

	
	for (int i = 0; i<9; i++) {
		delimage(boom[i]);
	}
	
	for (int i = 0; i<11; i++) {
		delimage(load[i]);
	}
	
	for (int i = 1; i<10; i++) {
		delimage(boom_self[i]);
	}
	
	//�ر������ļ� 
	bgm_fight.Close();
	bgm_wait.Close();
	sound_hit.Close();
	crashed.Close();
	sound_click.Close();
	
}

//����ӵ��Ƿ����� 
void ifhit() {
	//�����л��ṹ���� 
	for( int i = 0; i<numofenm_now; i++ ) {
		//������λ�ô��ڵл���δ������ 
		if ( enemy[i].s == 1 && enemy[i].y >= 0 && enemy[i].iskilled == 0 ) { 
			//�ж��ӵ��Ƿ����ел� 
			//�����ӵ��ṹ���� 
			for( int j = 0; j<ammo; j++) {	
				//�����λ�ô����ӵ� 
				if( bullet[j].s==1 ) {
					//�������y����С���ӵ�y���� 
					if ( enemy[i].y < bullet[j].y && (bullet[j].y-enemy[i].y)<55 ) {
						//�������x��������ӵ���x���� 
						if ( enemy[i].x>bullet[j].x ) {
							if ( (enemy[i].x-bullet[j].x) < 20 ) {
								if( bullet[i]._up == 1 ) enemy[i].blood -= 3;
								if( bullet[i]._up == 0 ) enemy[i].blood--;
								bullet[j].s = 0;
								if( enemy[i].blood <= 0 ) {
									enemy[i].iskilled = 1;
									for( int j = 0; j<ammo_enm; j++) {
										enemy[i].blt[j].s_blt = 0;
									}
									score+=10;
									sound_hit.Play(0);
									break; 
								}
							}
						//��֮ 
						} else {
							if( (bullet[j].x-enemy[i].x) < 60 ) {
								if( bullet[i]._up == 1 ) enemy[i].blood -= 3;
								if( bullet[i]._up == 0 ) enemy[i].blood--;
								bullet[j].s = 0;
								if( enemy[i].blood <= 0 ) {
									enemy[i].iskilled = 1;
									for( int j = 0; j<ammo_enm; j++) {
										enemy[i].blt[j].s_blt = 0;
									}
									score+=10;
									sound_hit.Play(0);		
									break;
								}
							}
						}
					}
				}	
			}
		}
	}
}

//�жϷɻ����л�֮���Ƿ���ײ 
void isagainst() {
	//�����л��ṹ������ӵ����飬���ɻ�֮�估�ɻ����ӵ�֮���Ƿ������ײ 
	for( int i = 0; i<numofenm_now; i++ ) {
		//������λ�ô��ڵл� 
		if ( enemy[i].s == 1 && enemy[i].y >= 0 ) {  
			if( ximgofplane<=enemy[i].x ) {
				if((enemy[i].x-ximgofplane)<=50) {
					if(yimgofplane<=enemy[i].y) {
						if((enemy[i].y-yimgofplane)<=50) {
							isExploded = 1;
							enemy[i].iskilled = 1;
							blood_x = 10;
							break;
						} 
					} else {
						if((yimgofplane-enemy[i].y)<=50) {
							isExploded = 1;
							enemy[i].iskilled = 1;
							blood_x = 10;
							break;
						}
					}
				}  
			} else {
				if((ximgofplane-enemy[i].x)<=100) {
					if(yimgofplane<=enemy[i].y) {
						if((enemy[i].y-yimgofplane)<=50) {
							isExploded = 1;
							enemy[i].iskilled = 1;
							blood_x = 10;
							break;
						} 
					} else {
						if((yimgofplane-enemy[i].y)<=50) {
							isExploded = 1;
							enemy[i].iskilled = 1;
							blood_x = 10;
							break;
						}
					}
				}  
			}				
		}
	}
} 

//�ɻ��ƶ�WASD/��������
void moving () {
 	float xNext = ximgofplane;//���С�ɻ��ĳ�ʼλ�� 
 	float yNext = yimgofplane;
	for(int i = 0; i < 4; i++) {
			if( keystate(keys[i]) || keystate(directkeys[i]) ) {
				xNext += xspeeds[i];
				yNext += yspeeds[i];
			}
		} 	
	//����ƶ��� 
	if (xNext != ximgofplane || yNext != yimgofplane) {
		if ( 200 <= xNext && xNext <= wide-sizeofplane && 0 <= yNext && yNext <= high-sizeofplane) {
			ximgofplane = xNext;
			yimgofplane = yNext;			
		}	
	}		//�ı�ɻ����� 
} 
 
void bulletget() {
	
	//��ȡ��ǰʱ�� 
	static double t2 = fclock();

	//��ȡ��ǰ�ӵ��ĳ�ʼλ�ò��洢�ڽṹ������ 
	for (int i = 0; i<ammo; i++)  {
		//i%btbullet���ڿ����ӵ����ɵ��ٶ�
		if ( bullet[i].s == 0 && (fclock()-t2) >= gapofbullet ) {
			//ÿһö�ӵ��ĳ�ʼλ�����Ƿɻ�ͷ��
			bullet[i].x = ximgofplane+10;	
			bullet[i].y = yimgofplane-50;	 
			bullet[i].s = 1;				//������λ�ô洢��һö�ӵ�
			if (isupdated) bullet[i]._up = 1;
			else bullet[i]._up = 0;
			t2 = fclock();
			break;
		}
	}
	
	//ÿһö�ӵ���λ�ò�ͣ�ı� 
	for (int j = 0; j<ammo; j++) {
		if ( bullet[j].s == 1 ) {
			if( bullet[j].y > 0 ) {
				bullet[j].y -= spdofblt;	//�ӵ��ƶ� 
			} else {
				bullet[j].y = high + 50;	//�ӵ����Ｋ��λ�ú��Ƴ���Ļ 
				bullet[j].s = 0;			//������λ������  
			}
		}
	}
	ifhit();	
}

//���ɵл� 
void enemyappear() {
	//��ȡ��ǰʱ�� 
	static double t2 = fclock();
//	isstarted = true;
	for( int i = 0; i<numofenm_now; i++ ) {
		//����л��ṹ�������λ��δ��ɻ�������һ�ܷɻ� 
		if( enemy[i].s==0 && enemy[i].iskilled == 0 && ( fclock()-t2 ) >= gapofenemy ) { 
			//�õл��ĳ�ʼ����Ӷ����������
			enemy[i].x = random(700)+200;		//�л���ʼ������ 
			enemy[i].y = -200;					//�л���ʼ������ 
			enemy[i].s = 1;						//����һ�ܵл� 
			t2 = fclock();						//����ʱ�� 
			break;
	}
		//����л��ṹ�������λ����һ�ܷɻ���������ɻ��ƶ����� 
		if ( enemy[i].s == 1 && enemy[i].iskilled == 0 ) {
			
			//����л�δʻ�����������·��У��������� 
			if ( enemy[i].y < high) {
				enemy[i].y += spdofenemy;					
			} else {
				score -= 1;
				enemy[i].s = 0;
			}		
		}	
	}
}	 

//���л��ӵ����������ƶ� 
void enemy_bullet() {
	for (int i = 0; i<numofenm_now; i++) {
		if( enemy[i].s == 1 && enemy[i].iskilled == 0 && enemy[i].y >= 0) {
				for (int j = 0; j<ammo_enm; j++) {
				if ( enemy[i].blt[j].s_blt == 0  )	{
					enemy[i].blt[j].blt_enm_x = enemy[i].x + 40;	//�л��ӵ�x����
					enemy[i].blt[j].blt_enm_y = enemy[i].y + 80;	//�л��ӵ�y����
					enemy[i].blt[j].s_blt = 1;						//�����ӵ�
					break;
				}				
		}
			for ( int j = 0; j<ammo_enm; j++) {
				if ( enemy[i].blt[j].s_blt == 1 ) {
					if ( enemy[i].blt[j].blt_enm_y < 670 ) enemy[i].blt[j].blt_enm_y += 4.0;
					else enemy[i].blt[j].s_blt = 0;	
				}
			}
		}
	}
}

//���� 
void update () {
	 
	//��ȡ��ǰʱ�䣬�ڻ��ǿ����������5s֮ǰ������ˢ��,����ȡһ�� 
	if ( !isrefresh ) {
		t_up = fclock();
		isrefresh = true;
	}	
	//ÿ��10sһ��������
	if ( (fclock() - t_up) >= 10 && isupexist == false ) {
	 	 isupexist = true; 
	 }	 
	 //������������������ƶ����� 
	 if ( isupexist && isupdated == 0 ) {
	 	x_up += dx; 
		y_up += dy;
		if(x_up<=200) dx = speed_up;
		if(y_up<=0) dy = speed_up;
		if( x_up>=(wide-51) ) dx = -speed_up;
		if( y_up>=(high-41) ) dy = -speed_up; 
	 }
	 //��������� 
	 if ( abs(ximgofplane-x_up) <=50 && abs(yimgofplane-y_up) <= 50 && isupexist ) {
		isupexist = false;
		isupdated = 1;
		//��������ʼ��
		x_up = random(700) + 200;
		y_up = -100;
		spdofblt = 6.0;
		gapofbullet = 0.2;
		//ǿ�������ӵ�
		for (int i = 0; i<ammo; i++) {
			if ( bullet[i].s == 1 ) bullet[i]._up = 1;
		} 
		dur = fclock();
	}
	 //ǿ������5s 
	 if( ( fclock() - dur ) >= 5 && isupdated == 1 ) {
			isupdated = 0;
			isrefresh = false;
			spdofblt = 3.0;
			gapofbullet =0.5;
		}
		
}	
	 
//��ʾ��ը�Ķ�ͼ 
void boom_gif( int x, int y, int i) {
	
	static double t1 = fclock();
	if ( (fclock()-t1)>0.03 ) {
		putimage_withalpha(NULL, boom[enemy[i].pboom], x, y);
		enemy[i].pboom++;
		t1 = fclock();
	}
	//��ը���������󣬳�ʼ�� 
	if( enemy[i].pboom > 8 ) {
		enemy[i].s = 0;
		enemy[i].iskilled = 0;
		enemy[i].blood =3;
		enemy[i].pboom = 0;
	}
}

//������
void load_gif() {
	
	double t3 = fclock();
	int i = 0;
	while(1) {
		if ( (fclock()-t3) >= 0.15 ) {
			cleardevice();
			putimage(0,0,load[i]);
			i++;
			t3 = fclock();
			delay_fps(60);
		}
		if ( i > 10 ) break;
	}

}

//׹��gif 
void crashed_gif() {
	double t4 = fclock();
	int i = 1;
	crashed.Play(0);
	while(1) {
		if ( (fclock()-t4) >= 0.1 ) {
			putimage_withalpha(NULL,boom_self[i],ximgofplane-20,yimgofplane-20);
			i++;
			t4 = fclock();
			delay_fps(60);
		}
		if ( i > 9 ) break;
	}
}


//��ʾ���� 
void show() {
		
		cleardevice();														//���� 
		//ץͼ���ڳ����з��� 	
		putimage(200, yofback1, gameback1);									//��ʾ��Ϸ����1 
		putimage(200, yofback2, gameback2);						        	//��ʾ��Ϸ����2 
		putimage_withalpha(NULL,wd_back,0,-50);								//��ʾ���ֱ��� 
		putimage_withalpha(NULL, imgofplane, ximgofplane, yimgofplane); 	//��ʾ�ɻ�
	 	
		//��ʾ�ӵ�
		for ( int i = ammo; i>=0; i-- ) {
			if( bullet[i].s==1  ) {
				if( bullet[i]._up ) putimage_withalpha(NULL,bullet3,bullet[i].x,bullet[i].y);
				else putimage_withalpha(NULL, bullet1, bullet[i].x, bullet[i].y); 
			}
							 						
		}
			
		//��ʾ�л� 
		for ( int i = 0; i<numofenm_now; i++ ) {
			//���λ���ел���δ������,��ʾ�л� 
			if( enemy[i].s==1 &&  enemy[i].iskilled == 0 ) {
				putimage_withalpha(NULL, enemy1, enemy[i].x, enemy[i].y);
				//��ʾ�ڻ����еĵл����ӵ� 
				for ( int j = 0; j<ammo_enm; j++) {
					if ( enemy[i].blt[j].s_blt == 1 ) putimage_withalpha(NULL, bullet_enm, enemy[i].blt[j].blt_enm_x, enemy[i].blt[j].blt_enm_y);
				}
			} 
			//���λ���ел����ѱ�������ʾ��ը���� 
			if( enemy[i].s==1  && enemy[i].iskilled == 1 ) {
				boom_gif( enemy[i].x, enemy[i].y, i );
			}
		}
		
		//��ʾ������ 
		if ( isupexist ) putimage_withalpha(NULL,img_update,x_up,y_up);
			 
	//������ص���ʾ
	setbkmode(TRANSPARENT);			//�������ֱ���ɫΪ͸��	
	setcolor(BLACK);
	setfont(30,0,"����"); 	
	xyprintf(0, high*0.95, "�÷֣� %d", score);
	setfont(30,0,"����");
	setcolor(RED);
	outtextxy(5,0,"���� :");
	outtextxy(5,35,"W ����");
	outtextxy(5,70,"S ����");
	outtextxy(5,105,"A : ��");
	outtextxy(5,140,"D : ��");
	outtextxy(5,175,"P:��ͣ��Ϸ");
	outtextrect(5,210,200,70,"M:��/�ر�����");
	setfont(35,0,"����"); 
	outtextxy(5,290,"Ѫ��");

	//Ѫ����ʾ 
	setfillcolor(YELLOW);
	bar(10,340,180,360);
	setfillcolor(BLACK);
	bar(blood_x,340,180,360);

}


//ˢ�±��� 
void refreshback() {
	yofback1 += spdofback;
	yofback2 += spdofback; 
	if( yofback1 == 0 )  {
		yofback2 = -high;
	}
	if( yofback2 == -1) {
		yofback1 = -high;
	}
}

 
//��ͣ
void p_pause () {

	key_msg keyMsg = {0};
	bool ispressP = false;
	static int ispressM = -1;
	
	if ( kbmsg() ) {
		keyMsg = getkey();
		if ( keyMsg.msg == key_msg_down && keyMsg.key == 'P' ) {
			ispressP = true;
		}
		if ( keyMsg.msg == key_msg_down && keyMsg.key == 'M') {
			ispressM = -ispressM;
		}
	}
	
	//�������M���򿪻�رձ�������
	if ( ispressM == 1 ) bgm_fight.Pause();
	if ( ispressM == -1 ) bgm_fight.Play();
		
	//�������p��������ͣ���� 
	if (ispressP) {
		bgm_fight.Pause();
		bgm_wait.Play();
		//���岢��ʼ��ͼƬ 
		PIMAGE pause_bk = newimage();
		PIMAGE pause_wd = newimage();
		
		//��ȡͼƬ 
		getimage(pause_bk,"ͼ��/pause_back.png");
		getimage(pause_wd,"ͼ��/pause.png");
		
		//����ͼƬ��С
		setimage(wide,high,pause_bk);
		setimage(200,150,pause_wd);
		
		//��ʾ��ͣ���� 
		cleardevice();
		putimage_withalpha(NULL,pause_bk,0,0);
		putimage_withalpha(NULL,pause_wd,400,200);
		
		setfont(30,0,"����");
		setcolor(BLUE);
		xyprintf(380,350,"��M�ر�/������");
		setfont(20,0,"����");
		outtextrect(200,400,600,200,"����һ�ܵл���10�֣�����һ�ܵл���1��; ��л���ײ�ɻ���ֱ��׹�٣��ܵ��з��������������ֵ������ʱ������ӣ���������Ҳ�����ӣ���׹��֮ǰ�������÷ְɣ����ͣ����ֱ�"); 
		
		int isPause = -1;
		//��ѭ��,��p�˳� 
		while (1) {	
			//��ⰴ����Ϣ	
			flushkey();
			keyMsg = getkey();
			if (isPause == -1) bgm_wait.Play();
			if (isPause == 1) bgm_wait.Pause();
			if ( keyMsg.key == 'M' && keyMsg.msg == key_msg_down ) {
				isPause = -isPause;
			}
			if ( keyMsg.key == 'P' && keyMsg.msg == key_msg_down ) {
				delimage(pause_bk);
				delimage(pause_wd);
				bgm_fight.Play();
				bgm_wait.Pause();
				break;
			}
		}	 
	}
} 

//��Ϸʧ��ʱ��ѡ����
void onceagain () {
	
			bgm_fight.Stop();
			cleardevice();
		
			putimage(0,0,Gameover);
		
			setfont(50,0,"����");

			setcolor(YELLOW);
		
			xyprintf(270,400,"��ĵ÷��ǣ� %d",score);
		
			outtextxy(150,500,"��R�����¿�ʼ����Q�������˵�");
		
			key_msg keymsg_choose = {0};

			int press_which;
		
			while ( 1 ) {
				if (kbmsg()) {	
					keymsg_choose = getkey();
					//��Q���������� 
					if ( keymsg_choose.msg == key_msg_down && keymsg_choose.key == 'Q') {
						press_which = 0;
						break;
				}
					//��R���¿�ʼ��Ϸ 
					if ( keymsg_choose.msg == key_msg_down && keymsg_choose.key == 'R') {
						press_which = 1;
						break;
				}
   			}
				delay_fps(60);
		}
		 
		if ( press_which == 1 ) {
			isback = 0;
			game_init();
		}
		
		if ( press_which == 0 ) {
			isback = 1;
			load_gif();
		}
			
} 
 
void music_get() {
		
		//��ȡ��Ƶ 
		bgm_fight.OpenFile("C:/Users/Hasee/Documents/С�ɻ�/Venom.mp3");
		bgm_wait.OpenFile("C:/Users/Hasee/Documents/С�ɻ�/BinaryStar.mp3");
		sound_hit.OpenFile("C:/Users/Hasee/Documents/С�ɻ�/hit.mp3");
		crashed.OpenFile("sound_click.Play(0);crashed.mp3");
		sound_click.OpenFile("C:/Users/Hasee/Documents/С�ɻ�/click.mp3");
}

//��Ϸ���ݳ�ʼ�� 
void game_init() {
	
	//������ʼ��
	score = 0;
	 
	//�ɻ�����
	isExploded = 0; 
	 
	//�ɻ�λ�ó�ʼ��
	ximgofplane = wide*0.5 - sizeofplane/2 + 100; 
	yimgofplane = high - sizeofplane;
	
	//�ӵ���ʼ�� 
	for ( int i = 0; i<ammo; i++ ) {
		bullet[i].s = 0;
		bullet[i]._up = 0;
	}  //�����ӵ��ṹ����
			
	//�л���ʼ�� 
	for ( int i = 0; i<numofenm; i++) {
		enemy[i].s = 0;
		enemy[i].iskilled = 0;
		enemy[i].pboom = 0; 		//Ĭ�ϴӵ�һ�ű�ըͼ��ʼ����
		enemy[i].blood = 3; 		//�л�Ѫ��Ϊ3
		for (int j = 0; j<ammo_enm; j++) enemy[i].blt[j].s_blt = 0;
	} //�����л��ṹ����
	 
	//��������ʼ��
	x_up = random(700) + 200;
	y_up = -100;
	isupdated = 0;
	isupexist = false;
	isrefresh = false;
	spdofblt = 3.0;
	
	//Ѫ����ʼ��
	blood_x = 180; 
}

//����Ƿ񱻵л��ӵ����� 
void isattacked() {
	//�Դ��ڵĵл����м�� 
	for ( int i = 0; i<numofenm_now; i++ ) {
		if ( enemy[i].s == 1 && enemy[i].iskilled == 0 ) {
			for ( int j = 0; j<ammo_enm; j++ ) {
				if ( enemy[i].blt[j].s_blt == 1 ) {
					if ( ( abs(ximgofplane-enemy[i].blt[j].blt_enm_x) + abs(yimgofplane-enemy[i].blt[j].blt_enm_y ) ) <= 40 ) {
						enemy[i].blt[j].s_blt = 0;
						if ( blood_x > 10 ) blood_x -= 10;
						if ( blood_x <= 10 ) isExploded = 1;
						break;
					}  
				}
			}
		}
	}
}

//����ʱ�����ӵл����� 
void harder() {
	static double ishard = fclock();
	if ( ( fclock() - ishard ) >= 2.5 ) {
		if ( numofenm_now < numofenm ) {
			 numofenm_now++;
			 if ( gapofenemy > 0.5 ) gapofenemy -= 0.1;
			 ishard = fclock();
		}
	} 
}


