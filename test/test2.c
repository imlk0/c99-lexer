int main() {
    int char1 = 0;   /* valid*/
    int 1yes = 1;    /* not a identifier */
    int 1y + es = 1; /* not a identifier */
    int 1ye+es = 1;  /* not a identifier */
    int ____asd_12113_131123kkk = 1; /* valid */
    int asd\u0040kkkkk = 1; /* valid in c99*/
}

void funcname() {} /* valid */
void 000kkk() {} /* invalid */
void _k_+kk() {} /* invalid */
void kk_+--funcname() {} /* invalid */