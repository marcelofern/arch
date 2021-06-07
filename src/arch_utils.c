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
    printf(YEL "Updating hardware and system clock via ntpd..." RESET "\n");
    update_system_time();
    printf(GRN "Hardware and system clock updated!" RESET "\n");

    printf(YEL "Installing dependencies..." RESET "\n");
    install_dependencies();
    printf(GRN "Dependencies installed..." RESET "\n");

    printf(YEL "Initiating systemctl services..." RESET "\n");
    initiate_systemctl_services();
    printf(GRN "systemctl services initiated..." RESET "\n");

    printf(YEL "Setting gruvbox color schema..." RESET "\n");
    system("wal --theme base16-gruvbox-soft");
    printf(GRN "Gruvbox was set..." RESET "\n");

    printf(YEL "Generating .ssh/ folder" RESET "\n");
    system("ssh-keygen");
    printf(GRN ".ssh/ folder was generated..." RESET "\n");

    printf(YEL "Setting up zsh as shell" RESET "\n");
    install_zsh_shell();
    printf(GRN "Zsh has been installed!" RESET "\n");

    printf(GRN "You are all set up now. reboot your PC!" RESET "\n");
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
    system("nitrogen --save --set-auto ~/Wallpapers/mountain-png.png");
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
    printf(YEL "updating hardware and system clock via ntpd..." RESET "\n");
    update_system_time();
    printf(GRN "hardware and system clock updated!" RESET "\n");

    printf(YEL "updating mirrors..." RESET "\n");
    update_system_time();
    system("sudo reflector --latest 10 --sort rate --save /etc/pacman.d/mirrorlist");
    printf(GRN "mirrors updated!" RESET "\n");

    printf(YEL "upgrading all packages..." RESET "\n");
    update_from_yay();
    printf(GRN "all packages updated!" RESET "\n");
}

static void install_zsh_shell(void)
{
    system("chsh -l && chsh -s $(which zsh) $(whoami)");
    // downloading oh my zsh
    system(
        "sh -c \"$(wget -O- "
        "https://raw.githubusercontent.com/"
        "ohmyzsh/ohmyzsh/master/tools/install.sh)"
    );
}

static void install_dependencies(void)
{
    char *pacman_cmd = 
        "sudo pacman -S --needed --noconfirm "
        // common
        "base-devel "
        // visual desktop
        "xorg-server xorg-xinit xorg-xrandr xorg-xev xorg-xset "
        "i3-gaps i3lock redshift nitrogen xf86-video-intel python-pywal "
        "picom "
        // desktop utility
        "flameshot peek rofi dmenu "
        // terminal
        "zsh termite neofetch tmux htop xclip "
        // linux utility
        "openssh zip unzip which tree "
        // bluetooth
        "bluez bluez-utils pulseaudio-bluetooth "
        // audio
        "pulseaudio alsa-utils "
        // development 
        "python python-pip git curl docker docker-compose "
        "wget "
        // python2 is a dependency for sass-loader used in nuxtJs
        "python2 "
        // neovim
        "neovim ack ripgrep "
        // pdf 
        "zathura zathura-pdf-poppler "
        // notification system
        "dunst libnotify at "
        // email
        "neomutt "
        // fonts
        "ttf-dejavu noto-fonts noto-fonts-emoji "
        // mirrors update
        "reflector "
        // rss
        "newsboat "
        // usb
        "udisks2 "
        // browser
        "firefox "
        // webcam
        "v4l-utils "
        // dictionary
        "dictd "
        // internet connection
        "networkmanager "
        // C documentation generator
        "doxygen "
        // GNU C debugger
        "gdb ";
    system(pacman_cmd);

    char *pip_cmd =
        "sudo pip install --upgrade "
        // dev dependencies
        "pynvim virtualenv ";
    system(pip_cmd);

    install_aur_package("yay");
    char *yay_cmd =
        "yay -S --needed --answerdiff=None --answerclean=All "
        "polybar "
        // icons for polybar
        "ttf-font-icons "
        // neovim ctrl+p requirement:
        "ctags-git "
        // neovim auto completer (commented out, use init.vim instead!)
        // "vim-youcompleteme-git "
        // C gibberish to english:
        "cdecl ";
    system(yay_cmd);

    // neovim plugin manager
    system(
        "curl -fLo ~/.local/share/nvim/site/autoload/plug.vim --create-dirs "
        "https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim"
    );
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
    char *cmd = NULL;

    /* 2>&1 redirects stderr to stdout so that we catch errors */
    sprintf(cmd, "which %s 2>&1", bin_name);

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
    char *clone_cmd = NULL;
    char *make_cmd = NULL;

    sprintf(
        clone_cmd,
        "git clone https://aur.archlinux.org/%s.git",
        package_name
    );
    system(clone_cmd);

    sprintf(
        make_cmd,
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