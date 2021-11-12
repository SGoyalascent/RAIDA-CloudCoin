#include <stdlib.h>
#include <stdio.h>
#include <mysql.h>
//#include <sql.h>

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

void Select_query(MYSQL *con) {

    uint32_t sr_no;

    if(mysql_query(con, "SELECT sn FROM fixit_log WHERE rn = $master_ticket") {
        finish_with_error(con);
    }

    do {
        MYSQL_RES *result = mysql_store_result(con);

        if( result == NULL) {
            printf("No Serial no. associated with the tickets");
            finish_with_error(con);
        }

        MYSQL_RQW row = mysql_fetch_row(result);

        printf("%s\n", row[0]);
        //store serial no.

        mysql_free_result(result);

        status = mysql_next_result(con);

        if(status > 0) {
            finish_with_error(con);
        }
    } while(status == 0);

    /*
    MYSQL_RES *result = mysql_store_result(con);

        if( result == NULL) {
            //mysql_field_count();
            printf("No Serial no. associated with the tickets");
            finish_with_error(con);
        }
        unsigned int no_rows = mysql_num_rows(result);

        for(int i = 0; i< no_rows; i++) {
            MYSQL_RQW row = mysql_fetch_row(result);

            printf("%s\n", row[i]);
            //store serial no.
        }

        mysql_free_result(result);

    */

    /*  //Memory efficient
        MYSQL_RES *result = mysql_use_result(con);

        if( result == NULL) {
            finish_with_error(con);
        }

        MYSQL_RQW row;
        while((row = mysql_fetch_row(result)) != NULL) {

            printf("%s\n", row[i]);
            //store serial no.
        }

        mysql_free_result(result);

    */
}

void Delete_query(MYSQL *con) {

    if(mysql_query(con, "DELETE FROM fixit_log WHERE rn= $master_ticket")) {
        finish_with_error(con);
    }
}

void Update_query(MYSQL *con){

    //loop until all sn updated
    if(mysql_query(con, "UPDATE ans SET NN = 2 WHERE SN = $sn AND NN = 1")) {
        finish_with_error(con);
    }

}

int main(int argc, char **argv) {

    MYSQL *con = mysql_init(ULL);

    if(con == NULL) {
        fprint(stderr, "%s\n", mysql_error(con));
        exit(1);
    }

    if(mysql_real_connection(con, "Host", "Username", "Password", "Database_name", "Listen_port", unix_socket, flag) == NULL) {
        finish_with_error(con);
    }

    Select_query(con);

    Delete_query(con);

    Update_query(con);


}
