all:

	g++ -o Server sql_functions.cpp treat_client.cpp sql.cpp server.cpp -lmysqlclient

	g++ -o Client menu.cpp client.cpp
clean:

	rm Server
	rm Client
