g++ -o ./bin/client -Iinclude/client -Iinclude/common ./src/common/*.cpp ./src/client/*.cpp -lmysqlcppconn
g++ -o ./bin/server -Iinclude/server -Iinclude/common ./src/common/*.cpp ./src/server/*.cpp -lmysqlcppconn
