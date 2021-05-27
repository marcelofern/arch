#include <stdio.h>
#include <stdlib.h>
#include "arch.h"

static void install_zsh_shell(void);
static void initiate_systemctl_services(void);
static void install_dependencies(void);

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
        "wget python2 "
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
