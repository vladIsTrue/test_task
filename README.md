# TASK TRACKER
Application for viewing, adding and deleting employee tasks

## Usage
- If there is no parent task, you must specify NULL in the field for entering the parent task
- A record is deleted by selecting its id when viewing all employee tasks

## Protocol

The protocol i've designed to enable data transferring is pretty straightforward, each message sent is made up of:
- A 128 byte fixed-size header
- A Non-fixed size data buffer

The header follows the following structure:
```
QString header = QString("fileType:%1, fileName:%2, fileSize:%3;")
```
Where:
- ```%1``` is the request code;
- ```%2``` is "null" for messages, the correspondent fileName for attachments sent;
- ```%3``` is the size of the message or the attachment sent.