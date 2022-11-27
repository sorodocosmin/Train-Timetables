SQLprogram: sql.h sql.cpp server.cpp

	g++ -o SQLprogram server.cpp sql.cpp -lmysqlclient
clean:

	rm SQLprogram
