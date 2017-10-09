#include<stdio.h>
#include<stdlib.h>

#define BASKETS_FILE "baskets.csv"
#define OUTPUT_FILE "output.csv"
#define LIKE_THRESHOLD 4.0
#define SIZE 10
int main() {

	FILE * fpbaskets;

	fpbaskets = fopen(BASKETS_FILE, "r");

	if (fpbaskets == NULL) {
		printf("file open error!\n");
		return -1;
	}

	int movie_showed[SIZE];
	int n;
	int predict_movie;

	printf("�Է��� n���� movie�� �� (������ threshold �̻�) ��� �� �Է��� movie�� �� ���.\n ");
	printf("Input the n (n <= %d) : ", SIZE);
	scanf("%d", &n);
	int i;
	printf("Input %d movie number(s)\n", n);
	for (i = 0; i < n; i++) {
		scanf("%d", &movie_showed[i]);
	}
	printf("Input a movie number to predict : ");
	scanf("%d", &predict_movie);

	printf("\n��� ��...\n");

	FILE * fpoutput;
	fpoutput = fopen(OUTPUT_FILE, "w");
	if (fpoutput == NULL) {
		printf("file open error!\n");
		return -1;
	}

	fprintf(fpoutput, "USER,");
	for (i = 0; i < n; i++) {
		fprintf(fpoutput, "MV. %d,", movie_showed[i]);
	}
	fprintf(fpoutput, "pred. %d\n", predict_movie);

	int pass_user = 0;	//pass �ؾ��ϴ� user�� ���
	int check_user = 0;	//n�� �� ������ Ȱ��ȭ
	int old_user = 0; //user

	int is_predict_movie = 0;	//predict movie�� �ִ��� .;.;.;

	int num_movie_showed = 0;

	int user, movie;
	float like;

	int print_check = 0;
	while (!feof(fpbaskets)) {
		fscanf(fpbaskets, "%d,%d,%f", &user, &movie, &like);
		if (old_user != user) { //user �� �ٲ���
			if (num_movie_showed == n && is_predict_movie == 0) {
				fprintf(fpoutput, "X\n");
			}
			is_predict_movie = 0;
			num_movie_showed = 0;
			old_user = user;
			print_check = 0;
		}

		//pass �ؾ��ϴ� user
		if (pass_user == user) {
			continue;
		}


		//predict movie check.
		if (movie == predict_movie) {
			if (like >= LIKE_THRESHOLD)
				is_predict_movie = 1;
			else
				is_predict_movie = -1;
		}

		//movie_showed check
		if (num_movie_showed < n) {
			for (i = 0; i < n; i++) {
				if (movie_showed[i] == movie) {
					if (like < LIKE_THRESHOLD) {
						pass_user = user;
					}
					else
						num_movie_showed++;
				}
			}
		}

		//if all movie showed -> print
		if (num_movie_showed == n && print_check == 0) {
			fprintf(fpoutput, "%d,", user);
			for (i = 0; i < n; i++) {
				fprintf(fpoutput, "O,");
			}
			print_check = 1;
		}
		//n���� �� ���� user
		if (num_movie_showed == n) {
			if (is_predict_movie == 1) {
				printf("1\n");
				fprintf(fpoutput, "O\n");
				pass_user = user;
			}
			else if (is_predict_movie == -1) {
				fprintf(fpoutput, "X\n");
				pass_user = user;
			}
		}
	}

	fclose(fpbaskets);
	fclose(fpoutput);
	printf("Success! Check the output file\n");
	return 0;

}