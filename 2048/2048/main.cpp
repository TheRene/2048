//
//  main.cpp
//  2048
//
//  Created by Pranav Rs on 24/06/20.
//  Copyright © 2020 Pranav Rs. All rights reserved.
//

#include <iostream>
#include<cstdlib>
#include<ctime>
using namespace std;
#define DIM 8
void clearScreen()
{
	int counter;
	counter=10;
	while(counter--)
		cout<<endl;
}
void startMenu(int &size)
{
	int option;
	while(1)
	{
		cout<<"Choose game mode!\n";
		cout<<"1. Tiny(3x3)\n";
		cout<<"2. Classic(4x4)\n";
		cout<<"3. Big(5x5)\n";
		cout<<"4. Bigger(6x6)\n";
		cout<<"5. Huge(8x8)\n";
		cin>>option;
		switch (option)
		{
			case 1:
				size=3;
				return;
			case 2:
				size=4;
				return;
			case 3:
				size=5;
				return;
			case 4:
				size=6;
				return;
			case 5:
				size=8;
				return;
			default:
				cout<<"Invalid option!!"<<endl;
		}
	}
}
//initialize the elements of gameMatrix and copyMatrix to 0
void initialize(int matrix[DIM][DIM],int size)
{
	int rowLooper,colLooper;
	for(rowLooper=0;rowLooper<size;rowLooper++)
		for(colLooper=0;colLooper<size;colLooper++)
			matrix[rowLooper][colLooper]=0;
}
//print the matrix
void output(int matrix[DIM][DIM],int size)
{
	int rowLooper,colLooper;
	for(rowLooper=0;rowLooper<size;rowLooper++,cout<<endl)
		for(colLooper=0;colLooper<size;colLooper++)
			cout<<matrix[rowLooper][colLooper]<<" ";
	cout<<endl;
}
//to select a random square
void generateRowCol(int gameMatrix[DIM][DIM],int size,int &row,int &col, int freeSpaces)
{
	int randomNum,rowLooper,colLooper;
	sranddev();
	randomNum=1+(rand()%freeSpaces);
	for(rowLooper=0;rowLooper<size && randomNum!=0;rowLooper++)
		for(colLooper=0;colLooper<size;colLooper++)
		{
			if(gameMatrix[rowLooper][colLooper])
				continue;
			if(randomNum==1)
			{
				row=rowLooper;
				col=colLooper;
				return;
			}
			randomNum--;
		}
}
//fill the random square that we get from generateRowCol()
void fillRandomPlace(int gameMatrix[DIM][DIM],int size,int &freeSpaces)
{
	int row,col;
	generateRowCol(gameMatrix, size, row, col, freeSpaces);
	freeSpaces--;
	gameMatrix[row][col]=2;
}
//check whether a given row or column is within the gameMatrix
//will be used in checkNeighbours()
int valid(int size,int row,int col)
{
	if(row<0||row>=size)
		return 0;
	if(col<0||col>=size)
		return 0;
	return 1;
}
//nested loops to check whether a given square can merge
//with one of it's neighbours
int checkNeighbours(int gameMatrix[DIM][DIM],int size,int row,int col)
{
	int rowLooper,colLooper;
	
	for(rowLooper=-1;rowLooper<2;rowLooper++)
	{
		for(colLooper=-1;colLooper<2;colLooper++)
		{
			if((rowLooper!=0 && colLooper!=0) || (rowLooper==0&&colLooper==0))
				continue;
			if(!valid(size,row+rowLooper,col+colLooper))
				continue;
			if(gameMatrix[row+rowLooper][col+colLooper]==gameMatrix[row][col])
				return 1;
		}
	}
	return 0;
}
//check whether any square can merge
int checkPossibleMoves(int gameMatrix[DIM][DIM],int size)
{
	int rowLooper,colLooper;
	for(rowLooper=0;rowLooper<size;rowLooper++)
	{
		for(colLooper=0;colLooper<size;colLooper++)
		{
			if(checkNeighbours(gameMatrix, size, rowLooper, colLooper))
				return 1;
		}
	}
	return 0;
}
//check whether any moves are left
int checkGameOver(int gameMatrix[DIM][DIM],int size,int freeSpaces)
{
	if(freeSpaces)
		return 1;
	int rowLooper,colLooper;
	for(rowLooper=0;rowLooper<size;rowLooper++)
		for(colLooper=0;colLooper<size;colLooper++)
		{
			if(checkPossibleMoves(gameMatrix,size))
				return 1;
		}
	return 0;
}
//check whether a number lies within two limits(closed interval)
//used in swipeUpDown()
int checkLimits(int limitOne,int limitTwo,int num)
{
	if(num<=limitOne && num>=limitTwo)
		return 1;
	if(num>=limitOne && num<=limitTwo)
		return 1;
	return 0;
}
void swap(int &num1,int &num2)
{
	int temp;
	temp=num1;
	num1=num2;
	num2=temp;
}
void transpose(int gameMatrix[DIM][DIM],int size)
{
	int rowLooper,colLooper;
	
	for(rowLooper=0;rowLooper<size;rowLooper++)
	{
		for(colLooper=rowLooper;colLooper<size;colLooper++)
		{
			swap(gameMatrix[rowLooper][colLooper],gameMatrix[colLooper][rowLooper]);
		}
	}
}
//reflects the necessary changes in the gameMatrix when we swipe UP or DOWN
//UP or DOWN is determined by startRow,endRow and rowDiff
void swipeUpDown(int gameMatrix[DIM][DIM],int size,int startRow,int endRow,int rowDiff,int &score,int &freeSpaces)
{
	int rowLooper,colLooper,checkLooper;
	//for every element in each column,check whether it will merge
	for(colLooper=0;colLooper<size;colLooper++)
	{
		for(rowLooper=startRow;checkLimits(startRow, endRow, rowLooper+rowDiff);rowLooper+=rowDiff)
		{
			if(!gameMatrix[rowLooper][colLooper])
				continue;
			for(checkLooper=rowDiff;checkLimits(startRow, endRow, rowLooper+checkLooper);checkLooper+=rowDiff)
			{
				if(gameMatrix[rowLooper][colLooper]==gameMatrix[rowLooper+checkLooper][colLooper])
				{
					gameMatrix[rowLooper][colLooper]*=2;
					gameMatrix[rowLooper+checkLooper][colLooper]=0;
					score+=gameMatrix[rowLooper][colLooper];
					//if we merge two squares,the freeSpaces count must increase by 1
					freeSpaces++;
					break;
				}
			}
		}
	}
	//for every 0 in each column,find the nearest non-zero element and move that element here
	for(colLooper=0;colLooper<size;colLooper++)
	{
		for(rowLooper=startRow;checkLimits(startRow, endRow, rowLooper+rowDiff);rowLooper+=rowDiff)
		{
			if(gameMatrix[rowLooper][colLooper]!=0)
				continue;
			for(checkLooper=rowDiff;checkLimits(startRow, endRow, rowLooper+checkLooper);checkLooper+=rowDiff)
			{
				if(gameMatrix[rowLooper+checkLooper][colLooper])
				{
					gameMatrix[rowLooper][colLooper]=gameMatrix[rowLooper+checkLooper][colLooper];
					gameMatrix[rowLooper+checkLooper][colLooper]=0;
					break;
				}
			}
		}
	}
}
//reflects the changes in gameMatrix for a move in any of the four directions
//if the move is RIGHT,applying the following operations,
//transpose,swipe DOWN,transpose
//will get us the necessary matrix
//similarly,if the move is LEFT,
//do transpose,swipe UP,transpose
//identify RIGHT and LEFT by the boolean flag variable
void swipeGeneralizer(int gameMatrix[DIM][DIM],int size,int start,int end,int diff,int &score,int &freeSpaces,bool flag=false)
{
	if(flag)
	{
		transpose(gameMatrix, size);
		swipeUpDown(gameMatrix, size, start, end, diff,score,freeSpaces);
		transpose(gameMatrix, size);
		return;
	}
	swipeUpDown(gameMatrix, size, start, end, diff,score,freeSpaces);
}
void endGame(int gameMatrix[DIM][DIM], int size, int score)
{
	clearScreen();
	output(gameMatrix, size);
	cout<<"Game over!!"<<endl;
	cout<<"Your score is "<<score;
}
void copy(int matrix[DIM][DIM],int copyMatrix[DIM][DIM],int size)
{
	int rowLooper,colLooper;
	for(rowLooper=0;rowLooper<size;rowLooper++)
		for(colLooper=0;colLooper<size;colLooper++)
			copyMatrix[rowLooper][colLooper]=matrix[rowLooper][colLooper];
}
// oompare two matrices
//if a move doesn't cause any changes in the gameMatrix,
//we shouldn't generate the value 2 in a random square
int compare(int gameMatrix[DIM][DIM],int copy[DIM][DIM],int size)
{
	int rowLooper,colLooper;
	for(rowLooper=0;rowLooper<size;rowLooper++)
		for(colLooper=0;colLooper<size;colLooper++)
			if(gameMatrix[rowLooper][colLooper]-copy[rowLooper][colLooper])
				return 0;
	return 1;
}
/*
 First,we have an empty matrix,where we generate 2 on a random square
 1.Each turn,we get the user's move,and implement it.
 2.If the move causes changes in the gameMatrix,
   generate 2 on a random square and repeat step 1.
 3. Else,the move doesn't cause any changes in the gameMatrix,
	which means that we shouldn't generate 2 on a random square.
 Repeat steps 1 to 3 till the game is over
 */
int main()
{
	int gameMatrix[DIM][DIM],copyMatrix[DIM][DIM],size;
	int freeSpaces, score;
	char choice;
	
	score=0;
	startMenu(size);
	freeSpaces=size*size;
	initialize(gameMatrix,size);
	initialize(copyMatrix,size);
	fillRandomPlace(gameMatrix, size, freeSpaces);
	copy(gameMatrix,copyMatrix,size);
	do
	{
		cout<<endl;
		output(gameMatrix, size);
		copy(gameMatrix,copyMatrix,size);
		cin>>choice;
		switch(choice)
		{
			case 'w':
			case 'W':
				swipeGeneralizer(gameMatrix, size, 0, size-1, 1,score,freeSpaces);
				break;
			case 'a':
			case 'A':
				swipeGeneralizer(gameMatrix, size, 0, size-1, 1,score,freeSpaces,true);
				break;
			case 's':
			case 'S':
				swipeGeneralizer(gameMatrix, size, size-1, 0, -1,score,freeSpaces);
				break;
			case 'd':
			case 'D':
				swipeGeneralizer(gameMatrix, size, size-1, 0, -1,score,freeSpaces,true);
				break;
			default:
				clearScreen();
				cout<<"Enter valid option!!\n";
				break;
		}
		if(compare(gameMatrix, copyMatrix, size))
			continue;
		fillRandomPlace(gameMatrix, size, freeSpaces);
	}while(checkGameOver(gameMatrix, size, freeSpaces));
	
	output(gameMatrix, size);
	endGame(gameMatrix,size,score);
	
	return 0;
}

