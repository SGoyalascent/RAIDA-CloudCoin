1. Get the program repo from this link:-   https://github.com/worthingtonse/RAIDA-Utils/tree/main/Coin_Converter_AES

2. Install MYSQL client library:    sudo apt install default-libmysqlclient-dev

3. Update Coin_Converter.config file:- (i.) Database = name of the database in which tables "fixit_log" and "ans" are located(Both tables should be located in the same Database). 
                                       (ii.) Host:- host ip address of the Database(generally ip is 127.0.0.1 but better confirm it once)
                                       (iii.) listenport:- port on which the Database is listening
                                       (iv.) Username, Password:- Password and Username of the Database

4. In the "Data" directory, update the raida_no.txt file. Change the raida no. according to the Raida deployed. 
	Example:- if Legacy_Raida-2, write 2; if Legacy_Raida-15, write 15. Similarly for other Legacy RAIDA's

5. Compile the program to confirm if MySQL client is working.  Command:-   gcc raida_server.c udp_socket.c aes.c -o coin_converter `mysql_config --cflags --libs`
   Note:- (``) does not represent inverted comma('') in  `mysql_config --cflags --libs`

6. Run the converter executable. Command to the file:-  $ ./coin_converter



For testing the coin_converter program, 
In udp_client.c, 

1. edit buffer_upgrade_coin for the ticket no. you want. (line no. 247)

unsigned char buffer_upgrade_coin[MAXLINE] = {0,0,2,0,0,215,0,0,0,0,0,0,22,22,0,1,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,

					245,203,185,30,21,126,219,54,115,46,238,42,155,50,130,149,59,125,160,101,113,04,   //ticket_no bytes
					0x3E, 0x3E};

For example, if ticket no. in the database is this:-    04 71  65  a0  7d  3b 95  82  32 9b  2a ee  2e 73  36 db  7e  15 1e b9  cb  f5   //hexadecimal bytes
		//hexadecimal-representation//		0x04 0x71 0x65 0xa0 0x7d 0x3b 0x95 0x82 0x32 0x9b 0x2a 0xee 0x2e 0x73 0x36 0xdb 0x7e 0x15 0x1e 0xb9 0xcb 0xf5
			//decimal bytes//		04 113 101 160 125 59 149 130 50 155 42 238 46 115 54 219 126 21 30 185 203 245	

You need to add the ticket starting with most-significant-byte or basically in reverse order of the ticket in the database(in any Hexa-decimal or decimal representation)	

Update the byte for the RAIDA ID too.

2. Edit the ip_address in the expression. (line 314)  
	servaddr.sin_addr.s_addr = inet_addr("139.99.155.124");   //Replace "139.99.155.124" with the ip_address of the Legacy raida you are running the coin_converter program on.

3. Test it by compiling the udp_client.c file. Command to compile:-   gcc udp_client.c aes.c -o client         
					       Command to run the executable:-   ./client	


							