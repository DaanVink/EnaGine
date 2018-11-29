# Settings

## Introduction
When EnaGine starts up, it reads and parses the configuration file, located under `enaConf.ini`. By default, this configuration file includes some basic settings help you get started. It is possible to overwride the default file and direct EnaGine to use a different configuration file instead. To do this, simple add `-> filename` as the first line of the configuration file. EnaGine's settings are broken up into 3 parts in this documentation:
1. Memory allocation
2. Files
3. Other

## Memory allocation
This section mostly deals with the size of buffers allocated when a request is handled. You can ignore most of these, as they have balanced values by default. If, however, you are running EnaGine on a lower-end system or are experiencing memory issues, it is advised to lower some of these manually.
* Packet size: Size of packets in which the response header is broken up. Note: if the header is less than the packet size, it will not be padded.
* Request buffer: Size of the buffer allocated to store the request header. If the header exceeds this size, HTTP error 431 (Request Header Fields Too Large) will be sent.
* Payload buffer: Size of the buffer allocated to store the payload of a POST, APPEND or related requests, or $-data for a GET request

## Files
This section contains everything related to on-disk locations, file handling, etc. Paths can either be a full path (`/home/user/server/logs/`) or a relative path (`@/logs/`). Relative paths are created from the install location of the software, and are designated by a `@`-sign. For example: `@/logs/` might resolve to `/etc/EnaGine/logs/`. Alternatively, a `$`-sign can be used to designate a relative path to the webroot. For example: `$/errors/404.html` might resolve to `/server/root/errors/404.html`.
* Logging path: Path where logging data is to be stored
* Status paths: designated by a `status` prefix followed by the HTTP status code. These paths instruct EnaGine to replace the default status response with this file. Helpfull for error pages. *Highly discouraged for status codes in the `1xx` - `3xx` range*, as they will overwrite whatever the response to the request header was, thus undoing all previous work.

## Other
This section contains miscellaneous settings that do not fit into the other sections.
* Logging level: see [`logging.MD`](logging.MD)
* Comments: comments can be added by prefixing the piece of text with a `#` or `//`. Multiline comments are designated by a `##` or `///`
