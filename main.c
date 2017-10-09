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
	int n = 0;
	int predict_movie;

	printf("입력한 n개의 movie를 본 (평점이 threshold 이상) 사람 중 입력한 movie를 본 사람.\n ");
	while (!(n <= SIZE && 0 < n)) {
		printf("Input the n (1<= n <= %d) : ", SIZE);
		scanf("%d", &n);
	}
	int i;
	printf("Input %d movie number(s)\n", n);
	for (i = 0; i < n; i++) {
		scanf("%d", &movie_showed[i]);
	}
	printf("Input a movie number to predict : ");
	scanf("%d", &predict_movie);

	printf("\n계산 중...\n");

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

	int pass_user = 0;	//pass 해야하는 user의 경우
	int check_user = 0;	//n개 다 맞을시 활성화
	int old_user = 0; //user

	int is_predict_movie = 0;	//predict movie가 있는지 .;.;.;

	int num_movie_showed = 0;

	int user, movie;
	float like;
	int print_check = 0;

	int total = 0;// 모든 movie_showed 가 나온 user의 수
	int num_include_predict = 0; //predict 까지 나온 user수

	while (!feof(fpbaskets)) {
		fscanf(fpbaskets, "%d,%d,%f", &user, &movie, &like);
		if (old_user != user) { //user 가 바뀐경우
			if (num_movie_showed == n ) {
				total++;
				if(is_predict_movie == 0)
					fprintf(fpoutput, "X\n");
			}
			is_predict_movie = 0;
			num_movie_showed = 0;
			old_user = user;
			print_check = 0;
		}

		//pass 해야하는 user
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
		//n개가 다 맞은 user
		if (num_movie_showed == n) {
			if (is_predict_movie == 1) {
				fprintf(fpoutput, "O\n");
				num_include_predict++;
				pass_user = user;
			}
			else if (is_predict_movie == -1) {
				fprintf(fpoutput, "X\n");
				pass_user = user;
			}
		}
	}

	fprintf(fpoutput, "\n\n\nTotal , %d\n", total);
	fprintf(fpoutput, "Including predict item , %d\n", num_include_predict);
	fprintf(fpoutput, "Confidence , %.3f\n", (float)num_include_predict/total);

	printf("\nTotal : %d\n", total);
	printf("Including predict item : %d\n", num_include_predict);
	printf("Confidence : %.3f\n", (float)num_include_predict / total);


	fclose(fpbaskets);
	fclose(fpoutput);
	printf("\nSuccess! Check the output file\n");
	return 0;

}