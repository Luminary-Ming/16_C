#include <stdio.h>

/*
	*
	**
	***
	****
	*****
	******
*/
void zuo_xia(int n) {
	// 行 
	for (int i = 1; i <= n; i++) {
		// 打印 * 号
		for (int j = 1; j <= i; j++) {
			printf("*");
		}
		// 打印空格
		for (int k = 1; k <= n - i; k++) {
			printf(" ");
		}
		printf("\n");
	}
}

/*
	******
	*****
	****
	***
	**
	*
*/
void zuo_shang(int n) {
	// 行 
	for (int i = 1; i <= n; i++) {
		// 打印空格
		for (int k = 1; k <= k - i; k++) {
			printf(" ");
		}
		// 打印 * 号
		for (int j = n; j >= i; j--) {
			printf("*");
		}
	

		printf("\n");
	}
}


/*
	******
	 *****
	  ****
	   ***
	    **
	     *
	i	  k       j
	1	  0 " "   6 *
	2	  1 " "   5 *
	3	  2 " "   4 *
	4	  3 " "   3 *
	5	  4 " "   2 *
	6	  5 " "   1 *
*/
void you_shang(int n) {
	// 行 
	for (int i = 1; i <= n; i++) {
		// 打印空格
		for (int k = 0; k <= i - 2; k++) {
			printf(" ");
		}
		// 打印 * 号
		for (int j = n; j >= i; j--) {
			printf("*");
		}
		printf("\n");
	}
}


/*
	     *
	    **
	   ***
	  ****
	 *****
	******
	i	  k       j
	1	  5 " "   1 *
	2	  4 " "   2 *
	3	  3 " "   3 *
	4	  2 " "   4 *
	5	  1 " "   5 *
	6	  0 " "   6 *
*/
void you_xia(int n) {
	// 行 
	for (int i = 1; i <= n; i++) {
		// 打印空格
		for (int k = 1; k <= n - i; k++) {
			printf(" ");
		}
		// 打印 * 号
		for (int j = 1; j <= i; j++) {
			printf("*");
		}
		printf("\n");
	}
}


/*
	      *	
		 ***
		*****
	   *******
	  *********
	 ***********
	*************
	i	  k       j
	1	  5 " "   1 *
	2	  4 " "   2 *
	3	  3 " "   3 *
	4	  2 " "   4 *
	5	  1 " "   5 *
	6	  0 " "   6 *
*/
void dengyao_shang(int n) {
	// 行 
	for (int i = 1; i <= n; i++) {
		// 打印空格
		for (int k = 1; k <= n - i; k++) {
			printf(" ");
		}
		// 打印 * 号
		for (int j = 1; j <= i; j++) {
			printf("*");
		}
		printf("\n");
	}
}


/*
	*************
	 ***********
	  *********
	   *******
	    *****
	     ***
	      *
*/
void dengyao_xia(int n) {
	// 行 
	for (int i = 1; i <= n; i++) {
		// 打印空格
		for (int k = 1; k <= n - i; k++) {
			printf(" ");
		}
		// 打印 * 号
		for (int j = 1; j <= i; j++) {
			printf("*");
		}
		printf("\n");
	}
}



/*          
	          *
	         **
	        ***
	       ****
	      *****
	     ******
	    *******
	   ********
	  *********
	 **********
	***********
	 **********
	  *********
	   ********
	    *******
	     ******
	      *****
	       ****
	        ***
	         **
	          *       
*/
void dengyao_zuo(int n) {
	// 行 
	for (int i = 1; i <= n; i++) {
		// 打印空格
		for (int k = 1; k <= n - i; k++) {
			printf(" ");
		}
		// 打印 * 号
		for (int j = 1; j <= i; j++) {
			printf("*");
		}
		printf("\n");
	}
}

/*
	*
	**
	***
	****
	*****
	******
	*******
	********
	*********
	**********
	***********
	**********
	*********
	********
	*******
	******
	*****
	****
	***
	**
	*	
*/
void dengyao_you(int n) {
	// 行 
	for (int i = 1; i <= n; i++) {
		// 打印空格
		for (int k = 1; k <= n - i; k++) {
			printf(" ");
		}
		// 打印 * 号
		for (int j = 1; j <= i; j++) {
			printf("*");
		}
		printf("\n");
	}
}


int main()
{
	int n;
	scanf("%d", &n);
	printf("\n");

	zuo_shang(n);  // 左上

	you_xia(n);  // 右下

	printf("\n\n");  // 分隔

	you_shang(n);  // 右上

	zuo_xia(n);  // 左下

	return 0;
}
