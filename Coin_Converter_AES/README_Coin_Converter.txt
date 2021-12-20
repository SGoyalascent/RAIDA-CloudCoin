1. Get the program repo from this link:-   https://github.com/SGoyalascent/RAIDA-CloudCoin/tree/main/Coin_Converter_AES

2. Install MYSQL client library:    sudo apt install default-libmysqlclient-dev

3. Update Coin_Converter.config file:- (i.) Database = name of the database in which tables "fixit_log" and "ans" are located(Both tables should be located in the same Database). 
                                       (ii.) Host:- host ip address of the Database(generally ip is 127.0.0.1 but better confirm it once)
                                       (iii.) listenport:- port on which the Database is listening
                                       (iv.) Username, Password:- Password and Username of the Database

4. In the "Data" directory, update the raida_no.txt file. Change the raida no. according to the Raida deployed

5. Compile the program to confirm if MySQL client is working.  Command:-   gcc raida_server.c udp_socket.c aes.c -o coin_converter `mysql_config --cflags --libs`

   Note:- (``) represents "Backtick" in  `mysql_config --cflags --libs`

6. Run the converter executable. Command to the file:-  $ ./coin_converter

7. Test it by compiling the udp_client.c file. Command to compile:-   gcc udp_client.c -o client         
					       Command to run the executable:-   ./client
