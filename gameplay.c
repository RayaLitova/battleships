#include "defines.h"

extern struct ship_t ships_A[10];
extern struct ship_t ships_B[10];
extern int sizeA, sizeB; 
extern int mode, is_hard;

struct tile_t **map_A_base;
struct tile_t **map_B_base;

struct tile_t **map_A;
struct tile_t **map_B;
int turn=1;

int last_fire_Bx=0;
int last_fire_By=0;
int last_fire_Ax=0;
int last_fire_Ay=0;

int ships_left_A=10, ships_left_B=10;


struct tile_t** surround_ship(int x, int y, char direction, int type, struct tile_t** map){
	int dirx, diry;
	char maindir;
	if(direction=='l') dirx=-1, diry=0, maindir='x';
	if(direction=='r') dirx=1,  diry=0, maindir='x';
	if(direction=='u') dirx=0,  diry=-1, maindir='y';
	if(direction=='d') dirx=0,  diry=1, maindir='y';

	for(int i=0; i<type; i++){
		if(i==0 || i==type-1){
			if(maindir=='x'){
				if(i==type-1) dirx*=-1;
				if(x-dirx>=0 && x-dirx<=9){
					if(y-dirx>=0 && y-dirx<=9){
						map[y-dirx][x-dirx].symbol='O';
						map[y-dirx][x-dirx].value=1;
					}
					if(y>=0 && y<=9){ 
						map[y][x-dirx].symbol='O';
						map[y][x-dirx].value=1;
					}
					if(y+dirx>=0 && y+dirx<=9){
						map[y+dirx][x-dirx].symbol='O';
						map[y+dirx][x-dirx].value=1;
					}
				}
				if(i==type-1) dirx*=-1;
			}
			if(maindir=='y'){
				if(i==type-1) diry*=-1;
				if(y-diry>=0 && y-diry<=9){
					if(x-diry>=0 && x-diry<=9){
						map[y-diry][x-diry].symbol='O';
						map[y-diry][x-diry].value=1;
					}
					if(x>=0 && x<=9){
						map[y-diry][x].symbol='O';
						map[y-diry][x].value=1;
					}
					if(x+diry>=0 && x+diry<=9){
						map[y-diry][x+diry].symbol='O';
						map[y-diry][x+diry].value=1;
					}
				}
				if(i==type-1) diry*=-1;
			}
		}
		if(y+dirx>=0 && y+dirx<=9){
			if(x+diry>=0 && x+diry<=9){
				map[y+dirx][x+diry].symbol='O';
				map[y+dirx][x+diry].value=1;
			}
		}
		if(y-dirx>=0 && y-dirx<=9){
			if(x-diry>=0 && x-diry<=9){
				map[y-dirx][x-diry].symbol='O';
				map[y-dirx][x-diry].value=1;
			}
		}
		x+=dirx;
		y+=diry;
	}
	return map;
}



void play(){
	map_A=create_empty_map();
	map_B=create_empty_map();
	int shoot=0;
	while(turn){
		printf("\nPlayer %d:\n", turn);
		if(turn == 1) printf("Fire at specified position(1)\nFire at last position - %c%d (2)\nSee map(3)\n", last_fire_Ax+'A',last_fire_Ay+1);
		if(turn == 2) printf("Fire at specified position(1)\nFire at last position - %c%d (2)\nSee map(3)\n", last_fire_Bx+'A',last_fire_By+1);
		scanf("%d", &shoot);
		if(shoot==1){
			spec_pos();
		}else if(shoot==2){
			fire_last();

		}else if(shoot==3){
			if(turn==1) print_map(map_A);
			else print_map(map_B);
		}else{
			printf("Invalid option!\n");
		}
	}
}

int check_ship(int x, int y){
	struct ship_t* temp=find_ship(x,y,turn);
	if(temp->hit==temp->type){
		return 1;
	}else{
		return 0;
	}
}


int min(int a, int b){
	if(a>b) return b;
	return a;
}

int max(int a, int b){
	if(a>b) return a;
	return b;
}


struct ship_t *find_ship(int x, int y, int turn){
	if(turn==2){
		for(int i=0;i<sizeA;i++){
			if(x>=min(ships_A[i].startx,ships_A[i].endx) && x<=max(ships_A[i].startx,ships_A[i].endx)){
				if(y>=min(ships_A[i].starty,ships_A[i].endy) && y<=max(ships_A[i].starty,ships_A[i].endy)){
					return &ships_A[i];
				}
			}
		}
	}else{
		for(int i=0;i<sizeB;i++){
			if(x>=min(ships_B[i].startx,ships_B[i].endx) && x<=max(ships_B[i].startx,ships_B[i].endx)){
				if(y>=min(ships_B[i].starty,ships_B[i].endy) && y<=max(ships_B[i].starty,ships_B[i].endy)){
					return &ships_B[i];
				}
			}
		}
	}
	return 0;
}


void win(){
	printf("The winner is player %d!\n", turn);
	if(mode==1 && is_hard && turn==2) printf("GG 2 EZ <3\n");
	if(mode==1 && is_hard && turn!=2) printf("Why u hacking bro :'((\n");
	turn=0;
}

void fix_last(int *x,int *y){
	if(*x>9) *x-=1;
	if(*x<0) *x+=1;
	if(*y>9) *y-=1;
	if(*y<0) *y+=1;
}

void fire_last(){
	char direction;
	do{
		printf("Choose direction Up(u), Down(d), Left(l), Right(r) ");
		scanf(" %c", &direction);
		if(direction!='d' && direction!='u' && direction!='l' && direction!='r') printf("No such direction\n");
	}while(direction!='d' && direction!='u' && direction!='l' && direction!='r');

	if(turn==1){
		if(direction=='r'){
			last_fire_Ax++;
			if(last_fire_Ax>9){
				printf("You're going out of course!\n");
				last_fire_Ax--;
				return;
			}
			while((last_fire_Ax>=0 && last_fire_Ax<=9) && map_A[last_fire_Ay][last_fire_Ax].value>0) last_fire_Ax++;
    		
    		if((last_fire_Ay>=0 && last_fire_Ay<=9) && (last_fire_Ax>=0 && last_fire_Ax<=9)){
	        	fire(last_fire_Ax,last_fire_Ay);
	        }else{
	        	fix_last(&last_fire_Ax,&last_fire_Ay);
	        	printf("You're going out of course!\n");
	        	return;
	        }
    	}
	    if(direction=='l'){
	    	last_fire_Ax--;
	    	if(last_fire_Ax<0){
	    		last_fire_Ax++;
				printf("You're going out of course!\n");
				return;
			}
			while((last_fire_Ax>=0 && last_fire_Ax<=9) && map_A[last_fire_Ay][last_fire_Ax].value>0) last_fire_Ax--;
	        
	        if((last_fire_Ay>=0 && last_fire_Ay<=9) && (last_fire_Ax>=0 && last_fire_Ax<=9)){
	        	fire(last_fire_Ax,last_fire_Ay);
	        }else{
	        	fix_last(&last_fire_Ax,&last_fire_Ay);
	        	printf("You're going out of course!\n");
	        	return;
	        }
	    }
	    if(direction=='d'){
	    	last_fire_Ay++;
	    	if(last_fire_Ay>9){
				printf("You're going out of course!\n");
				last_fire_Ay--;
				return;
			}
			while((last_fire_Ay>=0 && last_fire_Ay<=9) && map_A[last_fire_Ay][last_fire_Ax].value>0) last_fire_Ay++;

			if((last_fire_Ay>=0 && last_fire_Ay<=9) && (last_fire_Ax>=0 && last_fire_Ax<=9)){
	        	fire(last_fire_Ax,last_fire_Ay);
	        }else{
	        	fix_last(&last_fire_Ax,&last_fire_Ay);
	        	printf("You're going out of course!\n");
	        	return;
	        }
	    }
	    if(direction=='u'){
	    	last_fire_Ay--;
	    	if(last_fire_Ay<0){
	    		last_fire_Ay++;
				printf("You're going out of course!\n");
				return;
			}
			while((last_fire_Ay>=0 && last_fire_Ay<=9) && map_A[last_fire_Ay][last_fire_Ax].value>0) last_fire_Ay--;

	        if((last_fire_Ay>=0 && last_fire_Ay<=9) && (last_fire_Ax>=0 && last_fire_Ax<=9)){
	        	fire(last_fire_Ax,last_fire_Ay);
	        }else{
	        	fix_last(&last_fire_Ax,&last_fire_Ay);
	        	printf("You're going out of course!\n");
	        	return;
	        }
	    }

	}else{
		
		if(direction=='r'){
			last_fire_Bx++;
			if(last_fire_Bx>9){
				printf("You're going out of course!\n");
				last_fire_Bx--;
				return;
			}
			while((last_fire_Bx>=0 && last_fire_Bx<=9) && map_B[last_fire_By][last_fire_Bx].value>0) last_fire_Bx++;
    		
    		if((last_fire_By>=0 && last_fire_By<=9) && (last_fire_Bx>=0 && last_fire_Bx<=9)){
	        	fire(last_fire_Bx,last_fire_By);
	        }else{
	        	fix_last(&last_fire_Bx,&last_fire_By);
	        	printf("You're going out of course!\n");
	        	return;
	        }
    	}
	    if(direction=='l'){
	    	last_fire_Bx--;
	    	if(last_fire_Bx<0){
	    		last_fire_Bx++;
				printf("You're going out of course!\n");
				return;
			}
			while((last_fire_Bx>=0 && last_fire_Bx<=9) && map_B[last_fire_By][last_fire_Bx].value>0) last_fire_Bx--;
	        
	        if((last_fire_By>=0 && last_fire_By<=9) && (last_fire_Bx>=0 && last_fire_Bx<=9)){
	        	fire(last_fire_Bx,last_fire_By);
	        }else{
	        	fix_last(&last_fire_Bx,&last_fire_By);
	        	printf("You're going out of course!\n");
	        	return;
	        }
	    }
	    if(direction=='d'){
	    	last_fire_By++;
	    	if(last_fire_By>9){
				printf("You're going out of course!\n");
				last_fire_By--;
				return;
			}
			while((last_fire_By>=0 && last_fire_By<=9) && map_B[last_fire_By][last_fire_Bx].value>0) last_fire_By++;

			if((last_fire_By>=0 && last_fire_By<=9) && (last_fire_Bx>=0 && last_fire_Bx<=9)){
	        	fire(last_fire_Bx,last_fire_By);
	        }else{
	        	fix_last(&last_fire_Bx,&last_fire_By);
	        	printf("You're going out of course!\n");
	        	return;
	        }
	    }
	    if(direction=='u'){
	    	last_fire_By--;
	    	if(last_fire_By<0){
	    		last_fire_By++;
				printf("You're going out of course!\n");
				return;
			}
			while((last_fire_By>=0 && last_fire_By<=9) && map_B[last_fire_By][last_fire_Bx].value>0) last_fire_By--;

	        if((last_fire_By>=0 && last_fire_By<=9) && (last_fire_Bx>=0 && last_fire_Bx<=9)){
	        	fire(last_fire_Bx,last_fire_By);
	        }else{
	        	fix_last(&last_fire_Bx,&last_fire_By);
	        	printf("You're going out of course!\n");
	        	return;
	        }
	    }
	}
}

void fire(int x, int y){
	if(turn==1){
		if(map_A[y][x].value>0){
			printf("This place is already on fire!\n");
			return;
		}
		if(map_B_base[y][x].value>1){
			printf("A ship was hit!\n");

			map_A[y][x].value=map_B_base[y][x].value;
			map_A[y][x].symbol='X';
			struct ship_t *temp=find_ship(x,y, turn);
			temp->hit++;
			if(check_ship(x,y)){
				printf("The ship has been sunken!\n");
				ships_left_A--;
				if(ships_left_A==0){
					win();
					return;
				}
				map_A=surround_ship(temp->startx, temp->starty, temp->direction, temp->type, map_A);
			}
		}else{

			printf("No ships were hit! :(\n");
			map_A[y][x].value=1;
			map_A[y][x].symbol='O';
			turn=2;
		}
		last_fire_Ax=x;
		last_fire_Ay=y;

	}else if(turn==2){
		if(map_B[y][x].value>0){
			if(mode!=1 && !is_hard) printf("This place is already on fire!\n");
			return;
		}
		if(map_A_base[y][x].value>1){
			if(mode==1) printf("Your ship was hit!\n");
			else printf("A ship was hit!\n");

			map_B[y][x].value=map_A_base[y][x].value;
			map_B[y][x].symbol='X';

			struct ship_t *temp=find_ship(x,y, turn);
			temp->hit++;

			if(check_ship(x,y)){
				printf("The ship has been sunken!\n");
				if(mode==1 && !is_hard) print_map(map_B);
				ships_left_B--;
				if(ships_left_B==0){
					win();
					return;
				}
				map_B=surround_ship(temp->startx, temp->starty, temp->direction, temp->type, map_B);
			}
		}else{
			if(mode!=1 && !is_hard) printf("No ships were hit! :(\n");
			map_B[y][x].value=1;
			map_B[y][x].symbol='O';
			turn=1;
		}
		last_fire_Bx=x;
		last_fire_By=y;
	}
}

void spec_pos(){
	int x,y;
	do{
		printf("\nPlayer %d:\n", turn);
		printf("Choose position(A1,B2,..) ");
		char help_c;
		scanf(" %c", &help_c);
		if(help_c>='a') x = help_c-'a';
		else x = help_c-'A';
		scanf("%d", &y);
		y-=1;
		if(x<0||x>9) printf("No such x\n");
		if(y<0||y>9) printf("No such y\n");
	}
	while(y<0||y>9||x<0||x>9);
	fire(x,y);
}

void easy_mode(){
	map_A=create_empty_map();
	map_B=create_empty_map();
	int shoot=0, j, k;
	while(turn){
		if(turn==1){
			printf("\nPlayer %d:\n", turn);
			printf("Fire at specified position(1)\nFire at last position - %c%d (2)\nSee map(3)\n", last_fire_Ax+'A',last_fire_Ay+1);
			scanf("%d", &shoot);
			if(shoot==1){
				spec_pos();
			}else if(shoot==2){
				fire_last();
			}else if(shoot==3){
				print_map(map_A);
			}else{
				printf("Invalid option!\n");
			}
		}
		else{
			j = rand() % 10;
			k = rand() % 10;
			fire(j, k);
		}
	}
}

void hard_mode(){
	map_A=create_empty_map();
	map_B=create_empty_map();
	int shoot=0, j, k;
	while(turn==1){
			printf("\nPlayer %d:\n", turn);
			printf("Fire at specified position(1)\nFire at last position - %c%d (2)\nSee map(3)\n", last_fire_Ax+'A',last_fire_Ay+1);
			scanf("%d", &shoot);
			if(shoot==1){
				spec_pos();
			}else if(shoot==2){
				fire_last();
			}else if(shoot==3){
				print_map(map_A);
			}else{
				printf("Invalid option!\n");
			}
	}
	for(j=0;j<10;j++){
		for(k=0;k<10;k++){
			turn = 2;
			fire(j, k);
		}
	}
	
	return;
}
