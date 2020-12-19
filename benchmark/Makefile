all: cast

cast: cast.c
	clang -o $@ -O2 $<

cast-native: cast.c
	clang -o $@ -O2 -march=native $<

cast-armv8.5: cast.c
	clang -arch arm64 -o $@ -O2 -march=armv8.5-a -DUSE_FRINTTS $<

cast-x86_64: cast.c
	clang -arch x86_64 -o $@ -O2 $<

.PHONY: all
