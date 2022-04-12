#include<stdarg.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INT_MAX 4294967296 

char *convert(unsigned int num, int base) //functie preluata pentru conversia la char*
{ 
	static char Representation[]= "0123456789abcdef";
	static char buffer[50]; 
	char *ptr; 
	
	ptr = &buffer[49]; 
	*ptr = '\0'; 
	
	do 
	{ 
		*--ptr = Representation[num%base]; 
		num /= base; 
	}while(num != 0); 
	
	return(ptr); 
}

int iocla_printf(const char *format, ...)
{
	
	const char *ptr; 
	char *s; 
	int i;
	int aux = 0;
	va_list args; 
	va_start(args, format);

	for(ptr = format; *ptr != '\0'; ptr++) 
	{ 
		if (*ptr == '%' && (*(ptr + 1) == 'd' || *(ptr + 1) == 'u'   //testez daca % intra in alcatuirea 
			|| *(ptr + 1) == 'x' || *(ptr + 1) == 'c' || *(ptr + 1) == 's')) //unuia dintre specificatorii de date
			goto fetch;  //in caz afirmativ prelucrez si afisez argumentul corespunzator specificatorului
		
		if (*ptr == '%' && (*(ptr + 1) != 'd' || *(ptr + 1) != 'u'   //testez daca % este caracter printabil
			|| *(ptr + 1) != 'x' || *(ptr + 1) != 'c' || *(ptr + 1) != 's'))
			{
				fputs("%",stdout);
				aux++;
			}
		
		if(*ptr != '%') //afisez la stdout caractere care nu sunt in legatura cu specificatori
		{ 
			putc(*ptr,stdout);
			aux++;
			continue;
		} 

fetch:		
		ptr++; 
		
		if (*ptr) 
		{ 
			if (*ptr =='c') //prelucrez si afisez un argument de tip char si retin numarul
			{	            //de caractere printat
				i = va_arg(args,int);		
				putchar(i);
				aux = aux + 1;
			} 
						
			if (*ptr == 'd') //prelucrez si afisez un argument de tip intreg si retin numarul
			{					//de caractere printat
				i = va_arg(args,int); 
				if(i < 0)   //caz special pentru numerle negative
			 	{ 
			 		i = -i;
			 		fputs("-", stdout);
			 		aux++;
			 	}
			 	fputs(convert(i, 10), stdout);
			 	aux = aux + strlen(convert(i, 10));
			}	
						
			if (*ptr == 's')//prelucrez si afisez un argument de tip string si retin numarul
			{			   //de caractere printat
			 	s = va_arg(args,char *); 
			 	fputs(s, stdout);
			 	aux = aux + strlen(s);
			} 
						
			if (*ptr == 'x') //prelucrez un argument de tip intreg convertindu-l la hexazecimal
			{	             //dupa care afisez la stdout rezultatul convertirii si retin 
							//numarul de caractere printate
			 	i = va_arg(args,int);
			 	fputs(convert(i, 16), stdout);
			 	aux = aux + strlen(convert(i, 16));
		    }

			if (*ptr == 'u') //prelucrez un argument de tip unsigned int
			{
			 	i = va_arg(args, int);

			 	if (i < 0) //in cazul in care acesta este negativ va trebui convertit la unsigned
			 	{          //dupa care il afisez la stdout si retin numarul de caractere printate
			 		int aux1 = INT_MAX + i;
			 		fputs(convert(aux1, 10), stdout);
			 		aux = aux + strlen(convert(aux1, 10));
			 	}
				else{
					fputs(convert(i, 10), stdout); //in cazul in care numarule pozitiv, doar se
					aux = aux + strlen(convert(i, 10));//afiseaza continutul si se retine numarul de caractere
				}
			}
		} 
	  
	}
	
	va_end(args);

	if (aux != 0) //returneaza numarul de caractere afisate sau -1 daca nu se afiseaza nimic
		return aux;
	else
		return -1;
}


int main(int argc, char *argv[])
{
	FILE *f = fopen(argv[1],"r");

	fseek(f,0,SEEK_END);           //pozitionez cursorul la sfarsitul fisierului pentru a stoca numarul de caractere 
	size_t f_size = ftell(f);      //continut de fisierul de input, dupa care revin in pozitia initiala a fiserului
	fseek(f,0,SEEK_SET);
	char *string = malloc(f_size+1);

	fread(string,1,f_size,f);
	size_t i = 0, sw = 0;

	while (i < f_size && sw == 0) //caut unde se termina prima linie din fisier pentru a o elimina
	{
		if (string[i] == '\n')
			sw = 1;
		i++;
	}

	strcpy (string,(string + i));

	iocla_printf(string);
	
	fclose(f);
	free(string);

	return 0;
}