#include "debug.h"
#include "string.h"

static char buf[1024];

int vsprintf(char *buf, const char *fmt, va_list args);

char* itoa(int num,char* str,int radix)
{
    char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    unsigned unum;
    int i=0,j,k;
 
    
    if(radix==10&&num<0){
        unum=(unsigned)(-num);
	str[i++]='-';
    }
    else unum=(unsigned)num;
 
    do
    {
        str[i++]=index[unum%(unsigned)radix];
        unum/=radix;
 
    }while(unum);
 
    str[i]='\0';
 
    if(str[0]=='-') k=1;
    else k=0;
 
    char temp;
    for(j=k;j<=(i-1)/2;j++)
    {
        temp=str[j];
        str[j]=str[i-1+k-j];
        str[i-1+k-j]=temp;
    }
 
    return str;
 
}

int vsprintf(char *buf, const char *fmt, va_list args)
{
    char* p;
    char tmp[256];
    //va_list p_next_arg = args;
 
    for (p = buf; *fmt; fmt++)
    {
        if (*fmt != '%')
        {
            *p++ = *fmt;
            continue;
        }
 
        fmt++;
 
        switch (*fmt)
        {
	    case 'd':
            //int to str
            itoa((va_arg(args, int)),tmp,10);
            break;
        
        case 'x':
            //int to str
            itoa((va_arg(args, int)),tmp,16);
		break;
        case 's':
            //console_write(*((char*)p_next_arg));
            strcpy(tmp,va_arg(args, char*));
            break;
        default:
            break;
        }
	strcpy(p, tmp);
        //p_next_arg += 4;
        p += strlen(tmp);

    }
     
    return (p - buf);
} 


int printk(const char *format, ...)
{
	va_list args;
	int i;

	va_start(args,format);
	i=vsprintf(buf,format,args);
	va_end(args);
	
	//console_clear();	
	console_write(buf);	
	return i;	
	
}

