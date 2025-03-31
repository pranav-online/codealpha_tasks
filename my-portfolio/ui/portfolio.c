#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

void print_http_header() {
    printf("Content-Type: text/html\n\n");
}

int main() {
    char *data = getenv("CONTENT_LENGTH");  // Read content length
    int len = data ? atoi(data) : 0;
    char input[1024];

    if (len > 0 && len < sizeof(input)) {
        fgets(input, sizeof(input), stdin);
        
        // Parse input (example: "name=John&email=john@example.com&message=Hello")
        char name[100], email[100], message[500];
        sscanf(input, "name=%99[^&]&email=%99[^&]&message=%499[^\n]", name, email, message);
        
        // Decode URL encoding (replace %20 with space, etc.)
        // [You may need a URL decode function here]
        
        // Connect to MySQL
        MYSQL *conn = mysql_init(NULL);
        if (conn == NULL) {
            print_http_header();
            printf("<h2>MySQL Initialization Failed!</h2>");
            return 1;
        }

        if (mysql_real_connect(conn, "localhost", "root", "yourpassword", "yourdatabase", 3306, NULL, 0) == NULL) {
            print_http_header();
            printf("<h2>Database Connection Failed!</h2>");
            mysql_close(conn);
            return 1;
        }

        // Prepare and execute query
        char query[1024];
        snprintf(query, sizeof(query), "INSERT INTO contacts (name, email, message) VALUES ('%s', '%s', '%s')", name, email, message);
        
        if (mysql_query(conn, query)) {
            print_http_header();
            printf("<h2>Error Saving Data!</h2>");
        } else {
            print_http_header();
            printf("<h2>Form Submitted Successfully!</h2>");
        }

        // Close connection
        mysql_close(conn);
    } else {
        print_http_header();
        printf("<h2>Invalid Form Data</h2>");
    }

    return 0;
}
