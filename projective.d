Obj.o: Obj.cpp Obj.h gg.h
gg.o: gg.cpp gg.h
main.o: main.cpp gg.h Obj.h matrix.h
matrix.o: matrix.cpp matrix.h gg.h
