## Layout
This document contains some common terms used across the documentation, and is meant as a reference. It is structured as follows:

Name: description [example [if applicable]]

## Common terms
* $-data: $-data is data added after the file request in the request header. It is seperated by a $-sign. (index.html$name=john, test.html$id=0)
* Method: HTTP method used by the user request [POST, GET]
* File extension: extension the file has on disk [.txt, .png]
* Headerpath: path to file as designated in the header [/folder/index.html]
* Filepath: path to file as seen on disk [C:/server/root/index.html, /home/user/root/index.html]
* Folderpath: path to the parent folder of the requested file [C:/server/root/, /home/user/root/]
* Filename: name of file on disk, including extension [index.html, file.txt]
* Userdata: extra data added to the request by the user in any form
* Logging level: A single byte which determines the amount of loggin
* Webroot: Directory under which all content files are stored. (C:/server/root/, /home/user/server/root/)
