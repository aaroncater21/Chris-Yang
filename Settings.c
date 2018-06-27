#include <stdio.h>
#include <string.h>
#include <openssl/err.h>
#include "Settings.h"

/*
 * Free a string.
 * Parameters: str, char*
 * Return Value: None
 */
void free_str(char *str)
{
	if(str)
	{
		free(str);
		str = NULL;
	}
}

/*
 * Free the list and list->string.
 * Parameters: str_list, list*
 * Return Value: None
 */
void free_list(struct list *str_list)
{
	if(str_list)
	{
		free_str(str_list->string);
		if(str_list->next)
		{
			free_list(str_list->next);
		}
		else
		{
			free(str_list);
			str_list = NULL;
		}
	}
}

/*
 * Free the string array and the strings in the array.
 * Parameters: str_arrt, char**
 * Return Value: None
 */
void free_string_arr(char **str_arr)
{
	if(str_arr)
	{
		int i = 0;
		while(str_arr[i])
		{
			free_str(str_arr[i]);
			i++;
		}
		free(str_arr);
		str_arr = NULL;
	}
}

/*
 * Remove all the specific characters in the string.
 * Parameters:
 *        param: string, char*
 *        param: character, const char
 */
void remove_all_spec_chars_in_string(char *string, const char character)
{
	if(string == NULL)
	{
		printf("Parameter string is NULL.\n");
		return;
	}
	char *start = string;
	char *end = string;

	while(*end)
	{
		*start = *end++;
		if(*start != character)
			start++;
	}
	*start = 0;
}

/*
 * Remove all the specific characters at the beginning of the string.
 * Parameters:
 *        param: string, char*
 *        param: character, const char
 * Return Value: char*
 */
char *lstrip_spec_chars(char *string, const char character)
{
	if(string == NULL)
	{
		printf("Parameter string is NULL.\n");
		return NULL;
	}
	else
	{
		while(*string)
		{
			if (*string != character)
				break;
			else
				string++;
		}

		return string;
	}
}

/*
 * Split a string by a specific character into a list.
 * Parameters:
 *        param: string, char*
 *        param: character, const char
 * Return Value: new_list, list*
 * Note: whoever call this function, please don't forget to loop the list and free the list and list->string
 */
struct list *split_string_to_linklist(char *string, const char character)
{
	if(string == NULL)
	{
		printf("Parameter string is NULL.\n");
		return NULL;
	}
	else
	{
		struct list *new_list = (struct list*)(malloc)(1);
		if(new_list ==  NULL)
		{
			printf("Cannot alloc the space for variable new_list.\n");
			return NULL;
		}
		else
		{
			new_list->string = NULL;
			new_list->next = NULL;
			int len = strlen(string)+1;
			char *new_start = (char*)calloc(len, sizeof(char));
			if(new_start == NULL)
			{
				printf("Cannot alloc the space for variable new_start.\n");
				free_list(new_list);
				return NULL;
			}
			else
			{
				new_list->string = new_start;

				char *end = NULL;
				if((end = strchr(string, character)) == NULL)
				{
					int ret  = snprintf(new_start, len, "%s", string);
					if(ret < 0)
					{
						printf("cannot copy string: %s to new_start.\n", string);
						free_list(new_list);
						return NULL;
					}
					else
					{
						printf("Copied string: %s to new_start.\n", string);
						return new_list;
					}
				}
				else
				{
					if(end == string)
					{
						end = lstrip_spec_chars(string, character);
						if(strlen(end) == 0)
						{
							free_list(new_list);
							return NULL;
						}
					}
					else
						end = string;

					while(*end)
					{
						*new_start = *end++;
						if(strchr(new_start, character) == NULL)
						{
							new_start++;
						}
						else
						{
							*new_start = 0;
							new_list->next = split_string_to_linklist(end, character);
							break;
						}
					}
					return new_list;
				}
			}
		}
	}
}

/*
 * Split a string by a specific character into an array.
 * Parameters:
 *        param: string, char*
 *        param: character, char
 * Return Value: char**
 * Note: whoever call this function, please don't forget to free the array and the strings in the array.
 */
char **split_string_to_array(char *string, const char character)
{
	if(string == NULL)
	{
		printf("Parameter string is NULL.\n");
		return NULL;
	}
	else
	{
		char **arr = (char **)malloc(1);
		if(arr == NULL)
		{
			printf("Cannot malloc the space for variable arr.\n");
			return NULL;
		}
		else
		{
			arr[0] = NULL;
			char *start = string;
			char *end = string;

			int index = 0;
			while(*end)
			{
				start = lstrip_spec_chars(end, character);
				end = start;
				if(*end)
				{
					while(*end && *end != character)
						end++;
					int str_len = (end -start + 1)/sizeof(char);
					char *subString = (char *)calloc(str_len, sizeof(char));
					if(subString == NULL)
					{
						printf("Cannot calloc the space for variable subString.\n");
						return arr;
					}
					else
					{
						int ret = snprintf(subString, str_len, "%s", start);
						if(ret < 0)
						{
							printf("Cannot copy the value to variable subString.\n");
							free_str(subString);
							return arr;
						}
						else
						{
							char **tmp_arr = (char **)realloc(arr, (index+2)*sizeof(char*));
							if(tmp_arr == NULL)
							{
								printf("Cannot malloc the space for variable tmp_arr.\n");
								free_str(subString);
								return arr;
							}
							else
							{
								arr = tmp_arr;
								arr[index++] = subString;
								arr[index] = NULL;
								start = end;
							}
						}
					}
				}
				else
					break;
			}
			return arr;
		}
	}
}

/*
 * Search if the string is in the file.
 * Parameters:
 *        param: file, char*
 *        param: string, char*
 * Return Value: true or false
 */
bool is_the_string_in_file(const char *file, const char *string)
{
	if(file == NULL)
		printf("Parameter file is NULL.\n");
	else
	{
		FILE *fd = NULL;
		char line[FILE_LINE_LEN];

		if((fd = fopen(file, "r")) == NULL)
		{
			printf("Cannot open file: %s.\n", file);
		}
		else
		{
			while(fgets(line, FILE_LINE_LEN, fd) != NULL)
			{
				if (!strstr(line, string))
				{
					printf("Find the string: %s in the file: %s.\n", file, string);
					return true;
				}
			}
			printf("Cannot find the string: %s in the file: %s.\n", string, file);
		}
	}
	return false;
}

/*
 * Search if the string is in the file, and then return the first matched line.
 * Parameters:
 *        param: file, char*
 *        param: string, char*
 * Return Value: char* or NULL
 * Note: Please free the line after using.
 */
char *get_the_line_in_file(const char *file, const char *string)
{
	if(file == NULL)
		printf("Parameter file is NULL.\n");
	else
	{
		FILE *fd = NULL;
		char *line = NULL;

		if((fd = fopen(file, "r")) == NULL)
		{
			printf("Cannot open file: %s.\n", file);
		}
		else
		{
			line = calloc(FILE_LINE_LEN, sizeof(char));
			if(line == NULL)
			{
				printf("Cannot alloc the space for variable line.\n");

			}
			else
			{
				while(fgets(line, FILE_LINE_LEN, fd) != NULL)
				{
					if (strstr(line, string))
					{
						printf("Find the string: %s in the file: %s.\n", string, file);
						if(fd)
							fclose(fd);
						return line;
					}
				}
				printf("Cannot find the string: %s in the file: %s.\n", string, file);
			}
		}

		free_str(line);
		if(fd)
			fclose(fd);
	}
	return NULL;
}

/*
 * Detect if the system is in FIPS mode by checking the OPENSSL_FIPS existence in ENV_FILE.
 * Parameters: None
 * Return Value: true or false
 */
//This method is for using linked list
/*bool is_the_system_in_fips_mode()
{
	bool ret = false;
	char *line = NULL;
	struct list *str_list = NULL;
	line = get_the_line_in_file(ENV_FILE, FIPS_MODE);

	if(line)
	{
		// Remove the new line mark "\n"
		line[strlen(line)-1] = 0;
		str_list = split_string_to_linklist(line, SPACE);
		if(str_list)
		{
			if(str_list->string && !strcmp(str_list->string, STR_EXPORT))
			{
				if(str_list->next)
				{
					if(str_list->next->string && strstr(str_list->next->string, FIPS_MODE))
					{
						ret = true;
						printf("The system is in FIPS mode.\n");
					}
					else
						printf("FIPS configuration: %s is wrong in the file: %s.\n", line, ENV_FILE);
				}
				else
					printf("FIPS configuration: %s is wrong in the file: %s.\n", line, ENV_FILE);
			}
			else
				printf("FIPS configuration: %s is wrong in the file: %s.\n", line, ENV_FILE);
		}
		else
			printf("FIPS configuration: %s is wrong in the file: %s.\n", line, ENV_FILE);
	}
	else
		printf("Cannot find the configuration: %s in file: %s, the system is not in FIPS mode.\n", FIPS_MODE, ENV_FILE);

	// Free the memory
	free_str(line);
	free_list(str_list);

	return ret;
}*/

bool is_the_system_in_fips_mode()
{
	bool ret = false;
	char *line = NULL;
	char **arr = NULL;
	line = get_the_line_in_file(ENV_FILE, FIPS_MODE);

	if(line)
	{
		// Remove the new line mark "\n"
		line[strlen(line)-1] = 0;
		arr = split_string_to_array(line, SPACE);
		if(arr)
		{
			if(arr[0] && !strcmp(arr[0], STR_EXPORT) && arr[1] && strstr(arr[1], FIPS_MODE))
			{
				ret = true;
				printf("The system is in FIPS mode.\n");
			}
			else
				printf("FIPS configuration: %s is wrong in the file: %s.\n", line, ENV_FILE);
		}
		else
			printf("FIPS configuration: %s is wrong in the file: %s.\n", line, ENV_FILE);
	}
	else
		printf("Cannot find the configuration: %s in file: %s, the system is not in FIPS mode.\n", FIPS_MODE, ENV_FILE);

	// Free the memory
	free_str(line);
	free_string_arr(arr);

	return ret;
}

/*
 * Enable FIPS mode if the system is in FIPS mode.
 * Parameters: None
 * Return Value: SUCCESS or FAILED
 */
int __attribute__((constructor)) set_fips_mode(void)
{
	int ret = SUCCESS;
	bool fips_mode = is_the_system_in_fips_mode();

	if(FIPS_mode_set(fips_mode) == 1)
	{
		printf("Run FIPS_mode_set(%d) succeeded.\n", fips_mode);
	}
	else
	{
		ret = FAILED;
		printf("Run FIPS_mode_set(%d) failed, error code %lu.\n", fips_mode, ERR_get_error());
	}

	return ret;
}

int main()
{
	printf("main fun.\n");
}
