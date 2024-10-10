#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

struct termios orig_termios;

void disableRawMode() {
    // This will be used to revert to normal terminal mode from RAW mode.
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {

    /*Turn off Echoing - By default the terminal will show what characters are being input.
    We want to turn this off when creating a text editor.
    struct termios, tcgetattr(), tcsetattr(), ECHO, TCSAFLUSH all come from <termios.h>*/

    // read the current attributes of the terminal into raw
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    
    // declare and initialize a struct called raw
    struct termios raw = orig_termios;

    

    // modify the struct to exclude ECHO
    // c_lflag is for "local flags" or "miscellaneous flags". 
    // Other flag fields are c_iflag (input flag), c_oflag (output flag) and c_cflag (control flag)
    // ECHO is a bitflag. 
    raw.c_lflag &= ~(ECHO); 

    // pass the modified struct to tcsetattr()
    // TCSAFLUSH specifies when to apply the change: in this case, it waits for all pending output to be written to terminal
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(){
    enableRawMode();

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
    return 0;
}