struct hfa31 {
    long double a;
} hfa31 = {31.1};
union {
    int b1;
};
typedef struct {
    int a;
} s;

enum fred { a, b, c, d, e = 54, f = 73, g, h };

static unsigned int deref_uintptr(unsigned int *p) { return *p; }

struct hfa31 {
    double a;
    long double b;
    float c;
    int d;
    long e;
    long long f;
    char g;
    short h;
    unsigned int i;
    unsigned long j;
    unsigned long long k;
    unsigned char l;
    unsigned short m;
};

int main() {
    if (1) {
    } else {
    }
    while (1) {
        continue;
    }
    for (int i = 0; i < 0; ++i) {
    }
    do {
    } while (1);
    switch (1) {
    case 1:
        break;
    default:
        break;
    }
    return 0;
}

/* vim: set expandtab ts=4 sw=3 sts=3 tw=80 :*/
