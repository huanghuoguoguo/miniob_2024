import pymysql

# 数据库连接配置
config = {
    'unix_socket': '/tmp/miniob.sock',  # 替换为你的 socket 文件路径
    'database': 'sys',                  # 数据库名称
    'charset': 'utf8mb4',                         # 字符集
}

# 创建数据库连接
try:
    connection = pymysql.connect(**config)

    with connection.cursor() as cursor:
        # 创建一个表（如果不存在的话）
        create_table_query = """
        show tables;
        )
        """
        cursor.execute(create_table_query)

        # 插入数据
        insert_query = "INSERT INTO users (name, age) VALUES (%s, %s)"
        cursor.execute(insert_query, ('Alice', 30))
        cursor.execute(insert_query, ('Bob', 25))

        # 提交更改
        connection.commit()

        # 查询数据
        select_query = "SELECT * FROM users"
        cursor.execute(select_query)
        results = cursor.fetchall()

        for row in results:
            print(row)

except pymysql.MySQLError as e:
    print(f"Error connecting to database: {e}")

finally:
    if connection:
        connection.close()
