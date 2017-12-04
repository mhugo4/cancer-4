#Hugo Garcia nº 47800 grupo 34
#Dinis Silva nº47881 grupo 34
#Nuno Torcato nº47884 grupo 34
CC = gcc
SRC_DIR = source
INC_DIR = include
OBJ_DIR = object
TEST_DIR = testes
BIN_DIR = binary
CFLAGS = -Wall -I  $(INC_DIR)
OBJS = $(OBJ_DIR)/data.o $(OBJ_DIR)/entry.o $(OBJ_DIR)/table.o
OBJSPROF = $(OBJ_DIR)/test_data.o $(OBJ_DIR)/test_entry.o $(OBJ_DIR)/test_table.o



exec: data entry table table-private  message message-private  table-server table-client network_client tableskel  clientstub server client 

data: $(INC_DIR)/data.h $(SRC_DIR)/data.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/data.c  -o $(OBJ_DIR)/data.o

entry:$(INC_DIR)/entry.h $(SRC_DIR)/entry.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/entry.c  -o $(OBJ_DIR)/entry.o

table-private:$(INC_DIR)/table-private.h  $(SRC_DIR)/table-private.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/table-private.c  -o $(OBJ_DIR)/table-private.o


table:$(INC_DIR)/table.h $(INC_DIR)/table-private.h $(SRC_DIR)/table.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/table.c  -o $(OBJ_DIR)/table.o

test_data: $(SRC_DIR)/test_data.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/test_data.c  -o $(OBJ_DIR)/test_data.o


test_entry: $(SRC_DIR)/test_entry.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/test_entry.c  -o $(OBJ_DIR)/test_entry.o


test_table: $(SRC_DIR)/test_table.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/test_table.c  -o $(OBJ_DIR)/test_table.o


teste_data: $(OBJ_DIR)/data.o $(OBJ_DIR)/test_data.o
	$(CC) $(CFLAGS) $(OBJ_DIR)/data.o $(OBJ_DIR)/test_data.o -o $(BIN_DIR)/test_data


teste_entry: $(OBJ_DIR)/entry.o $(OBJ_DIR)/test_entry.o $(OBJ_DIR)/data.o
	$(CC) $(CFLAGS) $(OBJ_DIR)/data.o $(OBJ_DIR)/entry.o $(OBJ_DIR)/test_entry.o -o $(BIN_DIR)/test_entry 


teste_table: $(OBJ_DIR)/table.o $(OBJ_DIR)/test_table.o $(OBJ_DIR)/data.o $(OBJ_DIR)/entry.o $(OBJ_DIR)/table-private.o
	$(CC) $(CFLAGS) $(OBJ_DIR)/data.o $(OBJ_DIR)/entry.o $(OBJ_DIR)/table.o $(OBJ_DIR)/test_table.o $(OBJ_DIR)/table-private.o -o $(BIN_DIR)/test_table 
	

message:$(INC_DIR)/message.h $(SRC_DIR)/message.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/message.c  -o $(OBJ_DIR)/message.o

message-private:$(INC_DIR)/message-private.h $(SRC_DIR)/message-private.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/message-private.c  -o $(OBJ_DIR)/message-private.o

test_message: $(INC_DIR)/message.h $(SRC_DIR)/test_message.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/test_message.c  -o $(OBJ_DIR)/test_message.o

teste_message:$(OBJ_DIR)/test_message.o $(OBJ_DIR)/message.o $(OBJ_DIR)/table.o $(OBJ_DIR)/data.o $(OBJ_DIR)/entry.o $(OBJ_DIR)/table-private.o
	$(CC) $(CFLAGS) $(OBJ_DIR)/test_message.o $(OBJ_DIR)/message.o $(OBJ_DIR)/table.o $(OBJ_DIR)/data.o $(OBJ_DIR)/entry.o $(OBJ_DIR)/table-private.o -o $(BIN_DIR)/test_message

network_client:$(INC_DIR)/network_client-private.h $(SRC_DIR)/network_client.c $(INC_DIR)/inet.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/network_client.c  -o $(OBJ_DIR)/network_client.o

table-client:$(INC_DIR)/network_client-private.h $(INC_DIR)/table.h $(INC_DIR)/entry.h $(INC_DIR)/data.h $(INC_DIR)/message.h  $(SRC_DIR)/table-client.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/table-client.c -o $(OBJ_DIR)/table-client.o

table-server:$(INC_DIR)/table-private.h $(INC_DIR)/network_client-private.h $(INC_DIR)/inet.h $(SRC_DIR)/table-server.c 
	$(CC) $(CFLAGS) -c $(SRC_DIR)/table-server.c  -o $(OBJ_DIR)/table-server.o

tableskel:$(INC_DIR)/table_skel.h $(INC_DIR)/table.h $(INC_DIR)/table-private.h $(INC_DIR)/message.h $(SRC_DIR)/table_skel.c 
	$(CC) $(CFLAGS) -c $(SRC_DIR)/table_skel.c  -o $(OBJ_DIR)/table_skel.o


server:$(OBJ_DIR)/table.o $(OBJ_DIR)/data.o $(OBJ_DIR)/entry.o $(OBJ_DIR)/network_client.o $(OBJ_DIR)/table_skel.o $(OBJ_DIR)/table-private.o $(OBJ_DIR)/table-server.o $(OBJ_DIR)/message.o  
	$(CC) $(CFLAGS) $(OBJ_DIR)/table.o $(OBJ_DIR)/network_client.o $(OBJ_DIR)/data.o $(OBJ_DIR)/entry.o $(OBJ_DIR)/table_skel.o $(OBJ_DIR)/table-private.o $(OBJ_DIR)/message.o $(OBJ_DIR)/table-server.o   -o  $(BIN_DIR)/server

client:$(OBJ_DIR)/table.o $(OBJ_DIR)/data.o $(OBJ_DIR)/entry.o $(OBJ_DIR)/table-private.o $(OBJ_DIR)/client_stub.o $(OBJ_DIR)/network_client.o $(OBJ_DIR)/table-client.o $(OBJ_DIR)/message.o  
	$(CC) $(CFLAGS) $(OBJ_DIR)/table.o $(OBJ_DIR)/data.o $(OBJ_DIR)/entry.o $(OBJ_DIR)/table-private.o $(OBJ_DIR)/client_stub.o $(OBJ_DIR)/message.o  $(OBJ_DIR)/network_client.o $(OBJ_DIR)/table-client.o -o  $(BIN_DIR)/client -g

clientstub:$(INC_DIR)/client_stub.h $(INC_DIR)/client_stub-private.h $(INC_DIR)/table.h $(INC_DIR)/table-private.h $(INC_DIR)/network_client.h $(INC_DIR)/network_client-private.h $(INC_DIR)/client_stub.h $(SRC_DIR)/client_stub.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/client_stub.c  -o $(OBJ_DIR)/client_stub.o



clean:
	rm -f /object/*.o
	rm -f /binary/*