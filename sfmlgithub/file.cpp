#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include <iostream>
int main()
{
	char arr[10];
	int playerScore;
	FILE* fp;
	fp = fopen("scoreboard.txt", "a");
	std::cin >> arr >> playerScore;
	fprintf(fp, "%s %d\n", arr, playerScore);
	fclose(fp);

	fp = fopen("scoreboard.txt", "r");
	while (!feof(fp)) {
		char name[20];
		int score;
		fscanf(fp, "%s %d", &name, &score);
		std::cout << name << " " << score << "\n";
	}
	fclose(fp);

	return 0;
}