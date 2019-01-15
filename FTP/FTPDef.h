#pragma once
#define MAX_DATASOCKET 10
#define MAX_LENGTH 4096
#define MAX_CMD 128
#define MAX_FILES 4096

#define FTP_WEL 220
#define FTP_PWD 257
#define FTP_NEEDPASS 331
#define FTP_PASSOK 230
#define FTP_CWD 250
#define FTP_SUCC 200
#define FTP_PASV 227
#define FTP_ODC 150
#define FTP_TCOMPLETE 226
#define FTP_FNF 550
#define FTP_WPASS 530
#define FTP_OCF 425
#define FTP_TFAIL 426

#define T_LIST 0
#define T_DOWNLOAD 1
#define T_UPLOAD 2
