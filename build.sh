
project="swordfish"

inc_raylib="-Lraylib/src/ -lraylib -Iraylib/src"
inc_cvecs="external/cvecs.c -Iexternal"

execute() {
    echo $1
    eval $1
}

red_echo() {
    local text=$1
    echo -e "\e[31m$text\e[0m"
}

green_echo() {
    local text=$1
    echo -e "\e[32m$text\e[0m"
}

yellow_echo() {
    local text=$1
    echo -e "\e[33m$text\e[0m"
}

echo -n "Compiling all files for project "
yellow_echo "$project"

######### Compiling Raylib #########
    cd raylib/src/ 
    echo -n "Compiling "
    yellow_echo "Raylib"
    execute "make PLATFORM=PLATFORM_DESKTOP -j"
    error_code=$?
    echo -n "Compiling Raylib "
    if [ $error_code -eq 0 ]; then
        green_echo "finished"
    else
        red_echo "failed"
    fi
    cd ../..
####################################

########## Compiling Binary ##########
    echo -n "Compiling "
    yellow_echo "$project"
######### Add Source Files #########
    execute "gcc -o $project -Wextra -Wall -pedantic -g -Iinclude \
        main.c \
        $inc_raylib $inc_cvecs -lm"
###################################
    error_code=$?
    echo -n "Compiling "
    if [ $error_code -eq 0 ]; then
        green_echo "finished" 
    else
        red_echo "failed" 
    fi
