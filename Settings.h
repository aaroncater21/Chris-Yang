#ifndef COMMON_INCLUDE_GLOBALSETTINGS_H_
#define COMMON_INCLUDE_GLOBALSETTINGS_H_

// Global macro definitions
#define true                  1
#define false                 0
#define SPACE                 ' '
#define FILE_LINE_LEN         2048
#define STR_EXPORT            "export"
#define FIPS_MODE             "OPENSSL_FIPS="
#define ENV_FILE              "/root/.bash_profile"

// Type name definitions
typedef int                   bool;

struct list{
	char *string;
	struct list *next;
}list;

// Enum definition
enum RETURN_VALUES{
	SUCCESS,
	FAILED,
};

/*
 * Free a string.
 * Parameters: str, char*
 * Return Value: None
 */
void free_str(char *str);

/*
 * Free the list and list->string.
 * Parameters: str_list, list*
 * Return Value: None
 */
void free_list(struct list *str_list);

/*
 * Free the string array and the strings in the array.
 * Parameters: str_arrt, char**
 * Return Value: None
 */
void free_string_arr(char **str_arr);

/*
 * Remove all the specific characters in the string.
 * Parameters:
 *        param: string, char*
 *        param: character, const char
 */
void remove_all_spec_chars_in_string(char *string, const char character);

/*
 * Remove all the specific characters at the beginning of the string.
 * Parameters:
 *        param: string, char*
 *        param: character, const char
 * Return Value: char*
 */
char *lstrip_spec_chars(char *string, const char character);

/*
 * Split a string by a specific character into a list.
 * Parameters:
 *        param: string, char*
 *        param: character, const char
 * Return value: new_list, list*
 * Note: whoever call this function, please don't forget to loop the list and free the list and list.string
 */
struct list *split_string_to_linklist(char *string, const char character);

/*
 * Split a string by a specific character into an array.
 * Parameters:
 *        param: string, char*
 *        param: character, char
 * Return Value: char**
 * Note: whoever call this function, please don't forget to free the array and the strings in the array.
 */
char **split_string_to_array(char *string, const char character);

/*
 * Search if the string is in the file.
 * Parameters:
 *        param: file, char*
 *        param: string, char*
 * Return Value: true or false
 */
bool is_the_string_in_file(const char *file, const char *string);

/*
 * Detect if the system is in FIPS mode by checking the OPENSSL_FIPS existence in ENV_FILE.
 * Parameters: None
 * Return Value: true or false
 */
bool is_the_system_on_fips_mode();

/*
 * Search if the string is in the file, and then return the first matched line.
 * Parameters:
 *        param: file, char*
 *        param: string, char*
 * Return Value: char* or NULL
 * Note: Please free the line after using.
 */
char *get_the_line_in_file(const char *file, const char *string);

/*
 * Enable FIPS mode if the system is in FIPS mode.
 * Parameters: None
 * Return Value: SUCCESS or FAILED
 */
int set_fips_mode();

#endif /* COMMON_INCLUDE_GLOBALSETTINGS_H_ */
