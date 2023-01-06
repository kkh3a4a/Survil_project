#include <iostream>
using namespace std;
#define matrixsize 4

int main()
{
	int matrix[matrixsize][matrixsize];
	int num{1};
	for (int i = 0; i < matrixsize; ++i)
	{
		for (int j = 0; j < matrixsize; ++j)
		{
			matrix[i][j] = num;
			num++;
		}
	}
	
	for (int i = 0; i < matrixsize; ++i)
	{
		for (int j = 0; j < matrixsize; ++j)
		{
			printf("%3d", matrix[i][j]);
		}
		cout << endl;
	}

	int n, m;
	while (true)
	{
		cin >> n >> m;
		if (n == -1 && m == -1)
			break;
		else if (n < 0 || n > matrixsize-1 || m < 0 || m > matrixsize-1)
			cout << "없는 모래" << endl;
		else if (n == 0 || n == matrixsize-1 || m == 0 || m == matrixsize-1)
		{
			if (n == 0 && m == 0) {
				cout << matrix[n][m] << endl;
				cout << matrix[n][m + 1] - matrix[n][m] << endl;

				cout << matrix[n+1][m] - matrix[n][m] << " ";
				cout << matrix[n+1][m+1] - matrix[n][m] << endl;
			}
			else if (n == 0 && m == matrixsize-1) {
				cout << matrix[n][m] << endl;
				cout << matrix[n][m - 1] - matrix[n][m] << endl;

				cout << matrix[n + 1][m - 1] - matrix[n][m] << " ";
				cout << matrix[n + 1][m] - matrix[n][m] << endl;
			}
			else if (n == matrixsize-1 && m == 0) {
				cout << matrix[n][m] << endl;
				cout << matrix[n-1][m] - matrix[n][m] << " ";
				cout << matrix[n - 1][m + 1] - matrix[n][m] << endl;

				cout << matrix[n][m + 1] - matrix[n][m] << endl;
			}
			else if (n == matrixsize-1 && m == matrixsize-1) {
				cout << matrix[n][m] << endl;
				cout << matrix[n - 1][m - 1] - matrix[n][m] << " ";
				cout << matrix[n - 1][m] - matrix[n][m] << endl;

				cout << matrix[n][m - 1] - matrix[n][m] << endl;
			}
			else if (n == 0) {
				cout << matrix[n][m] << endl;
				cout << matrix[n][m + 1] - matrix[n][m] << " ";
				cout << matrix[n][m - 1] - matrix[n][m] << endl;

				cout << matrix[n + 1][m - 1] - matrix[n][m] << " ";
				cout << matrix[n + 1][m] - matrix[n][m] << " ";
				cout << matrix[n + 1][m + 1] - matrix[n][m] << endl;
			}
			else if (n == matrixsize-1) {
				cout << matrix[n][m] << endl;
				cout << matrix[n - 1][m - 1] - matrix[n][m] << " ";
				cout << matrix[n - 1][m] - matrix[n][m] << " ";
				cout << matrix[n - 1][m + 1] - matrix[n][m] << endl;

				cout << matrix[n][m + 1] - matrix[n][m] << " ";
				cout << matrix[n][m - 1] - matrix[n][m] << endl;
			}
			else if (m == 0) {
				cout << matrix[n][m] << endl;
				cout << matrix[n - 1][m] - matrix[n][m] << " ";
				cout << matrix[n - 1][m + 1] - matrix[n][m] << endl;

				cout << matrix[n][m + 1] - matrix[n][m] << endl;

				cout << matrix[n + 1][m] - matrix[n][m] << " ";
				cout << matrix[n + 1][m + 1] - matrix[n][m] << endl;
			}
			else {
				cout << matrix[n][m] << endl;
				cout << matrix[n - 1][m - 1] - matrix[n][m] << " ";
				cout << matrix[n - 1][m] - matrix[n][m] << endl;

				cout << matrix[n][m - 1] - matrix[n][m] << endl;

				cout << matrix[n + 1][m - 1] - matrix[n][m] << " ";
				cout << matrix[n + 1][m] - matrix[n][m] << endl;
			}
		}
		else
		{
			cout << matrix[n][m] << endl;
			cout << matrix[n - 1][m - 1] - matrix[n][m] << " ";
			cout << matrix[n - 1][m] - matrix[n][m] << " ";
			cout << matrix[n - 1][m + 1] - matrix[n][m] << endl;

			cout << matrix[n][m + 1] - matrix[n][m] << "    ";
			cout << matrix[n][m - 1] - matrix[n][m] << endl;

			cout << matrix[n + 1][m - 1] - matrix[n][m] << " ";
			cout << matrix[n + 1][m] - matrix[n][m] << " ";
			cout << matrix[n + 1][m + 1] - matrix[n][m] << endl;
		}
	}
}