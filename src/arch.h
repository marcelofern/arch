/* declaration of colors for printing on the terminal */
#define BLU   "\x1B[34m"
#define CYN   "\x1B[36m"
#define GRN   "\x1B[32m"
#define MAG   "\x1B[35m"
#define RED   "\x1B[31m"
#define RESET "\x1B[0m"
#define WHT   "\x1B[37m"
#define YEL   "\x1B[33m"

/* strings and files in this program are way below this threshold though. */
#define MAX_STR_LEN 1000
#define MAX_NUM_LINES 1000

/* prototypes */
char **read_command_line_output(char *cmd);
int binary_exists(char *bin_name);
void dotfiles(void);
void install(void);
void install_aur_package(char *package_name);
void sync(void);
void update(void);
void update_system_time(void);
