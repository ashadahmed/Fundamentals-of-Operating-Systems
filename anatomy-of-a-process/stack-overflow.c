int adding(int a, int b) {
    int c = adding(a, b);
    return c;
}

int main() {
    int a = 1;
    int b = 2;
    int c = adding(a, b);
    return 0;
}