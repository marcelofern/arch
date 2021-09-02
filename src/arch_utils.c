#include "cl_utils.h"
#include "arch_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int binary_exists(char *bin_name);
static void initiate_systemctl_services(void);
static void install_aur_package(char *package_name);
static void install_dependencies(void);
static void install_zsh_shell(void);
static void update_from_yay(void);
static void update_system_time(void);

void install(void)
{
    puts(YEL "Installing dependencies..." RESET "\n");
    install_dependencies();
    puts(GRN "Dependencies installed..." RESET "\n");

    puts(YEL "Initiating systemctl services..." RESET "\n");
    initiate_systemctl_services();
    puts(GRN "systemctl services initiated..." RESET "\n");

    puts(YEL "Setting gruvbox color schema..." RESET "\n");
    system("wal --theme base16-gruvbox-soft");
    puts(GRN "Gruvbox was set..." RESET "\n");

    puts(YEL "Generating .ssh/ folder" RESET "\n");
    system("ssh-keygen");
    puts(GRN ".ssh/ folder was generated..." RESET "\n");

    puts(YEL "Setting up zsh as shell" RESET "\n");
    install_zsh_shell();
    puts(GRN "Zsh has been installed!" RESET "\n");

    puts(GRN "You are all set up now. reboot your PC!" RESET "\n");
}

void dotfiles(void) {
    struct cl_it cl_it[1];
    char *git_status_cmd = "cd ~ && git status 2>&1";

    cl_open(cl_it, git_status_cmd);
    // only reads the first line from the iterator.
    cl_next(cl_it);
    char *missing_repo = strstr(cl_it->line, "fatal: not a git repository");
    cl_close(cl_it);

    if (missing_repo) {
        system("cd ~ && rm -rf dotfiles");
        system("cd ~ && git clone https://github.com/marcelofern/dotfiles.git");
        system("cd ~ && cp -rf dotfiles/.[!.]* . && rm -rf dotfiles");
        system("cd && git remote set-url origin git@github.com:marcelofern/dotfiles.git");
        system("cd ~ && git checkout .");
    }
    else {
        system("cd ~ && git pull origin main");
    }
    system("nitrogen --save --set-auto ~/wallpapers/mountain.png");
    system("git config --global commit.template ~/.config/git/commit_message_template.md");
}

void sync(void) {
    system("rm -rf arch");
    system("cd /usr/local/bin && sudo mv arch arch.old");
    system("git clone https://github.com/marcelofern/arch.git");
    system("cd arch && make");
    system("rm -rf arch");
}

void update(void)
{
    puts(YEL "updating hardware and system clock via ntpd..." RESET "\n");
    update_system_time();
    puts(GRN "hardware and system clock updated!" RESET "\n");

    puts(YEL "updating mirrors..." RESET "\n");
    system("sudo reflector --latest 10 --sort rate --save /etc/pacman.d/mirrorlist");
    puts(GRN "mirrors updated!" RESET "\n");

    puts(YEL "upgrading all packages..." RESET "\n");
    update_from_yay();
    puts(GRN "all packages updated!" RESET "\n");

    puts(YEL "Removing orphan packages..." RESET "\n");
    system("sudo pacman -Qtdq | sudo pacman -Rns -");
    puts(GRN "Orphan packages removed (if any)." RESET "\n");

}

static void install_zsh_shell(void)
{
    system("chsh -l && chsh -s $(which zsh) $(whoami)");
    // downloading oh my zsh
    system(
        "sh -c \"$(wget -O- "
        "https://raw.githubusercontent.com/"
        "ohmyzsh/ohmyzsh/master/tools/install.sh)\""
    );
}

static void install_dependencies(void)
{
    char *pacman_cmd = 
        "sudo pacman -S --needed --noconfirm "
        // visual desktop
        "xorg-server xorg-xinit xorg-xrandr xorg-xev xorg-xset "
        "i3-gaps i3lock redshift nitrogen xf86-video-intel python-pywal "
        "picom flameshot rofi "
        // package compiling
        "cmake "
        // network
        "ntp "
        // terminal
        "zsh alacritty neofetch htop xclip "
        // linux utility
        "openssh zip unzip which tree "
        // bluetooth
        "bluez bluez-utils pulseaudio-bluetooth "
        // audio
        "pulseaudio alsa-utils sof-firmware "
        // development 
        "base-devel python python-pip git curl docker docker-compose "
        // python2 is a dependency for sass-loader used in nuxtJs
        "python2 "
        // neovim
        "neovim ack ripgrep "
        // rss
        "newsboat "
        // pdf
        "zathura zathura-pdf-poppler "
        // notification system
        "dunst libnotify at "
        // fonts
        "ttf-dejavu "
        // mirrors update
        "reflector "
        // browser
        "firefox "
        // internet connection
        "networkmanager ";
    system(pacman_cmd);

    install_aur_package("yay");
    char *yay_cmd =
        "yay -S --needed --answerdiff=None --answerclean=All "
        "polybar "
        // polybar icons font
        "ttf-font-icons "
        // neovim ctrl+p requirement:
        "ctags-git ";
    system(yay_cmd);

    // neovim plugin manager
    system(
        "curl -fLo ~/.local/share/nvim/site/autoload/plug.vim --create-dirs "
        "https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim"
    );

    char *pip_cmd =
        "sudo pip install --upgrade "
        // dev dependencies
        "pynvim virtualenv ";
    system(pip_cmd);
}

static void initiate_systemctl_services(void)
{
    system("sudo systemctl start bluetooth.service");
    system("sudo systemctl enable bluetooth.service");

    system("sudo systemctl start docker");
    system("sudo systemctl enable docker");

    system("sudo systemctl start atd");
    system("sudo systemctl enable atd");

    system("sudo systemctl enable NetworkManager");
    system("sudo systemctl start NetworkManager");
}

static int binary_exists(char *bin_name) {
    struct cl_it cl_it[1];
    char cmd[256];

    /* 2>&1 redirects stderr to stdout so that we catch errors */
    snprintf(cmd, sizeof(cmd), "which %s 2>&1", bin_name);

    cl_open(cl_it, cmd);
    /* only grabs the first line from the iterator */
    cl_next(cl_it);
    int binary_exists = strstr(cl_it->line, "which: no") ? 0 : 1;
    cl_close(cl_it);
    return binary_exists;
}

static void update_system_time(void)
{
    system("timedatectl set-timezone \"NZ\"");
    system("sudo ntpd -qg");
    system("sudo hwclock --systohc");
}

static void install_aur_package(char *package_name)
{
    // No library has a long enough name to cover this length. So 1028
    // should be more than sufficient allocated space.
    int max_str_size = 1028;
    char clone_cmd[max_str_size];
    char make_cmd[max_str_size];

    snprintf(
        clone_cmd,
        sizeof(clone_cmd),
        "git clone https://aur.archlinux.org/%s.git",
        package_name
    );
    system(clone_cmd);

    snprintf(
        make_cmd,
        sizeof(make_cmd),
        "cd %s && yes | makepkg -sirc && cd .. && rm -rf %s",
        package_name, package_name
    );
    system(make_cmd);
}

static void update_from_yay(void) {
    if (!binary_exists("yay"))
        // print WARNING (installing yay)"
        install_aur_package("yay");
    system("yay -Syu --answerclean=None --answerdiff=None --answerclean=All");
}
