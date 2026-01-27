address_book : main.o contact.o contact_llist.o db_mysql.o utils.o http_server.o
	gcc -o $@ $^ -lmysqlclient -lmicrohttpd -lpthread
clean :
	rm -rf *.o address_book