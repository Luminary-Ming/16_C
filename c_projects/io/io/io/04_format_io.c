#include <stdio.h>

struct data
{
	int hum_data;
	int temp_data;
	float pm_data;
	float ch4_data;
	char str[16];
};

int main(void)
{
	struct data d = { 12,34,12.34,34.12,"SmartHome" };

	FILE *fp = fopen("data.txt", "w");
	if (!fp)
	{
		perror("无法打开文件");
		return -1;
	}

	fprintf(fp, "%d %d %.2f %.2f %s\n", d.hum_data, d.temp_data, d.pm_data, d.ch4_data, d.str);

	fclose(fp);
	return 0;
}