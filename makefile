all:

	g++ -o SQLprogram server.cpp sql.cpp -lmysqlclient

	g++ -o client client.cpp
clean:

	rm SQLprogram
	rm client
