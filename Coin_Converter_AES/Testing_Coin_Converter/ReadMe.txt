1. For ECHO, compile command: gcc test_echo.c aes.c -o test_echo 

	unsigned char buffer_echo[MAXLINE]={0,0,2,0,0,4,2,0,0,0,0,0,22,22,0,1,1,0,0,0,0,0,0x3E,0x3E};

Change the raida_no. byte(3rd byte) to the legacy_raida_no. you are sending request to.

	servaddr.sin_addr.s_addr = inet_addr("139.99.155.124");

Change the ip_address to the legacy_raida_no. you are sending request to.


  
2. For VERSION, compile command: gcc test_version.c aes.c -o test_version

	 unsigned char buffer_version[MAXLINE]={0,0,2,0,0,15,2,0,0,0,0,0,22,22,0,1,1,0,0,0,0,0,0x3E,0x3E};

Change the raida_no. byte(3rd byte) to the legacy_raida_no. you are sending request to.

	servaddr.sin_addr.s_addr = inet_addr("139.99.155.124");

Change the ip_address to the legacy_raida_no. you are sending request to.

3. For UPGRADE_COIN ,compile command:   gcc test_upgrade_coin.c aes.c -o test_upgrade

	unsigned char buffer_upgrade_coin[MAXLINE] = {0,0,2,0,0,215,0,0,0,0,0,0,22,22,0,1,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
						74,39,187,57,149,157,85,138,101,136,77,66,105,128,26,97,225,193,113,149,62,33, // ticket_no
						0x3E, 0x3E};

Change the raida_no. byte(3rd byte) to the legacy_raida_no. you are sending request to. You can also change the ticket no. 

	servaddr.sin_addr.s_addr = inet_addr("139.99.155.124");

Change the ip_address to the legacy_raida_no. you are sending request to.