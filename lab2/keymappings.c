char mapCharacter(int keycode, int shift) {
    switch (keycode) {
        case 0x04:
            return !shift ? 'a' : 'A';
        case 0x05:
            return !shift ? 'b' : 'B';
        case 0x06:
            return !shift ? 'c' : 'C';
        case 0x07:
            return !shift ? 'd' : 'D';
        case 0x08:
            return !shift ? 'e' : 'E';
        case 0x09:
            return !shift ? 'f' : 'F';
        case 0x0A:
            return !shift ? 'g' : 'G';
        case 0x0B:
            return !shift ? 'h' : 'H';
        case 0x0C:
            return !shift ? 'i' : 'I';
        case 0x0D:
            return !shift ? 'j' : 'J';
        case 0x0E:
            return !shift ? 'k' : 'K';
        case 0x0F:
            return !shift ? 'l' : 'L';
        case 0x10:
            return !shift ? 'm' : 'M';
        case 0x11:
            return !shift ? 'n' : 'N';
        case 0x12:
            return !shift ? 'o' : 'O';
        case 0x13:
            return !shift ? 'p' : 'P';
        case 0x14:
            return !shift ? 'q' : 'Q';
        case 0x15:
            return !shift ? 'r' : 'R';
        case 0x16:
            return !shift ? 's' : 'S';
        case 0x17:
            return !shift ? 't' : 'T';
        case 0x18:
            return !shift ? 'u' : 'U';
        case 0x19:
            return !shift ? 'v' : 'V';
        case 0x1a:
            return !shift ? 'w' : 'W';
        case 0x1b:
            return !shift ? 'x' : 'X';
        case 0x1c:
            return !shift ? 'y' : 'Y';
        case 0x1d:
            return !shift ? 'z' : 'Z';
        case 0x1e:
            return !shift ? '1' : '!';
        case 0x1f:
            return !shift ? '2' : '@';
        case 0x20:
            return !shift ? '3' : '#';
        case 0x21:
            return !shift ? '4' : '$';
        case 0x22:
            return !shift ? '5' : '%';
        case 0x23:
            return !shift ? '6' : '^';
        case 0x24:
            return !shift ? '7' : '&';
        case 0x25:
            return !shift ? '8' : '*';
        case 0x26:
            return !shift ? '9' : '(';
        case 0x27:
            return !shift ? '0' : ')';
        case 0x2d:
            return !shift ? '-' : '_'; 
        case 0x2e:
            return !shift ? '=' : '+';
        case 0x2f:
            return !shift ? '[' : '{';
        case 0x30:
            return !shift ? ']' : '}';
        case 0x31:
            return !shift ? '\\' : '|';
        case 0x33:
            return !shift ? ';' : ':';
        case 0x34:
            return !shift ? '\'' : '"';
        case 0x36:
            return !shift ? ',' : '<';
        case 0x37:
            return !shift ? '.' : '>';
        case 0x38:
            return !shift ? '/' : '?'; 
    }

}

