/* declaration of colors for printing on the terminal */
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

/* strings in this program are way below this threshold though. */
#define MAX_STR_LEN 1000

/* prototypes */
int binary_exists(char *bin_name);
void dotfiles(void);
void initiate_systemctl_services(void);
void install(void);
void install_aur_package(char *package_name);
void install_dependencies(void);
void install_zsh_shell(void);
void update(void);
void update_all_aur_packages(void);
void update_from_yay(void);
void update_system_time(void);
