/*
 * string_parser.c
 *
 *  Created on: Nov 25, 2020
 *      Author: gguan, Monil
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_parser.h"

#define _GUN_SOURCE

int count_token (char* buf, const char* delim)
{
	//TODO：
	/*
	*	#1.	Check for NULL string
	*	#2.	iterate through string counting tokens
	*		Cases to watchout for
	*			a.	string start with delimeter
	*			b. 	string end with delimeter
	*			c.	account NULL for the last token
	*	#3. return the number of token (note not number of delimeter)
	*/

	// To see if NULL String
	if (buf == NULL){
		return EXIT_FAILURE;
	}

	int count = 0;

	if(buf[0] == *delim){
		count=- 1;
	}
	for(int i = 0; i < strlen(buf); i++)
	{
	
		/*if (buf[i] != *delim){i
			continue;
		}

		else if(buf[strlen(buf)-1] != *delim){
			continue;
		}
		*/
		if(buf[i] == *delim){

		count+= 1;
		
		}

		
	}

	if(buf[strlen(buf)-2] != ";" && *delim == ";"){
		
		count+= 1;
	}

	return count + 1;
	

}

command_line str_filler (char* buf, const char* delim)
{
	//TODO：
	/*
	*	#1.	create command_line variable to be filled and returned
	*	#2.	count the number of tokens with count_token function, set num_token. 
    *           one can use strtok_r to remove the \n at the end of the line.
	*	#3. malloc memory for token array inside command_line variable
	*			based on the number of tokens.
	*	#4.	use function strtok_r to find out the tokens 
    *   #5. malloc each index of the array with the length of tokens,
	*			fill command_list array with tokens, and fill last spot with NULL.
	*	#6. return the variable.
	*/

	command_line answer;
	char* saved;
	char* token;

	int total_tokens = count_token(buf, delim);
	answer.command_list = (char**)malloc(sizeof(char*) * total_tokens);
	answer.num_token = total_tokens;
	buf = strtok_r(buf,"\n",&saved);
	if( *delim == " ") {
		total_tokens+= 1;
	}
	for(int i = 0; i < total_tokens - 1; i++){
		token = strtok_r(buf, delim, &saved);
		buf = NULL;
		answer.command_list[i] = (char*)malloc(sizeof(char) * (strlen(token)+ 1));
		strcpy(answer.command_list[i],token);

	}

	answer.command_list[total_tokens -1] = NULL;

	return answer;
}


void free_command_line(command_line* command)
{
	//TODO：
	/*
	*	#1.	free the array base num_token
	*/

	for(int i = 0; i < command->num_token; i++){
		free(command->command_list[i]);
	}

	free(command->command_list);
}
