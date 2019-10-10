char* encrypt(int table[], char buf[], int len)
{
    int j = 0;
    char *end = buf + len;
    while (buf < end) {
        *buf = (char)table[(unsigned char)*buf];
        buf++;
    }

    return buf - len;
}
