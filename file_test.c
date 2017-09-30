/*
this code for checking file
*/
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>

#define HBYTE(a)               ((unsigned char)(((unsigned short)(a) >> 8) & 0xFF))
#define LBYTE(a)               ((unsigned char)((unsigned short)(a)& 0xff))
#define MAKEWORD(bLow,bHigh)   ((unsigned short)(((unsigned char)(bLow))|(((unsigned short)((unsigned char)(bHigh)))<<8)))


unsigned short CRC16_isr(unsigned char *Dat, unsigned int len)
{
    static const unsigned short CRC16table[256] =
    {
        0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
        0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
        0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
        0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
        0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
        0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
        0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
        0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
        0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
        0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
        0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
        0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
        0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
        0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
        0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
        0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
        0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
        0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
        0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
        0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
        0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
        0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
        0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
        0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
        0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
        0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
        0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
        0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
        0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
        0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
        0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
        0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
    };
    unsigned int i;
    unsigned short TxCRC16 = 0;

    for(i = 0; i < len; i++)
    {
        if(Dat[i] != 0)
        {
            TxCRC16 = 0;

            for(i = 0; i < len; i++)
            {
                TxCRC16 = (CRC16table[Dat[i] ^ (TxCRC16 & 0xFF)] ^ (TxCRC16 / 0x100));
            }
        }
    }

    if(TxCRC16 == 0x0000)
    {
        TxCRC16 = 0xFFFF;
    }

    return TxCRC16;
}

int main()
{
    FILE *fp;
    FILE *fp_initfile ;
    unsigned long int cnt = 0;                             //循环次数
    unsigned long int cnt_initfile = 0;
    unsigned long int result = 0;                          //实际读文件的结果
    unsigned long int result_initfile = 0;
    unsigned long int lSize = 0;                           //文件大小
    unsigned long int lSize_initfile = 0;
    unsigned long int npack = 0;                           //包的总个数
    unsigned long int right_pack = 0;                      //正确包的个数
    unsigned long int crcerr_pack = 0;                     //校验位错误的包个数
    unsigned long int counterr_pack = 0;                   //计数器错误的包个数
    unsigned short strcrc=0;
    unsigned short ncount = 0;                            //上一个计数器
    unsigned short readcount = 0;                         //当前计数器
    unsigned short lostcount = 0;                         //丢包数量
    unsigned char str[188]= {0};                          //存放188包的中间变量
    unsigned char n_str[188]= {0};                        //存放上一个188包的中间变量
    unsigned char tmp[2]= {0};                            //存放计数器
    unsigned int i = 0;

    if((fp=fopen("ZY50M.ts","rb"))==NULL)
    {
        printf("open file error!\n");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    lSize = ftell(fp);
    rewind(fp);

    unsigned char *buffer = (unsigned char*) malloc(sizeof(unsigned char) * lSize);
    if(buffer == NULL)
    {
        printf("memory error\n");
        return -1;
    }

    result=fread(buffer,1,lSize,fp);
    if(result<0)
    {
        printf("read error!\n");
        return -1;
    }

    if((fp_initfile=fopen("myfileAC.ts","rb"))==NULL)
    {
        printf("open file error!\n");
        return -1;
    }

    fseek(fp_initfile,0, SEEK_END);
    lSize_initfile = ftell(fp_initfile);
    rewind(fp_initfile);

    unsigned char *buffer_initfile = (unsigned char*) malloc(sizeof(unsigned char) * lSize_initfile);
    if(buffer_initfile == NULL)
    {
        printf("memory error\n");
        return -1;
    }

    result_initfile=fread(buffer_initfile,1,lSize_initfile,fp_initfile);
    if(result_initfile<0)
    {
        printf("read error!\n");
        return -1;
    }


    printf("初始文件原数据包：");
    for(i = 0; i < 188; i++)
    {
        printf("%02X", buffer_initfile[i]);
    }
    printf("\n");

    if(result==lSize)
    {
        for(cnt=0; cnt<lSize; cnt++)
        {
            if((buffer[cnt] == 0x47) && (buffer[cnt + 1] == 0x20) && (buffer[cnt + 2] == 0x37) && (buffer[cnt + 188] == 0x47))
            {
                npack++;
                memset(str,0,188);
                strcrc = 0;
                memcpy(str, &buffer[cnt], 188);
                strcrc=CRC16_isr(str,186);
                tmp[0]=HBYTE(strcrc);
                tmp[1]=LBYTE(strcrc);
                readcount=MAKEWORD(buffer[cnt + 4], buffer[cnt + 3]);


                if(ncount == 0)
                {
                    ncount =(str[3]<<8)|str[4];
                }
                else
                {
                    ncount++;

                    if(ncount > 0xFFFF)
                    {
                        ncount = 0;
                    }
                }

                if((MAKEWORD(buffer[cnt + 187], buffer[cnt + 186]) == strcrc) && (ncount == readcount))
                {
                    right_pack++;
                    strcpy(n_str,str);
                }
                else
                {
                    printf("********error*********\n********locate:%d*********\n",cnt);

                    if(MAKEWORD(buffer[cnt + 187], buffer[cnt + 186]) != strcrc)
                    {
                        crcerr_pack++;
                        printf("crc err: read crc=%04X, cal crc=%04X\n", MAKEWORD(buffer[cnt + 187], buffer[cnt + 186]), strcrc);

                        printf("前一个包数据：    ");
                        for(i = 0; i < 188; i++)
                        {
                            printf("%02X", n_str[i]);
                        }
                        printf("\n");
                        printf("当前包数据：      ");
                        for(i = 0; i < 188; i++)
                        {
                            printf("%02X", buffer[cnt + i]);
                        }
                        printf("\n");
                        strcpy(n_str,str);
                        for(cnt_initfile=0; cnt_initfile<lSize_initfile; cnt_initfile++)
                        {
                            if((buffer_initfile[cnt_initfile] == 0x47) && (buffer_initfile[cnt_initfile + 1] == 0x20) && (buffer_initfile[cnt_initfile + 2] == 0x37) && (buffer_initfile[cnt_initfile + 3] == buffer[cnt + 3]) && (buffer_initfile[cnt_initfile + 4] == buffer[cnt + 4]))
                            {
                                printf("初始文件原数据包：");
                                for(i = 0; i < 188; i++)
                                {
                                    printf("%02X", buffer_initfile[cnt_initfile + i]);
                                }
                                printf("\ndiff:当前包          初始文件包\n");
                                for(i=0;i<188;i++)
                                {
                                    while((buffer_initfile[cnt_initfile + i])!=(buffer[cnt + i]))
                                    {
                                        printf("    [%d]----%02x       [%d]----%02x\n",i,buffer[cnt + i],i,buffer_initfile[cnt_initfile + i]);
                                        i++;
                                    }
                                }
                                break;
                            }
                        }

                        printf("\n\n");
                    }

                    if(ncount != readcount)
                    {
                        counterr_pack++;
                        lostcount = (readcount - ncount) + lostcount;
                        printf("count err: ncount=%04X, read count=%04X, lost count: %d\n", ncount, readcount, (readcount - ncount));
                        ncount = readcount;

                        printf("前一个包数据：");
                        for(i = 0; i < 188; i++)
                        {
                            printf("%02X", n_str[i]);
                        }
                        printf("\n");
                        printf("当前包数据：  ");
                        for(i = 0; i < 188; i++)
                        {
                            printf("%02X", buffer[cnt + i]);
                        }
                        strcpy(n_str,str);

                        printf("\n\n");
                    }
                }


            }
        }
    }

    printf("一共有%ld个包\n",npack);
    printf("一共有%ld个正确的包\n",right_pack);
    printf("一共%ld个包校验位错误\n",crcerr_pack);
    printf("一共%ld个包计数器错误\n",counterr_pack);
    printf("一共掉了%ld个包\n",lostcount);


    free(buffer);
    fclose(fp);
    free(buffer_initfile);
    fclose(fp_initfile);
    return 0;
}

