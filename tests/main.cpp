


int main() {
    long double array[500];
    array[499] = 40.52f;

    for (int i = 5000; i >= 0; i--) {
        array[i % 500] = array[i % 500 + 1] * 2;
    }
    return 0;
}