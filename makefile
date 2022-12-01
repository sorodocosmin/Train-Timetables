all:

	g++ -o SQLprogram treat_client.cpp sql.cpp server.cpp -lmysqlclient

	g++ -o client client.cpp
clean:

	rm SQLprogram
	rm client
