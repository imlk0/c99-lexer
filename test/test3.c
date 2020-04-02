int main() {
    int i = 0x77777777;
    int i = 0x77777777l;
    int i = -123; /* sign will not be a part of nunber */
    int i = 111;
    int i = 111l;
    int i = 111ll;
    int i = 111u;
    int i = 111ul;
    int i = 111ull;
    int i = 0111;
    int i = 0111l;

    float f = 1.0;
    float f = 1.0f;
    float f = 1.0l;
    float f = 1.0e2;    
    float f = 1.0e+2;    
    float f = 1.0e-2;

    char c = ''; /* invalid */
    char c = '''; /* invalid */
    char c = '\''; /* valid */
    char c = '"'; /* valid */
    char c = 'c';
    char c = 'cccc'; /* valid in c99 */
    char c = '\r';
    char c = '\n';
    char c = '\t';
    char c = '\\';
    char c = '\0';
    char c = '\00';
    char c = '\000';
    char c = '\123';
    char c = '\xA';
    char c = '\xAA';
    char c = '\xAAA';

    char* c = ""; /* valid */
    char* c = "'"; /* valid */
    char* c = """; /* invalid */
    char* c = "\""; /* valid */
    char* c = "c";
    char* c = "cccc"; /* valid in c99 */
    char* c = "\r";
    char* c = "\n";
    char* c = "\t";
    char* c = "\\";
    char* c = "\0";
    char* c = "\00";
    char* c = "\000";
    char* c = "\123";
    char* c = "\xA";
    char* c = "\xAA";
    char* c = "\xAAA\xAAA\xAAA\xAAA\xAAA";
}