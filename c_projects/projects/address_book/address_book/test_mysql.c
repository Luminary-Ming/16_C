#include <stdio.h>
#include <mysql/mysql.h>

int main(void)
{
	MYSQL *conn;
	const char *server = "localhost";
	const char *user = "jiahao";
	const char *password = "jiahao123";
	const char *database = "address_book";

	printf("=== Ubuntu虚拟机C程序连接MySQL测试 ===\n");

	// 1. 初始化连接
	conn = mysql_init(NULL);
	if (conn == NULL)
	{
		fprintf(stderr, "mysql_init() 失败\n");
		return 1;
	}

	// 2. 连接到数据库
	printf("正在连接到MySQL服务器...\n");
	if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) == NULL)
	{
		fprintf(stderr, "连接失败: %s\n", mysql_error(conn));
		mysql_close(conn);
		return 1;
	}

	printf("成功连接到数据库: %s\n", database);

	// 3. 设置字符集为 UTF-8（重要！）
	if (mysql_set_character_set(conn, "utf8"))
		fprintf(stderr, "设置字符集失败: %s\n", mysql_error(conn));
	else
		printf("字符集已设置为: %s\n", mysql_character_set_name(conn));

	// 4. 执行简单查询
	printf("\n执行查询...\n");
	if (mysql_query(conn, "select * from students"))
		fprintf(stderr, "查询失败: %s\n", mysql_error(conn));
	else
	{
		MYSQL_RES *result = mysql_store_result(conn);
		if (result == NULL)
			fprintf(stderr, "获取结果失败: %s\n", mysql_error(conn));
		else
		{
			int num_fields = mysql_num_fields(result);  // 获取字段个数
			MYSQL_ROW row = NULL;

			printf("查询结果 (%d 列):\n", num_fields);
			printf("ID\t姓名\t年龄\t邮箱\n");
			printf("---------------------------\n");
			while ((row = mysql_fetch_row(result)))
			{
				for (int i = 0; i < num_fields; i++)
				{
					printf("%s\t", row[i] ? row[i] : "NULL");
				}
				printf("\n");
			}
			mysql_free_result(result);
		}
	}

	// 5. 执行一个插入操作测试（可选）
	if(mysql_query(conn,"insert into students (name,age,email) values ('测试用户', 25, 'test@example.com')"))
		fprintf(stderr, "插入失败: %s\n", mysql_error(conn));
	else 
		printf("插入成功，影响行数: %lld\n", (long long)mysql_affected_rows(conn));

	printf("\n程序执行完成，连接已关闭\n");

	// 5. 关闭连接
	mysql_close(conn);

	return 0;
}