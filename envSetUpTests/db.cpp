#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

using namespace sql;

int main()
{
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;

    try
    {
        // Create a MySQL driver
        driver = sql::mysql::get_mysql_driver_instance();

        // Establish a connection to the MySQL database
        con = driver->connect("tcp://127.0.0.1:3306", "shrinath", "shri@123");

        // Use the 'test' database (replace with your actual database name)
        con->setSchema("cafeteria");

        // Create a statement object
        Statement *stmt = con->createStatement();

        // Create a table (if it doesn't exist)
        stmt->execute("CREATE TABLE IF NOT EXISTS example (id INT, name VARCHAR(50))");

        // Insert data into the table
        stmt->execute("INSERT INTO example(id, name) VALUES (1, 'John')");
        stmt->execute("INSERT INTO example(id, name) VALUES (2, 'Alice')");

        // Select and display data from the table
        ResultSet *res = stmt->executeQuery("SELECT * FROM example");
        while (res->next())
        {
            std::cout << "ID: " << res->getInt("id") << ", Name: " << res->getString("name") << std::endl;
        }

        // Update data in the table
        stmt->execute("UPDATE example SET name = 'Bob' WHERE id = 1");

        // Delete data from the table
        stmt->execute("DELETE FROM example WHERE id = 2");

        // Clean up
        delete res;
        delete stmt;
        delete con;
    }
    catch (SQLException &e)
    {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }

    return 0;
}
