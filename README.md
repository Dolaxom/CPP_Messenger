# Local C++ messenger written in Qt6(QTcpServer) and PostgreSQL.

[Русская версия находится в папке materials/]

![Application](misc/images/msgnr3.png)

### Application architecture diagram:
![Diagram](misc/images/msgnr1.png)

#### Client
***Client*** class encapsulates ***authorization*** and ***messenger*** widgets. The ***authorization*** class contains only signals and slots for working with the authorization interface. The ***messenger*** class contains communication with the server and the messenger interface.

#### Server
The ***server*** contains all the functions for processing input values from the client and writing them to the database, as well as transferring data from the database to the client. There is a separate ***utilities*** class for the server to help the server work, it contains a set of functions different from network ones.

### Database architecture diagram:
![Diagram2](misc/images/msgnr2.png)

The users table from the non-obvious stores the current socket (token) of the session. It is needed so that the sender and recipient receive messages in real time without reloading the session. You can find the sources of the SQL tables in the `materials/` folder!

### Build
At the moment there is no Conan support, only PostgreSQL libs are used from third-party libraries. There are 2 separate makefiles, one for building the client and the other for building the server. There may be problems with starting the server - it is easy to fix, it is necessary.move the conf file for Postgres to the executable file, or change the path to the .conf file in the arguments of the function that loads it (I'll fix it in the near future, I didn't have time to make normal CMake+Makefile files. Tested only on Linux (Ubuntu) and MacOS.
