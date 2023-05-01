// Author: Luke Treanor-Taylor
// Date: 27/04/2023
// Coursework for SET08122

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include <stdlib.h>

void printSquare(char str[4], int twoDimArray[9][9]);
bool checkValid(char str[4]);
bool columnCheck(int row, int column, int twoDimArray[9][9], int num);
bool rowCheck(int row, int column, int twoDimArray[9][9], int num);
bool blockCheck(int row, int column, int twoDimArray[9][9], int num);
bool solve(int row, int column, int twoDimArray[9][9]);
void recordGame(struct stack *s, int startingRecord[81]);
int generateBoard(int difficulty, int twoDimArray[9][9]);
void removeNumbers(int difficulty, int twoDimArray[9][9]);


// Stack Functions
void init_stack(struct stack *s);
void push(struct stack *s, char move[4]);
const char *pop(struct stack *s);

struct stack
{
	char array[200][4];
	int top;
};

int main(int argc, char **argv)
{
	struct stack s;
	init_stack(&s);
	
	int twoDimArray[9][9];
	for(int i = 0; i < 9; i++){      // Initialising Two Dimensional Array
		for(int j = 0; j < 9; j++){
			twoDimArray[i][j] = 0;
		}
	}
	
	// Initial Menu
	system("cls");
	int diffFlag = 0;
	char modeTemp[1];
	while(diffFlag == 0){
		printf("There are three game modes: Easy, Medium & Hard");
		printf("\nFor easy enter 1, medium enter 2, and hard enter 3");
		printf("\n\nEnter option)");
		
		scanf("%s", modeTemp);
		
		if(modeTemp[0] == '1' || modeTemp[0] == '2' || modeTemp[0] == '3') {
			diffFlag = 1;
		}
		else {
			printf("\nInput invalid");
		}
	}
	int startingRecord[81];
	
	int difficulty = modeTemp[0] - '0';
	if(generateBoard(difficulty, twoDimArray) == 1) {
		int staRecInc = 0;
		for(int i = 0; i < 9; i++){      // Recording Starting Record
			for(int j = 0; j < 9; j++){
				startingRecord[staRecInc] = twoDimArray[i][j];
				staRecInc++;
			}
		}
		printSquare("    ", twoDimArray);
	}
	else {
		printf("\nFAILED");
	}
	printf("\n");

	int whileFlag = 1;
	while( whileFlag == 1){ // Game loop
		char temp[100];
		char coor[2];
		int num = 0;
		
		printf("\n\n Game Controls: 0 = Exit, 1 = Solve, u = Undo Move, r = Redo Move ");
		printf("\n To enter coordinate and number enter in the format: A2=4");
		printf("\n\n Enter input: ");
		scanf("%s", temp);
		if(temp[0] == '0')
		{
			whileFlag = 0;
			continue;
		}
		
		if(temp[0] == '1'){
			if(solve(0, 0, twoDimArray))
			{
				printSquare("    ", twoDimArray);
				whileFlag = 0;
				continue;
			}
			else {
				printf("Mistake was made somewhere, Sudoku unsolvable.");
				continue;
			}
		}
		if(temp[0] == 'u'){
			char *undoMove = "";
			undoMove = (char*)pop(&s);
			if(undoMove == "NONE"){
				continue;
			}
			if(checkValid(undoMove) ){
				undoMove[3] = '0';
				printSquare(undoMove, twoDimArray);
			}
			continue;
		}
		if(temp[0] == 'r'){
			s.top++;
			char *redoMove = "";
			redoMove = (char*)pop(&s);
			if(checkValid(redoMove)){
				printSquare(redoMove, twoDimArray);
				s.top++;
			}
			else {
				printf("\nRedo not valid");
				s.top--;
			}
			continue;
		}
		
		if(checkValid(temp)){
			push(&s, temp);
			printSquare(temp, twoDimArray);
		}
		else {
			printf("\n Check format is correct for the input eg capital letter etc");
		}
		
	}
	recordGame(&s, startingRecord);
	int length = s.top;
	
	printf("\n");

	return 0;
}

void printSquare(char str[4], int twoDimArray[9][9])
{
	//char oneone[] = "    ___ ";
	//char onetwo[] = "___ ";
	//char twoone[] = "|   |";
	//char twotwo[] = "   |";
	//char threeone[] = "\n   |___|";
	//char threetwo[] = "___|";
	
	system("cls");
	printf("\n     1   2   3    4   5   6    7   8   9\n");
	
	int tDAone = str[0] - 'A';
	int tDAtwo = str[1] - '1';
	
	twoDimArray[tDAone][tDAtwo] = str[3] - '0';


	char z = 'A';
	for(int i = 0; i < 9; i++){
		
		if(i == 0){
			for(int j = 0; j < 9; j++){
				if(j == 0){
					printf("    ___ "); // oneone
				}
				else {
					printf("___ "); // onetwo
				}
				if((j+1) % 3 == 0 && j < 8) {
					printf(" ");
				}
			}
		}
		for(int j = 0; j < 9; j++){
			if(j == 0){
				if(twoDimArray[i][j] == 0) {
					printf("\n %c |   |", z); // twoone
				}
				else {
					printf("\n %c | %d |", z, twoDimArray[i][j]); // twoone
				}
				
			}
			else {
				if(twoDimArray[i][j] == 0) {
					printf("   |"); // twotwo
				}
				else {
					printf(" %d |", twoDimArray[i][j]); // twotwo
				}
				
			}
			if((j+1) % 3 == 0 && j < 8) {
				printf("|");
			}
		}
		for(int j = 0; j < 9; j++){
			if(j == 0){
				printf("\n   |___|"); // threeone
			}
			else {
				printf("___|"); // threetwo
			}
			if((j+1) % 3 == 0 && j < 8) {
				printf("|");
			}
		}
		if((i+1) % 3 == 0 && i < 8) {
			printf("\n    ___ ___ ___  ___ ___ ___  ___ ___ ___ ");
		}
		z = z + 1;
	}
}

bool checkValid(char str[4])
{
	bool valid = true;
	if(!(str[0] >= 'A' && str[0] <= 'I')){
		valid = false;
	}
	if(!(str[1] >= '1' && str[1] <= '9')){
		valid = false;
	}
	if(str[2] != '='){
		valid = false;
	}
	if(!(str[3] >= '1' && str[3] <= '9')){
		valid = false;
	}
	
	return valid;
}

bool columnCheck(int row, int column, int twoDimArray[9][9], int num)
{
	for(int i = 0; i < 9; i++){
		if(twoDimArray[i][column] == num){
			return false;
		}
	}
	return true;
}

bool rowCheck(int row, int column, int twoDimArray[9][9], int num)
{	
	for(int i = 0; i < 9; i++){
		if(twoDimArray[row][i] == num){
			return false;
		}
	}
	return true;
}

bool blockCheck(int row, int column, int twoDimArray[9][9], int num)
{
	int startColumn = 0;
	startColumn = column - (column % 3);
	int startRow = 0;
	startRow = row - (row % 3);
	
	for(int i = row - (row % 3); i < row - (row % 3) + 3; i++)
	{
		for(int j = column - (column % 3); j < (column - (column % 3) + 3); ++ j)
		{
			if(twoDimArray[i][j] == num)
			{
				return false;
			}
		}
	}

	return true;
}

bool solve(int row, int column, int twoDimArray[9][9])
{	
	if(row == 8 && column == 9)
	{
		return true;
	}
	
	if(column == 9)
	{
		row++;
		column = 0;
	}
	
	if(twoDimArray[row][column] != 0)
	{
		column++;
		return solve(row, column, twoDimArray); 
	}
	
	for(int n = 1; n < 10; n++) {
		if(rowCheck(row, column, twoDimArray, n) && columnCheck(row, column, twoDimArray, n) && blockCheck(row, column, twoDimArray, n))
		{
			twoDimArray[row][column] = n;
			
			if(solve(row, column + 1, twoDimArray)) 
			{
				return true;
			}
		}
		
		twoDimArray[row][column] = 0;
	}
	return false;
}

void init_stack(struct stack *s)
{
	s->top = -1;
}

void push(struct stack *s, char move[4])
{
	if(s->top == 199)
	{
		printf("Stack is full. Couldn't push %s onto stack\n", move);
		return;
	}
	s->top++;
	for(int i = 0; i < 4; i++){
		s->array[s->top][i] = move[i];
	}
}

const char *pop(struct stack *s)
{
	char *move = "";
	if(s->top == -1)
	{
		printf("Stack is empty\n");
		return "NULL\0";
	}
	for(int i = 0; i < 4; i++){
		move[i] = s->array[s->top][i];
	}
	move[5] = '\0';
	s->top--;
	return move;
}

void recordGame(struct stack *s, int startingRecord[81])
{
	FILE *output;
	output = fopen("SudokuRecord.csv", "a");
	
	fprintf(output, "\n");
	for(int i = 0; i <= 81; i++){
		int a = startingRecord[i];
		fprintf(output, "%d,", a);
	}
	int length = s->top;
	for(int i = 0; i < length+1; i++) {
		const char *move = pop(s);
		fprintf(output, "%s,", move);
	}
}

int generateBoard(int difficulty, int twoDimArray[9][9])
{
	srand(time(NULL)); 
	
	
	for(int i = 0; i < 4; i++) {
		for(int j = 0; i < 4; i++) {
			int randValid = 0;
			while(randValid == 0) {
				int n = rand() % 10;
				if(rowCheck(i, j, twoDimArray, n) && columnCheck(i, j, twoDimArray, n) && blockCheck(i, j, twoDimArray, n)) {
					twoDimArray[i][j] = n;
					randValid = 1;
				}
			}
		}
	}
	
	for(int i = 4; i < 7; i++) {
		for(int j = 4; i < 7; i++) {
			int randValid = 0;
			while(randValid == 0) {
				int n = rand() % 10;
				if(rowCheck(i, j, twoDimArray, n) && columnCheck(i, j, twoDimArray, n) && blockCheck(i, j, twoDimArray, n)) {
					twoDimArray[i][j] = n;
					randValid = 1;
				}
			}
		}
	}
	
	for(int i = 7; i < 10; i++) {
		for(int j = 7; i < 10; i++) {
			int randValid = 0;
			while(randValid == 0) {
				int n = rand() % 10;
				if(rowCheck(i, j, twoDimArray, n) && columnCheck(i, j, twoDimArray, n) && blockCheck(i, j, twoDimArray, n)) {
					twoDimArray[i][j] = n;
					randValid = 1;
				}
			}
		}
	}
	
	if(solve(0, 0, twoDimArray)) {
		removeNumbers(difficulty, twoDimArray);
	}
	else {
		return 0;
	}
	
	return 1;
}

void removeNumbers(int difficulty, int twoDimArray[9][9]){
	if(difficulty == 1) {
		int removedFlag = 0;
		while(removedFlag < 30) {
			int r = (rand() % 10) - 1;
			int c = (rand() % 10) - 1;
			if(twoDimArray[r][c] != 0) {
				twoDimArray[r][c] = 0;
				removedFlag++;
			}
		}
	}
	if(difficulty == 2) {
		int removedFlag = 0;
		while(removedFlag < 40) {
			int r = (rand() % 10) - 1;
			int c = (rand() % 10) - 1;
			if(twoDimArray[r][c] != 0) {
				twoDimArray[r][c] = 0;
				removedFlag++;
			}
		}
	}
	if(difficulty == 3) {
		int removedFlag = 0;
		while(removedFlag < 50) {
			int r = (rand() % 10) - 1;
			int c = (rand() % 10) - 1;
			if(twoDimArray[r][c] != 0) {
				twoDimArray[r][c] = 0;
				removedFlag++;
			}
		}
	}
}