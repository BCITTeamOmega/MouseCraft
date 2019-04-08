#pragma once

union UIntChar {
    unsigned int i;
    unsigned char c[4];
};

union IntChar {
    int i;
    unsigned char c[4];
};

union UShortChar {
    unsigned short s;
    unsigned char c[2];
};

union ShortChar {
    short s;
    unsigned char c[2];
};

union FloatChar {
    float f;
    unsigned char c[4];
};

union BoolChar {
    bool b;
    unsigned char c[1];
};