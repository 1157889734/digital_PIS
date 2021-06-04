//*****************************************************//
//			                   MyMiscFunc.c                                          //
//*****************************************************//

#include "MyAll.h"



/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(u32 nCount)	//assumably 1us
{
	int i;
	int j;

  	for(i=0; i<nCount; i++)
  	{
		for(j=0; j<20; j++)
		{
			j = j;
		}
  	}
  	
}



char bcd_to_data(char bcd)	
{
	char a,b,odata;
	a=bcd&0xf0;
	b=bcd&0x0f;
	a=a>>4;
	odata=a*10+b;
	return(odata);	
}

char data_to_bcd(char da)	
{
	char a,b,bcd;
	a=da/10;
	b=da%10;
	bcd=a*16+b;
	return(bcd);	
}

int shifang(int n)	
{
	int i;
	int out=10;

	if(n==0)
	{
		out=1;
	}
	else
	{
		for(i=1;i<n;i++)
		{
			out=out*10;
		}
	}
	
	return out;
}

int shiliufang(int n)	
{
	int i;
	int out=16;

	if(n==0)
	{
		out=1;
	}
	else
	{
		for(i=1;i<n;i++)
		{
			out=out*16;
		}
	}
	
	return out;
}



int IndexAddn(int index, int max, int n)
{
	int i;

	for(i=0; i<n; i++)
	{
		index++;
		if(index >= max)
		{
			index = 0;
		}
	}

	return index;
	
}

int IndexAdd1(int index, int max)
{

	index++;
	if(index >= max)
	{
		index = 0;
	}

	return index;
	
}

int LineToWords(char *line, int line_len, char* words, int word_maxnum, int word_maxsize)
{
	int index;
	int word_index;
	int byte_index;

	
	index = 0;
	word_index = 0;

	while((word_index<word_maxnum) && (index<line_len))
	{
		if((*(line+index)) == 0x00)
		{
			*(words + word_index*word_maxsize) = 0x00;
			break;
		}
	
		byte_index = 0;	
		while( ((*(line+ index)) > 0x20) && (byte_index < word_maxsize))
		{
			*(words + word_index*word_maxsize + byte_index) = *(line+index);

			index++;
			byte_index++;

		}

		if(byte_index != 0)
		{
			*(words + word_index*word_maxsize + byte_index) = 0x00;

			word_index++;

		}
		
		index++;

	}
	
	return word_index;
	
}

int StrSplitFrontChar(char *scr_str, char split_char, char *dest_str)
{
	int index;
	int word_index;
	int byte_index;
	

	index = 0;
	word_index = 0;
	byte_index = 0;
	while((*(scr_str + index) != 0x00) && (index<1024))
	{

		if( (*(scr_str + index) != split_char) && (word_index == 0) )
		{
			*(dest_str + byte_index) = *(scr_str + index);

			byte_index++;
			
		}
		else if( (*(scr_str + index) == split_char))
		{
			*(dest_str + byte_index) = 0x00;

			word_index++;
		}

		index++;
		
	}

	*(dest_str + byte_index) = 0x00;

	return byte_index;

}

int StrSplitBehindChar(char *scr_str, char split_char, char *dest_str)
{
	int index;
	int word_index;
	int byte_index;
	

	index = 0;
	word_index = 0;
	byte_index = 0;
	while((*(scr_str + index) != 0x00) && (index<1024))
	{

		if( (*(scr_str + index) != split_char) && (word_index == 1) )
		{
			*(dest_str + byte_index) = *(scr_str + index);

			byte_index++;

		}
		else if( (*(scr_str + index) == split_char))
		{
			*(dest_str + byte_index) = 0x00;
		
			word_index++;
		}

		index++;
		
	}

	
	*(dest_str + byte_index) = 0x00;

	return byte_index;


}


int StrToInt(char *str)
{
	int out;
	int len;
	int temp;
	int i;


	out = 0;

	len = strlen(str);

	if((len>2) && (*(str) == '0') && ((*(str+1) == 'x') || (*(str+1) == 'X')))
	{
		for(i=0; i<len-2; i++)
		{
			temp = *(str+2+i);

			if(temp>=0x30 && temp<=0x39)
			{
				temp = temp - 0x30;
				out = out + (temp * shiliufang(len-3-i));

			}
			else if(temp>=0x41 && temp<=0x46)
			{
				temp = temp - 0x41 + 0x0a;
				out = out + (temp * shiliufang(len-3-i));

			}
			else if(temp>=0x61 && temp<=0x66)
			{
				temp = temp - 0x61 + 0x0a;
				out = out + (temp * shiliufang(len-3-i));

			}

		}

	}
	else
	{
		for(i=0; i<len; i++)
		{
			temp = *(str+i);

			if(temp>=0x30 && temp<=0x39)
			{
				temp = temp - 0x30;
				out = out + (temp * shifang(len-1-i));

			}
			else
			{
				Debuger = Debuger;

			}

		}


	}


	return out;

}


// len is the length whithout the end(0x00)

int IntToStr(char *outstr, int inda, int len, char headchar)
{
	int temp;
	char tchar;
	
	int i;

	if(headchar == 0)
	{
		temp = inda % shifang(len);
	
		for(i=0; i<len; i++)
		{
			tchar = (char)( temp / shifang(len -1 -i) );
			temp =  temp % shifang(len -1 -i);

			*(outstr + i) = tchar + 0x30;

		}

		*(outstr + len) = 0x00;

	}
	else
	{
		*outstr = headchar;

		temp = inda % shifang(len-1);
	
		for(i=1; i<len; i++)
		{
			tchar = (char)( temp / shifang(len -1 -i) );
			temp =  temp % shifang(len -1 -i);

			*(outstr + i) = tchar + 0x30;

		}

		*(outstr + len) = 0x00;


	}
	


	return 0;

}




u32 Ip4toIp32(u8* ip4)
{
	u32 ip32;

	ip32 = ((*(ip4))<<24) | ((*(ip4+1))<<16) | ((*(ip4+2))<<8) | (*(ip4+3));
	

	return ip32;

}














