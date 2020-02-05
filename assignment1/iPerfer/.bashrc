
# see /usr/share/doc/bash/examples/startup-files (in the package bash-doc)
# for examples

# If not running interactively, don't do anything
case $- in
    *i*) ;;
      *) return;;
esac

# don't put duplicate lines or lines starting with space in the history.
# See bash(1) for more options
HISTCONTROL=ignoreboth

# append to the history file, don't overwrite it
shopt -s histappend

# for setting history length see HISTSIZE and HISTFILESIZE in bash(1)
HISTSIZE=1000
HISTFILESIZE=2000

# check the window size after each command and, if necessary,
# update the values of LINES and COLUMNS.
shopt -s checkwinsize

# If set, the pattern "**" used in a pathname expansion context will
# match all files and zero or more directories and subdirectories.
#shopt -s globstar

# make less more friendly for non-text input files, see lesspipe(1)
[ -x /usr/bin/lesspipe ] && eval "$(SHELL=/bin/sh lesspipe)"

# set variable identifying the chroot you work in (used in the prompt below)
if [ -z "${debian_chroot:-}" ] && [ -r /etc/debian_chroot ]; then
    debian_chroot=$(cat /etc/debian_chroot)
fi

# set a fancy prompt (non-color, unless we know we "want" color)
case "$TERM" in
    xterm-color) color_prompt=yes;;
esac

# uncomment for a colored prompt, if the terminal has the capability; turned
# off by default to not distract the user: the focus in a terminal window
# should be on the output of commands, not on the prompt
force_color_prompt=yes

if [ -n "$force_color_prompt" ]; then
    if [ -x /usr/bin/tput ] && tput setaf 1 >&/dev/null; then
	# We have color support; assume it's compliant with Ecma-48
	# (ISO/IEC-6429). (Lack of such support is extremely rare, and such
	# a case would tend to support setf rather than setaf.)
	color_prompt=yes
    else
	color_prompt=
    fi
fi

if [ "$color_prompt" = yes ]; then
    PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:$ '
else
    PS1='${debian_chroot:+($debian_chroot)}\u@\h:\w\$ '
fi
unset color_prompt force_color_prompt

# If this is an xterm set the title to user@host:dir
case "$TERM" in
xterm*|rxvt*)
    PS1="\[\e]0;${debian_chroot:+($debian_chroot)}\u@\h: \w\a\]$PS1"
    ;;
*)
    ;;
esac

# enable color support of ls and also add handy aliases
if [ -x /usr/bin/dircolors ]; then
    test -r ~/.dircolors && eval "$(dircolors -b ~/.dircolors)" || eval "$(dircolors -b)"
    alias ls='ls --color=auto'
    #alias dir='dir --color=auto'
    #alias vdir='vdir --color=auto'

    alias grep='grep --color=auto'
    alias fgrep='fgrep --color=auto'
    alias egrep='egrep --color=auto'
fi

# some more ls aliases
alias ll='ls -alF'
alias la='ls -A'
alias l='ls -CF'

# Add an "alert" alias for long running commands.  Use like so:
#   sleep 10; alert
alias alert='notify-send --urgency=low -i "$([ $? = 0 ] && echo terminal || echo error)" "$(history|tail -n1|sed -e '\''s/^\s*[0-9]\+\s*//;s/[;&|]\s*alert$//'\'')"'

# Alias definitions.
# You may want to put all your additions into a separate file like
# ~/.bash_aliases, instead of adding them here directly.
# See /usr/share/doc/bash-doc/examples in the bash-doc package.

if [ -f ~/.bash_aliases ]; then
    . ~/.bash_aliases
fi

# enable programmable completion features (you don't need to enable
# this, if it's already enabled in /etc/bash.bashrc and /etc/profile
# sources /etc/bash.bashrc).
if ! shopt -oq posix; then
  if [ -f /usr/share/bash-completion/bash_completion ]; then
    . /usr/share/bash-completion/bash_completion
  elif [ -f /etc/bash_completion ]; then
    . /etc/bash_completion
  fi
fi

#================================================================================================
#================================================================================================


cat << "EOF"

  Let's gooooooo!                   _._
                               _.-="_-         _
                          _.-="   _-          | ||"""""""---._______     __..
              ___.===""""-.______-,,,,,,,,,,,,`-''----" """""       """""  __'
       __.--""     __        ,'                   o \           __        [__|
  __-""=======.--""  ""--.=================================.--""  ""--.=======:
 ]       [w] : /        \ : |========================|    : /        \ :  [w] :
 V___________:|          |: |========================|    :|          |:   _-"
  V__________: \        / :_|=======================/_____: \        / :__-"
  -----------'  ""____""  `-------------------------------'  ""____""

EOF

echo -e "\033[1;36mSystem Start...\033[m"
echo -e "\033[1;33m[\033[1;31m----------\033[1;33m]\033[0;37mPrompt...\033[m"

#git in prompt

CYAN='\033[1;36m\]'
YELLOW='\033[1;33m\]'


#Ϣ  ༤ ༄   ༻   ཧ ⚔  ⚝  ⚛  ⚠  ⛩  
PS1="\033[1;36m\u⚔ \033[1;32m@\h⛩ \033[m \033[1;33m\w  \nϢ  \[\033[m\]"
PS2="\033[1;33m༄  \[\033[m\]"

PROMPT_COMMAND='__posh_git_ps1 "\033[1;36m\u⚔ \033[1;32m@\h⛩ \033[m" " \033[1;33m\w \nϢ  \[\033[m\]";'$PROMPT_COMMAND
source /home/ruizhe/Desktop/code_assembly/eecs-470-group4/backroom/Hunter_cat_bash_stuff/posh-git-sh/git-prompt.sh

echo -e "\033[1;33m[\033[1;32m===\033[1;31m-------\033[1;33m]\033[0;37malias...\033[m"

alias gk="gitk --all&"
alias sb="source ~/.bashrc"
alias g="git"
alias g+++="g++ -std=c++14 -Wall -Werror -Wvla -Wextra -pedantic -O3"
alias g+Depend="g++ -MM"
alias cppch="cppcheck --enable=all --suppress=missingIncludeSystem *.c* *.h*"
alias pdf="evince"
alias ccs="/opt/ti/ccsv7/eclipse/ccstudio &"
alias arduino="sudo ~/L2/arduino-1.8.5/arduino"
cgit="/mnt/c/classGit"
com="/mnt/c/Command_Hub"
wind="/mnt/c/Users/Hunter_Pfeiffer"
caenAddress="login-course.engin.umich.edu"

export EDITOR='vim'

echo -e "\033[1;33m[\033[1;32m=====\033[1;31m-----\033[1;33m]\033[0;37mEdit_* Functions...\033[m"

function Edit_bashrc
{
	vim ~/.bashrc
}

function Edit_vimrc
{
	vim ~/_vimrc
}

function Edit_git
{
	vim ~/.gitconfig
}

caen ()
{
	ssh $1@login-course.engin.umich.edu
}

function updateConfigs
{
	cp -f track_gitconfig .gitconfig
	cp -f trak_vimrc _vimrc
}
function Edit_i3
{
	vim ~/.i3/config
}

echo -e "\033[1;33m[\033[1;32m=======\033[1;31m---\033[1;33m]\033[0;37mmove up Functions...\033[m"

function ..
{
	cd ..
}

function ...
{
	cd ../..
}

function ....
{
	cd ../../..
}

echo -e "\033[1;33m[\033[1;32m========\033[1;31m--\033[1;33m]\033[0;37mutil Functions....\033[m"
caen ()
{
	ssh $1@login-course.engin.umich.edu
}

caenSend ()
{
	scp $1 $2@$caenAddress:loadingBay
}

caenGet ()
{
	scp $1@$caenAddress:$2 .
}

unload ()
{
	cp ~/loadingBay/* .
}

clearBay ()
{
	rm ~/loadingBay/*
}

function initGit
{
	git init
	git ci --allow-empty -m "INIT COMMIT"
}

function setwall ()
{
	gsettings set org.gnome.desktop.background picture-uri file://$1
}

function convertToPDF ()
{
	enscript -p tempPDFconvert.ps $1
	ps2pdf tempPDFconvert.ps $2
	rm tempPDFconvert.ps
}

function setBL(){
	sudo echo $1 | sudo tee /sys/class/backlight/intel_backlight/brightness
}
echo -e "\033[1;33m[\033[1;32m==========\033[1;33m]\033[1;36mWELCOME BACK COMMANDER\033[m"
echo
