#include <graphics.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define wide 1000
#define high 700								//程序窗口大小宽和高
#define sizeofplane 70							//飞机大小
#define ammo 50									//最大最大子弹数 
#define numofenm 20 							//最大敌机数
#define ammo_enm 10								//敌人子弹数 
#define xdistofPE abs(ximgofplane-enemy[i].x)	//敌机与我方飞机的x轴距离
#define ydistofPE abs(yimgofplane-enemy[i].y)	//敌机与我方飞机的y轴距离 

//图片的全局变量 
PIMAGE beginimg; 			//初始界面 			√ 
PIMAGE img_gamestart;		//开始游戏图片 		√ 
PIMAGE img_gameover;		//退出游戏图片		√ 
PIMAGE gameback;			//背景图片			√
PIMAGE gameback1;			//将背景图片分为1 	√ 
PIMAGE gameback2; 			//将背景图片分为2 	√
PIMAGE wd_back;				//文字背景图		√ 
PIMAGE planetextimg;		//初始界面文字		√
PIMAGE imgofplane;			//飞机图片 			√ 
PIMAGE bullet1; 			//子弹图片1			√ 
PIMAGE bullet3;				//子弹图片3			√
PIMAGE bullet_enm;			//敌机子弹			√ 
PIMAGE enemy1;      		//敌机图片			√ 
PIMAGE Gameover;			//游戏结束界面		√  
PIMAGE boom[9];				//爆炸图片，有9张	√ 
PIMAGE load[11]; 			//加载图片，有11张	√
PIMAGE boom_self[10];		//己方飞机爆炸图片  √
PIMAGE img_update;			//升级				√

//用户交互全局变量 
bool isClick;				//鼠标是否点击 		
int xClick, yClick;			//鼠标点击的坐标 	
int mouse_x,mouse_y;		//鼠标的实时坐标 	
int isclosed = 0;			//是否关闭窗口 
int isback = 0;				//是否返回主菜单
int isrunning = 1;			//是否在进行游戏		

//音乐结构体 
MUSIC bgm_fight; 	//背景音乐
MUSIC bgm_wait;		//等待时音乐 
MUSIC sound_hit; 	//子弹命中声音
MUSIC crashed;		//坠毁时的声音 
MUSIC sound_click;	//点击时的声音 


//背景图片y轴坐标 
float yofback1 = -high; 
float yofback2 = 0;
double spdofback = 0.5; //背景移动速度 


//子弹，飞机，敌机等位置变量及速度变量 
//关于飞机位置及移动的变量 
float ximgofplane = wide*0.5 - sizeofplane/2 + 100 ,  yimgofplane = high - sizeofplane;	//浮点型，精确小飞机位置,这里初始化为小飞机出生点  
float speed = 10.0f;	//移动速度，浮点型能任意控制速度快慢
float xspeeds[4] = { -speed, 0, speed, 0 }; 
float yspeeds[4] = { 0, -speed, 0, speed }; 
int keys[4] = {'A', 'W', 'D', 'S' };	//0，1，2，3分别代表左，上，右，下
int directkeys[4] = {key_left, key_up, key_right, key_down }; 
int blood_x ;		//显示飞机血量的x坐标 

//敌机属性及位置
struct enm {
	int x;					//x坐标 
	int y;					//y坐标 
	int s;					//状态判断 1代表数组这个位置存在一架敌机，0代表未存敌机，可以新存一架
	int iskilled;			//敌机是否被击杀 
	int pboom; 				//敌机的爆炸图片顺序储存 
	int blood;				//敌机的血量
	//在敌人结构数组中定义一个结构数组来表示敌人的子弹 
	struct {
		int blt_enm_x; 			//敌人的子弹的x坐标 
		int blt_enm_y; 			//敌人的子弹的y坐标
		int s_blt;				//敌人的子弹状态， 
	}blt[ammo_enm]; 
 } enemy[numofenm];
double spdofenemy = 1.5;	//敌机速度
double spdofenmblt = 2.0;	//敌机子弹速度 
double gapofenemy = 3.5;	//敌机出现间隔时间 
int numofenm_now = 5;		//当前最大敌机数 

//子弹属性及位置 
struct  bullet_plane {
	int x;		//x坐标 
	int y;		//y坐标 
	int s;		//状态判断 1代表数组这个位置存在一枚子弹，0代表未存子弹，可以新存一枚
	int _up;	//判断是否为升级子弹 
} bullet[ammo];
double spdofblt = 3.0;		//子弹速度
double gapofbullet = 0.5; 	//子弹出现的初始间隔时间  
int isExploded; 			//判断飞机是否爆炸的值 
int score = 0; 				//得分
 
//升级包位置
int x_up,y_up;
int speed_up = 2;
int dx = speed_up, dy = speed_up;
bool isupexist = false;				//是否出现升级包 
int isupdated = 0;					//是否升级 
double t_up, dur;
bool isrefresh = false;

//函数声明区 
void setimage();					//设置图片大小的函数
void imgandsound_init();			//游戏图片初始化
void game_init();					//游戏数据初始化 
void menu ();						//主菜单
void gameover ();					//游戏结束时的处理
void ifhit();						//检测子弹是否命中
void isagainst();					//判断飞机、敌机之间是否碰撞
void isattacked();					//是否被攻击 
void moving ();						//飞机移动WASD/上下左右
void bulletget();					//生成子弹
void enemy_bullet();				//敌机子弹 				
void enemyappear();					//生成敌机	
void update ();						//升级 
void harder();						//到时间后增加敌机数量 
void boomgif( int , int , int );	//显示爆炸的动图
void load_gif();					//加载中
void crashed_gif();					//飞机坠毁 
void show();						//显示画面	
void refreshback();					//刷新背景
void p_pause ();					//暂停
void onceagain ();					//游戏失败时的选择画面
void music_get();					//音乐获取				
void mainbody();					//封装正式游戏的所有函数 

int main() {
		initgraph( wide, high, 0 );	//初始化图形界面
		setcaption("飞机大战");		//设置标题
		
		imgandsound_init();			//游戏图片、音乐初始化	
		load_gif();					//显示加载画面 
		//进入游戏程序 
		while (1) {
		bgm_wait.SetVolume(0.2);			//调节音量 
		bgm_wait.Play();					//播放背景音乐 
		menu();								//用户菜单	
		
		//开始游戏 
		while ( isclosed == 0 ) {
			game_init();						//游戏数据初始化 
			bgm_wait.Pause();
			bgm_fight.SetVolume(0.2);
			bgm_fight.Play(0);
			load_gif();
			if (isrunning)
			for( ; is_run(); delay_fps(60) ) {
				mainbody();						//游戏主体 
				if(isExploded) {
					crashed.Play(0);			//爆炸音乐			
					bgm_fight.Stop();			//停止背景音乐 
					//强化结束 
					isupdated = 0;				
					isrefresh = false;
					
					crashed_gif();				//飞机坠毁画面 
					onceagain();				//如果飞机坠毁，进入选择画面 	
					if (isback==0) {
					break;						//重新开始 
			}
					if (isback) {
					isrunning = 0;				//返回主菜单 
					break;
				}
			}
		}
			if (isrunning==0) break;
			delay_fps(60);
	}
		
		//结束游戏 
		if ( isclosed ) {
			break;
		}
	
	}
	gameover();			//游戏结束后的处理 
	closegraph();
	return 0;
}

//封装正式游戏主体函数 
void mainbody() { 
	moving ();						//己方飞机移动 
	bulletget();					//发射子弹 
	enemyappear();					//生成敌机
	enemy_bullet();					//敌机子弹生成 
	refreshback();					//刷新背景，形成移动的背景
	update();						//升级
	harder();						//增加敌机数量 
	isagainst();					//判断是否与敌机相撞
	isattacked();					//检测是否被攻击 
	show();							//显示画面	
	p_pause();						//按p暂停游戏
}

//设置图片大小的函数 
void setimage(int pwidth, int pheight, PIMAGE img_1) {
	//获取参数图片对象的宽高
	int whidth = getwidth(img_1), height = getheight(img_1);

	//创建一个临时图片对象，这个新的图像对象的宽高为要重新设置的图像的宽高
	PIMAGE tempimg = newimage(pwidth, pheight);

	putimage(tempimg, 0, 0, pwidth, pheight, img_1, 0, 0, whidth, height);	//将原本img中的图像拉伸绘制到img_2中

	getimage(img_1, tempimg, 0, 0, pwidth, pheight);		 //img再获取temp中的图像

	delimage(tempimg);  //使用完毕将释放掉
}

//游戏图片初始化	
void imgandsound_init() {
	
	//初始化图片 
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
	
	//循环来获得所有爆炸的图片
	char fileofboom[100];		//用于保存文件名
	for (int i = 0; i<9; i++) {
		boom[i] = newimage();
		sprintf(fileofboom,"图像/爆炸%d.png",i);
		getimage(boom[i],fileofboom);
		setimage(100,100,boom[i]);
	}
	
	//循环来获得所有加载的图片
	char fileofload[100];
	for (int i = 0; i<11; i++) {
		load[i] = newimage();
		sprintf(fileofload,"图像/加载%d.jpg",i);
		getimage(load[i],fileofload);
		setimage(wide,high,load[i]);
	}
	
	//循环来获得所有坠毁的图片
	char fileofboom_self[100];	//用于保存文件名
	for (int i = 1; i<10; i++) {
		boom_self[i] = newimage();
		sprintf(fileofboom_self,"图像/坠毁%d.png",i);
		getimage(boom_self[i],fileofboom_self);
		setimage(100,100,boom_self[i]);
	} 
	
	getimage(beginimg,"图像/开始界面.jpg");	    	//获取开始界面图片
	setimage(wide, high, beginimg);
	
	getimage(planetextimg, "图像/飞机大战.png");
	setimage(450, 150, planetextimg);												//获取开始游戏文字图片 
	
	getimage(imgofplane,"图像/小飞机1.0.png");		//获取飞机图片 
	setimage(sizeofplane,sizeofplane,imgofplane);
	
	getimage(gameback,"图像/背景.jpg");   			//获取游戏背景图 
	setimage(wide-200, 2*high, gameback);
	
	getimage(gameback1,gameback,0,0,wide-200,high); 								//将得到的游戏背景图一分为二 
	
	getimage(gameback2,gameback,0,high,wide-200,high); 								//将得到的游戏背景图一分为二 
	
	getimage(wd_back,"图像/wd_bk.png");				//获取文字背景图 
	setimage(200, high+200, wd_back);
	
	getimage(bullet1,"图像/子弹1.png");				//获取子弹1图片		
	setimage(50,50,bullet1);
	
	getimage(bullet3,"图像/子弹3.png");				//获取子弹3图片 
	setimage(50,50,bullet3);
	
	getimage(bullet_enm,"图像/敌机子弹0.png");		//获取敌机子弹图片
	setimage(15,15,bullet_enm);
	 
	getimage(enemy1,"图像/敌机1.png");				//获取敌机1图片
	setimage(100,100,enemy1);
	
	getimage(Gameover,"图像/gameover.png");			//获取游戏结束界面 
	setimage(wide,high,Gameover);
	
	getimage(img_gamestart,"图像/开始游戏.png");	//开始游戏按钮 
	setimage(200,200,img_gamestart);
	
	getimage(img_gameover,"图像/退出游戏.png");		//结束游戏按钮 
	setimage(200,200,img_gameover);
	
	getimage(img_update,"图像/升级.png");			//升级包图片 
	setimage(50,40,img_update);
	
	music_get();																	//音乐 
}	 

//主菜单 
void menu () {
	
	cleardevice();
	
	putimage(0, 0, beginimg);							//显示背景图

	putimage_withalpha(NULL, planetextimg, 300, 0);		//显示文字图片
	
	putimage_withalpha(NULL, img_gamestart, 400, 200);	//开始游戏按钮	
	
	putimage_withalpha(NULL, img_gameover, 400, 350);	//退出游戏按钮 
	
	for (; is_run(); delay_fps(60)) {
		//标志位设为false, 这是必要的步骤
		isClick = false;
		
		//获取鼠标消息
		while (mousemsg()) {
			//定义鼠标消息结构体 
			mouse_msg msg = getmouse();
			//在消息处理循环中判断点击
			if (msg.is_left() && msg.is_up()) {
				isClick = true;
				xClick = msg.x;
				yClick = msg.y;
			}
		}
		
		//在这里通过标志位来判断	
		if ( isClick ) {
			if ( xClick >= 400 && xClick <=580 && yClick>=200 && yClick <=300 ) {			//开始游戏 
				isrunning = 1;
				sound_click.Play(1100);
				break;
			}
			if ( xClick >= 400 && xClick <=580 && yClick>=400 && yClick <=500 ) {			//退出游戏 
				sound_click.Play(1100);
				isclosed = 1;
				break;
			}
		}	
	}
} 

//游戏结束时的处理
void gameover () {
	//删除图片，释放内存	
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
	
	//关闭音乐文件 
	bgm_fight.Close();
	bgm_wait.Close();
	sound_hit.Close();
	crashed.Close();
	sound_click.Close();
	
}

//检测子弹是否命中 
void ifhit() {
	//遍历敌机结构数组 
	for( int i = 0; i<numofenm_now; i++ ) {
		//如果这个位置存在敌机且未被击毁 
		if ( enemy[i].s == 1 && enemy[i].y >= 0 && enemy[i].iskilled == 0 ) { 
			//判断子弹是否命中敌机 
			//遍历子弹结构数组 
			for( int j = 0; j<ammo; j++) {	
				//如果该位置存在子弹 
				if( bullet[j].s==1 ) {
					//如果敌人y坐标小于子弹y坐标 
					if ( enemy[i].y < bullet[j].y && (bullet[j].y-enemy[i].y)<55 ) {
						//如果敌人x坐标大于子弹的x坐标 
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
						//反之 
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

//判断飞机、敌机之间是否碰撞 
void isagainst() {
	//遍历敌机结构数组和子弹数组，检测飞机之间及飞机和子弹之间是否存在碰撞 
	for( int i = 0; i<numofenm_now; i++ ) {
		//如果这个位置存在敌机 
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

//飞机移动WASD/上下左右
void moving () {
 	float xNext = ximgofplane;//获得小飞机的初始位置 
 	float yNext = yimgofplane;
	for(int i = 0; i < 4; i++) {
			if( keystate(keys[i]) || keystate(directkeys[i]) ) {
				xNext += xspeeds[i];
				yNext += yspeeds[i];
			}
		} 	
	//如果移动了 
	if (xNext != ximgofplane || yNext != yimgofplane) {
		if ( 200 <= xNext && xNext <= wide-sizeofplane && 0 <= yNext && yNext <= high-sizeofplane) {
			ximgofplane = xNext;
			yimgofplane = yNext;			
		}	
	}		//改变飞机坐标 
} 
 
void bulletget() {
	
	//获取当前时间 
	static double t2 = fclock();

	//获取当前子弹的初始位置并存储在结构数组中 
	for (int i = 0; i<ammo; i++)  {
		//i%btbullet用于控制子弹生成的速度
		if ( bullet[i].s == 0 && (fclock()-t2) >= gapofbullet ) {
			//每一枚子弹的初始位置总是飞机头部
			bullet[i].x = ximgofplane+10;	
			bullet[i].y = yimgofplane-50;	 
			bullet[i].s = 1;				//该数组位置存储了一枚子弹
			if (isupdated) bullet[i]._up = 1;
			else bullet[i]._up = 0;
			t2 = fclock();
			break;
		}
	}
	
	//每一枚子弹的位置不停改变 
	for (int j = 0; j<ammo; j++) {
		if ( bullet[j].s == 1 ) {
			if( bullet[j].y > 0 ) {
				bullet[j].y -= spdofblt;	//子弹移动 
			} else {
				bullet[j].y = high + 50;	//子弹到达极限位置后移出屏幕 
				bullet[j].s = 0;			//该数组位置重置  
			}
		}
	}
	ifhit();	
}

//生成敌机 
void enemyappear() {
	//获取当前时间 
	static double t2 = fclock();
//	isstarted = true;
	for( int i = 0; i<numofenm_now; i++ ) {
		//如果敌机结构数组这个位置未存飞机，存入一架飞机 
		if( enemy[i].s==0 && enemy[i].iskilled == 0 && ( fclock()-t2 ) >= gapofenemy ) { 
			//让敌机的初始坐标从顶部随机生成
			enemy[i].x = random(700)+200;		//敌机初始横坐标 
			enemy[i].y = -200;					//敌机初始纵坐标 
			enemy[i].s = 1;						//存入一架敌机 
			t2 = fclock();						//更新时间 
			break;
	}
		//如果敌机结构数组这个位置有一架飞机，让这个飞机移动起来 
		if ( enemy[i].s == 1 && enemy[i].iskilled == 0 ) {
			
			//如果敌机未驶出画布，向下飞行，否则重置 
			if ( enemy[i].y < high) {
				enemy[i].y += spdofenemy;					
			} else {
				score -= 1;
				enemy[i].s = 0;
			}		
		}	
	}
}	 

//给敌机子弹并让它们移动 
void enemy_bullet() {
	for (int i = 0; i<numofenm_now; i++) {
		if( enemy[i].s == 1 && enemy[i].iskilled == 0 && enemy[i].y >= 0) {
				for (int j = 0; j<ammo_enm; j++) {
				if ( enemy[i].blt[j].s_blt == 0  )	{
					enemy[i].blt[j].blt_enm_x = enemy[i].x + 40;	//敌机子弹x坐标
					enemy[i].blt[j].blt_enm_y = enemy[i].y + 80;	//敌机子弹y坐标
					enemy[i].blt[j].s_blt = 1;						//生成子弹
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

//升级 
void update () {
	 
	//获取当前时间，在获得强化包并持续5s之前，不再刷新,仅获取一次 
	if ( !isrefresh ) {
		t_up = fclock();
		isrefresh = true;
	}	
	//每过10s一个升级包
	if ( (fclock() - t_up) >= 10 && isupexist == false ) {
	 	 isupexist = true; 
	 }	 
	 //如果升级包存在让它移动起来 
	 if ( isupexist && isupdated == 0 ) {
	 	x_up += dx; 
		y_up += dy;
		if(x_up<=200) dx = speed_up;
		if(y_up<=0) dy = speed_up;
		if( x_up>=(wide-51) ) dx = -speed_up;
		if( y_up>=(high-41) ) dy = -speed_up; 
	 }
	 //获得升级包 
	 if ( abs(ximgofplane-x_up) <=50 && abs(yimgofplane-y_up) <= 50 && isupexist ) {
		isupexist = false;
		isupdated = 1;
		//升级包初始化
		x_up = random(700) + 200;
		y_up = -100;
		spdofblt = 6.0;
		gapofbullet = 0.2;
		//强化所有子弹
		for (int i = 0; i<ammo; i++) {
			if ( bullet[i].s == 1 ) bullet[i]._up = 1;
		} 
		dur = fclock();
	}
	 //强化持续5s 
	 if( ( fclock() - dur ) >= 5 && isupdated == 1 ) {
			isupdated = 0;
			isrefresh = false;
			spdofblt = 3.0;
			gapofbullet =0.5;
		}
		
}	
	 
//显示爆炸的动图 
void boom_gif( int x, int y, int i) {
	
	static double t1 = fclock();
	if ( (fclock()-t1)>0.03 ) {
		putimage_withalpha(NULL, boom[enemy[i].pboom], x, y);
		enemy[i].pboom++;
		t1 = fclock();
	}
	//爆炸动画结束后，初始化 
	if( enemy[i].pboom > 8 ) {
		enemy[i].s = 0;
		enemy[i].iskilled = 0;
		enemy[i].blood =3;
		enemy[i].pboom = 0;
	}
}

//加载中
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

//坠毁gif 
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


//显示画面 
void show() {
		
		cleardevice();														//清屏 
		//抓图并在程序中放置 	
		putimage(200, yofback1, gameback1);									//显示游戏背景1 
		putimage(200, yofback2, gameback2);						        	//显示游戏背景2 
		putimage_withalpha(NULL,wd_back,0,-50);								//显示文字背景 
		putimage_withalpha(NULL, imgofplane, ximgofplane, yimgofplane); 	//显示飞机
	 	
		//显示子弹
		for ( int i = ammo; i>=0; i-- ) {
			if( bullet[i].s==1  ) {
				if( bullet[i]._up ) putimage_withalpha(NULL,bullet3,bullet[i].x,bullet[i].y);
				else putimage_withalpha(NULL, bullet1, bullet[i].x, bullet[i].y); 
			}
							 						
		}
			
		//显示敌机 
		for ( int i = 0; i<numofenm_now; i++ ) {
			//这个位置有敌机且未被消灭,显示敌机 
			if( enemy[i].s==1 &&  enemy[i].iskilled == 0 ) {
				putimage_withalpha(NULL, enemy1, enemy[i].x, enemy[i].y);
				//显示在画布中的敌机的子弹 
				for ( int j = 0; j<ammo_enm; j++) {
					if ( enemy[i].blt[j].s_blt == 1 ) putimage_withalpha(NULL, bullet_enm, enemy[i].blt[j].blt_enm_x, enemy[i].blt[j].blt_enm_y);
				}
			} 
			//这个位置有敌机但已被消灭，显示爆炸动画 
			if( enemy[i].s==1  && enemy[i].iskilled == 1 ) {
				boom_gif( enemy[i].x, enemy[i].y, i );
			}
		}
		
		//显示升级包 
		if ( isupexist ) putimage_withalpha(NULL,img_update,x_up,y_up);
			 
	//文字相关的显示
	setbkmode(TRANSPARENT);			//设置文字背景色为透明	
	setcolor(BLACK);
	setfont(30,0,"黑体"); 	
	xyprintf(0, high*0.95, "得分： %d", score);
	setfont(30,0,"黑体");
	setcolor(RED);
	outtextxy(5,0,"控制 :");
	outtextxy(5,35,"W ：上");
	outtextxy(5,70,"S ：下");
	outtextxy(5,105,"A : 左");
	outtextxy(5,140,"D : 右");
	outtextxy(5,175,"P:暂停游戏");
	outtextrect(5,210,200,70,"M:打开/关闭音乐");
	setfont(35,0,"楷体"); 
	outtextxy(5,290,"血量");

	//血量显示 
	setfillcolor(YELLOW);
	bar(10,340,180,360);
	setfillcolor(BLACK);
	bar(blood_x,340,180,360);

}


//刷新背景 
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

 
//暂停
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
	
	//如果按下M，打开或关闭背景音乐
	if ( ispressM == 1 ) bgm_fight.Pause();
	if ( ispressM == -1 ) bgm_fight.Play();
		
	//如果按下p，进入暂停界面 
	if (ispressP) {
		bgm_fight.Pause();
		bgm_wait.Play();
		//定义并初始化图片 
		PIMAGE pause_bk = newimage();
		PIMAGE pause_wd = newimage();
		
		//获取图片 
		getimage(pause_bk,"图像/pause_back.png");
		getimage(pause_wd,"图像/pause.png");
		
		//设置图片大小
		setimage(wide,high,pause_bk);
		setimage(200,150,pause_wd);
		
		//显示暂停画面 
		cleardevice();
		putimage_withalpha(NULL,pause_bk,0,0);
		putimage_withalpha(NULL,pause_wd,400,200);
		
		setfont(30,0,"宋体");
		setcolor(BLUE);
		xyprintf(380,350,"按M关闭/打开音乐");
		setfont(20,0,"黑体");
		outtextrect(200,400,600,200,"消灭一架敌机得10分，逃走一架敌机扣1分; 与敌机相撞飞机会直接坠毁，受到敌方攻击会减少生命值；随着时间的增加，敌人数量也会增加，在坠毁之前尽量多拿分吧！加油，特种兵"); 
		
		int isPause = -1;
		//死循环,按p退出 
		while (1) {	
			//检测按键信息	
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

//游戏失败时的选择画面
void onceagain () {
	
			bgm_fight.Stop();
			cleardevice();
		
			putimage(0,0,Gameover);
		
			setfont(50,0,"宋体");

			setcolor(YELLOW);
		
			xyprintf(270,400,"你的得分是： %d",score);
		
			outtextxy(150,500,"按R键重新开始，按Q返回主菜单");
		
			key_msg keymsg_choose = {0};

			int press_which;
		
			while ( 1 ) {
				if (kbmsg()) {	
					keymsg_choose = getkey();
					//按Q返回主界面 
					if ( keymsg_choose.msg == key_msg_down && keymsg_choose.key == 'Q') {
						press_which = 0;
						break;
				}
					//按R重新开始游戏 
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
		
		//获取音频 
		bgm_fight.OpenFile("C:/Users/Hasee/Documents/小飞机/Venom.mp3");
		bgm_wait.OpenFile("C:/Users/Hasee/Documents/小飞机/BinaryStar.mp3");
		sound_hit.OpenFile("C:/Users/Hasee/Documents/小飞机/hit.mp3");
		crashed.OpenFile("sound_click.Play(0);crashed.mp3");
		sound_click.OpenFile("C:/Users/Hasee/Documents/小飞机/click.mp3");
}

//游戏数据初始化 
void game_init() {
	
	//分数初始化
	score = 0;
	 
	//飞机复活
	isExploded = 0; 
	 
	//飞机位置初始化
	ximgofplane = wide*0.5 - sizeofplane/2 + 100; 
	yimgofplane = high - sizeofplane;
	
	//子弹初始化 
	for ( int i = 0; i<ammo; i++ ) {
		bullet[i].s = 0;
		bullet[i]._up = 0;
	}  //遍历子弹结构数组
			
	//敌机初始化 
	for ( int i = 0; i<numofenm; i++) {
		enemy[i].s = 0;
		enemy[i].iskilled = 0;
		enemy[i].pboom = 0; 		//默认从第一张爆炸图开始播放
		enemy[i].blood = 3; 		//敌机血量为3
		for (int j = 0; j<ammo_enm; j++) enemy[i].blt[j].s_blt = 0;
	} //遍历敌机结构数组
	 
	//升级包初始化
	x_up = random(700) + 200;
	y_up = -100;
	isupdated = 0;
	isupexist = false;
	isrefresh = false;
	spdofblt = 3.0;
	
	//血量初始化
	blood_x = 180; 
}

//检测是否被敌机子弹攻击 
void isattacked() {
	//对存在的敌机进行检测 
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

//随着时间增加敌机数量 
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


